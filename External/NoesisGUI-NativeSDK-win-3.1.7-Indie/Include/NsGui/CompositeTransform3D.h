////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COMPOSITETRANSFORM3D_H__
#define __GUI_COMPOSITETRANSFORM3D_H__


#include <NsCore/Noesis.h>
#include <NsGui/Transform3D.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents 3-D scale, rotation, and translate transforms to be applied to an element.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
///      xmlns:noesis="clr-namespace:NoesisGUIExtensions;assembly=Noesis.GUI.Extensions">
///        <Rectangle Fill="Red" Width="400" Height="200">
///            <noesis:Element.Transform3D>
///                <noesis:CompositeTransform3D RotationY="-40" CenterX="200"/>
///           </noesis:Element.Transform3D>
///        </Rectangle>
///    </Grid>
///
/// https://docs.microsoft.com/en-us/uwp/api/windows.ui.xaml.media.media3d.compositetransform3d
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API CompositeTransform3D final: public Transform3D
{
public:
    CompositeTransform3D();
    ~CompositeTransform3D();

    /// Constructs a 3D transform matrix from the CompositeTransform3D property values
    static Transform3 ConstructTransform3DMatrix(
        float centerX, float centerY, float centerZ,
        float rotX, float rotY, float rotZ,
        float scaleX, float scaleY, float scaleZ,
        float transX, float transY, float transZ);

    /// Gets or sets the x-coordinate of the transformation center in pixels
    //@{
    float GetCenterX() const;
    void SetCenterX(float centerX);
    //@}

    /// Gets or sets the y-coordinate of the transformation center in pixels
    //@{
    float GetCenterY() const;
    void SetCenterY(float centerY);
    //@}

    /// Gets or sets the z-coordinate of the transformation center in pixels
    //@{
    float GetCenterZ() const;
    void SetCenterZ(float centerZ);
    //@}

    /// Gets or sets the number of degrees to rotate the object around the x-axis of rotation
    //@{
    float GetRotationX() const;
    void SetRotationX(float rotationX);
    //@}

    /// Gets or sets the number of degrees to rotate the object around the y-axis of rotation
    //@{
    float GetRotationY() const;
    void SetRotationY(float rotationY);
    //@}

    /// Gets or sets the number of degrees to rotate the object around the z-axis of rotation
    //@{
    float GetRotationZ() const;
    void SetRotationZ(float rotationZ);
    //@}
    /// Gets or sets the x-axis scale factor
    //@{
    float GetScaleX() const;
    void SetScaleX(float scaleX);
    //@}

    /// Gets or sets the y-axis scale factor
    //@{
    float GetScaleY() const;
    void SetScaleY(float scaleY);
    //@}

    /// Gets or sets the z-axis scale factor
    //@{
    float GetScaleZ() const;
    void SetScaleZ(float scaleZ);
    //@}

    /// Gets or sets the distance to translate along the x-axis in pixels
    //@{
    float GetTranslateX() const;
    void SetTranslateX(float transX);
    //@}

    /// Gets or sets the distance to translate along the y-axis in pixels
    float GetTranslateY() const;
    void SetTranslateY(float transY);
    //@}

    /// Gets or sets the distance to translate along the z-axis in pixels
    //@{
    float GetTranslateZ() const;
    void SetTranslateZ(float transZ);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<CompositeTransform3D> Clone() const;
    Ptr<CompositeTransform3D> CloneCurrentValue() const;
    //@}

    /// From Transform3D
    //@{
    Transform3 GetTransform() const override;
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
    static const DependencyProperty* CenterXProperty;
    static const DependencyProperty* CenterYProperty;
    static const DependencyProperty* CenterZProperty;
    static const DependencyProperty* RotationXProperty;
    static const DependencyProperty* RotationYProperty;
    static const DependencyProperty* RotationZProperty;
    static const DependencyProperty* ScaleXProperty;
    static const DependencyProperty* ScaleYProperty;
    static const DependencyProperty* ScaleZProperty;
    static const DependencyProperty* TranslateXProperty;
    static const DependencyProperty* TranslateYProperty;
    static const DependencyProperty* TranslateZProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

private:
    RenderProxyCreatorFlags mUpdateFlags;

    enum UpdateFlags
    {
        UpdateFlags_CenterX,
        UpdateFlags_CenterY,
        UpdateFlags_CenterZ,
        UpdateFlags_RotationX,
        UpdateFlags_RotationY,
        UpdateFlags_RotationZ,
        UpdateFlags_ScaleX,
        UpdateFlags_ScaleY,
        UpdateFlags_ScaleZ,
        UpdateFlags_TranslateX,
        UpdateFlags_TranslateY,
        UpdateFlags_TranslateZ
    };

    NS_DECLARE_REFLECTION(CompositeTransform3D, Transform3D)
};

}


#endif
