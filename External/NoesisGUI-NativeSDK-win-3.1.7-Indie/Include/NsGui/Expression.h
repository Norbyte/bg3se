////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EXPRESSION_H__
#define __GUI_EXPRESSION_H__


#include <NsCore/Noesis.h>
#include <NsGui/DependencySystemApi.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

template<class T> class Ptr;
class DependencyObject;
class DependencyProperty;

////////////////////////////////////////////////////////////////////////////////////////////////////
enum BeforeSetAction
{
    /// Execute method and delete expression (updating cached value). This is the common return
    /// value for expressions that do not require the BeforeSet call
    BeforeSetAction_Delete = 0,
    BeforeSetAction_Default = 0,
    /// Execute method and update cached value (keep expression)
    BeforeSetAction_Maintain = 1
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base implementation for all expressions.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.expression.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_DEPENDENCYSYSTEM_API Expression: public BaseComponent
{
public:
    Expression();
    virtual ~Expression();

    /// Evaluates expression when applied to the specified target object
    virtual Ptr<BaseComponent> Evaluate() const;

    /// Applies expression to a new target, cloning the expression itself if necessary
    virtual Ptr<Expression> Reapply(DependencyObject* targetObject,
        const DependencyProperty* targetProperty) const;

    /// Executes the set
    virtual BeforeSetAction BeforeSet(DependencyObject* obj, const DependencyProperty* dp,
        const void* value, bool valueHasChanged);

    /// Executed after set is completed and the property changed has been notified
    virtual void AfterSet(DependencyObject* obj, const DependencyProperty* dp,
        const void* value, bool valueHasChanged);

    NS_DECLARE_REFLECTION(Expression, BaseComponent)
};

NS_WARNING_POP

}


#endif
