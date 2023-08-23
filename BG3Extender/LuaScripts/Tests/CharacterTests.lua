local GUID_LAEZEL = "58a69333-40bf-8358-1d17-fff240d7fb12"

function TestCharacterEnumeration()
    local chars = Ext.GetAllCharacters()
    local found = false
    for i,entity in ipairs(chars) do
        if entity.ServerCharacter.GUID == GUID_LAEZEL then
            found = true
        end
    end

    -- Laezel must be found in test map
    AssertEquals(found, true)

    -- Check if default arguments match
    local defaultArgChars = Ext.GetAllCharacters("", false, false)
    AssertEquals(#chars, #defaultArgChars)

    -- Check if checking for specific map returns all characters
    local avernusChars = Ext.GetAllCharacters("TUT_Avernus_C", false, false)
    AssertEquals(#chars, #avernusChars)

    -- Should find no characters on nonexistent map
    local nonexistentChars = Ext.GetAllCharacters("NONEXISTENT", false, false)
    AssertEquals(#nonexistentChars, 0)

    -- Should find no characters on inactive map
    local nonexistentChars = Ext.GetAllCharacters("WLD_Main", false, false)
    AssertEquals(#nonexistentChars, 0)
end


function TestCharacterProperties()
    local c = Ext.GetCharacter(GUID_LAEZEL).ServerCharacter

    AssertEquals(c.GUID, GUID_LAEZEL)
    Assert(type(c.NetID) == "number" and c.NetID > 0)
    AssertEquals(c.Flags, 0x21024010)
    AssertEquals(c.CurrentLevel, "TUT_Avernus_C")
    AssertEquals(c.Flags2, 1)
    AssertEquals(c.Flags3, 1)
    AssertEquals(c.WaitForTeleportAck, false)
    AssertEquals(c.Team, 0)
    AssertEquals(c.HasOsirisDialog, 2)
    AssertEquals(c.NeedsUpdate, true)
    AssertEquals(c.ForceSynch, false)
    -- userdata test! AssertEquals(c.InventoryHandle, )
    AssertEquals(c.IsDialogAiControlled, false)
    -- float test! AssertEquals(c.TurnTimer)
    AssertEquals(c.TriggerTrapsTimer, -1.0)
    AssertEquals(c.UserID, -65536)
    AssertEquals(c.ReservedUserID, -65536)
    AssertEquals(c.OwnerCharacterHandle, nil)
    AssertEquals(c.FollowCharacterHandle, nil)
    AssertEquals(c.EnemyCharacterHandle, nil)
    AssertEquals(c.SpiritCharacterHandle, nil)
    -- FIXME - bad? AssertEquals(c.MovementAP)
    AssertEquals(c.MovementSpeedMultiplier, 1.0)
    AssertEquals(c.PartyHandle, nil)
    AssertEquals(c.CreatedTemplateItems, {})
    AssertEquals(c.Treasures, {"Empty"})
    AssertEquals(c.CustomTradeTreasure, "")
    AssertEquals(c.MovingCasterHandle, nil)
    AssertEquals(c.EquipmentColor, "")
    AssertEquals(c.ProjectileTemplate, "")
    AssertEquals(c.TimeElapsed, 0)
    AssertEquals(c.PreferredAiTargetTags, {})
    AssertEquals(c.TagsFromItems, {})
    AssertEquals(c.ReadyCheckBlocked, false)
    AssertEquals(c.CorpseLootable, false)
    AssertEquals(c.PreviousLevel, "WLD_Main_A")
end


function TestCharacterTemplateProperties()
    local t = Ext.GetCharacter(GUID_LAEZEL).ServerCharacter.CurrentTemplate

    local cc = t.CombatComponent
    AssertEquals(cc.Archetype, "melee")
    AssertEquals(cc.Alignment, "6ae41c2e-e72a-c682-a014-18e638ced83f")
    AssertEquals(cc.CanFight, true)
    AssertEquals(cc.CanJoinCombat, true)
    AssertEquals(cc.CombatGroupID, "")
    AssertEquals(cc.CombatName, "")
    AssertEquals(cc.IsBoss, false)
    AssertEquals(cc.StayInAiHints, false)
    AssertEquals(cc.AiHint, "00000000-0000-0000-0000-000000000000")
    AssertEquals(cc.IsInspector, false)
    AssertEquals(cc.StartCombatRange, -1.0)

    -- GameObjectTemplate
    AssertEquals(t.Flags, 1)
    AssertEquals(t.Id, GUID_LAEZEL)
    AssertEquals(t.Name, "")
    AssertEquals(t.TemplateName, "3a0351bd-0b2f-4fbf-84a6-30ad80bb3100")
    AssertEquals(t.ParentTemplateId, "")
    AssertEquals(t.IsGlobal, true)
    AssertEquals(t.IsDeleted, false)
    AssertEquals(t.IsForcedType, false)
    AssertEquals(t.LevelName, "WLD_Main_A")
    AssertEquals(t.GroupID, 3178)
    AssertEquals(t.VisualTemplate, "23418296-f7fa-66e9-b986-466a12707c1a")
    AssertEquals(t.PhysicsTemplate, "950614ec-c898-c384-3bbe-3cb58a07cfde")
    AssertEquals(t.PhysicsOpenTemplate, "")
    AssertEquals(t.CastShadow, true)
    AssertEquals(t.ReceiveDecal, false)
    AssertEquals(t.AllowReceiveDecalWhenAnimated, false)
    AssertEquals(t.IsReflecting, false)
    AssertEquals(t.IsShadowProxy, false)
    AssertEquals(t.RenderChannel, 4)
    AssertEquals(t.CameraOffset, {0.0, 0.0, 0.0})
    AssertEquals(t.HasParentModRelation, false)
    AssertEquals(t.HasGameplayValue, false)
    AssertEquals(t.FileName, "C:/Program Files (x86)/Steam/steamapps/common/Baldurs Gate 3/Data/Mods/Gustav/Globals/WLD_Main_A/Characters/_merged.lsf")


    -- EoCGameObjectTemplate
    AssertEquals(t.DisplayName, "h4a8991d6g1618g41ecga5b8g9f59b82c2237")
    AssertEquals(t.Opacity, 0.0)
    AssertEquals(t.Fadeable, false)
    AssertEquals(t.FadeIn, false)
    AssertEquals(t.SeeThrough, false)
    AssertEquals(t.CollideWithCamera, false)
    AssertEquals(t.HierarchyOnlyFade, false)
    AssertEquals(t.FadeGroup, "")
    AssertEquals(t.FadeChildren, {})
    AssertEquals(t.ShootThroughType, 6)


    -- CharacterTemplate
    AssertEquals(t.Icon, "1088f4ee-579b-b49e-72cc-7ecc5fc5be79-EQ_Laezel_(Icon_Githyanki_Female)")
    AssertEquals(t.Stats, "POC_Player_Fighter")
    AssertEquals(t.SpellSet, "CommonPlayerActions")
    AssertEquals(t.Equipment, "EQ_Laezel")
    -- AssertEquals(t.EquipmentTypes, {})
    AssertEquals(t.Treasures, {"Empty"})
    AssertEquals(t.TradeTreasures, {"Empty"})
    AssertEquals(t.LightID, "362270fc-bf6b-4603-bb8a-6deebbcca47b")
    AssertEquals(t.ActiveCharacterLightID, "3faa1c66-5d35-4e87-868e-d124168b660f")
    AssertEquals(t.BloodType, "Red")
    AssertEquals(t.DefaultDialog, "")
    AssertEquals(t.GeneratePortrait, "Icon_Githyanki_Female")
    AssertEquals(t.LadderAttachOffset, 0.0)
    AssertEquals(t.LadderLoopSpeed, 2.5)
    AssertEquals(t.CanShootThrough, false)
    AssertEquals(t.ShootThroughType, 6)
    AssertEquals(t.WalkThrough, false)
    AssertEquals(t.CanClimbLadders, true)
    AssertEquals(t.IsPlayer, false)
    AssertEquals(t.SpotSneakers, true)
    AssertEquals(t.CanOpenDoors, true)
    AssertEquals(t.AvoidTraps, false)
    AssertEquals(t.InfluenceTreasureLevel, true)
    AssertEquals(t.HardcoreOnly, false)
    AssertEquals(t.NotHardcore, false)
    AssertEquals(t.JumpUpLadders, false)
    AssertEquals(t.EquipmentRace, "06aaae02-bb9e-4fa3-ac00-b08e13a5b0fa")
    AssertEqualsFloat(t.DeathRaycastMinLength, 0.4)
    AssertEquals(t.DeathRaycastMaxLength, 1.5)
    AssertEqualsFloat(t.DeathRaycastHeight, 0.4)
    AssertEquals(t.DeathRaycastVerticalLength, 0.5)
    AssertEquals(t.ExplodedResourceID, "")
    AssertEquals(t.ExplosionFX, "")
    AssertEquals(t.AnubisConfigName, "ORI_Laezel")
    AssertEquals(t.TrophyID, "")
    AssertEquals(t.SoundInitEvent, "")
    AssertEquals(t.SoundObjectIndex, -1)
    AssertEquals(t.SoundAttenuation, -1)
    AssertEquals(t.FoleyShortResourceID, "")
    AssertEquals(t.VocalAttackResourceID, "80f9adec-0dfb-7452-8a6d-33945ae8880c")
    -- ... vocal resources skipped ...
    AssertEquals(t.VocalWeakResourceID, "aac800d0-d1fe-cdd3-84bd-184029dff377")
    AssertEquals(t.CharacterVisualResourceID, "1088f4ee-579b-b49e-72cc-7ecc5fc5be79")
    AssertEquals(t.CoverAmount, 0)
    AssertEquals(t.LevelOverride, -1)
    AssertEquals(t.ForceUnsheathSkills, false)
    AssertEquals(t.CanBeTeleported, true)
    AssertEquals(t.ActivationGroupId, "")
    AssertEquals(t.SoftBodyCollisionTemplate, "e96eed75-850b-4497-b077-a8f62f731bf3")
    AssertEquals(t.RagdollTemplate, "")
    AssertEquals(t.DefaultState, 0)
    AssertEquals(t.IsLootable, true)
    AssertEquals(t.IsEquipmentLootable, true)
    AssertEquals(t.LightChannel, 5)
    AssertEquals(t.AliveInventoryType, 11)
    AssertEquals(t.InventoryType, 8)
    AssertEquals(t.Race, "bdf9b779-002c-4077-b377-8ea7c1faa795")
    AssertEquals(t.Title, "ls::TranslatedStringRepository::s_HandleUnknown")
    AssertEquals(t.AnimationSetResourceID, "")
    AssertEquals(t.HasPlayerApprovalRating, true)
    AssertEquals(t.DisableEquipping, false)
    AssertEquals(t.WorldClimbingSpeed, 6.0)
    AssertEquals(t.UseStandAtDestination, false)
    AssertEquals(t.IsMovementEnabled, true)
    AssertEquals(t.MovementAcceleration, 12.0)
    AssertEqualsFloat(t.MovementSpeedStroll, 1.4)
    AssertEquals(t.MovementSpeedWalk, 2.0)
    AssertEquals(t.MovementSpeedRun, 3.75)
    AssertEquals(t.MovementSpeedSprint, 6.0)
    AssertEquals(t.MovementSpeedDash, 6.0)
    AssertEquals(t.MaxDashDistance, 2.0)
    AssertEqualsFloat(t.MovementStepUpHeight, 0.6)
    AssertEquals(t.MovementTiltToRemap, "1df9c9ee-8dd8-ebeb-3b1a-83d31f141f0a")
    AssertEquals(t.TurningNodeAngle, 30.0)
    AssertEquals(t.TurningNodeOffset, 1.0)
    AssertEquals(t.IsSteeringEnabled, true)
    AssertEquals(t.SteeringSpeedCurveWithoutTransitions, "522881b9-ce7f-4259-9e81-4cbe55f1fc3f")
    AssertEquals(t.SteeringSpeedFallback, 360.0)
    AssertEquals(t.IsWorldClimbingEnabled, true)
    AssertEquals(t.WorldClimbingBlendspace_DownA, "1cd55755-bc55-767c-b32f-2f73fd6621d1")
    -- ... etc ...
    AssertEquals(t.ProbeLookAtOffset, -1.0)
    AssertEquals(t.ProbeSpineAOffset, 4.0)
    AssertEquals(t.ProbeSpineBOffset, 1.0)
    AssertEquals(t.ProbeTiltToOffset, -1.0)
    AssertEquals(t.VFXScale, 2.0)

end

RegisterTests("Character", {
    "TestCharacterEnumeration",
    "TestCharacterProperties",
    "TestCharacterTemplateProperties"
})
