////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TICKBAR_H__
#define __GUI_TICKBAR_H__


#include <NsCore/Noesis.h>
#include <NsCore/Vector.h>
#include <NsGui/CoreApi.h>
#include <NsGui/FrameworkElement.h>


namespace Noesis
{

class Brush;
class StreamGeometry;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that draws a set of tick marks for a Slider control.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.tickbar.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TickBar: public FrameworkElement
{
public:
    TickBar();
    ~TickBar();

    /// Gets or sets the Brush that is used to draw the tick marks
    //@{
    Brush* GetFill() const;
    void SetFill(Brush* fill);
    //@}

    /// Gets or sets the direction of increasing value of the tick marks
    //@{
    bool GetIsDirectionReversed() const;
    void SetIsDirectionReversed(bool directionReversed);
    //@}

    /// Gets or sets whether the TickBar displays a selection range
    //@{
    bool GetIsSelectionRangeEnabled() const;
    void SetIsSelectionRangeEnabled(bool selectionRangeEnabled);
    //@}

    /// Gets or sets the maximum value that is possible for a tick mark
    //@{
    float GetMaximum() const;
    void SetMaximum(float maximum);
    //@}

    /// Gets or sets the minimum value that is possible for a tick mark
    //@{
    float GetMinimum() const;
    void SetMinimum(float minimum);
    //@}

    /// Gets or sets where tick marks appear relative to a Track of a Slider control
    //@{
    TickBarPlacement GetPlacement() const;
    void SetPlacement(TickBarPlacement placement);
    //@}

    /// Gets or sets a space buffer for the area that contains the tick marks that are specified
    /// for a TickBar
    //@{
    float GetReservedSpace() const;
    void SetReservedSpace(float reservedSpace);
    //@}

    /// Gets or sets the end point of a selection
    //@{
    float GetSelectionEnd() const;
    void SetSelectionEnd(float selectionEnd);
    //@}

    /// Gets or sets the start point of a selection
    //@{
    float GetSelectionStart() const;
    void SetSelectionStart(float selectionStart);
    //@}

    /// Gets or sets the interval between tick marks
    //@{
    float GetTickFrequency() const;
    void SetTickFrequency(float tickFrequency);
    //@}

    /// Gets or sets the positions of the tick marks. For example, "0.5 1.0 43" or "0.5,1.0,43"
    //@{
    const char* GetTicks() const;
    void SetTicks(const char* ticks);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* FillProperty;
    static const DependencyProperty* IsDirectionReversedProperty;
    static const DependencyProperty* IsSelectionRangeEnabledProperty;
    static const DependencyProperty* MaximumProperty;
    static const DependencyProperty* MinimumProperty;
    static const DependencyProperty* PlacementProperty;
    static const DependencyProperty* ReservedSpaceProperty;
    static const DependencyProperty* SelectionEndProperty;
    static const DependencyProperty* SelectionStartProperty;
    static const DependencyProperty* TickFrequencyProperty;
    static const DependencyProperty* TicksProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From UIElement
    //@{
    void OnRender(DrawingContext* context) override;
    //@}
    
    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize) override;
    void OnTemplatedParentChanged(FrameworkElement* oldParent,
        FrameworkElement* newParent) override;
    //@}

private:
    friend class TickBarTest;

    void GetRenderInfo(float& range, float& offset, float& tickLength,
        Vector2& origin, Vector2& tickDirection, Vector2& tickBarDirection);
    bool UpdateFixedTicks();
    bool UpdateTicks(float range, float offset);
    bool UpdateSelection(float range, float offset);
    void UpdateGeometry(float range, float offset, float tickLength, const Vector2& origin,
        const Vector2& tickDirection, const Vector2& tickBarDirection);

    void RegisterBindings();
    void BindToTemplatedParent(const char* path, const DependencyProperty* dp);
    void BindReservedSpace();

private:
    int mUpdateFlags;

    float mPreviousRange;
    float mPreviousOffset;

    typedef Vector<float> Ticks;
    Ticks mFixedTicks;
    Ticks mTicksInternal;

    float mSelectionStartInternal;
    float mSelectionEndInternal;

    Ptr<StreamGeometry> mGeometry;

    bool mReservedSpaceBinded;

    NS_DECLARE_REFLECTION(TickBar, FrameworkElement)
};

NS_WARNING_POP

}

#endif
