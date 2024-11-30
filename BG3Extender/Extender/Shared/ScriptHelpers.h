#pragma once

#include <GameDefinitions/Base/Base.h>

namespace bg3se::script {

	std::optional<STDWString> GetPathForExternalIo(std::string_view scriptPath, PathRootType root);
	std::optional<STDString> LoadExternalFile(std::string_view path, PathRootType root);
	bool SaveExternalFile(std::string_view path, PathRootType root, std::string_view contents);

	bool GetTranslatedString(char const* handle, STDString& translated);
	bool GetTranslatedStringFromKey(FixedString const& key, TranslatedString& translated);
	bool CreateTranslatedStringKey(FixedString const& key, FixedString const& handle);
	bool CreateTranslatedString(FixedString const& handle, STDString const& string);

#if defined(__APPLE__)
    // MacOS-specific script helpers
    bool IsMacOS();
    bool IsAppleSilicon();
#endif
}
