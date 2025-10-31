////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CROPPEDBITMAP_H__
#define __GUI_CROPPEDBITMAP_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BitmapSource.h>


namespace Noesis
{

struct Int32Rect;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides an image source that crops another BitmapSource.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
///      xmlns:b="http://schemas.microsoft.com/xaml/behaviors"
///      xmlns:noesis="clr-namespace:NoesisGUIExtensions;assembly=Noesis.GUI.Extensions">
///      <Grid.Resources>
///        <BitmapImage x:Key="aladin" UriSource="aladin.png"/>
///        <CroppedBitmap x:Key="aladin01" Source="{StaticResource aladin}" SourceRect="145,58,33,54"/>
///      </Grid.Resources>
///      <Image Source="{StaticResource aladin01}"/>
///    </Grid>
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.imaging.croppedbitmap
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API CroppedBitmap: public BitmapSource
{
public:
    CroppedBitmap();
    CroppedBitmap(BitmapSource* source, const Int32Rect& sourceRect);
    ~CroppedBitmap();

    /// Gets or sets the source for the bitmap
    //@{
    BitmapSource* GetSource() const;
    void SetSource(BitmapSource* source);
    //@}

    /// Gets or sets the rectangular area that the bitmap is cropped to. If SourceRect is set to
    /// the Empty rectangle it will render the entire image. The default is Empty.
    //@{
    const Int32Rect& GetSourceRect() const;
    void SetSourceRect(const Int32Rect& sourceRect);
    //@}

    /// From Freezable
    //@{
    Ptr<CroppedBitmap> Clone() const;
    Ptr<CroppedBitmap> CloneCurrentValue() const;
    //@}

public:
    static const DependencyProperty* SourceProperty;
    static const DependencyProperty* SourceRectProperty;

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
    bool OnSubPropertyChanged(const DependencyProperty* dp) override;
    //@}

    /// From BitmapSource
    //@{
    float GetDpiXCore() const override;
    float GetDpiYCore() const override;
    int32_t GetPixelWidthCore() const override;
    int32_t GetPixelHeightCore() const override;
    //@}

private:
    RenderProxyCreatorFlags mUpdateFlags;

    NS_DECLARE_REFLECTION(CroppedBitmap, BitmapSource)
};

NS_WARNING_POP

}


#endif
