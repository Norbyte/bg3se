////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MATRIXTRANSFORM_H__
#define __GUI_MATRIXTRANSFORM_H__


#include <NsCore/Noesis.h>
#include <NsGui/Transform.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Creates an arbitrary affine matrix transformation that is used to manipulate objects or
/// coordinate systems in a 2D plane.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.matrixtransform.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API MatrixTransform: public Transform
{
public:
    MatrixTransform();
    MatrixTransform(const Transform2& matrix);
    ~MatrixTransform();

    /// Parses a string value to create an instance of a MatrixTransform
    static bool TryParse(const char* str, Ptr<MatrixTransform>& result);

    /// Gets or sets the 2D matrix that represents this MatrixTransform
    //@{
    const Transform2& GetMatrix() const;
    void SetMatrix(const Transform2& matrix);
    //@}

    /// From Transform
    //@{
    Transform2 GetTransform() const override;
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<MatrixTransform> Clone() const;
    Ptr<MatrixTransform> CloneCurrentValue() const;
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
    static const DependencyProperty* MatrixProperty;
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
        UpdateFlags_Matrix
    };

    NS_DECLARE_REFLECTION(MatrixTransform, Transform)
};

}


#endif
