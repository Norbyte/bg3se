#include "stdafx.h"
#include "HttpFetcher.h"
#include "ErrorUtils.h"
#include <sstream>
#include <iomanip>


HttpFetcher::HttpFetcher(wchar_t const * host)
	: host_(host)
{
#if !defined(HTTP_CURL)
	DEBUG("Updater - connecting to update host %s", ToUTF8(host).c_str());
	session_ = WinHttpOpen(L"OsiLoader/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (session_ == NULL) {
		LogError("WinHttpOpen");
		return;
	}

	httpSession_ = WinHttpConnect(session_, host, INTERNET_DEFAULT_HTTPS_PORT, 0);
	if (httpSession_ == NULL) {
		LogError("WinHttpConnect");
		return;
	}
#endif
}

HttpFetcher::~HttpFetcher()
{
#if !defined(HTTP_CURL)
	if (httpSession_ != NULL) WinHttpCloseHandle(httpSession_);
	if (session_ != NULL) WinHttpCloseHandle(session_);
#endif
}


#if defined(HTTP_CURL)
void HttpFetcher::LogError(CURLcode result)
{
	std::stringstream ss;
	ss << "(" << result << ") " << curl_easy_strerror(result);
	lastError_ = ss.str();
	DEBUG("Updater error: %s", lastError_.c_str());
}
#else
void HttpFetcher::LogError(char const* activity)
{
	auto lastError = ::GetLastError();
	std::stringstream ss;
	ss << activity << " returned error 0x" << std::hex << std::setw(8) << std::setfill('0')
		<< lastError << ": " << GetLastErrorString(lastError);
	lastError_ = ss.str();
	DEBUG("Updater error: %s", lastError_.c_str());
}
#endif

bool HttpFetcher::Fetch(wchar_t const * path, std::vector<uint8_t> & response)
{
#if defined(HTTP_CURL)
	auto curl = curl_easy_init();
	std::wstring url = L"https://";
	url += host_;
	url += path;
	curl_easy_setopt(curl, CURLOPT_URL, ToUTF8(url).c_str());
	curl_easy_setopt(curl, CURLOPT_NOBODY, 0);
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
	curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteFunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	lastResponse_.clear();

	auto result = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	if (result != CURLE_OK) {
		LogError(result);
		return false;
	}

	response = lastResponse_;
	return true;
#else
	if (httpSession_ == NULL) {
		return false;
	}

	DEBUG("Fetching contents of %s", ToUTF8(path).c_str());
	auto request = WinHttpOpenRequest(httpSession_, L"GET", path, NULL,
		WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
	if (request == NULL) {
		LogError("WinHttpOpenRequest for content fetch");
		return false;
	}

	bool result = false;
	if (SendRequest(request, path)) {
		result = FetchBody(request, response);
	}

	WinHttpCloseHandle(request);
	return result;
#endif
}

size_t HttpFetcher::WriteFunc(char* contents, size_t size, size_t nmemb, HttpFetcher* self)
{
	auto pos = self->lastResponse_.size();
	self->lastResponse_.resize(self->lastResponse_.size() + size * nmemb);
	memcpy(self->lastResponse_.data() + pos, contents, size * nmemb);
	return size * nmemb;
}

bool HttpFetcher::FetchETag(wchar_t const * path, std::string & etag)
{
#if defined(HTTP_CURL)
	auto curl = curl_easy_init();
	std::wstring url = L"https://";
	url += host_;
	url += path;
	curl_easy_setopt(curl, CURLOPT_URL, ToUTF8(url).c_str());
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
	curl_easy_setopt(curl, CURLOPT_HEADER, 1);
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
	curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteFunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	lastResponse_.clear();

	auto result = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	if (result != CURLE_OK) {
		LogError(result);
		return false;
	}

	std::string responseStr;
	responseStr.resize(lastResponse_.size());
	memcpy(responseStr.data(), lastResponse_.data(), lastResponse_.size());

	auto etagPos = responseStr.find("\r\nETag: ");
	if (etagPos == std::string::npos) {
		lastError_ = "Couldn't find ETag header in HTTP response.";
		return false;
	}

	auto etagEnd = responseStr.find("\r\n", etagPos + 8);
	if (etagEnd == std::string::npos) {
		lastError_ = "Couldn't find ETag header in HTTP response.";
		return false;
	}

	etag = responseStr.substr(etagPos + 8, etagEnd - etagPos - 8);

	return true;
#else
	if (httpSession_ == NULL) {
		return false;
	}

	DEBUG("Fetching ETag of %s", ToUTF8(path).c_str());
	auto request = WinHttpOpenRequest(httpSession_, L"HEAD", path, NULL,
		WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
	if (request == NULL) {
		LogError("WinHttpOpenRequest for ETag fetch");
		return false;
	}

	bool result = false;
	if (SendRequest(request, path)) {
		result = FetchETag(request, etag);
	}

	WinHttpCloseHandle(request);
	return result;
#endif
}

#if !defined(HTTP_CURL)
bool HttpFetcher::SendRequest(HINTERNET request, wchar_t const * path)
{
	auto sent = WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, -1l,
		WINHTTP_NO_REQUEST_DATA, 0, 0, NULL);
	if (sent != TRUE) {
		LogError("WinHttpSendRequest");
		return false;
	}

	auto recvdHeaders = WinHttpReceiveResponse(request, NULL);
	if (recvdHeaders != TRUE) {
		LogError("WinHttpReceiveResponse");
		return false;
	}

	wchar_t statusCode[4];
	DWORD statusCodeLength = sizeof(statusCode);
	auto gotStatus = WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE,
		WINHTTP_HEADER_NAME_BY_INDEX, statusCode, &statusCodeLength, 0);
	if (gotStatus != TRUE) {
		LogError("WinHttpQueryHeaders for status code");
		return false;
	}

	DEBUG("HTTP status code %s", ToUTF8(statusCode).c_str());
	if (wcscmp(statusCode, L"200") != 0) {
		lastError_ = "Server returned status code ";
		lastError_ += ToUTF8(statusCode);
		return false;
	}

	return true;
}

bool HttpFetcher::FetchBody(HINTERNET request, std::vector<uint8_t> & response)
{
	wchar_t contentLengthStr[32];
	DWORD contentLengthLength = sizeof(contentLengthStr);
	auto gotLength = WinHttpQueryHeaders(request, WINHTTP_QUERY_CONTENT_LENGTH,
		NULL, contentLengthStr, &contentLengthLength, 0);
	if (gotLength != TRUE) {
		LogError("WinHttpQueryHeaders for length");
		return false;
	}

	auto contentLength = (uint32_t)std::stoi(contentLengthStr);
	if (contentLength > 0x1000000) {
		lastError_ = "HTTP response body too long";
		return false;
	}

	DEBUG("Payload size: %d bytes", contentLength);
	response.resize(contentLength);
	DWORD totalBytesRead = 0;

	while (totalBytesRead < contentLength) {
		DWORD bytesToRead = contentLength - totalBytesRead;
		DWORD bytesRead = 0;
		auto readBytes = WinHttpReadData(request, response.data() + totalBytesRead,
			bytesToRead, &bytesRead);
		if (readBytes != TRUE) {
			LogError("WinHttpReadData");
			return false;
		}

		totalBytesRead += bytesRead;
	}

	return true;
}

bool HttpFetcher::FetchETag(HINTERNET request, std::string & etag)
{
	wchar_t etagStr[0x100];
	DWORD etagLength = sizeof(etagStr);
	auto gotETag = WinHttpQueryHeaders(request, WINHTTP_QUERY_ETAG,
		NULL, etagStr, &etagLength, 0);
	if (gotETag != TRUE) {
		LogError("WinHttpQueryHeaders for ETag");
		return false;
	}

	etag = ToUTF8(etagStr);
	DEBUG("Fetched ETag: %s", etag.c_str());

	return true;
}
#endif
