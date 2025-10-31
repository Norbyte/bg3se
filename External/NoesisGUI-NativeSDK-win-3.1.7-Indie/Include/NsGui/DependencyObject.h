////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DEPENDENCYOBJECT_H__
#define __GUI_DEPENDENCYOBJECT_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/CompilerTools.h>
#include <NsCore/Delegate.h>
#include <NsCore/HashMap.h>
#include <NsCore/String.h>
#include <NsGui/IComponentInitializer.h>
#include <NsGui/DispatcherObject.h>
#include <NsGui/DependencySystemApi.h>
#include <NsGui/DependencyObjectValueData.h>
#include <NsGui/ValueDestination.h>
#include <NsGui/FreezableEventReason.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
/// The following macro should be used by DependencyObject derived classes inside their constructors
/// when they need to set dependency properties. This way derived classes ensure that dependency
/// property identifiers are properly created.
////////////////////////////////////////////////////////////////////////////////////////////////////
#define ForceCreateDependencyProperties Noesis::TypeOf<SelfClass>


namespace Noesis
{

class DependencyObject;
class DependencyProperty;
class PropertyMetadata;
class Freezable;
struct ProviderValue;
struct DependencyPropertyChangedEventArgs;
class Expression;
template<class T> class ValueStorageManagerImpl;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Param type for DependencyObject::SetValue<T>
//
//      Property Type  | Param Type
//      ---------------------------------------
//      Ptr<T>           T*
//      String         const char*
//      T                T (basic types) or T&
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct SetValueType
{
    typedef typename Param<T>::Type Type;
};
template<> struct SetValueType<String>
{
    typedef const char* Type;
};
template<class T> struct SetValueType<Ptr<T>>
{
    typedef T* Type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// DependencyPropertyChangedEventArgs. Args passed on property changed event notification.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct DependencyPropertyChangedEventArgs
{
    const DependencyProperty* prop;
    const void* oldValue;
    const void* newValue;

    template<class T> const T& OldValue() const;
    template<class T> const T& NewValue() const;

private:
    friend class DependencyObject;
    friend class FrameworkElement;
    friend class BindingExpression;
    friend class ShaderEffect;

    DependencyPropertyChangedEventArgs(const DependencyProperty* dp, const void* oldValue,
        const void* newValue, const PropertyMetadata* metadata);

private:
    const PropertyMetadata* metadata;
};

typedef Delegate<void (BaseComponent*, const DependencyPropertyChangedEventArgs&)>
    DependencyPropertyChangedEventHandler;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an object that participates in the dependency property system.
///
/// The DependencyObject class enables the property system services on its many derived classes.
/// The property system's primary function is to compute the values of properties, and to provide
/// system notification about values that have changed. Another key class that participates in the
/// property system is DependencyProperty. DependencyProperty enables the registration of
/// dependency properties into the property system, and provides identification and information
/// about each dependency property, whereas DependencyObject as a base class enables objects to use
/// the dependency properties.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.dependencyobject.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_DEPENDENCYSYSTEM_API DependencyObject: public DispatcherObject,
    public IComponentInitializer
{
public:
    DependencyObject();
    DependencyObject(const DependencyObject&) = delete;
    DependencyObject& operator=(const DependencyObject&) = delete;
    virtual ~DependencyObject() = 0;

    /// Gets the provider that returns the effective value for the specified dependency property
    uint8_t GetValueProvider(const DependencyProperty* dp) const;

    /// Returns the current effective value of a dependency property on this instance of a
    /// DependencyObject
    template<class T>
    const T& GetValue(const DependencyProperty* dp) const;

    /// Returns the current effective value of a dependency property on this instance of a
    /// DependencyObject as a boxed value
    Ptr<BaseComponent> GetValueObject(const DependencyProperty* dp) const;

    /// Returns the local value of a dependency property, if it exists
    Ptr<BaseComponent> GetLocalValue(const DependencyProperty* dp) const;
    
    /// Returns the base value without animation nor coerce (this never returns Expression like
    /// GetLocalValue)
    template<class T>
    const T& GetBaseValue(const DependencyProperty* dp) const;

    /// Gets the expression, if any, used to evaluate the specified property value
    Expression* GetExpression(const DependencyProperty* dp) const;

    /// Sets the local value of a dependency property
    /// \remarks Once set, local value is only removed when calling ClearValue()
    template<class T>
    void SetValue(const DependencyProperty* dp, typename SetValueType<T>::Type value);

    /// Sets the local value (boxed) of a dependency property
    void SetValueObject(const DependencyProperty* dp, const Ptr<BaseComponent>& value);

    /// Sets the current value of a dependency property. The current value is set on the coerce
    /// field, without modifying source or animated value
    template<class T>
    void SetCurrentValue(const DependencyProperty* dp, typename SetValueType<T>::Type value);

    /// Sets the current value of a dependency property using a boxed value
    void SetCurrentValueObject(const DependencyProperty* dp, const Ptr<BaseComponent>& value);

    /// Sets the expression to be evaluated dynamically to obtain the value of the property
    void SetExpression(const DependencyProperty* dp, Expression* expression);

    /// Clears the local value of a property
    /// The property to be cleared is specified by a DependencyProperty identifier
    void ClearLocalValue(const DependencyProperty* dp);

    /// Re-evaluates the effective value for the specified dependency property if necessary
    /// If null is passed, a full re-evaluation could be needed
    void InvalidateProperty(const DependencyProperty* dp, uint8_t priority = 1);

    /// Sets the animated value of a property
    template<class T>
    void SetAnimation(const DependencyProperty* dp, typename SetValueType<T>::Type value);
    void SetAnimation(const DependencyProperty* dp, const Ptr<BaseComponent>& value);

    /// Clears the animation value of a property
    template<class T>
    void ClearAnimation(const DependencyProperty* dp);
    void ClearAnimation(const DependencyProperty* dp);

    /// Coerces and validates the effective property value
    template<class T>
    void CoerceValue(const DependencyProperty* dp);
    void CoerceValue(const DependencyProperty* dp);

    /// Returns if the value is stored in the cache. If true, the priority is returned in the 
    /// provider field
    bool IsCached(const DependencyProperty* dp, uint8_t* provider) const;

    /// Gets a value that indicates whether this instance is currently sealed (read-only)
    bool IsSealed() const;

    /// Returns true if there is any animated property
    bool HasAnimatedProperties() const;

    /// Returns the PropertyChanged event
    inline DependencyPropertyChangedEventHandler& DependencyPropertyChanged();

    typedef Delegate<void(DependencyObject*)> DestroyedDelegate;

    /// Destroyed delegate is raised when object is going to be destroyed
    inline DestroyedDelegate& Destroyed();

    /// From IComponentInitializer
    //@{
    bool IsInitialized() const override;
    void Init() override;
    void BeginInit() override;
    void EndInit() override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

protected:
    // Calls Destroyed delegate before destroying the object
    int32_t OnDestroy() override;

    // Indicates whether the property is set to its default value
    bool IsDefaultValue(const DependencyProperty* dp) const;

    // Sealed objects cannot be modified
    void Seal();

    // Called to initialize inheritors
    virtual void OnInit();

    // Called when initialization process is completed
    virtual void OnPostInit();

    // Called to allow descendants to manage a property value change
    // \return True when it has been processed, otherwise false
    virtual bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);

    // Called to allow descendants to manage a sub property value change
    // \return True when it has been processed, otherwise false
    virtual bool OnSubPropertyChanged(const DependencyProperty* dp);

    // Called to allow descendants to manage a component value change
    virtual void OnObjectValueSet(BaseComponent* oldValue, BaseComponent* newValue);

    // Called when uncached properties are locally set
    virtual void OnUncachedPropertySet(const DependencyProperty* dp);

    // Gets the value of a dependency property from the active provider
    //@{
    virtual ProviderValue GetProviderValue(const DependencyProperty* dp) const;
    virtual ProviderValue GetNonCachedProviderValue(const DependencyProperty* dp) const;
    virtual uint8_t GetNonCachedValueProvider(const DependencyProperty* dp) const;
    //@}

    template<class T>
    void SetReadOnlyProperty(const DependencyProperty* dp,
        typename SetValueType<T>::Type value) const;

    void ClearReadOnlyProperty(const DependencyProperty* dp);

    void SetReadOnlyExpression(const DependencyProperty* dp, Expression* expr) const;

    void InternalSetExpression(const DependencyProperty* dp, Expression* newExpression,
        uint8_t priority);

public:
    // Friend classes and functions
    //@{
    friend class ChangedHandler;
    friend class DependencyObjectTestHelper;
    friend struct BindingOperations;
    friend class BindingExpression;
    friend class VisualTreeInspectorHelper;
    friend class XamlContext;
    template<class T> friend class DependencyPropertyImpl;
    template<class T> friend class ValueStorageManagerImpl;
    //@}

    /// Hash of stored properties
    typedef HashMap<const DependencyProperty*, StoredValue*> Values;
    mutable Values mValues;

public:
    bool CheckReadOnly(const DependencyProperty* dp) const;
    bool CheckTypes(const Type* valueType, const DependencyProperty* dp) const;
    bool CheckTypes(const Ptr<BaseComponent>& value, const DependencyProperty* dp) const;
    StoredValue* ValidateAndGetEntry(const void *value, const DependencyProperty *dp,
        bool& justCreated);

    // SetValue helpers
    //@{
    typedef Int2Type<0> IsNotBaseComponent;
    typedef Int2Type<1> IsBaseComponent;
    typedef Int2Type<2> IsString;

    template<class T>
    void SetValue_(IsNotBaseComponent, const DependencyProperty* dp,
        typename Param<T>::Type value,
        Value::Destination destination = Value::Destination_BaseValue);

    template<class T>
    void SetValue_(IsBaseComponent, const DependencyProperty* dp, typename T::Type* value,
        Value::Destination destination = Value::Destination_BaseValue);

    template<class T>
    void SetValue_(IsString, const DependencyProperty* dp,
        const char* value, Value::Destination destination = Value::Destination_BaseValue);
    //@}

    void InternalSetValue(const DependencyProperty* dp, void* oldValue, const void* newValue,
        void* coercedValue, uint8_t priority, Expression* newExpression,
        const PropertyMetadata* metadata, Value::Destination destination, bool isBaseComponent);
    void InternalClearAnimation(const DependencyProperty* dp, void* oldValue,
        void* coercedValue, void* baseValue, bool isBaseComponent);
    void InternalInvalidateProperty(const DependencyProperty* dp, uint8_t priority);
    const void* InternalGetUncachedValue(const DependencyProperty* dp,
        const void* defaultValue, const PropertyMetadata* metadata) const;
    const void* InternalGetValue(const DependencyProperty* dp, uint32_t sizeOfT) const;
    const void* InternalGetValue(const DependencyProperty* dp, uint32_t sizeOfT,
        const PropertyMetadata* metadata) const;
    const void* InternalGetBaseValue(const DependencyProperty* dp, uint32_t sizeOfT) const;

    // Implementation of coerce value
    template<class T>
    void InternalCoerceValue(const DependencyProperty* dp, StoredValue* storedValue,
        const void* defaultValue, const PropertyMetadata* metadata);
    void InternalCoerceValue(const DependencyProperty* dp, StoredValue* sv,
        const void* defaultValue, void* oldValue, void* coercedValue,
        const PropertyMetadata* metadata, bool isBaseComponent);
    void InternalCoerceValue(const DependencyProperty* dp, void* oldValue, void* coercedValue,
        bool isBaseComponent);
    const void* InternalGetValue(const DependencyProperty* dp, StoredValue* storedValue,
        const void* defaultValue, uint32_t sizeOfT, const PropertyMetadata* metadata);

    // Management of properties that contain Freezable objects
    //@{
    void AddChangedHandler(const DependencyProperty* dp, Freezable* freezable);
    void RemoveChangedHandler(const DependencyProperty* dp);
    //@}

    // Launches OnPropertyChange when value changes by a SetValue or ClearValue
    void NotifyPropertyChanged(const DependencyProperty* dp, StoredValue* storedValue,
        const void* oldValue, const void* newValue, bool valueChanged, bool isBaseComponent,
        const PropertyMetadata* metadata);

private:
    uint32_t mFlags;

    class ChangedHandler
    {
    public:
        ChangedHandler() = default;
        ChangedHandler(const ChangedHandler& handler) = delete;
        ChangedHandler& operator=(ChangedHandler&) = delete;
        ChangedHandler(ChangedHandler&& handler);

        void OnChanged(Freezable* freezable, FreezableEventReason reason);
        void Attach(Freezable* object, DependencyObject* owner, const DependencyProperty* prop);
        void Detach();

    private:
        Freezable* mFreezable = nullptr;
        DependencyObject* mOwner = nullptr;
        const DependencyProperty* mOwnerProperty = nullptr;
    };

    /// List of handlers attached to property objects to monitor changes
    /// TODO [srodriguez] Maybe this can be a simple std::set because the value is already stored
    /// in the Values map
    typedef HashMap<const DependencyProperty*, ChangedHandler> ChangedHandlers;
    ChangedHandlers mChangedHandlers;

    /// Dependency property changed event
    DependencyPropertyChangedEventHandler mDependencyPropertyChangedEvent;

    //@{
    DestroyedDelegate mDestroyedDelegate;
    //@}

    NS_DECLARE_REFLECTION(DependencyObject, DispatcherObject)
};

NS_WARNING_POP

}

#include <NsGui/DependencyObject.inl>

#endif
