#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Stats/Common.h>

BEGIN_NS(ecl)

struct TaskInfo
{
	Array<TextLine> Texts;
	float field_10;
	bool field_14;
	bool Succeeded;
	bool field_16;
	bool HasSourceName_M;
	TranslatedString SourceName;
	EntityHandle field_28;
	EntityHandle APDescription;
	uint64_t Error;
	uint8_t field_40;
	int TaskType;
	int field_48;
	int NumberOfTargets;
	int HP;
	int MaximumTotalTargetHP;
	uint8_t IconType;
};

struct TaskPreviewInfo
{
	int SpellColor;
	int TargetingType;
};

struct CharacterTaskData
{
	glm::vec3 PreviewPosition;
	Array<stats::ActionResourceCost> TotalCosts;
	Array<stats::ActionResourceCost> Costs;
};

struct PreviewEffectList
{
	uint64_t NextEffectIndex;
	Array<EntityHandle> Effects;
};

struct PreviewEffectMap
{
	LegacyMap<FixedString, PreviewEffectList*> Effects;
	bool Updating;
};


struct CharacterTask : public ProtectedGameObject<CharacterTask>
{
	virtual ~CharacterTask() {}
	virtual CharacterTask* Create() = 0;
	virtual void RaiseFlags(CharacterTaskFlags flags) = 0;
	virtual void ClearFlags(CharacterTaskFlags flags) = 0;
	virtual void SetCharacter(Character* c) = 0;
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual bool CanExecute() = 0;
	virtual bool CanForceExecute() = 0;
	virtual int GetPriority(int previousPriority) = 0;
	virtual int GetExecutePriority(int previousPriority) = 0;
	virtual Array<stats::ActionResourceCost> GetActionCosts() = 0;
	virtual void GetAPWarning(TaskInfo&, bool) = 0;
	virtual uint64_t& GetError(uint64_t& error) = 0;
	virtual void RequestCursor() = 0;
	virtual void SetCursor() = 0;
	virtual void GetTaskInfo(TaskInfo&, void* /* TargetingInfo */) = 0;
	virtual uint16_t& OnInputEventText(uint16_t& result, input::InputEventText const& e) = 0;
	virtual bool EnterPreview() = 0;
	virtual void UpdatePreview(GameTime const&) = 0;
	virtual void ExitPreview() = 0;
	virtual bool IsPreviewing() = 0;
	virtual bool IsLimitedPreview() = 0;
	virtual bool InterruptsRunningTask() = 0;
	virtual bool GetShowPreview(TaskPreviewInfo&) = 0;
	virtual bool Enter() = 0;
	virtual bool Update(GameTime const&) = 0;
	virtual void Exit() = 0;
	virtual bool CanExit() = 0;
	virtual bool ShouldExitOnReset() = 0;
	virtual TranslatedString GetDescription() = 0;
	virtual TextLine GetDescriptionFormatted() = 0;
	virtual std::optional<Array<AiTilePos>> GetPreviewTiles() = 0;
	virtual EntityHandle& GetUsedGameObject(EntityHandle& eh) = 0;
	virtual bool IsReady() = 0;
	virtual EntityHandle& GetUsedGameObject2(EntityHandle& eh) = 0;
	virtual void GetAPDescription(TaskInfo& desc) = 0;

	TaskInfo TaskInfo;
	Character* Character;
	ecs::EntityRef CharacterEntity;
	CharacterTaskType TaskType;
	CharacterTaskFlags Flags;
	CharacterTaskData StartData;
	CharacterTaskData EndData;
	bool Previewing;
	bool Ready;
	Array<EntityHandle> HighlightTargets;
};


struct CharacterTask_ControllerSelection : public CharacterTask
{
	static constexpr auto Type = CharacterTaskType::ControllerSelection;
};


struct CharacterTask_MoveController : public CharacterTask
{
	static constexpr auto Type = CharacterTaskType::MoveController;
};


struct CharacterTask_DefaultTargetAction : public CharacterTask
{
	static constexpr auto Type = CharacterTaskType::DefaultTargetAction;

	EntityHandle Target;
	[[bg3::hidden]] void* TargetActionManager;
	bool ActionExecuted;
};


struct CharacterTask_SheathUnsheath : public CharacterTask
{
	static constexpr auto Type = CharacterTaskType::SheathUnsheath;

	bool Entered;
};


struct CharacterTask_MoveInDirection : public CharacterTask
{
	static constexpr auto Type = CharacterTaskType::MoveInDirection;

	glm::vec3 field_100;
	glm::vec3 field_10C;
	glm::vec3 field_118;
	glm::vec3 field_124;
	glm::vec3 field_130;
};


struct CharacterTask_Listen : public CharacterTask
{
	static constexpr auto Type = CharacterTaskType::Listen;

	EntityHandle ListenTo;
	EntityHandle PreviewListenTo;
};


struct CharacterTask_Item_Use_Remotely : public CharacterTask
{
	static constexpr auto Type = CharacterTaskType::ItemUseRemotely;

	CharacterTask_Item_Use_Remotely* Action;
	EntityHandle HighlightTarget;
};


struct CharacterTaskInteractItemData
{
	bool CannotInteract;
	bool HasHighlight;
	bool IsReachable;
};


struct CharacterTask_Item_Interact_Area : public CharacterTask
{
	static constexpr auto Type = CharacterTaskType::ItemInteractArea;

	LegacyRefMap<EntityHandle, CharacterTaskInteractItemData> Items;
	EntityHandle Effect;
	glm::vec3 AreaCenter;
	float UpdateTime;
	int field_128;
};


struct CharacterMoveTask : public CharacterTask
{
	virtual bool NeedsMovement(std::optional<glm::vec3>) = 0;
	virtual bool CheckCanReachTarget() = 0;
	virtual EntityHandle& GetTarget(EntityHandle&) = 0;
	virtual bool GetTargetInfo(navigation::TargetInfo&) = 0;
	virtual bool GetPreviewTargetInfo(navigation::TargetInfo&) = 0;
	virtual void OnCalculatePathStarted() = 0;
	virtual void OnCalculatePathFinished(AiPath*) = 0;
	virtual void OnMovePreviewDisabled() = 0;
	virtual void HandlePointAndClick(uint32_t PointAndClickActionType, EntityHandle const&) = 0;
	virtual void GeneratePreview(AiPath*, bool, bool) = 0;
	virtual void GenerateTargetCircleScaleAndColor(EntityHandle const&) = 0;
	virtual bool Unknown01() = 0;
	virtual bool TrimPath(AiPath&) = 0;
	virtual bool PrependPathToPickupEntity(AiPath*) = 0;
	virtual std::optional<glm::vec3> GetPickupEntityPosition() = 0;
	virtual EntityHandle& GetPickupEntity(EntityHandle&) = 0;

	float TargetCircleScale;
	int TargetCircleColor;
	PathSettings PathSettings;
	glm::vec3 PreviousPathTarget;
	glm::vec3 PreviousPathTarget2;
	int PathId;
	int PreviousPathId;
	int MoveBehaviourTransactionId;
	bool BehaviourUpdated;
	bool CanReachTarget;
	bool HasLimitPath;
	bool ShouldDrawPreview;
	bool PreviewPathCalculated;
	float PathDistance;
	float PathLimitDistance;
	uint8_t field_14C;
	Array<EntityHandle> TargetHighlights;
	PreviewEffectMap PreviewEffects;
	navigation::TargetInfo StartTargetInfo;
	navigation::TargetInfo EndTargetInfo;
};


struct CharacterTaskSpellResultData
{
	glm::vec3 Position;
	spell_cast::BaseTarget Target;
	uint8_t field_40;
};


struct CharacterTaskSpellFloodData
{
	uint16_t field_0;
	uint16_t field_2;
	glm::vec3 Position;
	spell_cast::BaseTarget Target;
};


struct MoveInSpellRangeSolution : public ProtectedGameObject<MoveInSpellRangeSolution>
{
	glm::vec3 field_0;
	int field_C;
	spell_cast::InitialTarget Target;
	__int64 field_70;
	__int64 field_78;
	float field_80;
	int field_94;
};


struct MoveInSpellRangeCache
{
	BYTE field_0;
	AiTilePos field_4;
	int field_C;
	__int64 field_10;
	uint8_t field_18;
	HashMap<AiTilePos, float> field_20;
	std::optional<AiTilePos> field_60;
	std::optional<MoveInSpellRangeSolution> Solution;
	[[bg3::hidden]] Queue<MoveInSpellRangeSolution> field_110;
	HashMap<EntityHandle, MoveInSpellRangeSolution> field_128;
};


struct CharacterTask_Spell : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::Spell;

	[[bg3::hidden]] void* LevelMapValues;
	[[bg3::hidden]] void* ActionResourceTypes;
	[[bg3::hidden]] void* ClassDescriptions;
	[[bg3::hidden]] void* FactionContainer;
	[[bg3::hidden]] GlobalSwitches* GlobalSwitches;
	SpellId Spell;
	EntityHandle SourceItem;
	PreviewEffectMap PreviewEffectMap;
	uint8_t SpellHoverPreviewType;
	spell_cast::BaseTarget Target;
	Array<EntityHandle> Targets;
	uint8_t SpellStateFlags;
	uint8_t RequestFlags;
	std::optional<CharacterTaskSpellFloodData> Flood;
	std::optional<CharacterTaskSpellResultData> Result;
	MoveInSpellRangeCache MoveInSpellRangeCache;
	std::optional<glm::vec3> DirectionToNextNode;
	std::optional<float> DistanceXZTotal;
	std::optional<glm::vec3> PreviewPosition;
	Guid SpellCastGuid;
	uint8_t SpellTargetingState;
	uint8_t PreviousSpellTargetingState;
	[[bg3::hidden]] CRITICAL_SECTION CS_;
	uint16_t PlayerId;
	// Editor only
	// MultiHashMap field_588_MHM_AiTilePos_float;
	// ecl::CharacterTask_Spell::FloodResult field_5C8;
	// char field_5E0_OPT;
	// ecl::CharacterTask_Spell::FloodResult field_5E4;
	// char field_5FC_OPT;
};


struct CharacterTask_Item_Use : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::ItemUse;

	CharacterTask_Item_Use* Task;
	EntityHandle Item;
	int32_t BehaviourTransactionId;
	uint8_t field_264;
	uint8_t field_265;
	bool BehaviourStarted;
	FixedString field_268;
};


struct CharacterTask_Dialog : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::Dialog;

	EntityHandle Target2;
	EntityHandle Target;
};


struct CharacterTask_Loot : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::Loot;

	EntityHandle Target;
};


struct CharacterTask_PickUp : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::PickUp;

	int16_t Inventory;
	int Amount;
	NetId TargetInventoryNetId;
	EntityHandle Item;
	int BehaviourTransactionId;
	float Timeout;
	int16_t Slot;
	uint8_t field_26E;
};


struct CharacterTask_Pickpocket : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::Pickpocket;

	EntityHandle TargetCharacter;
	EntityHandle HoverCharacter;
};


struct CharacterTask_MoveTo : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::MoveTo;

	navigation::TargetInfo Target;
	int MovementType;
	int16_t AiFloodId;
	navigation::TargetInfo NextTarget;
	bool NeedsTargetUpdate;
	[[bg3::hidden]] glm::aligned_vec3 HoverPosition;
	bool HasEntered;
	uint8_t field_33D;
	EntityHandle HoverTarget;
	EntityHandle TargetEffect;
	[[bg3::hidden]] void* EffectCallback;
	glm::vec3 TargetPos;
	glm::vec3 AlignedTargetPos;
	glm::vec3 HoverPos2;
	glm::vec3 GroundPosition;
	float HitBound;
	float StepHeight;
};


struct CharacterTask_Item_Move_Dummy : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::ItemMoveDummy;

	EntityHandle Item;
};


struct CharacterTask_Item_Move : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::ItemMove;

	Array<EntityHandle> PlacementStack;
	Array<glm::vec3> TargetLocations;
	Array<glm::quat> TargetRotations;
	int Amount;
	EntityHandle field_288;
	int BehaviourTransactionId;
	uint8_t field_294;
};


struct CharacterTask_Item_Combine_Dummy : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::ItemCombineDummy;

	EntityHandle Item1;
	EntityHandle HighlightTarget;
	EntityHandle Item2;
};


struct CharacterTask_Lockpick : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::Lockpick;

	EntityHandle Item;
	NetId ItemNetId;
	bool LockpickingStarted;
	bool TargetSelected;
	bool CanLockpick;
};


struct CharacterTask_Item_Combine : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::ItemCombine;

	Array<EntityHandle> Items;
	FixedString field_260;
	int BehaviourTransactionId;
	glm::vec3 Position;
	bool Entered_M;
	float field_278;
	uint8_t field_27C;
};


struct CharacterTask_Drop : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::Drop;

	Array<EntityHandle> Items;
	int field_260;
	glm::vec3 Position;
	glm::quat Rotation;
	float LastUpdate;
	bool NeedsPhysicsUpdate;
	bool DropCompleted;
	bool ItemDropRequested;
};


struct CharacterTask_Disarm_Trap : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::DisarmTrap;

	EntityHandle Target;
	EntityHandle DisarmToolkit2;
	EntityHandle DisarmToolkit;
	int DisarmRequested;
	bool TargetSelected;
	bool ForceSuccess_M;
};


struct CharacterTask_ClimbTo : public CharacterMoveTask
{
	static constexpr auto Type = CharacterTaskType::ClimbTo;

	EntityHandle Target;
};

END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(ecl::CharacterTask)
LUA_POLYMORPHIC(ecl::CharacterMoveTask)

END_NS()
