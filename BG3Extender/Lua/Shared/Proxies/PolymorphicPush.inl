BEGIN_NS(lua)

#define MAKE_REF(ty, cls) if (*type == GFS.ty) { MakeDirectObjectRef(L, static_cast<cls*>(obj), lifetime); return; }

static bool ShownTemplateTypeWarning{ false };

void LuaPolymorphic<GameObjectTemplate>::MakeRef(lua_State* L, GameObjectTemplate* obj, LifetimeHandle const& lifetime)
{
	auto type = obj->GetType();

	MAKE_REF(strcharacter, CharacterTemplate)
	MAKE_REF(stritem, ItemTemplate)
	MAKE_REF(strscenery, SceneryTemplate)
	// TODO - These mappings are disabled until they are updated
	//MAKE_REF(strLevelTemplate, LevelTemplate)
	//MAKE_REF(strsurface, SurfaceTemplate)
	//MAKE_REF(strprojectile, ProjectileTemplate)
	//MAKE_REF(strtrigger, TriggerTemplate)
	//MAKE_REF(strdecal, DecalTemplate)
	//MAKE_REF(strprefab, PrefabTemplate)
	//MAKE_REF(strlight, LightTemplate)

	if (!ShownTemplateTypeWarning) {
		ShownTemplateTypeWarning = true;
		OsiWarn("Found template of unknown type '" << *type << "'; unknown templates will be proxies as GameObjectTemplate");
	}

	MakeDirectObjectRef(L, obj, lifetime);
}

#undef MAKE_REF

END_NS()
