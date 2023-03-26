
/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include "StdAfx.h"

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

/* --------------------------- DEFINITION SECTION --------------------------- */

const char* wiServerName = NULL;
const char* wiGameURL = NULL;
int wiServerPort = 8500;

wiDispatcher wi_D;

void* wiCritical = NULL; 

void wiInit(void)
{
}

void wiFinit(void)
{
}

wiDispatcher::wiDispatcher(void)
{
}

wiDispatcher::~wiDispatcher(void)
{
}

void wiDispatcher::expand_inbuf(int sz)
{
}

void wiDispatcher::alloc_inbuf(int sz)
{
}

void wiDispatcher::free_inbuf(void)
{
}

void wiDispatcher::alloc_outbuf(int sz)
{
}

void wiDispatcher::free_outbuf(void)
{
}

int wiDispatcher::connect(const char* server,unsigned int port)
{
	return 0;
}

void wiDispatcher::disconnect(void)
{
}

int wiDispatcher::open_request(int action,const char* obj,const char* header,int header_len,const char* data,int data_len)
{
	return 1;
}

void wiDispatcher::close_request(void)
{
}

void wiDispatcher::quant(void)
{
}

int wiDispatcher::get_request_status(void)
{
	return 0;
}

char* wiDispatcher::get_request_status_str(void)
{
    return nullptr;
}

wiCriticalSection::wiCriticalSection(void* section)
{
}

wiCriticalSection::~wiCriticalSection(void)
{
}
