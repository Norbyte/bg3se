
BEGIN_CLS(extui::Renderable)
END_CLS()


BEGIN_CLS(extui::StyledRenderable)
INHERIT(extui::Renderable)
P_FUN(SetStyle, extui::StyledRenderable::SetStyleVar)
P_FUN(SetColor, extui::StyledRenderable::SetStyleColor)
P(Label)
END_CLS()


BEGIN_CLS(extui::TreeParent)
INHERIT(extui::StyledRenderable)
P_FUN(AddButton, extui::TreeParent::AddButton)
P_FUN(AddText, extui::TreeParent::AddText)
P_FUN(AddCheckbox, extui::TreeParent::AddCheckbox)
P_FUN(AddRadioButton, extui::TreeParent::AddRadioButton)
P_FUN(AddInputText, extui::TreeParent::AddInputText)
P_FUN(AddCombo, extui::TreeParent::AddCombo)
P_FUN(AddDrag, extui::TreeParent::AddDrag)
P_FUN(AddSlider, extui::TreeParent::AddSlider)
P_FUN(AddInputScalar, extui::TreeParent::AddInputScalar)
P_FUN(AddColorEdit, extui::TreeParent::AddColorEdit)
P_FUN(AddColorPicker, extui::TreeParent::AddColorPicker)
END_CLS()


BEGIN_CLS(extui::Window)
INHERIT(extui::TreeParent)
P(Closed)
P(Closeable)
END_CLS()


BEGIN_CLS(extui::Group)
INHERIT(extui::TreeParent)
END_CLS()


BEGIN_CLS(extui::CollapsingHeader)
INHERIT(extui::TreeParent)
END_CLS()


BEGIN_CLS(extui::Text)
INHERIT(extui::StyledRenderable)
END_CLS()


BEGIN_CLS(extui::SeparatorText)
INHERIT(extui::StyledRenderable)
END_CLS()


BEGIN_CLS(extui::Button)
INHERIT(extui::StyledRenderable)
END_CLS()


BEGIN_CLS(extui::Checkbox)
INHERIT(extui::StyledRenderable)
P(Checked)
END_CLS()


BEGIN_CLS(extui::RadioButton)
INHERIT(extui::StyledRenderable)
P(Active)
END_CLS()


BEGIN_CLS(extui::InputText)
INHERIT(extui::StyledRenderable)
P_GETTER_SETTER(Text, GetText, SetText)
END_CLS()


BEGIN_CLS(extui::Combo)
INHERIT(extui::StyledRenderable)
P(Options)
P(SelectedIndex)
END_CLS()


BEGIN_CLS(extui::DragScalar)
INHERIT(extui::StyledRenderable)
P(Value)
P(Min)
P(Max)
P(Components)
END_CLS()


BEGIN_CLS(extui::SliderScalar)
INHERIT(extui::StyledRenderable)
P(Value)
P(Min)
P(Max)
P(Components)
END_CLS()


BEGIN_CLS(extui::InputScalar)
INHERIT(extui::StyledRenderable)
P(Value)
P(Components)
END_CLS()


BEGIN_CLS(extui::ColorEdit)
INHERIT(extui::StyledRenderable)
P(Color)
P(HasAlpha)
END_CLS()


BEGIN_CLS(extui::ColorPicker)
INHERIT(extui::StyledRenderable)
P(Color)
P(HasAlpha)
END_CLS()

