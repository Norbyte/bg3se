---@diagnostic disable

return {
    ["NoesisBaseObject.DependencyProperties"] = {
        ReplaceReturnValue = "table<string, any>"
    },
    ["NoesisBaseObject.DirectProperties"] = {
        ReplaceReturnValue = "table<string, any>"
    },
    ["NoesisBaseObject.GetAllProperties"] = {
        ReplaceReturnValue = "table<string, any>"
    },

    ["GnGenomeBlueprintInstance.GetNode"] = {
        ReplaceReturnValue = "GnGenomeNode?"
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
    ["ExtuiTreeParent.AddMenu"] = {
        ReplaceReturnValue = "ExtuiMenu"
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
    ["ExtuiTreeParent.AddProgressBar"] = {
        ReplaceReturnValue = "ExtuiProgressBar"
    },
    ["ExtuiTreeParent.AddRadioButton"] = {
        ReplaceReturnValue = "ExtuiRadioButton"
    },
    ["ExtuiTreeParent.AddSelectable"] = {
        ReplaceReturnValue = "ExtuiSelectable"
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
    ["ExtuiTreeParent.AddTextLink"] = {
        ReplaceReturnValue = "ExtuiTextLink"
    },
    ["ExtuiTreeParent.AddTree"] = {
        ReplaceReturnValue = "ExtuiTree"
    }
}