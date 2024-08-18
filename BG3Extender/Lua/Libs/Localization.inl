BEGIN_SE()

RuntimeStringHandle::RuntimeStringHandle()
	: Handle(GFS.strUnknownTSHandle),
	Version(0)
{}

std::optional<StringView> TranslatedStringRepository::GetTranslatedString(RuntimeStringHandle const& handle)
{
	auto text = TranslatedStrings[0]->Texts.try_get(handle);
	if (!text) {
		text = VersionedFallbackPool->Texts.try_get(handle);
		if (!text) {
			text = FallbackPool->Texts.try_get(handle);
		}
	}

	return text ? *text : std::optional<StringView>{};
}

void TranslatedStringRepository::UpdateTranslatedString(RuntimeStringHandle const& handle, StringView translated)
{
	auto text = GameAlloc<STDString>(translated);
	TranslatedStrings[0]->Strings.push_back(text);
	TranslatedStrings[0]->Texts.set(handle, LSStringView(text->data(), text->size()));
}

END_SE()

/// <lua_module>Loca</lua_module>
BEGIN_NS(lua::loca)

STDString GetTranslatedString(FixedString handle, std::optional<char const*> fallbackText)
{
	auto repo = GetStaticSymbols().GetTranslatedStringRepository();
	if (repo) {
		auto text = repo->GetTranslatedString(RuntimeStringHandle(handle, 0));
		if (text) {
			return STDString(*text);
		}
	}

	return fallbackText ? *fallbackText : "";
}

unsigned NextDynamicStringHandleId{ 1 };

bool UpdateTranslatedString(FixedString handle, char const* value)
{
	auto repo = GetStaticSymbols().GetTranslatedStringRepository();
	if (!repo) return false;

	repo->UpdateTranslatedString(RuntimeStringHandle(handle, 0), value);
	return true;
}

void RegisterLocalizationLib()
{
	DECLARE_MODULE(Loca, Both)
	BEGIN_MODULE()
	MODULE_FUNCTION(GetTranslatedString)
	MODULE_FUNCTION(UpdateTranslatedString)
	END_MODULE()
}

END_NS()
