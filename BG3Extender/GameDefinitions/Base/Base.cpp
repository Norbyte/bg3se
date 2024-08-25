#include <stdafx.h>
#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Symbols.h>
#include <combaseapi.h>

#include <GameDefinitions/Base/TypeInformation.inl>

BEGIN_SE()

TranslatedString TranslatedString::FromString(StringView const& sv)
{
	TranslatedString ts;
	auto sep = sv.find_first_of(';');
	if (sep != STDString::npos) {
		ts.Handle.Handle = FixedString(sv.substr(0, sep));
		ts.Handle.Version = (uint16_t)atoi(sv.substr(sep + 1).data());
	} else {
		ts.Handle.Handle = FixedString(sv);
	}
	return ts;
}

std::optional<StringView> TranslatedString::Get() const
{
	return GetStaticSymbols().GetTranslatedStringRepository()->GetTranslatedString(Handle);
}

std::optional<StringView> TranslatedFSString::Get() const
{
	return GetStaticSymbols().GetTranslatedStringRepository()->GetTranslatedString(Handle);
}

END_SE()
