#include "stdafx.h"
#include "HttpUploader.h"
#include <sstream>
#include <iomanip>


HttpUploader::HttpUploader()
{}

HttpUploader::~HttpUploader()
{}


void HttpUploader::LogError(CURL* curl, CURLcode result)
{
    std::stringstream ss;

    if (result == CURLE_HTTP_RETURNED_ERROR) {
        lastHttpCode_ = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &lastHttpCode_);
        ss << "HTTP error " << lastHttpCode_;
    } else {
        ss << "(" << (long)result << ") " << curl_easy_strerror(result);
    }

    lastError_ = ss.str();
}

bool HttpUploader::Upload(std::string const& url, std::vector<uint8_t> const & payload, std::vector<uint8_t> & response)
{
    auto curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOBODY, 0);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
    //curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)payload.size());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, &ReadFunc);
    curl_easy_setopt(curl, CURLOPT_READDATA, this);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

    lastResponse_.clear();
    payload_ = payload;
    payloadPos_ = 0;

    lastResult_ = curl_easy_perform(curl);
    if (lastResult_ != CURLE_OK) {
        LogError(curl, lastResult_);
    }

    curl_easy_cleanup(curl);
    response = lastResponse_;
    return (lastResult_ == CURLE_OK);
}

size_t HttpUploader::WriteFunc(char* contents, size_t size, size_t nmemb, HttpUploader* self)
{
    auto pos = self->lastResponse_.size();
    self->lastResponse_.resize(self->lastResponse_.size() + size * nmemb);
    memcpy(self->lastResponse_.data() + pos, contents, size * nmemb);
    return size * nmemb;
}

size_t HttpUploader::ReadFunc(char* contents, size_t size, size_t nmemb, HttpUploader* self)
{
    auto pos = self->payloadPos_;
    auto bytesToReturn = min(self->payload_.size() - pos, size * nmemb);
    memcpy(contents, self->payload_.data() + pos, bytesToReturn);
    self->payloadPos_ += bytesToReturn;
    return bytesToReturn;
}
