////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BINDINGOPERATIONS_H__
#define __GUI_BINDINGOPERATIONS_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

class DependencyObject;
class DependencyProperty;
class BaseBinding;
class Binding;
class BaseBindingExpression;
class BindingExpression;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides static methods to manipulate bindings.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.data.bindingoperations.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API BindingOperations
{
    /// Removes all bindings from the specified DependencyObject
    static void ClearAllBindings(DependencyObject* target);

    /// Removes the binding from a property if there is one
    static void ClearBinding(DependencyObject* target, const DependencyProperty* dp);

    /// Retrieves the BindingBase object that is set on the specified property
    static BaseBinding* GetBindingBase(const DependencyObject* target,
        const DependencyProperty* dp);

    /// Retrieves the Binding object that is set on the specified property
    static Binding* GetBinding(const DependencyObject* target, const DependencyProperty* dp);

    /// Returns the BaseBindingExpression object associated with the specified binding target
    /// property on the specified object
    static BaseBindingExpression* GetBindingExpressionBase(const DependencyObject* target,
        const DependencyProperty* dp);

    /// Returns the BindingExpression object associated with the specified binding target property
    /// on the specified object
    static BindingExpression* GetBindingExpression(const DependencyObject* target, 
        const DependencyProperty* dp);

    /// Returns a value that indicates whether the specified property is currently data-bound
    static bool IsDataBound(const DependencyObject* target, const DependencyProperty* dp);

    /// Creates and associates a new instance of BaseBindingExpression with the specified binding 
    /// target property
    static BaseBindingExpression* SetBinding(DependencyObject* target, const DependencyProperty* dp,
        BaseBinding* binding);
};

}


#endif
