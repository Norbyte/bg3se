#pragma once

BEGIN_SE()

struct AABound
{
	glm::vec3 Min;
	glm::vec3 Max;
};


struct LocalTransform
{
	Transform LocalTransform;
	AABound LocalBound;
};


struct MoveableObject : ProtectedGameObject<MoveableObject>
{
	[[bg3::hidden]] void* VMT;
	Transform WorldTransform;
	LocalTransform* LocalTransform;
	[[bg3::hidden]] void* Scene;
	uint8_t DirtyFlags;
	AABound WorldBound;
	AABound BaseBound;
	glm::vec2 SceneSize_M;
	int SceneNodeIndex_M;
};


struct Visual : public MoveableObject
{
	struct ObjectDesc
	{
		RenderableObject* Renderable;
		uint8_t field_8;
	};


	struct Attachment
	{
		Visual* Visual;
		int field_8;
		int field_C;
		FixedString field_10;
		FixedString field_14;
		uint32_t Flags;
		uint8_t Type1;
		uint8_t Type2;
		uint8_t field_1E;
		uint8_t field_1F;
		FixedString field_20;
		glm::vec3 field_24;
	};


	struct [[bg3::hidden]] Bone
	{
		struct Remap
		{
			int16_t field_0;
			int16_t field_2;
		};

		FixedString SlotName;
		int field_4;
		uint64_t field_8;
		Remap* Indices;
		uint8_t IndicesCapacity;
		char NumIndices;
	};


	EntityHandle Handle;
	[[bg3::hidden]] Bone** Skeleton;
	[[bg3::hidden]] uint8_t BonesCapacity;
	[[bg3::hidden]] uint8_t BonesSize;
	Array<ObjectDesc> ObjectDescs;
	Array<float> LODDistances;
	Array<Attachment> Attachments;
	Visual* Parent;
	resource::VisualResource* VisualResource;
	[[bg3::hidden]] void* GameObject_M;
	[[bg3::hidden]] void* field_E0;
	[[bg3::hidden]] void* field_E8;
	uint32_t VisualFlags;
	uint16_t CullFlags;
	uint8_t field_F6;
	uint8_t field_F7;
	uint8_t OwnsNode;
	int16_t WrinkleBase;
	int16_t Wrinkle01;
	int16_t Wrinkle02;
	int16_t Wrinkle03;
	int16_t BlendShapeBase;
	ecs::EntityRef VisualEntity;
};


struct RenderPropertyList
{
	uint32_t RenderPasses;
	uint8_t OpaqueChannel;
	uint8_t AlphaChannel;
	uint8_t field_6;
	uint8_t field_7;
	uint32_t TransformType;
};


struct RenderableObject : public MoveableObject
{
	[[bg3::hidden]] void* Model;
	RenderPropertyList PropertyList;
	[[bg3::hidden]] void* field_98;
	[[bg3::hidden]] Array<void*> AppliedMaterials;
	[[bg3::hidden]] void* ActiveMaterial;
	[[bg3::hidden]] Array<void*> AppliedOverlayMaterials;
	uint8_t LOD;
	uint32_t _Pad;
	glm::vec4 MeshRandomData;
};


END_SE()
