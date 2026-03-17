#include "stdafx.h"
#include "HttpFetcher.h"
#include "GameHelpers.h"
#include <iomanip>

BEGIN_SE()

HttpFetcher::HttpFetcher()
{}

HttpFetcher::~HttpFetcher()
{
    Cleanup();
}

void HttpFetcher::Cleanup()
{
    if (curl_ != nullptr) {
        curl_easy_cleanup(curl_);
        curl_ = nullptr;
    }
}

void HttpFetcher::LogError(CURL* curl, CURLcode result)
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
    DEBUG("  Fetch failed: %s", lastError_.c_str());
}

OperationResult HttpFetcher::Fetch(std::string const& url, std::vector<char> & response)
{
    cancelling_ = false;
    socket_ = NULL;

    if (curl_ == NULL) {
        curl_ = curl_easy_init();
    }

    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_, CURLOPT_NOBODY, 0l);
    curl_easy_setopt(curl_, CURLOPT_HEADER, 0l);
    curl_easy_setopt(curl_, CURLOPT_FAILONERROR, 1l);
    curl_easy_setopt(curl_, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
    curl_easy_setopt(curl_, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA | CURLSSLOPT_REVOKE_BEST_EFFORT);

    if (Timeout) {
        curl_easy_setopt(curl_, CURLOPT_TIMEOUT_MS, *Timeout);
        curl_easy_setopt(curl_, CURLOPT_CONNECTTIMEOUT_MS, std::min(*Timeout, ConnectionTimeout));
    } else {
        curl_easy_setopt(curl_, CURLOPT_TIMEOUT_MS, 0);
        curl_easy_setopt(curl_, CURLOPT_CONNECTTIMEOUT_MS, ConnectionTimeout);
    }

    if (DebugLogging) {
        curl_easy_setopt(curl_, CURLOPT_VERBOSE, 1l);
        curl_easy_setopt(curl_, CURLOPT_DEBUGFUNCTION, &DebugFunc);
    }

    if (IPv4Only) {
        curl_easy_setopt(curl_, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    }

    curl_easy_setopt(curl_, CURLOPT_XFERINFOFUNCTION, &XferInfoFunc);
    curl_easy_setopt(curl_, CURLOPT_XFERINFODATA, this);
    curl_easy_setopt(curl_, CURLOPT_OPENSOCKETFUNCTION, &OpenSocketFunc);
    curl_easy_setopt(curl_, CURLOPT_OPENSOCKETDATA, &this->socket_);
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &WriteFunc);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, this);
    lastResponse_.clear();

    DEBUG("Start cURL fetch for URL %s", url.c_str());
    DEBUG("  (Debug %s, IPv4 only %s)", (DebugLogging ? "on" : "off"), (IPv4Only ? "on" : "off"));

    lastResult_ = curl_easy_perform(curl_);
    if (lastResult_ != CURLE_OK) {
        LogError(curl_, lastResult_);
    } else {
        DEBUG("  Fetch succeeded (%lld bytes)", lastResponse_.size());
    }

    response = lastResponse_;
    if (lastResult_ == CURLE_OK) {
        return OperationSuccessful{};
    } else {
        return ErrorReason{ TransferCategory, lastResult_, lastError_ };
    }
}

size_t HttpFetcher::XferInfoFunc(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    DEBUG("XferInfo: %lld/%lld", dlnow, dltotal);
    auto self = reinterpret_cast<HttpFetcher*>(clientp);
    if (self->cancelling_) {
        return 1;
    } else {
        return 0;
    }
}

void HttpFetcher::Cancel()
{
    cancelling_ = true;
    if (socket_ != NULL) {
        shutdown(socket_, SD_BOTH);
        closesocket(socket_);
    }
}

curl_socket_t HttpFetcher::OpenSocketFunc(SOCKET* data, curlsocktype purpose, struct curl_sockaddr* addr)
{
    auto sock = socket(addr->family, addr->socktype, addr->protocol);
    *data = sock;
    return sock;
}

size_t HttpFetcher::WriteFunc(char* contents, size_t size, size_t nmemb, HttpFetcher* self)
{
    auto pos = self->lastResponse_.size();
    self->lastResponse_.resize(self->lastResponse_.size() + size * nmemb);
    memcpy(self->lastResponse_.data() + pos, contents, size * nmemb);
    return size * nmemb;
}

int HttpFetcher::DebugFunc(CURL* handle, curl_infotype type, char* data, size_t size, void* clientp)
{
    std::string line;
    switch (type) {
    case CURLINFO_TEXT: line = "* "; line += std::string_view(data, size - 1); break;
    case CURLINFO_HEADER_IN: line = "< "; line += std::string_view(data, size - 2); break;
    case CURLINFO_HEADER_OUT: line = "> "; line += std::string_view(data, size - 2); break;
    default: return 0;
    }

    DEBUG("%s", line.c_str());

    return 0;
}

END_SE()
