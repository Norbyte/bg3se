#pragma once

BEGIN_SE()

struct PathingComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Pathing;
	static constexpr auto EngineClass = "eoc::PathingComponent";

	struct Param
	{
		Array<int32_t> Values1;
		Array<float> Values2;
		Array<glm::vec3> Values3;
		int field_30;
	};


	RefMap<FixedString, glm::vec4> VectorParameters;
	RefMap<FixedString, Param> PathParameters;
	__int64 field_20;
	int field_28;
	FixedString field_2C;
	__int64 field_30;
	int field_38;
	int field_3C;
	uint8_t field_40;
	float field_44;
	int field_48;
	uint8_t field_4C;
};

struct SteeringComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Steering;
	static constexpr auto EngineClass = "eoc::SteeringComponent";

	glm::vec3 field_0;
	float field_C;
	float field_10;
	uint8_t field_14;
	float field_18;
	uint8_t field_1C;
};

END_SE()

BEGIN_NS(esv)

struct AnubisExecutorComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerAnubisExecutor;
	static constexpr auto EngineClass = "esv::AnubisExecutorComponent";

	__int64 field_18;
	__int64 field_20;
	uint8_t field_28;
	uint8_t field_29;
};

struct ReplicationDependencyComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerReplicationDependency;
	static constexpr auto EngineClass = "esv::replication::ReplicationDependencyComponent";

	EntityHandle Dependency;
};

struct ReplicationDependencyOwnerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerReplicationDependencyOwner;
	static constexpr auto EngineClass = "esv::replication::ReplicationDependencyOwnerComponent";

	Array<EntityHandle> Dependents;
};

END_NS()