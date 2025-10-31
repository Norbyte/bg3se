////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ROTATETRANSFORM_H__
#define __GUI_ROTATETRANSFORM_H__


#include <NsCore/Noesis.h>
#include <NsGui/Transform.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Rotates an object clockwise about a specified point in 2D coordinate system.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.rotatetransform.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API RotateTransform final: public Transform
{
public:
    RotateTransform();
    RotateTransform(float angle);
    ~RotateTransform();

    /// Gets or sets the angle, in degrees, of clockwise rotation
    //@{
    float GetAngle() const;
    void SetAngle(float angle);
    //@}
    
    /// Gets or sets the x-coordinate of the rotation center point
    //@{
    float GetCenterX() const;
    void SetCenterX(float centerX);
    //@}
    
    /// Gets or sets the y-coordinate of the rotation center point
    //@{
    float GetCenterY() const;
    void SetCenterY(float centerY);
    //@}

    /// From Transform
    //@{
    Transform2 GetTransform() const override;
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<RotateTransform> Clone() const;
    Ptr<RotateTransform> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UnregisterRenderer(ViewId viewId) override;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* AngleProperty;
    static const DependencyProperty* CenterXProperty;
    static const DependencyProperty* CenterYProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& e) override;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

private:
    RenderProxyCreatorFlags mUpdateFlags;

    enum UpdateFlags
    {
        UpdateFlags_Angle,
        UpdateFlags_CenterX,
        UpdateFlags_CenterY
    };

    NS_DECLARE_REFLECTION(RotateTransform, Transform)
};

}


#endif
