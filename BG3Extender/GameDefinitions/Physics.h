#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_NS(phx)

struct PhysicsSceneReadLock;
struct PhysicsSkinnedObject;

struct [[bg3::hidden]] PhysicsSceneScopedReadLock : public ProtectedGameObject<PhysicsSceneScopedReadLock>
{
    PhysicsSceneBase* Scene;
};

using PhysicsHitFunction = void*;

struct PhysicsTemplate : public ProtectedGameObject<PhysicsTemplate>
{
    [[bg3::hidden]] void* VMT;
    FixedString ID;
};

struct PhysicsBaseShape : public ProtectedGameObject<PhysicsBaseShape>
{
    [[bg3::hidden]] void* VMT;
    FixedString Name;
    glm::quat Rotation;
    glm::vec3 Translate;
    glm::vec3 Scale;
};

struct PhysicsShape : public PhysicsBaseShape
{
    PhysicsObject* PhysicsObject;
    [[bg3::hidden]] void* PhysxObject;
};

struct PhysicsSoftShape : public PhysicsBaseShape
{
};


struct PhysicsObject : public ProtectedGameObject<PhysicsObject>
{
    [[bg3::hidden]] void* VMT;
    Array<PhysicsTemplate*> Templates;
    [[bg3::hidden]] Scene* Scene;
    // Editor only
    // [[bg3::hidden]] void* GameObject;
    EntityHandle Entity;
    Array<PhysicsShape*> Shapes;
    glm::vec3 Translate;
    glm::quat Rotate;
    glm::vec3 Scale;
    PhysicsGroupFlags PhysicsGroup;
    PhysicsGroupFlags CollidesWith;
    int PhysicsObjectIndex;
    PhysicsExtraFlags PhysicsExtraFlags;
    // Editor only
    // FixedString ResourceID;
};

struct UnskinnedBoneInfo
{
    int16_t ParentBoneIndex;
    int16_t ChildBoneIndex;
    float field_4;
    float field_8;
    glm::vec3 field_C;
    int field_18;
    glm::vec4 field_1C;
};

struct SkinnedActorInfo
{
    PhysicsShape* Shape;
    int16_t BoneIndex;
    float Blend;
    __int64 field_10;
};

struct PhysicsKinematicTargetInfo
{
    PhysicsShape* Shape;
    glm::vec3 Translate;
    glm::quat Rotate;
    int field_24;
};

struct PhysicsSkinnedCollisionObject : public PhysicsObject
{
    bool QueuedForDeletion;
    bool field_81;
    Array<SkinnedActorInfo> SkinnedActors;
    Visual* TargetVisual;
    [[bg3::hidden]] HashMap<PhysicsShape*, PhysicsKinematicTargetInfo> KinematicTargets;
    bool HasCustomScene;
    Array<FixedString> SoftbodyCollisionResources;
};

struct PhysicsSkinnedObject : public PhysicsSkinnedCollisionObject
{
    Array<FixedString> IgnoreColliders;
    Array<UnskinnedBoneInfo> UnskinnableBones;
    Skeleton* Skeleton;
    Visual* BaseCollisionVisual;
    bool ImmediateSkinningForward;
    bool HasValidWeight;
    glm::vec3 VisualTranslate;
    glm::quat VisualRotate;
    double LastVisibleTime;
    float SettleTimer;
    glm::vec3 InitialImpact;
    int field_160;
    float field_164;
    bool NeedsTranslateUpdate;
    Array<FixedString> field_170;
    [[bg3::readonly]] __int64 field_180;
    bool NeedsMeshBindingUpdate;
    bool AutoForward;
    Array<MeshBinding*> MeshBindings;
    [[bg3::hidden]] CRITICAL_SECTION CriticalSection;
};


struct PhysicsHit
{
    glm::vec3 Normal;
    glm::vec3 Position;
    float Distance;
    PhysicsGroupFlags PhysicsGroup;
    PhysicsExtraFlags PhysicsExtraFlags;
    PhysicsShape* Shape{ nullptr };
};

struct PhysicsHitAll
{
    Array<glm::vec3> Normals;
    Array<glm::vec3> Positions;
    Array<float> Distances;
    Array<PhysicsGroupFlags> PhysicsGroup;
    Array<PhysicsExtraFlags> PhysicsExtraFlags;
    Array<PhysicsShape*> Shapes;
};

struct [[bg3::hidden]] PhysicsSceneBase : ProtectedGameObject<PhysicsSceneBase>
{
    virtual ~PhysicsSceneBase() = 0;
    virtual PhysicsSceneReadLock* InstantiateReadLock() = 0;
    virtual void Unload() = 0;
    // Editor only
    // virtual void RenderDebug(void* RendererCommandBuffer) = 0;
    // virtual void RenderDebugShapes(void* RendererCommandBuffer, std::span<PhysicsShape const*> const&) = 0;
    // virtual bool IsRenderingDebug() const = 0;
    virtual void AddPhysicsObjects(std::span<PhysicsObject*>) = 0;
    virtual void RemovePhysicsObjects(std::span<PhysicsObject*>, bool) = 0;
    virtual void AddPhysicsShape(PhysicsShape*) = 0;
    virtual void RemovePhysicsShape(PhysicsShape*) = 0;
    virtual bool RaycastClosest(glm::vec3 const& source, glm::vec3 const& destination, PhysicsHit& hit, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context, int physicsObjectIndex, int excludePhysicsObjectIndex, Function<bool(PhysicsShape const*)>*) const = 0;
    virtual bool RaycastAll(glm::vec3 const& source, glm::vec3 const& destination, PhysicsHitAll& hits, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context, int physicsObjectIndex, int excludePhysicsObjectIndex, std::optional<PhysicsSceneScopedReadLock*>) const = 0;
    virtual bool RaycastAny(glm::vec3 const& source, glm::vec3 const& destination, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context, int physicsObjectIndex, int excludePhysicsObjectIndex, std::optional<PhysicsSceneScopedReadLock*>) const = 0;
    virtual bool SweepSphereClosest(float radius, glm::vec3 const& source, glm::vec3 const& destination, PhysicsHit& hit, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context, int physicsObjectIndex, int excludePhysicsObjectIndex) const = 0;
    virtual bool SweepCapsuleClosest(float radius, float halfHeight, glm::vec3 const& source, glm::vec3 const& destination, PhysicsHit& hit, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context, int physicsObjectIndex, int excludePhysicsObjectIndex) const = 0;
    virtual bool SweepBoxClosest(glm::vec3 const& extents, glm::vec3 const& source, glm::vec3 const& destination, PhysicsHit& hit, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context, int physicsObjectIndex, int excludePhysicsObjectIndex) const = 0;
    virtual bool SweepCylinderClosest(glm::vec3 const&, glm::vec3 const& source, glm::vec3 const& destination, PhysicsHit& hit, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context, int physicsObjectIndex, int excludePhysicsObjectIndex) const = 0;
    virtual bool SweepSphereAll(float radius, glm::vec3 const& source, glm::vec3 const& destination, PhysicsHitAll& hits, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context, int physicsObjectIndex, int excludePhysicsObjectIndex) const = 0;
    virtual bool SweepCapsuleAll(float radius, float halfHeight, glm::vec3 const& source, glm::vec3 const& destination, PhysicsHitAll& hits, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context, int physicsObjectIndex, int excludePhysicsObjectIndex) const = 0;
    virtual bool SweepBoxAll(glm::vec3 const& extents, glm::vec3 const& source, glm::vec3 const& destination, PhysicsHitAll& hits, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context, int physicsObjectIndex, int excludePhysicsObjectIndex) const = 0;
    virtual bool SweepCylinderAll(glm::vec3 const&, glm::vec3 const&, glm::vec3 const&, PhysicsHitAll& hits, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup, int context, int physicsObjectIndex, int excludePhysicsObjectIndex) const = 0;
    virtual bool SweepShapeAll(PhysicsShape const* shape, Transform const& transform, PhysicsHitAll& hits, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup) const = 0;
    virtual bool TestBoxFunc(glm::vec3 const& position, glm::vec3 const& extents, PhysicsHitFunction const&) const = 0;
    virtual bool TestBox(glm::vec3 const& position, glm::vec3 const& extents, PhysicsHitAll&, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup) const = 0;
    virtual bool TestShapeFunc(Transform const& transform, PhysicsShape const* shape, PhysicsHitFunction const&) const = 0;
    virtual bool TestShape(Transform const& transform, PhysicsShape const* shape, PhysicsHitAll&, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup) const = 0;
    virtual bool TestSphereFunc(glm::vec3 const& position, float radius, PhysicsHitFunction const&) const = 0;
    virtual bool TestSphere(glm::vec3 const& position, float radius, PhysicsHitAll&, PhysicsType physicsType, PhysicsGroupFlags includePhysicsGroup, PhysicsGroupFlags excludePhysicsGroup) const = 0;
    virtual void ProfileStepOut() = 0;
    virtual void ProfileStepInto(int) = 0;
    virtual void ProfileDump(char*, int) = 0;
    virtual void CalculateBound(Bound&, uint32_t physicsGroup, Array<PhysicsShape*>*) = 0;
    virtual void CalculateAaBbBound(glm::vec3& min, glm::vec3& max, uint32_t physicsGroup, Array<PhysicsShape*>*) = 0;
    virtual PhysicsSceneReadLock * InstantiateReadLock2() = 0;
    virtual bool VMT_110() = 0;
    virtual uint32_t GetSceneType() const = 0;
    virtual void AddPhysicsFileToScene(Path const&, FixedString const&, Transform const&) = 0;
    virtual void RemovePhysicsFileFromScene(FixedString const&) = 0;
    virtual void RefreshSpringObjectsForResource(FixedString const&) = 0;
    virtual void RemoveRagdollsWithTemplate(FixedString const&) = 0;

    bool IsSimulating;
    CRITICAL_SECTION CriticalSection;
    HashSet<PhysicsObject*> Objects;
    HashSet<PhysicsShape*> Shapes;
};

struct [[bg3::hidden]] PhysicsScene : public PhysicsSceneBase
{
    UnknownSignal SIG_field_98;
    UnknownSignal SIG_field_B0;
    UnknownSignal SIG_field_C8;
    UnknownSignal SIG_field_E0;
    UnknownSignal SIG_field_F8;
};

struct [[bg3::hidden]] PhysXScene : public PhysicsScene
{
    virtual PhysicsRagdoll* AddRagdoll(FixedString const&, float, ecs::EntityRef const&) = 0;
    virtual void SetRagdollsFrozen(bool) = 0;
    virtual void SetSpringsFrozen(bool) = 0;
    virtual bool InitializeScene(glm::vec3 const&, glm::vec3 const&, unsigned int, unsigned int, unsigned int, unsigned int) = 0;
    virtual bool IsActive() const = 0;
    virtual bool LinkSpringObject(EntityHandle const&, Visual*, std::span<FixedString const> const&, bool, unsigned int, float, bool) = 0;
    virtual bool LinkSpringCollisionObject(Visual*, std::span<FixedString const> const&, float) = 0;
    virtual void UnlinkSpringObject(Visual const*, bool, bool) = 0;
    virtual void UnlinkSpringCollisionObject(Visual*) = 0;
    virtual bool LinkDynamicPhysics(ecs::EntityRef const&, Visual*, unsigned int, float, glm::vec3 const&, glm::vec3 const&, float, float) = 0;
    virtual void UnlinkDynamicPhysics(Visual*) = 0;
    virtual void UpdatePhysicsSkinning() = 0;
    virtual void SetKinematicTarget(PhysicsShape*, glm::vec3 const&, glm::quat const&) = 0;
    virtual bool VMT_1A8() = 0;
    virtual bool SerializeStaticScene(Path, int, FixedString const&, bool (*)(PhysicsObject const*, FixedString const&)) = 0;

    void* VMT2;
    void* field_118;
    void* RagdollSubsystem;
    void* field_128;
    void* field_130;
    void* PxScene;
    void* PhysicsSkinnedSimulateBatch;
    Pool PhysicsSkinnedSimulateJobPool;
    // FIXME - 3 arrays with uint16_t size/capacity
    [[bg3::hidden]] Array<FixedString> PendingAddSkinnedObjects;
    [[bg3::hidden]] Array<void*> PendingReleaseSkinnedObjects;
    [[bg3::hidden]] Array<void*> UniqueSkinnedObjects;
    Array<PhysicsSkinnedObject*> SkinnedObjects;
    Array<void*> field_198;
    Array<void*> field_1A8;
    Pool PhysicsSkinningJobPool;
    HashMap<PhysicsShape*, PhysicsKinematicTargetInfo> PhysicsKinematicTargetInfos;
    void* ContactCallback;
    void* PhxSimulationFilter;
    bool SceneActive;
    int DisableSimulationMask;
    int ClothCollisionMask;
    glm::vec3 SceneBounds[2];
    void* PhysicsSkinningBatch;
    void* SceneQueryBatch;
    void* SceneQueryTaskBatch;
    void* SceneQueryUpdater;
    __int64 field_260;
    __int64 field_268;
    uint8_t field_270;
};


END_NS()
