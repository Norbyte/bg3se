#include "stdafx.h"

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Symbols.h>
#include <Lua/Shared/Proxies/LuaPropertyMapHelpers.h>
#include <Extender/ScriptExtender.h>
#include <GameDefinitions/Resources.h>

namespace bg3se
{
	EnumRegistry& EnumRegistry::Get()
	{
		static EnumRegistry reg;
		return reg;
	}

	void EnumRegistry::Register(EnumInfoStore<EnumUnderlyingType>* ei)
	{
		assert(EnumsByName.find(ei->EnumName) == EnumsByName.end());
		EnumsByName.insert(ei->EnumName, ei);
		ei->RegistryIndex = (int32_t)EnumsById.Size;
		EnumsById.Add(ei);
	}


	BitmaskRegistry& BitmaskRegistry::Get()
	{
		static BitmaskRegistry reg;
		return reg;
	}
	
	void BitmaskRegistry::Register(BitmaskInfoStore<EnumUnderlyingType>* ei)
	{
		assert(BitfieldsByName.find(ei->EnumName) == BitfieldsByName.end());
		BitfieldsByName.insert(ei->EnumName, ei);
		ei->RegistryIndex = (int32_t)BitfieldsById.Size;
		BitfieldsById.Add(ei);
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

	FixedString GameObjectTemplate::GetTemplateType()
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

	RefReturn<esv::Character> esv::Character::LuaGetSelf()
	{
		if (!CharacterGetSelfCalled) {
			CharacterGetSelfCalled = true;
			WARN("entity.ServerCharacter.Character is deprecated; use entity.ServerCharacter instead");
		}

		return this;
	}

	static bool ItemGetSelfCalled{ false };

	RefReturn<esv::Item> esv::Item::LuaGetSelf()
	{
		if (!ItemGetSelfCalled) {
			ItemGetSelfCalled = true;
			WARN("entity.ServerItem.Item is deprecated; use entity.ServerItem instead");
		}

		return this;
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

void LuaPolymorphic<stats::BaseFunctorExecParams>::MakeRef(lua_State* L, stats::BaseFunctorExecParams* value, LifetimeHandle const& lifetime)
{
#define V(type) case FunctorExecParamsType::type: \
			MakeDirectObjectRef(L, static_cast<stats::FunctorExecParams##type*>(value), lifetime); break;

	switch (value->ParamsTypeId) {
		V(Type1)
		V(Type2)
		V(Type3)
		V(Type4)
		V(Type5)
		V(Type6)
		V(Type7)
		V(Type8)

	default:
		MakeDirectObjectRef(L, value, lifetime);
		break;
	}

#undef V
}

void LuaPolymorphic<resource::AnimationResource::Event::PropertiesHolder>::MakeRef(lua_State* L, resource::AnimationResource::Event::PropertiesHolder* value, LifetimeHandle const& lifetime)
{
#define V(type) case AnimationEventPropertyType::type: \
			MakeDirectObjectRef(L, static_cast<resource::AnimationResource::Event::type##Properties*>(value), lifetime); break;

	switch (value->GetPropertyType()) {
		V(SoundObject)
		V(BoneEffect)
		V(Foot)
		V(BoneAttachBone)
		V(WeaponEffect)
		V(Unknown7)
		V(Unknown8)
		V(Prop9)
		V(Prop10)
		V(FloatData)
		V(EFoleySoundObject)
		V(EVocalSoundObject)
		V(U8Data)
		V(Unknown15)

	default:
		MakeDirectObjectRef(L, value, lifetime);
		break;
	}

#undef V
}

void LuaPolymorphic<resource::effects::Property>::MakeRef(lua_State* L, resource::effects::Property* value, LifetimeHandle const& lifetime)
{
#define V(type) case EffectPropertyType::type: \
			MakeDirectObjectRef(L, static_cast<resource::effects::type##Property*>(value), lifetime); break;

	switch (value->GetPropertyType()) {
		V(Bool)
		V(Int32)
		V(Int32Range)
		V(ColoredFrames)
		V(Float)
		V(FloatRange)
		V(Frames)
		V(STDString)
		V(Vector3)
		V(FunctionType)
		V(FixedString)
		V(Base)

	default:
		MakeDirectObjectRef(L, value, lifetime);
		break;
	}

#undef V
}

void LuaPolymorphic<resource::effects::FramesProperty::VirtualFrames>::MakeRef(lua_State* L, resource::effects::FramesProperty::VirtualFrames* value, LifetimeHandle const& lifetime)
{
#define V(type) case type: \
			MakeDirectObjectRef(L, static_cast<resource::effects::FramesProperty::Frames##type*>(value), lifetime); break;

	switch (value->GetType()) {
		V(0)
		V(1)

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

void LuaPolymorphic<resource::effects::EffectComponentBase>::MakeRef(lua_State* L, resource::effects::EffectComponentBase* value, LifetimeHandle const& lifetime)
{
#define V(type) else if (value->GetType().GetStringView() == #type) \
					MakeDirectObjectRef(L, static_cast<resource::effects::type##Component*>(value), lifetime);
#define V_timeline(type) else if (value->GetType().GetStringView() == #type) \
					MakeDirectObjectRef(L, static_cast<resource::effects::timeline::type##Component*>(value), lifetime);
#define V_tlkeybase(type) else if (value->GetType().GetStringView() == #type) \
					MakeDirectObjectRef(L, static_cast<resource::effects::timeline::type##Component<resource::effects::timeline::keys::KeyBase>*>(value), lifetime);
#define V_tlchannel(type) else if (value->GetType().GetStringView() == #type) \
					MakeDirectObjectRef(L, static_cast<resource::effects::timeline::channels::type##Component*>(value), lifetime);
	if (value->GetType().GetStringView() == "BaseComponent")
	{
		MakeDirectObjectRef(L, static_cast<resource::effects::BaseEffectComponent*>(value), lifetime);
	}
	// Special case for this one because it can't be its own V
	else if (value->GetType().GetStringView() == "Ribbon 2.0")
	{
		MakeDirectObjectRef(L, static_cast<resource::effects::Ribbon2Component*>(value), lifetime);
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
	V_timeline(TLBaseComponent)
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
	V_tlkeybase(TLEventKeyComponent)
	V_tlkeybase(TLInterpolationKeyComponent)
	V_tlkeybase(TLKeyBaseComponent)
	V_tlchannel(TimelineActorPropertiesReflectionKeyComponent)
	V_tlchannel(TLAtmosphereAndLightingChannel)
	V_tlchannel(TLCameraDoFChannel)
	V_tlchannel(TLCameraExposureChannel)
	V_tlchannel(TLMaterialKeyComponent)
	V_tlchannel(TLMaterialTextureKeyComponent)
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

void LuaPolymorphic<resource::effects::timeline::TLMaterialComponent::Parameter>::MakeRef(lua_State* L, resource::effects::timeline::TLMaterialComponent::Parameter* value, LifetimeHandle const& lifetime)
{
	resource::effects::timeline::TLMaterialComponent::Parameter::Range range;
	value->getRange(range);
	if (range.begin == nullptr || range.end == nullptr)
	{
		MakeDirectObjectRef(L, value, lifetime);
	}
	else if ((*range.begin)->GetType().GetStringView() == "TLMaterialKeyComponent")
	{
		MakeDirectObjectRef(L, static_cast<resource::effects::timeline::TLMaterialComponent::MaterialParameter*>(value), lifetime);
	}
	// For some reason, this takes the name TLInterpolationKeyComponent. I have no idea how it's supposed to be distinguished in actual code
	else if ((*range.begin)->GetType().GetStringView() == "TLInterpolationKeyComponent")
	{
		MakeDirectObjectRef(L, static_cast<resource::effects::timeline::TLMaterialComponent::TextureParameter*>(value), lifetime);
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
void MaterialInstance::SetUniformParam(MaterialInstance::UniformBindingData const& binding, T value)
{
	for (unsigned i = 0; i < std::size(ShaderDescriptions); i++) {
		if (binding.PerShaderCBOffsets[i] != -1 && ShaderDescriptions[i].MaterialCBSize > 0 && MaterialCBs[i].MaterialCB != nullptr) {
			*(T *)((uint8_t *)MaterialCBs[i].MaterialCB + binding.PerShaderCBOffsets[i]) = value;
		}
	}
}

bool MaterialInstance::SetScalar(FixedString const& paramName, float value)
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

bool MaterialInstance::SetVector2(FixedString const& paramName, glm::vec2 value)
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

bool MaterialInstance::SetVector3(FixedString const& paramName, glm::vec3 value)
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

bool MaterialInstance::SetVector4(FixedString const& paramName, glm::vec4 value)
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

void* MaterialInstance::GetOrCreateConstantBuffer(uint8_t shaderIndex)
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

bool MaterialParameterSet::CheckConstantBuffer(MaterialInstance& instance)
{
	if (MaterialCB == nullptr && MaterialCBSize > 0 && MaterialVkDescriptorSet != -1) {
		auto cb = instance.GetOrCreateConstantBuffer(ShaderIndex);
		MaterialCB = GameAllocRaw(MaterialCBSize);
		memcpy(MaterialCB, cb, MaterialCBSize);
	}

	return MaterialCB != nullptr;

}

template <class T>
void MaterialParameterSet::SetUniformParam(MaterialInstance& instance, MaterialInstance::UniformBindingData const& binding, T value)
{
	if (!CheckConstantBuffer(instance)) return;

	if (binding.PerShaderCBOffsets[ShaderIndex] != -1) {
		*(T*)((uint8_t*)MaterialCB + binding.PerShaderCBOffsets[ShaderIndex]) = value;
		// FIXME - VERY JANKY
		MaterialHash += rand();
	}
}

template <class T>
void ActiveMaterial::SetUniformParam(MaterialInstance::UniformBindingData const& binding, T value)
{
	if (DefaultParameterSet != nullptr) {
		DefaultParameterSet->SetUniformParam(*MaterialInstance, binding, value);
	}

	for (auto i = 0; i < std::size(ParameterSets); i++) {
		ParameterSets[i].SetUniformParam(*MaterialInstance, binding, value);
	}
}

bool ActiveMaterial::SetScalar(FixedString const& paramName, float value)
{
	for (auto const& param : MaterialInstance->Parameters.ScalarParameters) {
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
	for (auto const& param : MaterialInstance->Parameters.Vector2Parameters) {
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
	for (auto const& param : MaterialInstance->Parameters.Vector3Parameters) {
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
	for (auto const& param : MaterialInstance->Parameters.VectorParameters) {
		if (param.ParameterName == paramName) {
			SetUniformParam(param.Binding, value);
			return true;
		}
	}

	ERR("Material binding has no vec4 parameter named '%s'", paramName.GetString());
	return false;
}

END_SE()
