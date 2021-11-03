#include "scenemesh.h"

int sSubTexmap::Read(cMeshFile &f)
{
	while(!f.eof())
		switch(f.ReadFieldType())
		{
			case MF_TYPE_ID:
				ID=0; f.ReadField(&ID);
				break;
			case MF_TYPE_FILENAME:
				f.ReadField(&name);
				break;
			case MF_TYPE_CHANNELNUMBER:
				f.ReadField(&ChannelNumber);
				break;
			case MF_TYPE_BLOCK:
				switch(f.ReadBlock())
				{
					case MF_ID_BLOCK_SUBTEXMAP:
						return MESHFILE_OK;
				}
		}
	return MESHFILE_ERROR;
}
int sSubTexmap::Write(cMeshFile &f)
{
	if(ID==0) return MESHFILE_OK;
	f.BeginWriteBlock(MF_ID_BLOCK_SUBTEXMAP);
	
	f.WriteField(&ID,MF_TYPE_ID);
	f.WriteField(&name,MF_TYPE_FILENAME);
	f.WriteField(&ChannelNumber,MF_TYPE_CHANNELNUMBER);

	f.EndWriteBlock(MF_ID_BLOCK_SUBTEXMAP);
	return MESHFILE_OK;
}
/*
sSubTexmap& sSubTexmap::operator = (const sSubTexmap &SubMaterial)
{
	Release();
	ID=SubMaterial.ID;
	name=SubMaterial.name;
	return *this;
}
*/

int sAnimationMaterial::Read(cMeshFile &f)
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
			case MF_TYPE_AMBIENTCOLOR: 
				f.ReadField(AmbientColor);
				break;
			case MF_TYPE_DIFFUSECOLOR: 
				f.ReadField(DiffuseColor);
				break;
			case MF_TYPE_SPECULARCOLOR: 
				f.ReadField(SpecularColor);
				break;
			case MF_TYPE_SHININESS: 
				f.ReadField(&Shininess);
				break;
			case MF_TYPE_SHINSTRENGTH: 
				f.ReadField(&ShinStrength);
				break;
			case MF_TYPE_TRANSPARENCY: 
				f.ReadField(&Transparency);
				break;
			case MF_TYPE_BLOCK:
				switch(f.ReadBlock())
				{
					case MF_ID_BLOCK_ANIMATIONMATERIAL:
						return MESHFILE_OK;
					case MF_ID_BLOCK_SUBTEXMAP:
						SubTexmap.Append()->Read(f);
						break;
				}
				break; 
			default:
				assert(0);
		}
	return MESHFILE_ERROR;
}
int sAnimationMaterial::Write(cMeshFile &f)
{
	f.BeginWriteBlock(MF_ID_BLOCK_ANIMATIONMATERIAL);

	f.WriteField(&ID,MF_TYPE_ID);
	f.WriteField(&time,MF_TYPE_TIME);
	f.WriteField(AmbientColor,MF_TYPE_AMBIENTCOLOR);
	f.WriteField(DiffuseColor,MF_TYPE_DIFFUSECOLOR);
	f.WriteField(SpecularColor,MF_TYPE_SPECULARCOLOR);
	f.WriteField(&Shininess,MF_TYPE_SHININESS);
	f.WriteField(&ShinStrength,MF_TYPE_SHINSTRENGTH);
	f.WriteField(&Transparency,MF_TYPE_TRANSPARENCY);
	if(SubTexmap.Write(f)!=MESHFILE_OK) return f.error;

	f.EndWriteBlock(MF_ID_BLOCK_ANIMATIONMATERIAL);
	return MESHFILE_OK;
}
