#pragma once

#include <string>
#include <optional>

BEGIN_SE()

enum class ErrorCategory
{
    General,
    ManifestFetch,
    UpdateDownload,
    LocalUpdate,
    LocalLoad,
    NoMatchingVersion,
    UpdateRequired,
    Canceled
};

enum class OperationSuccessful {};

struct ErrorReason
{
    inline ErrorReason() {}

    inline ErrorReason(std::string const& msg)
        : Message(msg)
    {}

    inline ErrorReason(ErrorCategory category, std::string const& msg)
        : Message(msg),
        Category(category)
    {}

    inline ErrorReason(ErrorCategory category, int32_t curlResult, std::string const& msg)
        : Message(msg),
        Category(category),
        CurlResult(curlResult)
    {}

    std::string Message;
    ErrorCategory Category{ ErrorCategory::General };
    std::optional<int32_t> CurlResult;

    bool IsInternetIssue() const;
};

class OperationResult
{
public:
    inline OperationResult(OperationSuccessful) {}
    inline OperationResult(ErrorReason const& e)
        : error_(e)
    {}

    inline operator bool() const
    {
        return !error_.has_value();
    }

    inline bool operator !() const
    {
        return error_.has_value();
    }

    inline ErrorReason& error()
    {
        return *error_;
    }

    inline ErrorReason const& error() const
    {
        return *error_;
    }

private:
    std::optional<ErrorReason> error_;
};

END_SE()
