#include <Extender/Shared/FilesystemHelpers.h>
#include <fstream>
#include <shlwapi.h>

BEGIN_SE()

bool IsSafeRelativePath(WStringView path, bool isGlob, bool allowEmpty)
{
    if (!allowEmpty && path.empty()) {
        return false;
    }

    // File naming rules per https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
    // Rejecting ':' rules out drive letters ("C:\", "C:foo") and alternate data streams ("dir::$INDEX_ALLOCATION");
    // rejecting '?' rules out the "\\?\" and "\??\" prefixes.
    for (auto c : path) {
        if (c < 0x20 || c == L'<' || c == L'>' || c == L':' || c == L'"' || c == L'|' || c == L'?' || (c == L'*' && !isGlob)) {
            return false;
        }
    }

    std::size_t start = 0;
    while (start < path.size()) {
        auto end = path.find_first_of(L"\\/", start);
        if (end == WStringView::npos) end = path.size();
        auto component = path.substr(start, end - start);
        start = end + 1;

        // Empty path parts are stripped automatically ("a\\b" -> "a\b")
        if (component.empty()) continue;

        // ".", ".." (path traversal)
        // Trailing dots/spaces (these are removed by the Win32 filesystem APIs)
        if (component.back() == L'.' || component.back() == L' ') {
            return false;
        }
    }

    return true;
}

bool IsSafeRelativePath(StringView path, bool isGlob, bool allowEmpty)
{
    return IsSafeRelativePath(FromUTF8(path), isGlob, allowEmpty);
}

bool ValidateAbsolutePath(StringView path, PathRootType root, bool allowEmpty)
{
    auto rootDir = GetStaticSymbols().GetRootPath(root);
    // Path starts at root
    return !rootDir.empty()
        && path.size() >= rootDir.size()
        && path.compare(0, rootDir.size(), rootDir) == 0
        && (
            // Empty path
            path.size() == rootDir.size()
            || (
                // Valid path relative to root
                path[rootDir.size()] == '/'
                && IsSafeRelativePath(path.substr(rootDir.size() + 1), false, false)
            )
        );
}

std::optional<STDString> GetAndValidatePath(StringView path, PathRootType root, bool allowEmpty)
{
    if (!IsSafeRelativePath(path, false, allowEmpty)) {
        return {};
    }

    auto absolutePath = GetStaticSymbols().ToPath(path, root);
    return absolutePath.empty() ? std::optional<STDString>{} : absolutePath;
}

std::optional<STDWString> GetAndValidatePathW(StringView path, PathRootType root, bool allowEmpty)
{
    auto resolved = GetAndValidatePath(path, root, allowEmpty);
    return resolved ? FromUTF8(*resolved) : std::optional<STDWString>{};
}

std::optional<STDString> GetPathForExternalIo(StringView path, UserPathRootType root, bool allowEmpty)
{
    switch (root) {
        case UserPathRootType::Data:
            return GetAndValidatePath(path, PathRootType::Data, allowEmpty);
            
        case UserPathRootType::UserProfileScriptExtender:
            return GetAndValidatePath(STDString("Script Extender/") + STDString(path), PathRootType::UserProfile, allowEmpty);

        default:
            ERR("Unhandled UserPathRootType type");
            return {};
    }
}

std::optional<STDString> LoadExternalFile(StringView path, UserPathRootType root)
{
    auto resolvedPath = GetPathForExternalIo(path, root);
    if (!resolvedPath) {
        OsiError("Attempted to read file with invalid path: '" << path << "'");
        return {};
    }

    if (root == UserPathRootType::Data) {
        auto reader = GetStaticSymbols().MakeFileReaderAbsolute(*resolvedPath);
        if (reader.IsLoaded()) {
            return reader.ToString();
        }
    } else {
        std::ifstream f(FromUTF8(*resolvedPath).c_str(), std::ios::in | std::ios::binary);
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

Array<STDString> FindFiles(StringView path, StringView glob, UserPathRootType root, bool recursive, bool checkPackedFiles)
{
    auto resolvedRoot = GetPathForExternalIo(path, root, root == UserPathRootType::UserProfileScriptExtender);
    if (!resolvedRoot) {
        OsiError("Attempted to find files at invalid path: '" << path << "'");
        return {};
    }
    
    if (!IsSafeRelativePath(glob, true)) {
        OsiError("Attempted to find files with invalid pattern: '" << glob << "'");
        return {};
    }

    Array<Path> paths;
    (GetStaticSymbols().ls__FileSystem__CollectFilesWithGlob)(Path{ *resolvedRoot }, glob, recursive, paths, checkPackedFiles);

    unsigned stripSize = resolvedRoot->size() + ((*resolvedRoot->rbegin() == '/') ? 0 : 1);

    Array<STDString> names;
    for (auto const& path : paths) {
        names.push_back(path.FullPath.substr(stripSize));
    }

    return names;
}

bool CreateParentDirectoryRecursive(WStringView path)
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

bool SaveExternalFile(StringView path, UserPathRootType root, StringView contents, bool append)
{
    auto absolutePath = GetPathForExternalIo(path, root);
    if (!absolutePath) {
        OsiError("Attempted to write file with invalid path: '" << path << "'");
        return false;
    }

    auto absolutePathWide = FromUTF8(*absolutePath);
    if (!CreateParentDirectoryRecursive(absolutePathWide)) return false;

    std::ofstream f(absolutePathWide.c_str(), std::ios::out | std::ios::binary | (append ? std::ios::app : 0));
    if (!f.good()) {
        OsiError("Could not open file for writing: '" << path << "'");
        return false;
    }

    if (append) {
        f.seekp(0, std::ios::end);
    }

    f.write(contents.data(), contents.length());
    return true;
}

END_NS()
