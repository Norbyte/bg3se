#pragma once

BEGIN_SE()

struct StatsExpressionParam
{
	using StatsExpressionVariableData = uint8_t;
	using StatusGroup = uint8_t;
	using StatsExpressionType = uint8_t;
	using StatsExpressionVariableDataType = uint8_t;
	using StatsExpressionVariableDataModifier = uint8_t;
	using StatsContextType = uint8_t;

	using Variant2 = std::variant<StatsExpressionVariableData, AbilityId, SkillId, StatusGroup, STDString>;
	using Param = std::variant<StatsExpressionType, Variant2, StatsExpressionVariableDataType, StatsExpressionVariableDataModifier, RollDefinition, ResourceRollDefinition, StatsContextType, int32_t, bool>;

	Array<Param> Params;
	STDString Code;
};

struct StatsExpressionParamEx : public StatsExpressionParam
{
	int FastLock;
};

struct StatsExpressionWithMetadata : public StatsExpressionParam
{
	uint8_t field_28;
	BoostCause Cause;
	TranslatedString field_48;
	uint8_t field_58;
};

struct LuaExpressionManager : public ProtectedGameObject<LuaExpressionManager>
{
	void* VMT;
	RefMap<Guid, StatsExpressionParamEx*> Expressions;
	char field_18;
};

END_SE()
