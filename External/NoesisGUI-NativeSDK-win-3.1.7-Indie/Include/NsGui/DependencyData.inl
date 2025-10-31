////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyData::RegisterProperty(const DependencyProperty*& dp, const char* name,
    PropertyMetadata* metadata)
{
    RegisterProperty<T>(dp, name, metadata, nullptr, PropertyAccess_ReadWrite);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyData::RegisterProperty(const DependencyProperty*& dp, const char* name,
    PropertyMetadata* metadata, ValidateValueCallback validate)
{
    RegisterProperty<T>(dp, name, metadata, validate, PropertyAccess_ReadWrite);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyData::RegisterPropertyRO(const DependencyProperty*& dp, const char* name,
    PropertyMetadata* metadata)
{
    RegisterProperty<T>(dp, name, metadata, nullptr, PropertyAccess_ReadOnly);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyData::RegisterPropertyRO(const DependencyProperty*& dp, const char* name,
    PropertyMetadata* metadata, ValidateValueCallback validate)
{
    RegisterProperty<T>(dp, name, metadata, validate, PropertyAccess_ReadOnly);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyData::AddOwner(const DependencyProperty*& dp, const char* name,
    const DependencyProperty*& source, PropertyMetadata* metadata)
{
    static_assert(!IsInterface<RemovePtr<T>>::Result, "Interface not supported in DPs");
    const Type* type = TypeOf<RemovePtr<T>>();

    NS_ASSERT(!StrIsNullOrEmpty(name));
    NS_ASSERT(dp == 0 || dp->GetName() == Symbol(name));
    NS_ASSERT(dp == 0 || type == dp->GetType());

    if (source != 0)
    {
        NS_ASSERT(source->GetName() == Symbol(name));
        NS_ASSERT(type == source->GetType());
        dp = InsertProperty(source);
    }
    else
    {
        // Ensure insertion because the property could had been created in a previous call to
        // OverrideMetadata but not registered in the owner DependencyData
        if (dp != 0)
        {
            InsertProperty(dp);
        }
        else
        {
            Ptr<DependencyProperty> prop = CreateProperty<T>(name, Int2Type<IsPtr<T>::Result>());
            dp = InsertProperty(prop);
        }

        source = dp;
    }

    if (dp != 0 && metadata != 0)
    {
        NS_ASSERT(!metadata->HasDefaultValue() || metadata->GetValueManager()->CheckType(type));
        const_cast<DependencyProperty*>(dp)->AddMetadata(mOwnerType, metadata);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyData::OverrideMetadata(const DependencyProperty*& dp, const char* name,
    PropertyMetadata* metadata)
{
    static_assert(!IsInterface<RemovePtr<T>>::Result, "Interface not supported in DPs");
    const Type* type = TypeOf<RemovePtr<T>>();

    NS_CHECK(metadata != nullptr,
        "Can't override dependency property '%s.%s', metadata is null",
        mOwnerType->GetName(), name);

    if (dp != 0)
    {
        NS_ASSERT(!StrIsNullOrEmpty(name));
        NS_ASSERT(dp->GetName() == Symbol(name));
        NS_ASSERT(type == dp->GetType());
    }
    else
    {
        Ptr<DependencyProperty> prop = CreateProperty<T>(name, Int2Type<IsPtr<T>::Result>());
        dp = InsertProperty(prop.GetPtr());
    }

    if (dp != 0)
    {
        NS_ASSERT(!metadata->HasDefaultValue() || metadata->GetValueManager()->CheckType(type));
        const_cast<DependencyProperty*>(dp)->AddMetadata(mOwnerType, metadata);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyData::RegisterProperty(const DependencyProperty*& dp, const char* name,
    PropertyMetadata* metadata, ValidateValueCallback validate, PropertyAccess access)
{
    static_assert(!IsInterface<RemovePtr<T>>::Result, "Interface not supported in DPs");
    const Type* type = TypeOf<RemovePtr<T>>();

    CheckMetadata(name, metadata, type);

    if (dp != 0)
    {
        NS_ASSERT(type == dp->GetType());
        RegisterExistingProperty(dp, name, metadata, validate, access);
    }
    else
    {
        Ptr<DependencyProperty> prop = CreateProperty<T>(name, metadata, validate, access,
            Int2Type<IsPtr<T>::Result>());
        dp = InsertProperty(prop);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
Ptr<DependencyProperty> DependencyData::CreateProperty(const char* name, PropertyMetadata* metadata,
    ValidateValueCallback validate, PropertyAccess access, IsNotBaseComponent)
{
    return DependencyProperty::Create<T>(name, mOwnerType, metadata, validate, access);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
Ptr<DependencyProperty> DependencyData::CreateProperty(const char* name, PropertyMetadata* metadata,
    ValidateValueCallback validate, PropertyAccess access, IsBaseComponent)
{
    return DependencyProperty::Create<Ptr<BaseComponent>>(name, TypeOf<typename T::Type>(),
        mOwnerType, metadata, validate, access);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
Ptr<DependencyProperty> DependencyData::CreateProperty(const char* name, IsNotBaseComponent)
{
    return DependencyProperty::Create<T>(name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
Ptr<DependencyProperty> DependencyData::CreateProperty(const char* name, IsBaseComponent)
{
    return DependencyProperty::Create<Ptr<BaseComponent>>(name, TypeOf<typename T::Type>());
}

}
