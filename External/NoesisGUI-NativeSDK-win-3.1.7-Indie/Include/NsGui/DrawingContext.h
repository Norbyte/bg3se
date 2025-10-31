////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DRAWINGCONTEXT_H__
#define __GUI_DRAWINGCONTEXT_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>
#include <NsGui/CoreApi.h>


#undef DrawText


namespace Noesis
{

struct Point;
struct Rect;
class Brush;
class Pen;
class Geometry;
class ImageSource;
class FormattedText;
class Transform;
class DrawingCommands;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes visual content using draw, push, and pop commands during UIElement *OnRender*.
///
/// When you use a DrawingContext object's draw commands, you are actually storing a set of
/// rendering instructions that will later be used by the graphics system; you are not drawing to
/// the screen in real-time. 
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.drawingcontext
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API DrawingContext: public BaseComponent
{
public:
    DrawingContext();
    ~DrawingContext();

    /// Draws a line between the specified points using the specified Pen
    void DrawLine(Pen* pen, const Point& p0, const Point& p1);

    /// Draws a rectangle with the specified Brush and Pen
    void DrawRectangle(Brush* brush, Pen* pen, const Rect& rect);

    /// Draws a rounded rectangle with the specified Brush and Pen
    void DrawRoundedRectangle(Brush* brush, Pen* pen, const Rect& rect, float rX, float rY);

    /// Draws an ellipse with the specified Brush and Pen
    void DrawEllipse(Brush* brush, Pen* pen, const Point& center, float rX, float rY);

    /// Draws the specified Geometry using the specified Brush and Pen
    void DrawGeometry(Brush* brush, Pen* pen, Geometry* geometry);

    /// Draws an image into the region defined by the specified Rect
    void DrawImage(ImageSource* imageSource, const Rect& rect);

    /// Draws formatted text at the specified location
    void DrawText(FormattedText* formattedText, const Rect& bounds);

    /// Pops the last opacity mask, opacity, clip, effect, or transform operation that was pushed
    /// onto the drawing context
    void Pop();

    /// Pushes the specified clip region onto the drawing context
    void PushClip(Geometry* clipGeometry);

    /// Pushes the specified Transform onto the drawing context
    void PushTransform(Transform* transform);

private:
    friend class UIElement;
    friend class DrawingContextTestBase;

    Ptr<DrawingCommands> mCommands;
    uint32_t mPushes;

    NS_DECLARE_REFLECTION(DrawingContext, BaseComponent)
};

NS_WARNING_POP

}

#endif
