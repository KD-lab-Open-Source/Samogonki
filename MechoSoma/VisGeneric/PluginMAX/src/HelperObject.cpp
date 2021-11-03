#include "scenemesh.h"

int sHelperObject::Read(cMeshFile &f)
{
	while(!f.eof())
		switch(f.ReadFieldType())
		{
			case MF_TYPE_ID:
				ID=0; f.ReadField(&ID);
				break;
			case MF_TYPE_NAME:
				f.ReadField(&name);
				break;
			case MF_TYPE_PARENT:
				f.ReadField(&parent);
				break;
			case MF_TYPE_MATRIX:
				f.ReadField(&matrix);
				break;
			case MF_TYPE_HELPERNAME:
				f.ReadField(&HelperName);
				break;
			case MF_TYPE_BLOCK:
				switch(f.ReadBlock())
				{
					case MF_ID_BLOCK_HELPEROBJECT:
						return MESHFILE_OK;
					case MF_ID_BLOCK_ANIMATIONVISIBILITY:
						AnimationVisibility.Read(f);
						break;
					case MF_ID_BLOCK_ANIMATIONPOSITION:
						AnimationPosition.Read(f);
						break;
					case MF_ID_BLOCK_ANIMATIONROTATION:
						AnimationRotation.Read(f);
						break;
					case MF_ID_BLOCK_ANIMATIONSCALE:
						AnimationScale.Read(f);
						break;
				}
		}
	return MESHFILE_ERROR;
}
int sHelperObject::Write(cMeshFile &f)
{
	f.BeginWriteBlock(MF_ID_BLOCK_HELPEROBJECT);
	f.WriteField(&ID,MF_TYPE_ID);
	f.WriteField(&name,MF_TYPE_NAME);
	f.WriteField(&parent,MF_TYPE_PARENT);
	f.WriteField(&matrix,MF_TYPE_MATRIX);
	f.WriteField(&HelperName,MF_TYPE_HELPERNAME);
	if(AnimationVisibility.Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;
	if(AnimationPosition.Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;
	if(AnimationRotation.Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;
	if(AnimationScale.Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;
	f.EndWriteBlock(MF_ID_BLOCK_HELPEROBJECT);
	return MESHFILE_OK;
}
