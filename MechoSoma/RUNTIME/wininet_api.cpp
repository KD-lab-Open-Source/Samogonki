
/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include "StdAfx.h"

#include <windows.h>
#include <wininet.h>

#include <stdio.h>

#include "wininet_api.h"
#include "Xreal_utl.h"

/* ----------------------------- STRUCT SECTION ----------------------------- */

typedef struct 
{
	int status;
} REQUEST_CONTEXT;

/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */

void __stdcall wiStatus(HINTERNET hInternet,DWORD dwContext,DWORD dwInternetStatus,LPVOID lpvStatusInformation,DWORD dwStatusInformationLength);
char* win32_errmsg(int id);

/* --------------------------- DEFINITION SECTION --------------------------- */

//#define _ASYNC_MODE_

HINTERNET hIntRoot = NULL;

char* wiServerName = NULL;
char* wiGameURL = NULL;
int wiServerPort = 8500;

wiDispatcher wi_D;

CRITICAL_SECTION wiCriticalSec;
void* wiCritical = NULL; 

void wiInit(void)
{
	static char* agent = "Spanking Runners";

#ifdef _ASYNC_MODE_
	InitializeCriticalSection(&wiCriticalSec);
	wiCritical = &wiCriticalSec;

	hIntRoot = InternetOpen(agent,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,INTERNET_FLAG_ASYNC);
	InternetSetStatusCallback(hIntRoot,(INTERNET_STATUS_CALLBACK)wiStatus);

	CONTROL_FP();
#else
	hIntRoot = InternetOpen(agent,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);

	CONTROL_FP();
#endif
}

void wiFinit(void)
{
	if(hIntRoot)
		InternetCloseHandle(hIntRoot);

	CONTROL_FP();
}

void __stdcall wiStatus(HINTERNET hInternet,DWORD dwContext,DWORD dwInternetStatus,LPVOID lpvStatusInformation,DWORD dwStatusInformationLength)
{
	REQUEST_CONTEXT* cpContext;

	wiCriticalSection sec(wiCritical);

	cpContext = (REQUEST_CONTEXT*)dwContext;
	
	switch(dwInternetStatus){
		case INTERNET_STATUS_REQUEST_COMPLETE:
			if(LPINTERNET_ASYNC_RESULT(lpvStatusInformation) -> dwError == 0)
				cpContext -> status = dwInternetStatus;
			else 
				cpContext -> status = -1;
			break;
		default:
			cpContext -> status = dwInternetStatus;
			break;
	}
}

wiDispatcher::wiDispatcher(void)
{
	flags = 0;
	status = WI_IDLE;

	hConnect = NULL;
	hRequest = NULL;

	reqContext = new REQUEST_CONTEXT;
	memset(reqContext,0,sizeof(REQUEST_CONTEXT));

	inPos = inSize = 0;
	inBuffer = NULL;

	outPos = outSize = 0;
	outBuffer = NULL;
}

wiDispatcher::~wiDispatcher(void)
{
	close_request();
	disconnect();

	free_inbuf();
	free_outbuf();

	delete reqContext;
}

void wiDispatcher::expand_inbuf(int sz)
{
	char* buf;

	if(inSize < sz + 1){ 
		buf = new char[sz + 1];
		memset(buf,0,sz + 1);
		memcpy(buf,inBuffer,inSize);

		delete inBuffer;
		
		inBuffer = buf;
		inSize = sz + 1;
	}
}

void wiDispatcher::alloc_inbuf(int sz)
{
	if(inSize < sz + 1){ 
		free_inbuf();

		inSize = sz + 1;
		inBuffer = new char[sz + 1];
	}
	memset(inBuffer,0,sz + 1);

	inPos = 0;
}

void wiDispatcher::free_inbuf(void)
{
	if(inSize){
		inSize = 0;
		delete inBuffer;
	}
}

void wiDispatcher::alloc_outbuf(int sz)
{
	if(outSize < sz + 1){ 
		free_outbuf();

		outSize = sz + 1;
		outBuffer = new char[sz + 1];
	}
	memset(outBuffer,0,sz + 1);

	outPos = 0;
}

void wiDispatcher::free_outbuf(void)
{
	if(outSize){
		outSize = 0;
		delete outBuffer;
	}
}

int wiDispatcher::connect(char* server,unsigned int port)
{
	REQUEST_CONTEXT* rq = (REQUEST_CONTEXT*) reqContext;

	disconnect();

	if(port == 0xFFFFFFFF)
		port = INTERNET_DEFAULT_HTTP_PORT;

#ifdef _ASYNC_MODE_
	hConnect = InternetConnect(hIntRoot,server,port,NULL,NULL,INTERNET_SERVICE_HTTP,0,(DWORD)reqContext);

	CONTROL_FP();
#else
	DWORD cnt = 0;
	hConnect = InternetConnect(hIntRoot,server,port,NULL,NULL,INTERNET_SERVICE_HTTP,0,cnt);

	CONTROL_FP();
#endif

	if(hConnect) return 1;

	return 0;
}

void wiDispatcher::disconnect(void)
{
	if(hConnect) InternetCloseHandle(hConnect);
	hConnect = NULL;

	CONTROL_FP();
}

int wiDispatcher::open_request(int action,char* obj,char* header,int header_len,char* data,int data_len)
{
	unsigned long idx = 0;

	char* verb = (action == WI_POST) ? "POST" : "GET";

	if(status != WI_IDLE) close_request();

	if(!hConnect) return 0;

	inPos = outPos = 0;

#ifdef _ASYNC_MODE_
	if(!(hRequest = HttpOpenRequest(hConnect,verb,obj,NULL,NULL,NULL,INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD,(DWORD)reqContext)))
		return 0;
#else
	DWORD cnt = 0;
	if(!(hRequest = HttpOpenRequest(hConnect,verb,obj,NULL,NULL,NULL,INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD,cnt)))
		return 0;
#endif

	HttpSendRequest(hRequest,header,header_len,data,data_len);

	status = action;

	flags &= ~WI_REQUEST_COMPLETED;

#ifndef _ASYNC_MODE_
	REQUEST_CONTEXT *rc = (REQUEST_CONTEXT*)reqContext;
	rc -> status = INTERNET_STATUS_REQUEST_COMPLETE;
#endif

	CONTROL_FP();

	return 1;
}

void wiDispatcher::close_request(void)
{
	if(hRequest)
		InternetCloseHandle(hRequest);

	hRequest = 0;
	flags &= ~WI_REQUEST_COMPLETED;
	status = WI_IDLE;

	CONTROL_FP();
}

void wiDispatcher::quant(void)
{
	unsigned long sz,dl_sz;
	REQUEST_CONTEXT* rc;

	wiCriticalSection sec(wiCritical);

	rc = (REQUEST_CONTEXT*)reqContext;

	if(status == WI_LOAD_FILE){
		status = WI_FINISHED;
		return;
	}

	if(status == WI_GET){
		if(flags & WI_REQUEST_COMPLETED){
			if(!InternetQueryDataAvailable(hRequest,&sz,0,0)){
				return;
			}
			if(sz){
				if(sz > WI_TEMP_BUFFER_SIZE) sz = WI_TEMP_BUFFER_SIZE;

				if((int)sz + inPos >= inSize)
					sz = inSize - inPos;

				if(!InternetReadFile(hRequest,(LPVOID)(inBuffer + inPos),sz,&dl_sz)){
					set_error();
					return;
				}

				inPos += dl_sz;
				if(inPos >= inSize)
					status = WI_FINISHED;
			}
			else 
				status = WI_FINISHED;
			return;
		}

		if(rc -> status == -1){
			set_error();
			return;
		}
		if(rc -> status == INTERNET_STATUS_REQUEST_COMPLETE){
			if(!InternetQueryDataAvailable(hRequest,&sz,0,0)){
				set_error();
				return;
			}

			if(sz)
				alloc_inbuf(sz);

			flags |= WI_REQUEST_COMPLETED;
		}
	}
	if(status == WI_POST){
		if(flags & WI_REQUEST_COMPLETED){
			if(!InternetQueryDataAvailable(hRequest,&sz,0,0)){
				return;
			}
			if(sz){
				if(sz > WI_TEMP_BUFFER_SIZE) sz = WI_TEMP_BUFFER_SIZE;

				if((int)sz + inPos >= inSize)
					expand_inbuf((int)sz + inPos);

				if(!InternetReadFile(hRequest,(LPVOID)(inBuffer + inPos),sz,&dl_sz)){
					set_error();
					return;
				}

				inPos += dl_sz;
				if(inPos >= inSize)
					status = WI_FINISHED;
			}
			else 
				status = WI_FINISHED;
			return;
		}
		if(rc -> status == -1){
			set_error();
			return;
		}
		if(rc -> status == INTERNET_STATUS_REQUEST_COMPLETE){
			InternetQueryDataAvailable(hRequest,&sz,0,0);

			if(sz)
				alloc_inbuf(sz);

			flags |= WI_REQUEST_COMPLETED;
		}

	}
}

int wiDispatcher::get_request_status(void)
{
	return (flags & WI_REQUEST_COMPLETED);
}

char* wiDispatcher::get_request_status_str(void)
{
	REQUEST_CONTEXT *rc = (REQUEST_CONTEXT*)reqContext;

	static char* status_msg[] = {
		"closing connection",
		"connected to server",
		"connecting to server",
		"connection closed",
		"handle closing",
		"handle created",
		"intermediate response",
		"name resolved",
		"receiving response",
		"response received",
		"redirect",
		"request sent",
		"resolving name",
		"sending request",
		"state change",
		"request complete",
		" "
	};
	switch(rc -> status){
		case INTERNET_STATUS_CLOSING_CONNECTION:
			return status_msg[0];
		case INTERNET_STATUS_CONNECTED_TO_SERVER:
			return status_msg[1];
		case INTERNET_STATUS_CONNECTING_TO_SERVER:
			return status_msg[2];
		case INTERNET_STATUS_CONNECTION_CLOSED:
			return status_msg[3];
		case INTERNET_STATUS_HANDLE_CLOSING:
			return status_msg[4];
		case INTERNET_STATUS_HANDLE_CREATED:
			return status_msg[5];
		case INTERNET_STATUS_INTERMEDIATE_RESPONSE:
			return status_msg[6];
		case INTERNET_STATUS_NAME_RESOLVED:
			return status_msg[7];
		case INTERNET_STATUS_RECEIVING_RESPONSE:
			return status_msg[8];
		case INTERNET_STATUS_RESPONSE_RECEIVED:
			return status_msg[9];
		case INTERNET_STATUS_REDIRECT:
			return status_msg[10];
		case INTERNET_STATUS_REQUEST_SENT:
			return status_msg[11];
		case INTERNET_STATUS_RESOLVING_NAME:
			return status_msg[12];
		case INTERNET_STATUS_SENDING_REQUEST:
			return status_msg[13];
		case INTERNET_STATUS_STATE_CHANGE:
			return status_msg[14];
		case INTERNET_STATUS_REQUEST_COMPLETE:
			return status_msg[15];
		default:
			return status_msg[16];
	}
}

wiCriticalSection::wiCriticalSection(void* section)
{ 
#ifdef _ASYNC_MODE_
	csection = section; 
	EnterCriticalSection((LPCRITICAL_SECTION)csection); 
#endif
}

wiCriticalSection::~wiCriticalSection(void)
{ 
#ifdef _ASYNC_MODE_
	LeaveCriticalSection((LPCRITICAL_SECTION)csection); 
#endif
}
