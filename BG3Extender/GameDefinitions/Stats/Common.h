#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>

namespace bg3se
{
	struct StatsFunctorSet;
	struct SpellPrototype;

	struct GameRandom
	{
		int* FixedRollList;
		int CurFixedRollIndex;
		int Steps;
		int RerolledLCG_X;
		int BucketCount;
		int LCG_X;
		int MinRoll;
		int StepSize;
		uint8_t field_24;
	};

	struct SpellIdBase : public HasObjectProxy
	{
		FixedString OriginatorPrototype;
		uint8_t _Pad[4];
		SpellSourceType SourceType;
		Guid ProgressionSource;
	};

	struct SpellId : public HasObjectProxy
	{
		FixedString OriginatorPrototype;
		uint8_t _Pad[4];
		SpellSourceType SourceType;
		Guid ProgressionSource;
		FixedString Prototype;
	};

	struct SpellIdWithPrototype : public SpellId
	{
		SpellPrototype* SpellProto;
	};

	/*
	struct TextBuffer
	{
		wchar_t * Buf;
		uint64_t Capacity;
		uint64_t Length;
		uint64_t Unknown;

		void Replace(WStringView replacement);
	};

	namespace eoc
	{
		struct Text
		{
			struct Param
			{
				int32_t PlaceholderOffset;
				int32_t Unknown[2];
				int32_t PlaceholderSize;
			};

			void * VMT;
			TextBuffer * Buf;
			uint32_t Unknown[2];
			Param Params[8];

			void ReplaceParam(int index, WStringView replacement);
		};
	}*/

	template <class T>
	struct CNamedElementManager : public Noncopyable<CNamedElementManager<T>>
	{
		void* VMT{ nullptr };
		ObjectSet<T *> Primitives;
		Map<FixedString, uint32_t> NameHashMap;
		uint32_t NextHandle{ 0 };
		uint32_t NumSomeItems{ 0 };

		void Add(FixedString const& name, T* elem)
		{
			NameHashMap.insert(name, Primitives.Size);
			Primitives.Add(elem);
			NextHandle++;
		}

		std::optional<int> FindIndex(char const * str) const
		{
			FixedString fs{ str };
			if (fs) {
				return FindIndex(fs);
			} else {
				return {};
			}
		}

		std::optional<int> FindIndex(FixedString const& str) const
		{
			auto ptr = NameHashMap.find(str);
			if (ptr != NameHashMap.end()) {
				return (int)ptr.Value();
			} else {
				return {};
			}
		}

		T * Find(int index) const
		{
			if (index < 0 || index >= (int)Primitives.Size) {
				return nullptr;
			} else {
				return Primitives.Buf[index];
			}
		}

		T * Find(char const * str) const
		{
			FixedString fs(str);
			if (fs) {
				return Find(fs);
			} else {
				return nullptr;
			}
		}

		T * Find(FixedString const& str) const
		{
			auto ptr = NameHashMap.find(str);
			if (ptr != NameHashMap.end()) {
				return Primitives.Buf[ptr.Value()];
			} else {
				return nullptr;
			}
		}
	};

	template <class T>
	struct CHandleArray : public ProtectedGameObject<CHandleArray<T>>
	{
		void* VMT;
		Vector<T*> Handles;
		__int64 field_20;
	};

	template <class T>
	struct CDivinityStatsList : public ProtectedGameObject<CDivinityStatsList<T>>
	{
		void * VMT;
		CHandleArray<T> Handles;
		struct CDivinityStats * Stats;

		T * Get(int32_t index) const
		{
			if (index >= 0 && index < Handles.Size()) {
				return Handles.Start[index];
			} else {
				return nullptr;
			}
		}
	};

	// FIXME
	struct CRPGStats_LevelMap : public Noncopyable<CRPGStats_LevelMap>
	{
		virtual ~CRPGStats_LevelMap() {}
		virtual void SetModifierList(int modifierListIndex, int modifierIndex) = 0;
		virtual void SetModifierList(FixedString const& modifierListName, FixedString const& modifierName) = 0;
		virtual int64_t GetScaledValue(int difficultyScale, int level) = 0;

		int ModifierListIndex{ -1 };
		int ModifierIndex{ -1 };
		int RPGEnumerationIndex{ -1 };
		FixedString Name;
	};

	struct CRPGStats_Requirement
	{
		RequirementType RequirementId;
		int IntParam;
		Guid TagParam;
		bool Not;
	};


	struct CRPGStats_Object : public Noncopyable<CRPGStats_Object>
	{
		struct StatsFunctorInfo
		{
			FixedString Name;
			StatsFunctorSet* Functor;
		};
		
		struct RollConditionInfo
		{
			FixedString Name;
			int ConditionsId;
		};

		void* VMT{ nullptr };
		Vector<int32_t> IndexedProperties;
		FixedString Name;
		TranslatedString Unknown;
		FixedString FS2;
		struct CDivinityStats* DivStats{ nullptr };
		VirtualMultiHashMap<FixedString, Array<StatsFunctorInfo>> StatsFunctors;
		VirtualMultiHashMap<FixedString, Array<RollConditionInfo>> RollConditions;
		FixedString AIFlags;
		ObjectSet<CRPGStats_Requirement> Requirements;
		uint64_t Unknown4[6];
		int32_t Using{ -1 };
		uint32_t ModifierListIndex{ 0 };
		uint32_t StatsId{ 0 };
		uint32_t Level{ 0 };

		struct RPGEnumeration* GetAttributeInfo(FixedString const& attributeName, int& attributeIndex);
		std::optional<STDString> GetString(FixedString const& attributeName);
		std::optional<int> GetInt(FixedString const& attributeName);
		std::optional<int> GetIntScaled(FixedString const& attributeName, int level);
		std::optional<float> GetFloat(FixedString const& attributeName);
		std::optional<int64_t> GetInt64(FixedString const& attributeName);
		std::optional<Guid> GetGuid(FixedString const& attributeName);
		std::optional<ObjectSet<FixedString>> GetFlags(FixedString const& attributeName);
		std::optional<Array<StatsFunctorInfo>*> GetStatsFunctors(FixedString const& attributeName);
		std::optional<Array<RollConditionInfo>*> GetRollConditions(FixedString const& attributeName);
		bool SetString(FixedString const& attributeName, const char* value);
		bool SetInt(FixedString const& attributeName, int32_t value);
		bool SetFloat(FixedString const& attributeName, std::optional<float> value);
		bool SetInt64(FixedString const& attributeName, int64_t value);
		bool SetGuid(FixedString const& attributeName, std::optional<Guid> value);
		bool SetFlags(FixedString const& attributeName, ObjectSet<STDString> const& value);
		bool SetStatsFunctors(FixedString const& attributeName, std::optional<Array<StatsFunctorInfo>> const& value);
		bool SetRollConditions(FixedString const& attributeName, std::optional<Array<RollConditionInfo>> const& value);

		void ToProtobuf(class MsgS2CSyncStat* msg) const;
		void FromProtobuf(MsgS2CSyncStat const& msg);
		void BroadcastSyncMessage(bool syncDuringLoading) const;
	};

	struct CRPGStats_ObjectInstance : public CRPGStats_Object
	{
		uint32_t InstanceId;
	};
}
