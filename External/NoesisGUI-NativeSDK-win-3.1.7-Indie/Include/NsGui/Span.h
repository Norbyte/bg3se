////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SPAN_H__
#define __GUI_SPAN_H__


#include <NsCore/Noesis.h>
#include <NsGui/Inline.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Groups other Inline content elements.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.documents.span.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Span: public Inline
{
public:
    Span();
    Span(Inline* childInline);
    ~Span();

    /// Gets a collection containing the top-level Inline elements that comprise the Span's contents
    InlineCollection* GetInlines() const;

protected:
    /// From DependencyObject
    //@{
    void OnInit() override;
    //@}

    // From Visual
    //@{
    void OnConnectToView(IView* view) override;
    void OnDisconnectFromView() override;
    //@}

    /// From FrameworkElement
    //@{
    void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const override;
    uint32_t GetLogicalChildrenCount() const override;
    Ptr<BaseComponent> GetLogicalChild(uint32_t index) const override;
    //@}

private:
    void EnsureInlines();
    void OnInlinesChanged(BaseComponent* sender, const NotifyCollectionChangedEventArgs& e);

private:
    Ptr<InlineCollection> mInlines;

    NS_DECLARE_REFLECTION(Span, Inline)
};

NS_WARNING_POP

}


#endif
