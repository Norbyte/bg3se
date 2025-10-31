////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_PROPERTYMETADATA_H__
#define __GUI_PROPERTYMETADATA_H__


#include <NsCore/Noesis.h>
#include <NsGui/DependencySystemApi.h>
#include <NsGui/ValueStorageManager.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Delegate.h>


namespace Noesis
{

class DependencyProperty;
class DependencyObject;
struct DependencyPropertyChangedEventArgs;

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef Delegate<void (DependencyObject* d, const DependencyPropertyChangedEventArgs& e)>
    PropertyChangedCallback;

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef Delegate<bool (const DependencyObject* d, const void* baseValue, void* coercedValue)>
    CoerceValueCallback;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines certain behavior aspects of a dependency property as it is applied to a specific type,
/// including conditions it was registered with.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_DEPENDENCYSYSTEM_API PropertyMetadata: public BaseComponent
{
public:
    // Helper functions to create a new PropertyMetadata
    //@{
    static Ptr<PropertyMetadata> Create();

    static Ptr<PropertyMetadata> Create(const PropertyChangedCallback& changed);

    static Ptr<PropertyMetadata> Create(const CoerceValueCallback& coerce);

    template<class T>
    inline static Ptr<PropertyMetadata> Create(const T& defaultValue);

    template<class T>
    inline static Ptr<PropertyMetadata> Create(const T& defaultValue,
        const PropertyChangedCallback& changed);

    template<class T>
    inline static Ptr<PropertyMetadata> Create(const T& defaultValue,
        const CoerceValueCallback& coerce);

    template<class T>
    inline static Ptr<PropertyMetadata> Create(const T& defaultValue,
        const PropertyChangedCallback& changed, const CoerceValueCallback& coerce);
    //@}

    ~PropertyMetadata();

    /// Gets or sets the default value of the dependency property
    //@{
    inline const void* GetDefaultValue() const;
    void SetDefaultValue(ValueStorageManager* defaultValueManager, const void* defaultValue);

    template<class T>
    void SetDefaultValue(const T& defaultValue);
    //@}

    /// Gets default value as a boxed object
    inline Ptr<BaseComponent> GetDefaultValueObject() const;

    /// Gets *DefaultValue* memory manager
    inline ValueStorageManager* GetValueManager() const;

    /// Indicates if this metadata has a DefaultValue set
    inline bool HasDefaultValue() const;

    /// Gets or sets a PropertyChangedCallback implementation specified in this metadata
    /// \prop
    //@{
    inline const PropertyChangedCallback& GetPropertyChangedCallback() const;
    void SetPropertyChangedCallback(const PropertyChangedCallback& changed);

    /// Indicates if this metadata has a PropertyChangedCallback set
    inline bool HasPropertyChangedCallback() const;

    /// Gets or sets a CoerceValueCallback implementation specified in this metadata
    /// \prop
    //@{
    inline const CoerceValueCallback& GetCoerceValueCallback() const;
    void SetCoerceValueCallback(const CoerceValueCallback& coerce);
    //@}

    /// Indicates if this metadata has a CoerceValueCallback set
    inline bool HasCoerceValueCallback() const;

    /// Determines if the values of the property using this metadata should not be cached
    /// prop
    //@{
    inline bool GetUncached() const;
    void SetUncached(bool value);
    //@}

    /// Indicates if this metadata has a the Uncached flag set
    inline bool IsUncached() const;

    /// Remove inherited values from ancestors
    virtual void ClearInheritedValues();

    /// Inherit ancestor values if they are not local in this class
    virtual void Merge(const PropertyMetadata* ancestor);

    /// Notifies listeners when metadata is destroyed
    typedef Delegate<void(PropertyMetadata* m)> DestroyedDelegate;
    DestroyedDelegate& Destroyed();

protected:
    /// Constructors
    //@{
    PropertyMetadata();
    static Ptr<PropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue);
    static Ptr<PropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, const PropertyChangedCallback& changed);
    static Ptr<PropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, const CoerceValueCallback& coerce);
    static Ptr<PropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, const PropertyChangedCallback& changed,
        const CoerceValueCallback& coerce);
    //@}

    /// From BaseComponent
    //@{
    int32_t OnDestroy() override;
    //@}

    bool CheckSealed() const;

private:
    void ResetDefaultValue();

private:
    ValueStorage mDefaultValue;
    ValueStorageManager* mDefaultValueManager;
    PropertyChangedCallback mLocalChanged;
    PropertyChangedCallback mChanged;
    CoerceValueCallback mCoerce;

    DestroyedDelegate mDestroyed;

    enum PropertyFlags
    {
        PropertyFlags_None = 0,
        PropertyFlags_Default = 1 << 1,
        PropertyFlags_Coerce = 1 << 2,
        PropertyFlags_Changed = 1 << 3,
        PropertyFlags_Uncached = 1 << 4
    };

    // Flag to control if properties value are local or inherited
    uint32_t mLocalFlags;

    // Tells if the property must not be cached in the DependencyObject internal hash
    bool mLocalUncached;
    bool mUncached;

    friend class DependencyProperty;
    bool mSealed;

    NS_DECLARE_REFLECTION(PropertyMetadata, BaseComponent)
};

NS_WARNING_POP

}

// Inline Include
#include <NsGui/PropertyMetadata.inl>


#endif
