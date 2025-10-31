////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COMPOSITETRANSFORM_H__
#define __GUI_COMPOSITETRANSFORM_H__


#include <NsCore/Noesis.h>
#include <NsGui/Transform.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// This class lets you apply multiple different transforms to an object.
/// 
/// The transformations run in the following order: *scale* - *skew* - *rotate* - *translate*.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.compositetransform.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API CompositeTransform final: public Transform
{
public:
    CompositeTransform();
    ~CompositeTransform();

    /// Gets or sets the x-coordinate of the center point for all transforms
    //@{
    float GetCenterX() const;
    void SetCenterX(float centerX);
    //@}

    /// Gets or sets the y-coordinate of the center point for all transforms
    //@{
    float GetCenterY() const;
    void SetCenterY(float centerY);
    //@}

    /// Gets or sets the angle, in degrees, of clockwise rotation
    //@{
    float GetRotation() const;
    void SetRotation(float angle);
    //@}

    /// Gets or sets the x-axis scale factor. You can use this property to stretch or shrink an
    /// object horizontally
    //@{
    float GetScaleX() const;
    void SetScaleX(float scaleX);
    //@}

    /// Gets or sets the y-axis scale factor. You can use this property to stretch or shrink an
    /// object vertically
    //@{
    float GetScaleY() const;
    void SetScaleY(float scaleY);
    //@}

    /// Gets or sets the x-axis skew angle, which is measured in degrees counterclockwise from the
    /// y-axis
    //@{
    float GetSkewX() const;
    void SetSkewX(float angleX);
    //@}

    /// Gets or sets the y-axis skew angle, which is measured in degrees counterclockwise from the
    /// x-axis
    //@{
    float GetSkewY() const;
    void SetSkewY(float angleY);
    //@}

    /// Gets or sets the distance to translate along the x-axis
    //@{
    float GetTranslateX() const;
    void SetTranslateX(float x);
    //@}

    /// Gets or sets the distance to translate along the y-axis
    //@{
    float GetTranslateY() const;
    void SetTranslateY(float y);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<CompositeTransform> Clone() const;
    Ptr<CompositeTransform> CloneCurrentValue() const;
    //@}

    /// From Transform
    //@{
    Transform2 GetTransform() const override;
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
    static const DependencyProperty* RotationProperty;
    static const DependencyProperty* ScaleXProperty;
    static const DependencyProperty* ScaleYProperty;
    static const DependencyProperty* SkewXProperty;
    static const DependencyProperty* SkewYProperty;
    static const DependencyProperty* TranslateXProperty;
    static const DependencyProperty* TranslateYProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& e) override;
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
        UpdateFlags_Rotation,
        UpdateFlags_ScaleX,
        UpdateFlags_ScaleY,
        UpdateFlags_SkewX,
        UpdateFlags_SkewY,
        UpdateFlags_TranslateX,
        UpdateFlags_TranslateY
    };

    NS_DECLARE_REFLECTION(CompositeTransform, Transform)
};

}


#endif
