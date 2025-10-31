////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DYNAMICRESOURCEEXTENSION_H__
#define __GUI_DYNAMICRESOURCEEXTENSION_H__


#include <NsCore/Noesis.h>
#include <NsCore/String.h>
#include <NsGui/MarkupExtension.h>


namespace Noesis
{

template<class T> class Ptr;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements a markup extension that supports dynamic resource references made from XAML.
///
/// Dynamic resource references are necessary when referencing content that might change at
/// run-time. The reasons for the content change might be application-initiated or might be
/// external. One example is if you are including styles that rely on system resources that report
/// user-configured preferences, such as themes, font settings and so on. 
///
/// .. code-block:: xml
///
///     <Setter Property="BorderBrush" Value="{DynamicResource DefaultedBorderBrush}"/>
///
/// http://msdn.microsoft.com/en-us/library/system.windows.dynamicresourceextension.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API DynamicResourceExtension: public MarkupExtension
{
public:
    DynamicResourceExtension() = default;
    DynamicResourceExtension(const char* key);

    /// Gets or sets the key specified by this dynamic resource reference. The key is used to lookup
    /// a resource in resource dictionaries, by means of an intermediate expression
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

    NS_DECLARE_REFLECTION(DynamicResourceExtension, MarkupExtension)
};

NS_WARNING_POP

}


#endif
