////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_STATICRESOURCEEXTENSION_H__
#define __GUI_STATICRESOURCEEXTENSION_H__


#include <NsCore/Noesis.h>
#include <NsCore/String.h>
#include <NsGui/MarkupExtension.h>


namespace Noesis
{

template<class T> class Ptr;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements a markup extension that supports static resource references made from XAML.
///
/// Provides a value for any XAML property attribute by looking up a reference to an already defined
/// resource. Lookup behavior for that resource is analogous to load-time lookup, which will look
/// for resources that were previously loaded from the markup of the current XAML page as well as
/// other application sources, and will generate that resource value as the property value in the
/// run-time objects.
///
/// .. code-block:: xml
///
///      <Setter Property="FocusVisualStyle" Value="{StaticResource CheckRadioFocusVisual}"/>
/// 
/// http://msdn.microsoft.com/en-us/library/system.windows.staticresourceextension.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API StaticResourceExtension: public MarkupExtension
{
public:
    StaticResourceExtension() = default;
    StaticResourceExtension(const char* key);

    /// Gets or sets the key value passed by this static resource reference. They key is used to
    /// return the object matching that key in resource dictionaries
    //@{
    BaseComponent* GetResourceKey() const;
    void SetResourceKey(BaseComponent* key);
    //@}

    /// From MarkupExtension
    //@{
    Ptr<BaseComponent> ProvideValue(const ValueTargetProvider* provider) override;
    //@}

private:
    FixedString<128> mResourceKey;

    NS_DECLARE_REFLECTION(StaticResourceExtension, MarkupExtension)
};

NS_WARNING_POP

}


#endif
