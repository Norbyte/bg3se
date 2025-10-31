////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MULTIBINDINGEXPRESSION_H__
#define __GUI_MULTIBINDINGEXPRESSION_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>
#include <NsGui/CoreApi.h>
#include <NsGui/PropertyPath.h>
#include <NsGui/Binding.h>
#include <NsGui/BaseBindingExpression.h>
#include <NsGui/BindingListener.h>
#include <NsGui/Collection.h>


namespace Noesis
{

class MultiBinding;
class DependencyObject;
class DependencyProperty;
class FrameworkElement;
struct RoutedEventArgs;
NS_INTERFACE IMultiValueConverter;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Contains instance information about a single instance of a MultiBinding.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.data.multibindingexpression
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API MultiBindingExpression final: public BaseBindingExpression
{
public:
    ~MultiBindingExpression();

    /// Binding from which this expression was created
    /// \prop
    MultiBinding* GetParentBinding() const;

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
    friend class MultiBinding;
    MultiBindingExpression(MultiBinding* binding, DependencyObject* targetObject,
        const DependencyProperty* targetProperty);

    bool IsAttached() const;

    void Register();
    void Unregister();

    Ptr<BaseComponent> GetSourceValue() const;

    void UpdateSourceInternal(const void* value) const;
    void TransferSourceValue(const void* value) const;

    void OnTargetLostFocus(BaseComponent* sender, const RoutedEventArgs& e);

    void OnTargetDestroyed(DependencyObject* dob);
    bool IsTargetAlive() const;

private:
    class Listener: public BindingListener
    {
    public:
        Listener(MultiBindingExpression* o, BaseBinding* b);

        BaseComponent* GetValue() const;
        const Type* GetType() const;

    protected:
        DependencyObject* GetTarget() const override;
        BaseBinding* GetBinding() const override;

        void OnBindingChanged(BaseComponent* sourceValue, const Type* sourceType) override;

    private:
        MultiBindingExpression* owner;
        BaseBinding* binding;
        mutable Ptr<BaseComponent> value;
        mutable const Type* type;
    };

    typedef Vector<Listener, 2> BindingListeners;
    BindingListeners mListeners;

    // Effective binding mode (never set to Default because proper value is retrieved from metadata)
    BindingMode mEffectiveBindingMode;

    // The effective UpdateSourceTrigger value to use
    UpdateSourceTrigger mEffectiveUpdateSourceTrigger;

    // Target element in the UI tree 
    FrameworkElement* mTargetElement;

    // The delay timer identifier
    int mDelayTimer;

    union
    {
        mutable struct
        {
            bool oneTimeEvaluated : 1;
            bool skipInvalidate : 1;
            bool transferingSourceValue : 1;
            bool doNothing : 1;
        } mFlags;

        // To quickly set all flags to 0
        uint32_t mAllFlags;
    };

    NS_DECLARE_REFLECTION(MultiBindingExpression, BaseBindingExpression)
};

NS_WARNING_POP

}


#endif
