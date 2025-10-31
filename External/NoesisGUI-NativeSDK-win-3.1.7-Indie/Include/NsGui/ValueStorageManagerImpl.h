////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VALUESTORAGEMANAGERIMPL_H__
#define __GUI_VALUESTORAGEMANAGERIMPL_H__


#include <NsCore/Noesis.h>
#include <NsGui/ValueStorageManager.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements ValueStorageManager for the specified type.
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
class ValueStorageManagerImpl: public ValueStorageManager
{
protected:
    // From ValueStorageManager
    //@{
    ValueStorageManager* Clone() const override final;
    uint32_t Size() const override final;
    bool CheckType(const Type* type) const override final;
    bool CheckType(const Type* type, BaseComponent* value) const override;
    bool Validate(ValidateValueCallback validate, BaseComponent* value) const override;
    void Construct(ValueStorage* storage, const void* value) const override final;
    void Destroy(ValueStorage* storage) const override final;
    void* ToValue(const ValueStorage* storage) const override final;
    void Copy(void* dst, const void* value) const override final;
    void Copy(void* dst, BaseComponent* value) const override;
    Ptr<BaseComponent> Box(const void* value) const override;
    const void* GetValue(const DependencyObject* dob,
        const DependencyProperty* dp) const override final;
    const void* GetBaseValue(const DependencyObject* dob,
        const DependencyProperty* dp) const override final;
    bool IsSame(const void* left, const void* right) const override final;
    bool IsSame(const void* left, BaseComponent* right) const override;
    void SetValue(DependencyObject* dob, const DependencyProperty* dp, const void* value,
        uint8_t priority, Expression* expression, const PropertyMetadata* metadata,
        Value::Destination destination) const override final;
    void SetValue(DependencyObject* dob, const DependencyProperty* dp,
        BaseComponent* value, uint8_t priority, Expression* expression,
        const PropertyMetadata* metadata, Value::Destination destination) const override;
    void ClearAnimation(DependencyObject* dob, const DependencyProperty* dp) const override final;
    void CoerceValue(DependencyObject* dob, const DependencyProperty* dp,
        StoredValue* storedValue, const void* defaultValue,
        const PropertyMetadata* metadata) const override final;
    //@}
};

}

#include <NsGui/ValueStorageManagerImpl.inl>

#endif
