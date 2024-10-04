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
	float MinLODDistance;
	float MaxLODDistance;
	int CullDataHandleIndex;
};


struct Material : public ProtectedGameObject<Material>
{
	struct ShaderParamBinding
	{
		int8_t DxVsIndex;
		int8_t DxPsIndex;
		int8_t VkDescriptorSet;
		int8_t VkBindingIndex;
	};


	struct DecalParameters
	{
		int16_t DecalWorld;
		int16_t DecalDimensions;
		int16_t DecalTiling;
		int16_t DecalDeferredProperties;
		int16_t InvWorldMatrix;
		int16_t NormalMatrix;
	};


	struct ShaderDescription
	{
		uint16_t EngineParamFlags;
		uint16_t EngineCBSize;
		uint16_t MaterialCBSize;
		ShaderParamBinding EngineCBBinding;
		ShaderParamBinding MaterialCBBinding;
		int16_t WorldMatrixCurr;
		int16_t WorldMatrixPrev;
		int16_t TransformedVerticesOffsetCurr;
		int16_t TransformedVerticesOffsetPrev;
		int16_t MeshRandom;
		int16_t WorldBoundsMinSubVisual;
		int16_t WorldBoundsMinVisual;
		int16_t WorldBoundsMinParent;
		int16_t WorldBoundsMaxSubVisual;
		int16_t WorldBoundsMaxVisual;
		int16_t WorldBoundsMxaParent;
		int16_t LocalBoundsMin;
		int16_t LocalBoundsMax;
		int16_t ReceiveDecal;
		int16_t FadeOpacity;
		int16_t LightChannel;
		int16_t DiffusionProfileIndex;
		int16_t VirtualTexture_TilesetDataIndex;
		int16_t VirtualTexture_CB_Texture;
		uint16_t field_34;
		uint16_t field_36;
		DecalParameters* DecalParameters;
	};


	struct MaterialCB
	{
		[[bg3::hidden]] void* MaterialCB;
		uint64_t MaterialCBSize;
	};

	struct UniformBindingData
	{
		FixedString UniformName;
		std::array<int16_t, 15> PerShaderCBOffsets;
	};

	struct TextureBindingData
	{
		FixedString UniformName;
		std::array<ShaderParamBinding, 15> PerShaderBindings;
	};

	struct VirtualTextureShaderParamBinding
	{
		ShaderParamBinding Cache;
		ShaderParamBinding Cache1;
		ShaderParamBinding Cache2;
		ShaderParamBinding Cache3;
	};
	
	struct VirtualTextureBindingData
	{
		FixedString UniformName;
		std::array<ShaderParamBinding, 15> PerShaderBindings;
		std::array<VirtualTextureShaderParamBinding, 15> PerShaderVTBindings;
	};



	struct ScalarParameter : public resource::MaterialResource::ScalarParameter
	{
		[[bg3::hidden]] UniformBindingData Binding;
	};

	struct Vector2Parameter : public resource::MaterialResource::Vector2Parameter
	{
		[[bg3::hidden]] UniformBindingData Binding;
	};

	struct Vector3Parameter : public resource::MaterialResource::Vector3Parameter
	{
		[[bg3::hidden]] UniformBindingData Binding;
	};

	struct Vector4Parameter : public resource::MaterialResource::Vector4Parameter
	{
		[[bg3::hidden]] UniformBindingData Binding;
		[[bg3::hidden]] uint64_t _Padding3;
	};

	struct Texture2DParameter : public resource::MaterialResource::Texture2DParameter
	{
		[[bg3::hidden]] TextureBindingData Binding;
	};

	struct SamplerStateParameter : public resource::MaterialResource::Parameter
	{
		uint8_t TextureFilterOverride;
		uint8_t TextureAddressMode;
		[[bg3::hidden]] TextureBindingData Binding;
	};

	struct VirtualTextureParameter : public resource::MaterialResource::VirtualTextureParameter
	{
		[[bg3::hidden]] VirtualTextureBindingData Binding;
	};

	struct ParametersSet
	{
		Material* Material;
		Array<ScalarParameter> ScalarParameters;
		Array<Vector2Parameter> Vector2Parameters;
		Array<Vector3Parameter> Vector3Parameters;
		Array<Vector4Parameter> VectorParameters;
		Array<Texture2DParameter> Texture2DParameters;
		Array<SamplerStateParameter> SamplerStateParameters;
		Array<VirtualTextureParameter> VirtualTextureParameters;
	};



	[[bg3::hidden]] void* VMT;
	FixedString Name;
	[[bg3::hidden]] std::array<ShaderDescription, 15> ShaderDescriptions;
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
	Material* Parent;
	uint64_t Version64;

	//# P_FUN(SetScalar, Material::SetScalar)
	//# P_FUN(SetVector2, Material::SetVector2)
	//# P_FUN(SetVector3, Material::SetVector3)
	//# P_FUN(SetVector4, Material::SetVector4)
	bool SetScalar(FixedString const& param, float value);
	bool SetVector2(FixedString const& param, glm::vec2 value);
	bool SetVector3(FixedString const& param, glm::vec3 value);
	bool SetVector4(FixedString const& param, glm::vec4 value);

	template <class T>
	void SetUniformParam(UniformBindingData const& binding, T value);

	void* GetOrCreateConstantBuffer(uint8_t shaderIndex);
};


struct MaterialRenderingData
{
	EntityHandle Handle;
	EntityHandle field_8;
	uint64_t InstancingHash;
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
	[[bg3::hidden]] uint64_t MaterialCBBufferSize;

	template <class T>
	void SetUniformParam(Material& instance, Material::UniformBindingData const& binding, T value);
	bool CheckConstantBuffer(Material& instance);
};


struct PrimaryMaterialRenderingData : public MaterialRenderingData
{
	__int64 field_40;
	__int64 field_48;
	__int64 field_50;
	__int64 field_58;
};



struct ActiveMaterial
{
	struct Texture2DParam
	{
		[[bg3::hidden]] void* TextureResource;
		// Not here in dx11 build
		[[bg3::hidden]] void* VkTextureResource;
		FixedString Name;
		int16_t Index;
		bool IsStub;
		[[bg3::hidden]] uint8_t field_17;
	};

	struct VirtualTextureParam
	{
		FixedString ParameterName;
		FixedString ID;
		[[bg3::hidden]] void* VirtualTextureResource;
		int16_t Index;
	};


	[[bg3::hidden]] PrimaryMaterialRenderingData* PrimaryRenderingData;
	[[bg3::hidden]] std::array<MaterialRenderingData, 5> RenderingData;
	[[bg3::hidden]] std::array<void*, 13> PipelineStates;
	[[bg3::hidden]] Array<Texture2DParam> Texture2DParams;
	Array<VirtualTextureParam> VirtualTextureParams;
	RenderableObject* RenderableObject;
	[[bg3::legacy(MaterialInstance)]] Material* Material;
	[[bg3::hidden]] __int64 field_1E0;
	[[bg3::hidden]] __int64 ShaderCallback;
	uint64_t Hash;
	[[bg3::hidden]] void* DiffusionProfile;
	uint8_t DirtyFlags;
	uint8_t BlendStateID;
	uint8_t RasterizerStateID;
	uint8_t DebugMaterial;
	float OverlayOffset;
	FixedString MaterialName;
	AppliedMaterialFlags Flags;
	bool Initialized;
	uint8_t AlphaChannel;

	//# P_FUN(SetScalar, ActiveMaterial::SetScalar)
	//# P_FUN(SetVector2, ActiveMaterial::SetVector2)
	//# P_FUN(SetVector3, ActiveMaterial::SetVector3)
	//# P_FUN(SetVector4, ActiveMaterial::SetVector4)
	bool SetScalar(FixedString const& param, float value);
	bool SetVector2(FixedString const& param, glm::vec2 value);
	bool SetVector3(FixedString const& param, glm::vec3 value);
	bool SetVector4(FixedString const& param, glm::vec4 value);

	template <class T>
	void SetUniformParam(Material::UniformBindingData const& binding, T value);
};


struct AppliedMaterial : public ActiveMaterial
{
	using LoadTextureProc = TextureDescriptor* (void* self, FixedString const& textureGuid);

	glm::vec4 MeshVertexColor;
	glm::vec4 DynamicParameter;
};



struct Visual : public MoveableObject
{
	struct ObjectDesc
	{
		RenderableObject* Renderable;
		uint8_t field_8;
		uint8_t Flags;
	};


	struct Attachment
	{
		Visual* Visual;
		int BoneIndex1;
		int BoneIndex2;
		FixedString Bone1;
		FixedString Bone2;
		uint32_t Flags;
		uint8_t BoneType1;
		uint8_t BoneType2;
		FixedString field_20;
		glm::vec3 field_24;
	};


	struct [[bg3::hidden]] SkeletonSlot
	{
		struct Remap
		{
			int16_t field_0;
			int16_t field_2;
		};

		FixedString RemapperSlot;
		int field_4;
		void* Skeleton;
		Remap* Remaps;
		uint8_t RemapsCapacity;
		uint8_t RemapsSize;
	};

	[[bg3::hidden]] void* field_80;
	[[bg3::hidden]] SkeletonSlot** SkeletonSlots;
	[[bg3::hidden]] uint8_t SkeletonSlotsCapacity;
	[[bg3::hidden]] uint8_t SkeletonSlotsSize;
	Array<ObjectDesc> ObjectDescs;
	Array<float> LODDistances;
	Array<Attachment> Attachments;
	Visual* Parent;
	resource::VisualResource* VisualResource;
	[[bg3::hidden]] void* GameObject;
	[[bg3::hidden]] void* PhysicsScene;
	[[bg3::hidden]] void* ClothScene;
	uint32_t VisualFlags;
	uint16_t CullFlags;
	uint8_t PhysicsFlags;
	uint8_t LightChannel;
	bool HasValidPose;
	int16_t WrinkleBaseBoneIndex;
	int16_t Wrinkle01BoneIndex;
	int16_t Wrinkle02BoneIndex;
	int16_t Wrinkle03BoneIndex;
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
	[[bg3::hidden]] void* ModelData;
	Array<AppliedMaterial*> AppliedMaterials;
	AppliedMaterial* ActiveMaterial;
	Array<AppliedMaterial*> AppliedOverlayMaterials;
	uint8_t LOD;
	uint32_t _Pad;
	glm::vec4 MeshRandomData;
};


END_SE()
