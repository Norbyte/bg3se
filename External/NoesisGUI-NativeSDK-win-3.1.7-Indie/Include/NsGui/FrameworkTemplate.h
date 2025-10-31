////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_FRAMEWORKTEMPLATE_H__
#define __GUI_FRAMEWORKTEMPLATE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>
#include <NsCore/Delegate.h>
#include <NsGui/CoreApi.h>
#include <NsGui/INameScope.h>
#include <NsGui/ISealable.h>
#include <NsGui/IUITreeNode.h>


namespace Noesis
{

class Visual;
class FrameworkElement;
class NameScope;
class ResourceDictionary;
class BaseTrigger;

template<class T> class UICollection;
typedef UICollection<BaseTrigger> TriggerCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Enables the instantiation of a tree of elements for a template.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.frameworktemplate.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API FrameworkTemplate: public BaseComponent, public INameScope, public ISealable,
    public IUITreeNode
{
public:
    FrameworkTemplate();
    FrameworkTemplate(const FrameworkTemplate&) = delete;
    FrameworkTemplate& operator=(const FrameworkTemplate&) = delete;
    virtual ~FrameworkTemplate() = 0;

    /// Applies current template to the specified element
    Ptr<FrameworkElement> Apply(FrameworkElement* templatedParent) const;

    /// Gets or sets the collection of resources that can be used within the scope of this template
    //@{
    ResourceDictionary* GetResources() const;
    void SetResources(ResourceDictionary* resources);
    //@}

    /// Gets or sets the root node of the template
    //@{
    Visual* GetVisualTree() const;
    void SetVisualTree(Visual* root);
    //@}

    /// Finds the element associated with the specified name defined within this template
    BaseComponent* FindName(const char* name, FrameworkElement* templatedParent) const;

    /// Finds if element is a named object registered in the template. Returns null if not found
    const char* FindObject(BaseComponent* object) const;

    /// Gets template triggers if available
    virtual TriggerCollection* GetAvailableTriggers() const;

    /// From INameScope
    //@{
    BaseComponent* FindName(const char* name) const final;
    void RegisterName(const char* name, BaseComponent* object) final;
    void UnregisterName(const char* name) final;
    void UpdateName(const char* name, BaseComponent* object) final;
    INameScope::ChangedDelegate& NameScopeChanged() final;
    //@}

    /// From ISealable
    //@{
    bool CanSeal() const final override;
    bool IsSealed() const final override;
    void Seal() final override;
    //@}

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const final;
    void SetNodeParent(IUITreeNode* parent) final;
    BaseComponent* FindNodeResource(const char* key, bool fullElementSearch) const final;
    BaseComponent* FindNodeName(const char* name) const final;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const final;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

protected:
    virtual bool IsValidTemplatedParent(FrameworkElement* templatedParent) const;

private:
    void EnumNameScope(const char* name, BaseComponent* object, void* context) const;
    void EnsureNamescope() const;
    void EnsureResources() const;

private:
    friend class DynamicResourceExpression;
    friend class FrameworkElement;

    IUITreeNode* mOwner;

    mutable Ptr<NameScope> mNameScope;
    mutable Ptr<ResourceDictionary> mResources;
    Ptr<Visual> mVisualTree;

    bool mIsSealed;

    NS_DECLARE_REFLECTION(FrameworkTemplate, BaseComponent)
};

NS_WARNING_POP

}


#endif
