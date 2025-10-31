////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BITMAPIMAGE_H__
#define __GUI_BITMAPIMAGE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BitmapSource.h>
#include <NsDrawing/Int32Rect.h>


namespace Noesis
{

struct Uri;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a BitmapSource created from an image file located at the specifed Uri.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.media.imaging.bitmapimage.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BitmapImage final: public BitmapSource
{
public:
    BitmapImage();
    BitmapImage(const Uri& uriSource);
    ~BitmapImage();

    /// Gets or sets the image Uri source
    //@{
    const Uri& GetUriSource() const;
    void SetUriSource(const Uri& uriSource);
    //@}

    /// From Freezable
    //@{
    Ptr<BitmapImage> Clone() const;
    Ptr<BitmapImage> CloneCurrentValue() const;
    //@}

public:
    static const DependencyProperty* UriSourceProperty;

private:
    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UnregisterRenderer(ViewId viewId) override;
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    void CloneCommonCore(const Freezable* source) override;
    bool FreezeCore(bool isChecking) override;
    //@}

    /// From BitmapSource
    //@{
    float GetDpiXCore() const override;
    float GetDpiYCore() const override;
    int32_t GetPixelWidthCore() const override;
    int32_t GetPixelHeightCore() const override;
    //@}

    void UpdateImageInfo(const Uri& uri);

    static void OnTextureChanged(const Uri& uri);

private:
    friend class BitmapImageTest;

    RenderProxyCreatorFlags mUpdateFlags;

    enum UpdateFlags
    {
        UpdateFlags_Source,
        UpdateFlags_Rect
    };

    Int32Rect mRect;
    float mDpiScale;

    NS_DECLARE_REFLECTION(BitmapImage, BitmapSource)
};

NS_WARNING_POP

}


#endif
