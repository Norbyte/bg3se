////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TRACK_H__
#define __GUI_TRACK_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/FrameworkElement.h>


namespace Noesis
{

class RepeatButton;
class Thumb;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control primitive that handles the positioning and sizing of a Thumb
/// control and two RepeatButton controls that are used to set a Value.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.track.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Track: public FrameworkElement
{
public:
    Track();
    ~Track();

    /// Gets the RepeatButton that decreases the *Value* property of the Track
    /// \prop
    //@{
    RepeatButton* GetDecreaseRepeatButton() const;
    void SetDecreaseRepeatButton(RepeatButton* decButton);
    //@}

    /// Gets the RepeatButton that increases the *Value* property of the Track class
    /// \prop
    //@{
    RepeatButton* GetIncreaseRepeatButton() const;
    void SetIncreaseRepeatButton(RepeatButton* incButton);
    //@}

    /// Gets or sets if the direction of increasing *Value* is reversed from the default direction
    //@{
    bool GetIsDirectionReversed() const;
    void SetIsDirectionReversed(bool directionReversed);
    //@}

    /// Gets or sets the maximum possible *Value* of the Track
    //@{
    float GetMaximum() const;
    void SetMaximum(float maximum);
    //@}

    /// Gets or sets the minimum possible *Value* of the Track
    //@{
    float GetMinimum() const;
    void SetMinimum(float minimum);
    //@}

    /// Defines the different orientations that a control or layout can have
    //@{
    Orientation GetOrientation() const;
    void SetOrientation(Orientation orientation);
    //@}

    /// Gets the Thumb control that is used to change the *Value* of a Track
    //@{
    Thumb* GetThumb() const;
    void SetThumb(Thumb* thumb);
    //@}

    /// Gets or sets the current value of the Track as determined by the position of the Thumb
    //@{
    float GetValue() const;
    void SetValue(float val);
    //@}

    /// Gets or sets the size of the part of the scrollable content that is visible
    //@{
    float GetViewportSize() const;
    void SetViewportSize(float size);
    //@}
    
    /// Calculates the change in the *Value* of the Track when the Thumb moves
    float ValueFromDistance(float horizontal, float vertical) const;
    
    /// Calculates the distance value from a specified point along the Track
    float ValueFromPoint(const Point& point) const;

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* IsDirectionReversedProperty;
    static const DependencyProperty* MaximumProperty;
    static const DependencyProperty* MinimumProperty;
    static const DependencyProperty* OrientationProperty;
    static const DependencyProperty* ValueProperty;
    static const DependencyProperty* ViewportSizeProperty;
    //@}
    
protected:
    /// From DependencyObject
    //@{
    void OnInit() override;
    //@}

    /// From Visual
    //@{
    uint32_t GetVisualChildrenCount() const override;
    Visual* GetVisualChild(uint32_t index) const override;
    //@}

    /// From FrameworkElement
    //@{
    uint32_t GetLogicalChildrenCount() const override;
    Ptr<BaseComponent> GetLogicalChild(uint32_t index) const override;
    void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const override;
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

private:
    void ArrangeHorizontal(const Size& finalSize, float trackSize, float thumbPos,
        RepeatButton* decBtn, RepeatButton* incBtn);
    void ArrangeVertical(const Size& finalSize, float trackSize, float thumbPos,
        RepeatButton* decBtn, RepeatButton* incBtn);
    float ThumbSize(float finalSize, float trackSize, float desiredSize);

private:
    Ptr<Thumb> mThumb;
    Ptr<RepeatButton> mDecreaseButton;
    Ptr<RepeatButton> mIncreaseButton;

    NS_DECLARE_REFLECTION(Track, FrameworkElement)
};

NS_WARNING_POP

}


#endif
