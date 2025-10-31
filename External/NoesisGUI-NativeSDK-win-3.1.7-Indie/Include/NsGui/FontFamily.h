////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_FONTFAMILY_H__
#define __GUI_FONTFAMILY_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/String.h>
#include <NsCore/Vector.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Uri.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a family of related fonts.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.media.fontfamily.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API FontFamily: public BaseComponent
{
public:
    FontFamily();
    FontFamily(const char* source);
    FontFamily(const Uri& baseUri, const char* source);
    ~FontFamily();

    /// Gets the base resource identifier used to resolve font family names
    const Uri& GetBaseUri() const;

    /// Gets font family text used to construct the FontFamily object
    const char* GetSource() const;

    /// Gets the number of fonts in the family
    uint32_t GetNumFonts() const;

    /// Gets the base Uri for specified font. Returns 0 for system fonts
    const Uri& GetFontPath(uint32_t index) const;

    /// Gets the name for specified font
    const char* GetFontName(uint32_t index) const;

    /// From BaseObject
    //@{
    String ToString() const override;
    //@}

private:
    void FindFonts();

private:
    Uri mBaseUri;
    String mSource;

    struct Font
    {
        Uri path;
        String name;
    };

    Vector<Font> mFonts;

    NS_DECLARE_REFLECTION(FontFamily, BaseComponent)
};

NS_WARNING_POP

}

#endif
