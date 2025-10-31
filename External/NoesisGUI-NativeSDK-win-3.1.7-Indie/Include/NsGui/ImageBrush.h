////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IMAGEBRUSH_H__
#define __GUI_IMAGEBRUSH_H__


#include <NsCore/Noesis.h>
#include <NsGui/TileBrush.h>


namespace Noesis
{

class ImageSource;
class BrushShader;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Paints an area with an image.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.imagebrush.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ImageBrush final: public TileBrush
{
public:
    ImageBrush();
    ImageBrush(ImageSource* imageSource);
    ~ImageBrush();

    /// Gets or sets image source file
    //@{
    ImageSource* GetImageSource() const;
    void SetImageSource(ImageSource* imageSource);
    //@}

    /// Gets or sets brush shader
    //@{
    BrushShader* GetShader() const;
    void SetShader(BrushShader* shader);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<ImageBrush> Clone() const;
    Ptr<ImageBrush> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ImageSourceProperty;
    static const DependencyProperty* ShaderProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    bool OnSubPropertyChanged(const DependencyProperty* dp) override;
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

private:
    enum UpdateFlags
    {
        UpdateFlags_ImageSource = TileBrush::UpdateFlags_Sentinel
    };

    NS_DECLARE_REFLECTION(ImageBrush, TileBrush)
};

}


#endif
