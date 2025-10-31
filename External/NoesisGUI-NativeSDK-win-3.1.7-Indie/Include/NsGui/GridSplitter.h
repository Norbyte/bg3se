////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GRIDSPLITTER_H__
#define __GUI_GRIDSPLITTER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Ptr.h>
#include <NsGui/Thumb.h>
#include <NsGui/GridLength.h>

namespace Noesis
{

NS_INTERFACE ILayerManager;
class Grid;
class BaseDefinition;
class Style;
class Control;
class TranslateTransform;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies whether a GridSplitter control redistributes space between rows or between columns.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum GridResizeDirection : int32_t
{
    /// Space is redistributed based on the values of the HorizontalAlignment, VerticalAlignment,
    /// ActualWidth, and ActualHeight properties of the GridSplitter.
    GridResizeDirection_Auto = 0,

    /// Space is redistributed between columns.
    GridResizeDirection_Columns = 1,

    /// Space is redistributed between rows.
    GridResizeDirection_Rows = 2
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies the rows or columns that are resized by a GridSplitter control.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum GridResizeBehavior : int32_t
{
    /// Space is redistributed based on the value of the HorizontalAlignment and
    /// VerticalAlignment properties.
    GridResizeBehavior_BasedOnAlignment = 0,

    /// For a horizontal GridSplitter, space is redistributed between the row that is specified
    /// for the GridSplitter and the next row that is below it. For a vertical GridSplitter,
    /// space is redistributed between the column that is specified for the GridSplitter and 
    /// the next column that is to the right.
    GridResizeBehavior_CurrentAndNext = 1,

    /// For a horizontal GridSplitter, space is redistributed between the row that is specified
    /// for the GridSplitter and the next row that is above it. For a vertical GridSplitter, 
    /// space is redistributed between the column that is specified for the GridSplitter and 
    /// the next column that is to the left.
    GridResizeBehavior_PreviousAndCurrent = 2,

    /// For a horizontal GridSplitter, space is redistributed between the rows that are above 
    /// and below the row that is specified for the GridSplitter. For a vertical GridSplitter,
    /// space is redistributed between the columns that are to the left and right of the column
    /// that is specified for the GridSplitter.
    GridResizeBehavior_PreviousAndNext = 3
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the control that redistributes space between columns or rows of a Grid control.
///
/// .. code-block:: xml
///
///    <Grid>
///      <Grid.ColumnDefinitions>
///        <ColumnDefinition Width="1*"/>
///        <ColumnDefinition Width="4*"/>
///      </Grid.ColumnDefinitions>
/// 
///      <GridSplitter Width="5" HorizontalAlignment="Right" VerticalAlignment="Stretch"/>
///    </Grid>
///
/// .. code-block:: xml
///
///    <Grid>
///      <Grid.RowDefinitions>
///        <RowDefinition Height="5*"/>
///        <RowDefinition Height="2*"/>
///      </Grid.RowDefinitions>
/// 
///      <GridSplitter Height="5" HorizontalAlignment="Stretch" VerticalAlignment="Bottom"
///        ShowsPreview="True"/>
///    </Grid>
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.gridsplitter.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API GridSplitter : public Thumb
{
public:
    GridSplitter();
    ~GridSplitter();

    /// Gets or sets the distance that user must drag the GridSplitter control to resize
    /// columns or rows.
    //@{
    float GetDragIncrement() const;
    void SetDragIncrement(float value);
    //@}

    /// Gets or sets GridSplitter's step size when moving it with arrow keys.
    //@{
    float GetKeyboardIncrement() const;
    void SetKeyboardIncrement(float value);
    //@}

    /// Gets or sets whether to resize columns or rows.
    //@{
    GridResizeDirection GetResizeDirection() const;
    void SetResizeDirection(GridResizeDirection value);
    //@}

    /// Gets or sets whichs columns or rows to resize relative to the column or row in which
    /// the GridSplitter control is definied.
    //@{
    GridResizeBehavior GetResizeBehavior() const;
    void SetResizeBehavior(GridResizeBehavior value);
    //@}

    /// Gets or sets whether column or row size should be updated as the GridSplitter
    /// is being dragged.
    //@{
    bool GetShowsPreview() const;
    void SetShowsPreview(bool value);
    //@}

    /// Gets or sets the Style of the GridSplitter control preview which is displayed
    /// when the ShowsPreview property is set to true.
    //@{
    Style* GetPreviewStyle() const;
    void SetPreviewStyle(Style* value);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* DragIncrementProperty;
    static const DependencyProperty* KeyboardIncrementProperty;
    static const DependencyProperty* ResizeDirectionProperty;
    static const DependencyProperty* ResizeBehaviorProperty;
    static const DependencyProperty* ShowsPreviewProperty;
    static const DependencyProperty* PreviewStyleProperty;
    //@}

protected:
    /// From UIElement
    //@{
    void OnKeyDown(const KeyEventArgs& e) override;
    void OnRenderSizeChanged(const SizeChangedInfo& info) override;
    //@}

private:
    // Thumb event handlers
    void OnDragStarted(const DragStartedEventArgs&);
    void OnDragDelta(const DragDeltaEventArgs&);
    void OnDragCompleted(const DragCompletedEventArgs&);

    // Return actual values when ResizeBehavior is BasedOnAlignment and ResizeDirection is Auto
    GridResizeDirection GetActualResizeDirection() const;
    GridResizeBehavior GetActualResizeBehavior() const;

    // Wrapper around the GetColumnDefinitions/GetRowDefinitions
    int GetBaseDefinitionCount();
    BaseDefinition* GetBaseDefinition(int index);

    // Return ActualWidth/ActualHeight based on ResizeDirection
    float GetBaseDefinitionSize(BaseDefinition* baseDefinition) const;

    // Resize the columns/rows
    void Resize(float horiz, float vert);
    bool KeyboardResize(float horiz, float vert);
    void CancelResize();

    void CacheResizeData();

private:
    struct ResizeData;
    ResizeData* mResizeData;

    NS_DECLARE_REFLECTION(GridSplitter, Thumb)
};

NS_WARNING_POP

}

NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CORE_API, Noesis::GridResizeDirection)
NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CORE_API, Noesis::GridResizeBehavior)

#endif
