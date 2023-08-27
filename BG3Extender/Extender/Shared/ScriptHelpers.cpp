#include "stdafx.h"
#include "ScriptHelpers.h"
#include <Extender/ScriptExtender.h>
#include <fstream>

namespace bg3se::script {

#define SAFE_PATH_CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_./ "

bool IsSafeRelativePath(STDString const& path)
{
	if (path.find_first_not_of(SAFE_PATH_CHARS) != STDString::npos
		|| path.find("..") != STDString::npos) {
		OsiError("Illegal file name for external file access: '" << path << "'");
		return false;
	} else {
		return true;
	}
}

std::optional<STDWString> GetPathForExternalIo(std::string_view scriptPath, PathRootType root)
{
	STDString path(scriptPath);

	if (!IsSafeRelativePath(path)) {
		return {};
	}

	auto storageRoot = GetStaticSymbols().ToPath("/Osiris Data", root);
	if (storageRoot.empty()) {
		OsiErrorS("Could not fetch game storage path");
		return {};
	}

	return FromUTF8(storageRoot + "/" + path);
}

std::optional<STDString> LoadExternalFile(std::string_view path, PathRootType root)
{
	if (!IsSafeRelativePath(STDString(path))) {
		return {};
	}

	if (root == PathRootType::Data) {
		auto reader = GetStaticSymbols().MakeFileReader(path, root);
		if (reader.IsLoaded()) {
			return reader.ToString();
		}
	} else {
		auto absolutePath = GetPathForExternalIo(path, root);
		if (!absolutePath) return {};

		std::ifstream f(absolutePath->c_str(), std::ios::in | std::ios::binary);
		if (f.good()) {
			STDString body;
			f.seekg(0, std::ios::end);
			body.resize((unsigned)f.tellg());
			f.seekg(0, std::ios::beg);
			f.read(body.data(), body.size());
			return body;
		}
	}

	return {};
}

bool SaveExternalFile(std::string_view path, PathRootType root, std::string_view contents)
{
	auto absolutePath = GetPathForExternalIo(path, root);
	if (!absolutePath) return false;

	auto dirEnd = absolutePath->find_last_of('/');
	if (dirEnd == std::string::npos) return false;

	auto storageDir = absolutePath->substr(0, dirEnd);
	BOOL created = CreateDirectoryW(storageDir.c_str(), NULL);
	if (created == FALSE) {
		DWORD lastError = GetLastError();
		if (lastError != ERROR_ALREADY_EXISTS) {
			OsiError("Could not create storage directory: " << ToUTF8(storageDir));
			return {};
		}
	}

	std::ofstream f(absolutePath->c_str(), std::ios::out | std::ios::binary);
	if (!f.good()) {
		OsiError("Could not open file: '" << path << "'");
		return false;
	}

	f.write(contents.data(), contents.length());
	return true;
}

bool GetTranslatedString(char const* handle, STDString& translated)
{
	throw std::runtime_error("FIXME!");
	/*auto repo = GetStaticSymbols().GetTranslatedStringRepository();
	auto getter = GetStaticSymbols().TranslatedStringRepository__Get;

	if (repo == nullptr || getter == nullptr) {
		OsiError("TranslatedStringRepository functions not mapped!");
		return false;
	}

	auto key = ToFixedString(handle);
	if (!key) {
		return false;
	}

	RuntimeStringHandle runtimeString;
	runtimeString.Handle = key;
	uint64_t length{ 0 };
	auto translatedString = getter(repo, &runtimeString, &length, 0, 0, true);

	if (translatedString) {
		translated = *translatedString;
		return true;
	} else {
		return false;
	}*/
}

bool GetTranslatedStringFromKey(FixedString const& key, TranslatedString& translated)
{
	throw std::runtime_error("FIXME!");
	/*auto keyMgr = GetStaticSymbols().GetTranslatedStringKeyManager();
	auto getter = GetStaticSymbols().TranslatedStringKeyManager__GetTranlatedStringFromKey;

	if (keyMgr == nullptr || getter == nullptr) {
		OsiError("TranslatedStringKeyManager functions not mapped!");
		return false;
	}

	if (getter(keyMgr, translated, key, false)) {
		// If the string key is not localized, use the content from the localization LSB.
		if (translated.Handle.Handle != GFS.strNullStringHandle) {
			return GetTranslatedString(translated.Handle.Handle.Str, translated.Handle.ReferenceString);
		} else {
			return !translated.Handle.ReferenceString.empty();
		}
	} else {
		return false;
	}*/
}

bool CreateTranslatedStringKey(FixedString const& key, FixedString const& handle)
{
	throw std::runtime_error("FIXME!");
	/*auto keyMgr = GetStaticSymbols().GetTranslatedStringKeyManager();
	if (keyMgr == nullptr) {
		OsiError("TranslatedStringKeyManager not mapped!");
		return false;
	}

	auto mapping = keyMgr->StringKeys.Find(key);
	if (mapping) {
		mapping->Handle.Handle = handle;
	} else {
		TranslatedString str;
		str.Handle.Handle = handle;
		keyMgr->StringKeys.Insert(key, str);
	}

	return true;*/
}

bool CreateTranslatedString(FixedString const& handle, STDString const& string)
{
	throw std::runtime_error("FIXME!");
	/*auto repo = GetStaticSymbols().GetTranslatedStringRepository();
	auto getter = GetStaticSymbols().TranslatedStringRepository__Get;

	if (repo == nullptr || getter == nullptr) {
		OsiError("TranslatedStringRepository functions not mapped!");
		return false;
	}

	EnterCriticalSection(&repo->CriticalSection);
	auto str = repo->TranslatedStringOverrides[0]->Find(handle);
	if (str) {
		**str = string;
	} else {
		auto s = GameAlloc<STDWString>(string);
		repo->TranslatedStringOverrides[0]->Insert(handle, s);
	}

	LeaveCriticalSection(&repo->CriticalSection);

	return true;*/
}

}
