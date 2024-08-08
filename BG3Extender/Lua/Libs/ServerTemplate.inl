/// <lua_module>Template</lua_module>
BEGIN_NS(esv::lua::tmpl)

LegacyMap<FixedString, GameObjectTemplate*>* GetAllRootTemplates()
{
	auto bank = GetStaticSymbols().GetGlobalTemplateBank();
	return &bank->Templates;
}

GameObjectTemplate* GetRootTemplate(FixedString const& templateId)
{
	auto bank = GetStaticSymbols().GetGlobalTemplateBank();
	return bank->Templates.try_get(templateId);
}

HashMap<FixedString, GameObjectTemplate*>* GetAllCacheTemplates()
{
	auto cache = *GetStaticSymbols().esv__CacheTemplateManager;
	if (cache) {
		return &cache->Templates;
	}

	return nullptr;
}

LegacyRefMap<FixedString, GameObjectTemplate*>* GetAllLocalTemplates()
{
	auto level = GetStaticSymbols().GetCurrentServerLevel();
	if (level) {
		return &level->LocalTemplateManager->Templates;
	}

	return nullptr;
}

GameObjectTemplate* GetLocalTemplate(FixedString const& templateId)
{
	auto level = GetStaticSymbols().GetCurrentServerLevel();
	if (level) {
		return level->LocalTemplateManager->Templates.try_get(templateId);
	}

	return nullptr;
}

GameObjectTemplate* GetCacheTemplate(FixedString const& templateId)
{
	auto cache = *GetStaticSymbols().esv__CacheTemplateManager;
	if (cache) {
		auto tmpl = cache->Templates.try_get(templateId);
		return tmpl ? *tmpl : nullptr;
	}

	return nullptr;
}

HashMap<FixedString, GameObjectTemplate*>* GetAllLocalCacheTemplates()
{
	auto level = GetStaticSymbols().GetCurrentServerLevel();
	if (level) {
		return &level->CacheTemplateManager->Templates;
	}

	return nullptr;
}

GameObjectTemplate* GetLocalCacheTemplate(FixedString const& templateId)
{
	auto level = GetStaticSymbols().GetCurrentServerLevel();
	if (level) {
		auto tmpl = level->CacheTemplateManager->Templates.try_get(templateId);
		return tmpl ? *tmpl : nullptr;
	}

	return nullptr;
}

GameObjectTemplate* GetTemplate(FixedString const& templateId)
{
	auto tmpl = GetRootTemplate(templateId);
	if (!tmpl) {
		tmpl = GetLocalTemplate(templateId);
		if (!tmpl) {
			tmpl = GetCacheTemplate(templateId);
			if (!tmpl) {
				tmpl = GetLocalCacheTemplate(templateId);
			}
		}
	}

	return tmpl;
}

void RegisterTemplateLib()
{
	DECLARE_MODULE(Template, Server)
	BEGIN_MODULE()
	MODULE_FUNCTION(GetTemplate)
	MODULE_FUNCTION(GetAllRootTemplates)
	MODULE_FUNCTION(GetRootTemplate)
	MODULE_FUNCTION(GetAllLocalTemplates)
	MODULE_FUNCTION(GetLocalTemplate)
	MODULE_FUNCTION(GetAllCacheTemplates)
	MODULE_FUNCTION(GetCacheTemplate)
	MODULE_FUNCTION(GetAllLocalCacheTemplates)
	MODULE_FUNCTION(GetLocalCacheTemplate)
	END_MODULE()
}

END_NS()
