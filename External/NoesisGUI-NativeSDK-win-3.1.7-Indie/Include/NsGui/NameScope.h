////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_NAMESCOPE_H__
#define __GUI_NAMESCOPE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/INameScope.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/HashMap.h>
#include <NsCore/Delegate.h>
#include <NsCore/String.h>


namespace Noesis
{

class DependencyObject;
class DependencyProperty;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Store relationships between the XAML defined names of objects and their instances.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API NameScope: public BaseComponent, public INameScope
{
public:
    NameScope();
    ~NameScope();

    /// Gets the attached NameScope property
    static NameScope* GetNameScope(const DependencyObject* element);
    /// Sets the attached NameScope property
    static void SetNameScope(DependencyObject* element, NameScope* nameScope);

    /// Finds the name of an object if it is registered in the NameScope.
    /// Returns null if object was not found
    const char* FindObject(BaseComponent* obj) const;

    /// Calls the delegate for each named object registered in the NameScope
    typedef Noesis::Delegate<void (const char*, BaseComponent*, void*)> EnumNamedObjectsDelegate;
    void EnumNamedObjects(const EnumNamedObjectsDelegate& delegate, void* context = 0) const;

    /// From INameScope
    //@{
    BaseComponent* FindName(const char* name) const override;
    void RegisterName(const char* name, BaseComponent* obj) override;
    void UnregisterName(const char* name) override;
    void UpdateName(const char* name, BaseComponent* obj) override;
    INameScope::ChangedDelegate& NameScopeChanged() override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* NameScopeProperty; // attached property
    //@}

protected:
    int32_t OnDestroy() override;

private:
    typedef HashMap<String, BaseComponent*> NameObjectMap;
    NameObjectMap mNamedObjects;

    ChangedDelegate mChanged;

    NS_DECLARE_REFLECTION(NameScope, BaseComponent)
};

NS_WARNING_POP

}


#endif
