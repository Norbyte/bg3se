////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TILEBRUSH_H__
#define __GUI_TILEBRUSH_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Brush.h>


namespace Noesis
{

struct Rect;
enum AlignmentX: int32_t;
enum AlignmentY: int32_t;
enum Stretch: int32_t;
enum TileMode: int32_t;
enum BrushMappingMode: int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes a way to paint a region by using one or more tiles.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.tilebrush.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TileBrush: public Brush
{
public:
    TileBrush();
    TileBrush(const TileBrush&) = delete;
    TileBrush& operator=(const TileBrush&) = delete;
    virtual ~TileBrush() = 0;

    /// Gets or sets the horizontal alignment of content in the TileBrush base tile
    //@{
    AlignmentX GetAlignmentX() const;
    void SetAlignmentX(AlignmentX value);
    //@}

    /// Gets or sets the vertical alignment of content in the TileBrush base tile
    //@{
    AlignmentY GetAlignmentY() const;
    void SetAlignmentY(AlignmentY value);
    //@}

    /// Gets or sets a value that specifies how the content of this TileBrush stretches to fit its 
    /// tiles
    //@{
    Stretch GetStretch() const;
    void SetStretch(Stretch value);
    //@}

    /// Gets or sets a value that specifies how a TileBrush fills the area that you are painting if 
    /// the base tile is smaller than the output area
    //@{
    TileMode GetTileMode() const;
    void SetTileMode(TileMode value);
    //@}

    /// Gets or sets the position and dimensions of the content in a TileBrush tile
    //@{
    const Rect& GetViewbox() const;
    void SetViewbox(const Rect& rect);
    //@}

    /// Gets or sets a value that specifies whether the Viewbox value is relative to the bounding
    /// box of the TileBrush contents or whether the value is absolute
    //@{
    BrushMappingMode GetViewboxUnits() const;
    void SetViewboxUnits(BrushMappingMode units);
    //@}

    /// Gets or sets the position and dimensions of the base tile for a TileBrush
    //@{
    const Rect& GetViewport() const;
    void SetViewport(const Rect& rect);
    //@}

    /// Gets or sets a *BrushMappingMode* enumeration that specifies whether the value of the 
    /// *Viewport*, which indicates the size and position of the TileBrush base tile, is relative to 
    /// the size of the output area
    //@{
    BrushMappingMode GetViewportUnits() const;
    void SetViewportUnits(BrushMappingMode units);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<TileBrush> Clone() const;
    Ptr<TileBrush> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* AlignmentXProperty;
    static const DependencyProperty* AlignmentYProperty;
    static const DependencyProperty* StretchProperty;
    static const DependencyProperty* TileModeProperty;
    static const DependencyProperty* ViewboxProperty;
    static const DependencyProperty* ViewboxUnitsProperty;
    static const DependencyProperty* ViewportProperty;
    static const DependencyProperty* ViewportUnitsProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    //@}

protected:
    enum UpdateFlags
    {
        UpdateFlags_AlignmentX = Brush::UpdateFlags_Sentinel,
        UpdateFlags_AlignmentY,
        UpdateFlags_Stretch,
        UpdateFlags_TileMode,
        UpdateFlags_Viewbox,
        UpdateFlags_ViewboxUnits,
        UpdateFlags_Viewport,
        UpdateFlags_ViewportUnits,

        UpdateFlags_Sentinel
    };

private:
    NS_DECLARE_REFLECTION(TileBrush, Brush)
};

}


#endif
