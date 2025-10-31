////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VISUALTRANSITION_H__
#define __GUI_VISUALTRANSITION_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/DependencyObject.h>
#include <NsGui/IUITreeNode.h>
#include <NsGui/Duration.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class EasingFunctionBase;
class Storyboard;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the visual behavior that occurs when the control transitions from
/// one state to another.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.visualtransition.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API VisualTransition: public DependencyObject, public IUITreeNode
{
public:
    VisualTransition();
    ~VisualTransition();

    /// Gets or sets the name of the VisualState to transition from
    //@{
    Symbol GetFrom() const;
    void SetFrom(Symbol from);
    //@}

    /// Gets or sets the name of the VisualState to transition to
    //@{
    Symbol GetTo() const;
    void SetTo(Symbol to);
    //@}

    /// Gets or sets the amount of time it takes to move from one state to another
    //@{
    const Duration& GetGeneratedDuration() const;
    void SetGeneratedDuration(const Duration& duration);
    //@}

    /// Gets or sets the easing function applied to the generated animations
    //@{
    EasingFunctionBase* GetGeneratedEasingFunction() const;
    void SetGeneratedEasingFunction(EasingFunctionBase* function);
    //@}

    /// Gets or sets the Storyboard that occurs when the transition occurs
    //@{
    Storyboard* GetStoryboard() const;
    void SetStoryboard(Storyboard* storyboard);
    //@}

    /// If the transition has no From or To, is a default transition
    bool IsDefault() const;

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
    void OnInit() override;
    void OnObjectValueSet(BaseComponent* oldValue, BaseComponent* newValue) override;
    //@}

private:
    Symbol mFrom;
    Symbol mTo;
    Duration mGeneratedDuration;
    Ptr<EasingFunctionBase> mGeneratedEasingFunction;
    Ptr<Storyboard> mStoryboard;

    IUITreeNode* mOwner;

    NS_DECLARE_REFLECTION(VisualTransition, DependencyObject)
};

NS_WARNING_POP

}


#endif
