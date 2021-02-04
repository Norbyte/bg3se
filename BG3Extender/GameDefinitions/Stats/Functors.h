#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/Stats/Common.h>

namespace bg3se
{
	struct StatsFunctorBase
	{
		struct FunctorVMT
		{
			void* Destroy;
			void* ParseParams;
			void* GetDescription;
			void* GetDescription2;
			void* GetTooltipText;
			void* Clone;
		};

		FunctorVMT* VMT;
		FixedString UniqueName;
		StatsFunctorActionId TypeId;
		StatsPropertyContext PropertyContext;
		int32_t StatsConditionsId;
		bool IsSelf;
		int field_1C;
	};


	struct StatsFunctorSet : public ProtectedGameObject<StatsFunctorSet>
	{
		struct BaseVMT
		{
			void* Destroy;
			void* ClearNextIndex;
			void* Clear;
			void* AddOrUpdate;
			StatsFunctorBase* (*GetByIndex)(StatsFunctorSet*, int64_t);
			StatsFunctorBase* (*GetByName)(StatsFunctorSet*, FixedString const&);
			void* field_30;
			void* (__fastcall* GetSize)(StatsFunctorSet*);
			StatsFunctorBase* (*GetByIndex2)(StatsFunctorSet*, int64_t);
			StatsFunctorBase* (*GetByIndex3)(StatsFunctorSet*, int64_t);
			void* UpdateNameMap;
		};

		virtual ~StatsFunctorSet() = 0;
		virtual void ClearNextIndex() = 0;
		virtual void Clear() = 0;
		virtual void AddOrUpdate(StatsFunctorBase*) = 0;
		virtual StatsFunctorBase* GetByIndex(int64_t) = 0;
		virtual StatsFunctorBase* GetByName(FixedString const&) = 0;
		virtual void Unknown_30() = 0;
		virtual int64_t GetSize() = 0;
		virtual StatsFunctorBase* GetByIndex2(int64_t) = 0;
		virtual StatsFunctorBase* GetByIndex3(int64_t) = 0;
		virtual void UpdateNameMap() = 0;

		ObjectSet<StatsFunctorBase*> FunctorList;
		Map<FixedString, StatsFunctorBase*> FunctorsByName_M;
		int NextFunctorIndex;
		FixedString UniqueName;
		FixedString UniqueName2;
	};


	struct StatsFunctorSetImpl : public StatsFunctorSet
	{
		~StatsFunctorSetImpl() override;
		void ClearNextIndex() override;
		void Clear() override;
		void AddOrUpdate(StatsFunctorBase*) override;
		StatsFunctorBase* GetByIndex(int64_t) override;
		StatsFunctorBase* GetByName(FixedString const&) override;
		void Unknown_30() override;
		int64_t GetSize() override;
		StatsFunctorBase* GetByIndex2(int64_t) override;
		StatsFunctorBase* GetByIndex3(int64_t) override;
		void UpdateNameMap() override;
	};
}
