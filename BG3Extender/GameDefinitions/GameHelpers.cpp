#include "stdafx.h"

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Symbols.h>
#include <GameDefinitions/Enumerations.h>
//#include <GameDefinitions/Ai.h>
#include <Extender/ScriptExtender.h>

namespace bg3se
{
	StaticSymbols & GetStaticSymbols()
	{
		static auto sSymbols = new StaticSymbols();
		return *sSymbols;
	}

	void* MSVCMemoryAllocator::Alloc(std::size_t size)
	{
		return GetStaticSymbols().CrtAlloc(size);
	}

	void * GameAllocRaw(std::size_t size)
	{
		return GetStaticSymbols().EoCAlloc(size, 5, 0, 8);
	}

	void GameFree(void * ptr)
	{
		GetStaticSymbols().EoCFree(ptr, 7, 0);
	}

	void* CrtAllocRaw(std::size_t size)
	{
		return GetStaticSymbols().CrtAlloc(size);
	}

	void CrtFree(void* ptr)
	{
		GetStaticSymbols().CrtFree(ptr);
	}

	FixedString::FixedString(std::string_view str)
		: Index(NullIndex)
	{
		auto create = GetStaticSymbols().ls__FixedString__Create;
		if (create) {
			create(*this, str.data(), (int)str.length());
		}
	}

	FixedString::FixedString(char const* str)
		: Index(NullIndex)
	{
		auto create = GetStaticSymbols().ls__FixedString__Create;
		if (create) {
			create(*this, str, -1);
		}
	}

	char const* FixedString::GetString() const
	{
		if (Index <= 0xfffffffdu) {
			auto getter = GetStaticSymbols().ls__FixedString__GetString;
			if (getter) {
#if defined(_DEBUG)
				__try {
					return getter(*this);
				} __except (EXCEPTION_EXECUTE_HANDLER) {
					return "<<< EXCEPTION THROWN WHILE READING STRING >>>";
				}
#else
				return getter(*this);
#endif
			}
		}

		return "";
	}

	uint32_t FixedString::GetHash() const
	{
		if (Index <= 0xfffffffdu) {
			auto getter = GetStaticSymbols().ls__FixedString__GetString;
			if (getter) {
				auto str = getter(*this);
				if (str) {
					StringTableEntry const* entry = reinterpret_cast<StringTableEntry const*>(str - sizeof(StringTableEntry));
					return entry->Hash;
				}
			}
		}

		return 0;
	}

	void FixedString::IncRef()
	{
		if (Index <= 0xfffffffdu) {
			auto incRef = GetStaticSymbols().ls__FixedString__IncRef;
			if (incRef) {
				incRef(Index);
			}
		}
	}

	void FixedString::DecRef()
	{
		if (Index <= 0xfffffffdu) {
			auto decRef = GetStaticSymbols().ls__FixedString__DecRef;
			if (decRef) {
				decRef(Index);
			}
		}
	}

	unsigned int Primes[290] = {
		2, 3, 5, 7, 0x0B, 0x0D, 0x11, 0x13, 0x17, 0x1D, 0x1F, 0x25,
		0x29, 0x2B, 0x2F, 0x35, 0x3B, 0x3D, 0x43, 0x47, 0x49, 0x4F, 0x53,
		0x59, 0x61, 0x65, 0x71, 0x7F, 0x83, 0x95, 0x97, 0x0A3, 0x0AD,
		0x0B5, 0x0BF, 0x0D3, 0x0DF, 0x0E9, 0x0F1, 0x0FB, 0x107, 0x10F,
		0x119, 0x125, 0x133, 0x137, 0x14B, 0x15B, 0x161, 0x16F, 0x175,
		0x17F, 0x18D, 0x191, 0x1A3, 0x1A5, 0x1AF, 0x1BB, 0x1C9, 0x1CD,
		0x1DF, 0x1E7, 0x1EB, 0x1F7, 0x209, 0x21D, 0x22D, 0x233, 0x23B,
		0x24B, 0x251, 0x259, 0x265, 0x277, 0x281, 0x28D, 0x295, 0x2A1,
		0x2AB, 0x2B3, 0x2BD, 0x2CF, 0x2D7, 0x2DD, 0x2E7, 0x2EF, 0x2F9,
		0x305, 0x313, 0x31D, 0x329, 0x32B, 0x335, 0x347, 0x355, 0x35F,
		0x36D, 0x371, 0x38B, 0x38F, 0x3A1, 0x3A9, 0x3AD, 0x3B9, 0x3C7,
		0x3CB, 0x3D7, 0x3DF, 0x3F1, 0x41B, 0x44F, 0x47F, 0x4B1, 0x4EB,
		0x515, 0x5AB, 0x641, 0x679, 0x6AD, 0x6D9, 0x709, 0x79F, 0x7D3,
		0x805, 0x89B, 0x8CB, 0x905, 0x92F, 0x9C7, 0x9F7, 0x0A31, 0x0A61,
		0x0A93, 0x0AC1, 0x0AF1, 0x0B23, 0x0B57, 0x0B89, 0x0BB9, 0x0CB3,
		0x0CE5, 0x0D1F, 0x0D4F, 0x0D81, 0x0E17, 0x0E4B, 0x0E75, 0x0F0B,
		0x0F43, 0x0FD3, 0x1069, 0x109D, 0x1139, 0x1163, 0x119B, 0x122B,
		0x125F, 0x128F, 0x12C1, 0x1357, 0x138B, 0x13BB, 0x13ED, 0x1421,
		0x1459, 0x14E7, 0x151F, 0x15B5, 0x1645, 0x16DB, 0x170F, 0x1741,
		0x1777, 0x17A5, 0x17D5, 0x1807, 0x183B, 0x1871, 0x189D, 0x18D1,
		0x19CF, 0x19FD, 0x1A2D, 0x1AC9, 0x1AFB, 0x1B2F, 0x1B59, 0x1B91,
		0x1BBF, 0x1BEF, 0x1C27, 0x1C55, 0x1C8B, 0x1CB7, 0x1D53, 0x1D87,
		0x1DB3, 0x1E49, 0x1EDD, 0x1F0F, 0x1F49, 0x1F75, 0x1FA5, 0x20A1,
		0x2135, 0x2231, 0x2263, 0x22F7, 0x2329, 0x2363, 0x238F, 0x23BF,
		0x23F3, 0x2429, 0x24BB, 0x254F, 0x2581, 0x267B, 0x26AD, 0x2717,
		0x2905, 0x2AFB, 0x2CEF, 0x2EE7, 0x30D7, 0x32C9, 0x34C9, 0x36B9,
		0x38A7, 0x3AA5, 0x3C97, 0x3E81, 0x4087, 0x4273, 0x4465, 0x465D,
		0x4847, 0x4A39, 0x4C2D, 0x4E2B, 0x501B, 0x5209, 0x53FF, 0x55F3,
		0x57E5, 0x59DB, 0x5BD5, 0x5DC1, 0x5FBD, 0x61B5, 0x63B3, 0x6593,
		0x6785, 0x6983, 0x6B75, 0x6D61, 0x6F61, 0x7151, 0x733D, 0x753B,
		0x772D, 0x7925, 0x7B17, 0x7D03, 0x7EF7, 0x80F5, 0x82DF, 0x84E3,
		0x86C5, 0x88CF, 0x8AB3, 0x8CA7, 0x8EAB, 0x908B, 0x927D, 0x947B,
		0x9665, 0x986B, 0x9A4F, 0x9C49, 0x9E3B, 0x0A033, 0x0A223, 0x0A41D,
		0x0A60D, 0x0A7FB, 0x0A9FD, 0x0ABF1, 0x0ADD5, 0x0AFCF
	};

	unsigned int GetNearestLowerPrime(unsigned int num)
	{
		if (num < 2) return 2;
		if (num >= 45007) return 45007;

		unsigned start = 0;
		unsigned end = (unsigned)std::size(Primes);

		while (start < end) {
			auto mid = (start + end) >> 1;
			auto delta = (int64_t)num - Primes[mid];
			if (delta >= 0) {
				if (delta == 0) {
					return Primes[mid];
				}

				start = mid + 1;
			} else {
				end = (start + end) >> 1;
			}
		}

		return Primes[start];
	}

	unsigned int MultiHashMapPrimes[] = {
		0x35, 0x61, 0xC1, 0x185, 0x301, 0x607,
		0xC07, 0x1807, 0x3001, 0x6011, 0xC005, 
		0x1800D, 0x30005, 0x60019, 0xC0001, 0x180005,
		0x30000B, 0x60000D, 0xC00005, 0x1800013,
		0x3000005, 0x6000017, 0x0C000013, 0x18000005,
		0x30000059, 0x60000005, 0x400CCCCD
	};

	unsigned int GetNearestMultiHashMapPrime(unsigned int num)
	{
		for (auto i = 0; i < std::size(MultiHashMapPrimes); i++) {
			if (MultiHashMapPrimes[i] >= num) {
				return MultiHashMapPrimes[i];
			}
		}

		return MultiHashMapPrimes[std::size(MultiHashMapPrimes) - 1];
	}

	/*
	FIXME
	RuntimeStringHandle::RuntimeStringHandle()
	{
		Handle = GFS.strNullStringHandle;
	}*/

	void StaticSymbols::CanonicalizePath(STDString & path) const
	{
		if (path.find('\\') != std::string::npos) {
			WARN("Path contains backslashes: \"%s\"; canonical paths should only contain forward slashes.", path.c_str());
		}

		std::replace(path.begin(), path.end(), '\\', '/');
	}

	STDString StaticSymbols::ToPath(StringView path, PathRootType root, bool canonicalize) const
	{
		if (PathRoots == nullptr) {
			ERR("LibraryManager::ToPath(): Path root API not available!");
			return "";
		}

		STDString canonicalPath(path);
		if (canonicalize) {
			CanonicalizePath(canonicalPath);
		}

		auto rootPath = PathRoots[(unsigned)root];
		if (rootPath == nullptr) {
			ERR("StaticSymbols::ToPath(): Path roots not initialized!");
			return "";
		}

		STDString absolutePath(*rootPath);
		absolutePath += "/" + canonicalPath;
		return absolutePath;
	}

	bool StaticSymbols::FileExists(StringView path, PathRootType root, bool canonicalize) const
	{
		// TODO - implement using proper FS file exists call
		FileReaderPin reader(path, root, canonicalize);
		return reader.IsLoaded();
	}

	FileReader::FileReader(std::string_view path)
	{
		auto ctor = GetStaticSymbols().ls__FileReader__ctor;
		if (ctor) {
			Path p;
			p.Name = path;
			ctor(this, p, 2, 0);
		}
	}

	FileReader::~FileReader()
	{
		auto dtor = GetStaticSymbols().ls__FileReader__dtor;
		if (dtor) {
			dtor(this);
		}
	}

	FileReaderPin::FileReaderPin(std::string_view path)
		: reader_(std::make_unique<FileReader>(path))
	{
	}

	FileReaderPin::FileReaderPin(std::string_view path, PathRootType root, bool canonicalize)
	{
		auto roots = GetStaticSymbols().PathRoots;
		if (roots == nullptr) {
			OsiErrorS("Path roots not available!");
			return;
		}

		auto absolutePath = GetStaticSymbols().ToPath(path, root, canonicalize);
		reader_ = std::make_unique<FileReader>(absolutePath);
	}

	FileReaderPin::~FileReaderPin()
	{}

	STDString FileReaderPin::ToString() const
	{
		if (!IsLoaded()) {
			OsiErrorS("File not loaded!");
			return "";
		}

		STDString contents;
		contents.resize(Size());
		memcpy(contents.data(), Buf(), Size());
		return contents;
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

	uint32_t murmur3_32(const uint8_t* key, size_t len, uint32_t seed)
	{
		uint32_t h = seed;
		if (len > 3) {
			size_t i = len >> 2;
			do {
				uint32_t k;
				memcpy(&k, key, sizeof(uint32_t));
				key += sizeof(uint32_t);
				k *= 0xcc9e2d51;
				k = (k << 15) | (k >> 17);
				k *= 0x1b873593;
				h ^= k;
				h = (h << 13) | (h >> 19);
				h = h * 5 + 0xe6546b64;
			} while (--i);
		}
		if (len & 3) {
			size_t i = len & 3;
			uint32_t k = 0;
			do {
				k <<= 8;
				k |= key[i - 1];
			} while (--i);
			k *= 0xcc9e2d51;
			k = (k << 15) | (k >> 17);
			k *= 0x1b873593;
			h ^= k;
		}
		h ^= len;
		h ^= h >> 16;
		h *= 0x85ebca6b;
		h ^= h >> 13;
		h *= 0xc2b2ae35;
		h ^= h >> 16;
		return h;
	}

	uint32_t GlobalStringTable::Entry::Count() const
	{
		if (Next) {
			return StringPtrItems + Next->Count();
		}
		else {
			return StringPtrItems;
		}
	}

	char const * GlobalStringTable::Entry::Get(uint32_t i) const
	{
		if (i < StringPtrItems) {
			return StringPtrs[i];
		}
		else {
			return Next->Get(i - StringPtrItems);
		}
	}

	const char * GlobalStringTable::Find(char const * s, uint64_t length) const
	{
		/*auto const & entry = HashTable[Hash(s, length)];
		auto numItems = entry.Count();
		for (uint32_t i = 0; i < numItems; i++) {
			const char * str = entry.Get(i);
			if (str) {
				auto metadata = reinterpret_cast<FixedString::Metadata*>(const_cast<char*>(str - 0x10));
				if (metadata->Length == length && memcmp(s, str, length) == 0) {
					return str;
				}
			}
		}*/

		return nullptr;
	}

	uint32_t GlobalStringTable::Hash(char const * s, uint64_t length)
	{
		return murmur3_32((const uint8_t *)s, length, 0) % 0xFFF1;
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

	esv::Status * esv::StatusMachine::GetStatus(ComponentHandle handle) const
	{
		for (auto status : StackedStatuses) {
			if (status->StatusHandle == handle) {
				return status;
			}
		}

		return nullptr;
	}

	esv::Status* esv::StatusMachine::GetStatus(NetId netId) const
	{
		throw std::runtime_error("FIXME");
		// return FindByNetId(netId);
	}

	esv::Status* esv::StatusMachine::GetStatus(FixedString const& statusId) const
	{
		for (auto status : StackedStatuses) {
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


	esv::Status * esv::Character::GetStatus(ComponentHandle statusHandle/*, bool returnPending*/) const
	{
		if (StatusMachine == nullptr) {
			return nullptr;
		}

		auto status = StatusMachine->GetStatus(statusHandle);
		if (status != nullptr) {
			return status;
		}

		/* FIXME
		if (returnPending) {
			ComponentHandle ownerHandle;
			this->GetComponentHandle(ownerHandle);

			auto pendingStatus = ExtensionState::Get().PendingStatuses.Find(ownerHandle, statusHandle);
			if (pendingStatus != nullptr) {
				return pendingStatus->Status;
			}
		}*/

		return nullptr;
	}

	esv::Status* esv::Character::GetStatus(NetId netId) const
	{
		if (StatusMachine == nullptr) {
			return nullptr;
		}

		// Pending statuses have no NetID, so we can't check them here
		return StatusMachine->GetStatus(netId);
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
