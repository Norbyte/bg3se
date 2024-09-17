#pragma once

#include <Extender/Client/IMGUI/IMGUI.h>
#include <Extender/Client/IMGUI/Backends.h>
#include <Lua/LuaHelpers.h>
#include <imgui.h>

BEGIN_NS(extui)

class IMGUIObjectManager;

#define DECL_UI_TYPE(ty) static constexpr auto ObjectType = IMGUIObjectType::ty; \
    inline IMGUIObjectType GetType() override { return ObjectType; } \
    inline char const* GetTypeName() override { return #ty; } \
    inline lua::GenericPropertyMap& GetRTTI() override { return lua::GetStaticPropertyMap<ty>(); }


enum class IMGUIObjectType : uint8_t
{
    Window,
    ChildWindow,
    Group,
    CollapsingHeader,

    TabBar,
    TabItem,

    Tree,
    Table,
    TableRow,
    TableCell,

    Tooltip,
    Popup,

    MenuBar,
    Menu,
    MenuItem,

    Image,

    // Texts
    Text,
    BulletText,
    SeparatorText,

    // Layout helpers
    Spacing,
    Dummy,
    NewLine,
    Separator,

    // 
    Button,
    ImageButton,
    Checkbox,
    RadioButton,
    InputText,
    Combo,
    DragScalar,
    DragInt,
    SliderScalar,
    SliderInt,
    InputScalar,
    InputInt,
    ColorEdit,
    ColorPicker,
    ProgressBar,
    Max = ProgressBar
};


struct ImageReference : Noncopyable<ImageReference>
{
    ~ImageReference();
    bool Bind(FixedString const& iconOrTexture);
    bool BindTexture(FixedString const& textureUuid);
    bool BindIcon(FixedString const& iconName);

    inline bool IsValid() const
    {
        return TextureId != nullptr;
    }

    glm::vec2 Size{ 0.0f, 0.0f };
    glm::vec2 UV0{ 0.0f, 0.0f };
    glm::vec2 UV1{ 1.0f, 1.0f };

    FixedString TextureResource;
    FixedString Icon;
    ImTextureID TextureId{ nullptr };
};


struct Renderable : public Noncopyable<Renderable>
{
public:
    virtual ~Renderable();
    virtual IMGUIObjectType GetType() = 0;
    virtual char const* GetTypeName() = 0;
    virtual lua::GenericPropertyMap& GetRTTI() = 0;
    virtual void Render() = 0;
    void Destroy();

    lua::ImguiHandle GetParent() const;

    HandleType Handle{ InvalidHandle };
    HandleType Parent{ InvalidHandle };
    IMGUIObjectManager* Manager{ nullptr };
    lua::RegistryEntry UserData;
};


struct StyleColor
{
    GuiColor Key;
    glm::vec4 Value;
};

struct StyleVar
{
    GuiStyleVar Key;
    glm::vec2 Value;
};


struct StyledRenderable : public Renderable
{
public:
    virtual void StyledRender() = 0;

    void Render() override;
    std::optional<float> GetStyleVar(GuiStyleVar var);
    void SetStyleVar(GuiStyleVar var, float value, std::optional<float> value2);
    std::optional<glm::vec4> GetStyleColor(GuiColor color);
    void SetStyleColor(GuiColor color, glm::vec4 value);
    lua::ImguiHandle Tooltip();
    void Activate();

    Array<StyleVar> StyleVars;
    Array<StyleColor> StyleColors;
    STDString Label;
    STDString IDContext;
    bool SameLine{ false };
    bool Visible{ true };
    bool RequestActivate{ false };
    bool WasHovered{ false };
    FixedString Font;
    std::optional<glm::vec2> PositionOffset;
    std::optional<glm::vec2> AbsolutePosition;
    std::optional<float> ItemWidth;
    std::optional<float> TextWrapPos;
    GuiItemFlags ItemFlags{ 0 };
    GuiItemStatusFlags StatusFlags{ 0 };

    lua::LuaDelegate<void(lua::ImguiHandle)> OnActivate;
    lua::LuaDelegate<void(lua::ImguiHandle)> OnDeactivate;
    lua::LuaDelegate<void(lua::ImguiHandle)> OnHoverEnter;
    lua::LuaDelegate<void(lua::ImguiHandle)> OnHoverLeave;

private:
    lua::ImguiHandle tooltip_;
};


struct TreeParent : public StyledRenderable
{
public:
    virtual ~TreeParent();
    virtual bool BeginRender() = 0;
    virtual void EndRender() = 0;

    void StyledRender() override;

    lua::ImguiHandle AddGroup(char const* label);
    lua::ImguiHandle AddCollapsingHeader(char const* label);
    lua::ImguiHandle AddTabBar(char const* label);
    lua::ImguiHandle AddTree(char const* label);
    lua::ImguiHandle AddTable(char const* label, uint32_t columns);
    lua::ImguiHandle AddPopup(char const* label);
    lua::ImguiHandle AddChildWindow(char const* label);
    lua::ImguiHandle AddMenu(char const* label);

    lua::ImguiHandle AddButton(char const* label);
    lua::ImguiHandle AddImageButton(char const* label, FixedString iconOrTexture,
        std::optional<glm::vec2> size, std::optional<glm::vec2> uv0, std::optional<glm::vec2> uv1);

    lua::ImguiHandle AddImage(FixedString iconOrTexture,
        std::optional<glm::vec2> size, std::optional<glm::vec2> uv0, std::optional<glm::vec2> uv1);
    lua::ImguiHandle AddIcon(FixedString iconName, std::optional<glm::vec2> height);

    lua::ImguiHandle AddText(char const* label);
    lua::ImguiHandle AddBulletText(char const* label);
    lua::ImguiHandle AddSeparatorText(char const* label);

    lua::ImguiHandle AddSpacing();
    lua::ImguiHandle AddDummy(float width, float height);
    lua::ImguiHandle AddNewLine();
    lua::ImguiHandle AddSeparator();

    lua::ImguiHandle AddCheckbox(char const* label, std::optional<bool> checked);
    lua::ImguiHandle AddRadioButton(char const* label, std::optional<bool> active);
    lua::ImguiHandle AddInputText(char const* label, std::optional<STDString> value);
    lua::ImguiHandle AddCombo(char const* label);
    lua::ImguiHandle AddDrag(char const* label, std::optional<float> value, std::optional<float> min, std::optional<float> max);
    lua::ImguiHandle AddDragInt(char const* label, std::optional<int> value, std::optional<int> min, std::optional<int> max);
    lua::ImguiHandle AddSlider(char const* label, std::optional<float> value, std::optional<float> min, std::optional<float> max);
    lua::ImguiHandle AddSliderInt(char const* label, std::optional<int> value, std::optional<int> min, std::optional<int> max);
    lua::ImguiHandle AddInputScalar(char const* label, std::optional<float> value);
    lua::ImguiHandle AddInputInt(char const* label, std::optional<int> value);
    lua::ImguiHandle AddColorEdit(char const* label, std::optional<glm::vec3> value);
    lua::ImguiHandle AddColorPicker(char const* label, std::optional<glm::vec3> value);

    lua::ImguiHandle AddProgressBar();

    bool RemoveChild(lua::ImguiHandle child);
    bool DetachChild(lua::ImguiHandle child);
    bool AttachChild(lua::ImguiHandle child);
    Array<lua::ImguiHandle> GetChildren() const;

    Array<HandleType> Children;

protected:
    template <class T>
    T* AddChild();
};

struct WindowRenderRequests
{
    struct SetPos
    {
        ImVec2 Pos;
        ImVec2 Pivot;
        ImGuiCond Cond;
    };

    struct SetSize
    {
        ImVec2 Size;
        ImGuiCond Cond;
    };

    struct SetSizeConstraints
    {
        ImVec2 SizeMin;
        ImVec2 SizeMax;
    };

    struct SetCollapsed
    {
        bool Collapsed;
        ImGuiCond Cond;
    };


    std::optional<SetPos> Pos;
    std::optional<SetSize> Size;
    std::optional<SetSizeConstraints> SizeConstraints;
    std::optional<ImVec2> ContentSize;
    std::optional<ImVec2> Scroll;
    std::optional<SetCollapsed> Collapsed;
    bool Focus{ false };
    std::optional<float> BgAlpha;
};

struct Window : public TreeParent
{
public:
    DECL_UI_TYPE(Window)

    bool BeginRender() override;
    void EndRender() override;

    lua::ImguiHandle AddMainMenu();

    void SetPos(glm::vec2 pos, std::optional<GuiCond> cond, std::optional<glm::vec2> pivot);
    void SetSize(glm::vec2 size, std::optional<GuiCond> cond);
    void SetSizeConstraints(std::optional<glm::vec2> size_min, std::optional<glm::vec2> size_max);
    void SetContentSize(std::optional<glm::vec2> size);
    void SetCollapsed(bool collapsed, std::optional<GuiCond> cond);
    void SetFocus();
    void SetScroll(std::optional<glm::vec2> scroll);
    void SetBgAlpha(std::optional<float> alpha);

    bool Open{ true };
    bool Closeable{ false };
    GuiWindowFlags Flags{ 0 };
    lua::LuaDelegate<void(lua::ImguiHandle)> OnClose;
    lua::ImguiHandle MainMenu;

private:
    bool rendering_{ false };
    WindowRenderRequests req_;

    void ProcessRenderSettings();
};


struct MenuBar : public TreeParent
{
public:
    DECL_UI_TYPE(MenuBar)

    bool BeginRender() override;
    void EndRender() override;

private:
    bool rendering_{ false };
};


struct Menu : public TreeParent
{
public:
    DECL_UI_TYPE(Menu)

    bool BeginRender() override;
    void EndRender() override;

    lua::ImguiHandle AddItem(char const* label, std::optional<char const*> shortcut);

private:
    bool rendering_{ false };
};


struct MenuItem : public StyledRenderable
{
public:
    DECL_UI_TYPE(MenuItem)

    void StyledRender() override;

    bool Enabled{ true };
    std::optional<STDString> Shortcut;
    lua::LuaDelegate<void(lua::ImguiHandle)> OnClick;
};


struct Group : public TreeParent
{
public:
    DECL_UI_TYPE(Group)

    bool BeginRender() override;
    void EndRender() override;
};


struct CollapsingHeader : public TreeParent
{
public:
    DECL_UI_TYPE(CollapsingHeader)

    bool BeginRender() override;
    void EndRender() override;

    GuiTreeNodeFlags Flags{ 0 };
};


struct TabBar : public TreeParent
{
public:
    DECL_UI_TYPE(TabBar)

    bool BeginRender() override;
    void EndRender() override;

    lua::ImguiHandle AddTabItem(char const* label);

    GuiTabBarFlags Flags{ 0 };

private:
    bool rendering_{ false };
};


struct TabItem : public TreeParent
{
public:
    DECL_UI_TYPE(TabItem)

    bool BeginRender() override;
    void EndRender() override;

    GuiTabItemFlags Flags{ 0 };

private:
    bool rendering_{ false };
};


struct Tree : public TreeParent
{
public:
    DECL_UI_TYPE(Tree)

    bool BeginRender() override;
    void EndRender() override;
    void SetOpen(bool open, std::optional<GuiCond> cond);

    GuiTreeNodeFlags Flags{ 0 };

    lua::LuaDelegate<void(lua::ImguiHandle)> OnClick;
    lua::LuaDelegate<void(lua::ImguiHandle)> OnExpand;
    lua::LuaDelegate<void(lua::ImguiHandle)> OnCollapse;

private:
    struct SetOpenRequest
    {
        bool Open;
        ImGuiCond Cond;
    };

    bool rendering_{ false };
    std::optional<SetOpenRequest> open_;
};

struct ColumnDefinition
{
    STDString Name;
    GuiTableColumnFlags Flags;
    float Width;
};

struct Table : public TreeParent
{
public:
    DECL_UI_TYPE(Table)

    bool BeginRender() override;
    void EndRender() override;

    void AddColumn(char const* name, std::optional<GuiTableColumnFlags> flags, std::optional<float> width);
    lua::ImguiHandle AddRow();

    uint32_t Columns{ 1 };
    GuiTableFlags Flags{ 0 };
    bool ShowHeader{ false };
    bool AngledHeader{ false };
    Array<ColumnDefinition> ColumnDefs;
    std::optional<glm::vec2> Size;

private:
    bool rendering_{ false };
};


struct TableRow : public TreeParent
{
public:
    DECL_UI_TYPE(TableRow)

    bool BeginRender() override;
    void EndRender() override;

    lua::ImguiHandle AddCell();

    GuiTableRowFlags Flags{ 0 };
    float MinHeight{ 0.0f };
};


struct TableCell : public TreeParent
{
public:
    DECL_UI_TYPE(TableCell)

    bool BeginRender() override;
    void EndRender() override;
};


struct Tooltip : public TreeParent
{
public:
    DECL_UI_TYPE(Tooltip)

    bool BeginRender() override;
    void EndRender() override;

private:
    bool rendering_{ false };
};


struct Popup : public TreeParent
{
public:
    DECL_UI_TYPE(Popup)

    bool BeginRender() override;
    void EndRender() override;

    void Open(std::optional<GuiPopupFlags> flags);

    GuiWindowFlags Flags{ 0 };

private:
    bool rendering_{ false };
    bool requestOpen_{ false };
    GuiPopupFlags openFlags_{ 0 };
};


struct ChildWindow : public TreeParent
{
public:
    DECL_UI_TYPE(ChildWindow)

    bool BeginRender() override;
    void EndRender() override;

    GuiWindowFlags Flags{ 0 };
    GuiChildFlags ChildFlags{ 0 };
    std::optional<glm::vec2> Size;
};


struct Image : public StyledRenderable
{
public:
    DECL_UI_TYPE(Image)

    void StyledRender() override;

    ImageReference ImageData;
    glm::vec4 Border{ 0.0f };
    glm::vec4 Tint{ 1.0f };
};

struct Text : public StyledRenderable
{
public:
    DECL_UI_TYPE(Text)

    void StyledRender() override;
};


struct BulletText : public StyledRenderable
{
public:
    DECL_UI_TYPE(BulletText)

    void StyledRender() override;
};


struct SeparatorText : public StyledRenderable
{
public:
    DECL_UI_TYPE(SeparatorText)

    void StyledRender() override;
};


struct Spacing : public StyledRenderable
{
public:
    DECL_UI_TYPE(Spacing)

    void StyledRender() override;
};


struct Dummy : public StyledRenderable
{
public:
    DECL_UI_TYPE(Dummy)

    void StyledRender() override;

    float Width{ 0.0f };
    float Height{ 0.0f };
};


struct NewLine : public StyledRenderable
{
public:
    DECL_UI_TYPE(NewLine)

    void StyledRender() override;
};


struct Separator : public StyledRenderable
{
public:
    DECL_UI_TYPE(Separator)

    void StyledRender() override;
};


struct Button : public StyledRenderable
{
public:
    DECL_UI_TYPE(Button)

    void StyledRender() override;

    std::optional<glm::vec2> Size;
    GuiButtonFlags Flags;

    lua::LuaDelegate<void (lua::ImguiHandle)> OnClick;
};


struct ImageButton : public StyledRenderable
{
public:
    DECL_UI_TYPE(ImageButton)

    void StyledRender() override;

    ImageReference Image;
    glm::vec4 Background{ 0.0f };
    glm::vec4 Tint{ 1.0f };
    GuiButtonFlags Flags;

    lua::LuaDelegate<void(lua::ImguiHandle)> OnClick;
};


struct Checkbox : public StyledRenderable
{
public:
    DECL_UI_TYPE(Checkbox)

    void StyledRender() override;

    bool Checked{ false };
    lua::LuaDelegate<void (lua::ImguiHandle, bool)> OnChange;
};


struct RadioButton : public StyledRenderable
{
public:
    DECL_UI_TYPE(RadioButton)

    void StyledRender() override;

    bool Active{ false };
    lua::LuaDelegate<void (lua::ImguiHandle, bool)> OnChange;
};


struct InputText : public StyledRenderable
{
public:
    DECL_UI_TYPE(InputText)

    static constexpr unsigned GrowSize = 512;
    static constexpr unsigned MaxSize = 0x10000;

    InputText();
    void StyledRender() override;

    STDString GetText() const;
    void SetText(STDString text);

    STDString Text;
    std::optional<STDString> Hint;
    std::optional<glm::vec2> SizeHint;
    GuiInputTextFlags Flags{ 0 };
    lua::LuaDelegate<void (lua::ImguiHandle, STDString)> OnChange;

private:
    bool reloadText_{ false };
};


struct Combo : public StyledRenderable
{
public:
    DECL_UI_TYPE(Combo)

    void StyledRender() override;

    Array<STDString> Options;
    int SelectedIndex{ -1 };
    GuiComboFlags Flags{ 0 };
    lua::LuaDelegate<void (lua::ImguiHandle, int)> OnChange;
};


struct DragScalar : public StyledRenderable
{
public:
    DECL_UI_TYPE(DragScalar)

    void StyledRender() override;

    glm::vec4 Value{ 0.0f };
    glm::vec4 Min{ 0.0f };
    glm::vec4 Max{ 1.0f };
    int Components{ 1 };
    GuiSliderFlags Flags{ 0 };
    lua::LuaDelegate<void (lua::ImguiHandle, glm::vec4)> OnChange;
};


struct DragInt : public StyledRenderable
{
public:
    DECL_UI_TYPE(DragInt)

    void StyledRender() override;

    glm::ivec4 Value{ 0 };
    glm::ivec4 Min{ 0 };
    glm::ivec4 Max{ 1 };
    int Components{ 1 };
    GuiSliderFlags Flags{ 0 };
    lua::LuaDelegate<void (lua::ImguiHandle, glm::ivec4)> OnChange;
};


struct SliderScalar : public StyledRenderable
{
public:
    DECL_UI_TYPE(SliderScalar)

    void StyledRender() override;

    glm::vec4 Value{ 0.0f };
    glm::vec4 Min{ 0.0f };
    glm::vec4 Max{ 1.0f };
    int Components{ 1 };
    GuiSliderFlags Flags{ 0 };
    bool Vertical{ false };
    glm::vec2 VerticalSize{ 1.0f };
    lua::LuaDelegate<void (lua::ImguiHandle, glm::vec4)> OnChange;
};


struct SliderInt : public StyledRenderable
{
public:
    DECL_UI_TYPE(SliderInt)

    void StyledRender() override;

    glm::ivec4 Value{ 0 };
    glm::ivec4 Min{ 0 };
    glm::ivec4 Max{ 1 };
    int Components{ 1 };
    GuiSliderFlags Flags{ 0 };
    bool Vertical{ false };
    glm::vec2 VerticalSize{ 1.0f };
    lua::LuaDelegate<void (lua::ImguiHandle, glm::ivec4)> OnChange;
};


struct InputScalar : public StyledRenderable
{
public:
    DECL_UI_TYPE(InputScalar)

    void StyledRender() override;

    glm::vec4 Value{ 0.0f };
    int Components{ 1 };
    GuiInputTextFlags Flags{ 0 };
    lua::LuaDelegate<void (lua::ImguiHandle, glm::vec4)> OnChange;
};


struct InputInt : public StyledRenderable
{
public:
    DECL_UI_TYPE(InputInt)

    void StyledRender() override;

    glm::ivec4 Value{ 0 };
    int Components{ 1 };
    GuiInputTextFlags Flags{ 0 };
    lua::LuaDelegate<void (lua::ImguiHandle, glm::ivec4)> OnChange;
};


struct ColorEdit : public StyledRenderable
{
public:
    DECL_UI_TYPE(ColorEdit)

    void StyledRender() override;

    glm::vec4 Color{ 0.0f };
    GuiColorEditFlags Flags{ ImGuiColorEditFlags_DefaultOptions_ };
    lua::LuaDelegate<void (lua::ImguiHandle, glm::vec4)> OnChange;
};


struct ColorPicker : public StyledRenderable
{
public:
    DECL_UI_TYPE(ColorPicker)

    void StyledRender() override;

    glm::vec4 Color{ 0.0f };
    GuiColorEditFlags Flags{ ImGuiColorEditFlags_DefaultOptions_ };
    lua::LuaDelegate<void (lua::ImguiHandle, glm::vec4)> OnChange;
};


struct ProgressBar : public StyledRenderable
{
public:
    DECL_UI_TYPE(ProgressBar)

    void StyledRender() override;

    float Value{ 0.0f };
    glm::vec2 Size{ 0.0f };
    STDString Overlay;
};


class IMGUIObjectPoolInterface
{
public:
    virtual ~IMGUIObjectPoolInterface();
    virtual Renderable* Create() = 0;
    virtual Renderable* Get(HandleType handle) = 0;
    virtual bool Destroy(HandleType handle) = 0;
};


template <class T>
class IMGUIObjectPool : public IMGUIObjectPoolInterface
{
public:
    IMGUIObjectPool() {}
    ~IMGUIObjectPool() override {}

    T* Create() override
    {
        HandleType handle{};
        auto obj = pool_.Add(handle);
        *obj = GameAlloc<T>();
        (*obj)->Handle = handle | ((uint64_t)T::ObjectType << 56);
        return *obj;
    }
    
    T* Get(HandleType handle) override
    {
        auto ref = pool_.Find(handle);
        return ref ? *ref : nullptr;
    }

    bool Destroy(HandleType handle) override
    {
        auto ref = pool_.Find(handle);
        if (ref) {
            GameDelete(*ref);
        }

        return pool_.Free(handle);
    }

private:
    SaltedPool<T*, uint64_t, uint32_t, 24, 31> pool_;
};


class IMGUIObjectManager
{
public:
    IMGUIObjectManager();

    inline lua::DeferredLuaDelegateQueue& GetEventQueue()
    {
        return eventQueue_;
    }

    template <class T>
    T* CreateRenderable()
    {
        return static_cast<T*>(CreateRenderable(T::ObjectType));
    }

    Renderable* CreateRenderable(IMGUIObjectType type);
    Renderable* GetRenderable(HandleType handle);
    bool DestroyRenderable(HandleType handle);
    void Render();
    void EnableDemo(bool enable);
    void ClientUpdate();

private:
    std::array<std::unique_ptr<IMGUIObjectPoolInterface>, (unsigned)IMGUIObjectType::Max + 1> pools_;
    Array<HandleType> windows_;
    bool renderDemo_{ false };
    lua::DeferredLuaDelegateQueue eventQueue_;
};

END_NS()
