////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CACHEDFONTPROVIDER_H__
#define __GUI_CACHEDFONTPROVIDER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Vector.h>
#include <NsCore/HashMap.h>
#include <NsCore/String.h>
#include <NsGui/FontProvider.h>
#include <NsGui/CachedFontProviderApi.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper base class for implementing font providers
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CACHEDFONTPROVIDER_API CachedFontProvider: public FontProvider
{
protected:
    /// Registers a font filename in the given folder. Each time this function is invoked, the
    /// given filename is opened and scanned (through OpenFont). It is recommended deferring
    /// this call as much as possible (for example, until ScanFolder is invoked)
    void RegisterFont(const Uri& folder, const char* filename);

    /// Registers a font face with given font properties. In comparison with the previous function
    /// this one doesn't open the filename to scan it. Always use this function if possible
    void RegisterFont(const Uri& folder, const char* filename, uint32_t index, const char* family,
        FontWeight weight, FontStretch stretch, FontStyle style);

    /// First time a font is requested from a folder, this function is invoked to give inheritors
    /// the opportunity to register faces found in that folder
    virtual void ScanFolder(const Uri& folder);

    /// Returns a stream to a previously registered filename
    virtual Ptr<Stream> OpenFont(const Uri& folder, const char* filename) const = 0;

    /// From FontProvider
    //@{
    FontSource MatchFont(const Uri& baseUri, const char* familyName, FontWeight& weight,
        FontStretch& stretch, FontStyle& style) override;
    bool FamilyExists(const Uri& baseUri, const char* familyName) override;
    //@}

private:
    struct Face
    {
        FixedString<128> filename;
        uint32_t faceIndex;

        FontWeight weight;
        FontStretch stretch;
        FontStyle style;
    };

    template<class T> using Bucket = HashBucket_KHV<String, T, CaseStringHashKeyInfo>;

    typedef Vector<Face> Family;
    typedef HashMap<String, Family, 0, Bucket<Family>> Families;
    typedef HashMap<String, Families, 0, Bucket<Families>> Folders;
    Folders mFolders;

    Families& GetFolder(const Uri& folder);
    FontSource FindBestMatch(const Uri& folder, const Family& faces, FontWeight& weight,
        FontStretch& stretch, FontStyle& style) const;
};

NS_WARNING_POP

}

#endif
