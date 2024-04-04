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
    inline lua::GenericPropertyMap& GetRTTI() override { return lua::StaticLuaPropertyMap<ty>::PropertyMap; }


enum class IMGUIObjectType : uint8_t
{
    Window,
    Group,
    CollapsingHeader,
    Text,
    SeparatorText,
    Button,
    Checkbox,
    RadioButton,
    InputText,
    Combo,
    DragScalar,
    SliderScalar,
    InputScalar,
    ColorEdit,
    ColorPicker,
    Max = ColorPicker
};


class Renderable
{
public:
    virtual ~Renderable();
    virtual IMGUIObjectType GetType() = 0;
    virtual char const* GetTypeName() = 0;
    virtual lua::GenericPropertyMap& GetRTTI() = 0;
    virtual void Render() = 0;

    HandleType Handle{ InvalidHandle };
    IMGUIObjectManager* Manager{ nullptr };
};


struct StyleColor
{
    ImGuiCol Key;
    glm::vec4 Value;
};

struct StyleVar
{
    ImGuiStyleVar Key;
    float Value;
};


class StyledRenderable : public Renderable
{
public:
    virtual void StyledRender() = 0;

    void Render() override;
    void SetStyleVar(ImGuiStyleVar var, float value);
    void SetStyleColor(ImGuiCol color, glm::vec4 value);

    Array<StyleVar> StyleVars;
    Array<StyleColor> StyleColors;
    STDString Label;
    bool SameLine{ false };
};


class TreeParent : public StyledRenderable
{
public:
    virtual bool BeginRender() = 0;
    virtual void EndRender() = 0;

    void StyledRender() override;

    lua::ImguiHandle AddButton(char const* label);
    lua::ImguiHandle AddText(char const* label);
    lua::ImguiHandle AddSeparatorText(char const* label);
    lua::ImguiHandle AddCheckbox(char const* label, std::optional<bool> checked);
    lua::ImguiHandle AddRadioButton(char const* label, std::optional<bool> active);
    lua::ImguiHandle AddInputText(char const* label, std::optional<STDString> value);
    lua::ImguiHandle AddCombo(char const* label);
    lua::ImguiHandle AddDrag(char const* label, std::optional<float> value, std::optional<float> min, std::optional<float> max);
    lua::ImguiHandle AddSlider(char const* label, std::optional<float> value, std::optional<float> min, std::optional<float> max);
    lua::ImguiHandle AddInputScalar(char const* label, std::optional<float> value);
    lua::ImguiHandle AddColorEdit(char const* label, std::optional<glm::vec3> value);
    lua::ImguiHandle AddColorPicker(char const* label, std::optional<glm::vec3> value);

    Array<HandleType> Children;

private:
    template <class T>
    T* AddChild();
};


class Window : public TreeParent
{
public:
    DECL_UI_TYPE(Window)

    bool BeginRender() override;
    void EndRender() override;

    bool Closed{ false };
    bool Closeable{ false };
    ImGuiWindowFlags Flags{ 0 };
};


class Group : public TreeParent
{
public:
    DECL_UI_TYPE(Group)

    bool BeginRender() override;
    void EndRender() override;
};


class CollapsingHeader : public TreeParent
{
public:
    DECL_UI_TYPE(CollapsingHeader)

    bool BeginRender() override;
    void EndRender() override;
};


class Text : public StyledRenderable
{
public:
    DECL_UI_TYPE(Text)

    void StyledRender() override;
};


class SeparatorText : public StyledRenderable
{
public:
    DECL_UI_TYPE(SeparatorText)

    void StyledRender() override;
};


class Button : public StyledRenderable
{
public:
    DECL_UI_TYPE(Button)

    void StyledRender() override;
};


class Checkbox : public StyledRenderable
{
public:
    DECL_UI_TYPE(Checkbox)

    void StyledRender() override;

    bool Checked{ false };
};


class RadioButton : public StyledRenderable
{
public:
    DECL_UI_TYPE(RadioButton)

    void StyledRender() override;

    bool Active{ false };
};


class InputText : public StyledRenderable
{
public:
    DECL_UI_TYPE(InputText)

    InputText();
    void StyledRender() override;

    STDString GetText();
    void SetText(STDString text);

    STDString Text;
};


class Combo : public StyledRenderable
{
public:
    DECL_UI_TYPE(Combo)

    void StyledRender() override;

    Array<STDString> Options;
    int SelectedIndex{ -1 };
};


class DragScalar : public StyledRenderable
{
public:
    DECL_UI_TYPE(DragScalar)

    void StyledRender() override;

    glm::vec4 Value{ 0.0f };
    glm::vec4 Min{ 0.0f };
    glm::vec4 Max{ 1.0f };
    int Components{ 1 };
};


class SliderScalar : public StyledRenderable
{
public:
    DECL_UI_TYPE(SliderScalar)

    void StyledRender() override;

    glm::vec4 Value{ 0.0f };
    glm::vec4 Min{ 0.0f };
    glm::vec4 Max{ 1.0f };
    int Components{ 1 };
};


class InputScalar : public StyledRenderable
{
public:
    DECL_UI_TYPE(InputScalar)

    void StyledRender() override;

    glm::vec4 Value{ 0.0f };
    int Components{ 1 };
};


class ColorEdit : public StyledRenderable
{
public:
    DECL_UI_TYPE(ColorEdit)

    void StyledRender() override;

    glm::vec4 Color{ 0.0f };
    bool HasAlpha{ false };
};


class ColorPicker : public StyledRenderable
{
public:
    DECL_UI_TYPE(ColorPicker)

    void StyledRender() override;

    glm::vec4 Color{ 0.0f };
    bool HasAlpha{ false };
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

    Renderable* Create() override
    {
        HandleType handle{};
        auto obj = pool_.Add(handle);
        obj->Handle = handle | ((uint64_t)T::ObjectType << 56);
        return obj;
    }
    
    Renderable* Get(HandleType handle) override
    {
        return pool_.Find(handle);
    }

    bool Destroy(HandleType handle) override
    {
        return pool_.Free(handle);
    }

private:
    SaltedPool<T, uint64_t, uint32_t, 24, 31> pool_;
};


class IMGUIObjectManager
{
public:
    IMGUIObjectManager();

    template <class T>
    T* CreateRenderable()
    {
        return static_cast<T*>(CreateRenderable(T::ObjectType));
    }

    Renderable* CreateRenderable(IMGUIObjectType type);
    Renderable* GetRenderable(HandleType handle);
    bool DestroyRenderable(HandleType handle);
    void Render();

private:
    std::array<std::unique_ptr<IMGUIObjectPoolInterface>, (unsigned)IMGUIObjectType::Max + 1> pools_;
    Array<HandleType> windows_;
};

END_NS()
