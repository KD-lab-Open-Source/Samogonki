#include "StdAfx.h"
void dprintf(char *format, ...)
{
  va_list args;
  char    buffer[512];

  va_start(args,format);

  strcpy(buffer + vsprintf(buffer,format,args), "\r\n");

  OutputDebugString(buffer);
}

void ERRORM(BOOL b,char *format, ...)
{
	if(b)return;
//	if(g_pDD)g_pDD->FlipToGDISurface();
	va_list args;
	char    buffer[512];
	va_start(args,format);
	strcpy(buffer + vsprintf(buffer,format,args), "\r\n");
	OutputDebugString(buffer);

	MessageBox(g_hWnd, buffer, "Game", MB_OK|MB_ICONSTOP|MB_APPLMODAL);
	exit(0);
}

char TEXTMSG[1024];
int __cdecl AssertFail(char szErr[], char szFileName[], int nLine, char szMessage[])
{
//	if(g_pDD)g_pDD->FlipToGDISurface();

    if (szMessage != NULL)
        wsprintf(TEXTMSG, "Assert(%s);\nFile %s, line %d.  %s", szErr, szFileName, nLine, szMessage);
    else 
        wsprintf(TEXTMSG, "Assert(%s);\nFile %s, line %d.", szErr, szFileName, nLine);
    switch (MessageBox(g_hWnd, TEXTMSG, "Game", MB_ABORTRETRYIGNORE | MB_ICONSTOP | MB_APPLMODAL))
        {
        case IDABORT:
			exit(0);
			return 2;
        case IDRETRY:
//			if(FullScreen && g_pDD)
//				g_pDD->SetCooperativeLevel(g_hWnd , DDSCL_NORMAL);
			return 1;
            // Fall Through //
        case IDIGNORE:
			return 0;
        } // switch
	return 0;
} // AssertFail
