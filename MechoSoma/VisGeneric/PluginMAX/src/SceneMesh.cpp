#include "SceneMesh.h"

int cMeshScene::Read(cMeshFile &f)
{
	if((f.ReadFieldType()!=MF_TYPE_BLOCK)||(f.ReadBlock()!=MF_ID_BLOCK_SCENE)) return MESHFILE_ERROR;
	while(!f.eof())
		switch(f.ReadFieldType())
		{
			case MF_TYPE_ID:
				ID=0; f.ReadField(&ID);
				break;
			case MF_TYPE_NAME:
				name.Release(); f.ReadField(&name);
				break;
			case MF_TYPE_BLOCK:
				switch(f.ReadBlock())
				{
					case MF_ID_BLOCK_SCENE:
						return MESHFILE_OK;
					case MF_ID_BLOCK_CHANNEL:
						ChannelLibrary.Append()->Read(f);
						break;
				}
				break; 
		}
	return MESHFILE_ERROR;
}
int cMeshScene::Write(cMeshFile &f)
{
	if(ID==0) return MESHFILE_OK;
	f.BeginWriteBlock(MF_ID_BLOCK_SCENE);
	f.WriteField(&ID,MF_TYPE_ID); 
	f.WriteField(&name,MF_TYPE_NAME); 
	if(ChannelLibrary.Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;
	f.EndWriteBlock(MF_ID_BLOCK_SCENE);
	return MESHFILE_OK;
}
