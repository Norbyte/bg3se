////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ANIMATABLE_H__
#define __GUI_ANIMATABLE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Freezable.h>
#include <NsGui/IUITreeNode.h>


namespace Noesis
{

class FrameworkTemplate;
class NameScope;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Abstract class that provides animation support.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.animatable.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Animatable: public Freezable, public IUITreeNode
{
public:
    Animatable();
    virtual ~Animatable() = 0;

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const override;
    void SetNodeParent(IUITreeNode* parent) override;
    BaseComponent* FindNodeResource(const char* key, bool fullElementSearch) const override;
    BaseComponent* FindNodeName(const char* name) const override;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const override;
    //@}

    // Internal usage for template cloning
    static void SetTemplateNameScope(FrameworkTemplate* template_, NameScope* nameScope);

    NS_IMPLEMENT_INTERFACE_FIXUP

protected:
    /// From DependencyObject
    //@{
    void OnObjectValueSet(BaseComponent* oldValue, BaseComponent* newValue) override;
    //@}

    /// From Freezable
    //@{
    bool FreezeCore(bool isChecking) override;
    void CloneCommonCore(const Freezable* source) override;
    //@}

private:
    IUITreeNode* mOwner;

    NS_DECLARE_REFLECTION(Animatable, Freezable)
};

NS_WARNING_POP

}

#endif
