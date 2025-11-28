#pragma once

#include <GameDefinitions/AllSparkShared.h>
#include <GameDefinitions/Resources.h>

BEGIN_SE()

struct MoveableObject;

struct AABound
{
    glm::vec3 Min;
    glm::vec3 Max;
};

struct [[bg3::hidden]] Pose : ProtectedGameObject<Pose>
{
    glm::mat3x4* Pose1;
    glm::mat3x4* Pose2;
    uint16_t BoneCount;
};

struct MeshBinding : ProtectedGameObject<MeshBinding>
{
    [[bg3::hidden]] void* VMT;
    [[bg3::hidden]] void* field_8;
    [[bg3::hidden]] Pose Pose;
    MoveableObject* Object;
    glm::mat4 Transform;
    AABound Bound1;
    AABound Bound2;
};

struct LocalTransform
{
    Transform LocalTransform;
    AABound LocalBound;
};


struct MoveableObject : ProtectedGameObject<MoveableObject>
{
    static constexpr uint32_t StaticRTTI = 0x1;

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
    virtual char const* GetDebugName() const = 0;
    virtual StringView GetStrDebugName() = 0;
    virtual void GetDebugInfo(ScratchString &) const = 0;

    Transform WorldTransform;
    LocalTransform* LocalTransform;
    [[bg3::hidden]] Scene* Scene;
    AABound WorldBound;
    AABound BaseBound;
    float MinLODDistance;
    float MaxLODDistance;
    [[bg3::readonly]] int SceneIndex;

    // Lua call helpers
    //# P_FUN(SetWorldTranslate, MoveableObject::LuaSetWorldTranslate)
    //# P_FUN(SetWorldRotate, MoveableObject::LuaSetWorldRotate)
    //# P_FUN(SetWorldScale, MoveableObject::LuaSetWorldScale)
    void LuaSetWorldTranslate(glm::vec3 const& translate);
    void LuaSetWorldRotate(glm::quat const& rotate);
    void LuaSetWorldScale(glm::vec3 const& scale);
};

struct SceneObjectBounds
{
    Array<float> BoundMinX;
    Array<float> BoundMinY;
    Array<float> BoundMinZ;
    Array<float> BoundMaxX;
    Array<float> BoundMaxY;
    Array<float> BoundMaxZ;
};

struct SceneObjects
{
    SceneObjectBounds WorldBound;
    Array<float> BoundRadius;
    Array<float> MinLODDistanceSquared;
    Array<float> MaxLODDistanceSquared;
    SceneObjectBounds RootBound;
    SceneObjectBounds HLODBound;
    Array<bool> field_150;
    Array<MoveableObject*> Objects;
    Array<uint32_t> CullFlags;
    Array<uint32_t> RenderPasses;
};


struct Scene : public ProtectedGameObject<Scene>
{
    EntityHandle field_0;
    [[bg3::readonly]] bool Active;
    // Editor only
    // [[bg3::hidden]] void* SceneViewMode;
    [[bg3::hidden]] CRITICAL_SECTION DirtyListCS;
    [[bg3::hidden]] Array<MoveableObject*> DirtyList;
    [[bg3::hidden]] LevelBase* Level;
    [[bg3::hidden]] bool CullRegions;
    [[bg3::hidden]] SRWLOCK SRWLock;
    [[bg3::hidden]] std::array<SceneObjects, 11> ObjectsByType;
    [[bg3::hidden]] Array<MoveableObject*> DistantLightProbes;
    [[bg3::hidden]] HashSet<MoveableObject*> field_11A8;
};

struct ShaderParamBinding
{
    int8_t DxVsIndex;
    int8_t DxPsIndex;
    int8_t VkDescriptorSet;
    int8_t VkBindingIndex;
};

struct MaterialShaderDecalParameters
{
    int16_t DecalWorld;
    int16_t DecalDimensions;
    int16_t DecalTilingAndOffset;
    int16_t DecalDeferredProperties;
    int16_t InvWorldMatrix;
    int16_t NormalMatrix;
};

struct MaterialShaderDesc
{
    MaterialShaderDescFlags EngineParamFlags;
    uint16_t EngineCBSize;
    uint16_t MaterialCBSize;
    ShaderParamBinding EngineCBBinding;
    ShaderParamBinding MaterialCBBinding;
    int16_t WorldMatrixCurr;
    int16_t WorldMatrixPrev;
    int16_t WorldCurr;
    int16_t WorldPrev;
    int16_t TransformedVerticesOffsetCurr;
    int16_t TransformedVerticesOffsetPrev;
    int16_t MeshRandom;
    int16_t WorldBoundsMinSubVisual;
    int16_t WorldBoundsMinVisual;
    int16_t WorldBoundsMinParent;
    int16_t WorldBoundsMaxSubVisual;
    int16_t WorldBoundsMaxVisual;
    int16_t WorldBoundsMaxParent;
    int16_t LocalBoundsMin;
    int16_t LocalBoundsMax;
    int16_t ReceiveDecal;
    int16_t FadeOpacity;
    int16_t LightChannel;
    int16_t DiffusionProfileIndex;
    int16_t VirtualTexture_TilesetDataIndex;
    int16_t VirtualTexture_CB_Texture;
    MaterialShaderDecalParameters* DecalParameters;
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

struct MaterialParameters
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

struct Material : public ProtectedGameObject<Material>
{
    struct MaterialCB
    {
        [[bg3::hidden]] void* MaterialCB;
        uint64_t MaterialCBSize;
    };

    [[bg3::hidden]] void* VMT;
    FixedString Name;
    [[bg3::hidden]] std::array<MaterialShaderDesc, 15> ShaderDescriptions;
    [[bg3::hidden]] SRWLOCK MaterialCBLock;
    [[bg3::hidden]] std::array<MaterialCB, 15> MaterialCBs;
    std::array<FixedString, 15> Shaders;
    uint32_t ShadingModel;
    uint32_t ForwardLightingMode;
    uint8_t BlendStateID;
    uint8_t RasterizerStateID;
    uint8_t DepthStateID;
    uint32_t StencilRef;
    uint32_t UVCount;
    MaterialFlags Flags;
    MaterialUsedWithFlags UsedWithFlags;
    uint8_t MaterialPassHint;
    MaterialType MaterialType;
    RenderChannel RenderChannel;
    uint8_t MaterialUsage;
    FixedString DiffusionProfileUUID;
    MaterialParameters Parameters;
    Material* Parent;
    // Editor only
    // void* MaterialEditorHelperInternal;
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

struct SBBinding
{
    uint8_t field_0;
    uint8_t DxVsIndex;
    uint8_t field_2;
    uint8_t field_3;
    uint8_t field_4;
    uint8_t DxPsIndex;
    uint8_t field_6;
    uint8_t VkBindingIndex;
    uint8_t VkDescriptorSet;
};

struct MaterialRenderingData
{
    EntityHandle Handle;
    EntityHandle VertexFormat;
    uint64_t InstancingHash;
    uint8_t ShaderIndex;
    uint8_t field_19;
    uint16_t EngineCBSize;
    uint16_t MaterialCBSize;
    SBBinding EngineBinding;
    SBBinding MaterialBinding;
    [[bg3::hidden]] void* MaterialCB;
    [[bg3::hidden]] uint64_t MaterialCBBufferSize;

    template <class T>
    std::optional<T> GetUniformParam(Material& instance, UniformBindingData const& binding);
    template <class T>
    void SetUniformParam(Material& instance, UniformBindingData const& binding, T value);
    bool CheckConstantBuffer(Material& instance);
};


struct PrimaryMaterialRenderingData : public MaterialRenderingData
{
    __int64 field_40;
    __int64 field_48;
    __int64 field_50;
    __int64 field_58;
};

struct [[bg3::hidden]] QueuedRawTextures
{
    FixedString Param;
    void* Texture; // rf::Texture*
};

struct AppliedMaterialParameters : public MaterialParameters
{
    [[bg3::hidden]] Array<QueuedRawTextures> QueuedTextures;
};


struct AppliedMaterial
{
    using LoadTextureProc = TextureDescriptor * (void* self, FixedString const& textureGuid);

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
    [[bg3::hidden]] std::array<void*, 8> PipelineStates;
    [[bg3::hidden]] Array<Texture2DParam> Texture2DParams;
    Array<VirtualTextureParam> VirtualTextureParams;
    RenderableObject* RenderableObject;
    [[bg3::legacy(MaterialInstance)]] Material* Material;
    [[bg3::hidden]] void* RenderStage;
    [[bg3::hidden]] void* ShaderCallback;
    uint64_t Hash;
    [[bg3::hidden]] void* DiffusionProfile;
    AppliedMaterialDirtyFlags DirtyFlags;
    uint8_t BlendStateID;
    uint8_t RasterizerStateID;
    uint8_t DebugMaterial;
    float OverlayOffset;
    FixedString MaterialName;
    AppliedMaterialFlags Flags;
    bool Initialized;
    uint8_t AlphaChannel;
    glm::vec4 MeshVertexColor;
    glm::vec4 DynamicParameter;
    AppliedMaterialParameters* Parameters;

    //# P_FUN(GetScalar, AppliedMaterial::GetScalar)
    //# P_FUN(GetVector2, AppliedMaterial::GetVector2)
    //# P_FUN(GetVector3, AppliedMaterial::GetVector3)
    //# P_FUN(GetVector4, AppliedMaterial::GetVector4)
    std::optional<float> GetScalar(FixedString const& param);
    std::optional<glm::vec2> GetVector2(FixedString const& param);
    std::optional<glm::vec3> GetVector3(FixedString const& param);
    std::optional<glm::vec4> GetVector4(FixedString const& param);

    //# P_FUN(SetScalar, AppliedMaterial::SetScalar)
    //# P_FUN(SetVector2, AppliedMaterial::SetVector2)
    //# P_FUN(SetVector3, AppliedMaterial::SetVector3)
    //# P_FUN(SetVector4, AppliedMaterial::SetVector4)
    bool SetScalar(FixedString const& param, float value);
    bool SetVector2(FixedString const& param, glm::vec2 value);
    bool SetVector3(FixedString const& param, glm::vec3 value);
    bool SetVector4(FixedString const& param, glm::vec4 value);

    template <class T>
    std::optional<T> GetUniformParam(UniformBindingData const& binding);
    template <class T>
    void SetUniformParam(UniformBindingData const& binding, T value);
};



struct Visual : public MoveableObject
{
    static constexpr uint32_t StaticRTTI = 0x1001;

    using UpdateBlendshapeWeightsFromSkeletonProc = void(Visual* self);

    virtual bool AddObject(RenderableObject*, uint8_t flags) = 0;
    virtual void SetLODDistances(RenderableObject*) const = 0;
    virtual void Pick(void const* ray, void* result) = 0;
    virtual void UpdateCullFlags() = 0;

    struct ObjectDesc
    {
        RenderableObject* Renderable;
        [[bg3::legacy(field_8)]] uint8_t LOD;
        VisualObjectType Flags;
    };


    struct Attachment
    {
        Visual* Visual;
        [[bg3::legacy(BoneIndex1)]] int SourceIndex;
        [[bg3::legacy(BoneIndex2)]] int AttachIndex;
        [[bg3::legacy(Bone1)]] FixedString Attach;
        [[bg3::legacy(Bone2)]] FixedString Source;
        VisualAttachmentFlags Flags;
        [[bg3::legacy(BoneType1)]] VisualAttachType SourceType;
        [[bg3::legacy(BoneType2)]] VisualAttachType TargetType;
        FixedString field_20;
        [[bg3::legacy(field_24)]] glm::vec3 Offset;
    };


    struct SkeletonSlot
    {
        struct Remap
        {
            int16_t MapFrom;
            int16_t MapTo;
        };

        FixedString RemapperSlot;
        [[bg3::legacy(field_4)]] FixedString RemappedSlot;
        Skeleton* Skeleton;
        MiniCompactSet<Remap> Remaps;
    };

    [[bg3::hidden]] void* RenderPrecisionHack;
    MiniCompactSet<SkeletonSlot*> SkeletonSlots;
    Array<ObjectDesc> ObjectDescs;
    Array<float> LODDistances;
    Array<Attachment> Attachments;
    Visual* Parent;
    resource::VisualResource* VisualResource;
    [[bg3::hidden]] void* GameObject;
    [[bg3::hidden]] void* PhysicsScene;
    [[bg3::hidden]] void* ClothScene;
    VisualFlags VisualFlags;
    VisualCullFlags CullFlags;
    VisualPhysicsFlags PhysicsFlags;
    LightChannelValue LightChannel;
    bool HasValidPose;
    bool HasAnimatedObjects;
    int16_t WrinkleBaseBoneIndex;
    int16_t Wrinkle01BoneIndex;
    int16_t Wrinkle02BoneIndex;
    int16_t Wrinkle03BoneIndex;
    int16_t BlendShapeBase;
    ecs::EntityRef VisualEntity;
};


struct Effect : public Visual
{
    static constexpr uint32_t StaticRTTI = 0x3001;

    [[bg3::hidden]] Array<void*> PostProcess;
    float CullingDistance;
};

struct BasicModel : ProtectedGameObject<BasicModel>
{
    static constexpr uint32_t StaticRTTI = 0x1000000;

    virtual uint32_t GetRTTI() = 0;
    virtual void Destroy() = 0;

    FixedString Name;
    FixedString Id;
    FixedString LinkId;
    AABound Bound;
};

struct [[bg3::hidden]] PickingData
{
    float* Vertices;
    uint32_t* Indices;
    int IndexCount;
    int VertexCount;
};

struct FormatDesc
{
    uint8_t Stream;
    uint8_t Usage;
    uint8_t UsageIndex;
    uint8_t RefType;
    uint8_t Format;
    uint8_t Size;
};

struct Model : BasicModel
{
    static constexpr uint32_t StaticRTTI = 0x3000000;

    virtual bool IsBlendShape() = 0;
    virtual bool IsSkinned() = 0;
    virtual bool IsCloth() = 0;

    int IndexCount;
    int IndexSize;
    int VertexCount;
    int VertexSize;
    Array<FormatDesc> Attributes;
    [[bg3::hidden]] void* VertexBuffer;
    [[bg3::hidden]] void* IndexBuffer;
    uint8_t Topology;
    [[bg3::hidden]] PickingData Picking;
    bool HasUVs;
    uint32_t UVCount;
};

struct ModelProxy : BasicModel
{
    static constexpr uint32_t StaticRTTI = 0x5000000;

    Array<glm::vec3> Vertices;
    Array<int> Indices;
};

struct TransformingVertexModel;

struct GrBlendShapeModelData : ProtectedGameObject<GrBlendShapeModelData>
{
    virtual void Destroy() = 0;
    virtual uint32_t GetVertexCount(int) = 0;
    virtual std::span<void*>* GetVertexCount(std::span<void*>&, int) = 0;
    virtual std::span<void*>* GetVertices(std::span<void*>&, int) = 0;
    virtual FixedString* GetMorphMeshName(int) = 0;
    virtual uint32_t GetMorphTargetCount(int) = 0;
    virtual float GetMaxVertDisplacement(int) = 0;

    [[bg3::hidden]] void* Mesh;
    Array<FixedString> MorphTargetDrivers;
};

struct BlendShapeModelData : ProtectedGameObject<BlendShapeModelData>
{
    GrBlendShapeModelData* Data;
    TransformingVertexModel* TransformingModel;
    [[bg3::hidden]] void* VertexBuffer;
    uint32_t NumVertices;
    [[bg3::hidden]] void* Buffer1;
    [[bg3::hidden]] void* Buffer2;
    [[bg3::hidden]] void* MorphVBView[2];
    [[bg3::hidden]] void* MorphIBView[2];
};

struct TransformingVertexModel : Model
{
    [[bg3::hidden]] void* SkinnedModelData;
    BlendShapeModelData* BlendShapeModelData;
    [[bg3::hidden]] void* ClothModelData;
    bool HasClothInstance;
};

struct [[bg3::hidden]] BufferBindingInfo
{
    __int64 field_0;
    __int64 field_8;
};

struct BlendShapeWeightOverride
{
    int32_t BlendShapeIndex;
    float Weight;
};

struct BlendShapeWeights
{
    Array<float> Weights;
#if 0
    // Editor only
    // Array<WeightOverride> WeightOverrides;
#endif
};

struct [[bg3::hidden]] BlendShapeObjectDataDX11 : ProtectedGameObject<BlendShapeObjectDataDX11>
{
    void* VertexBuffer;
    void* Buffer;
    BufferBindingInfo BufferBinding;
    BlendShapeWeights Weights;
};

struct [[bg3::hidden]] BlendShapeObjectDataVK : ProtectedGameObject<BlendShapeObjectDataVK>
{
    void* VertexBuffer;
    void* Buffer;
    BufferBindingInfo BufferBinding[3];
    BlendShapeWeights Weights;
};


struct RenderableObject : public MoveableObject
{
    static constexpr uint32_t StaticRTTI = 0x3;

    virtual void Render(void const* RenderObjectData, void const* RenderContext, void const* InstancingDrawData) const = 0;
    virtual void NotifyTextureIDAdded(void const* Texture, bool) = 0;
    virtual void NotifyTextureIDRemoved(void const* Texture, bool) = 0;
    virtual void* SetupMaterialParameters(void const* RenderObjectData, void const* RenderContext) const = 0;
    virtual glm::mat4 GetRenderingTransform() const = 0;
    virtual void SetupTransformedVerticesOffset(void* TempReflectedCB, void const* MaterialShaderDesc, void const* RenderContext) const = 0;
    virtual bool GetAnimatableTransform(glm::mat3x4 const*&, uint64_t&) const = 0;
    virtual void CalculateRenderPass() = 0;
    virtual void BuildVertexFormatDesc() = 0;
    virtual void BuildVelocityVertexFormatDesc() = 0;
    virtual bool Ret0_2() const = 0;
    virtual bool IsBlendShape() const = 0;
    virtual bool IsSkinned() const = 0;
    virtual bool Ret0_4() const = 0;
    virtual bool Ret0_5() const = 0;
    virtual bool CanUseOverlayBoneTransforms() const = 0;

    BasicModel* Model;
    RenderPropertyList PropertyList;
    Visual* Parent;
    // either BlendShapeObjectDataDX11 or BlendShapeObjectDataVK
    [[bg3::hidden]] void* BlendShape;
    glm::vec4 MeshRandomData;
    Array<AppliedMaterial*> AppliedMaterials;
    AppliedMaterial* ActiveMaterial;
    Array<AppliedMaterial*> AppliedOverlayMaterials;
    uint8_t DirtyFlags;
    uint8_t LOD;

    // Lua helpers
    BlendShapeWeights* LuaGetBlendShape() const;
    bool SetBlendShapeWeight(FixedString const& param, std::optional<float> weight);
    bool ClearBlendShapeWeights(FixedString const& param);

    //# P_GETTER(BlendShape, LuaGetBlendShape)
    //# P_FUN(SetBlendShapeWeight, RenderableObject::SetBlendShapeWeight)
    //# P_FUN(ClearBlendShapeWeights, RenderableObject::ClearBlendShapeWeights)
};

struct AnimatableObject : public RenderableObject
{
    static constexpr uint32_t StaticRTTI = 0x7;

    MeshBinding* Mesh;
    bool IsRigid;
    bool HasStaticBounds;
    uint64_t OverrideTransformCount;
    glm::mat3x4* OverrideTransforms_Mat3x4;
    uint64_t SavedOverrideTransformCount;
    glm::mat3x4* SavedOverrideTransforms_Mat3x4;
    std::array<int, 2> TransformedVerticesOffset;
    std::array<int, 2> TransformedVerticesOffset2;
    std::array<int, 2> TransformedFrameCount;
};

struct Shape : public RenderableObject
{
    static constexpr uint32_t StaticRTTI = 0x103;

    Skeleton* SkeletonActor;
    float Lifetime;
    uint8_t field_F4;
    bool UseShapeRender;
    uint32_t Preset;
    bool TriangulationFailed;
};

struct DecalObject : public RenderableObject
{
    static constexpr uint32_t StaticRTTI = 0xB;

    FixedString Decal;
    FixedString Visual;
    glm::vec2 TilingUV;
    glm::vec2 OffsetUV;
    float NormalBlendingFactor;
    float AngleCutoff;
    int32_t Layer;
    float Opacity;
    bool IsGameDecal;
    glm::vec3 Dimensions;
    uint16_t CullFlags;
};

struct PhysicsClothInfo
{
    phx::PhysicsSoftShape* PhysicsShape;
    bool IsSimulated;
    bool IsCopyCloth;
    bool IsHiResProxy;
    [[bg3::hidden]] void* VertexModel; // rf::TransformingVertexModel*
};

struct RenderableObjectPhysicsInfo
{
    RenderableObjectPhysicsInfo* Parent;
    PhysicsClothInfo* ClothInfo;
};

struct AnimatablePhysicsObject : public AnimatableObject
{
    RenderableObjectPhysicsInfo Physics;
};

struct CullableInstance : public MoveableObject
{
    static constexpr uint32_t StaticRTTI = 0x40001;

    struct InstancingObject* Parent;
    uint32_t InstanceIndex;
};

struct InstancingRenderableObject : public RenderableObject
{
    static constexpr uint32_t StaticRTTI = 0x203;
};

struct InstancingObject : public RenderableObject
{
    static constexpr uint32_t StaticRTTI = 0x403;

    Array<CullableInstance*> Instances;
};

struct LightDesc
{
    FixedString LightCookieTexture;
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec3 Rotation;
    float SpotLightInnerAngle;
    int field_2C;
    glm::vec3 DirectionLightDimensions;
    float DirectionLightAttenuationStart;
    float DirectionLightAttenuationEnd;
    float DirectionLightAttenuationSide;
    float Kelvin;
    float Radius;
    float Intensity;
    float ScatteringIntensityScale;
    float Gain;
    float Speed;
    float Amount;
    float MovementSpeed;
    float MovementAmount;
    bool UseTemperature;
    bool IsFlickering;
    bool IsMoving;
    bool Shadow;
    bool VolumetricShadow;
    bool Enabled;
    bool PreExpose;
    bool FlatFalloff;
    LightType LightType;
    uint8_t LightChannelFlag;
    uint8_t DirectionLightAttenuationFunction;
};


struct [[bg3::component]] LightComponent : public MoveableObject
{
    static constexpr uint32_t StaticRTTI = 0x10001;

    DEFINE_PROXY_COMPONENT(Light, "ls::LightComponent")

    EntityHandle field_80;
    glm::vec3 Color;
    float SpotLightInnerAngle;
    float SpotLightOuterAngle;
    glm::vec3 DirectionLightDimensions;
    float DirectionLightAttenuationSide;
    float DirectionLightAttenuationEnd;
    float DirectionLightAttenuationSide2;
    float Kelvin;
    float Radius;
    float Intensity;
    [[bg3::legacy(field_C0)]] float IntensityOffset;
    float ScatteringIntensityScale;
    float Gain;
    float EdgeSharpening;
    uint8_t DirectionLightAttenuationFunction;
    LightFlags Flags;
    uint8_t LightChannelFlag;
    LightType LightType;
    glm::vec3 Blackbody;
    [[bg3::legacy(field_E0)]] glm::vec3 PositionOffset;
    uint8_t OwnerFlags;
    [[bg3::hidden]] void* LightCookieTexture;
    LightTemplate* Template;
    EntityHandle AssociatedScene;
    uint16_t CullFlags;
    FixedString UUID;
    [[bg3::hidden]] void* _PAD;
};

struct LightUpdateFadeRequest
{
    float field_0;
    float field_4;
    float Max;
    float Current;
};

struct LightSystem : public BaseSystem
{
    DEFINE_SYSTEM(Light, "ls::LightSystem")

    HashMap<EntityHandle, LightUpdateFadeRequest> FadeEdgeSharpening;
    HashMap<EntityHandle, LightUpdateFadeRequest> FadeRadius;
    HashMap<EntityHandle, LightUpdateFadeRequest> FadeIntensity;
    HashMap<EntityHandle, LightUpdateFadeRequest> FadeIntensityOffset;
    HashSet<EntityHandle> ExplicitUpdateTransform;
    HashSet<EntityHandle> CheckFlickering;
    HashSet<EntityHandle> CheckMoving;
};


END_SE()

BEGIN_NS(lua)

LUA_POLYMORPHIC(MoveableObject);
LUA_POLYMORPHIC(RenderableObject);
LUA_POLYMORPHIC(AnimatableObject);
LUA_POLYMORPHIC(Visual);

LUA_POLYMORPHIC(BasicModel);
LUA_POLYMORPHIC(Model);

END_NS()
