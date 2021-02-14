#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Misc.h>

namespace bg3se::esv
{
	struct Level;
	struct Surface;

	struct SurfaceCell
	{
		glm::i16vec2 Position;
	};

	struct SubSurface : ProtectedGameObject<SubSurface>
	{
		Surface* Surface;
		uint64_t SurfaceStateFlags;
		PrimitiveSmallSet<SurfaceCell> SurfaceCells1;
		PrimitiveSmallSet<SurfaceCell> SurfaceCells2;
		uint16_t SurfaceIndex;
		uint8_t Position[2];
		int field_44;
	};

	struct Surface : ProtectedGameObject<Surface>
	{
		FixedString MyGuid_M;
		NetId NetID;
		EntityHandle MyHandle;
		SurfaceType SurfaceType;
		uint8_t Flags;
		UUID TeamId;
		EntityHandle field_30;
		EntityHandle OwnerHandle;
		float LifeTime;
		int8_t Level;
		uint8_t _Pad[3];
		bool LifeTimeOverride;
		bool IsControlledByConcentration;
		Array<SurfaceCell> PrimSetSurfaceCell;
		ObjectHandle SurfaceTransformActions[9];
		int16_t Index;
		int16_t SurfaceConcentrationTarget;
		void* SurfaceManager;
		bool NeedsSplitEvaluation;
		int32_t OwnershipTimer;
		char field_C8;
		int32_t StoryActionID;
		ActionOriginator Originator;
		FixedString Parent;
		ObjectSet<SubSurface*> SubSurfaces;
	};

	struct SurfaceAction;

	struct SurfaceActionVMT : ProtectedGameObject<SurfaceActionVMT>
	{
		void (*Destroy)(SurfaceAction*, bool);
		SurfaceActionType(*GetTypeId)(SurfaceAction*);
		bool (*Visit)(SurfaceAction* self, ObjectVisitor* visitor);
		int* (*Unknown)(SurfaceAction* self, int*, bool);
		void (*Enter)(SurfaceAction* self);
		void (*Update)(SurfaceAction* self, void* time);
		void (*Unknown2)(SurfaceAction* self);
		void (*Reset)(SurfaceAction* self);
		bool (*IsFinished)(SurfaceAction* self);
		bool (*Unknown3)(SurfaceAction* self);
	};

	struct SurfaceAction : ProtectedGameObject<SurfaceAction>
	{
		SurfaceActionVMT* VMT;
		Level* Level;
		int StoryActionID;
		ActionOriginator Originator;
		ObjectHandle MyHandle;
	};

	struct CreateSurfaceActionBase : public SurfaceAction
	{
		EntityHandle Owner;
		float Duration;
		char IsControlledByConcentration;
		glm::vec3 Position;
		SurfaceType SurfaceType;
		ObjectHandle SurfaceHandlesByType[62];
		Array<SurfaceCell>* SurfaceChanges[62];
		Array<SurfaceCell>* SurfaceCellsByLayer[2];
	};

	struct CreateSurfaceAction : public CreateSurfaceActionBase
	{
		bool InitialChangesPushed;
		float Radius;
		float ExcludeRadius;
		float MaxHeight;
		bool IgnoreIrreplacableSurfaces;
		bool CheckExistingSurfaces;
		uint64_t LineCheckBlock;
		float Timer;
		float GrowTimer;
		int GrowStep;
		int CurrentCellCount;
		Array<SurfaceCell> SurfaceCells;
		uint8_t SurfaceLayer;
		uint8_t field_491;
		SomeGuidId SomeGuidId;
		uint16_t SurfaceConcentrationTarget;
	};

	struct CreatePuddleAction : public CreateSurfaceActionBase
	{
		int SurfaceCells;
		int Step;
		float GrowSpeed;
		int field_454;
		uint8_t field_458;
		bool IgnoreIrreplacableSurfaces;
		__int64 CellAtGrow[3];
		__int64 ClosedCells[3];
		float GrowTimer;
	};

	struct ExtinguishFireAction : public CreateSurfaceActionBase
	{
		glm::vec3 Position;
		float Radius;
		float Percentage;
		float GrowTimer;
		float Step;
		Array<void*> field_468;
		__int64 SomeFixedArray[3];
	};

	struct ZoneActionParams
	{
		int Shape;
		float Radius;
		float ZoneParam;
		float FrontOffset;
		float MaxHeight;
		float Height;
		uint8_t Flags;
	};

	struct ZoneAction : public CreateSurfaceActionBase
	{
		SomeGuidId Originator;
		FixedString TextKey;
		glm::vec3 Target;
		ZoneActionParams Params;
		int field_49C;
		int GrowStep;
		__int64 field_4A8;
		int field_4B0;
		Array<SurfaceCell> SurfaceCells;
		Array<EntityHandle> Targets;
		Array<void*> field_4E8;
		int64_t CurrentCellCount;
		uint8_t Flags;
	};

	struct TransformSurfaceAction : public SurfaceAction
	{
		float Timer;
		char SurfaceTransformAction;
		char OriginSurface;
		char SurfaceLayer;
		float GrowCellPerSecond;
		char Finished;
		void* CellSearcher;
		EntityHandle OwnerHandle;
		__int64 field_58;
		int field_60;
		float SurfaceLifetime;
		RefMap<SurfaceType, ObjectHandle> SurfaceMap;
		RefMap<SurfaceType, PrimitiveSet<SurfaceCell>> SurfaceCellMap;
		ObjectSet<SurfaceCell> SurfaceRemoveGroundCellMap;
		ObjectSet<SurfaceCell> SurfaceRemoveCloudCellMap;
		bool PlayerCharacterNearby;
		__int64 field_C0;
		__int64 field_C8;
		__int64 field_D0;
		int field_D8;
		int field_DC;
		__int64 field_E0;
	};

	struct ChangeSurfaceOnPathAction : public CreateSurfaceActionBase
	{
		EntityHandle FollowHandle;
		float Radius;
		bool IsFinished;
		Array<SurfaceCell> SurfaceCells;
		bool IgnoreIrreplacableSurfaces;
		bool CheckExistingSurfaces;
		bool IgnoreOwnerCells;
	};

	struct RectangleSurfaceAction : public CreateSurfaceActionBase
	{
		glm::vec3 Target;
		int field_458;
		float SurfaceArea_M;
		float Width;
		float Length;
		float GrowTimer;
		float MaxHeight;
		int GrowStep;
		ObjectSet<void*> DamageList;
		DeathType DeathType;
		uint64_t LineCheckBlock;
		void* SkillProperties_M;
		int CurrentGrowTimer_M;
		Array<SurfaceCell> SurfaceCells;
		ObjectSet<EntityHandle> Characters;
		ObjectSet<EntityHandle> Items;
		int CurrentCellCount;
	};

	struct PolygonSurfaceAction : public CreateSurfaceActionBase
	{
		FixedString field_448;
		int field_44C;
		ObjectSet<glm::vec2> PolygonVertices;
		int field_468;
		ObjectSet<void*> DamageList;
		char field_488;
		__int64 field_490;
		int field_498;
		float field_49C;
		int field_4A0;
		int field_4A4;
		int field_4A8;
		int field_4AC;
		int field_4B0;
		int field_4B4;
		Array<SurfaceCell> SurfaceCells;
		ObjectSet<ObjectHandle> Characters;
		ObjectSet<ObjectHandle> Items;
		char field_500;
	};

	struct SurfaceActionFactory : public ObjectFactoryBase
	{
		using CreateActionProc = SurfaceAction * (SurfaceActionFactory* self, uint8_t surfaceType, uint64_t actionHandle);
		using AddActionProc = void (SurfaceActionFactory* self, SurfaceAction* action);
	};
}
