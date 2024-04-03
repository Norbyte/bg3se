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
    Text,
    Button,
    Max = Button
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
};


class Window : public StyledRenderable
{
public:
    DECL_UI_TYPE(Window)

    void StyledRender() override;

    STDString Id;
    bool Closed{ false };
    bool Closeable{ false };
    ImGuiWindowFlags Flags{ 0 };

    Array<HandleType> Children;
};


class Text : public StyledRenderable
{
public:
    DECL_UI_TYPE(Text)

    void StyledRender() override;

    STDString Label;
};


class Button : public StyledRenderable
{
public:
    DECL_UI_TYPE(Button)

    void StyledRender() override;

    STDString Label;
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
