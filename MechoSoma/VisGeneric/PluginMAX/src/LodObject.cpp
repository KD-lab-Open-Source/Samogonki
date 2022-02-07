#include "scenemesh.h"

int sLodObject::Read(cMeshFile &f)
{
	while(!f.eof())
		switch(f.ReadFieldType())
		{
			case MF_TYPE_ID:
				ID=0; f.ReadField(&ID);
				break;
			case MF_TYPE_LOD:
				f.ReadField(&LevelDetail);
				break;
			case MF_TYPE_BLOCK:
				switch(f.ReadBlock())
				{
					case MF_ID_BLOCK_LOD:
						return MESHFILE_OK;
					case MF_ID_BLOCK_OBJECT:
						{
							sObjectMesh *ObjectMesh=new sObjectMesh;
							NodeObjectLibrary.New(ObjectMesh);
							ObjectMesh->Read(f);
							break;
						}
					case MF_ID_BLOCK_HELPEROBJECT:
						{
							sHelperObject *HelperObject=new sHelperObject;
							NodeObjectLibrary.New(HelperObject);
							HelperObject->Read(f);
							break;
						}
					case MF_ID_BLOCK_MATERIAL:
						MaterialLibrary.Append()->Read(f);
						break;
				}
		}
	return MESHFILE_ERROR;
}
int sLodObject::Write(cMeshFile &f)
{
	f.BeginWriteBlock(MF_ID_BLOCK_LOD);
	f.WriteField(&ID,MF_TYPE_ID);
	f.WriteField(&LevelDetail,MF_TYPE_LOD);
	if(MaterialLibrary.Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;
	if(NodeObjectLibrary.Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;
	f.EndWriteBlock(MF_ID_BLOCK_LOD);
	return MESHFILE_OK;
}
/*
sLodObject& sLodObject::operator = (const sLodObject &LodObject)
{
	Release();
	ID=LodObject.ID;
	LevelDetail=LodObject.LevelDetail;
	ChannelLibrary=LodObject.ChannelLibrary;
	return *this;
}
*/