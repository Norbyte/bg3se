////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INLINEUICONTAINER_H__
#define __GUI_INLINEUICONTAINER_H__


#include <NsCore/Noesis.h>
#include <NsGui/Inline.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

class UIElement;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// An inline-level flow content element which enables UIElement elements (i.e. a Button) to be
/// embedded in flow content.
///
/// .. code-block:: xml
///
///    <TextBlock >
///      <Run>
///        A UIElement element may be embedded directly in flow content
///        by enclosing it in an InlineUIContainer element
///      </Run>
///      <InlineUIContainer>
///        <Image Source="Images/item.png" Stretch="None"/>
///      </InlineUIContainer>
///    </TextBlock>
///
/// https://msdn.microsoft.com/en-us/library/system.windows.documents.inlineuicontainer.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API InlineUIContainer : public Inline
{
public:
    InlineUIContainer();
    InlineUIContainer(UIElement* child);
    ~InlineUIContainer();

    /// Gets or sets the UIElement hosted by the InlineUIContainer
    //@{
    UIElement* GetChild() const;
    void SetChild(UIElement* child);
    //@}

protected:
    /// From DependencyObject
    //@{
    void OnInit() override;
    //@}

    /// From FrameworkElement
    //@{
    void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const override;
    uint32_t GetLogicalChildrenCount() const override;
    Ptr<BaseComponent> GetLogicalChild(uint32_t index) const override;
    //@}

private:
    Ptr<UIElement> mChild;

    NS_DECLARE_REFLECTION(InlineUIContainer, Inline)
};

NS_WARNING_POP

}

#endif
