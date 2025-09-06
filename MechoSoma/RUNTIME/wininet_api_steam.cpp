
/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include "StdAfx.h"

#include <cstdio>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

#include <steam/steam_api.h>

#include "wininet_api.h"
#include "Xreal_utl.h"

/* ----------------------------- STRUCT SECTION ----------------------------- */

class SteamAuthTicket final
{
public:
	explicit SteamAuthTicket(std::function<void(const std::vector<uint8_t> &)> result) : _result(result), _callback(*this) {}

	~SteamAuthTicket()
	{
		cancel();
	}

	void request()
	{
		if (!_ticket.empty())
		{
			_result(_ticket);
			return;
		}

		if (_auth_ticket != 0)
		{
			return;
		}

		printf("SteamAuthTicket: requesting\n");
		_auth_ticket = SteamUser()->GetAuthTicketForWebApi(nullptr);
	}

	void cancel()
	{
		if (_auth_ticket == 0)
		{
			return;
		}

		SteamUser()->CancelAuthTicket(_auth_ticket);
		_auth_ticket = 0;
		_ticket.clear();
	}

private:
	void on_response(GetTicketForWebApiResponse_t *response)
	{
		if (response->m_eResult != k_EResultOK)
		{
			printf("SteamAuthTicket: error=%d\n", static_cast<unsigned>(response->m_eResult));
			return;
		}

		_ticket.resize(response->m_cubTicket);
		auto data = static_cast<const uint8_t *>(response->m_rgubTicket);
		_ticket.assign(data, data + response->m_cubTicket);

		printf("SteamAuthTicket: ready\n");
		_result(_ticket);
	}

	std::function<void(const std::vector<uint8_t> &)> _result;
	HAuthTicket _auth_ticket = 0;
	std::vector<uint8_t> _ticket;

	struct Callback final : private CCallbackImpl<sizeof(GetTicketForWebApiResponse_t)>
	{
		Callback(SteamAuthTicket &outer) : _outer(outer)
		{
			SteamAPI_RegisterCallback(this, GetTicketForWebApiResponse_t::k_iCallback);
		}

		Callback(const Callback &) = delete;

		Callback &operator=(const Callback &)
		{
			return *this;
		}

	private:
		void Run(void *parameter) override
		{
			_outer.on_response(reinterpret_cast<GetTicketForWebApiResponse_t *>(parameter));
		}

		SteamAuthTicket &_outer;
	};
	Callback _callback;
};

class AuthToken final
{
public:
	enum class State : uint8_t
	{
		empty, requesting, ready
	};

	static AuthToken &get()
	{
		static AuthToken token;
		return token;
	}

	void set_endpoint(const std::string &endpoint)
	{
		_address = endpoint + "/game-on-line/token";
	}

	State get_state() const
	{
		return _state;
	}

	const std::string &get_token() const
	{
		return _token;
	}

	void request()
	{
		if (_state != State::empty)
		{
			return;
		}
		_state = State::requesting;

		_token.clear();
		_auth_ticket.request();
	}

	void cancel()
	{
		if (_request != INVALID_HTTPREQUEST_HANDLE)
		{
			SteamHTTP()->ReleaseHTTPRequest(_request);
		}
	}

private:
	AuthToken() : _auth_ticket([this](const std::vector<uint8_t> &ticket) { on_auth_ticket(ticket); }) {}

	State _state = State::empty;
	SteamAuthTicket _auth_ticket;
	std::string _token;

	std::string _address;
	std::vector<uint8> _body;
	HTTPRequestHandle _request = INVALID_HTTPREQUEST_HANDLE;
	SteamAPICall_t _call_handle = 0;
	CCallResult<AuthToken, HTTPRequestCompleted_t> _request_complete;

	void on_auth_ticket(const std::vector<uint8_t> &ticket)
	{
		_body = ticket;
		_request = SteamHTTP()->CreateHTTPRequest(EHTTPMethod::k_EHTTPMethodPOST, _address.c_str());

		SteamHTTP()->SetHTTPRequestRawPostBody(
			_request,
			"application/octet-stream",
			_body.data(),
			_body.size()
		);

		if (!SteamHTTP()->SendHTTPRequest(_request, &_call_handle))
		{
			printf("AuthToken: failed to send request\n");
			return;
		}

		_request_complete.Set(_call_handle, this, &AuthToken::on_request_complete);
	}

	void on_request_complete(HTTPRequestCompleted_t *callback, bool io_failure)
	{
		if (io_failure || !callback->m_bRequestSuccessful)
		{
			printf("AuthToken: request failed\n");
			return;
		}

		printf("AuthToken: request status code=%d\n", callback->m_eStatusCode);
		if (callback->m_eStatusCode != 200)
		{
			return;
		}

		uint32 response_size = 0;
		if (!SteamHTTP()->GetHTTPResponseBodySize(_request, &response_size))
		{
			printf("AuthToken: response body size failed\n");
			return;
		}

		if (response_size == 0)
		{
			printf("AuthToken: empty response\n");
			return;
		}

		_token.resize(response_size);
		auto data = reinterpret_cast<uint8 *>(_token.data());
		if (!SteamHTTP()->GetHTTPResponseBodyData(_request, data, response_size))
		{
			printf("AuthToken: response body read failed\n");
			return;
		}

		_token.insert(0, "Bearer ");
		_state = State::ready;
	}
};

struct wiDispatcher::Properties final
{
	explicit Properties(wiDispatcher &dispatcher) : dispatcher(dispatcher) {}

	wiDispatcher &dispatcher;
	std::string address;
	std::string endpoint;

	bool is_waiting_token = false;
	std::vector<uint8_t> request_data;
	HTTPRequestHandle request = 0;
	SteamAPICall_t call_handle = 0;
	CCallResult<wiDispatcher::Properties, HTTPRequestCompleted_t> request_complete;

	void on_request_complete(HTTPRequestCompleted_t *callback, bool io_failure)
	{
		if (io_failure || !callback->m_bRequestSuccessful)
		{
			dispatcher.set_error();
			return;
		}

		printf("wiDispatcher: request status code=%d\n", callback->m_eStatusCode);
		if (callback->m_eStatusCode != 200)
		{
			dispatcher.set_error();
			return;
		}

		uint32 response_size = 0;
		if (!SteamHTTP()->GetHTTPResponseBodySize(request, &response_size))
		{
			dispatcher.set_error();
			return;
		}

		dispatcher.alloc_inbuf(response_size);
		auto data = reinterpret_cast<uint8 *>(dispatcher.inBuffer);
		if (!SteamHTTP()->GetHTTPResponseBodyData(request, data, response_size))
		{
			dispatcher.set_error();
			return;
		}

		dispatcher.status = WI_FINISHED;
	}
};

/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */

/* --------------------------- DEFINITION SECTION --------------------------- */

const char *wiServerName = nullptr;
const char *wiGameURL = nullptr;
int wiServerPort = 8500;

wiDispatcher wi_D;

void *wiCritical = nullptr;

void wiInit() {}

void wiFinit() {}

wiDispatcher::wiDispatcher() : _properties(std::make_unique<Properties>(*this))
{
	flags = 0;
	status = WI_IDLE;

	hConnect = nullptr;
	hRequest = nullptr;

	inPos = inSize = 0;
	inBuffer = nullptr;

	outPos = outSize = 0;
 	outBuffer = nullptr;
}

wiDispatcher::~wiDispatcher()
{
	close_request();
	disconnect();

 	free_inbuf();
	free_outbuf();
}

void wiDispatcher::expand_inbuf(int sz)
{
	char *buf = nullptr;
	if (inSize < sz + 1)
	{
		buf = new char[sz + 1];
		memset(buf, 0, sz + 1);
		memcpy(buf, inBuffer, inSize);

		delete[] inBuffer;

		inBuffer = buf;
 		inSize = sz + 1;
	}
}

void wiDispatcher::alloc_inbuf(int sz)
{
	if (inSize < sz + 1)
	{
		free_inbuf();

		inSize = sz + 1;
		inBuffer = new char[sz + 1];
	}
	memset(inBuffer, 0, sz + 1);

 	inPos = 0;
}

void wiDispatcher::free_inbuf()
{
	if (inSize)
	{
		inSize = 0;
 		delete[] inBuffer;
	}
}

void wiDispatcher::alloc_outbuf(int sz)
{
	if(outSize < sz + 1)
	{
		free_outbuf();

		outSize = sz + 1;
		outBuffer = new char[sz + 1];
	}
	memset(outBuffer, 0, sz + 1);

	outPos = 0;
}

void wiDispatcher::free_outbuf()
{
	if (outSize)
	{
 		outSize = 0;
		delete[] outBuffer;
	}
}

int wiDispatcher::connect(const char *server, unsigned int port)
{
	printf("wiDispatcher: connecting to server=%s, port=%d\n", server, port);

	std::stringstream address;
	address << "http://" << server << ":" << port;
	_properties->address = address.str();

	AuthToken::get().set_endpoint(_properties->address);

	return 0;
}

void wiDispatcher::disconnect()
{
}

int wiDispatcher::open_request(
	int action,
	const char *object,
	const char *header,
	int header_length,
	const char *data,
	int data_length
)
{
	if (status != WI_IDLE)
	{
		close_request();
	}
	inPos = outPos = 0;

	_properties->endpoint = _properties->address + object;
	printf("wiDispatcher: opening request with endpoint=%s\n", _properties->endpoint.c_str());

	const EHTTPMethod method = action == WI_POST ? EHTTPMethod::k_EHTTPMethodPOST : EHTTPMethod::k_EHTTPMethodGET;
	_properties->request = SteamHTTP()->CreateHTTPRequest(method, _properties->endpoint.c_str());

	if (action == WI_POST && data_length > 0)
	{
		_properties->request_data.resize(data_length);
		_properties->request_data.assign(data, data + data_length);
		SteamHTTP()->SetHTTPRequestRawPostBody(
			_properties->request,
			"application/x-www-form-urlencoded",
			_properties->request_data.data(),
			_properties->request_data.size()
		);
	}

	_properties->is_waiting_token = true;
	AuthToken::get().request();

	status = action;
	flags &= ~WI_REQUEST_COMPLETED;

	return 1;
}

void wiDispatcher::close_request()
{
	if (_properties->request != INVALID_HTTPREQUEST_HANDLE)
	{
		SteamHTTP()->ReleaseHTTPRequest(_properties->request);
	}

	_properties->request = INVALID_HTTPREQUEST_HANDLE;
	flags &= ~WI_REQUEST_COMPLETED;
	status = WI_IDLE;
}

void wiDispatcher::quant()
{
	if (status == WI_LOAD_FILE)
	{
 		status = WI_FINISHED;
		return;
	}

	SteamAPI_RunCallbacks();

	if (_properties->is_waiting_token)
	{
		auto &auth_token = AuthToken::get();
		if (auth_token.get_state() != AuthToken::State::ready)
		{
			return;
		}

		_properties->is_waiting_token = false;
		auto token = auth_token.get_token().c_str();
		SteamHTTP()->SetHTTPRequestHeaderValue(_properties->request, "Authorization", token);
		if (SteamHTTP()->SendHTTPRequest(_properties->request, &_properties->call_handle))
		{
			_properties->request_complete.Set(
				_properties->call_handle,
				_properties.get(),
				&Properties::on_request_complete
			);
		}
		else
		{
			printf("wiDispatcher: failed to send request\n");
			set_error();
		}
	}
}

int wiDispatcher::get_request_status()
{
	return flags & WI_REQUEST_COMPLETED;
}

const char *wiDispatcher::get_request_status_str()
{
	return " ";
}

wiCriticalSection::wiCriticalSection(void *section) {}
wiCriticalSection::~wiCriticalSection(void) {}
