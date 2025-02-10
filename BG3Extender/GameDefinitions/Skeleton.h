#pragma once

BEGIN_SE()

struct SkeletonBone
{
    FixedString Name;
    int16_t Index;
};

struct SkeletonSocket
{
    Transform Transform;
    FixedString Name;
    int16_t BoneConstraintIndex;
    int16_t BoneIndex;
    int16_t ConstraintsPos;
    int16_t ConstraintsRot;
    int16_t UpdatedSocketConstraint;
    glm::vec3 ConstraintTranslationOffset;
    glm::mat3 ConstraintRotationOffset;
};

struct SkeletonContent;

struct Skeleton : public ProtectedGameObject<Skeleton>
{
    [[bg3::hidden]] void* VMT;
    FixedString RagdollResourceID;
    FixedString ID;
    FixedString SpringResourceID;
    FixedString DynamicPhysicsResourceID;
    FixedString SoftbodyResourceID;
    FixedString ClothColliderResourceID;
    FixedString MirrorTableResourceID;
    FixedString IKRigResourceID;
    FixedString field_28;
    phx::PhysicsSkinnedObject* Physics;
    bool IsVisible;
    SkeletonBone** Bones;
    HashMap<FixedString, Array<FixedString>> BoneGroups;
    Array<SkeletonSocket> Sockets;
    uint16_t BoneCount;
    std::array<uint16_t, 6> SoundBoneIndices;
    std::array<uint16_t, 10> SoundActivationRanges;
    SkeletonContent* SkeletonContent;
};

struct SkeletonContent : public ProtectedGameObject<SkeletonContent>
{
    [[bg3::hidden]] void* VMT;
    FixedString Name;
};

END_SE()
