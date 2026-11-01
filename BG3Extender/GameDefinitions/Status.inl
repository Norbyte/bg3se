#include <GameDefinitions/Status.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Item.h>

BEGIN_NS(lua)

void MakePolymorphicRef(lua_State* L, esv::Status* v, LifetimeHandle lifetime)
{
#define V(type) case esv::Status##type::Type: \
            MakeDirectObjectRef(L, static_cast<esv::Status##type*>(v), lifetime); break;

    switch (v->GetStatusId()) {
        V(Dying)
        V(Heal)
        V(KnockedDown)
        V(TeleportFalling)
        V(Boost)
        V(Reaction)
        V(StoryFrozen)
        V(Sneaking)
        V(Unlock)
        V(Fear)
        V(Smelly)
        V(Invisible)
        V(Rotate)
        V(Material)
        V(Climbing)
        V(Incapacitated)
        V(InSurface)
        V(Polymorphed)
        V(Effect)
        V(Deactivated)
        V(Downed)

    default:
        MakeDirectObjectRef(L, v, lifetime);
        break;
    }

#undef V
}

void MakePolymorphicRef(lua_State* L, ecl::Status* v, LifetimeHandle lifetime)
{
#define V(type) case ecl::Status##type::Type: \
            MakeDirectObjectRef(L, static_cast<ecl::Status##type*>(v), lifetime); break;

    switch (v->GetStatusId()) {
        V(Dying)
        V(Heal)
        V(KnockedDown)
        V(TeleportFalling)
        V(Boost)
        V(Reaction)
        V(StoryFrozen)
        V(Sneaking)
        V(Unlock)
        V(Fear)
        V(Smelly)
        V(Invisible)
        V(Rotate)
        V(Material)
        V(Climbing)
        V(Incapacitated)
        V(InSurface)
        V(Polymorphed)
        V(Effect)
        V(Deactivated)
        V(Downed)

    default:
        MakeDirectObjectRef(L, v, lifetime);
        break;
    }

#undef V
}

void MakePolymorphicRef(lua_State* L, ecl::StatusVFX* v, LifetimeHandle lifetime)
{
    // add rcx, 18h
    // Insn for chain-calling EffectHandler::UpdateSafe
    const uint8_t vfxEffect[4] = { 0x48, 0x83, 0xc1, 0x18 };
    
    // cmp byte ptr [rcx+20h], 0
    // Insn checking this->Created
    const uint8_t vfxBoostMaterial[4] = { 0x80, 0x79, 0x20, 0x00 };
    
    // add rcx, 28h
    // Insn for chain-calling EffectHandler::UpdateSafe
    const uint8_t vfxManagedStatus[4] = { 0x48, 0x83, 0xc1, 0x28 };

    if (memcmp((uint8_t*)v->VMT->DestroyVisuals + 6, vfxBoostMaterial, 4) == 0) {
        MakeDirectObjectRef(L, static_cast<ecl::StatusVFXBoostMaterial*>(v), lifetime);
        return;
    }

    bool hasApply = *(uint8_t*)v->VMT->Apply != 0xC2; // instruction is not "ret"

    if (memcmp((uint8_t*)v->VMT->UpdateVisuals + 0, vfxEffect, 4) == 0) {
        if (hasApply) {
            MakeDirectObjectRef(L, static_cast<ecl::StatusVFXApplyEffect*>(v), lifetime);
        } else {
            MakeDirectObjectRef(L, static_cast<ecl::StatusVFXEffect*>(v), lifetime);
        }
        return;
    }

    if (hasApply) {
        MakeDirectObjectRef(L, static_cast<ecl::StatusVisual*>(v), lifetime);
        return;
    }

    bool hasUpdate = *(uint8_t*)v->VMT->UpdateVisuals != 0xC2; // instruction is not "ret"

    if (hasUpdate) {
        if (memcmp((uint8_t*)v->VMT->UpdateVisuals + 0, vfxManagedStatus, 4) == 0) {
            MakeDirectObjectRef(L, static_cast<ecl::ManagedStatusVFX*>(v), lifetime);
            return;
        } else {
            MakeDirectObjectRef(L, static_cast<ecl::StatusVFXBeam*>(v), lifetime);
            return;
        }
    }

    // Aura, Material statuses unused
    MakeDirectObjectRef(L, v, lifetime);
}

END_NS()

BEGIN_NS(esv)

Status* StatusMachine::GetStatus(FixedString const& statusId) const
{
    for (auto status : Statuses) {
        if (status->StatusId == statusId) {
            return status;
        }
    }

    return nullptr;
}

RefReturn<esv::Character> Character::LuaGetSelf() const
{
    WARN_ONCE("entity.ServerCharacter.Character is deprecated; use entity.ServerCharacter instead");
    return const_cast<esv::Character*>(this);
}

RefReturn<esv::Item> Item::LuaGetSelf() const
{
    WARN_ONCE("entity.ServerItem.Item is deprecated; use entity.ServerItem instead");
    return const_cast<esv::Item*>(this);
}

esv::Status * Character::GetStatus(FixedString statusId)
{
    if (StatusManager == nullptr) {
        return nullptr;
    }

    return StatusManager->GetStatus(statusId);
}

esv::Status* Character::GetStatusByType(StatusType type)
{
    if (StatusManager == nullptr) {
        return nullptr;
    }

    for (auto status : StatusManager->Statuses) {
        if (status->GetStatusId() == type) {
            return status;
        }
    }

    return nullptr;
}

END_NS()
