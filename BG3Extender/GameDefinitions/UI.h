#pragma once

#include <CoreLib/Base/Base.h>

#include <NsCore/BaseComponent.h>
#include <NsCore/TypeClass.h>
#include <NsGui/BaseCommand.h>
#include <NsGui/DependencyData.h>
#include <NsGui/DependencyObjectValueData.h>
#include <NsGui/RoutedEvent.h>
#include <NsGui/UIElement.h>
#include <NsGui/UIElementData.h>
#include <NsGui/FrameworkElement.h>
#include <NsGui/Panel.h>
#include <NsGui/ObservableCollection.h>
#include <NsGui/UIElementCollection.h>
#include <NsGui/ContentControl.h>

BEGIN_BARE_NS(Noesis)

using namespace bg3se;

using LoadXamlProc = Ptr<BaseComponent>* (Ptr<BaseComponent>& ret, char const* path);

struct SymbolManagerInternals
{
    Vector<char const*> Strings;
    HashMap<char const*, uint32_t> Keys;
};

struct ReflectionInternals
{
    Vector<Symbol> Names;
    HashMap<uint32_t, Type*> NameToType;
};

inline void* Alloc(SizeT size)
{
    return GameAllocRaw(size);
}

inline void* Realloc(void* ptr, SizeT size)
{
    if (ptr != nullptr) {
        auto newMem = Alloc(size);
        memcpy(newMem, ptr, size);
        Dealloc(ptr);
        return newMem;
    } else {
        return Alloc(size);
    }
}

inline void Dealloc(void* ptr)
{
    return GameFree(ptr);
}

inline char const* Type::GetName() const
{
    return mName.Str();
}

inline Symbol Type::GetTypeId() const
{
    return mName;
}

inline ReflectionInternals* GetReflection()
{
    return (ReflectionInternals *)((uintptr_t)GetStaticSymbols().Noesis__Reflection__Data - sizeof(Noesis::BaseVector_));
}

inline SymbolManagerInternals* GetSymbolManager()
{
    return (SymbolManagerInternals*)((uintptr_t)GetStaticSymbols().Noesis__SymbolManager__Data - sizeof(Noesis::BaseVector_));
}

inline char const* SymbolManager::GetString(uint32_t index)
{
    return GetSymbolManager()->Strings[index];
}

inline const Type* Reflection::GetType(Symbol name)
{
    auto lock = GetStaticSymbols().Noesis__Reflection__Lock;
    AcquireSRWLockShared(lock);

    Type* val;
    auto it = GetReflection()->NameToType.Find((uint32_t)name);
    if (it != GetReflection()->NameToType.End()) {
        val = it->value;
    } else {
        val = nullptr;
    }

    ReleaseSRWLockShared(lock);
    return val;
}

inline TypeClass const* TypeClass::GetBase() const
{
    return mBase;
}

inline bool TypeClass::IsInterface() const
{
    return mIsInterface;
}

struct ObjectHelpers
{
public:
    static PropertyOperationResult FallbackGetProperty(lua_State* L, lua::LifetimeHandle lifetime, BaseObject const* object, bg3se::FixedString const& prop);
    static PropertyOperationResult FallbackSetProperty(lua_State* L, BaseObject* object, bg3se::FixedString const& prop, int index);
    static UserReturn GetNamedProperty(lua_State* L, BaseObject const* o, bg3se::FixedString const& name);
    static UserReturn GetProperty(lua_State* L, BaseObject const* o, TypeProperty const* prop);
    static void SetNamedProperty(lua_State* L, BaseObject* o, bg3se::FixedString const& name, lua::AnyRef value);
    static void SetProperty(lua_State* L, BaseObject* o, TypeProperty const* prop, lua::AnyRef value);
    static UserReturn GetAllProperties(lua_State* L, BaseObject const* o);
    static UserReturn GetDirectProperties(lua_State* L, BaseObject const* o);
    static UserReturn GetDependencyProperties(lua_State* L, BaseObject const* o);

    static TypeClass* GetClassType(BaseObject const* o);
    static Symbol GetClassTypeName(BaseObject const* o);
    static STDString ToString(BaseObject const* o);
    static uint32_t GetNumReferences(BaseRefCounted const* o);

private:
    static void DoGetDirectProperties(lua_State* L, BaseObject const* o);
    static void DoGetDependencyProperties(lua_State* L, BaseObject const* o);
};

struct RoutedEventHelpers
{
    static Symbol GetName(RoutedEvent const* o);
    static TypeClass* GetOwnerType(RoutedEvent const* o);
};

struct DependencyObjectHelpers
{
    static UserReturn GetProperty(lua_State* L, DependencyObject const* o, bg3se::FixedString const& name);
    static UserReturn GetDependencyProperty(lua_State* L, DependencyObject const* o, DependencyProperty const* prop);
    static void SetProperty(lua_State* L, DependencyObject* o, bg3se::FixedString const& name, lua::AnyRef value);
    static void SetDependencyProperty(lua_State* L, DependencyObject* o, DependencyProperty const* prop, lua::AnyRef value);
};

struct DependencyPropertyHelpers
{
    static Symbol GetName(DependencyProperty const* o);
    static Type* GetType(DependencyProperty const* o);
    static TypeClass* GetOwnerType(DependencyProperty const* o);
    static bool IsReadOnly(DependencyProperty const* o);
};

struct StoredValueHolder
{
    inline StoredValueHolder() {}
    inline StoredValueHolder(void* val, bool owned) : Value(val), IsIntegral(true), IsOwned(owned) {}
    inline StoredValueHolder(BaseObject* val) : Value(val), IsIntegral(true), IsOwned(false) {}
    inline StoredValueHolder(String* val) : Value(val), IsIntegral(false), IsOwned(false) {}

    ~StoredValueHolder();

    void* Value{ nullptr };
    bool IsIntegral{ true };
    bool IsOwned{ false };
};

struct StoredValueHelpers
{
    static std::optional<int64_t> TryParseIntegralValue(lua_State* L, Type const* type, void* val);
    static void PushValue(lua_State* L, Type const* type, StoredValue const* o, Type const* objectType = nullptr, Symbol* propertyName = nullptr);
    static void PushValue(lua_State* L, Type const* type, void* val, Type const* objectType = nullptr, Symbol* propertyName = nullptr);
    static void PushProperty(lua_State* L, BaseObject const* obj, TypeClass const* objType, TypeProperty const* prop);
    static void PushValue(lua_State* L, TypeEnum const* type, uint64_t val);

    template <class T>
    static void PushRawValue(lua_State* L, Type const* type, void* val);
    template <class T>
    static void PushPtrValue(lua_State* L, Type const* type, void* val);

    template <class T>
    static StoredValueHolder GetRawValue(lua_State* L, Type const* type, lua::AnyRef value);

    static std::optional<StoredValueHolder> GetValue(lua_State* L, Type const* type, lua::AnyRef value);
};

struct CommandHelpers
{
    static bool CanExecute(lua_State* L, BaseCommand const* o, std::optional<BaseComponent*> arg);
    static void Execute(lua_State* L, BaseCommand const* o, std::optional<BaseComponent*> arg);
};

struct VisualHelpers
{
    static Visual* GetVisualParent(Visual const* o);
    static uint32_t GetVisualChildrenCount(lua_State* L, Visual const* o);
    static Visual* GetVisualChild(Visual const* o, uint32_t index);
};

struct UIElementDataHelpers
{
    static RoutedEvent* GetEvent(UIElementData const* o, Symbol evt);
    static Array<RoutedEvent*> GetAllEvents(UIElementData const* o);
};

struct UIElementHelpers
{
    static uint64_t Subscribe(lua_State* L, UIElement* o, bg3se::FixedString const& evt, lua::FunctionRef func);
    static bool Unsubscribe(lua_State* L, UIElement* o, uint64_t index);
};

struct FrameworkElementHelpers
{
    static FrameworkElement* GetLogicalParent(FrameworkElement const* o);
    static uint32_t GetLogicalChildrenCount(FrameworkElement const* o);
    static BaseComponent* GetLogicalChild(FrameworkElement const* o, uint32_t index);
    static BaseComponent* FindNodeName(FrameworkElement const* o, char const* name);
    static BaseComponent* GetResource(FrameworkElement const* o, char const* key, std::optional<bool> fullElementSearch);
    static BaseObject* GetTreeParent(FrameworkElement const* o);
    static FrameworkElement* AttachXamlChild(FrameworkElement* o, char const* path);
    static FrameworkElement* SetXamlProperty(FrameworkElement* o, bg3se::FixedString const& prop, char const* path);
};

struct TypePropertyHelpers
{
    static Symbol GetName(TypeProperty const* o);
    static Type* GetContentType(TypeProperty const* o);
    static Vector<TypeMetaData*> const* GetMeta(TypeProperty const* o);
    static bool IsReadOnly(TypeProperty const* o);
};

struct TypeHelpers
{
    static STDString GetName(Type const* o);
    static bool IsDescendantOf(Type const* type, TypeClass const* base);
    static bool IsDescendantOf(TypeClass const* type, TypeClass const* base);
    static TypeClass* GetBase(TypeClass const* o);
    static bool IsInterface(TypeClass const* o);
    static TypeClass::AncestorVector const* GetInterfaces(TypeClass const* o);

    static Array<TypeProperty const*> const* GetProperties(TypeClass const* o);
    static Array<DependencyProperty const*> const* GetDependencyProperties(TypeClass const* o);
    static Array<RoutedEvent const*> const* GetRoutedEvents(TypeClass const* o);

    static TypeProperty const* GetProperty(TypeClass const* o, bg3se::FixedString const& name);
    static DependencyProperty const* GetDependencyProperty(TypeClass const* o, bg3se::FixedString const& name);
    static RoutedEvent const* GetRoutedEvent(TypeClass const* o, bg3se::FixedString const& name);
    static TypeMetaData* FindMetaOrDescendant(TypeClass const* o, const TypeClass* metaDataType);

    static std::optional<uint64_t> StringToEnum(TypeEnum const* e, char const* value);
    static std::optional<uint64_t> StringToEnum(TypeEnum const* e, Symbol value);

    template <class Fun>
    static void ForEachMeta(TypeClass const* cls, const TypeClass* metaDataType, Fun fun);
};

END_BARE_NS()


BEGIN_NS(ui)

using namespace Noesis;

struct UICanvas;
struct UIManager_Sub1F0;
struct UIManager_Sub198;

struct UIInitialSubstate : public Noesis::BaseComponent
{
    String Name;
    String MetaData;
};


struct UIStateEvent : public Noesis::BaseComponent
{
    String Name;
    String HandlesEvent;
    BaseObservableCollection* Actions;
};


struct UIStateWidget : public Noesis::BaseComponent
{
    String Filename;
    String Layer;
    String SoundLayer;
    bool IgnoreHitTest;
    bool BlockedLoading;
    bool AllowCreationDelay;
    BaseObservableCollection* Libraries;
};


struct UIWidgetMetadata
{
    __int64 field_0;
    Guid StateGuid;
    int field_18;
    int field_1C;
    uint8_t Flags;
};


struct UIState : public Noesis::BaseComponent
{
    String Name;
    String Layout;
    String Owner;
    String DisableStatesBelow;
    String HideStatesBelow;
    String IsModal;
    String IsModalCameraControlAllowed;
    String ForceSplitscreen;
    String ForceVisible;
    String DisableSoundLayersAbove;
    String TutorialLayer;
    String ModType;
    String IsModdable;
    BaseObservableCollection* InitialSubstates;
    BaseObservableCollection* Events;
    BaseObservableCollection* Widgets;
    String ActivateOnRestore;
    __int64 field_1A8_Events_M;
    __int64 field_1B0;
    __int64 field_1B8;
    __int64 field_1C0;
    __int64 field_1C8;
    __int64 field_1D0;
    __int64 field_1D8;
    __int64 field_1E0;
    uint8_t field_1E8[2048];
    LegacyMap<int, int> field_9E8;
};


struct UIWidget : public ContentControl
{
    __int64 field_278;
    __int64 field_280;
    __int64 field_288;
    __int64 field_290;
    __int64 field_298;
    __int64 field_2A0;
    __int64 field_2A8;
    __int64 field_2B0;
    __int64 field_2B8;
    __int64 field_2C0;
    __int64 field_2C8[5];
    UIManager_Sub1F0* Sub1F0;
    UIStateWidget* WidgetData;
    __int64 field_2D0;
    Guid StateGuid;
    FrameworkElement* WidgetContainerGrid;
    int field_2F0;
    int field_2F4;
    STDString XAMLPath;
    uint8_t field_310;
    uint8_t field_311;
    uint8_t field_312;
    __int64 field_318;
    __int64 field_320;
    __int64 field_328;
    __int64 field_330;
};



struct UIStateInstance : public ProtectedGameObject<UIStateInstance>
{
    Array<UIWidget*> Widgets;
    Array<UIWidgetMetadata*> WidgetMetadata;
    Array<Array<UIStateInstance*>*> States;
    UIState* State;
    PlayerId PlayerID;
    uint8_t Flags;
    uint8_t Flags2;
    uint8_t Layout;
    uint8_t Owner;
    uint8_t field_3E;
    uint8_t field_3F;
    FixedString field_40;
    int field_44;
    String field_48;
    String field_68;
    String TutorialLayer;
    Guid StateGuid;
    Guid field_B8;
    UIElementCollection* StateWidgets;
};

struct UIStateMachine : public Noesis::BaseComponent
{
    UnknownSignal StateActivatedCallbacks;
    UnknownSignal StateDeactivatedCallbacks;
    UnknownSignal StateCallbacks;
    UnknownSignal field_58;
    UnknownSignal field_70;
    int field_88;
    bool CanProcessEvents;
    bool IsProcessingEvent;
    char field_8E;
    char field_8F;
    char field_90;
    UIStateInstance* State;
    UIManager_Sub1F0* field_A0;
    UIManager_Sub198* field_A8;
    HashMap<Guid, uint64_t> field_B0_MHM_Guid_pStateStack; // StateStack*
    HashMap<Guid, UIStateInstance*> field_F0_MHM_Guid_pState;
    HashMap<Guid, UIStateInstance*> field_130_MHM_Guid_pState;
    HashSet<UIStateInstance*> field_170_MHS_pState;
    HashSet<void*> field_1A0_pStateStack; // StateStack*
    HashSet<UIStateInstance*> field_1D0_pState;
    String RootState;
    BaseObservableCollection* States;
    HashMap<PlayerId, Array<Guid>> field_228_MHM_short_Guid;
    HashSet<PlayerId> field_268_MHS_short;
    HashMap<short, Guid> field_298_short_Guid;
    __int64 field_2D8;
    HashMap<PlayerId, bool> field_2E0_short_bool;
    HashMap<PlayerId, Array<Guid>> field_320_short_Guid;
    HashSet<PlayerId> field_360_MHS_short;
    __int64 SignalConnections;
    __int64 field_398;
    __int64 field_3A0;
    __int64 field_3A8;
    __int64 field_3B0;
    __int64 field_3B8;
    __int64 field_3C0;
    __int64 field_3C8;
    HashSet<PlayerId> field_3D0_MHS_short;
    PlayerId PlayerID;
    CRITICAL_SECTION CriticalSection;

    struct EventArgs
    {
        uint32_t EventIndex{ 0 };
        int field_4{ 3 };
        FixedString StateEvent;
        FixedString SubState;
        bool RemoveState{ false };
        int16_t PlayerId{ 0 };
        uint64_t field_18{ 0 };
    };

    struct EventResult
    {
        __int64 field_0;
        int field_8;
        int field_C;
        int field_10;
        int field_14;
        int field_18;
        int field_1C;
        __int64 field_20;
        int field_28;
        int field_2C;
        uint16_t field_30;
        int field_34;
        BYTE field_38;
        char field_39;
    };

    struct ECSData
    {
        ecs::EntityWorld* EntityWorld{ nullptr };
        ecs::EntityWorld* EntityWorld2{ nullptr };
        ecs::EntityStorageContainer* Storage{ nullptr };
        ecs::QueryRegistry* QuerySystem{ nullptr };
        ecs::GroupAllocator* GroupAllocator{ nullptr };
    };

    struct EntityContext
    {
        ECSData const* ECS{ nullptr };
        int WorldView{ 0 };
        uint8_t field_C{ 0 };
    };

};

struct UIManager_Sub50
{
    void* field_50;
    void* field_58;
    float field_60;
    float field_64;
    float field_68;
    void* field_70;
    int field_78;
    int Width;
    int Height;
    __int16 field_84;
};

struct UIManager_Sub70
{
    UIManager_Sub50 field_0;
    Noesis::BaseRefCounted* RenderContext;
    Noesis::BaseRefCounted* RenderDevice;
    Noesis::BaseRefCounted* field_48;
    Noesis::BaseRefCounted* field_50;
    Noesis::BaseRefCounted* ElementCacheManager;
    UICanvas* Canvas;
    void* field_68;
    __int64 field_70;
    void* field_78;
    int field_80;
    void* field_88;
    int field_90;
    int field_94;
    int field_98;
    float field_9C;
    __int64 field_A0;
    __int64 field_A8;
    __int64 field_B0;
};

struct UIManager_Sub120
{
    __int64 field_0;
    UnknownSignal field_8;
    UnknownSignal field_20;
};

struct UIManager_Sub168
{
    Array<void*> field_0;
    int field_10;
    int field_14;
};

struct UIManager_Sub198
{
    UnknownSignal field_0;
    HashSet<uint64_t> field_18;
    Array<void*> field_48;
};

struct UIManager_Sub1F0
{
    UnknownSignal field_0;
    UnknownSignal field_18;
    UnknownSignal field_30;
    UnknownSignal field_48;
    UnknownSignal field_60;
    __int64 field_78;
    __int64 field_80;
    Noesis::BaseComponent* field_88;
    Noesis::BaseComponent* field_90;
    Array<void*> field_98_Arr_pWidget;
    Array<void*> field_A8_Arr_pWidget;
    LegacyRefMap<int, int> field_B8;
    __int64 field_C8;
    Noesis::BaseObject* field_D0;
    __int64 field_D8;
    CRITICAL_SECTION field_E0;
    HashMap<void*, void*> field_108_MHM_StateKey_DCAccum;
    Noesis::BaseComponent* field_148;
    Noesis::BaseComponent* field_150;
    Noesis::BaseComponent* field_158;
    Noesis::BaseComponent* field_160;
    Array<void*> field_168_Arr_WidgetLoadDelayedData;
};

struct UIManager_Sub358
{
    Noesis::BaseComponent* field_0;
    Noesis::BaseComponent* field_8;
    Noesis::BaseComponent* field_10;
    Noesis::BaseComponent* field_18;
    CRITICAL_SECTION field_20;
    Noesis::BaseRefCounted* field_48;
    Noesis::BaseRefCounted* field_50;
    UIStateMachine* StateMachine;
};

struct UIManager : public ProtectedGameObject<UIManager>
{
    void* VMT;
    void* VMT3;
    void* VMT2;
    __int64 field_18;
    void* VMT4;
    __int64 field_28;
    void* VMT5;
    UnknownSignal field_38;
    UIManager_Sub50 field_50;
    void* field_88x;
    uint64_t field_90[6];
    UIManager_Sub70 field_88;
    __int64 field_120;
    UnknownSignal field_148;
    UnknownSignal field_160x;
    UnknownSignal field_160;
    __int64 field_198x[8];
    //__int64 field_180;
    //__int64 field_1A8;
    //UnknownSignal field_190;
    //__int64 field_1C8;
    UnknownSignal field_1D0;
    HashMap<int16_t, void*> field_1E8_MHM_short_PlayerContextMenu;
    __int64 field_228;
    Array<void*> field_230_Arr_ModData;
    UIManager_Sub1F0 field_240;
    UIManager_Sub358 field_3B8;
};

struct UICanvas : public Noesis::Panel
{
    __int64 field_2A0;
    __int64 field_2A8;
    __int64 field_2B0;
    __int64 field_2B8;
    __int64 field_2C0;
    __int64 field_2C8;
    __int64 field_2D0;
    UIManager_Sub70* UIManager70;
    __int64 ElementCacheManager;
    DependencyObject* field_2E8;
    float field_2F0;
    float field_2F4;
};

struct DeferredCommand : public BaseCommand
{
    __int64 field_38;
    __int64 VMT3;
    __int64 field_48;
    __int64 field_50;
    __int64 field_58;
    __int64 field_60;
    __int64 field_68;
    __int64 field_70;
    __int64 field_78;
    __int64 VMT4;
};


struct ViewModel : public Noesis::BaseComponent, public INotifyPropertyChanged
{
};

struct DCWidget : public ViewModel
{
    /*__int64 VMT3;
    __int64 field_20;
    __int64 field_28;
    __int64 field_30;
    uint8_t field_38;
    UIManager_Sub168 field_40;
    Array<void*> field_58;
    bool field_68;
    bool field_69;
    DCWidget* pThis;
    Noesis::String Name;
    Symbol NameSymbol;
    DCWidget* pThis2;
    uint8_t Layout;
    Symbol LayoutSymbol;
    __int64 field_B0;
    Array<UnknownSignal> field_B8;
    __int64 field_C8;
    __int64 field_D0;
    PlayerId PlayerId;
    ui::DeferredCommand* CustomEvent;
    ui::DeferredCommand* OpenMessageBox;
    ui::DeferredCommand* PasteFromClipboardToTextBoxCommand;
    __int64 field_F8;*/
};


END_NS()


BEGIN_NS(lua)

#define FOR_EACH_NOESIS_TYPE() \
    FOR_NOESIS_TYPE(Noesis::BaseObject) \
    FOR_NOESIS_TYPE(Noesis::BaseRefCounted) \
    FOR_NOESIS_TYPE(Noesis::BaseComponent) \
    FOR_NOESIS_TYPE(Noesis::Type) \
    FOR_NOESIS_TYPE(Noesis::TypeMeta) \
    FOR_NOESIS_TYPE(Noesis::TypeMetaData) \
    FOR_NOESIS_TYPE(Noesis::TypeClass) \
    FOR_NOESIS_TYPE(Noesis::BaseCommand) \
    FOR_NOESIS_TYPE(Noesis::DispatcherObject) \
    FOR_NOESIS_TYPE(Noesis::DependencyObject) \
    FOR_NOESIS_TYPE(Noesis::DependencyData) \
    FOR_NOESIS_TYPE(Noesis::Visual) \
    FOR_NOESIS_TYPE(Noesis::UIElement) \
    FOR_NOESIS_TYPE(Noesis::UIElementData) \
    FOR_NOESIS_TYPE(Noesis::FrameworkElement) \
    FOR_NOESIS_TYPE(Noesis::Panel) \
    FOR_NOESIS_TYPE(ui::UIInitialSubstate) \
    FOR_NOESIS_TYPE(ui::UIStateEvent) \
    FOR_NOESIS_TYPE(ui::UIStateWidget) \
    FOR_NOESIS_TYPE(ui::UIState) \
    FOR_NOESIS_TYPE(ui::UIWidget) \
    FOR_NOESIS_TYPE(ui::DCWidget)

#define FOR_NOESIS_TYPE(c) LUA_POLYMORPHIC(c)
FOR_EACH_NOESIS_TYPE()
#undef FOR_NOESIS_TYPE

LUA_POLYMORPHIC(Noesis::RoutedEventArgs)

END_NS()
