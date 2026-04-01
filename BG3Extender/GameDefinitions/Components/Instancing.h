#pragma once

BEGIN_NS(instancing)

struct InstancingGroupPaintParams
{
    float Flow;
    bool RandomRotX;
    bool RandomRotY;
    bool RandomRotZ;
    bool AlignNormal;
    float ScaleMin;
    float ScaleMax;
};

struct InstancingGroupDesc : public ProtectedGameObject<InstancingGroupDesc>
{
    [[bg3::hidden]] void* VMT;
    Guid FileID;
    FixedString GroupID;
    FixedString VisualID;
    uint16_t GridSize;
    bool Shadow;
    Path Path;
    InstancingGroupPaintParams PaintParams;
};

struct AddWorldInstances
{
    Array<glm::mat4x3> Instances;
};

struct EraseWorldInstances
{
    glm::vec3 Position;
    float Radius;
};

struct MoveLocalInstances
{
    glm::vec3 Position;
    float Radius;
    glm::vec3 Offset;
};

struct AddLocalInstances
{
    Array<glm::mat4x3> Instances;
};

struct EraseLocalInstances
{
    glm::vec3 Position;
    float Radius;
};

struct ReAlignLocalInstances
{
    glm::vec3 Position;
    float Radius;
    bool field_10{ true };
};

struct ReScaleLocalInstances
{
    glm::vec3 Position;
    float Radius;
    float FromScale;
    float ToScale;
};

struct RotateLocalInstances
{
    glm::vec3 Position;
    float Radius;
    glm::vec3 Rotation;
    bool field_1C{ true };
};

struct UpdateTransform
{};

struct ReAlignAllInstances
{
    bool field_0{ true };
};

struct ReScaleAllInstances
{
    float FromScale;
    float ToScale;
};

struct RotateAllInstances
{
    glm::vec3 Rotation;
    bool field_C{ true };
};

struct RemoveDuplicates
{
    float field_0;
};

struct RemoveFloating
{
    float field_0;
};

struct UpdateIsLocked
{
    bool field_0;
};

struct UpdateStorageIsLocked
{
    glm::ivec3 field_0;
    bool field_C;
};

struct SwapGroupVisual
{
    FixedString Visual;
};

struct UpdatePaintParams
{
    InstancingGroupPaintParams Params;
};

struct SetVisibility
{
    bool Visibility;
};

struct ShowGridLines
{
    bool Show;
};

struct SetShadow
{
    bool Shadow;
};

struct InstancingRequest
{
    using RequestVariant = std::variant<
        AddWorldInstances,
        EraseWorldInstances,
        MoveLocalInstances,
        AddLocalInstances,
        EraseLocalInstances,
        ReAlignLocalInstances,
        ReScaleLocalInstances,
        RotateLocalInstances,
        UpdateTransform,
        ReAlignAllInstances,
        ReScaleAllInstances,
        RotateAllInstances,
        RemoveDuplicates,
        RemoveFloating,
        UpdateIsLocked,
        UpdateStorageIsLocked,
        SwapGroupVisual,
        UpdatePaintParams,
        SetVisibility,
        ShowGridLines,
        SetShadow
    >;

    EntityHandle Group;
    RequestVariant Request;
};

using GroupModifyAction = std::variant<
    SwapGroupVisual,
    UpdatePaintParams,
    SetVisibility,
    ShowGridLines,
    SetShadow
>;

using BatchModifyAction = std::variant<
    AddLocalInstances,
    EraseLocalInstances,
    MoveLocalInstances,
    ReAlignLocalInstances,
    ReScaleLocalInstances,
    RotateLocalInstances,
    UpdateTransform,
    ReAlignAllInstances,
    ReScaleAllInstances,
    RotateAllInstances,
    RemoveDuplicates,
    RemoveFloating,
    UpdateIsLocked,
    UpdateStorageIsLocked,
    SetVisibility,
    ShowGridLines,
    SetShadow
>;

struct InstancingGroupModifyOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(InstancingGroupModifyOneFrame, "ls::InstancingGroupModifyOneFrameComponent")

    Array<GroupModifyAction> Actions;
};

struct InstancingBatchModifyOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(InstancingBatchModifyOneFrame, "ls::InstancingBatchModifyOneFrameComponent")

    Array<BatchModifyAction> Actions;
};

struct InstancingGroupVisualComponent : public BaseComponent
{
    DEFINE_COMPONENT(InstancingGroupVisual, "ls::InstancingGroupVisualComponent")

    FixedString VisualResource;
    [[bg3::hidden]] void* LoadCallback;
    uint32_t RefCount;
    bool field_14;
};

struct InstancingGroupComponent : public BaseComponent
{
    DEFINE_COMPONENT(InstancingGroup, "ls::InstancingGroupComponent")

    InstancingGroupDesc Desc;
    HashMap<glm::ivec3, EntityHandle> Batches;
    bool field_98;
    uint8_t field_99;
    uint8_t field_9A;
    HashSet<glm::ivec3> BatchCenters;
};

struct InstancingBatchComponent : public BaseComponent
{
    DEFINE_COMPONENT(InstancingBatch, "ls::InstancingBatchComponent")

    Array<glm::mat4x3> Instances;
    EntityHandle Group;
    glm::vec3 Position;
    int field_24;
    uint32_t dword28;
    uint8_t byte2C;
};

DEFINE_TAG_COMPONENT(ls, InstancingBatchInitializedComponent, InstancingBatchInitialized)
DEFINE_ONEFRAME_TAG_COMPONENT(ls, InstancingGroupVisualReloadOneFrameComponent, InstancingGroupVisualReloadOneFrame)
DEFINE_ONEFRAME_TAG_COMPONENT(ls, InstancingBatchVisualReloadedOneFrameComponent, InstancingBatchVisualReloadedOneFrame)

END_NS()
