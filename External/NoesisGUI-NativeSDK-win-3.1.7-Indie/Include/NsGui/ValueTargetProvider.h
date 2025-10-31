////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VALUETARGETPROVIDER_H__
#define __GUI_VALUETARGETPROVIDER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

class DependencyProperty;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides useful information during MarkupExtension *ProvideValue* calls.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ValueTargetProvider: public BaseObject
{
public:
    ValueTargetProvider(BaseComponent* targetObject, const DependencyProperty* targetProperty);

    /// Gets target object where the MarkupExtension is being applied
    BaseComponent* GetTargetObject() const;

    /// Gets target property where the MarkupExtension is being applied
    const DependencyProperty* GetTargetProperty() const;

private:
    Ptr<BaseComponent> mTargetObject;
    const DependencyProperty* mTargetProperty;

    NS_DECLARE_REFLECTION(ValueTargetProvider, BaseObject)
};

NS_WARNING_POP

}


#endif
