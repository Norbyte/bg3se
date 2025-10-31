////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASEKEYFRAME_H__
#define __GUI_BASEKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/Freezable.h>
#include <NsGui/IUITreeNode.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class KeyTime;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for KeyFrame classes
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API BaseKeyFrame: public Freezable, public IUITreeNode
{
public:
    BaseKeyFrame();
    ~BaseKeyFrame();

    /// Gets or sets the time at which the key frame's target *Value* should be reached
    //@{
    const KeyTime& GetKeyTime() const;
    void SetKeyTime(const KeyTime& time);
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

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* KeyTimeProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    void OnObjectValueSet(BaseComponent* oldValue, BaseComponent* newValue) override;
    //@}

private:
    IUITreeNode* mOwner;

    NS_DECLARE_REFLECTION(BaseKeyFrame, Freezable)

};

NS_WARNING_POP

}

#endif
