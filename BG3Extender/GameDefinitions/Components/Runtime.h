#pragma once

BEGIN_SE()

struct PathingComponent : public BaseComponent
{
	DEFINE_COMPONENT(Pathing, "eoc::PathingComponent")

	struct Param
	{
		Array<int32_t> Values1;
		Array<float> Values2;
		Array<glm::vec3> Values3;
		int field_30;
	};


	RefMap<FixedString, glm::vec4> VectorParameters;
	RefMap<FixedString, Param> PathParameters;
	glm::vec3 field_20;
	FixedString MovementTiltToRemap;
	__int64 field_30;
	int field_38;
	int PathId;
	uint8_t Flags;
	float PathMovementSpeed;
	int field_48;
	uint8_t ServerControl;
};

struct SteeringComponent : public BaseComponent
{
	DEFINE_COMPONENT(Steering, "eoc::SteeringComponent")

	glm::vec3 field_0;
	[[bg3::legacy(field_C)]] float TargetRotation;
	[[bg3::legacy(field_10)]] float Speed;
	[[bg3::legacy(field_14)]] uint8_t RequestSteering;
	float field_18;
	uint8_t field_1C;
};

END_SE()

BEGIN_NS(esv)

struct AnubisExecutorComponent : public BaseProxyComponent
{
	DEFINE_COMPONENT(ServerAnubisExecutor, "esv::AnubisExecutorComponent")

	[[bg3::hidden]] void* field_0;
	[[bg3::hidden]] void* Task;
	uint8_t field_10;
	uint8_t field_11;
	[[bg3::hidden]] void* field_18; // Unknown?
};

struct ReplicationDependencyComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerReplicationDependency, "esv::replication::ReplicationDependencyComponent")

	EntityHandle Dependency;
};

struct ReplicationDependencyOwnerComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerReplicationDependencyOwner, "esv::replication::ReplicationDependencyOwnerComponent")

	Array<EntityHandle> Dependents;
};

END_NS()