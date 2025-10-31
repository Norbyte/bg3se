////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DECORATOR_H__
#define __GUI_DECORATOR_H__


#include <NsCore/Noesis.h>
#include <NsGui/FrameworkElement.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a base class for elements that apply effects onto or around a single child
/// element, such as Border or Viewbox.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.decorator.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Decorator: public FrameworkElement
{
public:
    Decorator(bool logicalChild = true);
    ~Decorator();

    /// Gets or sets child element
    //@{
    UIElement* GetChild() const;
    void SetChild(UIElement* child);
    //@}

protected:
    // Child property value changed event
    virtual void OnChildChanged(UIElement* oldChild, UIElement* newChild);

    /// From BaseRefCounted
    //@{
    int32_t OnDestroy() override;
    //@}

    /// From FrameworkElement
    //@{
    void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const override;
    uint32_t GetLogicalChildrenCount() const override;
    Ptr<BaseComponent> GetLogicalChild(uint32_t index) const override;
    //@}

private:
    // Indicates if child is also logical
    bool mIsLogicalChild;

    NS_DECLARE_REFLECTION(Decorator, FrameworkElement)
};

NS_WARNING_POP

}

#endif
