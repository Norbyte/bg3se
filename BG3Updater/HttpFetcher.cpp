#include "stdafx.h"
#include "HttpFetcher.h"
#include "ErrorUtils.h"
#include <sstream>
#include <iomanip>


HttpFetcher::HttpFetcher(std::wstring const& host)
	: host_(host)
{}

HttpFetcher::~HttpFetcher()
{}

void HttpFetcher::LogError(CURL* curl, CURLcode result)
{
	std::stringstream ss;

	if (result == CURLE_HTTP_RETURNED_ERROR) {
		long httpCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
		ss << "HTTP error " << httpCode;
	} else {
		ss << "(" << result << ") " << curl_easy_strerror(result);
	}

	lastError_ = ss.str();
	DEBUG("Updater error: %s", lastError_.c_str());
}

bool HttpFetcher::Fetch(wchar_t const * path, std::vector<uint8_t> & response)
{
	auto curl = curl_easy_init();
	std::wstring url = L"https://";
	url += host_;
	url += path;
	curl_easy_setopt(curl, CURLOPT_URL, ToUTF8(url).c_str());
	curl_easy_setopt(curl, CURLOPT_NOBODY, 0);
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
	//curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteFunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	lastResponse_.clear();

	auto result = curl_easy_perform(curl);
	if (result != CURLE_OK) {
		LogError(curl, result);
	}

	curl_easy_cleanup(curl);
	response = lastResponse_;
	return (result == CURLE_OK);
}

size_t HttpFetcher::WriteFunc(char* contents, size_t size, size_t nmemb, HttpFetcher* self)
{
	auto pos = self->lastResponse_.size();
	self->lastResponse_.resize(self->lastResponse_.size() + size * nmemb);
	memcpy(self->lastResponse_.data() + pos, contents, size * nmemb);
	return size * nmemb;
}

