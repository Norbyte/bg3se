#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>

namespace bg3se
{
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
			NameHashMap.Insert(name, Primitives.Size);
			Primitives.Add(elem);
			NextHandle++;
		}

		std::optional<int> FindIndex(char const * str) const
		{
			auto fs = ToFixedString(str);
			if (fs) {
				return FindIndex(fs);
			} else {
				return {};
			}
		}

		std::optional<int> FindIndex(FixedString const& str) const
		{
			auto ptr = NameHashMap.Find(str);
			if (ptr != nullptr) {
				return (int)*ptr;
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
			auto ptr = NameHashMap.Find(str);
			if (ptr != nullptr) {
				return Primitives.Buf[*ptr];
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

	struct CRPGStats_Object : public Noncopyable<CRPGStats_Object>
	{
		void* VMT{ nullptr };
		Vector<int32_t> IndexedProperties;
		FixedString Name;
		uint64_t Unknown;
		uint32_t Unknown2;
		FixedString FS2;
		struct CDivinityStats* DivStats{ nullptr };
		uint64_t Unknown3[22];
		FixedString AIFlags;
		ObjectSet<void*, GameMemoryAllocator, true> Requirements; // CRPGStats_Requirement
		uint64_t Unknown4[6];
		int32_t Using{ -1 };
		uint32_t ModifierListIndex{ 0 };
		uint32_t StatsId{ 0 };
		uint32_t Level{ 0 };

		void ToProtobuf(class MsgS2CSyncStat* msg) const;
		void FromProtobuf(MsgS2CSyncStat const& msg);
		void BroadcastSyncMessage(bool syncDuringLoading) const;
	};

	struct CRPGStats_ObjectInstance : public CRPGStats_Object
	{
		uint32_t InstanceId;
	};
}
