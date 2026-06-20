BEGIN_SE()

RuntimeStringHandle::RuntimeStringHandle()
    : Handle(GFS.strUnknownTSHandle),
    Version(0)
{}

std::optional<StringView> TranslatedStringRepository::GetTranslatedString(RuntimeStringHandle const& handle)
{
    Lock.ReadLock();
    auto text = TranslatedStrings[0]->Texts.try_get(handle);
    if (!text) {
        text = VersionedFallbackPool->Texts.try_get(handle);
        if (!text) {
            text = FallbackPool->Texts.try_get(handle);
        }
    }
    Lock.ReadUnlock();

    return text ? *text : std::optional<StringView>{};
}

void TranslatedStringRepository::UpdateTranslatedString(RuntimeStringHandle const& handle, StringView translated)
{
    auto text = GameAlloc<STDString>(translated);
    Lock.WriteLock();
    TranslatedStrings[0]->Strings.push_back(text);
    TranslatedStrings[0]->Texts.set(handle, LSStringView(text->data(), text->size()));
    Lock.WriteUnlock();
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

LegacyMap<FixedString, TranslatedString>* GetAllTranslatedStringKeys(FixedString key)
{
    auto repo = GetStaticSymbols().GetTranslatedStringKeyManager();
    if (!repo) return {};

    return &repo->Keys;
}

std::optional<TranslatedString> GetTranslatedStringKey(FixedString key)
{
    auto repo = GetStaticSymbols().GetTranslatedStringKeyManager();
    if (!repo) return {};

    auto ts = repo->Keys.try_get(key);
    return ts ? *ts : std::optional<TranslatedString>{};
}

bool UpdateTranslatedStringKey(FixedString key, FixedString ts)
{
    auto repo = GetStaticSymbols().GetTranslatedStringKeyManager();
    if (!repo) return false;

    TranslatedString tsk;
    tsk.Handle.Handle = ts;
    repo->Keys.insert(key, tsk);
    return true;
}

void RegisterLocalizationLib()
{
    DECLARE_MODULE(Loca, Both)
    BEGIN_MODULE()
    MODULE_FUNCTION(GetTranslatedString)
    MODULE_FUNCTION(UpdateTranslatedString)
    MODULE_FUNCTION(GetAllTranslatedStringKeys)
    MODULE_FUNCTION(GetTranslatedStringKey)
    MODULE_FUNCTION(UpdateTranslatedStringKey)
    END_MODULE()
}

END_NS()
