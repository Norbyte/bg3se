////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_FONTPROVIDER_H__
#define __GUI_FONTPROVIDER_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsCore/Delegate.h>
#include <NsGui/FontProperties.h>


namespace Noesis
{

struct Uri;
class Stream;

struct FontSource
{
    // Font file (.ttf .otf .ttc)
    Ptr<Stream> file;

    // Index of the face in the font file, starting with value 0
    uint32_t faceIndex;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for implementing providers of fonts
////////////////////////////////////////////////////////////////////////////////////////////////////
class FontProvider: public BaseComponent
{
public:
    /// Finds the font in the given URI that best matches the specified properties
    /// Returns null stream when there are not matches
    virtual FontSource MatchFont(const Uri& baseUri, const char* familyName, FontWeight& weight,
        FontStretch& stretch, FontStyle& style) = 0;

    /// Returns true if the requested font family exists in given URI
    virtual bool FamilyExists(const Uri& baseUri, const char* familyName) = 0;

    /// Delegate to notify changes to the font file content
    typedef Delegate<void (const Uri& baseUri, const char* familyName, FontWeight weight,
        FontStretch stretch, FontStyle style)> FontChangedDelegate;
    FontChangedDelegate& FontChanged() { return mFontChanged; }
    void RaiseFontChanged(const Uri& baseUri, const char* familyName, FontWeight weight,
        FontStretch stretch, FontStyle style) { mFontChanged(baseUri, familyName, weight, stretch, 
        style); }

private:
    FontChangedDelegate mFontChanged;
};

}

#endif
