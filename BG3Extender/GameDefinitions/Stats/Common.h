#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

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
	stats::SpellPrototype* SpellProto;
};

END_SE()

BEGIN_NS(stats)

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

template <class T>
struct CNamedElementManager : public Noncopyable<CNamedElementManager<T>>
{
	void* VMT{ nullptr };
	Array<T *> Primitives;
	MultiHashMap<FixedString, uint32_t> NameHashMap;
	uint32_t NextHandle{ 0 };
	uint32_t NumSomeItems{ 0 };

	void Add(FixedString const& name, T* elem)
	{
		NameHashMap.Set(name, Primitives.Size());
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
		auto ptr = NameHashMap.Find(str);
		if (ptr) {
			return (int)**ptr;
		} else {
			return {};
		}
	}

	T * Find(int index) const
	{
		if (index < 0 || index >= (int)Primitives.Size()) {
			return nullptr;
		} else {
			return Primitives[index];
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
		if (ptr) {
			return Primitives[**ptr];
		} else {
			return nullptr;
		}
	}
};

struct Requirement
{
	RequirementType RequirementId;
	int IntParam;
	Guid TagParam;
	bool Not;
};

struct Object : public Noncopyable<Object>
{
	struct FunctorInfo
	{
		FixedString Name;
		Functors* Functor;
	};
		
	struct RollCondition
	{
		FixedString Name;
		int ConditionsId;
	};

	void* VMT{ nullptr };
	Vector<int32_t> IndexedProperties;
	FixedString Name;
	MultiHashMap<FixedString, Array<FunctorInfo>> Functors;
	MultiHashMap<FixedString, Array<RollCondition>> RollConditions;
	FixedString AIFlags;
	Array<Requirement> Requirements;
	Array<FixedString> Arr2;
	Array<FixedString> ComboCategories;
	int32_t Using{ -1 };
	uint32_t ModifierListIndex{ 0 };
	uint32_t field_E8{ 0 };

	RPGEnumeration* GetAttributeInfo(FixedString const& attributeName, int& attributeIndex);
	std::optional<STDString> GetString(FixedString const& attributeName);
	std::optional<int> GetInt(FixedString const& attributeName);
	std::optional<float> GetFloat(FixedString const& attributeName);
	std::optional<int64_t> GetInt64(FixedString const& attributeName);
	std::optional<Guid> GetGuid(FixedString const& attributeName);
	std::optional<TranslatedString> GetTranslatedString(FixedString const& attributeName);
	std::optional<Array<FixedString>> GetFlags(FixedString const& attributeName);
	std::optional<Array<FunctorInfo>*> GetFunctors(FixedString const& attributeName);
	std::optional<Array<RollCondition>*> GetRollConditions(FixedString const& attributeName);
	bool SetString(FixedString const& attributeName, const char* value);
	bool SetInt(FixedString const& attributeName, int32_t value);
	bool SetFloat(FixedString const& attributeName, std::optional<float> value);
	bool SetInt64(FixedString const& attributeName, int64_t value);
	bool SetGuid(FixedString const& attributeName, std::optional<Guid> value);
	bool SetTranslatedString(FixedString const& attributeName, std::optional<TranslatedString> value);
	bool SetFlags(FixedString const& attributeName, Array<STDString> const& value);
	bool SetFunctors(FixedString const& attributeName, std::optional<Array<FunctorInfo>> const& value);
	bool SetRollConditions(FixedString const& attributeName, std::optional<Array<RollCondition>> const& value);

	void ToProtobuf(class MsgS2CSyncStat* msg) const;
	void FromProtobuf(MsgS2CSyncStat const& msg);
	void BroadcastSyncMessage(bool syncDuringLoading) const;
};

struct ObjectInstance : public Object
{
	uint32_t InstanceId;
};

END_NS()
