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
	DEFINE_COMPONENT(ServerAnubisExecutor, "esv::AnubisExecutorComponent")

	__int64 field_18;
	__int64 field_20;
	uint8_t field_28;
	uint8_t field_29;
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