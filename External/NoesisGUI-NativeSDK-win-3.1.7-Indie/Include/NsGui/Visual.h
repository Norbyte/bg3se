////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VISUAL_H__
#define __GUI_VISUAL_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/DependencyObject.h>
#include <NsDrawing/Point.h>
#include <NsDrawing/Size.h>
#include <NsDrawing/Rect.h>


namespace Noesis
{

class Visual;
class DrawingCommands;
class Effect;
class Geometry;
class Brush;
class Transform;
class Transform3D;
class RenderTreeUpdater;
class Matrix4;
class Transform3;
class View;
NS_INTERFACE IView;
enum HitTestFilterBehavior: int32_t;
enum HitTestResultBehavior: int32_t;
enum BitmapScalingMode: int32_t;
enum BlendingMode: int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Indicates the visual that was hit in a HitTest operation
////////////////////////////////////////////////////////////////////////////////////////////////////
struct HitTestResult
{
    Visual* visualHit;

    HitTestResult(): visualHit(0) { }
};

typedef Delegate<HitTestFilterBehavior (Visual* target)> HitTestFilterCallback;
typedef Delegate<HitTestResultBehavior (const HitTestResult& result)> HitTestResultCallback;
typedef Noesis::Delegate<void (Visual* visual)> SubtreeDrawingCommandsChangedDelegate;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides rendering support, which includes hit testing, coordinate transformation, and bounding
/// box calculations.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.visual.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Visual: public DependencyObject
{
public:
    Visual();
    Visual(const Visual&) = delete;
    Visual& operator=(const Visual&) = delete;
    virtual ~Visual() = 0;

    /// Determines whether the visual object is an ancestor of the descendant visual object
    /// This object is not considered to be an ancestor of *visual* if they are the same
    bool IsAncestorOf(const Visual* visual) const;

    /// Determines whether the visual object is a descendant of the ancestor visual object
    /// This object is not considered to be a descendant of *visual* if they are the same
    bool IsDescendantOf(const Visual* visual) const;

    /// Finds the common ancestor of two visuals objects
    /// If *visual* is the same object as this, the common ancestor will be the parent
    Visual* FindCommonVisualAncestor(const Visual* visual) const;

    /// Converts a Point in screen coordinates into a Point that represents the current coordinate
    /// system of the Visual
    Point PointFromScreen(const Point& point) const;
    
    /// Converts a Point that represents the current coordinate system of the Visual into a Point
    /// in screen coordinates
    Point PointToScreen(const Point& point) const;

    /// Returns a transform that can be used to transform coordinates from the Visual to the
    /// specified ancestor of the visual object
    Matrix4 TransformToAncestor(const Visual* ancestor) const;

    /// Returns a transform that can be used to transform coordinates from the Visual to the
    /// specified visual object descendant
    Matrix4 TransformToDescendant(const Visual* descendant) const;

    /// Returns a transform that can be used to transform coordinates from the Visual to the
    /// specified visual object
    Matrix4 TransformToVisual(const Visual* visual) const;

    /// Occurs when subtree render commands have changed
    SubtreeDrawingCommandsChangedDelegate& SubtreeDrawingCommandsChanged();

    /// Indicates if this visual is invalidated and should send updates to the render tree
    bool IsInvalidated() const;

    /// Indicates if this visual has been connected to a View
    bool IsConnectedToView() const;

    /// Gets the view where this visual is connected to
    IView* GetView() const;

    /// Indicates if a render node has been created in the RenderTree for this visual
    bool IsInRenderTree() const;

    /// Gets RenderTree's identifier
    uint32_t GetRenderTreeId() const;

    /// Pushes all the layout updates into the UI render thread queue to update render nodes
    void UpdateRender(RenderTreeUpdater& updater);

    /// Sets the flag that indicates that this Visual contains a VisualBrush
    void SetVisualBrushFlag();

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Protected functions used by child classes or VisualTreeHelper
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //@{
    /// Gets the parent of the visual object
    Visual* GetVisualParent() const;

    /// Sets up the parent-child relationship between two visuals
    void AddVisualChild(Visual* child);

    /// Removes the parent-child relationship between two visuals
    void RemoveVisualChild(Visual* child);

    /// Gets the bounding box rectangle
    Rect GetContentBounds() const;

    /// Returns a bounding box that encloses this visual and all its children
    Rect GetDescendantBounds() const;

    /// Returns the minimum z of a bounding box that encloses this visual and all its children
    float GetDescendantBoundsMinZ() const;

    /// Returns the maximum z of a bounding box that encloses this visual and all its children
    float GetDescendantBoundsMaxZ() const;

    /// Gets the root of the View tree where this visual is connected to
    /// \return Null if this visual is not connected to a View
    Visual* GetRoot() const;

    /// Gets the surface size of the View where this visual is connected to
    Size GetSurfaceSize() const;

    /// Gets or sets the BitmapScalingMode for the Visual
    //@{
    BitmapScalingMode GetVisualBitmapScalingMode() const;
    void SetVisualBitmapScalingMode(BitmapScalingMode mode);
    //@}

    /// Gets or sets a value indicating how the visual's contents are mixed with the background
    //@{
    BlendingMode GetVisualBlendingMode() const;
    void SetVisualBlendingMode(BlendingMode mode);
    //@}

    /// Gets or sets the clip region of the Visual as a Geometry value
    //@{
    Geometry* GetVisualClip() const;
    void SetVisualClip(Geometry* clip);
    //@}

    /// Gets or sets the bitmap effect to apply to this visual
    //@{
    Effect* GetVisualEffect() const;
    void SetVisualEffect(Effect* value);
    //@}

    /// Gets or sets the offset value of the visual object
    //@{
    const Point& GetVisualOffset() const;
    void SetVisualOffset(const Point& offset);
    //@}

    /// Gets or sets the size value of the visual object
    //@{
    const Size& GetVisualSize() const;
    void SetVisualSize(const Size& size);
    //@}

    /// Gets or sets the opacity of the Visual
    //@{
    float GetVisualOpacity() const;
    void SetVisualOpacity(float opacity);
    //@}

    /// Gets or sets the Brush value that represents the opacity mask of the Visual
    //@{
    Brush* GetVisualOpacityMask() const;
    void SetVisualOpacityMask(Brush* mask);
    //@}

    /// Gets or sets the Transform value for the Visual
    //@{
    Transform* GetVisualTransform() const;
    void SetVisualTransform(Transform* transform);
    //@}

    /// Gets or sets the Transform3D value for the Visual
    //@{
    Transform3D* GetVisualTransform3D() const;
    void SetVisualTransform3D(Transform3D* transform);
    //@}

    /// Disables PPAA generation on this visual node
    void DisablePPAA(bool disable);

    /// Invalidates PPAA generated geometries for this visual node
    void InvalidatePPAA();

    /// Invalidate visual render commands
    void InvalidateDrawingCommands() const;

    /// Indicates to the visual that content bounds changed
    void InvalidateContentBounds() const;

    /// Invalidates visual if the visual or its children intersect the invalidation rectangle
    void InvalidateRect(const Rect& rect) const;

    /// Invalidates children Z-order
    void InvalidateChildrenZOrder() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Protected virtual functions to be reimplemented by child classes
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //@{
    /// Gets the number of child elements for the Visual
    /// \remarks Each visual implementation will decide how to store visual children
    virtual uint32_t GetVisualChildrenCount() const;

    /// Returns the specified Visual in the parent VisualCollection
    /// \remarks Each visual implementation will decide how to store visual children
    virtual Visual* GetVisualChild(uint32_t index) const;

    /// Called when the VisualCollection of the visual object is modified
    virtual void OnVisualChildrenChanged(Visual* added, Visual* removed);

    /// Called when the parent of the visual object is changed
    virtual void OnVisualParentChanged(Visual* oldParent);

    /// Called when this visual is connected/disconnected to/from a parent that belongs to a View
    //@{
    virtual void OnConnectToView(IView* view);
    virtual void OnDisconnectFromView();
    //@}

    /// Called when children of this visual are connected/disconnected to/from render tree
    //@{
    virtual void OnConnectToViewChildren();
    virtual void OnDisconnectFromViewChildren();
    //@}

    /// Called when the VisualOffset of the visual object is modified
    virtual void OnVisualOffsetChanged();

    /// Called when the VisualSize of the visual object is modified
    virtual void OnVisualSizeChanged();

    /// Called when the VisualTransform of the visual object is modified
    virtual void OnVisualTransformChanged();

    /// Called when the VisualTransform3D of the visual object is modified
    virtual void OnVisualTransform3DChanged();

    /// Gets the bounding box rectangle
    virtual Rect GetContentBoundsCore() const;

    /// Function to be implemented by Inheriting classes. This function is invoked whenever
    /// the content bound is hit in a HitTest operation
    virtual HitTestResult HitTestCore(const Point& point);

    /// Updates render commands in the corresponding render node
    virtual DrawingCommands* GetDrawingCommands() const;
    //@}

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    bool OnSubPropertyChanged(const DependencyProperty* prop) override;
    //@}

public:
    friend class VisualTest;
    friend struct VisualTreeHelper;
    friend class VisualCollection;
    friend class UIElementCollection;
    friend class ViewLayout;
    friend class View;
    friend class VisualTreeInspector;

    void ConnectToView(IView* view);
    void DisconnectFromView();

    void Invalidate();

    // Invalidates descendant bounds on a branch
    void InvalidateBranchBounds(bool raiseDrawingCommandsChanged) const;

    // Fires SubtreeDrawingCommandsChanged event
    void RaiseSubtreeDrawingCommandsChanged() const;

    Transform3 InternalTransformToRoot() const;
    Transform3 InternalTransformToAncestor(const Visual* ancestor) const;
    Transform3 InternalTransformToParent() const;

    Point InternalPointToScreen(const Vector4& point) const;
    Vector4 GetDirection(const Point& point) const;

    // Returns the topmost Visual object of a hit test by specifying a Point in local coordinates
    HitTestResult HitTest(const Point& point);
    HitTestResult InternalHitTest(const Point& point, const Vector4& dir);

    // Initiates a hit test on this Visual, with caller-defined HitTestFilterCallback and
    // HitTestResultCallback allowing to retrieve all of the visuals under the specified point,
    // not just the topmost one
    void HitTest(const Point& point, const HitTestFilterCallback& hitTestFilter,
        const HitTestResultCallback& hitTestResult);
    HitTestResultBehavior InternalHitTest(const Point& point, const Vector4& dir,
        const HitTestFilterCallback& hitTestFilter, const HitTestResultCallback& hitTestResult);

    void EnsureDescendantBounds() const;

    // Pushes all the necessary commands into the UI render thread queue to update corresponding
    // render nodes
    //@{
    void UpdateRenderTree(RenderTreeUpdater& updater);
    void UpdateFlags(RenderTreeUpdater& updater);
    void UpdateBounds(RenderTreeUpdater& updater);
    void UpdateTransform(RenderTreeUpdater& updater);
    void UpdateTransform3D(RenderTreeUpdater& updater);
    void UpdateBitmapScalingMode(RenderTreeUpdater& updater);
    void UpdateBlendingMode(RenderTreeUpdater& updater);
    void UpdateClip(RenderTreeUpdater& updater);
    void UpdateEffect(RenderTreeUpdater& updater);
    void UpdateOffset(RenderTreeUpdater& updater);
    void UpdateOpacity(RenderTreeUpdater& updater);
    void UpdateOpacityMask(RenderTreeUpdater& updater);
    void UpdateDrawingCommands(RenderTreeUpdater& updater);
    void UpdateChildren(RenderTreeUpdater& updater);

    void CreateRenderNode(RenderTreeUpdater& updater);
    void DestroyRenderNode(RenderTreeUpdater& updater);
    void AddNode(RenderTreeUpdater& updater);
    void RemoveNode(RenderTreeUpdater& updater, uint32_t parentIndex);
    void RemoveAllNodes(RenderTreeUpdater& updater);
    //@}

    /// Visual flag management
    //@{
    void UpdateBranchVisualFlag(uint32_t flag, bool value) const;
    void SetVisualFlag(uint32_t flag) const;
    void ClearVisualFlag(uint32_t flag) const;
    bool CheckVisualFlag(uint32_t flag) const;
    //@}

    /// Manages invalidation of visual in renderer for next update
    //@{
    void AddInvalidatedVisual() const;
    void RemoveInvalidatedVisual() const;
    void CancelUpdateRequest();
    //@}

public:
    View* mView;
    uint32_t mIndex;

    Visual* mVisualParent;
    Point mVisualOffset;
    Size mVisualSize;

    mutable Rect mContentBoundsCache;
    mutable Rect mDescendantBoundsCache;
    mutable float mDescendantBoundsMinZ;
    mutable float mDescendantBoundsMaxZ;

    mutable uint32_t mVisualFlags;

    void* mUpdateRequest;

    SubtreeDrawingCommandsChangedDelegate mSubtreeDrawingCommandsChanged;

    NS_DECLARE_REFLECTION(Visual, DependencyObject)
};

NS_WARNING_POP

}


#endif
