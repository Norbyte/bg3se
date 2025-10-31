////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_KEYSPLINE_H__
#define __GUI_KEYSPLINE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Freezable.h>
#include <NsGui/AnimationApi.h>
#include <NsDrawing/Point.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// This class is used by a spline key frame to define animation progress.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.keyspline.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API KeySpline: public Freezable
{
public:
    KeySpline();
    KeySpline(const Point& controlPoint1, const Point& controlPoint2);
    KeySpline(float controlPoint1X, float controlPoint1Y, float controlPoint2X,
        float controlPoint2Y);

    /// Gets or sets the first control point used to define a Bezier curve that describes
    /// a KeySpline
    //@{
    const Point& GetControlPoint1() const;
    void SetControlPoint1(const Point& point);
    //@}

    /// Gets or sets the second control point used to define a Bezier curve that describes 
    /// a KeySpline
    //@{
    const Point& GetControlPoint2() const;
    void SetControlPoint2(const Point& point);
    //@}

    /// Calculates spline progress from a supplied linear progress
    float GetSplineProgress(float linearProgress) const;

    /// Tries to parse a KeySpline from a string
    static bool TryParse(const char* str, Ptr<KeySpline>& result);

    // Hides Freezable methods for convenience
    //@{
    Ptr<Freezable> Clone() const;
    Ptr<Freezable> CloneCurrentValue() const;
    //@}

private:
    /// From Freezable
    //@{
    void CloneCommonCore(const Freezable* source) override;
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

private:
    Point mControlPoint1;
    Point mControlPoint2;

    NS_DECLARE_REFLECTION(KeySpline, Freezable)
};

NS_WARNING_POP

}


#endif
