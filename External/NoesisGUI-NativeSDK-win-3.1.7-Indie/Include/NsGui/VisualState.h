////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VISUALSTATE_H__
#define __GUI_VISUALSTATE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/DependencyObject.h>
#include <NsGui/IUITreeNode.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class Storyboard;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the visual appearance of the control when it is in a specific state.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.visualstate.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API VisualState: public DependencyObject, public IUITreeNode
{
public:
    VisualState();
    ~VisualState();

    /// Gets the name of the VisualState
    //@{
    Symbol GetName() const;
    void SetName(Symbol name);
    //@}

    /// Gets or sets a Storyboard that defines the appearance of the control when it is the state 
    /// that is represented by the VisualState
    //@{
    Storyboard* GetStoryboard() const;
    void SetStoryboard(Storyboard* storyBoard);
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

protected:
    /// From DependencyObject
    //@{
    void OnInit() override;
    void OnObjectValueSet(BaseComponent* oldValue, BaseComponent* newValue) override;
    //@}

private:
    Symbol mName;
    Ptr<Storyboard> mStoryboard;

    IUITreeNode* mOwner;

    NS_DECLARE_REFLECTION(VisualState, DependencyObject)
};

NS_WARNING_POP

}


#endif
