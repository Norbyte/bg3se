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
	int TaskType;
	CharacterTaskFlags Flags;
	CharacterTaskData StartData;
	CharacterTaskData EndData;
	bool Previewing;
	bool Ready;
	Array<EntityHandle> HighlightTargets;
};


struct CharacterMoveTask : public CharacterTask
{
	virtual bool NeedsMovement(std::optional<glm::vec3>) = 0;
	virtual bool CheckCanReachTarget() = 0;
	virtual EntityHandle& GetTarget(EntityHandle&) = 0;
	virtual bool GetTargetInfo(NavigationTargetInfo&) = 0;
	virtual bool GetPreviewTargetInfo(NavigationTargetInfo&) = 0;
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
	int BehaviourTransactionId;
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
	NavigationTargetInfo StartTargetInfo;
	NavigationTargetInfo EndTargetInfo;
};


END_NS()
