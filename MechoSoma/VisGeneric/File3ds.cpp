#include "File3ds.h"

#include <algorithm>
#include <filesystem>
#include <math.h>
#include <memory>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include <vector>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include "filesystem.h"
#include "BaseDefine.h"
#include "Math3d.h"

namespace
{

const std::string MaterialExtension = "3DS_material";
const std::string AnimationExtension = "3DS_animation";
const std::string DummyExtension = "3DS_dummy";
const std::string DummyName = "$$$DUMMY";

std::string to_lower(const std::string &input)
{
	std::string result;
	std::transform(input.begin(), input.end(), std::back_inserter(result), [](auto c) { return std::tolower(c); });
	return result;
}

std::string create_node_name(const char *file_name)
{
	const auto normalized = localization::getLocalizedFile(file_name);

	std::stringstream result;
	std::string part;
	for (auto c : normalized)
	{
		if (c == '\\' || c == '/')
		{
			if (!part.empty())
			{
				result << "_" << to_lower(part);
			}
			part.clear();
		}
		else
		{
			part.push_back(c);
		}
	}

	if (!part.empty())
	{
		result << "_" << to_lower(part);
	}

	return result.str().substr(1);
}

struct ModelStore
{
	ModelStore()
	{
		tinygltf::TinyGLTF loader;
		std::string error;
		std::string warning;
		const bool result = loader.LoadBinaryFromFile(&model, &error, &warning, "RESOURCE/3ds.gltf");
		if (!warning.empty())
		{
			std::cout << "WARNING: " << warning << std::endl;
		}

		if (!error.empty())
		{
			std::cout << "ERROR: " << error << std::endl;
		}

		if (!result)
		{
			ErrH.Abort("LoadBinaryFromFile", XERR_USER, 0, "");
		}

		assert(model.scenes.size() == 1);
		for (auto index : model.scenes[0].nodes)
		{
			const auto &node = model.nodes[index];
			assert(node_index.find(node.name) == node_index.end());
			node_index.emplace(node.name, &node);
		}
	}

	void SetParentNode(const char *file_name)
	{
		const auto node_name = create_node_name(file_name);
		const auto node = node_index.find(node_name);
		if (node == node_index.end())
		{
			ErrH.Abort("Node not found", XERR_USER, 0, "");
		}
		parent_node = node->second;

		dummies.clear();
		children.clear();
		for (auto index : parent_node->children)
		{
			const auto &node = model.nodes[index];
			if (node.name == DummyName)
			{
				dummies.push_back(index);
			}
			else
			{
				children.push_back(index);
			}
		}
	}

	void Reset()
	{
		parent_node = nullptr;
		current_node = nullptr;
		current_mesh = nullptr;
		current_dummy = nullptr;
		current_animation = nullptr;
	}

	tinygltf::Model model;
	std::unordered_map<std::string_view, const tinygltf::Node *> node_index;
	const tinygltf::Node *parent_node = nullptr;
	std::vector<int> children;
	std::vector<int> dummies;
	const tinygltf::Node *current_node = nullptr;
	const tinygltf::Mesh *current_mesh = nullptr;
	const tinygltf::Node *current_dummy = nullptr;
	const tinygltf::Animation *current_animation = nullptr;

};
std::unique_ptr<ModelStore> model_store;

}

cFile3ds::cFile3ds() : Scale(1.0), CountDummy(0) {}

cFile3ds::~cFile3ds()
{
	Close();
}

char cFile3ds::Open(const char *FileName)
{
	if (!model_store)
	{
		model_store = std::make_unique<ModelStore>();
	}

	model_store->SetParentNode(FileName);
	return 1;
}

void cFile3ds::Close()
{
	model_store->Reset();
}

//******************************** MESH **********************************
int cFile3ds::GetMeshCount()
{
	assert(model_store->parent_node != nullptr);
	return static_cast<int>(model_store->children.size());
}

void cFile3ds::GetMeshParameters(int nObject, int *nPoint, int *nFace, char *ObjectName)
{
	assert(model_store->parent_node != nullptr);
	assert(nObject < static_cast<int>(model_store->children.size()));

	const auto node_index = model_store->children[nObject];
	const auto &node = model_store->model.nodes[node_index];
	assert(node.mesh != -1);
	const auto &mesh = model_store->model.meshes[node.mesh];

	assert(mesh.primitives.size() == 1);
	const auto &primitive = mesh.primitives[0];

	{
		const auto index = primitive.attributes.at("POSITION");
		const auto &accessor = model_store->model.accessors[index];
		*nPoint = static_cast<int>(accessor.count);
	}

	const auto &accessor = model_store->model.accessors[primitive.indices];
	assert(accessor.count % 3 == 0);
	*nFace = static_cast<int>(accessor.count / 3);

	std::fill(ObjectName, ObjectName + ObjectNameSize, 0);
	assert(node.name.size() < ObjectNameSize);
	node.name.copy(ObjectName, node.name.size());

	auto extras = node.extras.Get<tinygltf::Value::Object>();
	auto animation_index = extras.find("animation_index");
	if (animation_index != extras.end())
	{
		const auto &animation = model_store->model.animations[animation_index->second.Get<int>()];
		model_store->current_animation = &animation;
	}

	model_store->current_node = &node;
	model_store->current_mesh = &mesh;
}

float cFile3ds::MaxSizeMesh(int nObject, float NewSize)
{
	float max = 0;
	for (int i = 0; i < nObject; i++)
	{
		const auto node_index = model_store->children[i];
		const auto &node = model_store->model.nodes[node_index];
		const auto &mesh = model_store->model.meshes[node.mesh];

		assert(mesh.primitives.size() == 1);
		const auto &primitive = mesh.primitives[0];
		const auto index = primitive.attributes.at("POSITION");
		const auto &accessor = model_store->model.accessors[index];

		assert(accessor.maxValues.size() == 3);
		for (auto v : accessor.maxValues)
		{
			max = std::max(max, static_cast<float>(abs(v)));
		}

		assert(accessor.minValues.size() == 3);
		for (auto v : accessor.minValues)
		{
			max = std::max(max, static_cast<float>(abs(v)));
		}		
	}

	Scale = NewSize / max;
	return max;
}

void cFile3ds::ReadMesh(float *PointXYZUV, int *FaceP1P2P3, float *Matrix)
{
	assert(model_store->current_node != nullptr);
	assert(model_store->current_mesh != nullptr);

	const auto &buffer = model_store->model.buffers[0];
	const auto &primitive = model_store->current_mesh->primitives[0];

	{
		const auto &accessor = model_store->model.accessors[primitive.indices];
		const auto &view = model_store->model.bufferViews[accessor.bufferView];
		auto data = reinterpret_cast<const uint16_t *>(buffer.data.data() + view.byteOffset);

		for (size_t i = 0; i < accessor.count; i++)
		{
			FaceP1P2P3[i] = data[i];
		}
	}

	const auto &accessor = model_store->model.accessors[primitive.attributes.at("POSITION")];
	const float *position_data = nullptr;
	const float *uv_data = nullptr;

	{
		const auto &view = model_store->model.bufferViews[accessor.bufferView];
		position_data = reinterpret_cast<const float *>(buffer.data.data() + view.byteOffset);
	}

	{
		const auto p = primitive.attributes.find("TEXCOORD_0");
		if (p != primitive.attributes.end())
		{
			const auto &accessor = model_store->model.accessors[p->second];
			const auto &view = model_store->model.bufferViews[accessor.bufferView];
			uv_data = reinterpret_cast<const float *>(buffer.data.data() + view.byteOffset);
		}
	}

	size_t position_cursor = 0;
	size_t uv_cursor = 0;
	for (size_t i = 0; i < accessor.count; i++)
	{
		PointXYZUV[5*i + 0] = position_data[position_cursor++] * Scale;
		PointXYZUV[5*i + 1] = position_data[position_cursor++] * Scale;
		PointXYZUV[5*i + 2] = position_data[position_cursor++] * Scale;

		if (uv_data != nullptr)
		{
			PointXYZUV[5*i + 3] = uv_data[uv_cursor++];
			PointXYZUV[5*i + 4] = uv_data[uv_cursor++];
		}
	}

	auto &m = model_store->current_node->matrix;
	assert(m.size() == 16);
	size_t matrix_cursor = 0;
	for (size_t i = 0; i < 12; i++)
	{
		if ((i + 1) % 4 != 0)
		{
			Matrix[matrix_cursor++] = m[i];
		}
	}
	for (size_t i = 12; i < 15; i++)
	{
		Matrix[matrix_cursor++] = m[i] * Scale;
	}
}

void cFile3ds::ReadMaterial(char *TextureName, char *OpacityName, float *Transparency, float *ShinStrength, float *Shininess, char *Shading, float *rDiffuse, float *gDiffuse, float *bDiffuse)
{
	assert(model_store->current_mesh != nullptr);

	const auto &primitive = model_store->current_mesh->primitives[0];
	if (primitive.material == -1)
	{
		ErrH.Abort("Bad material");
	}

	const auto &material = model_store->model.materials[primitive.material];
	const auto &extension = material.extensions.at(MaterialExtension).Get<tinygltf::Value::Object>();

	*Transparency = static_cast<float>(extension.at("transparency").Get<double>());
	*ShinStrength = static_cast<float>(extension.at("shininess_strength").Get<double>());
	*Shininess = static_cast<float>(extension.at("shininess").Get<double>());
	*Shading = static_cast<char>(extension.at("shading").Get<int>());

	const auto &diffuse = extension.at("diffuse").Get<tinygltf::Value::Array>();
	assert(diffuse.size() == 3);
	*rDiffuse = static_cast<float>(diffuse[0].Get<double>());
	*gDiffuse = static_cast<float>(diffuse[1].Get<double>());
	*bDiffuse = static_cast<float>(diffuse[2].Get<double>());

	std::fill(TextureName, TextureName + TextureNameSize, 0);
	auto texture_map_name = extension.find("texture_map_name");
	if (texture_map_name != extension.end())
	{
		const auto &s = texture_map_name->second.Get<std::string>();
		assert(s.size() < TextureNameSize);
		s.copy(TextureName, s.size());
	}

	std::fill(OpacityName, OpacityName + OpacityNameSize, 0);
	auto opacity_map_name = extension.find("opacity_map_name");
	if (opacity_map_name != extension.end())
	{
		const auto &s = opacity_map_name->second.Get<std::string>();
		assert(s.size() < OpacityNameSize);
		s.copy(OpacityName, s.size());
	}
}

void cFile3ds::ReadKeyFrame(char *ParentName, float **Pos, int *NumberPos, float **Rot, int *NumberRot, float **Scale, int *NumberScale, float *Pivot)
{
	assert(model_store->current_animation != nullptr);
	const auto &extension = model_store->current_animation->extensions.at(AnimationExtension).Get<tinygltf::Value::Object>();

	const auto &parent = extension.at("parent").Get<std::string>();
	assert(parent.size() < ParentNameSize);
	std::fill(ParentName, ParentName + ParentNameSize, 0);
	parent.copy(ParentName, parent.size());

	const auto &pivot_array = extension.at("pivot").Get<tinygltf::Value::Array>();
	assert(pivot_array.size() == 3);
	Pivot[0] = static_cast<float>(pivot_array[0].Get<double>()) * cFile3ds::Scale;
	Pivot[1] = static_cast<float>(pivot_array[1].Get<double>()) * cFile3ds::Scale;
	Pivot[2] = static_cast<float>(pivot_array[2].Get<double>()) * cFile3ds::Scale;

	*NumberPos = 0;
	*Pos = nullptr;

	*NumberRot = 0;
	*Rot = nullptr;

	*NumberScale = 0;
	*Scale = nullptr;

	for (const auto &channel : model_store->current_animation->channels)
	{
		const auto &sampler = model_store->current_animation->samplers[channel.sampler];

		const float *time_data = nullptr;
		size_t count = 0;
		{
			const auto &accessor = model_store->model.accessors[sampler.input];
			count = accessor.count;
			const auto &view = model_store->model.bufferViews[accessor.bufferView];
			const auto &buffer = model_store->model.buffers[0];
			time_data = reinterpret_cast<const float *>(buffer.data.data() + view.byteOffset);
		}

		if (count == 0)
		{
			continue;
		}

		const float *value_data = nullptr;
		{
			const auto &accessor = model_store->model.accessors[sampler.output];
			assert(count == accessor.count);
			const auto &view = model_store->model.bufferViews[accessor.bufferView];
			const auto &buffer = model_store->model.buffers[0];
			value_data = reinterpret_cast<const float *>(buffer.data.data() + view.byteOffset);
		}

		if (channel.target_path == "translation")
		{
			*NumberPos = static_cast<int>(count);
			(*Pos) = new float[4 * count];
			size_t cursor = 0;
			size_t value_cursor = 0;
			for (size_t i = 0; i < count; i++)
			{
				(*Pos)[cursor++] = time_data[i];
				(*Pos)[cursor++] = value_data[value_cursor++] * cFile3ds::Scale;
				(*Pos)[cursor++] = value_data[value_cursor++] * cFile3ds::Scale;
				(*Pos)[cursor++] = value_data[value_cursor++] * cFile3ds::Scale;
			}
		}
		else if (channel.target_path == "rotation")
		{
			*NumberRot = static_cast<int>(count);
			(*Rot) = new float[5 * count];
			size_t cursor = 0;
			size_t value_cursor = 0;
			for (size_t i = 0; i < count; i++)
			{
				(*Rot)[cursor++] = time_data[i];

				const float x = value_data[value_cursor++];
				const float y = value_data[value_cursor++];
				const float z = value_data[value_cursor++];
				const float w = value_data[value_cursor++];

				(*Rot)[cursor++] = w;
				(*Rot)[cursor++] = x;
				(*Rot)[cursor++] = y;
				(*Rot)[cursor++] = z;
			}
		}
		else if (channel.target_path == "scale")
		{
			*NumberScale = static_cast<int>(count);
			(*Scale) = new float[4 * count];
			size_t cursor = 0;
			size_t value_cursor = 0;
			for (size_t i = 0; i < count; i++)
			{
				(*Scale)[cursor++] = time_data[i];
				(*Scale)[cursor++] = value_data[value_cursor++];
				(*Scale)[cursor++] = value_data[value_cursor++];
				(*Scale)[cursor++] = value_data[value_cursor++];
			}
		}
	}
}

int cFile3ds::GetDummyCount()
{
	return static_cast<int>(model_store->dummies.size());
}

const char* cFile3ds::OpenDummy(int NumberKeyFrame)
{
	if (model_store->dummies.empty() || CountDummy == model_store->dummies.size())
	{
		ErrH.Abort("Dummy not found", XERR_USER, 0, "");
	}

	auto index = model_store->dummies[CountDummy++];
	const auto &node = model_store->model.nodes[index];
	model_store->current_dummy = &node;
	const auto &extension = node.extensions.at(DummyExtension).Get<tinygltf::Value::Object>();
	const auto &instance = extension.at("instance").Get<std::string>();
	return instance.c_str();
}

const char* cFile3ds::GetDummyParent()
{
	assert(model_store->current_dummy != nullptr);

	const auto &extension = model_store->current_dummy->extensions.at(DummyExtension).Get<tinygltf::Value::Object>();
	const auto &parent = extension.at("parent").Get<std::string>();
	return parent.c_str();
}

void cFile3ds::ReadDummy(float *x, float *y, float *z)
{
	assert(model_store->current_dummy != nullptr);

	const auto &translation = model_store->current_dummy->translation;
	assert(translation.size() == 3);

	const auto &rotation = model_store->current_dummy->rotation;
	const auto &scale = model_store->current_dummy->scale;

	if (rotation.empty() && scale.empty())
	{
		*x = translation[0] * Scale;
		*y = translation[1] * Scale;
		*z = translation[2] * Scale;
	}
	else
	{
		assert(rotation.size() == 4);
		assert(scale.size() == 3);

		const float tx = translation[0] * Scale;
		const float ty = translation[1] * Scale;
		const float tz = translation[2] * Scale;

		const float x1 = rotation[0];
		const float y1 = rotation[1];
		const float z1 = rotation[2];
		const float w1 = rotation[3];

		cMatrix Matrix;
		Matrix.NewMatrix();
		Matrix.Rotate(w1, x1, y1, z1);
		const float object_scale = 1.0f / sqrt(w1*w1 + x1*x1 + y1*y1 + z1*z1);
		Matrix.Scale(object_scale / scale[0], object_scale / scale[1], object_scale / scale[2]);

		*x = Matrix.rot()[0][0] * tx + Matrix.rot()[0][1] * ty + Matrix.rot()[0][2] * tz;
		*y = Matrix.rot()[1][0] * tx + Matrix.rot()[1][1] * ty + Matrix.rot()[1][2] * tz;
		*z = Matrix.rot()[2][0] * tx + Matrix.rot()[2][1] * ty + Matrix.rot()[2][2] * tz;
	}
}
