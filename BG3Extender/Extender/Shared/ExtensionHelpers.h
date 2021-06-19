#pragma once

#include <Osiris/Shared/CustomFunctions.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Item.h>
/*#include <GameDefinitions/GameAction.h>
#include <GameDefinitions/Projectile.h>
#include <GameDefinitions/TurnManager.h>*/

namespace bg3se
{
	ecl::EoCClient * GetEoCClient();
	esv::EoCServer * GetEoCServer();
	/*ModManager * GetModManagerClient();
	ModManager * GetModManagerServer();*/

	FixedString NameGuidToFixedString(char const * nameGuid);
	bool IsValidGuidString(const char * s);

	struct PropertyMapBase;

	namespace esv
	{
		/*esv::TurnBasedProtocol* GetTurnBasedProtocol();
		esv::Inventory* FindInventoryByHandle(ObjectHandle const& handle, bool logError = true);
		esv::GameAction* FindGameActionByHandle(ObjectHandle const& handle);

		PropertyMapBase & StatusToPropertyMap(esv::Status * status);

		struct ShootProjectileApiHelper
		{
			ShootProjectileHelper Helper;
			ShootProjectileHelperHitObject HitObject;
			DamagePairList DamageList;
			bool HasStartPosition{ false };
			bool HasEndPosition{ false };
			bool HasHitObject{ false };
			bool HasHitObjectPosition{ false };
			bool HasDamageList{ false };
			bool AlwaysDamage{ false };
			bool CanDeflect{ true };

			ShootProjectileApiHelper();
			void SetGuidString(FixedString const& prop, char const * value);
			void SetVector(FixedString const& prop, glm::vec3 const & value);
			void AddDamage(DamageType type, int32_t amount);
			bool Shoot();
		};


		struct DamageHelpers
		{
			static constexpr uint32_t HitHandleTypeId = 100;

			enum HelperType
			{
				HT_CustomHit = 0,
				HT_PrepareHitEvent = 1
			};

			ObjectHandle Handle;
			esv::Character * Source{ nullptr };
			IEoCServerObject * Target{ nullptr };

			HelperType Type{ HelperType::HT_CustomHit };
			HitDamageInfo * Hit{ nullptr };
			bool SimulateHit{ false };
			HitType HitType{ HitType::Melee };
			bool NoHitRoll{ false };
			bool ProcWindWalker{ false };
			bool ForceReduceDurability{ false };
			// TODO - SkillProperties
			HighGroundBonus HighGround{ HighGroundBonus::Unknown };
			CriticalRoll CriticalRoll{ CriticalRoll::Roll };

			// 0 - ASAttack
			// 1 - Character::ApplyDamage, StatusDying, ExecPropertyDamage, StatusDamage
			// 2 - AI hit test
			// 3 - Explode
			// 4 - Trap
			// 5 - InSurface
			// 6 - SetHP, osi::ApplyDamage, StatusConsume
			uint32_t HitReason{ 6 };
			FixedString SkillId;
			Vector3 ImpactPosition{ .0f };
			bool HasImpactPosition{ false };
			Vector3 ImpactOrigin{ .0f };
			bool HasImpactOrigin{ false };
			Vector3 ImpactDirection{ .0f };
			bool HasImpactDirection{ false };

			CauseType DamageSourceType{ CauseType::None };
			float Strength{ 0.0f };

			DamagePairList * DamageList{ nullptr };

			DamageHelpers();
			~DamageHelpers();

			void SetInternalDamageInfo();
			void SetExternalDamageInfo(HitDamageInfo * damageInfo, DamagePairList * damageList);

			void SetVector(FixedString const& prop, Vector3 const & value);
			void AddDamage(DamageType DamageType, int32_t Amount);
			esv::StatusHit * Execute();

		private:
			std::unique_ptr<HitDamageInfo> MyDamageInfo;
			std::unique_ptr<DamagePairList> MyDamageList;
		};*/
	}

	namespace ecl
	{
		//ecl::Inventory* FindInventoryByHandle(ObjectHandle const& handle, bool logError = true);
	}
}