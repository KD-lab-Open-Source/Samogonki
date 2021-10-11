#ifndef __CDCHECK_H__
#define __CDCHECK_H__

//#define CDCHECK

#ifdef CDCHECK

extern char* fcPath;

#define cdCheck_Silent(v) { \
	char* cd0_str = new char[MAX_PATH]; \
	char* cd0_label = new char[64]; \
	char* cd0_system = new char[64]; \
	int cd0_ret = 1; \
	sprintf(cd0_str,"%s\\",fcPath); \
	GetVolumeInformation(cd0_str,cd0_label,64,NULL,NULL,NULL,cd0_system,64); \
	if(cd0_system[3] != 'S' || cd0_system[1] != 'D' || cd0_system[0] != 'C' || cd0_system[2] != 'F') cd0_ret = 0; \
	if(cd0_ret) if(stricmp(cd0_label,iGetText(iTXT_CDLABEL))) cd0_ret = 0; \
	if(!cd0_ret) quantRTO -> NextID = v; \
	delete cd0_str; \
	delete cd0_label; \
	delete cd0_system; \
}

#define cdCheck(f0,f1) { \
	char* cd_str = new char[MAX_PATH]; \
	char* cd_label = new char[64]; \
	char* cd_system = new char[64]; \
	int cd_ret = 1; \
	sprintf(cd_str,"%s\\",fcPath); \
	GetVolumeInformation(cd_str,cd_label,64,NULL,NULL,NULL,cd_system,64); \
	if(cd_system[3] != 'S' || cd_system[1] != 'D' || cd_system[0] != 'C' || cd_system[2] != 'F') cd_ret = 0; \
	if(cd_ret) if(stricmp(cd_label,iGetText(iTXT_CDLABEL))) cd_ret = 0; \
	if(cd_ret){  \
		sprintf(cd_str,"%s\\%s",fcPath,f0); \
		DWORD cd_a = GetFileAttributes(cd_str); \
		if(cd_a == 0xFFFFFFFF || !(cd_a & FILE_ATTRIBUTE_READONLY)) cd_ret = 0; \
		if(cd_ret){ \
			sprintf(cd_str,"%s\\%s",fcPath,f1); \
			DWORD cd_a = GetFileAttributes(cd_str); \
			if(cd_a == 0xFFFFFFFF || !(cd_a & FILE_ATTRIBUTE_READONLY)) cd_ret = 0; \
		} \
	} \
	if(!cd_ret) ErrH.Abort(iGetText(iTXT_NOCD_MSG)); \
	delete cd_str; \
	delete cd_label; \
	delete cd_system; \
}
#else
#define cdCheck_Silent(v)
#define cdCheck(f0,f1)
#endif

#endif /* __CDCHECK_H__ */