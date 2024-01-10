


/* ---------------------------- INCLUDE SECTION ----------------------------- */

// #include "StdAfx.h"

#include <string>
#include <stdio.h>

#include <curl/curl.h>

#include "wininet_api.h"
// #include "Xreal_utl.h"


/* ----------------------------- STRUCT SECTION ----------------------------- */

typedef struct 
{
	int status;
} REQUEST_CONTEXT;

/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */

/* --------------------------- DEFINITION SECTION --------------------------- */

const char* wiServerName = NULL;
const char* wiGameURL = NULL;
int wiServerPort = 8500;

wiDispatcher wi_D;

void* wiCritical = NULL; 

// cURL multi-handle
// static CURLM* main_handle = NULL;
// static std::string endpoint;

void wiInit(void)
{
	// main_handle = curl_multi_init();
	// endpoint
}

void wiFinit(void)
{
	//TODO:
	// 	Cleans up and removes a whole multi stack. It does not free or touch any individual easy handles in any way - they still need to be closed individually, using the usual curl_easy_cleanup way. The order of cleaning up should be:
	// [O] 1 - curl_multi_remove_handle before any easy handles are cleaned up
	// [O] 2 - curl_easy_cleanup can now be called independently since the easy handle is no longer connected to the multi handle
	// [X] 3 - curl_multi_cleanup should be called when all easy handles are removed
	// Passing in a NULL pointer in multi_handle makes this function return CURLM_BAD_HANDLE immediately with no other action. 

	// curl_multi_cleanup(main_handle);
}

wiDispatcher::wiDispatcher(void)
{
	is_request_completed = false;
	status = WI_IDLE;

	inPos = inSize = 0;
	inBuffer = NULL;

	outPos = outSize = 0;
	outBuffer = NULL;

	http_headers = curl_slist_append(NULL, "Samogonki-Protocol: 1");
	hCurlMulti = curl_multi_init();
}

wiDispatcher::~wiDispatcher(void)
{
	curl_slist_free_all(http_headers);
	curl_multi_cleanup(hCurlMulti);
}

void wiDispatcher::expand_inbuf(int sz)
{
	char* buf;

	if(inSize < sz + 1){ 
		buf = new char[sz + 1];
		memset(buf,0,sz + 1);
		memcpy(buf,inBuffer,inSize);

		delete[] inBuffer;
		
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
		delete[] inBuffer;
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
		delete[] outBuffer;
	}
}

int wiDispatcher::connect(const char* server,unsigned int port)
{
	stringstream ss;
	ss
		<< "http" << "://"
		<< server << ":" << port;

	endpoint = ss.str();
	
	printf("Connect to: %s", endpoint);

	return 1;
}

void wiDispatcher::disconnect(void)
{
}

static void progress(void *contents, size_t size, size_t nmemb, vector<char>* buff) {
	printf("callback curl called, input len: %d\r\n", nmemb);
	for (int i = 0; i < nmemb; i++) {
		buff->push_back(*(i + (char *)contents));
	}
}

int wiDispatcher::open_request(int action,const char* obj,const char* header,int header_len,const char* data,int data_len)
{
	string url = endpoint + string(obj);

	printf("wiDispatcher::open_request, URL: %s\r\n", url);

	if (status != WI_IDLE) {
		close_request();
	}

	this->iBuffer.clear();

	if (action != WI_POST && action != WI_GET) {
		return 0;
	}

	CURL *handle = curl_easy_init();
	if (!handle) {
		return 0;
	}

	int running_handles = 0;
	curl_easy_setopt(handle, CURLOPT_URL, url.data());

	if (action == WI_POST) {
		curl_easy_setopt(handle, CURLOPT_POST, 1);
		curl_easy_setopt(handle, CURLOPT_POSTFIELDS, data);
	}
	
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, http_headers);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, &this->iBuffer);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, progress);
		// curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, [](void *contents, size_t size, size_t nmemb, wiDispatcher *self) {
		// 	printf("callback curl called, input len: %d\r\n", nmemb);
		// 	for (int i = 0; i < nmemb; i++) {
		// 		self->iBuffer.push_back(*((char *)contents));
		// 	}
		// });

	curl_multi_add_handle(hCurlMulti, handle);
	curl_multi_perform(hCurlMulti, &running_handles);
	if (running_handles == 0) {
		action = WI_ERROR;
		// TODO: free curl handles
		throw "error occured when trying to send a request";
	}

	hCurlRequest = handle;
	is_request_completed = false;
	status = action;

	return 1;
}

void wiDispatcher::close_request(void)
{
	if (hCurlRequest) {
		curl_multi_remove_handle(hCurlMulti, hCurlRequest);
		curl_easy_cleanup(hCurlRequest);
		hCurlRequest = NULL;
	}

	// iBuffer.clear();
	// oBuffer.clear();

	is_request_completed = false;
	status = WI_IDLE;
}

void wiDispatcher::quant(void)
{
	printf("wiDispatcher::quant\r\n");

	int running_handles = 0;

	if(status == WI_LOAD_FILE){
		status = WI_FINISHED;
		return;
	}
	else if(status == WI_GET || status == WI_POST){
		curl_multi_perform(hCurlMulti, &running_handles);
		printf("wiDispatcher::quant running handles: %d\r\n", running_handles);
		if (running_handles > 0) {
			return;
		} else {
			iBuffer.push_back(0);
			alloc_inbuf(iBuffer.capacity());
			memcpy(inBuffer, iBuffer.data(), iBuffer.capacity());

			if (hCurlRequest) {
				curl_multi_remove_handle(hCurlMulti, hCurlRequest);
				curl_easy_cleanup(hCurlRequest);
				hCurlRequest = NULL;
			}

			status = WI_FINISHED;
			is_request_completed = true;
		}
	}
}

int wiDispatcher::get_request_status(void)
{
	return is_request_completed;
}

const char* wiDispatcher::get_request_status_str(void)
{
	int running_handles = 0;
	curl_multi_perform(hCurlMulti, &running_handles);
	return running_handles == 1 ? "Completed" : "Running";
}

wiCriticalSection::wiCriticalSection(void* section)
{
}

wiCriticalSection::~wiCriticalSection(void)
{
}
