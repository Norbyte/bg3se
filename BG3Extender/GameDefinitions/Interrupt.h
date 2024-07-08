#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Hit.h>

BEGIN_NS(interrupt)

struct InterruptIdentifier
{
	uint64_t field_0;
	uint64_t field_8;
	uint64_t field_10;

	inline bool operator == (InterruptIdentifier const& o) const
	{
		return field_0 == o.field_0
			&& field_8 == o.field_8
			&& field_10 == o.field_10;
	}
};

struct ResolveData
{
	struct Element
	{
		uint8_t A;
		uint8_t B;
	};

	int field_0;
	[[bg3::legacy(ResolvedRolls)]] Array<FixedRollBonus> FixedRollBonuses;
	std::optional<int> field_18;
	uint8_t field_20;
	Array<Element> Arr_2b;
	Array<Element> Arr2_2b;
	std::optional<int> field_48;
	uint8_t field_50;
};

struct ExecuteResult
{
	uint8_t field_0;
	MultiHashMap<Guid, ResolveData> ResolveData;
	[[bg3::hidden]]
	MultiHashMap<InterruptIdentifier, void*> MHM_u24_unk; // FIXME
};

struct InterruptType0
{
	FixedString field_0;
	Guid field_8;
	SpellId field_18;
};

struct InterruptType1
{
	__int64 field_0;
	Guid field_8;
	int field_18;
	uint8_t field_1C;
	int field_20;
	Array<EntityHandle> field_28;
	EntityHandle field_38;
	SpellId field_40;
	uint8_t field_68;
	uint8_t field_69;
};

struct InterruptType2
{
	Guid field_0;
	FixedString field_10;
	Guid field_18;
	uint8_t field_28;
	uint8_t _Pad[3];
	uint8_t field_2C;
	uint8_t field_2D;
	int field_30;
	int field_34;
	int field_38;
	uint8_t field_3C;
	uint8_t field_3D;
	int field_40;
	uint8_t field_44;
	int field_48;
	uint8_t field_4C;
	uint8_t _Pad2[3];
	uint8_t field_50;
	uint8_t field_51;
	uint8_t field_52;
	Guid field_58;
	SpellId field_68;
};

struct InterruptType3
{
	__int64 field_0;
	__int64 field_8;
	__int64 field_10;
	__int64 field_18;
	__int64 field_20;
	__int64 field_28;
	FixedString field_30;
	uint8_t field_34;
	__int64 field_38;
	__int64 field_40;
	__int64 field_48;
	int field_50;
	uint8_t field_54;
	uint8_t field_55;
};

struct InterruptType4
{
	FixedString field_0;
	glm::vec3 field_4;
};

struct InterruptType5
{
	FixedString field_0;
	glm::vec3 field_4;
};

struct InterruptType6
{
	Guid field_0;
	FixedString field_10;
	Guid field_18;
	STDString field_28;
	char field_40;
	int field_44;
	char field_48;
	char field_49;
	Guid field_50;
	char field_60;
	Guid field_68;
	char field_78;
	SpellId field_80;
};

struct InterruptType7
{
	Guid field_0;
	Guid field_10;
	FixedString field_20;
	Guid field_28;
	Guid field_38;
	uint8_t field_48;
	__int64 field_50;
	FixedString field_58;
	int field_5C;
};

struct InterruptType8
{
	Guid field_0;
	FixedString field_10;
	Guid field_18;
	Guid field_28;
	int field_38;
	__int64 field_40;
	char field_48;
	int field_4C;
};

struct InterruptEntities
{
	EntityHandle field_0;
	EntityHandle field_8;
};

struct InterruptVariant2
{
	std::variant<InterruptType0, InterruptType1, InterruptType2, InterruptType3, InterruptType4, InterruptType5, InterruptType6, InterruptType7, InterruptType8> Variant;
	EntityHandle Source;
	EntityHandle SourceProxy;
	EntityHandle Target;
	EntityHandle TargetProxy;
	Array<InterruptEntities> field_D0;
	std::optional<glm::vec3> SourcePos;
	std::optional<glm::vec3> TargetPos;
	uint8_t field_100;

	// FIXME - needs adjustment depending on variant
	inline bool operator == (InterruptVariant2 const& o) const
	{
		return Source == o.Source
			&& Target == o.Target;
	}
};

struct InterruptVariantContainer
{
	InterruptVariant2 Variant;
	bool field_108;
	InterruptVariant2 field_110;
	MultiHashMap<EntityHandle, MultiHashSet<EntityHandle>> MHM_EH_MHS_EH;
};

END_NS()

BEGIN_SE()

template <>
inline uint64_t MultiHashMapHash<interrupt::InterruptVariant2>(interrupt::InterruptVariant2 const& v)
{
	// FIXME - needs adjustment depending on variant
	return HashMulti(v.Source, v.Target);
}

END_SE()
