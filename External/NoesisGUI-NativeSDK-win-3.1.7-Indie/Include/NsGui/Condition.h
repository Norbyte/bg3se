////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CONDITION_H__
#define __GUI_CONDITION_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/String.h>
#include <NsGui/CoreApi.h>
#include <NsGui/IComponentInitializer.h>
#include <NsGui/IUITreeNode.h>


namespace Noesis
{

class DependencyProperty;
class BaseBinding;
class Condition;

template<class T> class UICollection;
typedef UICollection<Condition> ConditionCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a condition for the MultiTrigger and the MultiDataTrigger, which apply changes to
/// property values based on a set of conditions.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.condition.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Condition final: public BaseComponent, public IUITreeNode
{
public:
    Condition();
    ~Condition();

    static void SealConditions(ConditionCollection* conditions);

    /// Gets or sets the binding that produces the property value of the data object.
    //@{
    BaseBinding* GetBinding() const;
    void SetBinding(BaseBinding* binding);
    //@}

    /// Gets or sets the property of the condition. This is only applicable to MultiTrigger objects
    //@{
    const DependencyProperty* GetProperty() const;
    void SetProperty(const DependencyProperty* property);
    //@}

    /// Gets or sets the name of the object with the property that causes the associated setters to 
    /// be applied. This is only applicable to MultiTrigger objects. 
    //@{
    const char* GetSourceName() const;
    void SetSourceName(const char* name);
    //@}

    /// Gets or sets the value of the condition.
    //@{
    BaseComponent* GetValue() const;
    void SetValue(BaseComponent* object);
    //@}

    /// Seals this condition
    void Seal();

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
    IUITreeNode* mOwner;

    Ptr<BaseBinding> mBinding;
    const DependencyProperty* mProperty;
    Ptr<BaseComponent> mValue;
    String mSourceName;

    bool mIsSealed;

    NS_DECLARE_REFLECTION(Condition, BaseComponent)
};

NS_WARNING_POP

}


#endif
