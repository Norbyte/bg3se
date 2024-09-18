#include "stdafx.h"

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Symbols.h>
#include <Lua/Shared/Proxies/LuaPropertyMapHelpers.h>
#include <Extender/ScriptExtender.h>
#include <GameDefinitions/Resources.h>
#include <GameDefinitions/Dialog.h>
#include <GameDefinitions/Stats/UseActions.h>
#include <GameDefinitions/Stats/Functors.h>

#include <GameDefinitions/Ai.inl>

namespace bg3se
{
	EnumRegistry& EnumRegistry::Get()
	{
		static EnumRegistry reg;
		return reg;
	}

	void EnumRegistry::Register(EnumInfoStore* ei, int32_t id)
	{
		assert(EnumsByName.find(ei->EnumName) == EnumsByName.end());
		EnumsByName.insert(ei->EnumName, ei);
		ei->RegistryIndex = id;

		if (EnumsById.size() < (uint32_t)id + 1) {
			EnumsById.resize(id + 1);
		}

		EnumsById[id] = ei;
	}


	BitfieldRegistry& BitfieldRegistry::Get()
	{
		static BitfieldRegistry reg;
		return reg;
	}
	
	void BitfieldRegistry::Register(BitfieldInfoStore* ei, int32_t id)
	{
		assert(BitfieldsByName.find(ei->EnumName) == BitfieldsByName.end());
		BitfieldsByName.insert(ei->EnumName, ei);
		ei->RegistryIndex = id;

		if (BitfieldsById.size() < (uint32_t)id + 1) {
			BitfieldsById.resize(id + 1);
		}

		BitfieldsById[id] = ei;
	}

	StaticSymbols* gStaticSymbols{ nullptr };

	void InitStaticSymbols()
	{
		gStaticSymbols = new StaticSymbols();
		gCoreLibPlatformInterface.StaticSymbols = gStaticSymbols;
	}

	void StaticSymbols::CanonicalizePath(STDString & path) const
	{
		if (path.find('\\') != STDString::npos) {
			WARN("Path contains backslashes: \"%s\"; canonical paths should only contain forward slashes.", path.c_str());
		}

		std::replace(path.begin(), path.end(), '\\', '/');
	}

	STDString StaticSymbols::ToPath(StringView path, PathRootType root, bool canonicalize) const
	{
		if (ls__PathRoots == nullptr) {
			ERR("LibraryManager::ToPath(): Path root API not available!");
			return "";
		}

		STDString canonicalPath(path);
		if (canonicalize) {
			CanonicalizePath(canonicalPath);
		}

		if (root == PathRootType::Root) {
			return canonicalPath;
		}

		auto rootPath = ls__PathRoots[(unsigned)root];
		if (rootPath == nullptr) {
			ERR("StaticSymbols::ToPath(): Path roots not initialized!");
			return "";
		}

		STDString absolutePath(*rootPath);
		absolutePath += "/" + canonicalPath;
		return absolutePath;
	}

	FileReaderPin StaticSymbols::MakeFileReader(StringView path, PathRootType root, bool canonicalize) const
	{
		if (ls__PathRoots == nullptr || ls__FileReader__ctor == nullptr) {
			ERR("StaticSymbols::MakeFileReader(): File reader API not available!");
			return FileReaderPin(nullptr);
		}

		auto absolutePath = ToPath(path, root, canonicalize);

		Path lsPath;
		lsPath.Name = absolutePath;

		auto reader = GameAlloc<FileReader>();
		ls__FileReader__ctor(reader, lsPath, 2, 0);
		return FileReaderPin(reader);
	}

	void StaticSymbols::DestroyFileReader(FileReader* reader)
	{
		if (ls__FileReader__dtor != nullptr) {
			ls__FileReader__dtor(reader);
		}
	}

	bool StaticSymbols::FileExists(StringView path, PathRootType root, bool canonicalize) const
	{
		// TODO - implement using proper FS file exists call
		auto reader = MakeFileReader(path, root, canonicalize);
		return reader.IsLoaded();
	}

	GlobalTemplateBank* StaticSymbols::GetGlobalTemplateBank() const
	{
		auto gtm = GetStaticSymbols().GetGlobalTemplateManager();
		if (!gtm) return nullptr;

#if defined(OSI_EOCAPP)
		auto tls = *(uint64_t*)__readgsqword(0x58);
		auto slot = ((uint8_t*)tls)[8];
		return gtm->Banks[slot];
#else
		auto getter = GetStaticSymbols().GlobalTemplateManager__GetGlobalTemplateBank;
		return getter(gtm);
#endif
	}

	ResourceBank* StaticSymbols::GetCurrentResourceBank() const
	{
		auto resMgr = ls__gGlobalResourceManager;
		if (!resMgr || !*resMgr || !(*resMgr)->ResourceBanks[0]) {
			return nullptr;
		}

		if ((*resMgr)->ResourceBanks[0]->Packages.size() > 0) {
			return (*resMgr)->ResourceBanks[0];
		} else {
			return (*resMgr)->ResourceBanks[1];
		}
	}

	FileReaderPin::~FileReaderPin()
	{
		if (reader_ != nullptr) {
			GetStaticSymbols().DestroyFileReader(reader_);
		}
	}

	STDString FileReaderPin::ToString() const
	{
		if (!IsLoaded()) {
			OsiErrorS("File not loaded!");
			return "";
		}

		STDString contents;
		contents.resize((uint32_t)Size());
		memcpy(contents.data(), Buf(), Size());
		return contents;
	}

	FixedString GameObjectTemplate::GetTemplateType() const
	{
		return *GetType();
	}

	Module const * ModManager::FindModByNameGuid(char const * nameGuid) const
	{
		auto modUuidFS = NameGuidToFixedString(nameGuid);
		if (!modUuidFS) {
			return nullptr;
		}

		for (auto const& mod : BaseModule.LoadOrderedModules) {
			if (mod.Info.ModuleUUIDString == modUuidFS) {
				return &mod;
			}
		}

		return nullptr;
	}

	esv::Status* esv::StatusMachine::GetStatus(FixedString const& statusId) const
	{
		for (auto status : Statuses) {
			if (status->StatusId == statusId) {
				return status;
			}
		}

		return nullptr;
	}

	static bool CharacterGetSelfCalled{ false };

	RefReturn<esv::Character> esv::Character::LuaGetSelf() const
	{
		if (!CharacterGetSelfCalled) {
			CharacterGetSelfCalled = true;
			WARN("entity.ServerCharacter.Character is deprecated; use entity.ServerCharacter instead");
		}

		return const_cast<esv::Character*>(this);
	}

	static bool ItemGetSelfCalled{ false };

	RefReturn<esv::Item> esv::Item::LuaGetSelf() const
	{
		if (!ItemGetSelfCalled) {
			ItemGetSelfCalled = true;
			WARN("entity.ServerItem.Item is deprecated; use entity.ServerItem instead");
		}

		return const_cast<esv::Item*>(this);
	}

	esv::Status * esv::Character::GetStatus(FixedString statusId)
	{
		if (StatusManager == nullptr) {
			return nullptr;
		}

		return StatusManager->GetStatus(statusId);
	}

	esv::Status* esv::Character::GetStatusByType(StatusType type)
	{
		if (StatusManager == nullptr) {
			return nullptr;
		}

		for (auto status : StatusManager->Statuses) {
			if (status->GetStatusId() == type) {
				return status;
			}
		}

		return nullptr;
	}

	char const * TempStrings::Make(STDString const & str)
	{
		auto s = _strdup(str.c_str());
		pool_.push_back(s);
		return s;
	}

	char const * TempStrings::Make(std::string const & str)
	{
		auto s = _strdup(str.c_str());
		pool_.push_back(s);
		return s;
	}
	
	TempStrings gTempStrings;
}

BEGIN_NS(lua)

void LuaPolymorphic<stats::ContextData>::MakeRef(lua_State* L, stats::ContextData* value, LifetimeHandle const& lifetime)
{
#define V(type) case FunctorContextType::type: \
			MakeDirectObjectRef(L, static_cast<stats::type##ContextData*>(value), lifetime); break;

	switch (value->Type) {
		V(AttackTarget)
		V(AttackPosition)
		V(Move)
		V(Target)
		V(NearbyAttacked)
		V(NearbyAttacking)
		V(Equip)
		V(Source)
		V(Interrupt)

	default:
		MakeDirectObjectRef(L, value, lifetime);
		break;
	}

#undef V
}

void LuaPolymorphic<stats::Functor>::MakeRef(lua_State* L, stats::Functor* value, LifetimeHandle const& lifetime)
{
#define V(type) case stats::FunctorId::type: \
			MakeDirectObjectRef(L, static_cast<stats::type##Functor*>(value), lifetime); break;

	switch (value->TypeId) {
		V(CustomDescription)
		V(ApplyStatus)
		V(SurfaceChange)
		V(Resurrect)
		V(Sabotage)
		V(Summon)
		V(Force)
		V(Douse)
		V(SwapPlaces)
		V(Pickup)
		V(CreateSurface)
		V(CreateConeSurface)
		V(RemoveStatus)
		V(DealDamage)
		V(ExecuteWeaponFunctors)
		V(RegainHitPoints)
		V(TeleportSource)
		V(SetStatusDuration)
		V(UseSpell)
		V(UseActionResource)
		V(UseAttack)
		V(CreateExplosion)
		V(BreakConcentration)
		V(ApplyEquipmentStatus)
		V(RestoreResource)
		V(Spawn)
		V(Stabilize)
		V(Unlock)
		V(ResetCombatTurn)
		V(RemoveAuraByChildStatus)
		V(SummonInInventory)
		V(SpawnInInventory)
		V(RemoveUniqueStatus)
		V(DisarmWeapon)
		V(DisarmAndStealWeapon)
		V(SwitchDeathType)
		V(TriggerRandomCast)
		V(GainTemporaryHitPoints)
		V(FireProjectile)
		V(ShortRest)
		V(CreateZone)
		V(DoTeleport)
		V(RegainTemporaryHitPoints)
		V(RemoveStatusByLevel)
		V(SurfaceClearLayer)
		V(Unsummon)
		V(CreateWall)
		V(Counterspell)
		V(AdjustRoll)
		V(SpawnExtraProjectiles)
		V(Kill)
		V(TutorialEvent)
		V(Drop)
		V(ResetCooldowns)
		V(SetRoll)
		V(SetDamageResistance)
		V(SetReroll)
		V(SetAdvantage)
		V(SetDisadvantage)
		V(MaximizeRoll)
		V(CameraWait)
		V(Extender)

	default:
		MakeDirectObjectRef(L, value, lifetime);
		break;
	}

#undef V
}

void LuaPolymorphic<IActionData>::MakeRef(lua_State* L, IActionData* value, LifetimeHandle const& lifetime)
{
#define V(type) case ActionDataType::type: \
			MakeDirectObjectRef(L, static_cast<type##ActionData*>(value), lifetime); break;

	switch (value->Type) {
		V(OpenClose)
		V(Destroy)
		V(Teleport)
		V(CreateSurface)
		V(DestroyParameters)
		V(Equip)
		V(Consume)
		V(StoryUse)
		V(Door)
		V(CreatePuddle)
		V(Book)
		V(UseSpell)
		V(SpellBook)
		V(Sit)
		V(Lie)
		V(Insert)
		V(Stand)
		V(Lockpick)
		V(StoryUseInInventory)
		V(DisarmTrap)
		V(ShowStoryElementUI)
		V(Combine)
		V(Ladder)
		V(PlaySound)
		V(SpawnCharacter)
		V(Constrain)
		V(Recipe)
		V(Unknown31)
		V(Throw)
		V(LearnSpell)
		V(Unknown34)
		V(Unknown35)

	default:
		MakeDirectObjectRef(L, value, lifetime);
		break;
	}

#undef V
}

void LuaPolymorphic<TextKeyTypeProperties>::MakeRef(lua_State* L, TextKeyTypeProperties* value, LifetimeHandle const& lifetime)
{
#define V(type) case TextKeyType::type: \
			MakeDirectObjectRef(L, static_cast<TextKey##type##TypeProperties*>(value), lifetime); break;

	switch (value->GetType()) {
		V(Sound)
		V(Effect)
		V(FootStep)
		V(Attach)
		V(WeaponEffect)
		V(Genome)
		V(Attack)
		V(Ragdoll)
		V(VisualCullFlag)
		V(FloatData)
		V(Foley)
		V(Vocal)
		V(FootMove)
		V(React)

	default:
		MakeDirectObjectRef(L, value, lifetime);
		break;
	}

#undef V
}

void LuaPolymorphic<aspk::Property>::MakeRef(lua_State* L, aspk::Property* value, LifetimeHandle const& lifetime)
{
#define V(type) case aspk::PropertyType::type: \
			MakeDirectObjectRef(L, static_cast<aspk::type##Property*>(value), lifetime); break;

	switch (value->GetPropertyType()) {
		V(Boolean)
		V(Integer)
		V(IntegerRange)
		V(ColorARGBKeyFrame)
		V(Float)
		V(FloatRange)
		V(FloatKeyFrame)
		V(String)
		V(Vector3)
		V(FixedFunction)
		V(FixedString)
		V(Base)

	default:
		MakeDirectObjectRef(L, value, lifetime);
		break;
	}

#undef V
}

void LuaPolymorphic<aspk::KeyFrameData>::MakeRef(lua_State* L, aspk::KeyFrameData* value, LifetimeHandle const& lifetime)
{
	switch (value->GetType()) {
	case 0:
		MakeDirectObjectRef(L, static_cast<aspk::LinearFloatKeyFrameData*>(value), lifetime);
		break;

	case 1:
		MakeDirectObjectRef(L, static_cast<aspk::CubicFloatKeyFrameData*>(value), lifetime);
		break;

	default:
		MakeDirectObjectRef(L, value, lifetime);
		break;
	}

#undef V
}

void LuaPolymorphic<resource::PhysicsResource::ObjectTemplate::PhysicsObject>::MakeRef(lua_State* L, resource::PhysicsResource::ObjectTemplate::PhysicsObject* value, LifetimeHandle const& lifetime)
{
	if (value->GetType().GetStringView() == "box")
	{
		MakeDirectObjectRef(L, static_cast<resource::PhysicsResource::ObjectTemplate::PhysicsBox*>(value), lifetime);
	}
	else if (value->GetType().GetStringView() == "capsule")
	{
		MakeDirectObjectRef(L, static_cast<resource::PhysicsResource::ObjectTemplate::PhysicsCapsule*>(value), lifetime);
	}
	else
	{
		MakeDirectObjectRef(L, value, lifetime);
	}
}

void LuaPolymorphic<dlg::DialogNode>::MakeRef(lua_State* L, dlg::DialogNode* value, LifetimeHandle const& lifetime)
{
#define V(type) if (value->ConstructorID == GFS.str##type) { MakeDirectObjectRef(L, static_cast<dlg::type##Node*>(value), lifetime); return; }

	V(Alias)
	V(Jump)
	V(CinematicTagged)
	V(SelectSpeaker)
	V(FallibleQuestionResult)
	V(VisualState)
	V(NestedDialog)

	V(ActiveRoll)
	V(PassiveRoll)
	V(Trade)

#undef V

	if (value->ConstructorID == GFS.strTagQuestion) {
		MakeDirectObjectRef(L, static_cast<dlg::TaggedQuestionNode*>(value), lifetime);
		return;
	}

	if (value->ConstructorID == GFS.strTagGreeting || value->ConstructorID == GFS.strTagAnswer) {
		MakeDirectObjectRef(L, static_cast<dlg::BaseTaggedNode*>(value), lifetime);
		return;
	}
	
	// TODO - map client/server-specific parts
	// V(PopNode)

	MakeDirectObjectRef(L, value, lifetime);
}

void LuaPolymorphic<aspk::Component>::MakeRef(lua_State* L, aspk::Component* value, LifetimeHandle const& lifetime)
{
#define V(type) else if (value->GetTypeName().GetStringView() == #type) \
					MakeDirectObjectRef(L, static_cast<aspk::type##Component*>(value), lifetime);
#define V_timeline(type) else if (value->GetTypeName().GetStringView() == #type) \
					MakeDirectObjectRef(L, static_cast<aspk::type##Component*>(value), lifetime);
#define V_tlkeybase(type) else if (value->GetTypeName().GetStringView() == #type) \
					MakeDirectObjectRef(L, static_cast<aspk::type##Component<aspk::keys::KeyBase>*>(value), lifetime);
#define V_tlchannel(type) else if (value->GetTypeName().GetStringView() == #type) \
					MakeDirectObjectRef(L, static_cast<aspk::channels::type##Component*>(value), lifetime);
	if (value->GetTypeName().GetStringView() == "BaseComponent")
	{
		MakeDirectObjectRef(L, static_cast<aspk::Component*>(value), lifetime);
	}
	// Special case for this one because it can't be its own V
	else if (value->GetTypeName().GetStringView() == "Ribbon 2.0")
	{
		MakeDirectObjectRef(L, static_cast<aspk::Ribbon2Component*>(value), lifetime);
	}
	V(Billboard)
	V(BoundingBox)
	V(BoundingSphere)
	V(CameraShake)
	V(Decal)
	V(Deflector)
	V(GravityForce)
	V(Light)
	V(Model)
	V(MovingLevel)
	V(OrbitForce)
	V(OverlayMaterial)
	V(ParticleSystem)
	V(PostProcess)
	V(PreRoll)
	V(RadialForce)
	V(Sound)
	V(SpinForce)
	V(TurbulentForce)
	V(VortexForce)
	V(WindForce)
	V(TLBase)
	V_timeline(TimelineActorPropertiesReflection)
	V_timeline(TLAdditiveAnimation)
	V_timeline(TLAnimation)
	V_timeline(TLAtmosphereAndLighting)
	V_timeline(TLAttachToEvent)
	V_timeline(TLAttitudeEvent)
	V_timeline(TLCameraDoF)
	V_timeline(TLCameraExposure)
	V_timeline(TLCameraFoV)
	V_timeline(TLCameraLookAt)
	V_timeline(TLEffectPhaseEvent)
	V_timeline(TLEmotionEvent)
	V_timeline(TLFloatRTPC)
	V_timeline(TLGenomeTextEvent)
	V_timeline(TLHandsIK)
	V_timeline(TLLayeredAnimation)
	V_timeline(TLLookAtEvent)
	V_timeline(TLMaterial)
	V_timeline(TLPhysics)
	V_timeline(TLPlayEffectEvent)
	V_timeline(TLPlayRate)
	V_timeline(TLShapeShift)
	V_timeline(TLShot)
	V_timeline(TLShotHoldPrevious)
	V_timeline(TLShotZoom)
	V_timeline(TLShowArmor)
	V_timeline(TLShowHUD)
	V_timeline(TLShowPeanuts)
	V_timeline(TLShowVisual)
	V_timeline(TLShowWeapon)
	V_timeline(TLSoundEvent)
	V_timeline(TLSplatter)
	V_timeline(TLSprings)
	V_timeline(TLSteppingFade)
	V_timeline(TLSwitchLocationEvent)
	V_timeline(TLSwitchStageEvent)
	V_timeline(TLTransform)
	V_timeline(TLVoice)
	V_tlkeybase(TLEventKey)
	V_tlkeybase(TLInterpolationKey)
	V_tlkeybase(TLKeyBase)
	V_tlchannel(TimelineActorPropertiesReflectionKey)
	V_tlchannel(TLAtmosphereAndLightingChannel)
	V_tlchannel(TLCameraDoFChannel)
	V_tlchannel(TLCameraExposureChannel)
	V_tlchannel(TLMaterialKey)
	V_tlchannel(TLMaterialTextureKey)
	V_tlchannel(TLShowArmorChannel)
	V_tlchannel(TLSplatterChannel)
	// Theoretically there are a lot of other EffectComponents that could get output here; haven't finished researching them
	else
	{
		MakeDirectObjectRef(L, value, lifetime);
	}

#undef V_tlchannel
#undef V_tlkeybase
#undef V_timeline
#undef V
}

void LuaPolymorphic<aspk::TLMaterialComponent::Parameter>::MakeRef(lua_State* L, aspk::TLMaterialComponent::Parameter* value, LifetimeHandle const& lifetime)
{
	aspk::TLMaterialComponent::Parameter::Range range;
	value->getRange(range);
	if (range.begin == nullptr || range.end == nullptr)
	{
		MakeDirectObjectRef(L, value, lifetime);
	}
	else if ((*range.begin)->GetTypeName().GetStringView() == "TLMaterialKeyComponent")
	{
		MakeDirectObjectRef(L, static_cast<aspk::TLMaterialComponent::MaterialParameter*>(value), lifetime);
	}
	// For some reason, this takes the name TLInterpolationKeyComponent. I have no idea how it's supposed to be distinguished in actual code
	else if ((*range.begin)->GetTypeName().GetStringView() == "TLInterpolationKeyComponent")
	{
		MakeDirectObjectRef(L, static_cast<aspk::TLMaterialComponent::TextureParameter*>(value), lifetime);
	}
	else
	{
		MakeDirectObjectRef(L, value, lifetime);
	}
}

END_NS()

#include <GameDefinitions/Render.h>

BEGIN_SE()

template <class T>
void Material::SetUniformParam(Material::UniformBindingData const& binding, T value)
{
	for (unsigned i = 0; i < std::size(ShaderDescriptions); i++) {
		if (binding.PerShaderCBOffsets[i] != -1 && ShaderDescriptions[i].MaterialCBSize > 0 && MaterialCBs[i].MaterialCB != nullptr) {
			*(T *)((uint8_t *)MaterialCBs[i].MaterialCB + binding.PerShaderCBOffsets[i]) = value;
		}
	}
}

bool Material::SetScalar(FixedString const& paramName, float value)
{
	for (auto& param : Parameters.ScalarParameters) {
		if (param.ParameterName == paramName) {
			param.Value = value;
			SetUniformParam(param.Binding, value);
			return true;
		}
	}

	ERR("Material binding has no float parameter named '%s'", paramName.GetString());
	return false;
}

bool Material::SetVector2(FixedString const& paramName, glm::vec2 value)
{
	for (auto& param : Parameters.Vector2Parameters) {
		if (param.ParameterName == paramName) {
			param.Value = value;
			SetUniformParam(param.Binding, value);
			return true;
		}
	}

	ERR("Material binding has no vec2 parameter named '%s'", paramName.GetString());
	return false;
}

bool Material::SetVector3(FixedString const& paramName, glm::vec3 value)
{
	for (auto& param : Parameters.Vector3Parameters) {
		if (param.ParameterName == paramName) {
			param.Value = value;
			SetUniformParam(param.Binding, value);
			return true;
		}
	}

	ERR("Material binding has no vec3 parameter named '%s'", paramName.GetString());
	return false;
}

bool Material::SetVector4(FixedString const& paramName, glm::vec4 value)
{
	for (auto& param : Parameters.VectorParameters) {
		if (param.ParameterName == paramName) {
			param.Value = value;
			SetUniformParam(param.Binding, value);
			return true;
		}
	}

	ERR("Material binding has no vec4 parameter named '%s'", paramName.GetString());
	return false;
}

void* Material::GetOrCreateConstantBuffer(uint8_t shaderIndex)
{
	assert(shaderIndex < std::size(ShaderDescriptions));

	if (MaterialCBs[shaderIndex].MaterialCB != nullptr) {
		return MaterialCBs[shaderIndex].MaterialCB;
	}

	auto size = ShaderDescriptions[shaderIndex].MaterialCBSize;
	auto cb = (uint8_t *)GameAllocRaw(size);
	memset(cb, 0, size);

	for (auto const& param : Parameters.ScalarParameters) {
		if (param.Binding.PerShaderCBOffsets[shaderIndex] != -1) {
			*(float*)(cb + param.Binding.PerShaderCBOffsets[shaderIndex]) = param.Value;
		}
	}

	for (auto const& param : Parameters.Vector2Parameters) {
		if (param.Binding.PerShaderCBOffsets[shaderIndex] != -1) {
			*(glm::vec2*)(cb + param.Binding.PerShaderCBOffsets[shaderIndex]) = param.Value;
		}
	}

	for (auto const& param : Parameters.Vector3Parameters) {
		if (param.Binding.PerShaderCBOffsets[shaderIndex] != -1) {
			*(glm::vec3*)(cb + param.Binding.PerShaderCBOffsets[shaderIndex]) = param.Value;
		}
	}

	for (auto const& param : Parameters.VectorParameters) {
		if (param.Binding.PerShaderCBOffsets[shaderIndex] != -1) {
			*(glm::vec4*)(cb + param.Binding.PerShaderCBOffsets[shaderIndex]) = param.Value;
		}
	}

	MaterialCBs[shaderIndex].MaterialCB = cb;
	MaterialCBs[shaderIndex].MaterialCBSize = size;
	return cb;
}

bool MaterialRenderingData::CheckConstantBuffer(Material& instance)
{
	if (MaterialCB == nullptr && MaterialCBSize > 0 && MaterialVkDescriptorSet != -1) {
		auto cb = instance.GetOrCreateConstantBuffer(ShaderIndex);
		MaterialCB = GameAllocRaw(MaterialCBSize);
		memcpy(MaterialCB, cb, MaterialCBSize);
	}

	return MaterialCB != nullptr;

}

template <class T>
void MaterialRenderingData::SetUniformParam(Material& instance, Material::UniformBindingData const& binding, T value)
{
	if (!CheckConstantBuffer(instance)) return;

	if (binding.PerShaderCBOffsets[ShaderIndex] != -1) {
		*(T*)((uint8_t*)MaterialCB + binding.PerShaderCBOffsets[ShaderIndex]) = value;
		// FIXME - VERY JANKY
		InstancingHash += rand();
	}
}

template <class T>
void ActiveMaterial::SetUniformParam(Material::UniformBindingData const& binding, T value)
{
	if (PrimaryRenderingData != nullptr) {
		PrimaryRenderingData->SetUniformParam(*Material, binding, value);
	}

	for (auto i = 0; i < std::size(RenderingData); i++) {
		RenderingData[i].SetUniformParam(*Material, binding, value);
	}
}

bool ActiveMaterial::SetScalar(FixedString const& paramName, float value)
{
	for (auto const& param : Material->Parameters.ScalarParameters) {
		if (param.ParameterName == paramName) {
			SetUniformParam(param.Binding, value);
			return true;
		}
	}

	ERR("Material binding has no float parameter named '%s'", paramName.GetString());
	return false;
}

bool ActiveMaterial::SetVector2(FixedString const& paramName, glm::vec2 value)
{
	for (auto const& param : Material->Parameters.Vector2Parameters) {
		if (param.ParameterName == paramName) {
			SetUniformParam(param.Binding, value);
			return true;
		}
	}

	ERR("Material binding has no vec2 parameter named '%s'", paramName.GetString());
	return false;
}

bool ActiveMaterial::SetVector3(FixedString const& paramName, glm::vec3 value)
{
	for (auto const& param : Material->Parameters.Vector3Parameters) {
		if (param.ParameterName == paramName) {
			SetUniformParam(param.Binding, value);
			return true;
		}
	}

	ERR("Material binding has no vec3 parameter named '%s'", paramName.GetString());
	return false;
}

bool ActiveMaterial::SetVector4(FixedString const& paramName, glm::vec4 value)
{
	for (auto const& param : Material->Parameters.VectorParameters) {
		if (param.ParameterName == paramName) {
			SetUniformParam(param.Binding, value);
			return true;
		}
	}

	ERR("Material binding has no vec4 parameter named '%s'", paramName.GetString());
	return false;
}

END_SE()
