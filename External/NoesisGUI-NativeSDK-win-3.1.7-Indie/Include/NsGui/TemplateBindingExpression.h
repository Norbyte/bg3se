////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TEMPLATEBINDINGEXPRESSION_H__
#define __GUI_TEMPLATEBINDINGEXPRESSION_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Expression.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

class TypeConverter;
class DependencyObject;
class FrameworkElement;
class TemplateBindingExtension;
struct DependencyPropertyChangedEventArgs;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes a run-time instance of a TemplateBindingExtension.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.templatebindingexpression.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TemplateBindingExpression: public Expression
{
public:
    ~TemplateBindingExpression();

    /// Gets the TemplateBindingExtension object of this expression instance
    /// \prop
    TemplateBindingExtension* GetTemplateBindingExtension() const;

    /// From Expression
    //@{
    Ptr<BaseComponent> Evaluate() const;
    Ptr<Expression> Reapply(DependencyObject* targetObject,
        const DependencyProperty* targetProperty) const;
    //@}

private:
    friend class TemplateBindingExtension;
    TemplateBindingExpression(TemplateBindingExtension* extension, DependencyObject* targetObject,
        const DependencyProperty* targetProperty);

    void Register();
    void Unregister();

    void OnTemplatedParentPropertyChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);

    void TemplatedParentDestroyed(DependencyObject* object);

    void TargetDestroyed(DependencyObject* object);

private:
    Ptr<TemplateBindingExtension> mBinding;
    DependencyObject* mTargetObject;
    const DependencyProperty* mTargetProperty;
    FrameworkElement* mTemplatedParent;
    Ptr<TypeConverter> mConverter;

    NS_DECLARE_REFLECTION(TemplateBindingExpression, Expression)
};

NS_WARNING_POP

}


#endif
