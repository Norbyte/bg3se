#include <stdafx.h>

#if defined(ENABLE_IMGUI)

#include <Extender/Client/IMGUI/IMGUI.h>
#include <Extender/Client/IMGUI/Objects.h>
#include <Extender/Shared/ScriptHelpers.h>
#include <CoreLib/Wrappers.h>

// #define IMGUI_DEBUG(msg, ...) DEBUG("[IMGUI] " msg, __VA_ARGS__)
#define IMGUI_DEBUG(msg, ...)

#include <Extender/Client/IMGUI/Vulkan.inl>
#include <Extender/Client/IMGUI/DX11.inl>
#include <Lua/Shared/LuaMethodCallHelpers.h>

BEGIN_NS(extui)

inline ImVec2 ToImVec(glm::vec2 const& v)
{
    return ImVec2(v.x, v.y);
}

inline ImVec4 ToImVec(glm::vec4 const& v)
{
    return ImVec4(v.x, v.y, v.z, v.w);
}

inline ImVec2 ToImVec(std::optional<glm::vec2> const& v, ImVec2 defaultVal = ImVec2(0.0f, 0.0f))
{
    return v ? ImVec2(v->x, v->y) : defaultVal;
}

ImageReference::~ImageReference()
{
    if (TextureId) {
        gExtender->IMGUI().UnregisterTexture(TextureId, TextureResource);
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
        TextureId = result->Id;
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
    TextureId = texture->Id;
    return true;
}


template <class T>
T* TreeParent::AddChild()
{
    auto child = Manager->CreateRenderable<T>();
    child->Parent = Handle;
    Children.Add(child->Handle);
    return child;
}

RenderingBackend::~RenderingBackend() {}

Renderable::~Renderable() {}

void Renderable::Destroy()
{
    if (Parent) {
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

void StyledRenderable::Render()
{
    if (!Visible) return;

    ImFont* font{ nullptr };
    if (Font) {
        auto info = gExtender->IMGUI().GetFont(Font);
        if (info) font = info->Font;
    }

    if (font) ImGui::PushFont(font);

    for (auto const& var : StyleVars) {
        auto varInfo = ImGui::GetStyleVarInfo((ImGuiStyleVar)var.Key);
        if (varInfo->Count == 2) {
            ImGui::PushStyleVar((ImGuiStyleVar)var.Key, ToImVec(var.Value));
        } else {
            ImGui::PushStyleVar((ImGuiStyleVar)var.Key, var.Value.x);
        }
    }

    for (auto const& var : StyleColors) {
        ImGui::PushStyleColor((ImGuiCol)var.Key, ImVec4(var.Value.r, var.Value.g, var.Value.b, var.Value.a));
    }

    if (!IDContext.empty()) {
        ImGui::PushID(IDContext.data(), IDContext.data() + IDContext.size());
    } else if (Label.empty()) {
        ImGui::PushID((int32_t)Handle | (int32_t)(Handle >> 32));
    }

    if (SameLine) ImGui::SameLine();

    if (PositionOffset) {
        auto pos = ImGui::GetCursorPos();
        ImGui::SetCursorPos(ImVec2(pos.x + PositionOffset->x, pos.y + PositionOffset->y));
    } else if (AbsolutePosition) {
        ImGui::SetCursorPos(ToImVec(*AbsolutePosition));
    }

    if (ItemFlags != (GuiItemFlags)0) ImGui::PushItemFlag((ImGuiItemFlags)ItemFlags, true);
    if (TextWrapPos) ImGui::PushTextWrapPos(*TextWrapPos);
    if (ItemWidth) ImGui::SetNextItemWidth(*ItemWidth);

    if (RequestActivate) {
        ImGui::ActivateItemByID(ImGui::GetCurrentWindow()->GetID(Label.c_str()));
        RequestActivate = false;
    }

    StyledRender();

    auto status = (GuiItemStatusFlags)ImGui::GetItemStatusFlags();
    if (ImGui::IsItemFocused()) status |= GuiItemStatusFlags::Focused;
    if (ImGui::IsItemActive()) status |= GuiItemStatusFlags::Active;
    if (StatusFlags != status) StatusFlags = status;

    if (ItemFlags != (GuiItemFlags)0) ImGui::PopItemFlag();
    if (TextWrapPos) ImGui::PopTextWrapPos();

    if (!StyleColors.empty()) {
        ImGui::PopStyleColor(StyleColors.size());
    }

    if (!StyleVars.empty()) {
        ImGui::PopStyleVar(StyleVars.size());
    }

    if (font) ImGui::PopFont();

    if (tooltip_) {
        auto tooltip = Manager->GetRenderable(tooltip_.Handle);
        if (tooltip) tooltip->Render();
    }

    if (!IDContext.empty() || Label.empty()) {
        ImGui::PopID();
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
    for (auto child : Children) {
        Manager->DestroyRenderable(child);
    }
}


void TreeParent::StyledRender()
{
    if (BeginRender()) {
        for (auto childHandle : Children) {
            auto child = Manager->GetRenderable(childHandle);
            if (child) {
                child->Render();
            }
        }
    }

    EndRender();
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
    Children.Add(ele->Handle);
    return true;
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

bool Window::BeginRender()
{
    if (!Open) return false;

    ProcessRenderSettings();

    bool wasOpen = Open;
    bool renderChildren = ImGui::Begin(Label.c_str(), Closeable ? &Open : nullptr, (ImGuiWindowFlags)Flags);

    rendering_ = true;

    if (wasOpen && !Open && OnClose) {
        Manager->GetEventQueue().Call(OnClose, lua::ImguiHandle(Handle));
    }

    return renderChildren;
}

void Window::EndRender()
{
    if (rendering_) {
        ImGui::End();
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

void Window::SetPos(glm::vec2 pos, std::optional<GuiCond> cond, std::optional<glm::vec2> pivot)
{
    req_.Pos = WindowRenderRequests::SetPos { 
        ToImVec(pos),
        ToImVec(pivot),
        cond ? (ImGuiCond )*cond : ImGuiCond_Always
    };
}

void Window::SetSize(glm::vec2 size, std::optional<GuiCond> cond)
{
    req_.Size = WindowRenderRequests::SetSize{ ToImVec(size), cond ? (ImGuiCond)*cond : ImGuiCond_Always };
}

void Window::SetSizeConstraints(std::optional<glm::vec2> size_min, std::optional<glm::vec2> size_max)
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

void Window::SetContentSize(std::optional<glm::vec2> size)
{
    req_.ContentSize = size ? std::optional<ImVec2>(ToImVec(*size)) : std::optional<ImVec2>{};
}

void Window::SetCollapsed(bool collapsed, std::optional<GuiCond> cond)
{
    req_.Collapsed = WindowRenderRequests::SetCollapsed{ collapsed, cond ? (ImGuiCond)*cond : ImGuiCond_Always };
}

void Window::SetFocus()
{
    req_.Focus = true;
}

void Window::SetScroll(std::optional<glm::vec2> scroll)
{
    req_.Scroll = scroll ? std::optional<ImVec2>(ToImVec(*scroll)) : std::optional<ImVec2>{};
}

void Window::SetBgAlpha(std::optional<float> alpha)
{
    req_.BgAlpha = alpha;
}


void Window::ProcessRenderSettings()
{
    if (req_.Pos) {
        ImGui::SetNextWindowPos(req_.Pos->Pos, req_.Pos->Cond, req_.Pos->Pivot);
        req_.Pos = {};
    }

    if (req_.Size) {
        ImGui::SetNextWindowSize(req_.Size->Size, req_.Size->Cond);
        req_.Size = {};
    }

    if (req_.SizeConstraints) {
        ImGui::SetNextWindowSizeConstraints(req_.SizeConstraints->SizeMin, req_.SizeConstraints->SizeMax);
        req_.SizeConstraints = {};
    }

    if (req_.ContentSize) {
        ImGui::SetNextWindowContentSize(*req_.ContentSize);
        req_.ContentSize = {};
    }

    if (req_.Scroll) {
        ImGui::SetNextWindowScroll(*req_.Scroll);
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

bool MenuBar::BeginRender()
{
    rendering_ = ImGui::BeginMenuBar();
    return rendering_;
}

void MenuBar::EndRender()
{
    if (rendering_) {
        ImGui::EndMenuBar();
        rendering_ = false;
    }
}

bool Menu::BeginRender()
{
    rendering_ = ImGui::BeginMenu(Label.c_str());
    return rendering_;
}

void Menu::EndRender()
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

void MenuItem::StyledRender()
{
    if (ImGui::MenuItem(Label.c_str(), Shortcut ? Shortcut->c_str() : nullptr, false, Enabled) && OnClick) {
        Manager->GetEventQueue().Call(OnClick, lua::ImguiHandle(Handle));
    }
}

bool Group::BeginRender()
{
    ImGui::BeginGroup();
    return true;
}


void Group::EndRender()
{
    ImGui::EndGroup();
}


bool CollapsingHeader::BeginRender()
{
    return ImGui::CollapsingHeader(Label.c_str(), (ImGuiTreeNodeFlags)Flags);
}

void CollapsingHeader::EndRender()
{}


bool TabBar::BeginRender()
{
    rendering_ = ImGui::BeginTabBar(Label.c_str(), (ImGuiTabBarFlags)Flags);
    return rendering_;
}

void TabBar::EndRender()
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


bool TabItem::BeginRender()
{
    rendering_ = ImGui::BeginTabItem(Label.c_str(), nullptr, (ImGuiTabItemFlags)Flags);
    return rendering_;
}

void TabItem::EndRender()
{
    if (rendering_) ImGui::EndTabItem();
    rendering_ = false;
}


bool Tree::BeginRender()
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

    if (ImGui::IsItemClicked() && OnClick) {
        Manager->GetEventQueue().Call(OnClick, lua::ImguiHandle(Handle));
    }

    return rendering_;
}

void Tree::EndRender()
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

bool Table::BeginRender()
{
    rendering_ = ImGui::BeginTable(Label.c_str(), (int)Columns, (ImGuiTableFlags)Flags, ToImVec(Size));

    if (rendering_) {
        for (auto const& def : ColumnDefs) {
            ImGui::TableSetupColumn(def.Name.c_str(), (ImGuiTableColumnFlags)def.Flags, def.Width);
        }
    }

    if (ShowHeader && AngledHeader) ImGui::TableAngledHeadersRow();
    if (ShowHeader) ImGui::TableHeadersRow();

    return rendering_;
}

void Table::EndRender()
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


bool TableRow::BeginRender()
{
    ImGui::TableNextRow((ImGuiTableRowFlags)Flags, MinHeight);
    return true;
}

void TableRow::EndRender()
{
}

lua::ImguiHandle TableRow::AddCell()
{
    return AddChild<TableCell>();
}


bool TableCell::BeginRender()
{
    ImGui::TableNextColumn();
    return true;
}

void TableCell::EndRender()
{
}


bool Tooltip::BeginRender()
{
    rendering_ = ImGui::BeginItemTooltip();
    return rendering_;
}


void Tooltip::EndRender()
{
    if (rendering_) ImGui::EndTooltip();
    rendering_ = false;
}


bool Popup::BeginRender()
{
    if (requestOpen_) {
        ImGui::OpenPopup(Label.c_str(), (ImGuiPopupFlags)openFlags_);
        requestOpen_ = false;
    }

    rendering_ = ImGui::BeginPopup(Label.c_str(), (ImGuiWindowFlags)Flags);
    return rendering_;
}


void Popup::EndRender()
{
    if (rendering_) ImGui::EndPopup();
    rendering_ = false;
}


void Popup::Open(std::optional<GuiPopupFlags> flags)
{
    requestOpen_ = true;
    openFlags_ = flags.value_or((GuiPopupFlags)0);
}


bool ChildWindow::BeginRender()
{
    ImGuiID id = ImGui::GetCurrentWindow()->GetID(Label.c_str());
    return ImGui::BeginChildEx(Label.c_str(), id, ToImVec(Size), (ImGuiWindowFlags)Flags, (ImGuiChildFlags)ChildFlags);
}


void ChildWindow::EndRender()
{
    ImGui::EndChild();
}


void Image::StyledRender()
{
    if (ImageData.IsValid()) {
        ImGui::Image(
            ImageData.TextureId,
            ToImVec(ImageData.Size),
            ToImVec(ImageData.UV0),
            ToImVec(ImageData.UV1),
            ToImVec(Tint),
            ToImVec(Border)
        );
    }
}


void Text::StyledRender()
{
    ImGui::TextUnformatted(Label.c_str(), Label.c_str() + Label.size());
}


void BulletText::StyledRender()
{
    ImGui::BulletText("%s", Label.c_str());
}


void SeparatorText::StyledRender()
{
    if (Label.empty()) {
        ImGui::Separator();
    } else {
        ImGui::SeparatorText(Label.c_str());
    }
}


void Spacing::StyledRender()
{
    ImGui::Spacing();
}


void Dummy::StyledRender()
{
    ImGui::Dummy(ImVec2(Width, Height));
}


void NewLine::StyledRender()
{
    ImGui::NewLine();
}


void Separator::StyledRender()
{
    ImGui::Separator();
}


void Button::StyledRender()
{
    if (ImGui::ButtonEx(Label.c_str(), ToImVec(Size), (ImGuiButtonFlags)Flags)) {
        if (OnClick) {
            Manager->GetEventQueue().Call(OnClick, lua::ImguiHandle(Handle));
        }
    }
}


void ImageButton::StyledRender()
{
    if (!Image.IsValid()) return;

    auto id = ImGui::GetCurrentWindow()->GetID(Label.c_str());
    if (ImGui::ImageButtonEx(id, Image.TextureId, ToImVec(Image.Size), ToImVec(Image.UV0), ToImVec(Image.UV1), ToImVec(Background), ToImVec(Tint), (ImGuiButtonFlags)Flags)) {
        if (OnClick) {
            Manager->GetEventQueue().Call(OnClick, lua::ImguiHandle(Handle));
        }
    }
}


void Checkbox::StyledRender()
{
    if (ImGui::Checkbox(Label.c_str(), &Checked)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Checked);
        }
    }
}


void RadioButton::StyledRender()
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


void InputText::StyledRender()
{
    if (reloadText_) {
        auto id = ImGui::GetCurrentWindow()->GetID(Label.c_str());
        auto state = ImGui::GetInputTextState(id);
        if (state != nullptr) {
            state->ReloadUserBufAndKeepSelection();
        }
        reloadText_ = false;
    }

    if (ImGui::InputTextEx(Label.c_str(), Hint ? Hint->c_str() : nullptr, Text.data(), Text.capacity(), ToImVec(SizeHint), (ImGuiInputTextFlags)Flags, nullptr, nullptr)) {
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


void Combo::StyledRender()
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


void DragScalar::StyledRender()
{
    if (ImGui::DragScalarN(Label.c_str(), ImGuiDataType_Float, &Value, Components, 1.0f, &Min, &Max, nullptr, (ImGuiSliderFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
        }
    }
}


void DragInt::StyledRender()
{
    if (ImGui::DragScalarN(Label.c_str(), ImGuiDataType_S32, &Value, Components, 1.0f, &Min, &Max, nullptr, (ImGuiSliderFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
        }
    }
}


void SliderScalar::StyledRender()
{
    if (ImGui::SliderScalarN(Label.c_str(), ImGuiDataType_Float, &Value, Components, &Min, &Max, nullptr, (ImGuiSliderFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
        }
    }
}


void SliderInt::StyledRender()
{
    if (ImGui::SliderScalarN(Label.c_str(), ImGuiDataType_S32, &Value, Components, &Min, &Max, nullptr, (ImGuiSliderFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
        }
    }
}


void InputScalar::StyledRender()
{
    if (ImGui::InputScalarN(Label.c_str(), ImGuiDataType_Float, &Value, Components, nullptr, nullptr, nullptr, (ImGuiInputTextFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
        }
    }
}


void InputInt::StyledRender()
{
    if (ImGui::InputScalarN(Label.c_str(), ImGuiDataType_S32, &Value, Components, nullptr, nullptr, nullptr, (ImGuiInputTextFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Value);
        }
    }
}


void ColorEdit::StyledRender()
{
    if (ImGui::ColorEdit4(Label.c_str(), &Color.x, (ImGuiColorEditFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Color);
        }
    }
}


void ColorPicker::StyledRender()
{
    if (ImGui::ColorPicker4(Label.c_str(), &Color.x, (ImGuiColorEditFlags)Flags)) {
        if (OnChange) {
            Manager->GetEventQueue().Call(OnChange, lua::ImguiHandle(Handle), Color);
        }
    }
}


void ProgressBar::StyledRender()
{
    ImGui::ProgressBar(Value, ToImVec(Size), Overlay.empty() ? nullptr : Overlay.c_str());
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
    pools_[(unsigned)IMGUIObjectType::BulletText] = std::make_unique<IMGUIObjectPool<BulletText>>();
    pools_[(unsigned)IMGUIObjectType::SeparatorText] = std::make_unique<IMGUIObjectPool<SeparatorText>>();

    pools_[(unsigned)IMGUIObjectType::Spacing] = std::make_unique<IMGUIObjectPool<Spacing>>();
    pools_[(unsigned)IMGUIObjectType::Dummy] = std::make_unique<IMGUIObjectPool<Dummy>>();
    pools_[(unsigned)IMGUIObjectType::NewLine] = std::make_unique<IMGUIObjectPool<NewLine>>();
    pools_[(unsigned)IMGUIObjectType::Separator] = std::make_unique<IMGUIObjectPool<Separator>>();

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
    return pools_[type]->Destroy(handle);
}

void IMGUIObjectManager::Render()
{
    if (renderDemo_) {
        ImGui::ShowDemoWindow();
    }

    for (auto windowHandle : windows_) {
        auto window = GetRenderable(windowHandle);
        if (window) {
            window->Render();
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

IMGUIManager::IMGUIManager(SDLManager& sdl)
    : sdl_(sdl)
{
    if (GetModuleHandleW(L"bg3_dx11.exe") == NULL) {
        renderer_ = std::make_unique<VulkanBackend>(*this);
    } else {
        renderer_ = std::make_unique<DX11Backend>(*this);
    }
}

IMGUIManager::~IMGUIManager()
{
    ImGui::GetIO().Fonts->ClearFonts();
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

    initialized_ = false;
    renderer_->DestroyUI();
    sdl_.DestroyUI();
    ImGui::DestroyContext();
}

void IMGUIManager::EnableUI(bool enabled)
{
    enableUI_ = enabled;
}

void IMGUIManager::SetObjects(IMGUIObjectManager* objects)
{
    objects_ = objects;

    if (objects_ == nullptr) {
        renderer_->ClearFrame();
    }
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
            ImGui::GetIO().Fonts->TexDesiredWidth = 8192;
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
    cfg.RasterizerDensity = requestedScale_;
    cfg.SizePixels = request.SizePixels;
    cfg.GlyphRanges = glyphRanges;

    request.Font = io.Fonts->AddFontFromMemoryTTF(blob->data(), blob->size(), 0.0f, &cfg);
    if (!request.Font) {
        OsiError("Failed to load font file (not a valid TTF font?): " << path);
        return false;
    } else {
        request.Font->Scale = requestedScale_;
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

glm::ivec2 IMGUIManager::GetViewportSize()
{
    return renderer_->GetViewportSize();
}

void IMGUIManager::OnRenderBackendInitialized()
{
    IMGUI_DEBUG("IMGUIManager::OnRenderBackendInitialized()");
    OnViewportUpdated();

    if (!initialized_) {
        InitializeUI();
    }
}

void IMGUIManager::OnViewportUpdated()
{
    auto viewport = renderer_->GetViewportSize();

    requestedScale_ = (float)viewport.y / 1610.0f;
}

#define DEFAULT_COLOR(name, value) style.Colors[ImGuiCol_##name] = value;

void IMGUIManager::UpdateStyle()
{
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
    DEFAULT_COLOR(TabActive, BoxActiveColor);
    DEFAULT_COLOR(TabUnfocused, ImVec4(0.05f, 0.05f, 0.05f, 0.78f));
    DEFAULT_COLOR(TabUnfocusedActive, ImVec4(0.05f, 0.05f, 0.05f, 0.78f));
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
    DEFAULT_COLOR(NavHighlight, ImVec4(0.55f, 0.00f, 0.00f, 0.78f));
    DEFAULT_COLOR(NavWindowingHighlight, ImVec4(0.55f, 0.00f, 0.00f, 0.78f));
    DEFAULT_COLOR(NavWindowingDimBg, ImVec4(0.07f, 0.07f, 0.07f, 0.78f));
    DEFAULT_COLOR(ModalWindowDimBg, ImVec4(0.18f, 0.15f, 0.15f, 0.73f));

    style.ScaleAllSizes(requestedScale_);
    scale_ = requestedScale_;

    ImGui::GetIO().Fonts->Clear();
    for (auto& font : fonts_) {
        font->Value().Font = nullptr;
        LoadFont(font->Value());
    }
    renderer_->ReloadFonts();
}

void IMGUIManager::Update()
{
    if (!enableUI_
        || !initialized_
        || !objects_
        || !renderer_->IsInitialized()) {
        return;
    }

    if (scale_ != requestedScale_) {
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

    renderer_->NewFrame();
    ImGui::NewFrame();

    objects_->Render();

    ImGui::Render();
    renderer_->FinishFrame();

    objects_->ClientUpdate();
}

std::optional<TextureLoadResult> IMGUIManager::RegisterTexture(FixedString const& textureGuid)
{
    auto descriptor = (*GetStaticSymbols().ls__AppliedMaterial__LoadTexture)(nullptr, textureGuid);
    if (!descriptor) return {};

    return renderer_->RegisterTexture(descriptor);
}

void IMGUIManager::UnregisterTexture(ImTextureID id, FixedString const& textureGuid)
{
    renderer_->UnregisterTexture(id);
    auto textureManager = (*GetStaticSymbols().ls__gGlobalResourceManager)->TextureManager;
    (*GetStaticSymbols().ls__TextureManager__UnloadTexture)(textureManager, textureGuid);
}

END_NS()

#endif
