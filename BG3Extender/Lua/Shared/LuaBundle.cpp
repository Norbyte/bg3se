#include <stdafx.h>
#include <Lua/Shared/LuaBundle.h>
#include <filesystem>

BEGIN_NS(lua)

void LuaBundle::SetResourcePath(std::wstring const& path)
{
    resourcePath_ = path;
}

bool LuaBundle::LoadBuiltinResource(int resourceId)
{
    auto res = GetExeResource(resourceId);
    if (res) {
        LoadBuffer(std::span((uint8_t*)res->data(), res->size()));
        return true;
    } else {
        return false;
    }
}

void LuaBundle::LoadBuffer(std::span<uint8_t> const& buf)
{
    std::size_t offset = 0;
    while (offset < buf.size()) {
        auto hdr = reinterpret_cast<ResourceHeader*>(buf.data() + offset);
        STDString path((char const*)buf.data() + offset + sizeof(ResourceHeader), hdr->FileNameSize);
        STDString body((char const*)buf.data() + offset + sizeof(ResourceHeader) + hdr->FileNameSize, hdr->FileSize);
        resources_.insert(std::make_pair(path, body));
        offset += sizeof(ResourceHeader) + hdr->FileNameSize + hdr->FileSize;
    }
}

std::optional<STDString> LuaBundle::GetResource(STDString const& path) const
{
    if (!resourcePath_.empty()) {
        auto resPath = resourcePath_ + L"/" + FromUTF8(path).c_str();
        std::ifstream f(resPath.c_str(), std::ios::in | std::ios::binary);
        if (f.good()) {
            STDString body;
            f.seekg(0, std::ios::end);
            body.resize((uint32_t)f.tellg());
            f.seekg(0, std::ios::beg);
            f.read(body.data(), body.size());
            return body;
        }
    }

    auto it = resources_.find(path);
    if (it != resources_.end()) {
        return it->second;
    } else {
        return {};
    }
}

END_NS()
