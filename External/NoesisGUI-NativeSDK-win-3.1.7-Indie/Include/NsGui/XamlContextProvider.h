////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_XAMLCONTEXTPROVIDER_H__
#define __GUI_XAMLCONTEXTPROVIDER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ValueTargetProvider.h>


namespace Noesis
{

class Type;
class XamlContext;
struct Uri;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides information about XAML context during MarkupExtension *ProvideValue* calls.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API XamlContextProvider: public ValueTargetProvider
{
public:
    XamlContextProvider(BaseComponent* targetObject, const DependencyProperty* targetProperty,
        const XamlContext* context);

    /// Gets internal xaml context
    const XamlContext* GetContext() const;

    /// Gets the base uri of the current xaml context
    const Uri& GetBaseUri() const;

    /// Resolves a named XAML type to the corresponding Type. Returns null when type is not found
    const Type* ResolveType(const char* qualifiedTypeName) const;

    /// Retrieves a XAML namespace identifier for the specified prefix string. Returns null when
    /// prefix is not defined
    const char* GetNamespace(const char* prefix) const;

    /// Finds an object from a name reference. Returns null when object is not found
    BaseComponent* FindName(const char* name) const;

private:
    const XamlContext* mContext;

    NS_DECLARE_REFLECTION(XamlContextProvider, ValueTargetProvider)
};

NS_WARNING_POP

}


#endif
