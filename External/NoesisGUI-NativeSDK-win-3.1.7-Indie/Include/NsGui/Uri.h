////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_URI_H__
#define __GUI_URI_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/String.h>
#include <NsGui/ProvidersApi.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a representation of a uniform resource identifier and easy access to parts of the URI.
///
/// In NoesisGUI, uniform resource identifiers (URIs) are used to identify and load resources like
/// *Images*, *Fonts* and *Dictionaries*. Two kind of URIs are supported: absolute and relative.
///
/// Non-absolute URIs are considered relative to the location of the XAML that contains the
/// reference. For example:
///
/// .. code-block:: xml
///
///    <Image Source="Images/icon.png" />
///
/// Absolute URIs start with '/' and might be preceded by *pack://application:,,,* and/or a
/// reference to an assembly like */Assembly;component*:
///
/// .. code-block:: xml
///
///    <Image Source="pack://application:,,,/Images/icon.png" />
///    <Image Source="pack://application:,,,/Assembly;component/Images/icon.png" />
///    <Image Source="/Assembly;component/Images/icon.png" />
///    <Image Source="/Images/icon.png" />
///
/// Paths beginning with a drive letter are also considered absolute:
/// 
/// .. code-block:: xml
///
///    <Image Source="C:/Images/icon.png" />
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.uri
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_PROVIDERS_API Uri
{
    /// Constructor for empty URI
    Uri();

    /// Constructs from string that identifies the resource to be represented
    /// The the following string formats are supported:
    ///   1. "RelativePath/resource.ext"
    ///   2. "/ReferencedAssembly;component/LocalPath/resource.ext"
    ///   3. "pack://application:,,,/ReferencedAssembly;component/LocalPath/resource.ext"
    Uri(const char* uri);

    /// Constructor from base URI and relative URI string
    Uri(const Uri& baseUri, const char* uri);

    /// Indicates if this Uri is invalid
    bool IsValid() const;

    /// Indicates if this Uri is absolute
    bool IsAbsolute() const;

    /// Gets the original string that was passed to this Uri constructor
    const char* Str() const;

    /// Gets assembly part of the Uri
    void GetAssembly(BaseString& assembly) const;

    /// Gets path part of the Uri
    void GetPath(BaseString& path) const;

    /// Generates a string representation of the Uri
    String ToString() const;

    bool operator==(const Uri& uri) const;
    bool operator!=(const Uri& uri) const;

public:
    FixedString<512> mUri;
    uint16_t mAssemblyLen;
    bool mIsValid;
    bool mIsAbsolute;

    NS_DECLARE_REFLECTION(Uri, NoParent)
};

NS_WARNING_POP

}


#endif
