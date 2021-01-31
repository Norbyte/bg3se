#pragma once

#include <winhttp.h>
#include <vector>
#include <string>
#include <curl/curl.h>

// Switch to toggle between WinHTTP and cURL engines
#define HTTP_CURL

class HttpFetcher
{
public:
	HttpFetcher(wchar_t const * host);
	~HttpFetcher();

	bool Fetch(wchar_t const * path, std::vector<uint8_t> & response);
	bool FetchETag(wchar_t const * path, std::string & etag);

	inline std::string const & GetLastError() const
	{
		return lastError_;
	}

private:
#if defined(HTTP_CURL)
	wchar_t const* host_;
#else
	HINTERNET session_{ NULL };
	HINTERNET httpSession_{ NULL };
#endif
	std::string lastError_;
	std::vector<uint8_t> lastResponse_;

#if defined(HTTP_CURL)
	void LogError(CURLcode result);
	static size_t WriteFunc(char* contents, size_t size, size_t nmemb, HttpFetcher* self);
#else
	void LogError(char const * activity);
	bool SendRequest(HINTERNET request, wchar_t const * path);

	bool FetchBody(HINTERNET request, std::vector<uint8_t> & response);
	bool FetchETag(HINTERNET request, std::string & etag);
#endif
};
