#pragma once

BEGIN_SE()

struct GameTime
{
	double Time{ 0 };
	float DeltaTime{ 0 };
	int32_t Ticks{ 0 };
	double Unknown{ 0 };
};

struct ActionOriginator
{
	Guid ActionGuid;
	FixedString PassiveId;
	FixedString StatusId;
	FixedString InterruptId;
	bool CanApplyConcentration;
};

struct SpellMetaId
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
inline uint64_t HashMapHash<SpellId>(SpellId const& v)
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

struct RollDefinition
{
	DiceSizeId DiceValue;
	uint8_t AmountOfDices;
	int DiceAdditionalValue;
	[[bg3::legacy(field_8)]] bool DiceNegative;
};

struct BoostParameters
{
	FixedString Boost;
	FixedString Params;
	FixedString Params2;
};

struct BoostSource
{
	BoostSourceType Type;
	FixedString Cause;
	EntityHandle Entity;
	ComponentHandle Status;
};

struct BoostDescription
{
	FixedString field_0;
	FixedString field_4;
	FixedString field_8;
};

struct ResourceRollDefinition
{
	Guid field_0;
	uint8_t field_10;
};

END_SE()
