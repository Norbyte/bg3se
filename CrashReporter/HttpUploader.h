#pragma once

#include <winhttp.h>
#include <vector>
#include <string>

class HttpUploader
{
public:
	HttpUploader(wchar_t const * host);
	~HttpUploader();

	bool Upload(wchar_t const * path, std::vector<uint8_t> const & request, std::vector<uint8_t> & response);

	inline std::string const & GetLastError() const
	{
		return lastError_;
	}

private:
	HINTERNET session_{ NULL };
	HINTERNET httpSession_{ NULL };
	std::string lastError_;

	void LogError(char const * activity);
	bool SendRequest(HINTERNET request, wchar_t const * path, std::vector<uint8_t> const & payload);
	bool FetchBody(HINTERNET request, std::vector<uint8_t> & response);
};
