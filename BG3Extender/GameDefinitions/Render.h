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
	virtual uint32_t GetRTTI() = 0;
	virtual ~MoveableObject() = 0;

	virtual void SetWorldTranslate(glm::vec3 const& translate) = 0;
	virtual void SetWorldRotate(glm::quat const& rotate) = 0;
	virtual void SetWorldScale(glm::vec3 const& scale) = 0;
	virtual void SetWorldTransform(Transform const& transform) = 0;
	virtual void SetWorldTransform(glm::vec3 const& translate, glm::quat const& rotate, glm::vec3 const& scale) = 0;

	virtual void SetLocalTranslate(glm::vec3 const& translate) = 0;
	virtual void SetLocalRotate(glm::quat const& rotate) = 0;
	virtual void SetLocalScale(glm::vec3 const& scale) = 0;
	virtual void SetLocalTransform(Transform const& mat) = 0;
	virtual void SetLocalTransform(glm::vec3 const& translate, glm::quat const& rotate, glm::vec3 const& scale) = 0;

	virtual uint8_t CalculateTextureLOD(float*, glm::vec3 const& translate, float, uint8_t) = 0;
	virtual void SetTextureLOD(uint8_t, uint8_t, uint64_t) = 0;
	virtual uint16_t GetCullFlags(uint16_t&) = 0;
	virtual void SetSomeCullFlag(void*, bool) = 0;
	virtual void ToggleCullFlag600(bool cull) = 0;
	virtual bool AttachToScene(Scene* scene) = 0;
	virtual bool DetachFromScene() = 0;
	virtual bool IsAttachedToScene() = 0;
	virtual void NotifyBoundChange() = 0;
	virtual void UpdateBoundChange() = 0;

	Transform WorldTransform;
	LocalTransform* LocalTransform;
	[[bg3::hidden]] void* Scene;
	uint8_t DirtyFlags;
	AABound WorldBound;
	AABound BaseBound;
	glm::vec2 SceneSize_M;
	int SceneNodeIndex_M;
};


struct MaterialParameterSet
{
	EntityHandle Handle;
	EntityHandle field_8;
	uint64_t MaterialHash;
	uint8_t ShaderIndex;
	uint8_t field_19;
	uint16_t EngineCBSize;
	uint16_t MaterialCBSize;
	uint8_t field_1E;
	uint8_t EngineDxVsIndex;
	uint8_t field_20;
	uint8_t field_21;
	uint8_t field_22;
	uint8_t EngineDxPsIndex;
	uint8_t field_24;
	uint8_t EngineVkBindingIndex;
	uint8_t EngineVkDescriptorSet;
	uint8_t field_27;
	uint8_t MaterialDxVsIndex;
	uint8_t field_29;
	uint8_t field_2A;
	uint8_t field_2B;
	uint8_t MaterialDxPsIndex;
	uint8_t field_2D;
	uint8_t MaterialVkBindingIndex;
	uint8_t MaterialVkDescriptorSet;
	[[bg3::hidden]] void* MaterialCB;
	int field_38;
	int field_3C;
};


struct PrimaryMaterialParameterSet : public MaterialParameterSet
{
	__int64 field_40;
	__int64 field_48;
	__int64 field_50;
	__int64 field_58;
};


struct MaterialInstance
{
	struct ShaderParamBinding
	{
		uint8_t DxVsIndex;
		uint8_t DxPsIndex;
		uint8_t VkDescriptorSet;
		uint8_t VkBindingIndex;
	};


	struct DecalParameters
	{
		uint16_t DecalWorld;
		uint16_t DecalDimensions;
		uint16_t DecalTiling;
		uint16_t DecalDeferredProperties;
		uint16_t InvWorldMatrix;
		uint16_t NormalMatrix;
	};


	struct ShaderDescription
	{
		uint16_t EngineParamFlags;
		uint16_t EngineCBSize;
		uint16_t MaterialCBSize;
		ShaderParamBinding EngineCBBinding;
		ShaderParamBinding MaterialCBBinding;
		uint16_t WorldMatrixCurr;
		uint16_t WorldMatrixPrev;
		uint16_t TransformedVerticesOffsetCurr;
		uint16_t TransformedVerticesOffsetPrev;
		uint16_t MeshRandom;
		uint16_t WorldBoundsMinSubVisual;
		uint16_t WorldBoundsMinVisual;
		uint16_t WorldBoundsMinParent;
		uint16_t WorldBoundsMaxSubVisual;
		uint16_t WorldBoundsMaxVisual;
		uint16_t WorldBoundsMxaParent;
		uint16_t LocalBoundsMin;
		uint16_t LocalBoundsMax;
		uint16_t ReceiveDecal;
		uint16_t FadeOpacity;
		uint16_t LightChannel;
		uint16_t DiffusionProfileIndex;
		uint16_t VirtualTexture_TilesetDataIndex;
		uint16_t VirtualTexture_CB_Texture;
		uint16_t field_34;
		uint16_t field_36;
		DecalParameters* DecalParameters;
	};


	struct MaterialCB
	{
		[[bg3::hidden]] void* MaterialCB;
		uint64_t MaterialCBSize;
	};

	struct ParametersSet
	{
		MaterialInstance* Material;
		[[bg3::hidden]] Array<void*> ScalarParameters;
		[[bg3::hidden]] Array<void*> Vector2Parameters;
		[[bg3::hidden]] Array<void*> Vector3Parameters;
		[[bg3::hidden]] Array<void*> VectorParameters;
		[[bg3::hidden]] Array<void*> Texture2DParameters;
		[[bg3::hidden]] Array<void*> SamplerStateParameters;
		[[bg3::hidden]] Array<void*> VirtualTextureParameters;
	};



	[[bg3::hidden]] void* VMT;
	FixedString Name;
	std::array<ShaderDescription, 15> ShaderDescriptions;
	[[bg3::hidden]] uint64_t FastLock;
	[[bg3::hidden]] std::array<MaterialCB, 15> MaterialCBs;
	std::array<FixedString, 15> Shaders;
	uint32_t ShadingModel;
	uint32_t ForwardLightingMode;
	uint8_t BlendStateID;
	uint8_t RasterizerStateID;
	uint8_t DepthStateID;
	uint32_t field_510;
	uint32_t UVCount;
	uint32_t Flags;
	uint32_t UsedWithFlags;
	uint8_t MaterialPassHint;
	uint8_t MaterialType;
	uint8_t RenderChannel;
	uint8_t MaterialUsage;
	FixedString DiffusionProfileUUID;
	ParametersSet Parameters;
	MaterialInstance* Parent;
	uint64_t Version64;
};



struct ActiveMaterial
{
	struct Texture2DParam
	{
		[[bg3::hidden]] void* TextureResource;
		[[bg3::hidden]] __int64 field_8;
		FixedString Name;
		[[bg3::hidden]] char field_14;
		[[bg3::hidden]] char field_15;
		[[bg3::hidden]] char field_16;
		[[bg3::hidden]] char field_17;
	};


	PrimaryMaterialParameterSet* DefaultParameterSet;
	std::array<MaterialParameterSet, 5> ParameterSets;
	[[bg3::hidden]] std::array<void*, 13> SomeObjects;
	Array<Texture2DParam> Texture2DParams;
	[[bg3::hidden]] Array<void*> Arr_Element1;
	RenderableObject* RenderableObject;
	MaterialInstance* MaterialInstance;
	[[bg3::hidden]] __int64 field_1E0;
	[[bg3::hidden]] __int64 field_1E8;
	[[bg3::hidden]] __int64 field_1F0;
	[[bg3::hidden]] void* DiffusionProfile;
	uint8_t BlendFlags;
	uint8_t BlendStateID;
	uint8_t RasterizerStateID;
	uint8_t UVCount_M;
	[[bg3::hidden]] int field_204;
	FixedString MaterialName;
	uint8_t Flags;
	[[bg3::hidden]] uint8_t field_20D;
	[[bg3::hidden]] uint8_t field_20E;
};


struct AppliedMaterial : public ActiveMaterial
{
	glm::vec4 MeshVertexColor;
	glm::vec4 DynamicParameter;
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
		int BoneIndex1;
		int BoneIndex2;
		FixedString Bone1;
		FixedString Bone2;
		uint32_t Flags;
		bool HasBone1;
		bool HasBone2;
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
	[[bg3::hidden]] Visual* ParentVisual_M;
	Array<AppliedMaterial*> AppliedMaterials;
	AppliedMaterial* ActiveMaterial;
	Array<AppliedMaterial*> AppliedOverlayMaterials;
	uint8_t LOD;
	uint32_t _Pad;
	glm::vec4 MeshRandomData;
};


END_SE()
