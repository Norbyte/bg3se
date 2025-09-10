#include <stdafx.h>
#include "Manifest.h"
#include <CoreLib/Crypto.h>

BEGIN_SE()

using namespace rapidjson;

std::optional<VersionNumber> GetFileVersion(std::wstring const& path)
{
    DWORD versionSize = GetFileVersionInfoSizeW(path.c_str(), 0);
    if (!versionSize) {
        std::cout << GetLastError() << std::endl;
        return {};
    }

    std::vector<uint8_t> versionInfo;
    versionInfo.resize(versionSize);

    if (GetFileVersionInfoW(path.c_str(), 0, versionSize, versionInfo.data()) != TRUE) {
        return {};
    }

    UINT verLength{ 0 };
    VS_FIXEDFILEINFO* fixedFileInfo{ nullptr };
    if (VerQueryValueW(versionInfo.data(), L"\\", (LPVOID*)&fixedFileInfo, &verLength) != TRUE) {
        return {};
    }

    return VersionNumber(
        HIWORD(fixedFileInfo->dwFileVersionMS),
        LOWORD(fixedFileInfo->dwFileVersionMS),
        HIWORD(fixedFileInfo->dwFileVersionLS),
        LOWORD(fixedFileInfo->dwFileVersionLS));
}

std::optional<std::string> GetFileDigest(std::wstring const& path)
{
    std::vector<uint8_t> contents;
    if (!LoadFile(path, contents)) {
        return {};
    }

    uint8_t digest[TC_SHA256_DIGEST_SIZE];
    CryptoUtils::SHA256(contents.data(), contents.size(), digest);

    static char const* hex = "0123456789abcdef";

    std::string digestStr;
    for (auto i = 0; i < std::size(digest); i++) {
        digestStr += hex[digest[i] >> 4];
        digestStr += hex[digest[i] & 0x0f];
    }

    return digestStr;
}

bool Manifest::ResourceVersion::UpdatePackageMetadata(std::wstring const& path)
{
    auto digest = GetFileDigest(path);
    if (!digest) {
        return false;
    }

    Digest = *digest;

    auto hFile = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    FILETIME updateTime;
    if (!GetFileTime(hFile, NULL, NULL, &updateTime)) {
        return false;
    }

    CloseHandle(hFile);

    PackageSignature sig;
    if (!CryptoUtils::GetFileSignature(path, sig)) {
        return false;
    }

    Signature = "";
    for (auto p = 0; p < sizeof(PackageSignatureBase); p++) {
        auto b = reinterpret_cast<uint8_t *>(&sig)[p];
        char hex[4];
        sprintf_s(hex, "%02x", (unsigned)b);
        Signature += hex;
    }

    BuildDate = (uint64_t)updateTime.dwLowDateTime | ((uint64_t)updateTime.dwHighDateTime << 32);

    return true;
}

bool Manifest::ResourceVersion::UpdateDLLMetadata(std::wstring const& path)
{
    auto fileVer = GetFileVersion(path);
    if (!fileVer) {
        return false;
    }

    Version = *fileVer;
    return true;
}


std::optional<Manifest::ResourceVersion> Manifest::Resource::FindResourceVersionWithOverrides(VersionNumber const& gameVersion,
    UpdaterConfig const& config) const
{
    std::optional<Manifest::ResourceVersion> version;
    if (!config.TargetResourceDigest.empty()) {
        auto found = ResourceVersions.find(config.TargetResourceDigest);
        if (found != ResourceVersions.end()) {
            return found->second;
        } else {
            return {};
        }
    }

    if (!config.TargetVersion.empty()) {
        auto resourceVersion = VersionNumber::FromString(config.TargetVersion.c_str());
        return FindResourceVersion(gameVersion, resourceVersion);
    }

    return FindResourceVersion(gameVersion, {});
}

std::optional<Manifest::ResourceVersion> Manifest::Resource::FindResourceVersion(VersionNumber const& gameVersion,
    std::optional<VersionNumber> resourceVersion) const
{
    std::vector<ResourceVersion> availableVersions;
    for (auto const& ver : ResourceVersions) {
        if (!ver.second.Revoked
            && (!resourceVersion || ver.second.Version == *resourceVersion)
            && !(ver.second.MinGameVersion && *ver.second.MinGameVersion > gameVersion)
            && !(ver.second.MaxGameVersion && *ver.second.MaxGameVersion < gameVersion)) {
            availableVersions.push_back(ver.second);
        }
    }

    std::sort(availableVersions.begin(), availableVersions.end(), [](ResourceVersion const& a, ResourceVersion const& b) {
        if (a.MinGameVersion && b.MinGameVersion && a.MinGameVersion != b.MinGameVersion) {
            return a.MinGameVersion < b.MinGameVersion;
        }

        return a.BuildDate < b.BuildDate;
    });

    if (availableVersions.empty()) {
        return {};
    } else {
        return *availableVersions.rbegin();
    }
}

std::optional<bool> GetBoolProperty(Value const& node, std::string_view key, std::optional<bool> defaultVal = {})
{
    auto attr = node.FindMember(key.data());
    if (attr == node.MemberEnd() || !attr->value.IsBool()) {
        return defaultVal;
    }

    return attr->value.GetBool();
}

std::optional<int> GetIntProperty(Value const& node, std::string_view key, std::optional<int> defaultVal = {})
{
    auto attr = node.FindMember(key.data());
    if (attr == node.MemberEnd() || !attr->value.IsInt()) {
        return defaultVal;
    }

    return attr->value.GetInt();
}

std::optional<int64_t> GetInt64Property(Value const& node, std::string_view key, std::optional<int64_t> defaultVal = {})
{
    auto attr = node.FindMember(key.data());
    if (attr == node.MemberEnd() || !attr->value.IsInt64()) {
        return defaultVal;
    }

    return attr->value.GetInt64();
}

std::optional<std::string_view> GetStringProperty(Value const& node, std::string_view key, std::optional<std::string_view> defaultVal = {})
{
    auto attr = node.FindMember(key.data());
    if (attr == node.MemberEnd() || !attr->value.IsString()) {
        return defaultVal;
    }

    return std::string_view(attr->value.GetString(), attr->value.GetStringLength());
}

ManifestParseResult ManifestSerializer::Parse(std::string const& json, Manifest& manifest, std::string& parseError)
{
    Document root;
    if (root.Parse(json.data(), json.size()).HasParseError()) {
        parseError = "Unable to parse JSON";
        return ManifestParseResult::Failed;
    }

    manifest.Resources.clear();

    auto version = GetIntProperty(root, "ManifestVersion");
    if (!version) {
        parseError = "Manifest has no 'ManifestVersion' property";
        return ManifestParseResult::Failed;
    }

    if (*version != Manifest::CurrentVersion) {
        parseError = "Expected manifest version 1; got ";
        parseError += std::to_string(*version);
        if (*version > Manifest::CurrentVersion) {
            return ManifestParseResult::UpdateRequired;
        } else {
            return ManifestParseResult::Failed;
        }
    }

    manifest.ManifestVersion = *version;
    manifest.ManifestMinorVersion = *GetIntProperty(root, "ManifestMinorVersion", 0);
    manifest.Notice = *GetStringProperty(root, "Notice", "");
    manifest.NoMatchingVersionNotice = *GetStringProperty(root, "NoMatchingVersionNotice", "");

    if (Parse(root, manifest, parseError)) {
        return ManifestParseResult::Successful;
    } else {
        return ManifestParseResult::Failed;
    }
}

bool ManifestSerializer::Parse(Value const& node, Manifest& manifest, std::string& parseError)
{
    manifest.Resources.clear();

    auto resources = node.FindMember("Resources");
    if (resources == node.MemberEnd() || !resources->value.IsArray()) {
        parseError = "Manifest has no 'Resources' array";
        return false;
    }

    for (auto const& resourceNode : resources->value.GetArray()) {
        if (!resourceNode.IsObject()) {
            parseError = "Bundle info is not an object";
            return false;
        }

        Manifest::Resource resource;
        if (!ParseResource(resourceNode, resource, parseError)) {
            return false;
        }

        manifest.Resources.insert(std::make_pair(resource.Name, resource));
    }

    return true;
}

bool ManifestSerializer::ParseResource(Value const& node, Manifest::Resource& resource, std::string& parseError)
{
    resource.Name = *GetStringProperty(node, "Name", "");

    auto versions = node.FindMember("Versions");
    if (versions == node.MemberEnd() || !versions->value.IsArray()) {
        parseError = "Manifest resource has no 'Versions' array";
        return false;
    }

    for (auto const& versionNode : versions->value.GetArray()) {
        if (!versionNode.IsObject()) {
            parseError = "Bundle version info is not an object";
            return false;
        }

        Manifest::ResourceVersion version;
        if (!ParseVersion(versionNode, version, parseError)) {
            return false;
        }

        resource.ResourceVersions.insert(std::make_pair(version.Digest, version));
    }

    return true;
}

bool ManifestSerializer::ParseVersion(Value const& node, Manifest::ResourceVersion& version, std::string& parseError)
{
    auto minGameVersion = GetStringProperty(node, "MinGameVersion");
    if (minGameVersion) {
        auto minVersion = VersionNumber::FromString(minGameVersion->data());
        if (!minVersion) {
            parseError = "Unable to parse 'MinGameVersion'.";
            return false;
        }

        version.MinGameVersion = minVersion;
    }

    auto maxGameVersion = GetStringProperty(node, "MaxGameVersion");
    if (maxGameVersion) {
        auto maxVersion = VersionNumber::FromString(maxGameVersion->data());
        if (!maxVersion) {
            parseError = "Unable to parse 'MaxGameVersion'.";
            return false;
        }

        version.MaxGameVersion = maxVersion;
    }

    auto resVersion = VersionNumber::FromString(GetStringProperty(node, "Version", "")->data());
    if (!resVersion) {
        parseError = "Unable to parse 'Version'.";
        return false;
    }

    version.Version = *resVersion;

    version.URL = *GetStringProperty(node, "URL", "");
    version.Digest = *GetStringProperty(node, "Digest", "");
    version.BuildDate = (uint64_t)*GetInt64Property(node, "BuildDate", 0);
    version.Revoked = *GetBoolProperty(node, "Revoked", false);
    version.Signature = *GetStringProperty(node, "Signature", "");
    version.Notice = *GetStringProperty(node, "Notice", "");
    return true;
}

std::string ManifestSerializer::Stringify(Manifest& manifest)
{
    Document doc;
    auto& alloc = doc.GetAllocator();

    Value resources{ kObjectType };
    for (auto const& res : manifest.Resources) {
        Value jsonRes{ kObjectType };
        jsonRes.AddMember("Name", res.first, alloc);

        Value versions{ kArrayType };
        for (auto const& ver : res.second.ResourceVersions) {
            Value jsonVer{ kObjectType };
            if (ver.second.MinGameVersion) {
                jsonVer.AddMember("MinGameVersion", ver.second.MinGameVersion->ToString(), alloc);
            }

            if (ver.second.MaxGameVersion) {
                jsonVer.AddMember("MaxGameVersion", ver.second.MaxGameVersion->ToString(), alloc);
            }

            jsonVer.AddMember("Version", ver.second.Version.ToString(), alloc);
            jsonVer.AddMember("URL", ver.second.URL, alloc);
            jsonVer.AddMember("Digest", ver.second.Digest, alloc);
            jsonVer.AddMember("BuildDate", ver.second.BuildDate, alloc);

            if (ver.second.Revoked) {
                jsonVer.AddMember("BuildDate", true, alloc);
            }

            if (!ver.second.Signature.empty()) {
                jsonVer.AddMember("Signature", ver.second.Signature, alloc);
            }

            if (!ver.second.Notice.empty()) {
                jsonVer.AddMember("Notice", ver.second.Notice, alloc);
            }

            versions.PushBack(jsonVer, alloc);
        }

        jsonRes.AddMember("Versions", versions, alloc);
        resources.PushBack(jsonRes, alloc);
    }

    doc.AddMember("ManifestVersion", Value(manifest.ManifestVersion), alloc);
    doc.AddMember("ManifestMinorVersion", manifest.ManifestMinorVersion, alloc);

    if (!manifest.Notice.empty()) {
        doc.AddMember("Notice", Value(manifest.Notice.c_str(), alloc), alloc);
    }

    if (!manifest.NoMatchingVersionNotice.empty()) {
        doc.AddMember("NoMatchingVersionNotice", Value(manifest.NoMatchingVersionNotice.c_str(), alloc), alloc);
    }

    doc.AddMember("Resources", resources, alloc);

    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    doc.Accept(writer);

    std::ostringstream os;
    os.write(sb.GetString(), sb.GetLength());
    return os.str();
}

END_SE()
