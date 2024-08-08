BEGIN_NS(lua)

#define MAKE_REF(ty, cls) if (*type == GFS.ty) { MakeDirectObjectRef(L, static_cast<cls*>(obj), global); return; }

static bool ShownTemplateTypeWarning{ false };

void LuaPolymorphic<GameObjectTemplate>::MakeRef(lua_State* L, GameObjectTemplate* obj, LifetimeHandle const& lifetime)
{
	auto type = obj->GetType();
	auto global = State::FromLua(L)->GetGlobalLifetime();

	MAKE_REF(strcharacter, CharacterTemplate)
	MAKE_REF(stritem, ItemTemplate)
	MAKE_REF(strscenery, SceneryTemplate)
	MAKE_REF(strprojectile, ProjectileTemplate)
	// TODO - These mappings are disabled until they are updated
	//MAKE_REF(strLevelTemplate, LevelTemplate)
	//MAKE_REF(strsurface, SurfaceTemplate)
	//MAKE_REF(strtrigger, TriggerTemplate)
	//MAKE_REF(strdecal, DecalTemplate)
	//MAKE_REF(strprefab, PrefabTemplate)
	//MAKE_REF(strlight, LightTemplate)

	if (!ShownTemplateTypeWarning) {
		ShownTemplateTypeWarning = true;
		OsiWarn("Found template of unknown type '" << *type << "'; unknown templates will be proxied as GameObjectTemplate");
	}

	MakeDirectObjectRef(L, obj, global);
}

#undef MAKE_REF

END_NS()
