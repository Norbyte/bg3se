#pragma once

#include <GameDefinitions/Base/Base.h>

namespace bg3se::script {

    std::optional<STDWString> GetPathForExternalIo(std::string_view scriptPath, PathRootType root);
    std::optional<STDString> LoadExternalFile(std::string_view path, PathRootType root);
    bool SaveExternalFile(std::string_view path, PathRootType root, StringView contents, bool append = false);
    Array<STDString> FindFiles(StringView path, StringView glob, PathRootType root, bool recursive = false, bool checkPackedFiles = true);

    bool GetTranslatedString(char const* handle, STDString& translated);
    bool GetTranslatedStringFromKey(FixedString const& key, TranslatedString& translated);
    bool CreateTranslatedStringKey(FixedString const& key, FixedString const& handle);
    bool CreateTranslatedString(FixedString const& handle, STDString const& string);
}
