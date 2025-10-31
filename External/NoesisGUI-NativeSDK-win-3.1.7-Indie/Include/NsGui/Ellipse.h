////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ELLIPSE_H__
#define __GUI_ELLIPSE_H__


#include <NsCore/Noesis.h>
#include <NsGui/Shape.h>
#include <NsDrawing/Rect.h>


namespace Noesis
{

class EllipseGeometry;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Draws an ellipse.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.shapes.ellipse.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Ellipse final: public Shape
{
public:
    Ellipse();
    ~Ellipse();

protected:
    /// From UIElement
    //@{
    void OnRender(DrawingContext* context) override;
    //@}

    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

private:
    Size GetMinimumSize() const;

private:
    Ptr<EllipseGeometry> mRenderGeometry;

    NS_DECLARE_REFLECTION(Ellipse, Shape)
};

}


#endif
