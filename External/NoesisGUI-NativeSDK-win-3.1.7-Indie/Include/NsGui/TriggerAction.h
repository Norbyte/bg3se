////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TRIGGERACTION_H__
#define __GUI_TRIGGERACTION_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/DependencyObject.h>
#include <NsGui/IUITreeNode.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

class FrameworkElement;
class TriggerAction;

template<class T> class UICollection;
typedef UICollection<TriggerAction> TriggerActionCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes an action to perform for a trigger.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.triggeraction.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TriggerAction: public DependencyObject, public IUITreeNode
{
public:
    TriggerAction();
    virtual ~TriggerAction() = 0;

    /// Seals all actions in the collection
    static void SealActions(TriggerActionCollection* actions);

    /// Performs the action over the supplied element
    virtual void Invoke(FrameworkElement* target, FrameworkElement* nameScope = 0) = 0;

    /// Seals the actions
    virtual void Seal();

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const override;
    void SetNodeParent(IUITreeNode* parent) override;
    BaseComponent* FindNodeResource(const char* key, bool fullElementSearch) const override;
    BaseComponent* FindNodeName(const char* name) const override;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

protected:
    /// From DependencyObject
    //@{
    void OnObjectValueSet(BaseComponent* oldValue, BaseComponent* newValue) override;
    //@}

protected:
    bool mIsSealed;

private:
    IUITreeNode* mOwner;

    NS_DECLARE_REFLECTION(TriggerAction, DependencyObject)
};

NS_WARNING_POP

}


#endif
