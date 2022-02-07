#include "scenemesh.h"

int sMaterialObject::Read(cMeshFile &f)
{
	sAnimationMaterial *CurrentAnimationMaterial=0;
	while(!f.eof())
		switch(f.ReadFieldType())
		{
			case MF_TYPE_ID:
				ID=0; f.ReadField(&ID);
				break;
			case MF_TYPE_NAME:
				name.Release(); f.ReadField(&name);
				break;
			case MF_TYPE_PARENT:
				f.ReadField(&parent);
				break;
			case MF_TYPE_SHADING:
				f.ReadField(&Shading);
				break;
			case MF_TYPE_NUMBERSUBOBJECT:
				f.ReadField(&NumberSubObject);
				break;
			// для совместимости версий с анимацией материала и без анимации материала
			case MF_TYPE_AMBIENTCOLOR:
				if(CurrentAnimationMaterial==0) 
					CurrentAnimationMaterial=AnimationMaterialLibrary.Append();
				f.ReadField(CurrentAnimationMaterial->AmbientColor);
				break;
			case MF_TYPE_DIFFUSECOLOR:
				if(CurrentAnimationMaterial==0) 
					CurrentAnimationMaterial=AnimationMaterialLibrary.Append();
				f.ReadField(CurrentAnimationMaterial->DiffuseColor);
				break;
			case MF_TYPE_SPECULARCOLOR: 
				if(CurrentAnimationMaterial==0) 
					CurrentAnimationMaterial=AnimationMaterialLibrary.Append();
				f.ReadField(CurrentAnimationMaterial->SpecularColor);
				break;
			case MF_TYPE_SHININESS: 
				if(CurrentAnimationMaterial==0) 
					CurrentAnimationMaterial=AnimationMaterialLibrary.Append();
				f.ReadField(&CurrentAnimationMaterial->Shininess);
				break;
			case MF_TYPE_SHINSTRENGTH: 
				if(CurrentAnimationMaterial==0) 
					CurrentAnimationMaterial=AnimationMaterialLibrary.Append();
				f.ReadField(&CurrentAnimationMaterial->ShinStrength);
				break;
			case MF_TYPE_TRANSPARENCY: 
				if(CurrentAnimationMaterial==0) 
					CurrentAnimationMaterial=AnimationMaterialLibrary.Append();
				f.ReadField(&CurrentAnimationMaterial->Transparency);
				break;
			case MF_TYPE_BLOCK:
				switch(f.ReadBlock())
				{
					case MF_ID_BLOCK_MATERIAL:
						CurrentAnimationMaterial=0;
						return MESHFILE_OK;
					case MF_ID_BLOCK_ANIMATIONMATERIAL:
						CurrentAnimationMaterial=0;
						AnimationMaterialLibrary.Append()->Read(f);
						break;
					case MF_ID_BLOCK_SUBTEXMAP:
						if(CurrentAnimationMaterial==0) 
							CurrentAnimationMaterial=AnimationMaterialLibrary.Append();
						CurrentAnimationMaterial->SubTexmap.Append()->Read(f);
						break;
				}
				break; 
			default:
				assert(0);
		}
	return MESHFILE_ERROR;
}
int sMaterialObject::Write(cMeshFile &f)
{
	f.BeginWriteBlock(MF_ID_BLOCK_MATERIAL);

	f.WriteField(&ID,MF_TYPE_ID);
	f.WriteField(&name,MF_TYPE_NAME);
	f.WriteField(&Shading,MF_TYPE_SHADING);
	if(parent) f.WriteField(&parent,MF_TYPE_PARENT);
	if(NumberSubObject) f.WriteField(&NumberSubObject,MF_TYPE_NUMBERSUBOBJECT);
	if(AnimationMaterialLibrary.Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;

	f.EndWriteBlock(MF_ID_BLOCK_MATERIAL);
	return MESHFILE_OK;
}
/*
sMaterialObject& sMaterialObject::operator = (const sMaterialObject &Material)
{
	Release();
	ID=Material.ID;					// порядковый номер в библиотеке
	name=Material.name;				// имя материала
	parent=Material.parent;			// родитель, тот который содержит этот материал (MultiSubObjectMaterial)
	MEMCPY(AmbientColor,Material.AmbientColor,3*sizeof(float));
	MEMCPY(DiffuseColor,Material.DiffuseColor,3*sizeof(float));
	MEMCPY(SpecularColor,Material.SpecularColor,3*sizeof(float));
	Shininess=Material.Shininess;
	ShinStrength=Material.ShinStrength;
	Transparency=Material.Transparency;
	NumberSubObject=Material.NumberSubObject;	// число материалов в данном, которые будут ссылаться через parent на данный
	SubTexmap=Material.SubTexmap;
	return *this;
}
*/
