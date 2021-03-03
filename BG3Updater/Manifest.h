#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include "json/json.h"

struct Manifest
{
	struct Version
	{
		std::string Path;
		std::string Digest;
	};

	std::unordered_map<std::string, Version> Versions;
};

class ManifestParser
{
public:
	bool Parse(std::string const& json, Manifest& manifest, std::string& parseError)
	{
		Json::CharReaderBuilder factory;
		Json::Value root;
		std::string errs;
		auto reader = factory.newCharReader();
		if (!reader->parse(json.data(), json.data() + json.size(), &root, &errs)) {
			parseError = errs;
			return false;
		}

		manifest.Versions.clear();

		auto versions = root["Versions"];
		if (!versions.isArray()) {
			parseError = "Manifest has no 'Versions' array";
			return false;
		}

		for (auto const& ver : versions) {
			if (!ver.isObject()) {
				parseError = "Version info is not an object";
				return false;
			}

			Manifest::Version version;
			auto versionNumber = ver["Version"].asString();
			version.Path = ver["Path"].asString();
			version.Digest = ver["Digest"].asString();
			manifest.Versions.insert(std::make_pair(versionNumber, version));
		}

		return true;
	}

	std::string Write(Manifest& manifest)
	{
		Json::Value manifests(Json::arrayValue);
		for (auto const& ver : manifest.Versions) {
			Json::Value jsonManifest(Json::objectValue);
			jsonManifest["Version"] = ver.first;
			jsonManifest["Path"] = ver.second.Path;
			jsonManifest["Digest"] = ver.second.Digest;
			manifests.append(jsonManifest);
		}

		Json::Value root(Json::objectValue);
		root["Versions"] = manifests;

		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "None";
		builder["indentation"] = "    ";
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		std::ostringstream os;
		writer->write(root, &os);
		return os.str();
	}
};

