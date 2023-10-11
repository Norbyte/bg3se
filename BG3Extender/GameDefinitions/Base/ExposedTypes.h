#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

struct GameTime
{
	double Time;
	float DeltaTime;
	int32_t Ticks;
};

struct ActionOriginator
{
	Guid ActionGuid;
	FixedString PassiveId;
	FixedString StatusId;
	FixedString InterruptId;
	bool CanApplyConcentration;
};

struct SpellIdBase
{
	FixedString OriginatorPrototype;
	uint8_t _Pad[4];
	SpellSourceType SourceType;
	Guid ProgressionSource;
};

struct SpellId
{
	FixedString OriginatorPrototype;
	uint8_t _Pad[4];
	SpellSourceType SourceType;
	Guid ProgressionSource;
	FixedString Prototype;

	inline bool operator == (SpellId const& o) const
	{
		return
			OriginatorPrototype == o.OriginatorPrototype
			&& SourceType == o.SourceType
			&& ProgressionSource == o.ProgressionSource
			&& Prototype == o.Prototype;
	}
};

template <>
inline uint64_t MultiHashMapHash<SpellId>(SpellId const& v)
{
	return HashMulti(v.ProgressionSource, v.SourceType, v.Prototype, v.OriginatorPrototype);
}

struct SpellIdWithPrototype : public SpellId
{
	stats::SpellPrototype* SpellProto;
	std::optional<Guid> SpellCastSource;
};

struct DamagePair
{
	int32_t Amount;
	DamageType DamageType;
};

struct DiceValues
{
	DiceSizeId DiceValue;
	uint8_t AmountOfDices;
	int DiceAdditionalValue;
	uint8_t field_8;
};

struct BoostParameters
{
	FixedString Boost;
	STDString Params;
	STDString Params2;
};

struct BoostCause
{
	BoostCauseType Type;
	FixedString Cause;
	EntityHandle Entity;
	uint64_t field_10;
};

struct ResourceRollDefinition
{
	Guid field_0;
	uint8_t field_10;
};

END_SE()
