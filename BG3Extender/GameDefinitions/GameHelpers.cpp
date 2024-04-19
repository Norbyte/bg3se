#include "stdafx.h"

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Symbols.h>
#include <Lua/Shared/Proxies/LuaPropertyMapHelpers.h>
//#include <GameDefinitions/Ai.h>
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

	/*
	FIXME
	RuntimeStringHandle::RuntimeStringHandle()
	{
		Handle = GFS.strNullStringHandle;
	}*/

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

	/*
	void TextBuffer::Replace(WStringView replacement)
	{
		if (Buf) {
			GameFree(Buf);
		}

		Buf = GameAllocArray<wchar_t>(replacement.size() + 1);
		memcpy(Buf, replacement.data(), sizeof(wchar_t) * (replacement.size() + 1));
		Capacity = replacement.size() + 1;
		Length = replacement.size();
	}

	void eoc::Text::ReplaceParam(int index, WStringView replacement)
	{
		if (index < 1 || index > std::size(Params)) {
			OsiWarnS("Param index out of bounds");
			return;
		}

		auto & param = Params[index - 1];
		if (param.PlaceholderOffset == -1 || param.PlaceholderSize == -1) {
			return;
		}

		STDWString s;
		s.resize((std::size_t)Buf->Length);
		memcpy(s.data(), Buf->Buf, sizeof(wchar_t) * Buf->Length);

		STDWString newS = s.substr(0, param.PlaceholderOffset);
		newS += replacement;
		newS += s.substr(param.PlaceholderOffset + param.PlaceholderSize);
		Buf->Replace(newS);

		for (auto i = 0; i < 8; i++) {
			if (i != index - 1
				&& Params[i].PlaceholderSize != -1
				&& Params[i].PlaceholderOffset > param.PlaceholderOffset) {
				Params[i].PlaceholderOffset = Params[i].PlaceholderOffset - param.PlaceholderSize + (uint32_t)replacement.size();
			}
		}

		param.PlaceholderOffset = -1;
		param.PlaceholderSize = -1;
	}*/

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


	/*void esv::ProxyProjectileHit::Destroy(bool b)
	{
		if (WrappedHit) {
			WrappedHit->Destroy(b);
		}

		this->~ProxyProjectileHit();
		GameDelete(this);
	}

	void esv::ProxyProjectileHit::OnHit(glm::vec3 const& position, ComponentHandle const& hitObject, Projectile* projectile)
	{
		if (WrappedHit) {
			DisableCrashReporting _;
			WrappedHit->OnHit(position, hitObject, projectile);
		}

		LuaServerPin lua(ExtensionState::Get());
		if (lua) {
			lua->OnProjectileHit(projectile, hitObject, position);
		}
	}

	void esv::ProxyProjectileHit::Visit(ObjectVisitor* visitor)
	{
		if (WrappedHit) {
			DisableCrashReporting _;
			WrappedHit->Visit(visitor);
		}
	}

	int esv::ProxyProjectileHit::GetTypeId()
	{
		if (WrappedHit) {
			return WrappedHit->GetTypeId();
		} else {
			// Use nonexistent ID to prevent instantiation after reload if our hook
			// somehow got into a savegame.
			return 255;
		}
	}


	void PendingStatuses::Add(esv::Status * status)
	{
		PendingStatus pend { status, false };
		statuses_.insert(std::make_pair(status->StatusHandle, pend));
	}

	void PendingStatuses::Remove(esv::Status * status)
	{
		auto it = statuses_.find(status->StatusHandle);
		if (it != statuses_.end()) {
			statuses_.erase(it);
		} else {
			OsiError("Attempted to remove non-pending status " << std::hex << (int64_t)status->StatusHandle);
		}
	}

	PendingStatus * PendingStatuses::Find(ComponentHandle owner, ComponentHandle handle)
	{
		auto it = statuses_.find(handle);
		if (it != statuses_.end()) {
			auto & status = it->second;
			if (owner == status.Status->TargetHandle) {
				return &status;
			} else {
				OsiError("Attempted to retrieve pending status " << std::hex << (int64_t)status.Status->StatusHandle
					<< " on wrong character!");
				return nullptr;
			}
		} else {
			return nullptr;
		}
	}*/

	esv::Status* esv::StatusMachine::GetStatus(FixedString const& statusId) const
	{
		for (auto status : Statuses) {
			if (status->StatusId == statusId) {
				return status;
			}
		}

		return nullptr;
	}

	/*ecl::Status* ecl::StatusMachine::GetStatus(StatusType type) const
	{
		for (auto status : Statuses) {
			if (status->GetStatusId() == type) {
				return status;
			}
		}

		return nullptr;
	}

	ecl::Status* ecl::StatusMachine::GetStatus(FixedString const& statusId) const
	{
		for (auto status : Statuses) {
			if (status->StatusId == statusId) {
				return status;
			}
		}

		return nullptr;
	}

	ecl::Status* ecl::StatusMachine::GetStatus(NetId netId) const
	{
		return FindByNetId(netId);
	}*/

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

	/*esv::Status * esv::Item::GetStatus(ComponentHandle statusHandle, bool returnPending) const
	{
		if (StatusMachine == nullptr) {
			return nullptr;
		}

		auto status = StatusMachine->GetStatus(statusHandle);
		if (status != nullptr) {
			return status;
		}

		if (returnPending) {
			ComponentHandle ownerHandle;
			this->GetComponentHandle(ownerHandle);

			auto pendingStatus = ExtensionState::Get().PendingStatuses.Find(ownerHandle, statusHandle);
			if (pendingStatus != nullptr) {
				return pendingStatus->Status;
			}
		}

		return nullptr;
	}

	esv::Status* esv::Item::GetStatus(NetId netId) const
	{
		if (StatusMachine == nullptr) {
			return nullptr;
		}

		return StatusMachine->GetStatus(netId);
	}

	ecl::Status* ecl::Character::GetStatus(ComponentHandle statusHandle) const
	{
		if (StatusMachine == nullptr) {
			return nullptr;
		}

		return StatusMachine->Get(statusHandle);
	}

	ecl::Status* ecl::Character::GetStatus(NetId netId) const
	{
		if (StatusMachine == nullptr) {
			return nullptr;
		}

		return StatusMachine->GetStatus(netId);
	}

	void HitDamageInfo::ClearDamage()
	{
		TotalDamage = 0;
		ArmorAbsorption = 0;
		DamageList.Clear();
	}

	void HitDamageInfo::ClearDamage(bg3se::DamageType damageType)
	{
		for (auto const& dmg : DamageList) {
			if (dmg.DamageType == damageType) {
				TotalDamage -= dmg.Amount;
			}
		}

		DamageList.ClearDamage(damageType);
	}

	void HitDamageInfo::AddDamage(bg3se::DamageType damageType, int32_t amount)
	{
		TotalDamage += amount;
		DamageList.AddDamage(damageType, amount);
	}

	void HitDamageInfo::CopyFrom(HitDamageInfo const& src)
	{
		Equipment = src.Equipment;
		TotalDamage = src.TotalDamage;
		DamageDealt = src.DamageDealt;
		DeathType = src.DeathType;
		DamageType = src.DamageType;
		AttackDirection = src.AttackDirection;
		ArmorAbsorption = src.ArmorAbsorption;
		LifeSteal = src.LifeSteal;
		EffectFlags = src.EffectFlags;
		HitWithWeapon = src.HitWithWeapon;
		DamageList.CopyFrom(src.DamageList);
	}


	void DamagePairList::ClearDamage(bg3se::DamageType damageType)
	{
		for (uint32_t i = 0; i < Size; i++) {
			if (Buf[i].DamageType == damageType) {
				Remove(i);
				i--;
			}
		}
	}

	void DamagePairList::AddDamage(DamageType damageType, int32_t amount)
	{
		if (amount == 0) return;

		bool added{ false };
		for (uint32_t i = 0; i < Size; i++) {
			if (Buf[i].DamageType == damageType) {
				auto newAmount = Buf[i].Amount + amount;
				if (newAmount == 0) {
					Remove(i);
				} else {
					Buf[i].Amount = newAmount;
				}

				added = true;
				break;
			}
		}

		if (!added && amount != 0) {
			TDamagePair dmg;
			dmg.DamageType = damageType;
			dmg.Amount = amount;
			if (!SafeAdd(dmg)) {
				OsiErrorS("DamageList capacity exceeded!");
			}
		}
	}


	namespace esv
	{
		char const * const sActionStateNames[] = {
			"None",
			nullptr,
			"Attack",
			"Die",
			"Hit",
			"Idle",
			"Animation",
			"PrepareSkill",
			"UseSkill",
			"PickUp",
			"Drop",
			"MoveItem",
			"KnockedDown",
			"Summoning",
			"Polymorphing",
			"UseItem",
			"CombineItem",
			"TeleportFall",
			"Sheath",
			"Unsheath",
			"Identify",
			"Repair",
			"Lockpick",
			"DisarmTrap",
			"Incapacitated",
			"JumpFlight",
			nullptr
		};


		PropertyMapBase * ActionState::GetPropertyMap()
		{
			switch (GetType()) {
			case ActionStateType::ASAttack:
				return &gASAttackPropertyMap;

			case ActionStateType::ASPrepareSkill:
				return &gASPrepareSkillPropertyMap;

			default:
				return nullptr;
			}
		}

		char const * ActionState::GetTypeName()
		{
			auto type = GetType();
			if (type <= ActionStateType::ASJumpFlight) {
				return sActionStateNames[(unsigned)type];
			} else {
				return nullptr;
			}
		}
	}
	*/
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
	/*

	bool ig::FlashObject::GetValueWorkaround(char const* path, ig::DataType desiredType, InvokeDataValue& value, int arrayIndex)
	{
		switch (desiredType) {
		case ig::DataType::None:
			return GetValueWorkaround(path, ig::DataType::Bool, value, arrayIndex)
				|| GetValueWorkaround(path, ig::DataType::Double, value, arrayIndex)
				|| GetValueWorkaround(path, ig::DataType::String, value, arrayIndex);

		// Bool and Double are implemented correctly in EoCApp
		case ig::DataType::Bool:
		case ig::DataType::Double:
			return GetValue(path, desiredType, value, arrayIndex);

		// Strings returned by the vanilla GetValue() are truncated at 512 bytes; for some use cases (status description etc.)
		// this is too short, so we use a non-static buffer size instead
		case ig::DataType::String:
		{
			ig::IggyValuePath val;
			auto const& sym = GetStaticSymbols();
			if (!sym.IgValuePathMakeNameRef(&val, IggyValue, path)) {
				return false;
			}

			if (arrayIndex >= 0) {
				sym.IgValuePathSetArrayIndex(&val, arrayIndex);
			}

			int resultLength{ 0 };
			// Get length of string
			if (sym.IgValueGetStringUTF8(&val, 0, 0, 0x10000, nullptr, &resultLength)) {
				return false;
			}

			// Fetch string directly to STDString buffer
			value.TypeId = ig::DataType::String;
			value.StringVal.resize(resultLength);
			return sym.IgValueGetStringUTF8(&val, 0, 0, resultLength, value.StringVal.data(), &resultLength) == 0;
		}

		default:
			ERR("Attempted to fetch Flash value with unknown type %d", desiredType);
			return false;
		}
	}

	UIObject* UIObjectManager::GetByType(int typeId) const
	{
		for (auto ui : UIObjects) {
			if (ui->Type == typeId) {
				return ui;
			}
		}

		return nullptr;
	}

	void UIObject::OnFunctionCalled(const char * a1, unsigned int a2, ig::InvokeDataValue * a3)
	{
		return GetStaticSymbols().EoCUI__vftable->OnFunctionCalled(this, a1, a2, a3);
	}

	void UIObject::OnCustomDrawCallback(void * a1)
	{
		return GetStaticSymbols().EoCUI__vftable->OnCustomDrawCallback(this, a1);
	}

	void UIObject::Destroy(bool a1)
	{
		return GetStaticSymbols().EoCUI__vftable->Destroy(this, a1);
	}

	void UIObject::SetHandle(ComponentHandle * a1)
	{
		return GetStaticSymbols().EoCUI__vftable->SetHandle(this, a1);
	}

	ComponentHandle * UIObject::GetHandle(ComponentHandle * a1)
	{
		return GetStaticSymbols().EoCUI__vftable->GetHandle(this, a1);
	}

	void UIObject::RequestDelete()
	{
		return GetStaticSymbols().EoCUI__vftable->RequestDelete(this);
	}

	void UIObject::SetOwnerPlayerId(uint64_t a1)
	{
		return GetStaticSymbols().EoCUI__vftable->SetOwnerPlayerId(this, a1);
	}

	void UIObject::SetPos(int * a1)
	{
		return GetStaticSymbols().EoCUI__vftable->SetPos(this, a1);
	}

	void UIObject::KeepWithin(int a1, int a2)
	{
		return GetStaticSymbols().EoCUI__vftable->KeepWithin(this, a1, a2);
	}

	void UIObject::Show()
	{
		return GetStaticSymbols().EoCUI__vftable->Show(this);
	}

	void UIObject::Hide()
	{
		return GetStaticSymbols().EoCUI__vftable->Hide(this);
	}

	const char * UIObject::GetDebugName()
	{
		return GetStaticSymbols().EoCUI__vftable->GetDebugName(this);
	}

	bool UIObject::IsControllerUI()
	{
		return GetStaticSymbols().EoCUI__vftable->IsControllerUI(this);
	}

	void UIObject::Init()
	{
		return GetStaticSymbols().EoCUI__vftable->Init(this);
	}

	void UIObject::InitAPI()
	{
		return GetStaticSymbols().EoCUI__vftable->InitAPI(this);
	}

	void UIObject::Update(float a1)
	{
		return GetStaticSymbols().EoCUI__vftable->Update(this, a1);
	}

	void UIObject::PostUpdate(float a1)
	{
		return GetStaticSymbols().EoCUI__vftable->PostUpdate(this, a1);
	}

	void UIObject::Render(void * a1, void * a2)
	{
		return GetStaticSymbols().EoCUI__vftable->Render(this, a1, a2);
	}

	void UIObject::RegisterInvokeNames()
	{
		return GetStaticSymbols().EoCUI__vftable->RegisterInvokeNames(this);
	}

	void UIObject::Resize()
	{
		return GetStaticSymbols().EoCUI__vftable->Resize(this);
	}

	void * UIObject::OnInputEvent(void * a1, void * a2)
	{
		return GetStaticSymbols().EoCUI__vftable->OnInputEvent(this, a1, a2);
	}

	uint8_t * UIObject::SendEventToFlash(uint8_t * a2, void *a3, unsigned int a4)
	{
		return GetStaticSymbols().EoCUI__vftable->SendEventToFlash(this, a2, a3, a4);
	}

	void * UIObject::OnInputEventText(void * a1, void * a2)
	{
		return GetStaticSymbols().EoCUI__vftable->OnInputEventText(this, a1, a2);
	}

	uint16_t * UIObject::OnUnlinkedInput(uint16_t * a1, uint32_t a2, uint16_t a3)
	{
		return GetStaticSymbols().EoCUI__vftable->OnUnlinkedInput(this, a1, a2, a3);
	}

	void UIObject::SetModifierStates(bool a1, bool a2, bool a3, bool a4)
	{
		return GetStaticSymbols().EoCUI__vftable->SetModifierStates(this, a1, a2, a3, a4);
	}

	bool UIObject::OnAPIPreResetDevice(void * a1)
	{
		return GetStaticSymbols().EoCUI__vftable->OnAPIPreResetDevice(this, a1);
	}

	bool UIObject::OnAPIPostResetDevice(void * a1)
	{
		return GetStaticSymbols().EoCUI__vftable->OnAPIPostResetDevice(this, a1);
	}

	void UIObject::OnControllerModeChanged()
	{
		return GetStaticSymbols().EoCUI__vftable->OnControllerModeChanged(this);
	}

	void UIObject::OnPlayerDisconnect(int a1)
	{
		return GetStaticSymbols().EoCUI__vftable->OnPlayerDisconnect(this, a1);
	}

	void UIObject::ReleaseRenderData()
	{
		return GetStaticSymbols().EoCUI__vftable->ReleaseRenderData(this);
	}

	void UIObject::PrepareRenderData()
	{
		return GetStaticSymbols().EoCUI__vftable->PrepareRenderData(this);
	}

	void UIObject::DoPrepareRenderData()
	{
		return GetStaticSymbols().EoCUI__vftable->DoPrepareRenderData(this);
	}

	void UIObject::Activate()
	{
		return GetStaticSymbols().EoCUI__vftable->Activate(this);
	}

	void UIObject::Deactivate()
	{
		return GetStaticSymbols().EoCUI__vftable->Deactivate(this);
	}

	void UIObject::LoseFocus()
	{
		return GetStaticSymbols().EoCUI__vftable->LoseFocus(this);
	}

	int64_t UIObject::GetBitmapHeight()
	{
		return GetStaticSymbols().EoCUI__vftable->GetBitmapHeight(this);
	}

	int64_t UIObject::GetBitmapWidth()
	{
		return GetStaticSymbols().EoCUI__vftable->GetBitmapWidth(this);
	}

	void * UIObject::GetCharacter()
	{
		return GetStaticSymbols().EoCUI__vftable->GetCharacter(this);
	}

	bool UIObject::SetPlayerHandle(ComponentHandle * handle)
	{
		return GetStaticSymbols().EoCUI__vftable->SetPlayerHandle(this, handle);
	}

	ComponentHandle * UIObject::GetPlayerHandle(ComponentHandle * handle)
	{
		return GetStaticSymbols().EoCUI__vftable->GetPlayerHandle(this, handle);
	}

	bool UIObject::Unknown1()
	{
		return GetStaticSymbols().EoCUI__vftable->Unknown1(this);
	}

	void UIObject::Unknown2()
	{
		return GetStaticSymbols().EoCUI__vftable->Unknown2(this);
	}

	void * UIObject::Unknown3()
	{
		return GetStaticSymbols().EoCUI__vftable->Unknown3(this);
	}

	void UIObject::Unknown4(void * a1)
	{
		return GetStaticSymbols().EoCUI__vftable->Unknown4(this, a1);
	}

	namespace ecl
	{
		EoCUI::EoCUI(bg3se::Path * path)
		{
			GetStaticSymbols().EoCUI__ctor(this, path);
		}
	}

	int16_t eoc::AiGrid::GetSurfaceIndex(AiGridTile* tile, uint8_t layer) const
	{
		if (tile->SurfaceIndexAndMeta == -1) {
			return -1;
		}

		int16_t surfaceIndex = -1;
		if (tile->SurfaceIndexAndMeta & 1) {
			uint64_t layerMask;
			if (layer == 0) {
				layerMask = AiGridTile::BaseSurfaceLayerMask;
			} else if (layer == 1) {
				layerMask = AiGridTile::CloudSurfaceLayerMask;
			} else {
				OsiError("Requested unknown surface layer!");
				layerMask = 0;
			}

			if (layerMask & tile->AiFlags) {
				surfaceIndex = tile->SurfaceIndexAndMeta >> 1;
			}
		} else {
			auto metadata = Surfaces[tile->SurfaceIndexAndMeta >> 1];
			if (layer == 0) {
				surfaceIndex = metadata->BaseSurfaceIndex;
			} else if (layer == 1) {
				surfaceIndex = metadata->CloudSurfaceIndex;
			} else {
				OsiError("Requested unknown surface layer!");
			}
		}

		return surfaceIndex;
	}

	eoc::AiMetaData* eoc::AiGrid::GetAiMetaData(AiGridTile* tile) const
	{
		if (tile->AiMetaDataIndex == -1) {
			return nullptr;
		}

		return AiMetaData[tile->AiMetaDataIndex];
	}*/
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
		//MakeDirectObjectRef(L, static_cast<resource::effects::Ribbon2Component*>(value), lifetime);
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
