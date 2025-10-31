////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_RANGEBASE_H__
#define __GUI_RANGEBASE_H__


#include <NsCore/Noesis.h>
#include <NsGui/Control.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an element that has a value within a specific range.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.rangebase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API RangeBase: public Control
{
public:
    RangeBase();
    virtual ~RangeBase() = 0;

    /// Gets or sets a value to be added to or subtracted from the Value of a RangeBase control
    //@{
    float GetLargeChange() const;
    void SetLargeChange(float value);
    //@}

    /// Gets or sets the highest possible Value of the range element
    //@{
    float GetMaximum() const;
    void SetMaximum(float value);
    //@}

    /// Gets or sets the Minimum possible Value of the range element
    //@{
    float GetMinimum() const;
    void SetMinimum(float value);
    //@}

    /// Gets or sets a value to be added to or subtracted from the Value of a RangeBase control
    //@{
    float GetSmallChange() const;
    void SetSmallChange(float value);
    //@}

    /// Gets or sets the current magnitude of the range control
    //@{
    float GetValue() const;
    void SetValue(float value);
    //@}

    /// Occurs when the range value changes
    UIElement::RoutedEvent_<RoutedPropertyChangedEventHandler<float>::Handler> ValueChanged();

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* LargeChangeProperty;
    static const DependencyProperty* MaximumProperty;
    static const DependencyProperty* MinimumProperty;
    static const DependencyProperty* SmallChangeProperty;
    static const DependencyProperty* ValueProperty;
    //@}
    
    /// Routed Events
    //@{
    static const RoutedEvent* ValueChangedEvent;
    //@}
    
protected:
    virtual void OnMaximumValueChanged(float oldMaximum, float newMaximum);
    virtual void OnMinimumValueChanged(float oldMinimum, float newMinimum);
    virtual void OnValueChanged(float oldValue, float newValue);

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}
    
    /// From Control
    //@{
    void UpdateVisualStates() override;
    //@}

private:
    static bool StaticCoerceMaxValue(const DependencyObject* object, const void* value,
        void* coercedValue);
    static bool StaticCoerceValue(const DependencyObject* object, const void* value,
        void* coercedValue);

    NS_DECLARE_REFLECTION(RangeBase, Control)
};

}


#endif
