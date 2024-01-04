
#ifndef __WININET_API_H__
#define __WININET_API_H__

#include <vector>
#include <string>
#include <sstream>
#include <exception>

using namespace std;

struct wiCriticalSection
{
	void* csection;
	wiCriticalSection(void *section);
	~wiCriticalSection(void);
};

const int WI_TEMP_BUFFER_SIZE	= 512;

enum wiStatus 
{
	WI_IDLE = 0,
	WI_GET,
	WI_POST,
	WI_FINISHED,
	WI_LOAD_FILE,
	WI_ERROR
};

class wiDispatcher
{
	bool is_request_completed;
	int status;

	void* hCurlMulti;
	void* hCurlRequest;

	vector<char> oBuffer;
	vector<char> iBuffer;

	void* hConnect;
	void* hRequest;

	void* reqContext;

	int inSize;
	int inPos;
	char* inBuffer;

	int outSize;
	int outPos;
	char* outBuffer;

	string endpoint = "http://127.0.0.1:8080";

//	char tmpBuffer[WI_TEMP_BUFFER_SIZE];

public:
	void set_error(void){ 
		status = WI_ERROR; 
	}

	void set_status(int st){
		status = st;
	}

	int get_status(void) const { return status; }
	int get_request_status(void);
	const char* get_request_status_str(void);

	void alloc_inbuf(int sz);
	void expand_inbuf(int sz);
	void free_inbuf(void);

	void alloc_outbuf(int sz);
	void free_outbuf(void);

	int input_pos(void) const { return inPos; }
	int input_size(void) const { return inSize; }
	char* input_buffer(void) { return inBuffer; }

	int output_pos(void) const { return outPos; }
	int output_size(void) const { return outSize; }
	char* output_buffer(void) { return outBuffer; }

	int connect(const char* server,unsigned int port);
	void disconnect(void);

	int open_request(int action,const char* obj,const char* header = 0,int header_len = 0,const char* data = 0,int data_len = 0);
	void close_request(void);

	void quant(void);

	wiDispatcher(void);
	~wiDispatcher(void);
};

void wiInit(void);
void wiFinit(void);

extern wiDispatcher wi_D;
extern const char* wiServerName;
extern const char* wiGameURL;
extern int wiServerPort;

extern void* wiCritical;

#endif /* __WININET_API_H__ */