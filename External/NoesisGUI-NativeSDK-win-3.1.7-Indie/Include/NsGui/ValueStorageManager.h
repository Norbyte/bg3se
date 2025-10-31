////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VALUESTORAGEMANAGER_H__
#define __GUI_VALUESTORAGEMANAGER_H__


#include <NsCore/Noesis.h>
#include <NsGui/DependencySystemApi.h>
#include <NsGui/ValueDestination.h>


namespace Noesis
{

class Type;
class TypeClass;
class BaseComponent;
class DependencyObject;
class DependencyProperty;
class DependencyData;
class PropertyMetadata;
template<class T> class Ptr;
struct StoredValue;
class Expression;


////////////////////////////////////////////////////////////////////////////////////////////////////
typedef bool (*ValidateValueCallback)(const void* value);
typedef void* ValueStorage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Creates, destroys and assigns values to a ValueStorage.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_DEPENDENCYSYSTEM_API ValueStorageManager
{
public:
    virtual ~ValueStorageManager();

    virtual ValueStorageManager* Clone() const = 0;
    virtual uint32_t Size() const = 0;
    virtual bool CheckType(const Type* type) const = 0;
    virtual bool CheckType(const Type* type, BaseComponent* value) const = 0;
    virtual bool Validate(ValidateValueCallback validate, BaseComponent* value) const = 0;
    virtual void Construct(ValueStorage* storage, const void* value) const = 0;
    virtual void Destroy(ValueStorage* storage) const = 0;
    virtual void* ToValue(const ValueStorage* storage) const = 0;
    virtual void Copy(void* dst, const void* value) const = 0;
    virtual void Copy(void* dst, BaseComponent* value) const = 0;
    virtual Ptr<BaseComponent> Box(const void* value) const = 0;
    virtual const void* GetValue(const DependencyObject* dob,
        const DependencyProperty* dp) const = 0;
    virtual const void* GetBaseValue(const DependencyObject* dob,
        const DependencyProperty* dp) const = 0;
    virtual bool IsSame(const void* left, const void* right) const = 0;
    virtual bool IsSame(const void* left, BaseComponent* right) const = 0;
    virtual void SetValue(DependencyObject* dob, const DependencyProperty* dp, const void* value,
        uint8_t priority, Expression* expression, const PropertyMetadata* metadata,
        Value::Destination destination) const = 0;
    void SetValueObject(DependencyObject* dob, const DependencyProperty* dp,
        BaseComponent* value, uint8_t priority, Expression* expression,
        const PropertyMetadata* metadata, Value::Destination destination) const;
    virtual void ClearAnimation(DependencyObject* dob, const DependencyProperty* dp) const = 0;
    virtual void CoerceValue(DependencyObject* dob, const DependencyProperty* dp,
        StoredValue* storedValue, const void* defaultValue,
        const PropertyMetadata* metadata) const = 0;
    void OverrideMetadata(const DependencyProperty* dp, const TypeClass* forType,
        PropertyMetadata* metadata) const;

protected:
    virtual void SetValue(DependencyObject* dob, const DependencyProperty* dp,
        BaseComponent* value, uint8_t priority, Expression* expression,
        const PropertyMetadata* metadata, Value::Destination destination) const = 0;
};

}

#endif
