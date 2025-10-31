////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MARKUPEXTENSION_H__
#define __GUI_MARKUPEXTENSION_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsGui/IUITreeNode.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

template<class T> class Ptr;
class ValueTargetProvider;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a base class for XAML markup extension implementations.
/// 
/// Markup extensions return objects in a more sophisticated way than type converters alone can
/// accomplish. Markup extensions use the known character sequence of an opening curly brace ``{``
/// to enter the markup extension scope, and a closing curly brace ``}`` to exit.
///
/// .. code-block:: xml
///
///      <Path Fill="{StaticResource GlyphBrush}"
///            Data="{Binding Path=Content, RelativeSource={RelativeSource TemplatedParent}}"/>
///
/// http://msdn.microsoft.com/en-us/library/system.windows.markup.markupextension.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API MarkupExtension: public BaseComponent, public IUITreeNode
{
public:
    MarkupExtension();

    /// Returns an object that is provided as the value of the target property for this extension
    virtual Ptr<BaseComponent> ProvideValue(const ValueTargetProvider* provider) = 0;

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const final;
    void SetNodeParent(IUITreeNode* parent) final;
    BaseComponent* FindNodeResource(const char* key, bool fullElementSearch) const final;
    BaseComponent* FindNodeName(const char* name) const final;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const final;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    IUITreeNode* mOwner;

    NS_DECLARE_REFLECTION(MarkupExtension, BaseComponent)
};

NS_WARNING_POP

}


#endif
