---@diagnostic disable

return {
    ["Ext_Stats.Get"] = {
        efore = "--- @return StatEntryType"
    },
    ["Ext_ServerOsiris.RegisterListener"] = {
        Replace = [[--- @param name string 
--- @param arity integer 
--- @param typeName OsirisEventType 
--- @param callback fun(...:string|number)
function Ext_ServerOsiris.RegisterListener(name, arity, typeName, callback) end
]]
    },

    -- IMGUI type helpers
    ["Ext_ClientIMGUI.NewWindow"] = {
        ReplaceReturnValue = "ExtuiWindow"
    },
    
    ["ExtuiMenu.AddItem"] = {
        ReplaceReturnValue = "ExtuiMenuItem"
    },
    ["ExtuiMenu.AddMenu"] = {
        ReplaceReturnValue = "ExtuiMenu"
    },
    ["ExtuiMenuBar.AddMenu"] = {
        ReplaceReturnValue = "ExtuiMenu"
    },

    ["ExtuiStyledRenderable.Tooltip"] = {
        ReplaceReturnValue = "ExtuiTooltip"
    },

    ["ExtuiTabBar.AddTabItem"] = {
        ReplaceReturnValue = "ExtuiTabItem"
    },

    ["ExtuiTable.AddRow"] = {
        ReplaceReturnValue = "ExtuiTableRow"
    },

    ["ExtuiTableRow.AddCell"] = {
        ReplaceReturnValue = "ExtuiTableCell"
    },

    ["ExtuiWindow.AddMainMenu"] = {
        ReplaceReturnValue = "ExtuiMenu"
    },

    ["ExtuiTreeParent.AddBulletText"] = {
        ReplaceReturnValue = "ExtuiBulletText"
    },
    ["ExtuiTreeParent.AddButton"] = {
        ReplaceReturnValue = "ExtuiButton"
    },
    ["ExtuiTreeParent.AddCheckbox"] = {
        ReplaceReturnValue = "ExtuiCheckbox"
    },
    ["ExtuiTreeParent.AddCollapsingHeader"] = {
        ReplaceReturnValue = "ExtuiCollapsingHeader"
    },
    ["ExtuiTreeParent.AddColorEdit"] = {
        ReplaceReturnValue = "ExtuiColorEdit"
    },
    ["ExtuiTreeParent.AddColorPicker"] = {
        ReplaceReturnValue = "ExtuiColorPicker"
    },
    ["ExtuiTreeParent.AddCombo"] = {
        ReplaceReturnValue = "ExtuiCombo"
    },
    ["ExtuiTreeParent.AddDrag"] = {
        ReplaceReturnValue = "ExtuiDrag"
    },
    ["ExtuiTreeParent.AddDragInt"] = {
        ReplaceReturnValue = "ExtuiDragInt"
    },
    ["ExtuiTreeParent.AddDummy"] = {
        ReplaceReturnValue = "ExtuiDummy"
    },
    ["ExtuiTreeParent.AddGroup"] = {
        ReplaceReturnValue = "ExtuiGroup"
    },
    ["ExtuiTreeParent.AddIcon"] = {
        ReplaceReturnValue = "ExtuiIcon"
    },
    ["ExtuiTreeParent.AddImage"] = {
        ReplaceReturnValue = "ExtuiImage"
    },
    ["ExtuiTreeParent.AddImageButton"] = {
        ReplaceReturnValue = "ExtuiImageButton"
    },
    ["ExtuiTreeParent.AddInputInt"] = {
        ReplaceReturnValue = "ExtuiInputInt"
    },
    ["ExtuiTreeParent.AddInputScalar"] = {
        ReplaceReturnValue = "ExtuiInputScalar"
    },
    ["ExtuiTreeParent.AddInputText"] = {
        ReplaceReturnValue = "ExtuiInputText"
    },
    ["ExtuiTreeParent.AddNewLine"] = {
        ReplaceReturnValue = "ExtuiNewLine"
    },
    ["ExtuiTreeParent.AddPopup"] = {
        ReplaceReturnValue = "ExtuiPopup"
    },
    ["ExtuiTreeParent.AddChildWindow"] = {
        ReplaceReturnValue = "ExtuiChildWindow"
    },
    ["ExtuiTreeParent.AddRadioButton"] = {
        ReplaceReturnValue = "ExtuiRadioButton"
    },
    ["ExtuiTreeParent.AddSeparator"] = {
        ReplaceReturnValue = "ExtuiSeparator"
    },
    ["ExtuiTreeParent.AddSeparatorText"] = {
        ReplaceReturnValue = "ExtuiSeparatorText"
    },
    ["ExtuiTreeParent.AddSlider"] = {
        ReplaceReturnValue = "ExtuiSlider"
    },
    ["ExtuiTreeParent.AddSliderInt"] = {
        ReplaceReturnValue = "ExtuiSliderInt"
    },
    ["ExtuiTreeParent.AddSpacing"] = {
        ReplaceReturnValue = "ExtuiSpacing"
    },
    ["ExtuiTreeParent.AddTabBar"] = {
        ReplaceReturnValue = "ExtuiTabBar"
    },
    ["ExtuiTreeParent.AddTable"] = {
        ReplaceReturnValue = "ExtuiTable"
    },
    ["ExtuiTreeParent.AddText"] = {
        ReplaceReturnValue = "ExtuiText"
    },
    ["ExtuiTreeParent.AddTree"] = {
        ReplaceReturnValue = "ExtuiTree"
    }
}