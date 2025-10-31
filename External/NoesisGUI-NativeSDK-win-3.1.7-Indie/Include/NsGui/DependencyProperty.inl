////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsGui/ValueStorageManagerImpl.h>
#include <NsGui/PropertyMetadata.h>
#include <NsCore/TypeProperty.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
Ptr<DependencyProperty> DependencyProperty::Create(const char* name)
{
    // NOTE: This constructor doesn't create metadata because it doesn't have the ownerType and
    // because this constructor is intended to be used from DependencyData to partially create
    // the class and later be fulfilled (example: when using AddOwner or OverrideMetadata
    // before RegisterProperty)
    ValueStorageManager* valueManager = new ValueStorageManagerImpl<T>();
    return *new DependencyProperty(Symbol(name), valueManager, TypeOf<RemovePtr<T>>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
Ptr<DependencyProperty> DependencyProperty::Create(const char* name, const Type* type)
{
    // NOTE: This constructor doesn't create metadata because it doesn't have the ownerType and
    // because this constructor is intended to be used from DependencyData to partially create
    // the class and later be fulfilled (example: when using AddOwner or OverrideMetadata
    // before RegisterProperty)

    ValueStorageManager* valueManager = new ValueStorageManagerImpl<T>();
    return *new DependencyProperty(Symbol(name), valueManager, type);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
Ptr<DependencyProperty> DependencyProperty::Create(const char* name, const TypeClass* ownerType,
    PropertyMetadata* metadata, ValidateValueCallback validate, PropertyAccess access)
{
    ValueStorageManager* valueManager = new ValueStorageManagerImpl<T>();
    return *new DependencyProperty(Symbol(name), valueManager, TypeOf<RemovePtr<T>>(),
        ownerType, metadata, validate, access);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
Ptr<DependencyProperty> DependencyProperty::Create(const char* name, const Type* type,
    const TypeClass* ownerType, PropertyMetadata* metadata, ValidateValueCallback validate,
    PropertyAccess access)
{
    ValueStorageManager* valueManager = new ValueStorageManagerImpl<T>();
    return *new DependencyProperty(Symbol(name), valueManager, type, ownerType, metadata,
        validate, access);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol DependencyProperty::GetName() const
{
    return mName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Type* DependencyProperty::GetType() const
{
    return mType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const TypeClass* DependencyProperty::GetOwnerType() const
{
    return mOwnerType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const void* DependencyProperty::GetDefaultValue(const TypeClass* forType) const
{
    return GetMetadata(forType)->GetDefaultValue();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<BaseComponent> DependencyProperty::GetDefaultValueObject(
    const TypeClass* forType) const
{
    return GetMetadata(forType)->GetDefaultValueObject();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool DependencyProperty::IsReadOnly() const
{
    return mIsReadOnly;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t DependencyProperty::GetSize() const
{
    return mValueManager->Size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline ValidateValueCallback DependencyProperty::GetValidateValueCallback() const
{
    return mValidate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline ValueStorageManager* DependencyProperty::GetValueManager() const
{
    return mValueManager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DependencyProperty::OverrideMetadata(const TypeClass* forType,
    PropertyMetadata* metadata) const
{
    mValueManager->OverrideMetadata(this, forType, metadata);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const TypeProperty* DependencyProperty::FindTypeProperty() const
{
    NS_ASSERT(mOwnerType != 0);
    return mOwnerType->FindProperty(mName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
const T* DependencyProperty::FindTypeMetaData() const
{
    const TypeProperty* prop = FindTypeProperty();
    return prop != 0 ? FindMeta<T>(prop) : 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool DependencyProperty::IsSameValue(const DependencyObject* obj,
    const Ptr<BaseComponent>& value) const
{
    return mValueManager->IsSame(mValueManager->GetValue(obj, this), value.GetPtr());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool DependencyProperty::IsSameValue(const void* left,
    const Ptr<BaseComponent>& right) const
{
    return mValueManager->IsSame(left, right.GetPtr());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool DependencyProperty::IsSameValue(const void* left, const void* right) const
{
    return mValueManager->IsSame(left, right);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool DependencyProperty::CheckType(const Type* valueType) const
{
    return mValueManager->CheckType(valueType);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool DependencyProperty::CheckType(const Ptr<BaseComponent>& value) const
{
    return mValueManager->CheckType(GetType(), value.GetPtr());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const void* DependencyProperty::GetValue(const DependencyObject* obj) const
{
    return mValueManager->GetValue(obj, this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<BaseComponent> DependencyProperty::GetValueObject(const void* value) const
{
    return mValueManager->Box(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<BaseComponent> DependencyProperty::GetValueObject(
    const DependencyObject* obj) const
{
    return mValueManager->Box(mValueManager->GetValue(obj, this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<BaseComponent> DependencyProperty::GetBaseValueObject(
    const DependencyObject* obj) const
{
    return mValueManager->Box(mValueManager->GetBaseValue(obj, this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void DependencyProperty::SetValue(DependencyObject* obj, const void* value, uint8_t priority,
    Expression* expression, const PropertyMetadata* metadata, Value::Destination destination) const
{
    mValueManager->SetValue(obj, this, value, priority, expression, metadata, destination);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void DependencyProperty::SetValueObject(DependencyObject* obj, BaseComponent* value,
    uint8_t priority, Expression* expression, const PropertyMetadata* metadata,
    Value::Destination destination) const
{
    mValueManager->SetValueObject(obj, this, value, priority, expression, metadata, destination);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void DependencyProperty::ClearAnimation(DependencyObject* obj) const
{
    mValueManager->ClearAnimation(obj, this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void DependencyProperty::CoerceValue(DependencyObject* obj, StoredValue* storedValue,
    const void* defaultValue, const PropertyMetadata* metadata) const
{
    mValueManager->CoerceValue(obj, this, storedValue, defaultValue, metadata);
}

}
