#pragma once

#include <vector>
#include <string>
#include <curl/curl.h>

class HttpUploader
{
public:
    HttpUploader();
    ~HttpUploader();

    bool Upload(std::string const& url, std::vector<uint8_t> const & request, std::vector<uint8_t> & response);

    inline CURLcode GetLastResultCode() const
    {
        return lastResult_;
    }

    inline std::string const& GetLastError() const
    {
        return lastError_;
    }

private:
    std::string lastError_;
    std::vector<uint8_t> payload_;
    std::size_t payloadPos_{ 0 };
    std::vector<uint8_t> lastResponse_;
    long lastHttpCode_{ 0 };
    CURLcode lastResult_{ CURLE_OK };

    void LogError(CURL* curl, CURLcode result);
    static size_t WriteFunc(char* contents, size_t size, size_t nmemb, HttpUploader* self);
    static size_t ReadFunc(char* contents, size_t size, size_t nmemb, HttpUploader* self);
};
