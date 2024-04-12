
BEGIN_CLS(extui::Renderable)
END_CLS()


BEGIN_CLS(extui::StyledRenderable)
INHERIT(extui::Renderable)
P_FUN(SetStyle, extui::StyledRenderable::SetStyleVar)
P_FUN(SetColor, extui::StyledRenderable::SetStyleColor)
P_FUN(Tooltip, extui::TreeParent::Tooltip)
P(Label)
P(OnActivate)
P(OnDeactivate)
END_CLS()


BEGIN_CLS(extui::TreeParent)
INHERIT(extui::StyledRenderable)

P_FUN(AddGroup, extui::TreeParent::AddGroup)
P_FUN(AddCollapsingHeader, extui::TreeParent::AddCollapsingHeader)
P_FUN(AddTabBar, extui::TreeParent::AddTabBar)
P_FUN(AddTree, extui::TreeParent::AddTree)
P_FUN(AddTable, extui::TreeParent::AddTable)
P_FUN(AddPopup, extui::TreeParent::AddPopup)

P_FUN(AddButton, extui::TreeParent::AddButton)
P_FUN(AddText, extui::TreeParent::AddText)
P_FUN(AddBulletText, extui::TreeParent::AddBulletText)
P_FUN(AddSeparatorText, extui::TreeParent::AddSeparatorText)

P_FUN(AddSpacing, extui::TreeParent::AddSpacing)
P_FUN(AddDummy, extui::TreeParent::AddDummy)
P_FUN(AddNewLine, extui::TreeParent::AddNewLine)
P_FUN(AddSeparator, extui::TreeParent::AddSeparator)

P_FUN(AddCheckbox, extui::TreeParent::AddCheckbox)
P_FUN(AddRadioButton, extui::TreeParent::AddRadioButton)
P_FUN(AddInputText, extui::TreeParent::AddInputText)
P_FUN(AddCombo, extui::TreeParent::AddCombo)
P_FUN(AddDrag, extui::TreeParent::AddDrag)
P_FUN(AddDragInt, extui::TreeParent::AddDragInt)
P_FUN(AddSlider, extui::TreeParent::AddSlider)
P_FUN(AddSliderInt, extui::TreeParent::AddSliderInt)
P_FUN(AddInputScalar, extui::TreeParent::AddInputScalar)
P_FUN(AddInputInt, extui::TreeParent::AddInputInt)
P_FUN(AddColorEdit, extui::TreeParent::AddColorEdit)
P_FUN(AddColorPicker, extui::TreeParent::AddColorPicker)
END_CLS()


BEGIN_CLS(extui::Window)
INHERIT(extui::TreeParent)
P(Open)
P(Closeable)
P_BITMASK(Flags)
P(OnClose)
END_CLS()


BEGIN_CLS(extui::Group)
INHERIT(extui::TreeParent)
END_CLS()


BEGIN_CLS(extui::CollapsingHeader)
INHERIT(extui::TreeParent)
P_BITMASK(Flags)
END_CLS()


BEGIN_CLS(extui::TabBar)
INHERIT(extui::TreeParent)
P_FUN(AddTabItem, extui::TabBar::AddTabItem)
P_BITMASK(Flags)
END_CLS()


BEGIN_CLS(extui::TabItem)
INHERIT(extui::TreeParent)
P_BITMASK(Flags)
END_CLS()


BEGIN_CLS(extui::Tree)
INHERIT(extui::TreeParent)
P_BITMASK(Flags)
END_CLS()


BEGIN_CLS(extui::Table)
INHERIT(extui::TreeParent)
P(Columns)
P_BITMASK(Flags)
P_FUN(AddRow, extui::Table::AddRow)
END_CLS()


BEGIN_CLS(extui::TableRow)
INHERIT(extui::TreeParent)
P_BITMASK(Flags)
P_FUN(AddCell, extui::TableRow::AddCell)
END_CLS()


BEGIN_CLS(extui::TableCell)
INHERIT(extui::TreeParent)
END_CLS()


BEGIN_CLS(extui::Tooltip)
INHERIT(extui::TreeParent)
END_CLS()


BEGIN_CLS(extui::Popup)
INHERIT(extui::TreeParent)
P_BITMASK(Flags)
P_FUN(Open, extui::Popup::Open)
END_CLS()


BEGIN_CLS(extui::Text)
INHERIT(extui::StyledRenderable)
END_CLS()


BEGIN_CLS(extui::BulletText)
INHERIT(extui::StyledRenderable)
END_CLS()


BEGIN_CLS(extui::SeparatorText)
INHERIT(extui::StyledRenderable)
END_CLS()


BEGIN_CLS(extui::Spacing)
INHERIT(extui::StyledRenderable)
END_CLS()


BEGIN_CLS(extui::Dummy)
INHERIT(extui::StyledRenderable)
P(Width)
P(Height)
END_CLS()


BEGIN_CLS(extui::NewLine)
INHERIT(extui::StyledRenderable)
END_CLS()


BEGIN_CLS(extui::Separator)
INHERIT(extui::StyledRenderable)
END_CLS()


BEGIN_CLS(extui::Button)
INHERIT(extui::StyledRenderable)
P(OnClick)
END_CLS()


BEGIN_CLS(extui::Checkbox)
INHERIT(extui::StyledRenderable)
P(Checked)
P(OnChange)
END_CLS()


BEGIN_CLS(extui::RadioButton)
INHERIT(extui::StyledRenderable)
P(Active)
P(OnChange)
END_CLS()


BEGIN_CLS(extui::InputText)
INHERIT(extui::StyledRenderable)
P_GETTER_SETTER(Text, GetText, SetText)
P_BITMASK(Flags)
P(OnChange)
END_CLS()


BEGIN_CLS(extui::Combo)
INHERIT(extui::StyledRenderable)
P(Options)
P(SelectedIndex)
P_BITMASK(Flags)
P(OnChange)
END_CLS()


BEGIN_CLS(extui::DragScalar)
INHERIT(extui::StyledRenderable)
P(Value)
P(Min)
P(Max)
P(Components)
P_BITMASK(Flags)
P(OnChange)
END_CLS()


BEGIN_CLS(extui::DragInt)
INHERIT(extui::StyledRenderable)
P(Value)
P(Min)
P(Max)
P(Components)
P_BITMASK(Flags)
P(OnChange)
END_CLS()


BEGIN_CLS(extui::SliderScalar)
INHERIT(extui::StyledRenderable)
P(Value)
P(Min)
P(Max)
P(Components)
P_BITMASK(Flags)
P(OnChange)
END_CLS()


BEGIN_CLS(extui::SliderInt)
INHERIT(extui::StyledRenderable)
P(Value)
P(Min)
P(Max)
P(Components)
P_BITMASK(Flags)
P(OnChange)
END_CLS()


BEGIN_CLS(extui::InputScalar)
INHERIT(extui::StyledRenderable)
P(Value)
P(Components)
P_BITMASK(Flags)
P(OnChange)
END_CLS()


BEGIN_CLS(extui::InputInt)
INHERIT(extui::StyledRenderable)
P(Value)
P(Components)
P_BITMASK(Flags)
P(OnChange)
END_CLS()


BEGIN_CLS(extui::ColorEdit)
INHERIT(extui::StyledRenderable)
P(Color)
P_BITMASK(Flags)
P(OnChange)
END_CLS()


BEGIN_CLS(extui::ColorPicker)
INHERIT(extui::StyledRenderable)
P(Color)
P_BITMASK(Flags)
P(OnChange)
END_CLS()

