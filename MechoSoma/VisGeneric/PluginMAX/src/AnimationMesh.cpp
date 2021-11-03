#include "scenemesh.h"

int sAnimationMesh::Read(cMeshFile &f)
{
	while(!f.eof())
		switch(f.ReadFieldType())
		{
			case MF_TYPE_ID:
				ID=0; f.ReadField(&ID);
				break;
			case MF_TYPE_TIME:
				f.ReadField(&time);
				break;
			case MF_TYPE_BLOCK:
				switch(f.ReadBlock())
				{
					case MF_ID_BLOCK_ANIMATIONMESH:
						return MESHFILE_OK;
					case MF_ID_BLOCK_VERTEXMESH:
						Vertex.Read(f);
						break;
					case MF_ID_BLOCK_FACEMESH:
						Face.Read(f);
						break;
					case MF_ID_BLOCK_VERTEXNORMALMESH:
						VertexNormal.Read(f);
						break;
					case MF_ID_BLOCK_FACENORMALMESH:
						FaceNormal.Read(f);
						break;
					case MF_ID_BLOCK_CHANNELMAPPING:
						ChannelMappingLibrary.Append()->Read(f);
						break;
				}
		}
	return MESHFILE_ERROR;
}
int sAnimationMesh::Write(cMeshFile &f)
{
	f.BeginWriteBlock(MF_ID_BLOCK_ANIMATIONMESH);
	f.WriteField(&ID,MF_TYPE_ID);
	f.WriteField(&time,MF_TYPE_TIME);
	if(Vertex.Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;
	if(Face.Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;
	if(VertexNormal.Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;
	if(FaceNormal.Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;
	if(ChannelMappingLibrary.Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;
	f.EndWriteBlock(MF_ID_BLOCK_ANIMATIONMESH);
	return MESHFILE_OK;
}
/*
sAnimationMesh& sAnimationMesh::operator = (const sAnimationMesh &AnimationMesh)
{
	Release();
	ID=AnimationMesh.ID;
	name=AnimationMesh.name;
	Vertex=AnimationMesh.Vertex;
	Face=AnimationMesh.Face;
	return *this;
}
*/