////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BINDINGEXPRESSION_H__
#define __GUI_BINDINGEXPRESSION_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>
#include <NsGui/CoreApi.h>
#include <NsGui/PropertyPath.h>
#include <NsGui/Binding.h>
#include <NsGui/BaseBindingExpression.h>


namespace Noesis
{

NS_INTERFACE IValueConverter;
NS_INTERFACE INameScope;
class DependencyObject;
class DependencyProperty;
class FrameworkElement;
class CollectionView;
struct PropertyChangedEventArgs;
struct NotifyDictionaryChangedEventArgs;
struct NotifyCollectionChangedEventArgs;
struct DependencyPropertyChangedEventArgs;
struct RoutedEventArgs;
struct EventArgs;
struct AncestorNameScopeChangedArgs;
struct NameScopeChangedArgs;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Contains information about a single instance of a Binding.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.data.bindingexpression.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BindingExpression final: public BaseBindingExpression
{
public:
    ~BindingExpression();

    /// Binding from which this expression was created
    /// \prop
    Binding* GetParentBinding() const;

    /// From BaseBindingExpression
    //@{
    void UpdateTarget() const override;
    void UpdateSource() const override;
    //@}

    /// From Expression
    //@{
    Ptr<BaseComponent> Evaluate() const override;
    Ptr<Expression> Reapply(DependencyObject* targetObject,
        const DependencyProperty* targetProperty) const override;
    BeforeSetAction BeforeSet(DependencyObject* obj, const DependencyProperty* prop, 
        const void* value, bool valueChanged) override;
    void AfterSet(DependencyObject* obj, const DependencyProperty* prop, 
        const void* value, bool valueChanged) override;
    //@}

private:
    friend class Binding;
    BindingExpression(BaseBinding* binding, DependencyObject* targetObject,
        const DependencyProperty* targetProperty);

    bool IsAttached() const;

    void Register();
    void Unregister();

    void RegisterTarget(FrameworkElement* target, Binding* binding);
    void UnregisterTarget(FrameworkElement* target);

    void RegisterScope();
    void UnregisterScope();

    void Initialize();
    void Shutdown();

    void AddPathElement(const PathElement& element, void* context);

    void RegisterSource();
    void UnregisterSource();

    struct WeakPathElement;
    uint32_t GetRegisteredFlags(uint32_t end, const WeakPathElement& element) const;
    void RegisterNotification(const WeakPathElement& element, uint32_t registeredFlags);
    void UnregisterNotification(const WeakPathElement& element, uint32_t registeredFlags);
    bool IsSelf() const;
    bool IsSourceReadOnly() const;
    Ptr<BaseComponent> GetValue(const WeakPathElement& element, const Type*& valueType) const;

    // Checks if a converter is needed, creating one when required
    bool CheckConverter(const Type* sourceType, BaseComponent* sourceVal,
        const Type* targetType, BaseComponent* targetVal) const;

    void UpdateSourceInternal(const void* value) const;
    void TransferSourceValue(const void* value) const;
    Ptr<BaseComponent> GetSourceValue() const;

    void ResetTargetValue() const;
    void InvalidateTarget() const;
    void InvalidateSource(bool invalidateTargetProperty = true);

    // Source change notification
    //@{
    void OnDataContextParentChanged(FrameworkElement* oldParent);

    void OnDataContextChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);

    void OnAncestorChanged(FrameworkElement* ancestor);

    void OnAncestorNameScopeChanged(FrameworkElement* sender,
        const AncestorNameScopeChangedArgs& args);

    void OnSourceNameScopeChanged(INameScope* sender, const NameScopeChangedArgs& args);
    //@}

    // Property change notifications
    //@{
    void OnDependencyPropertyChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);

    void OnNotifyPropertyChanged(BaseComponent* sender, const PropertyChangedEventArgs& e);

    void OnCollectionChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);
    void OnCollectionReset(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);

    void OnDictionaryChanged(BaseComponent* sender,
        const NotifyDictionaryChangedEventArgs& args);
    void OnDictionaryReset(BaseComponent* sender,
        const NotifyDictionaryChangedEventArgs& args);

    void OnCurrentChanged(BaseComponent* sender, const EventArgs& args);
    //@}

    // Event notification
    //@{
    void OnTargetLostFocus(BaseComponent*, const RoutedEventArgs& args);

    void OnTargetLoaded(BaseComponent*, const RoutedEventArgs&);
    void OnTargetUnloaded(BaseComponent*, const RoutedEventArgs&);

    void OnTargetDestroyed(DependencyObject* object);
    void OnSourceDestroyed(DependencyObject* object);
    //@}

    bool IsTargetAlive() const;

private:
    BaseComponent* mSource;
    FrameworkElement* mTargetElement;
    INameScope* mNameScope;

    struct WeakPathElement
    {
        BaseComponent* source;
        CollectionView* collection;
        const TypeProperty* property;
        const DependencyProperty* dp;
        const char* key;
        int index;
    };

    // List of objects to access final source property
    Vector<WeakPathElement> mPaths;

    // Effective binding mode (never set to Default because proper value is retrieved from metadata)
    BindingMode mEffectiveBindingMode;

    // Value converter is used when source and target property types are different, or when set by
    // the user in the Converter field of the Binding class
    mutable Ptr<IValueConverter> mEffectiveConverter;

    // The effective UpdateSourceTrigger value to use
    UpdateSourceTrigger mEffectiveUpdateSourceTrigger;

    // The delay timer identifier
    int mDelayTimer;

    union
    {
        mutable struct
        {
            bool fullReevaluationNeeded : 1;
            bool pathPartiallySolved : 1;
            bool sourceRegistered : 1;
            bool targetDestroyRegistered : 1;
            bool targetLostFocusRegistered : 1;
            bool targetAncestorChangedRegistered : 1;
            bool targetDataContextParentChangedRegistered : 1;
            bool targetDataContextChangedRegistered : 1;
            bool targetAncestorNameScopeChangedRegistered : 1;
            bool targetLoadedRegistered : 1;
            bool targetValueReset : 1;
            bool propertyChangeRegistered : 1;
            bool collectionResetRegistered : 1;
            bool dictionaryResetRegistered : 1;
            bool updatingSource : 1;
            bool updatingTarget : 1;
            bool transferingSourceValue : 1;
            bool doNothing : 1;
            bool checkConverter : 1;
            bool oneTimeEvaluated : 1;
        } mFlags;

        // To quickly set all flags to 0
        uint32_t mAllFlags;
    };

    NS_DECLARE_REFLECTION(BindingExpression, BaseBindingExpression)
};

NS_WARNING_POP

}


#endif
