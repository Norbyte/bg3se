////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_PATHFIGURE_H__
#define __GUI_PATHFIGURE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Animatable.h>


namespace Noesis
{

struct Point;
class PathGeometry;
struct PathContext;
struct NotifyCollectionChangedEventArgs;
class PathSegment;
template<class T> class FreezableCollection;
typedef FreezableCollection<PathSegment> PathSegmentCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a subsection of a geometry, a single connected series of two-dimensional segments.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.pathfigure
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API PathFigure: public Animatable
{
public:
    PathFigure();
    ~PathFigure();

    /// Gets or sets a value that specifies whether first and last segments are connected.
    //@{
    bool GetIsClosed() const;
    void SetIsClosed(bool isClosed);
    //@}

    /// Gets or sets whether the contained area of this PathFigure is to be used for hit-testing,
    /// rendering, and clipping. The default value is true.
    //@{
    bool GetIsFilled() const;
    void SetIsFilled(bool isFilled);
    //@}

    /// Gets or sets the collection of segments that define the shape of this PathFigure object.
    //@{
    PathSegmentCollection* GetSegments() const;
    void SetSegments(PathSegmentCollection* segments);
    //@}

    /// Gets or sets the Point where the PathFigure begins. The default value is 0,0.
    //@{
    const Point& GetStartPoint() const;
    void SetStartPoint(const Point& startPoint);
    //@}

    /// Indicates if this figure does not contain any segments
    bool IsEmpty() const;

    /// Adds this PathFigure to the path
    void BuildPath(PathContext& path);

    // Hides Freezable methods for convenience
    //@{
    Ptr<PathFigure> Clone() const;
    Ptr<PathFigure> CloneCurrentValue() const;
    //@}

    /// From BaseObject
    //@{
    String ToString() const override;
    //@}

public:
    /// Dependency property
    static const DependencyProperty* IsClosedProperty;
    static const DependencyProperty* IsFilledProperty;
    static const DependencyProperty* SegmentsProperty;
    static const DependencyProperty* StartPointProperty;

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

private:
    friend class PathSegment;
    friend class PathGeometry;
    void SetPathGeometry(PathGeometry* pathGeometry);

    void InvalidatePath();

    void OnSegmentsCollectionChanged(BaseComponent* s, const NotifyCollectionChangedEventArgs& e);

private:
    PathGeometry* mPathGeometry;

    NS_DECLARE_REFLECTION(PathFigure, Animatable)
};

}


#endif
