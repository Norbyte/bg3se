////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DEPENDENCYPROPERTY_H__
#define __GUI_DEPENDENCYPROPERTY_H__


#include <NsCore/Noesis.h>
#include <NsGui/DependencySystemApi.h>
#include <NsGui/ValueDestination.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/HashMap.h>
#include <NsCore/Delegate.h>
#include <NsCore/Symbol.h>


namespace Noesis
{

class Type;
class TypeProperty;
class DependencyObject;
class PropertyMetadata;
class ValueStorageManager;
class DependencyPropertyTest;
template<class T> class Ptr;
struct StoredValue;
class Expression;

////////////////////////////////////////////////////////////////////////////////////////////////////
enum PropertyAccess
{
    PropertyAccess_ReadWrite,
    PropertyAccess_ReadOnly
};

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef bool (*ValidateValueCallback)(const void* value);
typedef void* ValueStorage;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a dependency property that is registered in the property system.
/// Dependency properties provide support for value expressions, property invalidation and
/// dependent-value coercion, default values, inheritance, data binding, animation, property change
/// notification, and styling.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_DEPENDENCYSYSTEM_API DependencyProperty: public BaseComponent
{
public:
    template<class T>
    static Ptr<DependencyProperty> Create(const char* name);
    template<class T>
    static Ptr<DependencyProperty> Create(const char* name, const Type* type);
    template<class T>
    static Ptr<DependencyProperty> Create(const char* name, const TypeClass* ownerType,
        PropertyMetadata* metadata, ValidateValueCallback validate,
        PropertyAccess access = PropertyAccess_ReadWrite);
    template<class T>
    static Ptr<DependencyProperty> Create(const char* name, const Type* type,
        const TypeClass* ownerType, PropertyMetadata* metadata, ValidateValueCallback validate,
        PropertyAccess access = PropertyAccess_ReadWrite);

    virtual ~DependencyProperty();

    /// Returns a static value that is used by the property system rather than a null Ptr to 
    /// indicate that the property exists, but does not have its value set by the property system.
    static BaseComponent* GetUnsetValue();

    /// Gets property name
    inline Symbol GetName() const;

    /// Gets property type
    inline const Type* GetType() const;

    /// Gets property owner type
    inline const TypeClass* GetOwnerType() const;

    /// Gets property default value for the specified type
    inline const void* GetDefaultValue(const TypeClass* forType) const;

    /// Gets property default value as a boxed object for the specified type
    inline Ptr<BaseComponent> GetDefaultValueObject(const TypeClass* forType) const;

    /// Checks if property is read-only
    inline bool IsReadOnly() const;
    
    /// Returns size of the property type
    inline uint32_t GetSize() const;

    /// Gets the callback used to validate property value
    inline ValidateValueCallback GetValidateValueCallback() const;

    /// Gets the object that manages value storage for this property
    inline ValueStorageManager* GetValueManager() const;

    /// Gets property metaData
    const PropertyMetadata* GetMetadata(const TypeClass* forType) const;

    /// Uses dependency property type information to override its metadata
    inline void OverrideMetadata(const TypeClass* forType, PropertyMetadata* metadata) const;

    /// Looks for a reflection property in the owner type of this dependency property with the same
    /// name. If not found, null is returned
    inline const TypeProperty* FindTypeProperty() const;

    /// Finds a metaData in the corresponding reflection property
    /// Returns metaData of the specified type if found, otherwise it returns null
    template<class T>
    const T* FindTypeMetaData() const;

    /// Compares the value of this property in the specified dependency object with
    /// the supplied value and returns true if they are the same
    inline bool IsSameValue(const DependencyObject* obj,
        const Ptr<BaseComponent>& value) const;

    /// Compares 2 values and returns true when they are the same value
    inline bool IsSameValue(const void* left, const Ptr<BaseComponent>& right) const;

    /// Compares 2 values and returns true when they are the same value
    inline bool IsSameValue(const void* left, const void* right) const;

    /// Indicates if the value type is valid for this property
    inline bool CheckType(const Type* valueType) const;

    /// Indicates if the boxed value is valid for this property type
    inline bool CheckType(const Ptr<BaseComponent>& value) const;

    /// Gets the raw value of the property from the specified dependency object
    inline const void* GetValue(const DependencyObject* obj) const;

    /// Gets the a boxed value or Ptr object for this property from a raw pointer
    inline Ptr<BaseComponent> GetValueObject(const void* value) const;

    /// Gets the boxed value of this property in the given dependency object
    inline Ptr<BaseComponent> GetValueObject(const DependencyObject* obj) const;

    /// Gets the boxed baseValue of this property in the given dependency object
    inline Ptr<BaseComponent> GetBaseValueObject(const DependencyObject* obj) const;

    /// Sets a new value for this property with in the specified destination
    inline void SetValue(DependencyObject* obj, const void* value, uint8_t priority,
        Expression* expression, const PropertyMetadata* metadata,
        Value::Destination destination = Value::Destination_BaseValue) const;

    /// Sets a new value (boxed) for this property with in the specified destination
    inline void SetValueObject(DependencyObject* obj, BaseComponent* value,
        uint8_t priority, Expression* expression, const PropertyMetadata* metadata,
        Value::Destination destination = Value::Destination_BaseValue) const;

    /// Clears animation value for this property
    inline void ClearAnimation(DependencyObject* obj) const;

    // Coerces and validates the effective value for this property
    inline void CoerceValue(DependencyObject* obj, StoredValue* storedValue,
        const void* defaultValue, const PropertyMetadata* metadata) const;

    /// Indicates if the specified value is valid for this property. The value is verifyed against
    /// property type and property validate callback
    bool IsValidValue(const Ptr<BaseComponent>& value) const;

    /// Public methods to change internal values. Are used from DependencyData
    //@{
    void SetOwnerType(const TypeClass* owner);
    void SetIsReadOnly(bool value);
    void AddMetadata(const PropertyMetadata* metaData);
    void AddMetadata(const TypeClass* type, const PropertyMetadata* metaData);
    void RemoveMetadata(const TypeClass* forType);
    void SetValidateValueCallback(ValidateValueCallback validate);
    //@}

protected:
    /// Constructors
    //@{
    DependencyProperty(Symbol name, ValueStorageManager* valueManager, const Type* type);
    DependencyProperty(Symbol name, ValueStorageManager* valueManager, const Type* type,
        const TypeClass* ownerType, PropertyMetadata* metadata, ValidateValueCallback validate,
        PropertyAccess access);
    //@}

private:
    friend class DependencyObject;
    friend class DependencyObjectTestHelper;
    friend class Freezable;
    friend class DependencyPropertyTest;

    // Indicates if value is valid for this property according to the validate callback
    bool ValidateValue(const void* value) const;

    void MergeMetaData(PropertyMetadata* metadata, const TypeClass* type);

private:
    Symbol mName;
    ValueStorageManager* mValueManager;
    const Type* mType;
    const TypeClass* mOwnerType;
    ValidateValueCallback mValidate;
    bool mIsReadOnly;

    typedef HashMap<const TypeClass*, Ptr<const PropertyMetadata>> MetaDatas;
    mutable MetaDatas mMetaDatas;

    NS_DECLARE_REFLECTION(DependencyProperty, BaseComponent)
};

NS_WARNING_POP

/// Search for dependency properties in the class hierarchy
NS_GUI_DEPENDENCYSYSTEM_API const DependencyProperty* FindDependencyProperty(
    const TypeClass* typeClass, Symbol propId);

/// Indicates if the specified property is of type BaseComponent
NS_GUI_DEPENDENCYSYSTEM_API bool IsBaseComponentProperty(const DependencyProperty* dp);

}

#include <NsGui/DependencyProperty.inl>

#endif
