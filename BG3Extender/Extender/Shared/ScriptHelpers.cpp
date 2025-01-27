#include "stdafx.h"
#include "ScriptHelpers.h"
#include <Extender/ScriptExtender.h>
#include <fstream>
#include <shlwapi.h>

namespace bg3se::script {

bool IsSafeRelativePath(STDString const& path)
{
    if (path.empty()) {
        OsiError("IO path cannot be empty");
        return false;
    }

    // File naming rules per https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
    for (auto c : path) {
        // Note: since path is UTF-8, we let all code points > 0x80 through
        if (c < 0x20 || c == '<' || c == '>' || c == ':' || c == '"' || c == '|' || c == '?' || c == '*') {
            OsiError("Illegal character in filename: '" << path << "'");
            return false;
        }
    }

    if (*path.rbegin() == ' ' || *path.rbegin() == '.') {
        OsiError("Path cannot end with space or dot: '" << path << "'");
        return false;
    }

    if (path.find("..") != STDString::npos) {
        OsiError("Path cannot contain traversal: '" << path << "'");
        return false;
    }

    return true;
}

std::optional<STDWString> GetPathForExternalIo(std::string_view scriptPath, PathRootType root)
{
    STDString path(scriptPath);

    if (!IsSafeRelativePath(path)) {
        return {};
    }

    auto storageRoot = GetStaticSymbols().ToPath("/Script Extender", root);
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

bool CreateParentDirectoryRecursive(std::wstring_view path)
{
    auto dirEnd = path.find_last_of('/');
    if (dirEnd == std::string::npos) return true;
    STDWString parentDir(path.substr(0, dirEnd));

    if (PathFileExistsW(parentDir.c_str())) {
        return true;
    }

    if (!CreateParentDirectoryRecursive(parentDir)) {
        return false;
    }

    BOOL created = CreateDirectoryW(parentDir.c_str(), NULL);
    if (created == FALSE) {
        DWORD lastError = GetLastError();
        if (lastError != ERROR_ALREADY_EXISTS) {
            OsiError("Could not create storage directory: " << ToUTF8(parentDir));
            return false;
        }
    }

    return true;
}

bool SaveExternalFile(std::string_view path, PathRootType root, StringView contents)
{
    auto absolutePath = GetPathForExternalIo(path, root);
    if (!absolutePath) return false;

    if (!CreateParentDirectoryRecursive(*absolutePath)) return false;

    std::ofstream f(absolutePath->c_str(), std::ios::out | std::ios::binary);
    if (!f.good()) {
        OsiError("Could not open file for writing: '" << path << "'");
        return false;
    }

    f.write(contents.data(), contents.length());
    return true;
}

}
