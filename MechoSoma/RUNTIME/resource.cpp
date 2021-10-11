
/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "mechosoma.h"

#include "mch_common.h" // For far target

/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */

char* GetTargetName(char*);

/* --------------------------- DEFINITION SECTION --------------------------- */

#define _XRESOURCE_

#ifdef _XRESOURCE_
XZIP_Resource* mchRes;
XZIP_Resource* mch_vmapRes = NULL;
#endif

int mchUseContainer = 0;
char* acsResourceDir = "RESOURCE\\ISCREEN\\";
char* acsResourceDirL = "RESOURCE\\ISCREEN\\RES_LOW\\";
char* acsResourceDirH = "RESOURCE\\ISCREEN\\RES_HIGH\\";
XBuffer mch_ResXBuf;

int acsOpenResource(char* fname,XStream& fh,int res_flag)
{
	mch_ResXBuf.init();
	if(res_flag == -1)
		mch_ResXBuf < acsResourceDir < fname;
	else {
		if(res_flag)
			mch_ResXBuf < acsResourceDirL < fname;
		else
			mch_ResXBuf < acsResourceDirH < fname;
	}

#ifdef _XRESOURCE_
	if(mchUseContainer){
		if(!mchRes -> open(mch_ResXBuf.address(),fh))
			return fh.open(mch_ResXBuf.address(),XS_IN);
		else 
			return 1;
	}
	else
		return fh.open(mch_ResXBuf.address(),XS_IN);
#else
	return fh.open(mch_ResXBuf.address(),XS_IN);
#endif	
}

int m3dOpenResource(char* fname,XStream& fh)
{
#ifdef _XRESOURCE_
	if(mchUseContainer){
		if(!mchRes -> open(fname,fh))
			return fh.open(fname,XS_IN);
		return 1;
	}
	else
		return fh.open(fname,XS_IN);
#else
	return fh.open(fname,XS_IN);
#endif	
}

void mchXResourceInit(void)
{
#ifdef _XRESOURCE_
	if(mchUseContainer){
		mchRes = new XZIP_Resource("RESOURCE\\resource.pak",XZIP_ENABLE_ZIP_HEADERS);
	}
#endif	
}

int vmapOpenResource(char* fname,XStream& fh)
{
#ifdef _XRESOURCE_
	if(mchUseContainer){
		if(!mch_vmapRes -> open(fname,fh))
			return fh.open(fname,XS_IN);
		return 1;
	}
	else
		return fh.open(fname,XS_IN);
#else
	return fh.open(fname,XS_IN);
#endif	
}

int mch_vmapResourceInit(char * Path2World)
{
#ifdef _XRESOURCE_
	XStream test(0);

	if(mchUseContainer){
		if(mch_vmapRes)
			delete mch_vmapRes;

		const char * resourceFileName="resource.pak";
		XBuffer name;
		if(Path2World==NULL) name < GetTargetName((char*)resourceFileName);
		else name < Path2World < resourceFileName;

		if(test.open(name)){
			test.close();
			mch_vmapRes = new XZIP_Resource(name,XZIP_ENABLE_ZIP_HEADERS);
			return 1;
		}
		else
			return 0;
	}
#endif	
	return 1;
}
