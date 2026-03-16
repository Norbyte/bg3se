#pragma once

#include <vector>
#include <string>
#include <curl/curl.h>
#include "Result.h"

BEGIN_SE()

class HttpFetcher
{
public:
    bool DebugLogging{ false };
    bool IPv4Only{ false };
    ErrorCategory TransferCategory{ ErrorCategory::General };

    HttpFetcher();
    ~HttpFetcher();

    OperationResult Fetch(std::string const& url, std::vector<char> & response);
    void Cancel();

private:
    std::string lastError_;
    std::vector<char> lastResponse_;
    long lastHttpCode_{ 0 };
    CURLcode lastResult_{ CURLE_OK };
    CURL* curl_{ NULL };
    SOCKET socket_{ NULL };
    bool cancelling_{ false };

    void LogError(CURL* curl, CURLcode result);
    static size_t WriteFunc(char* contents, size_t size, size_t nmemb, HttpFetcher* self);
    static size_t XferInfoFunc(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
    static curl_socket_t OpenSocketFunc(SOCKET* data, curlsocktype purpose, struct curl_sockaddr* addr);
    static int DebugFunc(CURL* handle, curl_infotype type, char* data, size_t size, void* clientp);
};

END_SE()
