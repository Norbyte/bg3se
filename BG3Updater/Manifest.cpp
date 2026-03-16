#include <stdafx.h>
#include "Manifest.h"
#include <CoreLib/Crypto.h>

#include "Result.inl"

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


std::optional<Manifest::ResourceVersion> Manifest::FindResourceVersionWithOverrides(std::string const& resourceName,
    VersionNumber const& gameVersion, UpdaterConfig const& config) const
{
    auto resIt = Resources.find(resourceName);
    if (resIt == Resources.end()) {
        return {};
    }

    return resIt->second.FindResourceVersionWithOverrides(gameVersion, config);
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

    if (config.TargetVersion) {
        return FindResourceVersion(gameVersion, config.TargetVersion);
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

    std::sort(availableVersions.begin(), availableVersions.end());

    if (availableVersions.empty()) {
        return {};
    } else {
        return *availableVersions.rbegin();
    }
}

bool GetBoolProperty(Value const& node, std::string_view key, bool defaultVal = false)
{
    auto attr = node.FindMember(key.data());
    if (attr == node.MemberEnd() || !attr->value.IsBool()) {
        return defaultVal;
    }

    return attr->value.GetBool();
}

int GetIntProperty(Value const& node, std::string_view key, int defaultVal = 0)
{
    auto attr = node.FindMember(key.data());
    if (attr == node.MemberEnd() || !attr->value.IsInt()) {
        return defaultVal;
    }

    return attr->value.GetInt();
}

int64_t GetInt64Property(Value const& node, std::string_view key, int64_t defaultVal = 0)
{
    auto attr = node.FindMember(key.data());
    if (attr == node.MemberEnd() || !attr->value.IsInt64()) {
        return defaultVal;
    }

    return attr->value.GetInt64();
}

std::string_view GetStringProperty(Value const& node, std::string_view key, std::string_view defaultVal = {})
{
    auto attr = node.FindMember(key.data());
    if (attr == node.MemberEnd() || !attr->value.IsString()) {
        return defaultVal;
    }

    return std::string_view(attr->value.GetString(), attr->value.GetStringLength());
}

OperationResult ManifestSerializer::Parse(std::string_view json, Manifest& manifest)
{
    Document root;
    if (root.Parse(json.data(), json.size()).HasParseError()) {
        return ErrorReason{ "Unable to parse JSON" };
    }

    manifest.Resources.clear();

    auto version = GetIntProperty(root, "ManifestVersion");
    if (!version) {
        return ErrorReason{ "Manifest has no 'ManifestVersion' property" };
    }

    if (version != Manifest::CurrentVersion) {
        return ErrorReason{
            (version > Manifest::CurrentVersion) ? ErrorCategory::UpdateRequired : ErrorCategory::General,
            std::string("Expected manifest version 1; got ") + std::to_string(version)
        };
    }

    manifest.ManifestVersion = version;
    manifest.ManifestMinorVersion = GetIntProperty(root, "ManifestMinorVersion", 0);
    manifest.Partial = GetBoolProperty(root, "Partial", false);
    manifest.Notice = GetStringProperty(root, "Notice", "");
    manifest.NoMatchingVersionNotice = GetStringProperty(root, "NoMatchingVersionNotice", "");

    return Parse(root, manifest);
}

OperationResult ManifestSerializer::Parse(Value const& node, Manifest& manifest)
{
    manifest.Resources.clear();

    auto resources = node.FindMember("Resources");
    if (resources == node.MemberEnd() || !resources->value.IsArray()) {
        return ErrorReason{ "Manifest has no 'Resources' array" };
    }

    for (auto const& resourceNode : resources->value.GetArray()) {
        if (!resourceNode.IsObject()) {
            return ErrorReason{ "Bundle info is not an object" };
        }

        Manifest::Resource resource;
        auto result = ParseResource(resourceNode, resource);
        if (!result) {
            return result;
        }

        manifest.Resources.insert(std::make_pair(resource.Name, resource));
    }

    return OperationSuccessful{};
}

OperationResult ManifestSerializer::ParseResource(Value const& node, Manifest::Resource& resource)
{
    resource.Name = GetStringProperty(node, "Name", "");

    auto versions = node.FindMember("Versions");
    if (versions == node.MemberEnd() || !versions->value.IsArray()) {
        return ErrorReason{ "Manifest resource has no 'Versions' array" };
    }

    for (auto const& versionNode : versions->value.GetArray()) {
        if (!versionNode.IsObject()) {
            return ErrorReason{ "Bundle version info is not an object" };
        }

        Manifest::ResourceVersion version;
        auto result = ParseVersion(versionNode, version);
        if (!result) {
            return result;
        }

        resource.ResourceVersions.insert(std::make_pair(version.Digest, version));
    }

    return OperationSuccessful{};
}

OperationResult ManifestSerializer::ParseVersion(Value const& node, Manifest::ResourceVersion& version)
{
    auto minGameVersion = GetStringProperty(node, "MinGameVersion");
    if (!minGameVersion.empty()) {
        auto minVersion = VersionNumber::FromString(minGameVersion.data());
        if (!minVersion) {
            return ErrorReason{ "Unable to parse resource 'MinGameVersion'." };
        }

        version.MinGameVersion = minVersion;
    }

    auto maxGameVersion = GetStringProperty(node, "MaxGameVersion");
    if (!maxGameVersion.empty()) {
        auto maxVersion = VersionNumber::FromString(maxGameVersion.data());
        if (!maxVersion) {
            return ErrorReason{ "Unable to parse resource 'MaxGameVersion'." };
        }

        version.MaxGameVersion = maxVersion;
    }

    auto resVersion = VersionNumber::FromString(GetStringProperty(node, "Version", "").data());
    if (!resVersion) {
        return ErrorReason{ "Unable to parse resource 'Version'." };
    }

    version.Version = *resVersion;

    version.URL = GetStringProperty(node, "URL", "");
    version.Digest = GetStringProperty(node, "Digest", "");
    version.BuildDate = (uint64_t)GetInt64Property(node, "BuildDate", 0);
    version.Revoked = GetBoolProperty(node, "Revoked", false);
    version.Signature = GetStringProperty(node, "Signature", "");
    version.Notice = GetStringProperty(node, "Notice", "");
    return OperationSuccessful{};
}

std::string ManifestSerializer::Stringify(Manifest const& manifest)
{
    Document doc{ kObjectType };
    auto& alloc = doc.GetAllocator();

    Stringify(doc, manifest, alloc);

    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    doc.Accept(writer);

    std::ostringstream os;
    os.write(sb.GetString(), sb.GetLength());
    return os.str();
}

void ManifestSerializer::Stringify(rapidjson::Value& doc, Manifest const& manifest, RAPIDJSON_DEFAULT_ALLOCATOR& alloc)
{
    Value resources{ kArrayType };
    for (auto const& res : manifest.Resources) {
        Stringify(resources, res.second, alloc);
    }

    doc.AddMember("ManifestVersion", Value(manifest.ManifestVersion), alloc);
    doc.AddMember("ManifestMinorVersion", manifest.ManifestMinorVersion, alloc);
    doc.AddMember("Partial", manifest.Partial, alloc);

    if (!manifest.Notice.empty()) {
        doc.AddMember("Notice", Value(manifest.Notice.c_str(), alloc), alloc);
    }

    if (!manifest.NoMatchingVersionNotice.empty()) {
        doc.AddMember("NoMatchingVersionNotice", Value(manifest.NoMatchingVersionNotice.c_str(), alloc), alloc);
    }

    doc.AddMember("Resources", resources, alloc);
}

void ManifestSerializer::Stringify(rapidjson::Value& resources, Manifest::Resource const& resource, RAPIDJSON_DEFAULT_ALLOCATOR& alloc)
{
    Value jsonRes{ kObjectType };
    jsonRes.AddMember("Name", resource.Name, alloc);

    std::vector<Manifest::ResourceVersion> sortedVersions;
    for (auto const& ver : resource.ResourceVersions) {
        sortedVersions.push_back(ver.second);
    }

    // Sort by build date in the generated manifest
    std::sort(sortedVersions.begin(), sortedVersions.end(), [](Manifest::ResourceVersion const& a, Manifest::ResourceVersion const& b) {
        return a.BuildDate < b.BuildDate;
    });

    Value versions{ kArrayType };
    for (auto const& ver : sortedVersions) {
        Stringify(versions, ver, alloc);
    }

    jsonRes.AddMember("Versions", versions, alloc);
    resources.PushBack(jsonRes, alloc);
}

void ManifestSerializer::Stringify(rapidjson::Value& versions, Manifest::ResourceVersion const& ver, RAPIDJSON_DEFAULT_ALLOCATOR& alloc)
{
    Value jsonVer{ kObjectType };
    if (ver.MinGameVersion) {
        jsonVer.AddMember("MinGameVersion", ver.MinGameVersion->ToString(), alloc);
    }

    if (ver.MaxGameVersion) {
        jsonVer.AddMember("MaxGameVersion", ver.MaxGameVersion->ToString(), alloc);
    }

    jsonVer.AddMember("Version", ver.Version.ToString(), alloc);
    jsonVer.AddMember("URL", ver.URL, alloc);
    jsonVer.AddMember("Digest", ver.Digest, alloc);
    jsonVer.AddMember("BuildDate", ver.BuildDate, alloc);

    if (ver.Revoked) {
        jsonVer.AddMember("BuildDate", true, alloc);
    }

    if (!ver.Signature.empty()) {
        jsonVer.AddMember("Signature", ver.Signature, alloc);
    }

    if (!ver.Notice.empty()) {
        jsonVer.AddMember("Notice", ver.Notice, alloc);
    }

    versions.PushBack(jsonVer, alloc);
}

END_SE()
