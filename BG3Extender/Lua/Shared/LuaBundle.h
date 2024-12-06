#pragma once

#include <GameDefinitions/Base/Base.h>
#include <unordered_map>
#include <span>
#include <vector>

BEGIN_NS(lua)

class LuaBundle
{
public:
    void SetResourcePath(std::wstring const& path);
    bool LoadBuiltinResource(int resourceId);
    void LoadBuffer(std::span<uint8_t> const& buf);

    std::optional<STDString> GetResource(STDString const& path) const;

private:
    std::unordered_map<STDString, STDString> resources_;
    std::wstring resourcePath_;

    struct ResourceHeader
    {
        uint32_t FileNameSize;
        uint32_t FileSize;
    };
};

END_NS()
