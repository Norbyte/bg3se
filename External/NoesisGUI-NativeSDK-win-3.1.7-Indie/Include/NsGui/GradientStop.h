////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GRADIENTSTOP_H__
#define __GUI_GRADIENTSTOP_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Animatable.h>


namespace Noesis
{

struct Color;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes the location and color of a transition point in a gradient.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.gradientstop.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API GradientStop final: public Animatable
{
public:
    GradientStop();
    ~GradientStop();

    /// Gets or sets the color of the gradient stop
    //@{
    const Color& GetColor() const;
    void SetColor(const Color& color);
    //@}

    /// Gets or sets the location of the gradient stop within the gradient vector
    //@{
    float GetOffset() const;
    void SetOffset(float offset);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<GradientStop> Clone() const;
    Ptr<GradientStop> CloneCurrentValue() const;
    //@}

    /// From BaseObject
    //@{
    String ToString() const override;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ColorProperty;
    static const DependencyProperty* OffsetProperty;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    NS_DECLARE_REFLECTION(GradientStop, Animatable)
};

}


#endif
