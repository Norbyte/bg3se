#include <stdafx.h>

#include <Extender/Client/IMGUI/IMGUI.h>
#include <Extender/Client/IMGUI/Objects.h>
#include <Extender/Shared/ScriptHelpers.h>
#include <CoreLib/Wrappers.h>

// #define IMGUI_DEBUG(msg, ...) DEBUG("[IMGUI] " msg, __VA_ARGS__)
#define IMGUI_DEBUG(msg, ...)

#define IMGUI_FRAME_DEBUG(msg, ...) if ((frameNo_ % 100) == 0) { IMGUI_DEBUG(msg, __VA_ARGS__); }

#include <Extender/Client/IMGUI/Vulkan.inl>
#include <Extender/Client/IMGUI/DX11.inl>
#include <Lua/Shared/LuaMethodCallHelpers.h>

BEGIN_NS(extui)

void DrawingContext::PushScaling(GuiMeasureScaling scaling)
{
    ScalingStack.push_back(Scaling);
    Scaling = scaling;
}

void DrawingContext::PopScaling()
{
    Scaling = ScalingStack.pop_last();
}

ImageReference::~ImageReference()
{
    if (TextureHandle) {
        gExtender->IMGUI().UnregisterTexture(TextureHandle, TextureResource);
    }
}

bool ImageReference::Bind(FixedString const& iconOrTexture)
{
    if (!BindIcon(iconOrTexture) && !BindTexture(iconOrTexture)) {
        WARN("Unable to load icon or texture resource '%s'", iconOrTexture.GetString());
        return false;
    }

    return true;
}

bool ImageReference::BindTexture(FixedString const& textureUuid)
{
    TextureResource = textureUuid;
    auto result = gExtender->IMGUI().RegisterTexture(textureUuid);
    if (result) {
        TextureHandle = result->OpaqueHandle;
        Size = glm::vec2((float)result->Width, (float)result->Height);
        return true;
    } else {
        return false;
    }
}

bool ImageReference::BindIcon(FixedString const& iconName)
{
    auto atlas = (*GetStaticSymbols().ls__gTextureAtlasMap)->IconMap.try_get(iconName);
    if (!atlas) {
        return false;
    }

    auto uvs = atlas->Icons.try_get(iconName);
    if (!uvs) {
        WARN("Failed to find UVs for icon '%s'", iconName.GetString());
        return false;
    }

    auto texture = gExtender->IMGUI().RegisterTexture(atlas->TextureUuid);
    if (!texture) {
        WARN("Failed to load texture '%s' for icon '%s'", atlas->TextureUuid.GetString(), iconName.GetString());
        return false;
    }

    Size = glm::vec2(
        static_cast<float>(atlas->IconWidth),
        static_cast<float>(atlas->IconHeight)
    );

    UV0 = uvs->UV0;
    UV1 = uvs->UV1;

    TextureResource = atlas->TextureUuid;
    Icon = iconName;
    TextureHandle = texture->OpaqueHandle;
    return true;
}

ImTextureID ImageReference::PrepareRender()
{
    if (TextureId) return TextureId;
    if (!IsValid()) return ImTextureID();

    auto tex = gExtender->IMGUI().BindTexture(TextureHandle);
    if (tex) {
        TextureId = *tex;
    } else {
        // Failed to create render texture, remove binding
        gExtender->IMGUI().UnregisterTexture(TextureHandle, TextureResource);
        TextureHandle = 0;
    }

    return TextureId;
}

template <class T>
T* TreeParent::AddChild()
{
    auto child = Manager->CreateRenderable<T>();
    child->Parent = Handle;
    Children.push_back(child->Handle);
    return child;
}

RenderingBackend::~RenderingBackend() {}

Renderable::~Renderable() {}

void Renderable::Destroy()
{
    if (Parent != InvalidHandle) {
        auto parent = Manager->GetRenderable(Parent);
        if (parent != nullptr) {
            static_cast<TreeParent*>(parent)->RemoveChild(lua::ImguiHandle(Handle));
        }
    } else {
        Manager->DestroyRenderable(Handle);
    }
}

lua::ImguiHandle Renderable::GetParent() const
{
    return lua::ImguiHandle(Parent);
}

bool StyledRenderable::OverridesClickEvent() const
{
    return false;
}

lua::ImguiHandle StyledRenderable::GetDragPreview() const
{
    return lua::ImguiHandle(DragPreview);
}

FixedString StyledRenderable::GetDragDropType() const
{
    return DragDropType;
}

void StyledRenderable::SetDragDropType(lua_State* L, FixedString type)
{
    if (type.GetLength() >= IM_ARRAYSIZE(ImGuiPayload::DataType)) {
        luaL_error(L, "Drag drop type must be at most %d characters", IM_ARRAYSIZE(ImGuiPayload::DataType));
        return;
    }

    DragDropType = type;
}

void StyledRenderable::Render(DrawingContext& context)
{
    if (!Visible) return;

    ImFont* font{ nullptr };
    PushStyleChanges(context);
    PushWindowStyleChanges(context, font);

    StyledRender(context);
    UpdateStatusFlags();
    FireEvents();

    PopWindowStyleChanges(font);
    PopStyleChanges();

    DrawTooltip(context);
    HandleDragDrop(context);
}


void StyledRenderable::FireEvents()
{
    if (OnClick && !OverridesClickEvent() && ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
        Manager->GetEventQueue().Call(OnClick, lua::ImguiHandle(Handle));
    }
    
    if (OnRightClick && ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        Manager->GetEventQueue().Call(OnRightClick, lua::ImguiHandle(Handle));
    }

    if (OnActivate && ImGui::IsItemActivated()) {
        Manager->GetEventQueue().Call(OnActivate, lua::ImguiHandle(Handle));
    }

    if (OnDeactivate && ImGui::IsItemDeactivated()) {
        Manager->GetEventQueue().Call(OnDeactivate, lua::ImguiHandle(Handle));
    }

    // IsItemHovered() is expensive so we need to make sure we have a handler before doing a hover check
    if (OnHoverEnter || OnHoverLeave) {
        auto hovered = ImGui::IsItemHovered();
        if (WasHovered != hovered) {
            if (hovered && OnHoverEnter) {
                Manager->GetEventQueue().Call(OnHoverEnter, lua::ImguiHandle(Handle));
            }
            if (!hovered && OnHoverLeave) {
                Manager->GetEventQueue().Call(OnHoverLeave, lua::ImguiHandle(Handle));
            }
        }
        WasHovered = hovered;
    }
}


void StyledRenderable::HandleDragDrop(DrawingContext& context)
{
    if (CanDrag && DragDropType) {
        if (ImGui::BeginDragDropSource((ImGuiDragDropFlags)DragFlags)) {
            if (!IsDragging) {
                if (OnDragStart) {
                    DragPreview = Manager->CreateRenderable<Group>()->Handle;
                    Manager->GetEventQueue().Call(OnDragStart, lua::ImguiHandle(Handle), lua::ImguiHandle(DragPreview));
                }
                IsDragging = true;
            }

            ImGui::SetDragDropPayload(DragDropType.GetString(), &Handle, sizeof(Handle));
            if (DragPreview) {
                auto preview = Manager->GetRenderable(DragPreview);
                if (preview) {
                    preview->Render(context);
                }
            }
            ImGui::EndDragDropSource();
        } else if (IsDragging) {
            Manager->GetEventQueue().Call(OnDragEnd, lua::ImguiHandle(Handle));
            if (DragPreview) {
                Manager->DestroyRenderable(DragPreview);
                DragPreview = InvalidHandle;
            }
            IsDragging = false;
        }
    }

    if (DragDropType && OnDragDrop && ImGui::BeginDragDropTarget())
    {
        auto payload = ImGui::AcceptDragDropPayload(DragDropType.GetString());
        if (payload) {
            auto source = *reinterpret_cast<HandleType*>(payload->Data);
            Manager->GetEventQueue().Call(OnDragDrop, lua::ImguiHandle(Handle), lua::ImguiHandle(source));
        }
        ImGui::EndDragDropTarget();
    }
}


void StyledRenderable::DrawTooltip(DrawingContext& context)
{
    if (tooltip_) {
        auto tooltip = Manager->GetRenderable(tooltip_.Handle);
        if (tooltip) tooltip->Render(context);
    }
}


void StyledRenderable::UpdateStatusFlags()
{
    auto status = (GuiItemStatusFlags)ImGui::GetItemStatusFlags();
    if (ImGui::IsItemFocused()) status |= GuiItemStatusFlags::Focused;
    if (ImGui::IsItemActive()) status |= GuiItemStatusFlags::Active;
    if (StatusFlags != status) StatusFlags = status;
}


void StyledRenderable::PushStyleChanges(DrawingContext& context)
{
    for (auto const& var : StyleVars) {
        auto varInfo = ImGui::GetStyleVarInfo((ImGuiStyleVar)var.Key);
        if (varInfo->Count == 2) {
            ImGui::PushStyleVar((ImGuiStyleVar)var.Key, ToImVec(var.Value));
        } else {
            ImGui::PushStyleVar((ImGuiStyleVar)var.Key, var.Value.x);
        }
    }

    for (auto const& var : StyleColors) {
        ImGui::PushStyleColor((ImGuiCol)var.Key, ToImVec(var.Value));
    }

    if (ItemFlags != (GuiItemFlags)0) ImGui::PushItemFlag((ImGuiItemFlags)ItemFlags, true);
    if (ItemWidth) ImGui::SetNextItemWidth(context.Scale(*ItemWidth));

    if (RequestActivate) {
        ImGui::ActivateItemByID(ImGui::GetCurrentWindow()->GetID(Label.c_str()));
        RequestActivate = false;
    }
}


void StyledRenderable::PopStyleChanges()
{
    if (ItemFlags != (GuiItemFlags)0) ImGui::PopItemFlag();

    if (!StyleColors.empty()) {
        ImGui::PopStyleColor(StyleColors.size());
    }

    if (!StyleVars.empty()) {
        ImGui::PopStyleVar(StyleVars.size());
    }
}


void StyledRenderable::PushWindowStyleChanges(DrawingContext& context, ImFont*& font)
{
    if (Font) {
        auto info = gExtender->IMGUI().GetFont(Font);
        if (info) font = info->Font;
    }

    if (font) ImGui::PushFont(font);

    if (!IDContext.empty()) {
        ImGui::PushID(IDContext.data(), IDContext.data() + IDContext.size());
    } else if (Label.empty()) {
        ImGui::PushID((int32_t)Handle | (int32_t)(Handle >> 32));
    }

    if (PositionOffset) {
        auto pos = ImGui::GetCursorPos();
        auto offset = context.Scale(*PositionOffset);
        ImGui::SetCursorPos(ImVec2(pos.x + offset.x, pos.y + offset.y));
    } else if (AbsolutePosition) {
        ImGui::SetCursorPos(ToImVec(*AbsolutePosition));
    }

    if (SameLine) ImGui::SameLine();
    if (SamePosition) ImGui::SamePosition();
    if (TextWrapPos) ImGui::PushTextWrapPos(context.Scale(*TextWrapPos));
}


void StyledRenderable::PopWindowStyleChanges(ImFont* font)
{
    if (TextWrapPos) ImGui::PopTextWrapPos();

    if (!IDContext.empty() || Label.empty()) {
        ImGui::PopID();
    }

    if (font) ImGui::PopFont();
}


std::optional<float> StyledRenderable::GetStyleVar(GuiStyleVar var)
{
    for (auto& kv : StyleVars) {
        if (kv.Key == var) {
            return kv.Value.x;
        }
    }

    return {};
}


void StyledRenderable::SetStyleVar(GuiStyleVar var, float value, std::optional<float> value2)
{
    for (auto& kv : StyleVars) {
        if (kv.Key == var) {
            kv.Value = glm::vec2(value, value2 ? *value2 : value);
            return;
        }
    }

    StyleVars.push_back(StyleVar{ var, glm::vec2(value, value2 ? *value2 : value) });
}


std::optional<glm::vec4> StyledRenderable::GetStyleColor(GuiColor color)
{
    for (auto& kv : StyleColors) {
        if (kv.Key == color) {
            return kv.Value;
        }
    }

    return {};
}


void StyledRenderable::SetStyleColor(GuiColor color, glm::vec4 value)
{
    for (auto& kv : StyleColors) {
        if (kv.Key == color) {
            kv.Value = value;
            return;
        }
    }

    StyleColors.push_back(StyleColor{ color, value });
}


lua::ImguiHandle StyledRenderable::Tooltip()
{
    if (tooltip_) return tooltip_;

    tooltip_ = Manager->CreateRenderable<extui::Tooltip>();
    return tooltip_;
}

void StyledRenderable::Activate()
{
    RequestActivate = true;
}

TreeParent::~TreeParent()
{
    RemoveAllChildren();
}


void TreeParent::StyledRender(DrawingContext& context)
{
    if (BeginRender(context)) {
        for (auto childHandle : Children) {
            auto child = Manager->GetRenderable(childHandle);
            if (child) {
                child->Render(context);
            }
        }
    }

    EndRender(context);
}


lua::ImguiHandle TreeParent::AddGroup(char const* label)
{
    auto btn = AddChild<Group>();
    btn->Label = label;
    return btn;
}


lua::ImguiHandle TreeParent::AddCollapsingHeader(char const* label)
{
    auto btn = AddChild<CollapsingHeader>();
    btn->Label = label;
    return btn;
}


lua::ImguiHandle TreeParent::AddTabBar(char const* label)
{
    auto btn = AddChild<TabBar>();
    btn->Label = label;
    return btn;
}


lua::ImguiHandle TreeParent::AddTree(char const* label)
{
    auto e = AddChild<Tree>();
    e->Label = label;
    return e;
}


lua::ImguiHandle TreeParent::AddTable(char const* label, uint32_t columns)
{
    auto tbl = AddChild<Table>();
    tbl->Label = label;
    tbl->Columns = columns;
    return tbl;
}


lua::ImguiHandle TreeParent::AddPopup(char const* label)
{
    auto e = AddChild<Popup>();
    e->Label = label;
    return e;
}


lua::ImguiHandle TreeParent::AddChildWindow(char const* label)
{
    auto e = AddChild<ChildWindow>();
    e->Label = label;
    return e;
}


lua::ImguiHandle TreeParent::AddMenu(char const* label)
{
    auto menu = AddChild<Menu>();
    menu->Label = label;
    return menu;
}


lua::ImguiHandle TreeParent::AddSelectable(char const* label, std::optional<GuiSelectableFlags> flags, std::optional<glm::vec2> size)
{
    auto sel = AddChild<Selectable>();
    sel->Label = label;
    if (flags) sel->Flags = *flags;
    if (size) sel->Size = size;
    return sel;
}


lua::ImguiHandle TreeParent::AddButton(char const* label)
{
    auto btn = AddChild<Button>();
    btn->Label = label;
    return btn;
}


lua::ImguiHandle TreeParent::AddImageButton(char const* label, FixedString iconOrTexture,
    std::optional<glm::vec2> size, std::optional<glm::vec2> uv0, std::optional<glm::vec2> uv1)
{
    auto btn = AddChild<ImageButton>();
    btn->Image.Bind(iconOrTexture);
    if (size) btn->Image.Size = *size;
    if (uv0) btn->Image.UV0 = *uv0;
    if (uv1) btn->Image.UV1 = *uv1;
    btn->Label = label;
    return btn;
}


lua::ImguiHandle TreeParent::AddImage(FixedString iconOrTexture,
    std::optional<glm::vec2> size, std::optional<glm::vec2> uv0, std::optional<glm::vec2> uv1)
{
    auto img = AddChild<Image>();
    img->ImageData.Bind(iconOrTexture);
    if (size) img->ImageData.Size = *size;
    if (uv0) img->ImageData.UV0 = *uv0;
    if (uv1) img->ImageData.UV1 = *uv1;

    return img;
}


lua::ImguiHandle TreeParent::AddIcon(FixedString iconName, std::optional<glm::vec2> size)
{
    WARN("TreeParent::AddIcon() is deprecated; use TreeParent::AddImage() instead");
    return AddImage(iconName, size, {}, {});
}


lua::ImguiHandle TreeParent::AddText(char const* label)
{
    auto txt = AddChild<Text>();
    txt->Label = label;
    return txt;
}


lua::ImguiHandle TreeParent::AddTextLink(char const* label)
{
    auto txt = AddChild<TextLink>();
    txt->Label = label;
    return txt;
}


lua::ImguiHandle TreeParent::AddBulletText(char const* label)
{
    auto txt = AddChild<BulletText>();
    txt->Label = label;
    return txt;
}


lua::ImguiHandle TreeParent::AddSeparatorText(char const* label)
{
    auto txt = AddChild<SeparatorText>();
    txt->Label = label;
    return txt;
}


lua::ImguiHandle TreeParent::AddSpacing()
{
    return AddChild<Spacing>();
}


lua::ImguiHandle TreeParent::AddDummy(float width, float height)
{
    auto txt = AddChild<Dummy>();
    txt->Width = width;
    txt->Height = height;
    return txt;
}


lua::ImguiHandle TreeParent::AddNewLine()
{
    return AddChild<NewLine>();
}


lua::ImguiHandle TreeParent::AddSeparator()
{
    return AddChild<Separator>();
}


lua::ImguiHandle TreeParent::AddCheckbox(char const* label, std::optional<bool> checked)
{
    auto e = AddChild<Checkbox>();
    e->Label = label;
    if (checked) e->Checked = *checked;
    return e;
}


lua::ImguiHandle TreeParent::AddRadioButton(char const* label, std::optional<bool> active)
{
    auto e = AddChild<RadioButton>();
    e->Label = label;
    if (active) e->Active = *active;
    return e;
}


lua::ImguiHandle TreeParent::AddInputText(char const* label, std::optional<STDString> value)
{
    auto e = AddChild<InputText>();
    e->Label = label;
    if (value) {
        e->SetText(*value);
    }

    return e;
}


lua::ImguiHandle TreeParent::AddCombo(char const* label)
{
    auto e = AddChild<Combo>();
    e->Label = label;
    return e;
}


lua::ImguiHandle TreeParent::AddDrag(char const* label, std::optional<float> value, std::optional<float> min, std::optional<float> max)
{
    auto e = AddChild<DragScalar>();
    e->Label = label;
    if (value) e->Value = glm::vec4(*value);
    if (min) e->Min = glm::vec4(*min);
    if (max) e->Max = glm::vec4(*max);
    return e;
}


lua::ImguiHandle TreeParent::AddDragInt(char const* label, std::optional<int> value, std::optional<int> min, std::optional<int> max)
{
    auto e = AddChild<DragInt>();
    e->Label = label;
    if (value) e->Value = glm::ivec4(*value);
    if (min) e->Min = glm::ivec4(*min);
    if (max) e->Max = glm::ivec4(*max);
    return e;
}


lua::ImguiHandle TreeParent::AddSlider(char const* label, std::optional<float> value, std::optional<float> min, std::optional<float> max)
{
    auto e = AddChild<SliderScalar>();
    e->Label = label;
    if (value) e->Value = glm::vec4(*value);
    if (min) e->Min = glm::vec4(*min);
    if (max) e->Max = glm::vec4(*max);
    return e;
}


lua::ImguiHandle TreeParent::AddSliderInt(char const* label, std::optional<int> value, std::optional<int> min, std::optional<int> max)
{
    auto e = AddChild<SliderInt>();
    e->Label = label;
    if (value) e->Value = glm::ivec4(*value);
    if (min) e->Min = glm::ivec4(*min);
    if (max) e->Max = glm::ivec4(*max);
    return e;
}


lua::ImguiHandle TreeParent::AddInputScalar(char const* label, std::optional<float> value)
{
    auto e = AddChild<InputScalar>();
    e->Label = label;
    if (value) e->Value = glm::vec4(*value);
    return e;
}


lua::ImguiHandle TreeParent::AddInputInt(char const* label, std::optional<int> value)
{
    auto e = AddChild<InputInt>();
    e->Label = label;
    if (value) e->Value = glm::ivec4(*value);
    return e;
}


lua::ImguiHandle TreeParent::AddColorEdit(char const* label, std::optional<glm::vec3> value)
{
    auto e = AddChild<ColorEdit>();
    e->Label = label;
    if (value) e->Color = glm::vec4(*value, 1.0f);
    return e;
}


lua::ImguiHandle TreeParent::AddColorPicker(char const* label, std::optional<glm::vec3> value)
{
    auto e = AddChild<ColorPicker>();
    e->Label = label;
    if (value) e->Color = glm::vec4(*value, 1.0f);
    return e;
}


lua::ImguiHandle TreeParent::AddProgressBar()
{
    return AddChild<ProgressBar>();
}


bool TreeParent::RemoveChild(lua::ImguiHandle child)
{
    auto it = Children.find(child.Handle);
    if (it != Children.end()) {
        Children.erase(it);
        return Manager->DestroyRenderable(child.Handle);
    } else {
        return false;
    }
}

bool TreeParent::DetachChild(lua::ImguiHandle child)
{
    auto it = Children.find(child.Handle);
    if (it != Children.end()) {
        Children.erase(it);
        auto ele = Manager->GetRenderable(child.Handle);
        if (ele != nullptr) {
            ele->Parent = InvalidHandle;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool TreeParent::AttachChild(lua::ImguiHandle child)
{
    auto ele = Manager->GetRenderable(child.Handle);
    if (ele == nullptr) {
        ERR("Tried to attach nonexistent child UI element");
        return false;
    }
    
    if (ele->Parent != InvalidHandle) {
        ERR("Element is already attached to a parent");
        return false;
    }

    ele->Parent = Handle;
    Children.push_back(ele->Handle);
    return true;
}

void TreeParent::RemoveAllChildren()
{
    for (auto child : Children) {
        auto ele = Manager->GetRenderable(child);
        if (ele) {
            ele->Parent = InvalidHandle;
            Manager->DestroyRenderable(child);
        }
    }

    Children.clear();
}

// Lua helper for making a copy of children without returning a reference to the internal child list.
// Needed until Lua read-only container proxies are added
Array<lua::ImguiHandle> TreeParent::GetChildren() const
{
    Array<lua::ImguiHandle> handles;
    for (auto h : Children) {
        handles.push_back(lua::ImguiHandle(h));
    }
    return handles;
}

void WindowBase::SetPos(glm::vec2 pos, std::optional<GuiCond> cond, std::optional<glm::vec2> pivot)
{
    req_.Pos = WindowRenderRequests::SetPos { 
        ToImVec(pos),
        ToImVec(pivot),
        cond ? (ImGuiCond )*cond : ImGuiCond_Always
    };
}

void WindowBase::SetSize(glm::vec2 size, std::optional<GuiCond> cond)
{
    req_.Size = WindowRenderRequests::SetSize{ ToImVec(size), cond ? (ImGuiCond)*cond : ImGuiCond_Always };
}

void WindowBase::SetSizeConstraints(std::optional<glm::vec2> size_min, std::optional<glm::vec2> size_max)
{
    if (size_min || size_max) {
        req_.SizeConstraints = WindowRenderRequests::SetSizeConstraints{ 
            ToImVec(size_min),
            ToImVec(size_max)
        };
    } else {
        req_.SizeConstraints = {};
    }
}

void WindowBase::SetContentSize(std::optional<glm::vec2> size)
{
    req_.ContentSize = size ? std::optional<ImVec2>(ToImVec(*size)) : std::optional<ImVec2>{};
}

void WindowBase::SetCollapsed(bool collapsed, std::optional<GuiCond> cond)
{
    req_.Collapsed = WindowRenderRequests::SetCollapsed{ collapsed, cond ? (ImGuiCond)*cond : ImGuiCond_Always };
}

void WindowBase::SetFocus()
{
    req_.Focus = true;
}

void WindowBase::SetScroll(std::optional<glm::vec2> scroll)
{
    req_.Scroll = scroll ? std::optional<ImVec2>(ToImVec(*scroll)) : std::optional<ImVec2>{};
}

void WindowBase::SetBgAlpha(std::optional<float> alpha)
{
    req_.BgAlpha = alpha;
}

void WindowBase::BeginDrawingWindow()
{
}

void WindowBase::EndDrawingWindow()
{
    auto size = ImGui::GetWindowSize();
    LastSize = glm::vec2(size.x, size.y);
    auto pos = ImGui::GetWindowPos();
    LastPosition = glm::vec2(pos.x, pos.y);
}

void WindowBase::ProcessRenderSettings(DrawingContext& context)
{
    if (req_.Pos) {
        ImGui::SetNextWindowPos(req_.Pos->Pos, req_.Pos->Cond, req_.Pos->Pivot);
        req_.Pos = {};
    }

    if (req_.Size) {
        ImGui::SetNextWindowSize(context.Scale(req_.Size->Size), req_.Size->Cond);
        req_.Size = {};
    }

    if (req_.SizeConstraints) {
        ImGui::SetNextWindowSizeConstraints(
            context.Scale(req_.SizeConstraints->SizeMin), 
            context.Scale(req_.SizeConstraints->SizeMax));
        req_.SizeConstraints = {};
    }

    if (req_.ContentSize) {
        ImGui::SetNextWindowContentSize(context.Scale(*req_.ContentSize));
        req_.ContentSize = {};
    }

    if (req_.Scroll) {
        ImGui::SetNextWindowScroll(context.Scale(*req_.Scroll));
        req_.Scroll = {};
    }

    if (req_.Collapsed) {
        ImGui::SetNextWindowCollapsed(req_.Collapsed->Collapsed, req_.Collapsed->Cond);
        req_.Collapsed = {};
    }

    if (req_.Focus) {
        ImGui::SetNextWindowFocus();
        req_.Focus = false;
    }

    if (req_.BgAlpha) {
        ImGui::SetNextWindowBgAlpha(*req_.BgAlpha);
        req_.BgAlpha = {};
    }
}

bool Window::BeginRender(DrawingContext& context)
{
    if (!Open || (Label.empty() && IDContext.empty())) return false;

    ProcessRenderSettings(context);

    bool wasOpen = Open;
    context.PushScaling(Scaling);
    bool renderChildren = ImGui::Begin(Label.c_str(), Closeable ? &Open : nullptr, (ImGuiWindowFlags)Flags);

    rendering_ = true;
    if (wasOpen && !Open && OnClose) {
        Manager->GetEventQueue().Call(OnClose, lua::ImguiHandle(Handle));
    }

    BeginDrawingWindow();

    return renderChildren;
}

void Window::EndRender(DrawingContext& context)
{
    if (rendering_) {
        EndDrawingWindow();
        ImGui::End();
        context.PopScaling();
        rendering_ = false;
    }
}

lua::ImguiHandle Window::AddMainMenu()
{
    if (!MainMenu) {
        MainMenu = AddChild<MenuBar>();
        Flags |= GuiWindowFlags::MenuBar;
    }

    return MainMenu;
}

bool MenuBar::BeginRender(DrawingContext& context)
{
    rendering_ = ImGui::BeginMenuBar();
    return rendering_;
}

void MenuBar::EndRender(DrawingContext& context)
{
    if (rendering_) {
        ImGui::EndMenuBar();
        rendering_ = false;
    }
}

bool Menu::BeginRender(DrawingContext& context)
{
    rendering_ = ImGui::BeginMenu(Label.c_str());
    return rendering_;
}

void Menu::EndRender(DrawingContext& context)
{
    if (rendering_) {
        ImGui::EndMenu();
        rendering_ = false;
    }
}

lua::ImguiHandle Menu::AddItem(char const* label, std::optional<char const*> shortcut)
{
    auto menu = AddChild<MenuItem>();
    menu->Label = label;
    if (shortcut) menu->Shortcut = shortcut;
    return menu;
}

void MenuItem::StyledRender(DrawingContext& context)
{
    if (ImGui::MenuItem(Label.c_str(), Shortcut ? Shortcut->c_str() : nullptr, false, Enabled) && OnClick) {
        Manager->GetEventQueue().Call(OnClick, lua::ImguiHandle(Handle));
    }
}

bool MenuItem::OverridesClickEvent() const
{
    return true;
}

bool Group::BeginRender(DrawingContext& context)
{
    ImGui::BeginGroup();
    return true;
}


void Group::EndRender(DrawingContext& context)
{
    ImGui::EndGroup();
}


bool CollapsingHeader::BeginRender(DrawingContext& context)
{
    return ImGui::CollapsingHeader(Label.c_str(), (ImGuiTreeNodeFlags)Flags);
}

void CollapsingHeader::EndRender(DrawingContext& context)
{}


bool TabBar::BeginRender(DrawingContext& context)
{
    rendering_ = ImGui::BeginTabBar(Label.c_str(), (ImGuiTabBarFlags)Flags);
    return rendering_;
}

void TabBar::EndRender(DrawingContext& context)
{
    if (rendering_) ImGui::EndTabBar();
    rendering_ = false;
}

lua::ImguiHandle TabBar::AddTabItem(char const* label)
{
    auto btn = AddChild<TabItem>();
    btn->Label = label;
    return btn;
}


bool TabItem::BeginRender(DrawingContext& context)
{
    rendering_ = ImGui::BeginTabItem(Label.c_str(), nullptr, (ImGuiTabItemFlags)Flags);
    return rendering_;
}

void TabItem::EndRender(DrawingContext& context)
{
    if (rendering_) ImGui::EndTabItem();
    rendering_ = false;
}


bool Tree::BeginRender(DrawingContext& context)
{
    if (open_) {
        ImGui::SetNextItemOpen(open_->Open, open_->Cond);
        open_ = {};
    }

    rendering_ = ImGui::TreeNodeEx(Label.c_str(), (ImGuiTreeNodeFlags)Flags);

    if (ImGui::IsItemToggledOpen()) {
        if (rendering_ && OnExpand) {
            Manager->GetEventQueue().Call(OnExpand, lua::ImguiHandle(Handle));
        } else if (!rendering_ && OnCollapse) {
            Manager->GetEventQueue().Call(OnCollapse, lua::ImguiHandle(Handle));
        }
    }

    return rendering_;
}

void Tree::EndRender(DrawingContext& context)
{
    if (rendering_ && !(Flags & GuiTreeNodeFlags::NoTreePushOnOpen)) {
        ImGui::TreePop();
    }
    rendering_ = false;
}

void Tree::SetOpen(bool open, std::optional<GuiCond> cond)
{
    open_ = SetOpenRequest{
        .Open = open,
        .Cond = cond ? (ImGuiCond)*cond : ImGuiCond_Once
    };
}

void Table::UpdateSorting()
{
    auto sorting = ImGui::TableGetSortSpecs();

    if (!needsSortingUpdate_ && !(sorting && sorting->SpecsDirty)) {
        return;
    }

    Sorting.clear();
    if (sorting) {
        for (int i = 0; i < sorting->SpecsCount; i++) {
            auto const& spec = sorting->Specs[i];
            Sorting.push_back(SortSpec{
                .ColumnIndex = spec.ColumnIndex,
                .Direction = (GuiSortDirection)spec.SortDirection
            });
        }
    }

    if (sorting && sorting->SpecsDirty) {
        sorting->SpecsDirty = false;
        if (OnSortChanged) {
            Manager->GetEventQueue().Call(OnSortChanged, lua::ImguiHandle(Handle));
        }
    }

    needsSortingUpdate_ = false;
}

void Table::StyledRender(DrawingContext& context)
{
    if (OptimizedDraw) {
        if (BeginRender(context)) {
            ImGuiListClipper clipper;
            clipper.Begin(Children.size());
            while (clipper.Step()) {
                for (auto row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
                    auto child = Manager->GetRenderable(Children[row]);
                    if (child) {
                        child->Render(context);
                    }
                }
            }
        }

        EndRender(context);
    } else {
        TreeParent::StyledRender(context);
    }
}

bool Table::BeginRender(DrawingContext& context)
{
    // Handle possible discrepancy between Columns and # of items in ColumnDefs (use whichever is higher).
    // IMGUI will crash if we try to add more columns than the col count in BeginTable()
    auto cols = std::max(Columns, ColumnDefs.size());

    rendering_ = ImGui::BeginTable(Label.c_str(), (int)cols, (ImGuiTableFlags)Flags, context.Scale(ToImVec(Size)));

    if (rendering_) {
        for (auto const& def : ColumnDefs) {
            ImGui::TableSetupColumn(def.Name.c_str(), (ImGuiTableColumnFlags)def.Flags, context.Scale(def.Width));
        }

        if (FreezeRows || FreezeCols) ImGui::TableSetupScrollFreeze((int)FreezeCols, (int)FreezeRows);
        if (ShowHeader && AngledHeader) ImGui::TableAngledHeadersRow();
        if (ShowHeader) ImGui::TableHeadersRow();
        UpdateSorting();
    }

    return rendering_;
}

void Table::EndRender(DrawingContext& context)
{
    if (rendering_) ImGui::EndTable();
    rendering_ = false;
}

void Table::AddColumn(char const* name, std::optional<GuiTableColumnFlags> flags, std::optional<float> width)
{
    ColumnDefs.push_back(ColumnDefinition{
        name, flags ? *flags : GuiTableColumnFlags(0), width ? *width : 0.0f
    });
}

lua::ImguiHandle Table::AddRow()
{
    return AddChild<TableRow>();
}


bool TableRow::BeginRender(DrawingContext& context)
{
    ImGui::TableNextRow((ImGuiTableRowFlags)Flags, context.Scale(MinHeight));
    return true;
}

void TableRow::EndRender(DrawingContext& context)
{
    if (GImGui->CurrentTable && GImGui->CurrentTable->IsInsideRow) {
        ImGui::TableEndRow(GImGui->CurrentTable);
    }
}

lua::ImguiHandle TableRow::AddCell()
{
    return AddChild<TableCell>();
}


bool TableCell::BeginRender(DrawingContext& context)
{
    ImGui::TableNextColumn();
    return true;
}

void TableCell::EndRender(DrawingContext& context)
{
}


bool Tooltip::BeginRender(DrawingContext& context)
{
    // BeginItemTooltip() doesn't consume next window settings so we need to set them conditionally
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip)) {
        ProcessRenderSettings(context);
        rendering_ = ImGui::BeginTooltipEx(ImGuiTooltipFlags_None, (ImGuiWindowFlags)Flags);
    } else {
        rendering_ = false;
    }

    if (rendering_) {
        BeginDrawingWindow();
    }
    
    return rendering_;
}


void Tooltip::EndRender(DrawingContext& context)
{
    if (rendering_) {
        EndDrawingWindow();
        ImGui::EndTooltip();
    }
    rendering_ = false;
}


bool Popup::BeginRender(DrawingContext& context)
{
    if (requestOpen_) {
        ImGui::OpenPopup(Label.c_str(), (ImGuiPopupFlags)openFlags_);
        requestOpen_ = false;
    }

    ProcessRenderSettings(context);
    rendering_ = ImGui::BeginPopup(Label.c_str(), (ImGuiWindowFlags)Flags);

    if (rendering_) {
        BeginDrawingWindow();
    }

    return rendering_;
}


void Popup::EndRender(DrawingContext& context)
{
    if (rendering_) {
        EndDrawingWindow();
        ImGui::EndPopup();
    }
    rendering_ = false;
}


void Popup::Open(std::optional<GuiPopupFlags> flags)
{
    requestOpen_ = true;
    openFlags_ = flags.value_or((GuiPopupFlags)0);
}


bool ChildWindow::BeginRender(DrawingContext& context)
{
    ImGuiID id = ImGui::GetCurrentWindow()->GetID(Label.c_str());
    ProcessRenderSettings(context);
    auto rendering = ImGui::BeginChildEx(Label.c_str(), id, context.Scale(ToImVec(Size)), (ImGuiWindowFlags)Flags, (ImGuiChildFlags)ChildFlags);

    if (rendering) {
        BeginDrawingWindow();
    }

    return rendering;
}


void ChildWindow::EndRender(DrawingContext& context)
{
    EndDrawingWindow();
    ImGui::EndChild();
}


void Image::StyledRender(DrawingContext& context)
{
    auto texture = ImageData.PrepareRender();
    if (texture) {
        ImGui::ImageWithBg(
            texture,
            context.Scale(ImageData.Size),
            ToImVec(ImageData.UV0),
            ToImVec(ImageData.UV1),
            ImVec4(0, 0, 0, 0),
            ToImVec(Tint)
        );
    }
}


void Text::StyledRender(DrawingContext& context)
{
    ImGui::TextUnformatted(Label.c_str(), Label.c_str() + Label.size());
}


void TextLink::StyledRender(DrawingContext& context)
{
    if (ImGui::TextLink(Label.c_str())) {
        if (OnClick) {
            Manager->GetEventQueue().Call(OnClick, lua::ImguiHandle(Handle));
        }
    }
}

bool TextLink::OverridesClickEvent() const
{
    return true;
}


void BulletText::StyledRender(DrawingContext& context)
{
    ImGui::BulletText("%s", Label.c_str());
}


void SeparatorText::StyledRender(DrawingContext& context)
{
    if (Label.empty()) {
        ImGui::Separator();
    } else {
        ImGui::SeparatorText(Label.c_str());
    }
}


void Spacing::StyledRender(DrawingContext& context)
{
    ImGui::Spacing();
}


void Dummy::StyledRender(DrawingContext& context)
{
    ImGui::Dummy(context.Scale(ImVec2(Width, Height)));
}


void NewLine::StyledRender(DrawingContext& context)
{
    ImGui::NewLine();
}


void Separator::StyledRender(DrawingContext& context)
{
    ImGui::Separator();
}


void Selectable::StyledRender(DrawingContext& context)
{
    if (ImGui::Selectable(Label.c_str(), &Selected, (ImGuiSelectableFlags)Flags, context.Scale(ToImVec(Size)))) {
        if (OnClick) {
            Manager->GetEventQueue().Call(OnClick, lua::ImguiHandle(Handle));
        }
    }
}

bool Selectable::OverridesClickEvent() const
{
    return true;
}


void Button::StyledRender(DrawingContext& context)
{
    if (ImGui::ButtonEx(Label.c_str(), context.Scale(ToImVec(Size)), (ImGuiButtonFlags)Flags)) {
        if (OnClick) {
            Manager->GetEventQueue().Call(OnClick, lua::ImguiHandle(Handle));
        }
    }
}

bool Button::OverridesClickEvent() const
{
    return true;
}


void ImageButton::StyledRender(DrawingContext& context)
{
    auto texture = Image.PrepareRender();
    if (!texture) return;

    auto id = ImGui::GetCurrentWindow()->GetID(Label.c_str());
    if (ImGui::ImageButtonEx(id, texture, context.Scale(ToImVec(Image.Size)), ToImVec(Image.UV0), ToImVec(Image.UV1), ToImVec(Background), ToImVec(Tint), (ImGuiButtonFlags)Flags)) {
        if (OnClick) {
            Manager->GetEventQueue().Call(OnClick, lua::ImguiHandle(Handle));
        }
    }
}

bool ImageButton::OverridesClickEvent() const
{
    return true;
}


void Checkbox::StyledRender(DrawingContext& context)
{
    if (ImGui::Checkbox(Label.c_str(), &Checked)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Checked);
        }
    }
}


void RadioButton::StyledRender(DrawingContext& context)
{
    if (ImGui::RadioButton(Label.c_str(), Active)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Active);
        }
    }
}


InputText::InputText()
{
    Text.reserve(GrowSize);
}


void InputText::StyledRender(DrawingContext& context)
{
    if (reloadText_) {
        auto id = ImGui::GetCurrentWindow()->GetID(Label.c_str());
        auto state = ImGui::GetInputTextState(id);
        if (state != nullptr) {
            state->ReloadUserBufAndKeepSelection();
        }
        reloadText_ = false;
    }

    if (ImGui::InputTextEx(Label.c_str(), Hint ? Hint->c_str() : nullptr, Text.data(), Text.capacity(), context.Scale(ToImVec(SizeHint)), (ImGuiInputTextFlags)Flags, nullptr, nullptr)) {
        Text.resize((uint32_t)strlen(Text.data()));

        if (Text.size() + GrowSize - 64 > Text.capacity() && Text.capacity() < MaxSize) {
            Text.reserve(std::min(MaxSize, Text.size() + GrowSize));
            reloadText_ = true;
        }

        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Text);
        }
    }
}

STDString InputText::GetText() const
{
    return Text;
}

void InputText::SetText(STDString text)
{
    Text = text;
    Text.reserve(text.size() + GrowSize);
    reloadText_ = true;
}


void Combo::StyledRender(DrawingContext& context)
{
    char const* preview = nullptr;
    if (SelectedIndex >= 0 && SelectedIndex < (int)Options.size()) {
        preview = Options[SelectedIndex].c_str();
    }

    if (ImGui::BeginCombo(Label.c_str(), preview, (ImGuiComboFlags)Flags)) {
        int selectedIndex = -1;

        int i = 0;
        for (auto const& val : Options) {
            bool selected = (SelectedIndex == i);
            if (ImGui::Selectable(val.c_str(), &selected)) {
                selectedIndex = i;
            }

            if (selected) {
                ImGui::SetItemDefaultFocus();
            }

            i++;
        }

        if (SelectedIndex != selectedIndex && selectedIndex != -1) {
            SelectedIndex = selectedIndex;
            if (OnChange) {
                Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), SelectedIndex);
            }
        }

        ImGui::EndCombo();
    }
}


void DragScalar::StyledRender(DrawingContext& context)
{
    if (ImGui::DragScalarN(Label.c_str(), ImGuiDataType_Float, &Value, Components, 1.0f, &Min, &Max, nullptr, (ImGuiSliderFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
        }
    }
}


void DragInt::StyledRender(DrawingContext& context)
{
    if (ImGui::DragScalarN(Label.c_str(), ImGuiDataType_S32, &Value, Components, 1.0f, &Min, &Max, nullptr, (ImGuiSliderFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
        }
    }
}


void SliderScalar::StyledRender(DrawingContext& context)
{
    if (Vertical) {
        if (ImGui::VSliderScalar(Label.c_str(), ToImVec(VerticalSize), ImGuiDataType_Float, &Value, &Min, &Max, nullptr, (ImGuiSliderFlags)Flags)) {
            if (OnChange) {
                Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
            }
        }
    } else {
        if (ImGui::SliderScalarN(Label.c_str(), ImGuiDataType_Float, &Value, Components, &Min, &Max, nullptr, (ImGuiSliderFlags)Flags)) {
            if (OnChange) {
                Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
            }
        }
    }
}


void SliderInt::StyledRender(DrawingContext& context)
{
    if (Vertical) {
        if (ImGui::VSliderInt(Label.c_str(), ToImVec(VerticalSize), &Value.x, Min.x, Max.x, nullptr, (ImGuiSliderFlags)Flags)) {
            if (OnChange) {
                Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
            }
        }
    } else {
        if (ImGui::SliderScalarN(Label.c_str(), ImGuiDataType_S32, &Value, Components, &Min, &Max, nullptr, (ImGuiSliderFlags)Flags)) {
            if (OnChange) {
                Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
            }
        }
    }
}


void InputScalar::StyledRender(DrawingContext& context)
{
    if (ImGui::InputScalarN(Label.c_str(), ImGuiDataType_Float, &Value, Components, nullptr, nullptr, nullptr, (ImGuiInputTextFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
        }
    }
}


void InputInt::StyledRender(DrawingContext& context)
{
    if (ImGui::InputScalarN(Label.c_str(), ImGuiDataType_S32, &Value, Components, nullptr, nullptr, nullptr, (ImGuiInputTextFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
        }
    }
}


void ColorEdit::StyledRender(DrawingContext& context)
{
    if (ImGui::ColorEdit4(Label.c_str(), &Color.x, (ImGuiColorEditFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Color);
        }
    }
}


void ColorPicker::StyledRender(DrawingContext& context)
{
    if (ImGui::ColorPicker4(Label.c_str(), &Color.x, (ImGuiColorEditFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Color);
        }
    }
}


void ProgressBar::StyledRender(DrawingContext& context)
{
    ImGui::ProgressBar(Value, context.Scale(ToImVec(Size)), Overlay.empty() ? nullptr : Overlay.c_str());
}


IMGUIObjectPoolInterface::~IMGUIObjectPoolInterface() {}

IMGUIObjectManager::IMGUIObjectManager()
{
    pools_[(unsigned)IMGUIObjectType::Window] = std::make_unique<IMGUIObjectPool<Window>>();
    pools_[(unsigned)IMGUIObjectType::ChildWindow] = std::make_unique<IMGUIObjectPool<ChildWindow>>();
    pools_[(unsigned)IMGUIObjectType::Group] = std::make_unique<IMGUIObjectPool<Group>>();
    pools_[(unsigned)IMGUIObjectType::CollapsingHeader] = std::make_unique<IMGUIObjectPool<CollapsingHeader>>();
    pools_[(unsigned)IMGUIObjectType::TabBar] = std::make_unique<IMGUIObjectPool<TabBar>>();
    pools_[(unsigned)IMGUIObjectType::TabItem] = std::make_unique<IMGUIObjectPool<TabItem>>();
    pools_[(unsigned)IMGUIObjectType::Tree] = std::make_unique<IMGUIObjectPool<Tree>>();
    pools_[(unsigned)IMGUIObjectType::Table] = std::make_unique<IMGUIObjectPool<Table>>();
    pools_[(unsigned)IMGUIObjectType::TableRow] = std::make_unique<IMGUIObjectPool<TableRow>>();
    pools_[(unsigned)IMGUIObjectType::TableCell] = std::make_unique<IMGUIObjectPool<TableCell>>();
    pools_[(unsigned)IMGUIObjectType::Tooltip] = std::make_unique<IMGUIObjectPool<Tooltip>>();
    pools_[(unsigned)IMGUIObjectType::Popup] = std::make_unique<IMGUIObjectPool<Popup>>();

    pools_[(unsigned)IMGUIObjectType::MenuBar] = std::make_unique<IMGUIObjectPool<MenuBar>>();
    pools_[(unsigned)IMGUIObjectType::Menu] = std::make_unique<IMGUIObjectPool<Menu>>();
    pools_[(unsigned)IMGUIObjectType::MenuItem] = std::make_unique<IMGUIObjectPool<MenuItem>>();

    pools_[(unsigned)IMGUIObjectType::Image] = std::make_unique<IMGUIObjectPool<Image>>();

    pools_[(unsigned)IMGUIObjectType::Text] = std::make_unique<IMGUIObjectPool<Text>>();
    pools_[(unsigned)IMGUIObjectType::TextLink] = std::make_unique<IMGUIObjectPool<TextLink>>();
    pools_[(unsigned)IMGUIObjectType::BulletText] = std::make_unique<IMGUIObjectPool<BulletText>>();
    pools_[(unsigned)IMGUIObjectType::SeparatorText] = std::make_unique<IMGUIObjectPool<SeparatorText>>();

    pools_[(unsigned)IMGUIObjectType::Spacing] = std::make_unique<IMGUIObjectPool<Spacing>>();
    pools_[(unsigned)IMGUIObjectType::Dummy] = std::make_unique<IMGUIObjectPool<Dummy>>();
    pools_[(unsigned)IMGUIObjectType::NewLine] = std::make_unique<IMGUIObjectPool<NewLine>>();
    pools_[(unsigned)IMGUIObjectType::Separator] = std::make_unique<IMGUIObjectPool<Separator>>();

    pools_[(unsigned)IMGUIObjectType::Selectable] = std::make_unique<IMGUIObjectPool<Selectable>>();
    pools_[(unsigned)IMGUIObjectType::Button] = std::make_unique<IMGUIObjectPool<Button>>();
    pools_[(unsigned)IMGUIObjectType::ImageButton] = std::make_unique<IMGUIObjectPool<ImageButton>>();
    pools_[(unsigned)IMGUIObjectType::Checkbox] = std::make_unique<IMGUIObjectPool<Checkbox>>();
    pools_[(unsigned)IMGUIObjectType::RadioButton] = std::make_unique<IMGUIObjectPool<RadioButton>>();
    pools_[(unsigned)IMGUIObjectType::InputText] = std::make_unique<IMGUIObjectPool<InputText>>();
    pools_[(unsigned)IMGUIObjectType::Combo] = std::make_unique<IMGUIObjectPool<Combo>>();

    pools_[(unsigned)IMGUIObjectType::DragScalar] = std::make_unique<IMGUIObjectPool<DragScalar>>();
    pools_[(unsigned)IMGUIObjectType::DragInt] = std::make_unique<IMGUIObjectPool<DragInt>>();
    pools_[(unsigned)IMGUIObjectType::SliderScalar] = std::make_unique<IMGUIObjectPool<SliderScalar>>();
    pools_[(unsigned)IMGUIObjectType::SliderInt] = std::make_unique<IMGUIObjectPool<SliderInt>>();
    pools_[(unsigned)IMGUIObjectType::InputScalar] = std::make_unique<IMGUIObjectPool<InputScalar>>();
    pools_[(unsigned)IMGUIObjectType::InputInt] = std::make_unique<IMGUIObjectPool<InputInt>>();

    pools_[(unsigned)IMGUIObjectType::ColorEdit] = std::make_unique<IMGUIObjectPool<ColorEdit>>();
    pools_[(unsigned)IMGUIObjectType::ColorPicker] = std::make_unique<IMGUIObjectPool<ColorPicker>>();

    pools_[(unsigned)IMGUIObjectType::ProgressBar] = std::make_unique<IMGUIObjectPool<ProgressBar>>();
}

IMGUIObjectManager::~IMGUIObjectManager()
{
    Clear();
}

Renderable* IMGUIObjectManager::CreateRenderable(IMGUIObjectType type)
{
    auto obj = pools_[(unsigned)type]->Create();
    obj->Manager = this;

    if (type == IMGUIObjectType::Window) {
        windows_.push_back(obj->Handle);
    }

    return obj;
}

Renderable* IMGUIObjectManager::GetRenderable(HandleType handle)
{
    auto type = (handle >> 56);
    if (type > (unsigned)IMGUIObjectType::Max) return nullptr;
    return pools_[type]->Get(handle);
}

bool IMGUIObjectManager::DestroyRenderable(HandleType handle)
{
    auto type = (handle >> 56);
    if (type > (unsigned)IMGUIObjectType::Max) return false;
    auto destroyed = pools_[type]->Destroy(handle);
    if (destroyed) {
        if (type == (unsigned)IMGUIObjectType::Window) {
            auto it = windows_.find(handle);
            if (it != windows_.end()) {
                windows_.erase(it);
            }
        }
    }
    return destroyed;
}

void IMGUIObjectManager::Render(DrawingContext& context)
{
    if (renderDemo_) {
        ImGui::ShowDemoWindow();
    }

    for (auto windowHandle : windows_) {
        auto window = GetRenderable(windowHandle);
        if (window) {
            window->Render(context);
        }
    }
}

void IMGUIObjectManager::EnableDemo(bool enable)
{
    renderDemo_ = enable;
}

void IMGUIObjectManager::ClientUpdate()
{
    LuaVirtualPin lua(ecl::ExtensionState::Get());
    if (lua) {
        eventQueue_.Flush();
    }
}

void IMGUIObjectManager::Clear()
{
    for (auto window : windows_) {
        DestroyRenderable(window);
    }
    
    for (auto& pool : pools_) {
        pool->Clear();
    }
}

IMGUIManager::IMGUIManager(SDLManager& sdl)
    : sdl_(sdl)
{
    if (GetModuleHandleW(L"bg3_dx11.exe") == NULL) {
        renderer_ = std::make_unique<VulkanBackend>(*this);
    } else {
        renderer_ = std::make_unique<DX11Backend>(*this);
    }

    textureLoader_.BindRenderer(renderer_.get());
}

IMGUIManager::~IMGUIManager()
{
    DestroyUI();
}

void IMGUIManager::EnableHooks()
{
    renderer_->EnableHooks();
}

void IMGUIManager::DisableHooks()
{
    renderer_->DisableHooks();
}

void IMGUIManager::InitializeUI()
{
    if (initialized_) return;

    IMGUI_DEBUG("IMGUIManager::InitializeUI()");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
#if !defined(NDEBUG)
    io.ConfigDebugIsDebuggerPresent = IsDebuggerPresent();
#endif
    io.ConfigDebugHighlightIdConflicts = gExtender->GetConfig().DeveloperMode;
    io.ConfigDebugIniSettings = gExtender->GetConfig().DeveloperMode;

    auto configPath = GetStaticSymbols().ToPath("imgui.ini", PathRootType::UserProfile);
    if (!configPath.empty()) {
        io.IniFilename = _strdup(configPath.c_str());
    }

    sdl_.InitializeUI();
    renderer_->InitializeUI();

    UpdateStyle();

    auto const& language = GetStaticSymbols().GetGlobalSwitches()->Language;
    // Disable very small font sizes for fonts with small glyphs
    reducedFontAtlas_ =
        language == "Japanese"
        || language == "Chinese"
        || language == "ChineseTraditional";

    if (reducedFontAtlas_) {
        LoadFont(GFS.strMedium, "", 32.0f);
        LoadFont(GFS.strDefault, "", 36.0f);
        LoadFont(GFS.strLarge, "", 40.0f);
    } else {
        LoadFont(GFS.strTiny, "", 24.0f);
        LoadFont(GFS.strSmall, "", 28.0f);
        LoadFont(GFS.strMedium, "", 32.0f);
        LoadFont(GFS.strDefault, "", 36.0f);
        LoadFont(GFS.strLarge, "", 40.0f);
        LoadFont(GFS.strBig, "", 44.0f);
    }

    initialized_ = true;
}

void IMGUIManager::DestroyUI()
{
    if (!initialized_) return;

    IMGUI_DEBUG("IMGUIManager::DestroyUI()");

    initialized_ = false;
    renderer_->DestroyUI();
    sdl_.DestroyUI();
    GImGui->Viewports.clear();
    ImGui::DestroyContext();
}

void IMGUIManager::EnableUI(bool enabled)
{
    enableUI_ = enabled;
    IMGUI_DEBUG("IMGUIManager::EnableUI: %d", enabled ? 1 : 0);
}

void IMGUIManager::SetObjects(IMGUIObjectManager* objects)
{
    objects_ = objects;

    if (objects_ == nullptr) {
        renderer_->ClearFrame();
    }

    IMGUI_DEBUG("IMGUIManager::SetObjects() - Binding object manager");
}

bool IMGUIManager::LoadFont(FixedString const& name, char const* path, float size)
{
    if (fonts_.find(name) != fonts_.end()) {
        return true;
    }

    FontData font{
        .Key = name,
        .Path = path,
        .SizePixels = size
    };
    if (LoadFont(font)) {
        fonts_.set(name, font);
        return true;
    } else {
        return false;
    }
}

const ImWchar* GetGlyphRangesTurkish()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x011E, 0x011F, // Turkish characters
        0x0130, 0x0131,
        0x015E, 0x015F,
        0x2010, 0x2033,
        0,
    };
    return &ranges[0];
}

bool IMGUIManager::LoadFont(FontData& request)
{
    auto path = request.Path;
    auto glyphRanges = ImGui::GetIO().Fonts->GetGlyphRangesDefault();
    if (path.empty()) {
        auto const& language = GetStaticSymbols().GetGlobalSwitches()->Language;
        if (language == "Korean") {
            path = "Public/Game/GUI/Assets/Fonts/NotoSerifKR-Regular.otf";
            glyphRanges = ImGui::GetIO().Fonts->GetGlyphRangesKorean();
        } else if (language == "Japanese") {
            path = "Public/Game/GUI/Assets/Fonts/NotoSerifSC-Regular.otf";
            glyphRanges = ImGui::GetIO().Fonts->GetGlyphRangesJapanese();
        } else if (language == "Chinese" || language == "ChineseTraditional") {
            path = "Public/Game/GUI/Assets/Fonts/NotoSerifSC-Regular.otf";
            glyphRanges = ImGui::GetIO().Fonts->GetGlyphRangesChineseFull();
            // IMGUI creates a texture with small width so height will be 32768 for CN fonts.
            // (DX11 can't create textures larger than 16384).
            // Compensate by using a larger width than default
            ImGui::GetIO().Fonts->TexDesiredWidth = 16384;
        } else {
            if (language == "Russian" || language == "Ukrainian") {
                glyphRanges = ImGui::GetIO().Fonts->GetGlyphRangesCyrillic();
            } else if (language == "Turkish") {
                glyphRanges = GetGlyphRangesTurkish();
            }

            path = "Public/Game/GUI/Assets/Fonts/QuadraatOffcPro/QuadraatOffcPro.ttf";
        }
    }

    auto blob = script::LoadExternalFile(path, PathRootType::Data);
    if (!blob) {
        OsiError("Failed to open font file: " << path);
        return false;
    }

    auto& io = ImGui::GetIO();
    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;
    cfg.RasterizerDensity = requestedScale_ * requestedFontScaleMultiplier_;
    cfg.SizePixels = request.SizePixels;
    cfg.GlyphRanges = glyphRanges;

    request.Font = io.Fonts->AddFontFromMemoryTTF(blob->data(), blob->size(), 0.0f, &cfg);
    if (!request.Font) {
        OsiError("Failed to load font file (not a valid TTF font?): " << path);
        return false;
    } else {
        request.Font->Scale = requestedScale_ * requestedFontScaleMultiplier_;
    }

    renderer_->ReloadFonts();
    return true;
}

IMGUIManager::FontData* IMGUIManager::GetFont(FixedString const& name)
{
    if (reducedFontAtlas_) {
        if (name == GFS.strTiny || GFS.strSmall) {
            return fonts_.try_get(GFS.strMedium);
        }

        if (name == GFS.strLarge) {
            return fonts_.try_get(GFS.strBig);
        }
    }

    return fonts_.try_get(name);
}

void IMGUIManager::SetScale(float scale)
{
    requestedScale_ = scale;
}

void IMGUIManager::SetUIScaleMultiplier(float scale)
{
    requestedUiScaleMultiplier_ = scale;
}

void IMGUIManager::SetFontScaleMultiplier(float scale)
{
    requestedFontScaleMultiplier_ = scale;
}

glm::ivec2 IMGUIManager::GetViewportSize()
{
    return renderer_->GetViewportSize();
}

void IMGUIManager::OnRenderBackendInitialized()
{
    IMGUI_DEBUG("IMGUIManager::OnRenderBackendInitialized()");
    OnViewportUpdated();
    frameNo_ = 0;

    if (!initialized_) {
        InitializeUI();
    }

    IMGUI_DEBUG("READY STATE CHECK - enableUI %d, initialized %d, objects %d, rendererInitialized %d",
        enableUI_ ? 1 : 0, initialized_ ? 1 : 0, objects_ ? 1 : 0, renderer_->IsInitialized() ? 1 : 0);
}

void IMGUIManager::OnViewportUpdated()
{
    auto viewport = renderer_->GetViewportSize();

    requestedScale_ = (float)viewport.y / 1610.0f;
}

#define DEFAULT_COLOR(name, value) style.Colors[ImGuiCol_##name] = value;

void IMGUIManager::UpdateStyle()
{
    IMGUI_DEBUG("Recalculating IMGUI style");
    auto& style = ImGui::GetStyle();
    style = ImGuiStyle();

    style.WindowPadding = ImVec2(10.0f, 8.0f);
    style.WindowRounding = 4.0f;
    style.ChildRounding = 4.0f;
    style.PopupRounding = 2.0f;
    style.FrameRounding = 3.0f;
    style.ScrollbarSize = 20.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 16.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 6.0f;
    style.TabBarBorderSize = 1.0f;
    style.SeparatorTextBorderSize = 4.0f;
    style.SeparatorTextPadding = ImVec2(5.0f, 3.f);

    auto BoxColor = ImVec4(0.18f, 0.15f, 0.15f, 0.78f);
    auto BoxHoverColor = ImVec4(0.38f, 0.26f, 0.21f, 0.78f);
    auto BoxActiveColor = ImVec4(0.32f, 0.24f, 0.16f, 0.78f);

    DEFAULT_COLOR(Text, ImVec4(0.86f, 0.79f, 0.68f, 0.78f));
    DEFAULT_COLOR(TextDisabled, ImVec4(0.86f, 0.79f, 0.68f, 0.28f));
    DEFAULT_COLOR(WindowBg, ImVec4(0.07f, 0.07f, 0.07f, 0.90f));
    DEFAULT_COLOR(ChildBg, ImVec4(0.12f, 0.11f, 0.11f, 0.4f));
    DEFAULT_COLOR(PopupBg, ImVec4(0.18f, 0.15f, 0.15f, 0.90f));
    DEFAULT_COLOR(Border, ImVec4(0.24f, 0.15f, 0.08f, 0.00f));
    DEFAULT_COLOR(BorderShadow, ImVec4(0.07f, 0.07f, 0.07f, 0.78f));
    DEFAULT_COLOR(FrameBg, BoxColor);
    DEFAULT_COLOR(FrameBgHovered, BoxHoverColor);
    DEFAULT_COLOR(FrameBgActive, BoxActiveColor);
    DEFAULT_COLOR(TitleBg, ImVec4(0.07f, 0.07f, 0.07f, 1.00f));
    DEFAULT_COLOR(TitleBgActive, BoxActiveColor);
    DEFAULT_COLOR(TitleBgCollapsed, ImVec4(0.05f, 0.05f, 0.05f, 0.75f));
    DEFAULT_COLOR(MenuBarBg, ImVec4(0.07f, 0.07f, 0.07f, 0.47f));
    DEFAULT_COLOR(ScrollbarBg, BoxColor);
    DEFAULT_COLOR(ScrollbarGrab, ImVec4(0.36f, 0.30f, 0.27f, 0.76f));
    DEFAULT_COLOR(ScrollbarGrabHovered, ImVec4(0.47f, 0.35f, 0.28f, 0.86f));
    DEFAULT_COLOR(ScrollbarGrabActive, BoxActiveColor);
    DEFAULT_COLOR(CheckMark, ImVec4(0.86f, 0.79f, 0.68f, 0.78f));
    DEFAULT_COLOR(SliderGrab, ImVec4(0.95f, 0.82f, 0.60f, 0.14f));
    DEFAULT_COLOR(SliderGrabActive, ImVec4(0.52f, 0.52f, 0.25f, 0.3f));
    DEFAULT_COLOR(Button, ImVec4(0.46f, 0.40f, 0.29f, 0.5f));
    DEFAULT_COLOR(ButtonHovered, ImVec4(0.64f, 0.40f, 0.28f, 0.5f));
    DEFAULT_COLOR(ButtonActive, ImVec4(0.60f, 0.48f, 0.29f, 0.5f));
    DEFAULT_COLOR(Header, ImVec4(0.36f, 0.30f, 0.27f, 0.76f));
    DEFAULT_COLOR(HeaderHovered, ImVec4(0.41f, 0.28f, 0.22f, 0.86f));
    DEFAULT_COLOR(HeaderActive, BoxActiveColor);
    DEFAULT_COLOR(Separator, BoxColor);
    DEFAULT_COLOR(SeparatorHovered, BoxHoverColor);
    DEFAULT_COLOR(SeparatorActive, BoxActiveColor);
    DEFAULT_COLOR(ResizeGrip, ImVec4(0.95f, 0.82f, 0.60f, 0.15f));
    DEFAULT_COLOR(ResizeGripHovered, BoxHoverColor);
    DEFAULT_COLOR(ResizeGripActive, BoxActiveColor);
    DEFAULT_COLOR(Tab, BoxColor);
    DEFAULT_COLOR(TabHovered, BoxHoverColor);
    DEFAULT_COLOR(TabSelected, BoxActiveColor);
    DEFAULT_COLOR(TabDimmed, ImVec4(0.05f, 0.05f, 0.05f, 0.78f));
    DEFAULT_COLOR(TabDimmedSelected, ImVec4(0.05f, 0.05f, 0.05f, 0.78f));
    DEFAULT_COLOR(PlotLines, ImVec4(0.86f, 0.79f, 0.68f, 0.63f));
    DEFAULT_COLOR(PlotLinesHovered, ImVec4(0.41f, 0.28f, 0.22f, 1.00f));
    DEFAULT_COLOR(PlotHistogram, ImVec4(0.86f, 0.79f, 0.68f, 0.63f));
    DEFAULT_COLOR(PlotHistogramHovered, ImVec4(0.41f, 0.28f, 0.22f, 1.00f));
    DEFAULT_COLOR(TableHeaderBg, ImVec4(0.72f, 0.62f, 0.43f, 0.47f));
    DEFAULT_COLOR(TableBorderStrong, ImVec4(0.66f, 0.37f, 0.09f, 0.78f));
    DEFAULT_COLOR(TableBorderLight, ImVec4(0.56f, 0.46f, 0.26f, 0.78f));
    DEFAULT_COLOR(TableRowBg, ImVec4(0.63f, 0.69f, 0.34f, 0.43f));
    DEFAULT_COLOR(TableRowBgAlt, ImVec4(0.52f, 0.29f, 0.15f, 0.43f));
    DEFAULT_COLOR(TextSelectedBg, ImVec4(0.57f, 0.21f, 0.21f, 0.43f));
    DEFAULT_COLOR(DragDropTarget, ImVec4(0.6f, 0.45f, 0.3f, 1.0f));
    DEFAULT_COLOR(NavCursor, ImVec4(0.55f, 0.00f, 0.00f, 0.78f));
    DEFAULT_COLOR(NavWindowingHighlight, ImVec4(0.55f, 0.00f, 0.00f, 0.78f));
    DEFAULT_COLOR(NavWindowingDimBg, ImVec4(0.07f, 0.07f, 0.07f, 0.78f));
    DEFAULT_COLOR(ModalWindowDimBg, ImVec4(0.18f, 0.15f, 0.15f, 0.73f));

    bool fontReloadRequired = scale_ != requestedScale_
        || fontScaleMultiplier_ != requestedFontScaleMultiplier_;

    style.ScaleAllSizes(requestedScale_ * requestedUiScaleMultiplier_);
    scale_ = requestedScale_;
    uiScaleMultiplier_ = requestedUiScaleMultiplier_;
    fontScaleMultiplier_ = requestedFontScaleMultiplier_;

    if (fontReloadRequired) {
        ImGui::GetIO().Fonts->Clear();
        for (auto& font : fonts_) {
            font->Value().Font = nullptr;
            LoadFont(font->Value());
        }
        renderer_->ReloadFonts();
    }
}

void IMGUIManager::Update()
{
    if (!enableUI_
        || !initialized_
        || !objects_
        || !renderer_->IsInitialized()) {

        IMGUI_FRAME_DEBUG("Skip drawing - enableUI %d, initialized %d, objects %d, rendererInitialized %d",
            enableUI_ ? 1 : 0, initialized_ ? 1 : 0, objects_ ? 1 : 0, renderer_->IsInitialized() ? 1 : 0);
        frameNo_++;
        return;
    }

    if (scale_ != requestedScale_ 
        || uiScaleMultiplier_ != requestedUiScaleMultiplier_
        || fontScaleMultiplier_ != requestedFontScaleMultiplier_) {
        UpdateStyle();
    }

    {
        std::lock_guard _(sdl_.GetSDLMutex());
        sdl_.NewFrame();
    }

    auto defaultFont = GetFont(GFS.strMedium);
    if (defaultFont != nullptr && defaultFont->Font != nullptr) {
        ImGui::GetIO().FontDefault = defaultFont->Font;
    }

    if (frameNo_ == 0) {
        IMGUI_DEBUG("Draw initial frame");
    }

    frameNo_++;

    textureLoader_.Update();
    renderer_->NewFrame();
    ImGui::NewFrame();

    DrawingContext dc{
        .UIScale = scale_
    };
    objects_->Render(dc);
    se_assert(dc.ScalingStack.empty());

    ImGui::Render();
    renderer_->FinishFrame();

    objects_->ClientUpdate();
}

void IMGUITextureLoader::BindRenderer(RenderingBackend* renderer)
{
    renderer_ = renderer;
}

void IMGUITextureLoader::Update()
{
    Array<FixedString> unloaded;
    for (auto const& it : pendingUnloads_) {
        if (--it.Value().WaitForFrames == 0) {
            renderer_->UnregisterTexture(it.Value().Id);

            auto textureManager = (*GetStaticSymbols().ls__gGlobalResourceManager)->TextureManager;
            (*GetStaticSymbols().ls__TextureManager__UnloadTexture)(textureManager, it.Key());

            unloaded.push_back(it.Key());
        }
    }

    for (auto const& tex : unloaded) {
        pendingUnloads_.remove(tex);
    }
}

std::optional<TextureLoadResult> IMGUITextureLoader::IncTextureRef(FixedString const& textureGuid)
{
    if (!renderer_) {
        ERR("Loading texture with no rendering backend?");
        return {};
    }

    auto refs = refCounts_.try_get(textureGuid);
    if (refs) {
        refs->RefCount++;
        return refs->LoadResult;
    }

    auto tex = (resource::TextureResource*)GetStaticSymbols().GetCurrentResourceBank()->GetResource(ResourceBankType::Texture, textureGuid);
    if (!tex) {
        return {};
    }
    
    if (tex->Type != TextureType::T1D // shouldn't be here, but tex type is set incorrectly for some atlases
        && tex->Type != TextureType::T2D) {
        ERR("Cannot render texture '%s': expected 2D texture, got %d", textureGuid.GetString(), tex->Type);
        return {};
    }

    auto descriptor = (*GetStaticSymbols().ls__AppliedMaterial__LoadTexture)(nullptr, textureGuid);
    if (!descriptor) {
        return {};
    }

    auto loadResult = renderer_->RegisterTexture(descriptor);
    if (!loadResult) {
        auto textureManager = (*GetStaticSymbols().ls__gGlobalResourceManager)->TextureManager;
        (*GetStaticSymbols().ls__TextureManager__UnloadTexture)(textureManager, textureGuid);
        return {};
    }

    refCounts_.set(textureGuid, TextureRefCount{ descriptor, *loadResult, 1 });
    pendingUnloads_.remove(textureGuid);
    return *loadResult;
}


bool IMGUITextureLoader::DecTextureRef(TextureOpaqueHandle id, FixedString const& textureGuid)
{
    if (!renderer_) {
        ERR("Unloading texture with no rendering backend?");
        return false;
    }

    auto refs = refCounts_.try_get(textureGuid);
    if (!refs) {
        ERR("Trying to release reference to texture '%s' but it has no references?", textureGuid.GetString());
        return false;
    }
    
    if (--refs->RefCount == 0) {
        refCounts_.remove(textureGuid);
        pendingUnloads_.set(textureGuid, TextureUnloadRequest{ id, DeleteAfterFrames });
    }

    return true;
}

std::optional<TextureLoadResult> IMGUIManager::RegisterTexture(FixedString const& textureGuid)
{
    return textureLoader_.IncTextureRef(textureGuid);
}

void IMGUIManager::UnregisterTexture(TextureOpaqueHandle id, FixedString const& textureGuid)
{
    textureLoader_.DecTextureRef(id, textureGuid);
}

std::optional<ImTextureID> IMGUIManager::BindTexture(TextureOpaqueHandle opaqueHandle)
{
    return renderer_->BindTexture(opaqueHandle);
}

END_NS()
