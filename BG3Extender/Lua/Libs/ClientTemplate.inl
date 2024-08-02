/// <lua_module>Template</lua_module>
BEGIN_NS(ecl::lua::tmpl)

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

GameObjectTemplate* GetTemplate(FixedString const& templateId)
{
	return GetRootTemplate(templateId);
}

void RegisterTemplateLib()
{
	DECLARE_MODULE(Template, Client)
	BEGIN_MODULE()
	MODULE_FUNCTION(GetTemplate)
	MODULE_FUNCTION(GetAllRootTemplates)
	MODULE_FUNCTION(GetRootTemplate)
	END_MODULE()
}

END_NS()
