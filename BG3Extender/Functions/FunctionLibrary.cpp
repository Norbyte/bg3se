#include <stdafx.h>
#include <OsirisProxy.h>
#include <Functions/FunctionLibrary.h>
#include <Version.h>
#include <ScriptHelpers.h>
#include <fstream>
#include "json/json.h"

namespace bg3se::esv
{
	namespace func
	{
		void ShowErrorMessage(OsiArgumentDesc const & args)
		{
			auto message = args[0].String;
			auto wmsg = FromUTF8(message);
			gOsirisProxy->GetLibraryManager().ShowStartupError(wmsg, false, false);
		}

		bool GetVersion(OsiArgumentDesc & args)
		{
			args[0].Set((int32_t)CurrentVersion);
			return true;
		}

		bool LoadFile(OsiArgumentDesc & args)
		{
			auto path = args[0].String;
			auto & contents = args[1];

			auto loaded = script::LoadExternalFile(path, PathRootType::GameStorage);
			if (loaded) {
				contents.Set(gTempStrings.Make(*loaded));
				return true;
			} else {
				return false;
			}
		}

		void SaveFile(OsiArgumentDesc const & args)
		{
			auto path = args[0].String;
			auto contents = args[1].String;

			script::SaveExternalFile(path, PathRootType::GameStorage, contents);
		}

		void DoExperiment(OsiArgumentDesc const & args)
		{
			OsiErrorS("Nothing to see here");
		}
	}

	CustomFunctionLibrary::CustomFunctionLibrary(OsirisProxy & osiris)
		: osiris_(osiris)
	{}

	void CustomFunctionLibrary::Register()
	{
		auto & functionMgr = osiris_.GetCustomFunctionManager();
		functionMgr.BeginStaticRegistrationPhase();

		RegisterHelperFunctions();
		RegisterMathFunctions();
		RegisterStatFunctions();
		RegisterLuaFunctions();

		auto experiment = std::make_unique<CustomCall>(
			"NRD_Experiment",
			std::vector<CustomFunctionParam>{
				{ "Arg1", ValueType::String, FunctionArgumentDirection::In },
				{ "Arg2", ValueType::String, FunctionArgumentDirection::In },
				{ "Arg3", ValueType::String, FunctionArgumentDirection::In },
			},
			&func::DoExperiment
		);
		functionMgr.Register(std::move(experiment));

		auto showError = std::make_unique<CustomCall>(
			"NRD_ShowErrorMessage",
			std::vector<CustomFunctionParam>{
				{ "Message", ValueType::String, FunctionArgumentDirection::In }
			},
			&func::ShowErrorMessage
		);
		functionMgr.Register(std::move(showError));

		auto getVersion = std::make_unique<CustomQuery>(
			"NRD_GetVersion",
			std::vector<CustomFunctionParam>{
				{ "Version", ValueType::Integer, FunctionArgumentDirection::Out }
			},
			&func::GetVersion
		);
		functionMgr.Register(std::move(getVersion));

		auto loadFile = std::make_unique<CustomQuery>(
			"NRD_LoadFile",
			std::vector<CustomFunctionParam>{
				{ "Path", ValueType::String, FunctionArgumentDirection::In },
				{ "Contents", ValueType::String, FunctionArgumentDirection::Out }
			},
			&func::LoadFile
		);
		functionMgr.Register(std::move(loadFile));

		auto saveFile = std::make_unique<CustomCall>(
			"NRD_SaveFile",
			std::vector<CustomFunctionParam>{
				{ "Path", ValueType::String, FunctionArgumentDirection::In },
				{ "Contents", ValueType::String, FunctionArgumentDirection::In }
			},
			&func::SaveFile
		);
		functionMgr.Register(std::move(saveFile));

		functionMgr.EndStaticRegistrationPhase();
	}

	void CustomFunctionLibrary::PostStartup()
	{
		if (PostLoaded) {
			return;
		}

		using namespace std::placeholders;

		/*osiris_.GetLibraryManager().UIObjectManager__CreateUIObject.SetPostHook(
			std::bind(&CustomFunctionLibrary::OnCreateUIObject, this, _1, _2, _3, _4, _5, _6, _7, _8)
		);
		osiris_.GetLibraryManager().esv__Status__GetEnterChanceHook.SetWrapper(
			std::bind(&CustomFunctionLibrary::OnStatusGetEnterChance, this, _1, _2, _3)
		);
		osiris_.GetLibraryManager().esv__StatusHeal__Enter.SetPreHook(
			std::bind(&CustomFunctionLibrary::OnStatusHealEnter, this, _1)
		);
		osiris_.GetLibraryManager().esv__ActionMachine__SetState.AddPreHook(
			std::bind(&CustomFunctionLibrary::OnBeforeActionMachineSetState, this, _1, _2, _3, _4, _5, _6)
		);
		osiris_.GetLibraryManager().esv__ActionMachine__SetState.AddPostHook(
			std::bind(&CustomFunctionLibrary::OnActionMachineSetState, this, _1, _2, _3, _4, _5, _6, _7)
		);
		osiris_.GetLibraryManager().esv__ActionMachine__ResetState.SetPreHook(
			std::bind(&CustomFunctionLibrary::OnActionMachineResetState, this, _1, _2)
		);
		osiris_.GetLibraryManager().esv__ProjectileHelpers__ShootProjectile.SetWrapper(
			std::bind(&CustomFunctionLibrary::OnShootProjectile, _1, _2)
		);
		osiris_.GetLibraryManager().esv__Projectile__Explode.SetPreHook(
			std::bind(&CustomFunctionLibrary::OnProjectileExplode, this, _1)
		);

		osiris_.GetLibraryManager().RPGStats__ParseProperties.SetWrapper(
			std::bind(&CustomFunctionLibrary::OnParseSkillProperties, this, _1, _2, _3)
		);
		osiris_.GetLibraryManager().SkillPrototype__FormatDescriptionParam.SetWrapper(
			std::bind(&CustomFunctionLibrary::OnSkillFormatDescriptionParam, this, _1, _2, _3, _4, _5, _6, _7, _8, _9)
		);
		osiris_.GetLibraryManager().SkillPrototype__GetAttackAPCost.SetWrapper(
			std::bind(&CustomFunctionLibrary::OnGetSkillAPCost, this, _1, _2, _3, _4, _5, _6, _7)
		);
		osiris_.GetLibraryManager().StatusPrototype__FormatDescriptionParam.SetWrapper(
			std::bind(&CustomFunctionLibrary::OnStatusFormatDescriptionParam, this, _1, _2, _3, _4, _5, _6, _7, _8, _9)
		);
		osiris_.GetLibraryManager().esv__TurnManager__UpdateTurnOrder.SetPostHook(
			std::bind(&CustomFunctionLibrary::OnUpdateTurnOrder, this, _1, _2)
		);
		osiris_.GetLibraryManager().esv__ItemHelpers__GenerateTreasureItem.SetWrapper(
			std::bind(&CustomFunctionLibrary::OnGenerateTreasureItem, this, _1, _2, _3)
		);
		osiris_.GetLibraryManager().esv__CombineManager__ExecuteCombination.SetWrapper(
			std::bind(&CustomFunctionLibrary::OnCraftingExecuteCombination, this, _1, _2, _3, _4, _5, _6, _7, _8)
		);
		osiris_.GetLibraryManager().esv__ExecutePropertyDataOnGroundHit.SetPostHook(
			std::bind(&CustomFunctionLibrary::OnExecutePropertyDataOnGroundHit, this, _1, _2, _3, _4, _5)
		);
		osiris_.GetLibraryManager().esv__ExecutePropertyDataOnPositionOnly.SetPostHook(
			std::bind(&CustomFunctionLibrary::OnExecutePropertyDataOnPositionOnly, this, _1, _2, _3, _4, _5, _6, _7, _8)
		);
		osiris_.GetLibraryManager().esv__ExecuteCharacterSetExtraProperties.SetPostHook(
			std::bind(&CustomFunctionLibrary::OnExecuteCharacterSetExtraProperties, this, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12)
		);
		GetStaticSymbols().CharStatsGetters.WrapperHitChance.SetWrapper(
			std::bind(&CustomFunctionLibrary::OnGetHitChance, this, _1, _2, _3)
		);
		osiris_.GetLibraryManager().esv__LoadProtocol__HandleModuleLoaded.SetWrapper(
			std::bind(&CustomFunctionLibrary::OnPeerModuleLoaded, this, _1, _2, _3, _4)
		);
		osiris_.GetLibraryManager().App__OnInputEvent.SetPreHook(
			std::bind(&CustomFunctionLibrary::OnAppInputEvent, this, _1, _2, _3)
		);*/

		PostLoaded = true;
	}

	void CustomFunctionLibrary::OnBaseModuleLoadedServer()
	{
		DEBUG("CustomFunctionLibrary::OnBaseModuleLoadedServer(): Re-initializing module state.");
		auto & functionMgr = osiris_.GetCustomFunctionManager();
		functionMgr.ClearDynamicEntries();
		esv::ExtensionState::Get().LuaReset(true);
	}

	void CustomFunctionLibrary::OnBaseModuleLoadedClient()
	{
		DEBUG("CustomFunctionLibrary::OnBaseModuleLoadedClient(): Re-initializing module state.");
		ecl::ExtensionState::Get().LuaReset(true);
	}
}
