////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BITMAPSOURCE_H__
#define __GUI_BITMAPSOURCE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ImageSource.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a single, constant set of pixels at a certain size and resolution.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.media.imaging.bitmapsource.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BitmapSource: public ImageSource
{
public:
    BitmapSource();
    virtual ~BitmapSource() = 0;

    enum Format
    {
        Format_BGRA8,
        Format_BGR8,
        Format_RGBA8,
        Format_RGB8
    };

    /// Creates a new BitmapSource from an array of pixels that are stored in memory
    static Ptr<BitmapSource> Create(int32_t pixelWidth, int32_t pixelHeight, float dpiX,
        float dpiY, const uint8_t* buffer, int32_t stride, Format format);

    /// Horizontal DPI of the bitmap
    /// \prop
    float GetDpiX() const;

    /// Vertical DPI of the bitmap
    /// \prop
    float GetDpiY() const;

    /// Width, in pixels, of the bitmap
    /// \prop
    int32_t GetPixelWidth() const;

    /// Height, in pixels, of the bitmap
    /// \prop
    int32_t GetPixelHeight() const;

    /// Gets bitmap format
    /// \prop
    Format GetFormat() const;

    /// Copies the bitmap pixel data into an array of pixels with the specified stride,
    /// starting at the specified offset
    virtual void CopyPixels(uint8_t* buffer, uint32_t bufferSize, int32_t stride,
        int32_t offset) const;

    /// From Freezable
    //@{
    Ptr<BitmapSource> Clone() const;
    Ptr<BitmapSource> CloneCurrentValue() const;
    //@}

protected:
    /// From ImageSource
    //@{
    float GetWidthCore() const override;
    float GetHeightCore() const override;
    //@}

    /// Implemented by inheritors
    //@{
    virtual float GetDpiXCore() const = 0;
    virtual float GetDpiYCore() const = 0;

    virtual int32_t GetPixelWidthCore() const = 0;
    virtual int32_t GetPixelHeightCore() const = 0;

    virtual void CopyPixelsCore(uint8_t* buffer, uint32_t bufferSize, int32_t stride,
        int32_t offset) const;
    //@}

    NS_DECLARE_REFLECTION(BitmapSource, ImageSource)
};

}

#endif
