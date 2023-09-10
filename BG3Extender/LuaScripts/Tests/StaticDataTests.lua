function TestGuidResourceEnumeration()
    local resources = Ext.StaticData.GetAll("ClassDescription")
    AssertContains(resources, "d21368ac-c776-465c-9dcf-6123dd52734f")
end

function TestGuidResourceFetch()
    local res = Ext.StaticData.Get("d21368ac-c776-465c-9dcf-6123dd52734f", "ClassDescription")
    AssertEquals(res.ResourceUUID, "d21368ac-c776-465c-9dcf-6123dd52734f")
    AssertEquals(res.Name, "LoreCollege")
end

function TestGuidResourceUpdate()
    local res = Ext.StaticData.Get("d21368ac-c776-465c-9dcf-6123dd52734f", "ClassDescription")
    AssertEquals(res.SoundClassType, "Bard")
    res.SoundClassType = "Paladin"

    res = Ext.StaticData.Get("d21368ac-c776-465c-9dcf-6123dd52734f", "ClassDescription")
    AssertEquals(res.SoundClassType, "Paladin")
    res.SoundClassType = "Bard"
end

function TestGuidResourceLayout()
    local types = {
        "ActionResource",
        "ActionResourceGroup",
        "ClassDescription",
        "Tag",
        "Faction",
        "Race",
        "Origin", 
        "Background",
        "God",
        "Progression",
        "ProgressionDescription",
        "Gossip",
        "ColorDefinition",
        "EquipmentType",
        "Flag",
        "Feat",
        "FeatDescription",
        "PassiveList",
        "SkillList",
        "SpellList",
        "CharacterCreationPreset",
        "CharacterCreationSkinColor",
        "CharacterCreationEyeColor",
        "CharacterCreationHairColor",
        "CharacterCreationAccessorySet",
        "CharacterCreationEquipmentIcons",
        "CharacterCreationIconSettings",
        "CharacterCreationMaterialOverride",
        "CharacterCreationAppearanceMaterial",
        "CharacterCreationPassiveAppearance",
        "CharacterCreationAppearanceVisual",
        "CharacterCreationSharedVisual"
    }

    for i,type in ipairs(types) do
        local resources = Ext.StaticData.GetAll(type)
        for j,uuid in ipairs(resources) do
            local res = Ext.StaticData.Get(uuid, type)
            AssertValid(res)
            AssertEquals(res.ResourceUUID, uuid)
        end
    end
end

RegisterTests("StaticData", {
    "TestGuidResourceEnumeration",
    "TestGuidResourceFetch",
    "TestGuidResourceUpdate",
    "TestGuidResourceLayout"
})
