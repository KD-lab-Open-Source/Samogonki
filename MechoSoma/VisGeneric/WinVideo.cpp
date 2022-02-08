#include <amstream.h>	// DirectShow multimedia stream interfaces
#include <control.h>
#include <uuids.h>
#include <assert.h>
#include <evcode.h>
#include "WinVideo.h"

//#pragma comment(lib,"strmbase")

void sWinVideo::Init()
{
	CoInitialize(NULL);
}
void sWinVideo::Done()
{
	CoUninitialize();
}
void sWinVideo::SetWin(void *hWnd,int x,int y,int xsize,int ysize)
{
	sWinVideo::hWnd=hWnd;
	if(pVideoWindow&&hWnd) 
	{	//Set the video window.
		pVideoWindow->put_Owner((OAHWND)hWnd);
		pVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPSIBLINGS);

		RECT grc={x,y,xsize,ysize};
		if(grc.right==0||grc.bottom==0) GetClientRect((HWND)hWnd, &grc);
		pVideoWindow->SetWindowPosition(grc.left,grc.top,grc.right,grc.bottom);
	}
}
int sWinVideo::Open(char *fname)
{
	sWinVideo::Close();
    // Create the filter graph manager.
    CoCreateInstance(CLSID_FilterGraph, NULL,CLSCTX_INPROC,IID_IGraphBuilder,(void**)&pGraphBuilder);
	if(pGraphBuilder==0) { Close(); return 1; }
    pGraphBuilder->QueryInterface(IID_IMediaControl,(void**)&pMediaControl);
	if(pMediaControl==0) { Close(); return 2; }
    pGraphBuilder->QueryInterface(IID_IVideoWindow,(void**)&pVideoWindow);
	if(pVideoWindow==0)  { Close(); return 3; }
	pGraphBuilder->QueryInterface(IID_IMediaEvent,(void**)&pMediaEvent);
	if(pMediaEvent==0)  { Close(); return 3; }
    WCHAR wPath[MAX_PATH]; // Convert the file name to a wide-character string.
    MultiByteToWideChar(CP_ACP, 0, fname, -1, wPath, MAX_PATH);    
    if(pGraphBuilder->RenderFile(wPath,NULL)) { Close(); return 4; }
	if(hWnd) SetWin(hWnd);
	return 0;
}
void sWinVideo::Play()
{	// Run the graph.
	if(pGraphBuilder&&pVideoWindow&&pMediaControl&&hWnd) 
	{
		pVideoWindow->put_AutoShow(-1);
		pVideoWindow->put_Visible(-1);
		pMediaControl->Run();
	}
}
void sWinVideo::Stop()
{	// stop the graph.
	if(pGraphBuilder&&pVideoWindow&&pMediaControl&&hWnd) 
		pMediaControl->Stop();
}
int sWinVideo::IsPlay()
{
	if(pGraphBuilder&&pVideoWindow&&pMediaControl&&hWnd) 
	{
		OAFilterState pfs;
		if(pMediaControl->GetState(INFINITE,&pfs)!=S_OK) 
			return 0;
		if(pfs==State_Running) return 1;
		if(pfs==State_Stopped) return 0;
		if(pfs==State_Paused) return -1;
	}
	return 0;
};
void sWinVideo::WaitEnd()
{
    long evCode;
    if(pMediaEvent)
		pMediaEvent->WaitForCompletion(INFINITE,&evCode);
}
int sWinVideo::IsComplete()
{
    long evCode,param1,param2;
	while(pMediaEvent->GetEvent(&evCode,&param1,&param2,0)==S_OK)
	{
		pMediaEvent->FreeEventParams(evCode,param1,param2);
		if((EC_COMPLETE==evCode)||(EC_USERABORT==evCode))
			return 1;
	}
	return 0;
}
void sWinVideo::FullScreen(int bFullScreen)
{
	if(pVideoWindow)	
		pVideoWindow->put_FullScreenMode(bFullScreen);
}
void sWinVideo::HideCursor(int hide)
{
	if(pVideoWindow==0)	return;
	pVideoWindow->HideCursor(hide); 
}
void sWinVideo::GetSize(int *xsize,int *ysize)
{
	if(pVideoWindow==0)	return;
	pVideoWindow->get_Width((long*)xsize); 
	pVideoWindow->get_Height((long*)ysize);
}
void sWinVideo::SetSize(int xsize,int ysize)
{
	if(pVideoWindow==0)	return;
	pVideoWindow->put_Width(xsize); 
	pVideoWindow->put_Height(ysize);
}
void sWinVideo::Close()
{
	if(pMediaEvent)
	{
		pMediaEvent->Release();
		pMediaEvent=0;
	}
    if(pVideoWindow) 
	{
		pVideoWindow->put_Visible(FALSE);
//		pVideoWindow->put_Owner(NULL);   // может произойти потеря фокуса в полноэкранном режиме
	}
    if(pMediaControl)
	{
		pMediaControl->Release(); 
		pMediaControl=0;
	}
    if(pVideoWindow) 
	{
		pVideoWindow->Release(); 
		pVideoWindow=0;
	}
    if(pGraphBuilder)
	{
		pGraphBuilder->Release(); 
		pGraphBuilder=0;
	}
}
