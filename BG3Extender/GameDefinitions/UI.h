#pragma once

#include <CoreLib/Base/Base.h>

#include <NsCore/BaseComponent.h>
#include <NsCore/TypeClass.h>
#include <NsCore/Delegate.h>
#include <NsGui/GridLength.h>
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
#include <NsGui/VisualTreeHelper.h>
#include <NsGui/UserControl.h>
#include <NsGui/Uri.h>

BEGIN_BARE_NS(Noesis)

using namespace bg3se;

// Equivalent to Noesis GridLength
struct GridLengthHelper
{
    GridUnitType GridUnitType;
    float Value;
};

using LoadXamlProc = Ptr<BaseComponent>* (Ptr<BaseComponent>& ret, char const* path);
using Visual__PointConversionProc = Point (Visual const*, Point const& point);
using Visual__TreeHelperHitTestProc = HitTestResult (Visual* visual, Point const& point);

struct SymbolManagerInternals
{
    Vector<char const*> Strings;
    HashMap<char const*, uint32_t, 0, HashBucket_KHV<const char*, uint32_t, CaseCharArrayHashKeyInfo>> Keys;
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
    auto syms = GetSymbolManager();
    // Handle possibly corrupted symbol ptrs
    if (index < syms->Strings.Size()) {
        return syms->Strings[index];
    } else {
        return "";
    }
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

enum class StoredValueCopy {};

struct StoredValueHolder
{
    inline StoredValueHolder() {}

    template <class T>
    inline StoredValueHolder(T const& val, StoredValueCopy)
    {
        if constexpr (sizeof(T) <= sizeof(void*)) {
            Value = *reinterpret_cast<void* const*>(&val);
            IsIntegral = true;
            IsOwned = false;
        } else {
            Value = GameAlloc<T>(val);
            IsIntegral = false;
            IsOwned = true;
        }
    }

    inline StoredValueHolder(BaseObject* val) : Value(val), IsIntegral(true), IsOwned(false) {}

    StoredValueHolder(StoredValueHolder const&) = delete;
    StoredValueHolder & operator = (StoredValueHolder const&) = delete;

    inline StoredValueHolder(StoredValueHolder && o) noexcept
        : Value(o.Value), IsIntegral(o.IsIntegral), IsOwned(o.IsOwned)
    {
        o.IsOwned = false;
    }

    inline StoredValueHolder& operator = (StoredValueHolder&& o) noexcept
    {
        if (&o != this) {
            if (IsOwned) {
                GameFree(Value);
            }

            Value = o.Value;
            IsIntegral = o.IsIntegral;
            IsOwned = o.IsOwned;
            o.IsOwned = false;
        }

        return *this;
    }

    inline ~StoredValueHolder()
    {
        if (IsOwned) {
            GameFree(Value);
        }
    }

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
    static bool InterfaceCanExecute(lua_State* L, ICommand const* o, std::optional<BaseComponent*> arg);
    static void InterfaceExecute(lua_State* L, ICommand const* o, std::optional<BaseComponent*> arg);
};

struct VisualHelpers
{
    static Visual* GetVisualParent(Visual const* o);
    static uint32_t GetVisualChildrenCount(lua_State* L, Visual const* o);
    static Visual* GetVisualChild(Visual const* o, uint32_t index);
    static Point PointFromScreen(lua_State* L, Visual const* o, Point const& point);
    static Point PointToScreen(lua_State* L, Visual const* o, Point const& point);
    static Visual* HitTest(lua_State* L, Visual* o, Point const& point);
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
    // Variant of GetMeta() for Lua calls (Noesis::Vector is not exposed to Lua as a container type)
    static Array<TypeMetaData*> GetMetaLua(TypeProperty const* o);
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

class LuaDelegateCommand final : public BaseCommand
{
public:
    bool CanExecute(BaseComponent* param) const override;
    void Execute(BaseComponent* param) const override;
    void BindHandler(lua_State* L, lua::Ref const& handler);

    NS_DECLARE_REFLECTION(LuaDelegateCommand, BaseCommand)

private:
    lua::PersistentRegistryEntry handler_;
};

END_BARE_NS()


BEGIN_NS(ui)

using namespace Noesis;

struct UICanvas;
struct TooltipManager;
struct ContextMenuManager;
struct StateMachine;
struct DragAndDropManager;
struct MessageBoxManager;
struct BrushManager;
struct WindowManager;

struct UIEvent
{
    uint32_t GameStateId{ 0 };
    uint8_t Type{ 0 };
    FixedString EventName;
    FixedString field_C;
    bool field_10{ false };
    bool field_11{ false };
    PlayerId PlayerId;
    Array<input::FireEventDesc>* InputEvents{ nullptr };
};


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
    Noesis::Uri Uri;
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


struct UIWidget : public UserControl
{
    CRITICAL_SECTION CS_;
    Array<void*> field_2A0_InputChildInfo;
    Array<void*> field_2B0_InputChildChange;
    uint64_t qword2C0[2];
    ContextMenuManager* ContextMenuManager;
    void* qword2D8;
    TooltipManager* TooltipManager;
    WindowManager* WindowManager;
    StateMachine* StateMachine;
    UIStateWidget* WidgetData;
    input::InputManager* InputManager;
    Guid StateId;
    void* ParentCanvas;
    int LayerIndex;
    int IndexInLayer;
    STDString FileName;
    Noesis::BaseRefCounted* ContextMenu;
    Noesis::UIElement* FocusRefresh;
    Noesis::UIElement* GetRequestedFocusFallback;
    Noesis::BaseRefCounted* FocusVisual;
    Noesis::BaseRefCounted* qword360;
    int FocusDownEvent;
    int FocusLeftEvent;
    int FocusRightEvent;
    int FocusUpEvent;
    int UIPasteEvent;
    int UICopyEvent;
    int UICutEvent;
    int UISelectAllTextEvent;
    int UIContextMenuEvent;
    int MoveFocusInputState;
    uint8_t Flags;
    uint8_t Flags2;
    uint8_t Flags3;
    PlayerId PlayerId;
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
    void* field_A0;
    void* field_A8;
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

template <class T>
struct alignas(64) UnsafeMessageQueue : public ProtectedGameObject<UnsafeMessageQueue<T>>
{
    T* Head;
    struct alignas(64)
    {
        T* Tail;
        T* BufferEnd;
        T* SomeBufferPtr;
        T* NextBuffer;
    };
};

struct GameUIParams
{
    void* RendererCommandBuffer;
    void* gap8;
    float oword10;
    float field_14;
    float field_18;
    int field_1C;
    void* ResourceManager;
    int CurHeight_M;
    int ScreenWidth;
    int ScreenHeight;
    BYTE IsNuWT;
    char field_35;
    char field_36;
    char field_37;
};

template <class T> requires requires(T v) { { v.QueueNext } -> std::same_as<T*&>; }
struct IntrusiveMPSCQueueSmall : public ProtectedGameObject<IntrusiveMPSCQueueSmall<T>>
{
    T* WriteStart{ nullptr };
    T* ReadStart{ nullptr };
    T Sentinel;
};

struct BaseCollectionCommand
{
    virtual ~BaseCollectionCommand() {}
    virtual void Run(Noesis::BaseCollection*) {}

    uint64_t ID;
    BaseCollectionCommand* QueueNext;
};

struct IBaseProperty
{
    virtual void SetFromUI(Noesis::BaseComponent* value) = 0;
    virtual void DeferChange(Noesis::BaseComponent* value) = 0;
};

template <class T>
struct DeferredProperty : public IBaseProperty
{
    // TODO - unimplemented
    void SetFromUI(Noesis::BaseComponent* value) override {}
    void DeferChange(Noesis::BaseComponent* value) override {}

    Noesis::BaseComponent* Object;
    T Value;
    Noesis::Symbol Name;
};

struct DeferredCollectionsManager : public ProtectedGameObject<DeferredCollectionsManager>
{
    uint64_t qword0;
    HashMap<uint64_t, Noesis::Ptr<Noesis::BaseObservableCollection>> Collections;
    IntrusiveMPSCQueueSmall<BaseCollectionCommand> Queue;
    CRITICAL_SECTION CS;
    uint8_t Flags;
};

struct DeferredPredicateRefreshParam
{
    void* Predicate; // FilterPredicate*
    DeferredPredicateRefreshParam* QueueNext;
};

struct DeferredPredicatesManager : public ProtectedGameObject<DeferredPredicatesManager>
{
    IntrusiveMPSCQueueSmall<DeferredPredicateRefreshParam> Queue;
    uint8_t Flags;
};

struct DeferredPropertyChangedParams
{
    ViewModel* ViewModel;
    Noesis::Symbol Name;
    DeferredPropertyChangedParams* QueueNext;
};

struct DeferredSetFromUIParams
{
    IBaseProperty* Property;
    ui::ViewModel* ViewModel;
    Noesis::BaseComponent* Value;
    DeferredSetFromUIParams* QueueNext;
};

struct DeferredListenersStorageParams
{
    IBaseProperty* Listener;
    DeferredListenersStorageParams* QueueNext;
};

struct DeferredPropertiesManager : public ProtectedGameObject<DeferredPropertiesManager>
{
    IntrusiveMPSCQueueSmall<DeferredPropertyChangedParams> PropertyChanges;
    IntrusiveMPSCQueueSmall<DeferredSetFromUIParams> SetFromUI;
    IntrusiveMPSCQueueSmall<DeferredListenersStorageParams> ListenerStorage;
    uint8_t Flags;
};


struct NoesisUIManager : public ProtectedGameObject<NoesisUIManager>
{
    GameUIParams Params;
    Noesis::BaseRefCounted* RenderContext;
    void* RenderDevice;
    Noesis::BaseRefCounted* XamlProvider;
    Noesis::BaseRefCounted* FontProvider;
    Noesis::BaseRefCounted* ElementCache;
    UICanvas* MainCanvas;
    Noesis::BaseRefCounted* field_68;
    void* View;
    Noesis::BaseRefCounted* DebugOverlay;
    Noesis::BaseRefCounted* ReloadingXaml;
    Noesis::BaseRefCounted* DirectlyOver;
    uint32_t field_90;
    DeferredCollectionsManager* DeferredCollectionsManager;
    DeferredPredicatesManager* DeferredPredicatesManager;
    DeferredPropertiesManager* DeferredPropertiesManager;
    BrushManager* BrushManager;
    ContextMenuManager* ContextMenuManager;
    ModManager* ModManager;
    TooltipManager* TooltipManager;
    uint32_t CurHeight_M;
    uint32_t ScreenWidth;
    uint32_t ScreenHeight;
    float ViewScale;
    double field_E0;
    int LastMouseX;
    int LastMouseY;
    __int64 field_F0;
    __int64 field_F8;
    UnsafeMessageQueue<void*> InputEvents;
    uint8_t MouseOverUI;
    uint8_t PointOverUI;
    uint8_t ControllerPointPosChanged;
    uint8_t gap183[5];
    float ControllerPointPos[2];
    float ViewMousePosition[2];
    CRITICAL_SECTION CS_;
    char Flags;
    uint8_t Flags2;
};

struct BrushManagerBase : public ProtectedGameObject<BrushManagerBase>
{
    void* VMT;
    void* TextureProvider;
    Noesis::BaseCollection* BrushesToRemove;
    CRITICAL_SECTION CS_;
};


struct BrushManager : public BrushManagerBase
{
    UnknownSignal qword40;
    UnknownSignal qword58;
    UnknownSignal qword70;
    void* TextureProvider2;
};

struct CommandProcessor : public ProtectedGameObject<CommandProcessor>
{
    CRITICAL_SECTION CS_;
    uint8_t Flags;
    Queue<void*> CommandQueue;
};

struct ModManager : public ProtectedGameObject<ModManager>
{
    void* RegisterCallback;
    Array<void*> Mods_Arr_uiModData;
};

struct WindowManager : public ProtectedGameObject<WindowManager>
{
    UnknownSignal FinishLoadWidgetSignal;
    UnknownSignal FinishLoadWidgetSignal2;
    UnknownSignal CancelOpenWidgetSignal;
    UnknownSignal CloseWidgetSignal;
    UnknownSignal field_60;
    CRITICAL_SECTION LoadXAMLCS;
    CRITICAL_SECTION LoadCS;
    Array<void*> PendingPreloads_pPreloadWidgetData;
    // TK only
    // Queue<void*> field_d8_pPreloadWidgetData;
    ModManager* ModManager;
    NoesisUIManager* NoesisUIManager;
    void* field_F8;
    TooltipManager* TooltipManager;
    ContextMenuManager* ContextMenuManager;
    StateMachine* StateMachine;
    Array<Noesis::Ptr<UIWidget>> Widgets;
    Array<Noesis::Ptr<UIWidget>> field_128;
    LegacyRefMap<FixedString, void*> field_138_Map_FS_Unk;
    uint8_t field_148;
    uint8_t field_149;
    void* field_150;
    input::InputManager* InputManager;
    CRITICAL_SECTION UnloadCS;
    HashMap<FixedString, void*> field_188_MHM_StateKey_DCAccum;
    Noesis::BaseCollection* field_1C8;
    Noesis::BaseCollection* field_1D0;
    Noesis::BaseCollection* field_1D8;
    Noesis::BaseCollection* field_1E0;
    Noesis::BaseCollection* field_1E8;
    Noesis::BaseCollection* field_1F0;
    Noesis::BaseCollection* DataContextStorage;
    HashMap<Guid, void*> field_200_Guid_StateWidgetsToRemove;
    HashMap<Guid, void*> field_240_Guid_WidgetToFinishLoadingData;
    UnsafeMessageQueue<void*> DelayedWidgetLoads_UnsafeMessageQueue;
};

struct StateMachine : public ProtectedGameObject<StateMachine>
{
    MessageBoxManager* MessageBoxManager;
    ModManager* ModManager;
    ContextMenuManager* ContextMenuManager;
    WindowManager* WindowManager;
    CRITICAL_SECTION CS_;
    Noesis::BaseRefCounted* qword48;
    UIStateMachine* StateMachineComponent;
    Noesis::BaseRefCounted* StoredStateMachineComponent;
    UnknownFunction ContextMenu_UpdateFromUI;
    Array<void*> LoadedMods_Arr_UIModData;
    bool ApplyModsRequested;
};

struct ViewModelProviderBase : public ProtectedGameObject<ViewModelProviderBase>
{
    void* VMT;
    ViewModel* VMGameData;
    HashMap<uint32_t, void*> Systems_MHM_u32_Ptr_uiSystem;
};

struct FlagHandler : public ProtectedGameObject<FlagHandler>
{
    HashMap<uint64_t, HashSet<EntityHandle>> DirtyHandles;
    HashMap<uint64_t, HashSet<EntityHandle>> NextFrameDirtyHandles;
};


struct ViewModelProvider : public ViewModelProviderBase
{
    void* field_50;
    void* ShroudEventListenerVMT;
    void* DefaultQuestListenerVMT;
    void* NetEventListenerVMT;
    void* CommunityAPIEventListenerVMT;
    char field_78[56];
    void* GameEventListenerVMT;
    UnknownSignal InitializeLocalPlayerSignal;
    UnknownSignal PlayerCombatNotificationSignal;
    UnknownSignal CharacterFTBEnteredSignal;
    UnknownSignal qword100;
    UnknownSignal SelectedCharacterChangedSignal;
    UnknownSignal qword130;
    UnknownSignal CharacterActivatedSignal;
    UnknownSignal CharacterDeactivatedSignal;
    UnknownSignal ActiveSlotChangedSignal;
    UnknownSignal qword190;
    UnknownSignal GameMenuOpenChangedSignal;
    UnknownSignal qword1C0;
    UnknownSignal InventoryOperationSignal;
    UnknownSignal qword1F0;
    UnknownSignal ItemRegisteredSignal;
    UnknownSignal UIInputMethodChangedSignal;
    UnknownSignal ConnectedSignal;
    uint16_t TargetGameObjectFlags;
    ecs::EntityRef TargetGameObject;
    void* TranslatedStringKeyManager;
    void* SpellPrototypeManager;
    void* ImmutableDataHeadmaster;
    void* ActionResourceTypes;
    input::InputManager* InputManager;
    void* ProgressionManager;
    void* ProgressionDescriptionManager;
    void* BoostPrototypeManager;
    void* ExternalCommunityAPI;
    void* PlayerManager;
    void* qword2C0;
    BrushManager* BrushManager;
    NoesisUIManager* NoesisUIManager;
    StateMachine* StateMachine;
    DragAndDropManager* DragAndDropManager;
    UnknownFunction field_2e8;
    HashMap<PlayerId, UnknownFunction> MHM_u16_SignalConnectionCollection;
    UnknownSignalSubscriber field_360[27];
    HashSet<EntityHandle> SummonsUpdated_MHS_EH;
    HashSet<EntityHandle> CharacterActivated_MHS_EH;
    HashSet<EntityHandle> field_C38_MHS_EH;
    HashSet<EntityHandle> ApprovalRatingChanged_MHS_EH;
    HashSet<EntityHandle> ShortRestAvailabilityChanged_MHS_EH;
    HashMap<EntityHandle, EntityHandle> UnregisterInventoryContainer_MHM_EH_EH;
    HashMap<PlayerId, EntityHandle> TargetGameObjects_MHM_short_EH;
    HashMap<EntityHandle, HashSet<ComponentHandle>> StatusUpdated_MHM_EH_HashSet_OH;
    HashMap<EntityHandle, HashSet<ComponentHandle>> StatusApplied_MHM_EH_HashSet_OH;
    HashMap<EntityHandle, HashSet<ComponentHandle>> StatusRemoved_MHM_EH_HashSet_OH;
    HashMap<EntityHandle, HashSet<ComponentHandle>> UpdatedStatusLifetimes_MHM_EH_HashSet_OH;
    HashMap<EntityHandle, HashSet<ComponentHandle>> UpdatedAddedStatuses_MHM_EH_HashSet_OH;
    HashMap<EntityHandle, HashSet<ComponentHandle>> UpdatedRemovedStatuses_MHM_EH_HashSet_OH;
    HashMap<PlayerId, void*> DelayDialogueRequestSwitchCharacter_MHM_short_CharacterSwapDelayData;
    FlagHandler DirtyFlags;
    uint64_t qwordF88;
    uint8_t Flags;
    uint8_t Flags2;
    HashMap<UserId, float> RemotePlayerFloats_MHM_u32_float;
    HashSet<UserId> RemotePlayerUserIds_MHS_u32;
    HashMap<PlayerId, uint8_t> DeferredSetContainerState_MHM_short_EContainerState;
    HashMap<EntityHandle, uint8_t> LevelLoad_MHM_EH_u8;
    HashMap<PlayerId, bool> GameMenuOpen_MHM_short_bool;
    HashMap<PlayerId, void*> CursorInfo_MHM_short_eclTaskInfo;
    HashSet<PlayerId> CursorInfo2_MHS_short;
    HashSet<PlayerId> PreviewTaskChanged_MHS_short;
    struct ecs::EntityWorld* EntityWorld;
    void* Thoth;
    Array<UnknownFunction> SystemUpdate_Arr_Function;
    HashSet<EntityHandle> OwnedCharacterActivated_MHS_EH;
    HashMap<EntityHandle, HashSet<SpellId>> field_11B8_MHM_EH_HashSet_SpellId;
    uint32_t GameStateID;
    void* ShroudTexture;
    int8_t RequestSetControllerType;
    HashMap<PlayerId, Array<EntityHandle>> DelayedShowActiveSearch_MHM_short_Arr_EH;
    Array<EntityHandle> DelayedInventoryRefresh;
    Array<void*> TimelineEvents_Arr_TTimelineEvent;
    HashMap<PlayerId, Array<EntityHandle>> ActiveRollOpen_MHM_short_MHS_EH;
    Array<EntityHandle> SetActiveDialogue;
    DeferredPredicatesManager* DeferredPredicatesManager;
    Array<PlayerId> ShroudReload;
};

struct ContextMenuManagerBase : public ProtectedGameObject<ContextMenuManagerBase>
{
    void* VMT;
    HashMap<PlayerId, void*> Menus_MHM_u16_PlayerContextMenu;
};

struct ContextMenuManager : public ContextMenuManagerBase
{
    TooltipManager* TooltipManager;
    MPMCQueueBounded<PlayerId> OPOCCircularQueue_short;
};

struct DataContextProviderBase : public ProtectedGameObject<DataContextProviderBase>
{
    void* VMT;
    DCWidget* GlobalDataContext;
    LegacyRefMap<uint16_t, HashMap<FixedString, Noesis::Ptr<DCWidget>*>> CustomDC_RefMap_u16_MHM_FS_Ptr_DCWidget;
    StateMachine* StateMachine;
    ViewModelProvider* ViewModelProvider;
    Noesis::BaseCollection* DataContextToInit;
    Noesis::BaseCollection* DataContextToInitFromUI;
    Noesis::BaseCollection* DataContextToRelease;
    Noesis::BaseCollection* qword48;
    CRITICAL_SECTION CS_;
};


struct DataContextProvider : public DataContextProviderBase
{
    DragAndDropManager* DragAndDropManager;
    DCWidget* GlobalDataContext2;
    BrushManager* BrushManager;
    ModManager* ModManager;
    HashMap<FixedString, Noesis::Ptr<Noesis::BaseCollection>> qword90_MHM_FS_Ptr_Collection_DCWidget;
    MPMCQueueBounded<FixedString> InitDataContexts_OPOCCircularQueue_FS;
    uint8_t byteF0;
};

struct DragAndDropManager : public ProtectedGameObject<DragAndDropManager>
{
    UnknownSignal qword0;
    NoesisUIManager* NoesisUIManager;
    ViewModelProvider* ViewModelProvider;
    void* eclDragDropManager;
    void* qword30;
    void* qword38;
    void* qword40;
    void* qword48;
    void* qword50;
    Array<PlayerId> qword58;
    Array<PlayerId> qword68;
    bool CanExecuteDragEvents;
    bool NeedsUpdate;
    uint8_t byte7A;
    MPMCQueueBounded<PlayerId> field_80_OPOCCircularQueue_i16;
    HashMap<PlayerId, uint64_t> field_a0_MHM_u16_u64;
    HashMap<PlayerId, uint64_t> field_e0_MHM_u16_u64;
    Array<void*> DelayedDragEvents_Arr_TDelayedDragEvent[4];
    MPMCQueueBounded<void*> field_160_Queue_TStartDraggingRequest;
};

struct TooltipManager : public ProtectedGameObject<TooltipManager>
{
    void* VMT;
    UnknownFunction SignalCollection;
    void* qword40;
    NoesisUIManager* NoesisUIManager;
    ViewModelProvider* ViewModelProvider;
    DragAndDropManager* DragAndDropManager;
    StateMachine* StateMachine;
    Array<void*> field_70_MHM_i16_pPlayerTooltipData;
    uint16_t wordB0;
    HashMap<PlayerId, void*> field_b8_MHM_i16_DelayedTooltipData;
    HashSet<PlayerId> field_f8_MHS_i16;
    HashSet<PlayerId> field_128_HashSet_u16;
    void* qword158;
    MPMCQueueBounded<std::pair<short, bool>> field_160_Queue_Pair_short_bool;
    MPMCQueueBounded<std::pair<short, bool>> field_180_Queue_Pair_short_bool;
    bool Initialized;
    bool field_1A1;
    bool byte1A2;
    MPMCQueueBounded<PlayerId> field_1A8_Queue_i16;
    MPMCQueueBounded<PlayerId> field_1c8_Queue_i16;
    MPMCQueueBounded<PlayerId> field_1e8_Queue_i16;
    MPMCQueueBounded<PlayerId> field_208_Queue_i16;
    MPMCQueueBounded<std::pair<short, bool>> field_228_Queue_Pair_short_bool;
};

struct MessageBoxManager : public ProtectedGameObject<MessageBoxManager>
{
    void* VMT;
    void* VMT2;
    BYTE word10;
    char field_11;
    TranslatedString field_14;
    int field_24;
    Noesis::BaseCollection* field_28;
    void* MessageBoxDC;
    Noesis::BaseComponent* MessageBox;
    Noesis::BaseRefCounted* field_40;
    uint8_t gap48[32];
    StateMachine* StateMachine;
    CommandProcessor* CommandProcessor;
    DataContextProvider* DataContextProvider;
    ViewModelProvider* ViewModelProvider;
    Array<void*> field_88_Arr_PendingMessageBox;
};

struct UINotificationManager : public ProtectedGameObject<UINotificationManager>
{
    void* VMT;
    UnknownFunction SignalCollection;
    void* SoundManager;
    void* BrushManager;
    ViewModelProvider* ViewModelProvider;
    void* PlayerManager;
    void* TutorialManager;
    void* TranslatedStringKeyManager;
    void* BackgroundManager;
    void* BackgroundGoals;
    void* ExperienceRewards;
    void* SpellPrototypeManager;
    HashSet<EntityHandle> field_98;
};

struct TwitchOverlayManager : public ProtectedGameObject<TwitchOverlayManager>
{
    uint64_t RequestTime;
    void* qword8;
    uint16_t word10;
    __int64 field_18;
    __int64 field_20;
    __int64 field_28;
    __int64 field_30;
    __int64 field_38;
    __int64 field_40;
    STDString ls__stdstring48;
    ViewModelProvider* ViewModelProvider;
    void* JsonConverter;
    HashSet<uint64_t> field_70_MHS_u64;
    void* qwordA0;
    uint64_t RequestTimeDeadline;
    STDString TwitchExtBaseURL;
    STDString TwitchExtToken;
    STDString TwitchExtSecret;
    CRITICAL_SECTION CS_;
    int net__defaulthttprequestmanager120;
    EntityHandle qword128;
    EntityHandle HttpRequest;
};

struct TutorialManager : public ProtectedGameObject<TutorialManager>
{
    ViewModelProvider* ViewModelProvider;
    StateMachine* StateMachine;
    UnknownFunction SignalCollection;
    Array<PlayerId> field_50_Arr_i16;
    Array<void*> field_60_Arr_HideRequest;
    Array<void*> field_70_Arr_ShowRequest;
    uint8_t byte80;
    Array<void*> field_88_Arr_ActiveTutorial;
};


struct GameUI : public ProtectedGameObject<GameUI>
{
    void* VMT;
    void* InputEventListenerVMT;
    void* PlayerEventListenerVMT;
    uint64_t field_18;
    void* GameStateEventListenerVMT;
    uint64_t ls__eventlistenerbase28;
    void* GameEventManagerListenerVMT;
    UnknownSignal SplitScreenChangedSignal;
    GameUIParams GameUIParams;
    NoesisUIManager NoesisUIManager;
    BrushManager BrushManager;
    alignas(64) uint16_t field_380;
    MPMCQueueBounded<void*> CommandQueue;
    ModManager ModManager;
    __int64 field_3B0;
    __int64 field_3B8;
    WindowManager WindowManager;
    StateMachine StateMachine;
    ViewModelProvider ViewModelProvider;
    ContextMenuManager ContextMenuManager;
    DataContextProvider DataContextProvider;
    DragAndDropManager DragAndDropManager;
    TooltipManager TooltipManager;
    MessageBoxManager MessageBoxManager;
    UINotificationManager NotificationManager;
    TwitchOverlayManager TwitchOverlayManager;
    TutorialManager TutorialManager;
    DeferredPropertiesManager* DeferredPropertiesManager;
    __int64 field_22C0;
    void* SplitscreenRequestSystem;
    int field_22D0;
    int field_22D4;
    void* TextureProvider;
    __int64 field_22E0;
    __int64 field_22E8;
    __int64 field_22F0;
    __int64 field_22F8;
    Array<void*> field_2300_OpenTradeRequest;
    Array<void*> field_2310_OpenActiveRollRequest;
    Array<void*> field_2320_OpenRewardRequest;
    Array<void*> field_2330_OpenMakeCampRequest;
    HashMap<PlayerId, void*> field_2340_MHM_i16_ShowDialogueUIRequest;
    HashSet<PlayerId> field_2380_MHS_i16;
    Array<void*> field_23b0_TimelineTransitionRequest;
    Array<EntityHandle> field_23c0_CharacterCreationPlayers;
    HashMap<PlayerId, HashSet<EntityHandle>> field_23d0_MHM_u16_MHS_EH;
    HashSet<PlayerId> field_2410_i16;
    UnknownFunction field_2440_SignalConnectionCollection;
    int64_t SetControllerModeSubscriberId;
    int64_t OnWidgetFinishedLoadingSubscriberId;
    HashMap<PlayerId, uint64_t> field_2490_u16_u64;
    uint32_t DeviceType2;
    uint8_t ControllerType;
    uint8_t gap24D5[3];
    uint32_t DeviceType;
    uint8_t InitState;
    uint8_t Flags_byte24DD;
    uint8_t byte24DE;
    uint8_t dword24DF;
    uint8_t field_24E0;
    uint8_t field_24E1;
    uint8_t DisableUpdates;
    uint8_t byte24E3;
    void* GameControl;
    void* PlayerManager;
    uint32_t dword24F8;
    uint8_t ReloadState;
    HashSet<EntityHandle> field_2500_EH_Timelines;
    HashSet<uint32_t> field_2530_u32;
    void* NoesisGUI;
    uint64_t field_2E20;
    uint32_t field_2E28;
    uint32_t field_2E2C;
    const char* field_2E30;
    const char* field_2580;
    uint64_t field_2E40;
    uint32_t field_2E48;
    const char* field_2E50;
    const char* field_2E58;
    const char* field_2E60;
    uint64_t field_2E68;
    uint32_t field_2E70;
    const char* field_2e78;
    const char* field_2e80;
    const char* field_2e88;
    const char* field_2e90;
    const char* field_2e98;
    const char* field_2ea0;
    HashMap<uint32_t, void*> field_25f0_MHM_u32_pUiSystem[4];
    Array<UIEvent> QueuedEvents;
    Array<void*> field_2700_TDelayedOpenCombine;
    Array<input::FireEventDesc> QueuedFireEvents;
    Array<input::InputEvent> QueuedInputEvents;
    CRITICAL_SECTION EventsCS_;
    CRITICAL_SECTION UpdateCS_;
    CRITICAL_SECTION RenderCS_;
    CRITICAL_SECTION RenderOffscreenWithoutUpdatingCS_;
    Array<PlayerId> AddLocalPlayers;
    Array<PlayerId> InitializeLocalPlayers;
    HashMap<PlayerId, void*> field_27f0_MHM_short_OpenActiveRollWidgetEventDelayedData;
    UnsafeMessageQueue<void*> qword2840_UnsafeMessageQueue_WidgetClosedData;
    double GameTime;
    float ScaledTickDelta;
    float TickDelta;
    uint32_t Ticks;
    uint32_t gap28D4;
    uint32_t dword28D8;
    uint32_t gap28DC;
    Array<void*> qword28E0_Arr_EGameStateID;
    int RenderOffscreenRetries;
    int LastTicks;
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
    NoesisUIManager* UIManager70;
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
    Noesis::Delegate<void ()> UnknownDelegate;
};

struct DCWidget : public ViewModel
{
    CommandProcessor CommandProcessor;
    DeferredProperty<Noesis::String> NameProperty;
    DeferredProperty<uint8_t> LayoutProperty;
    DeferredProperty<bool> IsInitializedProperty;
    UnknownSignal field_E8;
    StateMachine* StateMachine;
    PlayerId PlayerId;
    uint8_t Owner;
    DeferredCommand* DeferredCommand1;
    DeferredCommand* DeferredCommand2;
    Noesis::Delegate<void ()> NotifyWidgetClosing;
    ViewModelProvider* ViewModelProvider;
};

struct CustomPropertyDefn
{
    FixedString Name;
    FixedString Type;
    bool Notify{ true };
    lua::PersistentRegistryEntry WriteCallback;
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
    FOR_NOESIS_TYPE(Noesis::LuaDelegateCommand) \
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
