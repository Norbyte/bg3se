#pragma once

#include <vector>
#include <string>
#include <curl/curl.h>

class HttpFetcher
{
public:
	HttpFetcher();
	~HttpFetcher();

	bool Fetch(std::string const& url, std::vector<uint8_t> & response);

	inline std::string const & GetLastError() const
	{
		return lastError_;
	}

private:
	std::string lastError_;
	std::vector<uint8_t> lastResponse_;

	void LogError(CURL* curl, CURLcode result);
	static size_t WriteFunc(char* contents, size_t size, size_t nmemb, HttpFetcher* self);
};
