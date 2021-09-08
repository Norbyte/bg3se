#include "stdafx.h"
#include "Manifest.h"
#include "Crypto.h"

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

std::optional<Manifest::ResourceVersion> Manifest::Resource::FindResourceVersion(VersionNumber const& gameVersion) const
{
	std::vector<ResourceVersion> availableVersions;
	for (auto const& ver : ResourceVersions) {
		if (!ver.second.Revoked
			&& !(ver.second.MinGameVersion && ver.second.MinGameVersion > gameVersion)
			&& !(ver.second.MaxGameVersion && ver.second.MaxGameVersion < gameVersion)) {
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

ManifestParseResult ManifestSerializer::Parse(std::string const& json, Manifest& manifest, std::string& parseError)
{
	Json::CharReaderBuilder factory;
	Json::Value root;
	std::string errs;
	auto reader = factory.newCharReader();
	if (!reader->parse(json.data(), json.data() + json.size(), &root, &errs)) {
		parseError = errs;
		return ManifestParseResult::Failed;
	}

	manifest.Resources.clear();

	auto& version = root["ManifestVersion"];
	if (!version.isNumeric()) {
		parseError = "Manifest has no 'ManifestVersion' property";
		return ManifestParseResult::Failed;
	}

	if (version.asInt() != Manifest::CurrentVersion) {
		parseError = "Expected manifest version 1; got ";
		parseError += std::to_string(version.asInt());
		if (version.asInt() > Manifest::CurrentVersion) {
			return ManifestParseResult::UpdateRequired;
		} else {
			return ManifestParseResult::Failed;
		}
	}

	manifest.ManifestVersion = version.asInt();

	if (Parse(root, manifest, parseError)) {
		return ManifestParseResult::Successful;
	} else {
		return ManifestParseResult::Failed;
	}
}

bool ManifestSerializer::Parse(Json::Value const& node, Manifest& manifest, std::string& parseError)
{
	manifest.Resources.clear();

	auto& resources = node["Resources"];
	if (!resources.isArray()) {
		parseError = "Manifest has no 'Resources' array";
		return false;
	}

	for (auto const& resourceNode : resources) {
		if (!resourceNode.isObject()) {
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

bool ManifestSerializer::ParseResource(Json::Value const& node, Manifest::Resource& resource, std::string& parseError)
{
	resource.Name = node["Name"].asString();

	auto& versions = node["Versions"];
	if (!versions.isArray()) {
		parseError = "Manifest resource has no 'Versions' array";
		return false;
	}

	for (auto const& versionNode : versions) {
		if (!versionNode.isObject()) {
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

bool ManifestSerializer::ParseVersion(Json::Value const& node, Manifest::ResourceVersion& version, std::string& parseError)
{
	if (!node["MinGameVersion"].isNull()) {
		auto minVersion = VersionNumber::FromString(node["MinGameVersion"].asString().c_str());
		if (!minVersion) {
			parseError = "Unable to parse 'MinGameVersion'.";
			return false;
		}

		version.MinGameVersion = minVersion;
	}

	if (!node["MaxGameVersion"].isNull()) {
		auto maxVersion = VersionNumber::FromString(node["MaxGameVersion"].asString().c_str());
		if (!maxVersion) {
			parseError = "Unable to parse 'MaxGameVersion'.";
			return false;
		}

		version.MaxGameVersion = maxVersion;
	}

	auto resVersion = VersionNumber::FromString(node["Version"].asString().c_str());
	if (!resVersion) {
		parseError = "Unable to parse 'Version'.";
		return false;
	}

	version.Version = *resVersion;

	version.URL = node["URL"].asString();
	version.Digest = node["Digest"].asString();
	version.BuildDate = node["BuildDate"].asUInt64();
	version.Revoked = node["Revoked"].isBool() ? node["Revoked"].asBool() : false;
	return true;
}

std::string ManifestSerializer::Stringify(Manifest& manifest)
{
	Json::Value resources(Json::arrayValue);
	for (auto const& res : manifest.Resources) {
		Json::Value jsonRes(Json::objectValue);
		jsonRes["Name"] = res.first;

		Json::Value versions(Json::arrayValue);
		for (auto const& ver : res.second.ResourceVersions) {
			Json::Value jsonVer(Json::objectValue);
			if (ver.second.MinGameVersion) {
				jsonVer["MinGameVersion"] = ver.second.MinGameVersion->ToString();
			}

			if (ver.second.MaxGameVersion) {
				jsonVer["MaxGameVersion"] = ver.second.MaxGameVersion->ToString();
			}

			jsonVer["Version"] = ver.second.Version.ToString();
			jsonVer["URL"] = ver.second.URL;
			jsonVer["Digest"] = ver.second.Digest;
			jsonVer["BuildDate"] = ver.second.BuildDate;

			if (ver.second.Revoked) {
				jsonVer["Revoked"] = true;
			}

			versions.append(jsonVer);
		}

		jsonRes["Versions"] = versions;
		resources.append(jsonRes);
	}

	Json::Value root(Json::objectValue);
	root["ManifestVersion"] = manifest.ManifestVersion;
	root["Resources"] = resources;

	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "    ";
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	std::ostringstream os;
	writer->write(root, &os);
	return os.str();
}
