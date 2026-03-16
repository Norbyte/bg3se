#include "Result.h"
#include <curl/curl.h>

BEGIN_SE()

bool ErrorReason::IsInternetIssue() const
{
    if (CurlResult) {
        auto res = (CURLcode)*CurlResult;
        return
            res == CURLE_COULDNT_RESOLVE_PROXY
            || res == CURLE_COULDNT_RESOLVE_HOST
            || res == CURLE_COULDNT_CONNECT
            || res == CURLE_WEIRD_SERVER_REPLY
            || res == CURLE_OPERATION_TIMEDOUT;
    } else {
        return false;
    }
}

END_SE()
