////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_STYLE_H__
#define __GUI_STYLE_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>
#include <NsGui/CoreApi.h>
#include <NsGui/IUITreeNode.h>
#include <NsGui/ISealable.h>


namespace Noesis
{

class BaseSetter;
class BaseTrigger;
class Condition;
class ResourceDictionary;
class FrameworkElement;

template<class T> class UICollection;
typedef UICollection<BaseSetter> BaseSetterCollection;
typedef UICollection<BaseTrigger> TriggerCollection;
typedef UICollection<Condition> ConditionCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Enables the sharing of properties, resources, and event handlers between instances of a type.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Style: public BaseComponent, public ISealable, public IUITreeNode
{
public:
    Style();
    ~Style();

    /// Gets or sets the type for which this style is intended
    //@{
    const Type* GetTargetType() const;
    void SetTargetType(const Type* targetType);
    //@}

    /// Gets or sets a defined style that is the basis of the current style
    //@{
    Style* GetBasedOn() const;
    void SetBasedOn(Style* style);
    //@}

    /// Gets or sets the collection of resources that can be used within the scope of this style
    //@{
    ResourceDictionary* GetResources() const;
    void SetResources(ResourceDictionary* resources);
    //@}

    /// Gets a collection of Setter and EventSetter objects
    BaseSetterCollection* GetSetters() const;

    /// Gets a collection of TriggerBase objects that apply property values based on specified
    /// conditions
    TriggerCollection* GetTriggers() const;

    /// From ISealable
    //@{
    bool CanSeal() const final override;
    bool IsSealed() const final override;
    void Seal() final override;
    //@}

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const override;
    void SetNodeParent(IUITreeNode* parent) override;
    BaseComponent* FindNodeResource(const char* key, bool fullElementSearch) const override;
    BaseComponent* FindNodeName(const char* name) const override;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    Ptr<BaseComponent> FindInResources(const char* key) const;

    void EnsureResources() const;
    void EnsureSetters() const;
    void EnsureTriggers() const;

    bool CheckTargetType(const TypeClass* elementType) const;

    bool CheckBasedOnType() const;
    bool CheckBasedOnCircularity() const;
    void CheckSetters(BaseSetterCollection* setters) const;
    void CheckTriggers(TriggerCollection* triggers) const;
    void CheckConditions(ConditionCollection* conditions) const;

private:
    friend struct BaseStyleTriggerProvider;
    friend struct BaseStyleSetterProvider;
    friend class DynamicResourceExpression;
    friend class StaticResourceExtension;
    friend class FrameworkElement;

    IUITreeNode* mOwner;
    const Type* mTargetType;
    Ptr<Style> mBasedOn;
    mutable Ptr<ResourceDictionary> mResources;
    mutable Ptr<BaseSetterCollection> mSetters;
    mutable Ptr<TriggerCollection> mTriggers;
    bool mIsSealed;

    NS_DECLARE_REFLECTION(Style, BaseComponent)
};

NS_WARNING_POP

}


#endif
