
BEGIN_CLS(extui::Renderable)
END_CLS()


BEGIN_CLS(extui::StyledRenderable)
INHERIT(extui::Renderable)
P_FUN(SetStyle, extui::StyledRenderable::SetStyleVar)
P_FUN(SetColor, extui::StyledRenderable::SetStyleColor)
END_CLS()


BEGIN_CLS(extui::Window)
INHERIT(extui::StyledRenderable)
END_CLS()


BEGIN_CLS(extui::Text)
INHERIT(extui::StyledRenderable)
END_CLS()


BEGIN_CLS(extui::Button)
INHERIT(extui::StyledRenderable)
END_CLS()

