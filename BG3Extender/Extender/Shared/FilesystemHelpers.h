#pragma once

BEGIN_SE()

bool IsSafeRelativePath(WStringView path, bool isGlob = false, bool allowEmpty = false);
bool IsSafeRelativePath(StringView path, bool isGlob = false, bool allowEmpty = false);
bool ValidateAbsolutePath(StringView path, PathRootType root, bool allowEmpty = false);
std::optional<STDString> GetAndValidatePath(StringView path, PathRootType root, bool allowEmpty = false);
std::optional<STDWString> GetAndValidatePathW(StringView path, PathRootType root, bool allowEmpty = false);
std::optional<STDString> GetPathForExternalIo(StringView path, UserPathRootType root, bool allowEmpty = false);
std::optional<STDString> LoadExternalFile(StringView path, UserPathRootType root);
bool SaveExternalFile(StringView path, UserPathRootType root, StringView contents, bool append = false);
Array<STDString> FindFiles(StringView path, StringView glob, UserPathRootType root, bool recursive = false, bool checkPackedFiles = true);

END_SE()
