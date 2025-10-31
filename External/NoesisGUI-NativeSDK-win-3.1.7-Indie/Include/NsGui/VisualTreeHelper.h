////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VISUALTREEHELPER_H__
#define __GUI_VISUALTREEHELPER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

class Visual;
class Geometry;
struct HitTestResult;
struct Rect;
struct Point;
struct Size;
enum HitTestFilterBehavior: int32_t;
enum HitTestResultBehavior: int32_t;

template<class T> class Delegate;
typedef Delegate<HitTestFilterBehavior (Visual* target)> HitTestFilterCallback;
typedef Delegate<HitTestResultBehavior (const HitTestResult& result)> HitTestResultCallback;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides utility methods that perform common tasks involving nodes in a visual tree.
///
/// http://msdn.microsoft.com/en-us/library/System.Windows.Media.VisualTreeHelper.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API VisualTreeHelper
{
    /// Returns the root of the visual tree where the specified visual is connected to
    static Visual* GetRoot(const Visual* visual);

    /// Returns the parent of the visual object
    static Visual* GetParent(const Visual* visual);

    /// Returns the number of children that a parent visual contains
    static uint32_t GetChildrenCount(const Visual* visual);

    /// Returns the child visual object of a parent at the specified index
    static Visual* GetChild(const Visual* visual, uint32_t childIndex);

    /// Returns the cached bounding box rectangle for the visual
    static Rect GetContentBounds(const Visual* visual);
    
    /// Returns the union of all the content bounding boxes for all visual object descendants,
    /// which includes the content bounding box of the visual itself
    static Rect GetDescendantBounds(const Visual* visual);

    /// Returns the minimum z value of the union of all the content bounding boxes for all visual
    /// object descendants, which includes the content bounding box of the visual itself
    static float GetDescendantBoundsMinZ(const Visual* visual);

    /// Returns the maximum z value of the union of all the content bounding boxes for all visual
    /// object descendants, which includes the content bounding box of the visual itself
    static float GetDescendantBoundsMaxZ(const Visual* visual);

    /// Returns the offset of the visual
    static const Point& GetOffset(const Visual* visual);

    /// Returns the size of the visual
    static const Size& GetSize(const Visual* visual);

    /// Returns the clip geometry of the visual
    static Geometry* GetClip(const Visual* visual);

    /// Returns the top-most visual object of a hit test. The coordinate value you pass as the
    /// point parameter has to be relative to the coordinate space of the visual object
    static HitTestResult HitTest(Visual* visual, const Point& point);

    /// Returns the top-most visual object of a hit test. The coordinate value you pass as the
    /// point parameter has to be relative to the coordinate space of the visual object.
    /// The specified callback will be called each time a visual is hit while traversing the tree
    static void HitTest(Visual* visual, const Point& point,
        const HitTestFilterCallback& hitTestFilter, const HitTestResultCallback& hitTestResult);
};

}


#endif
