////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_PROGRESSBAR_H__
#define __GUI_PROGRESSBAR_H__


#include <NsCore/Noesis.h>
#include <NsGui/RangeBase.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Indicates the progress of an operation.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.progressbar.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ProgressBar: public RangeBase
{
public:
    ProgressBar();

    /// Gets or sets whether the ProgressBar shows actual values or generic, continuous progress 
    /// feedback
    //@{
    bool GetIsIndeterminate() const;
    void SetIsIndeterminate(bool value);
    //@}

    /// Gets or sets the orientation of a ProgressBar: horizontal or vertical.
    //@{
    Orientation GetOrientation() const;
    void SetOrientation(Orientation orientation);
    //@}

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* IsIndeterminateProperty;
    static const DependencyProperty* OrientationProperty;
    //@}

protected:
    /// From RangeBase
    //@{
    void OnMaximumValueChanged(float oldMaximum, float newMaximum) override;
    void OnMinimumValueChanged(float oldMinimum, float newMinimum) override;
    void OnValueChanged(float oldValue, float newValue) override;
    //@}

    /// From FrameworkElement
    //@{
    void OnTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkElement* oldRoot,
        FrameworkTemplate* newTemplate, FrameworkElement* newRoot) override;
    void UpdateVisualStates() override;
    //@}

    /// From DependencyProperty
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

private:
    void OnTrackSizeChanged(BaseComponent* o, const SizeChangedEventArgs& e);
    void UpdateIndicator();

private:
    Ptr<FrameworkElement> mTrack;
    Ptr<FrameworkElement> mIndicator;

    NS_DECLARE_REFLECTION(ProgressBar, RangeBase)
};

NS_WARNING_POP

}


#endif
