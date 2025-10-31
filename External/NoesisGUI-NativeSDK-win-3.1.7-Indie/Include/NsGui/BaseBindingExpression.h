////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASEBINDINGEXPRESSION_H__
#define __GUI_BASEBINDINGEXPRESSION_H__


#include <NsCore/Noesis.h>
#include <NsGui/Expression.h>
#include <NsGui/CoreApi.h>
#include <NsGui/UpdateSourceTrigger.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

class BaseBinding;
class DependencyObject;
class DependencyProperty;
enum BindingMode: int32_t;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the base class for all Binding Expressions.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.data.bindingexpressionbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseBindingExpression: public Expression
{
public:
    BaseBindingExpression();
    BaseBindingExpression(BaseBinding* binding, DependencyObject* targetObject,
        const DependencyProperty* targetProperty);
    BaseBindingExpression(const BaseBindingExpression&) = delete;
    BaseBindingExpression& operator=(const BaseBindingExpression&) = delete;
    virtual ~BaseBindingExpression() = 0;

    /// Gets the element to which this BindingExpression is attached
    /// \prop
    DependencyObject* GetTarget() const;

    /// Gets the property to which this BindingExpression is attached
    /// \prop
    const DependencyProperty* GetTargetProperty() const;

    /// Gets the BaseBinding object from which this BaseBindingExpression object is created
    /// \prop
    BaseBinding* GetParentBindingBase() const;

    /// Forces a data transfer from source to target
    virtual void UpdateTarget() const = 0;

    /// Sends the current value back to the source. Does nothing when binding's Mode is not TwoWay
    /// or OneWayToSource
    virtual void UpdateSource() const = 0;

protected:
    void ResolveEffectiveDefaultValues(BindingMode& mode, UpdateSourceTrigger& trigger);

protected:
    // Base binding which originated the expression
    Ptr<BaseBinding> mBinding;

    // Dependency object where the expression is applied
    DependencyObject* mTargetObject;

    // Dependency property where the expression is applied
    const DependencyProperty* mTargetProperty;

    // The effective fallback value to assign in case of fail in the binding
    Ptr<BaseComponent> mEffectiveFallback;

    // The effective value to assign in case the final target value is null
    Ptr<BaseComponent> mEffectiveTargetNull;

    NS_DECLARE_REFLECTION(BaseBindingExpression, Expression)
};

NS_WARNING_POP

}

#endif
