////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GRIDVIEWCOLUMNHEADER_H__
#define __GUI_GRIDVIEWCOLUMNHEADER_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsGui/ControlsApi.h>
#include <NsGui/BaseButton.h>


namespace Noesis
{

class GridViewColumn;
class GridViewHeaderRowPresenter;
class Thumb;
class Canvas;
struct DragStartedEventArgs;
struct DragCompletedEventArgs;
struct DragDeltaEventArgs;
struct MouseButtonEventArgs;

////////////////////////////////////////////////////////////////////////////////////////////////////
enum GridViewColumnHeaderRole
{
    /// The column header displays above its associated column
    GridViewColumnHeaderRole_Normal,
    /// The column header is the object of a drag-and-drop operation to move a column
    GridViewColumnHeaderRole_Floating,
    /// The column header is the last header in the row of column headers and is used for padding
    GridViewColumnHeaderRole_Padding 
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a column header for a GridViewColumn.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.gridviewcolumnheader.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CONTROLS_API GridViewColumnHeader: public BaseButton
{
public:
    GridViewColumnHeader();
    ~GridViewColumnHeader();

    /// Gets the GridViewColumn that is associated with the GridViewColumnHeader
    GridViewColumn* GetColumn() const;

    /// Gets the role of a GridViewColumnHeader
    GridViewColumnHeaderRole GetRole() const;
    
public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* RoleProperty;
    //@}

protected:
    /// From UIElement
    //@{
    void OnMouseLeftButtonDown(const MouseButtonEventArgs& e) override;
    void OnMouseLeftButtonUp(const MouseButtonEventArgs& e) override;
    void OnMouseMove(const MouseEventArgs& e) override;
    //@}

    /// From FrameworkElement
    //@{
    void OnTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkElement* oldRoot,
        FrameworkTemplate* newTemplate, FrameworkElement* newRoot) override;
    //@}

private:
    void SetColumn(GridViewColumn* column);
    void OnColumnPropertyChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);
    void OnColumnDestroyed(DependencyObject* dob);

    void OnGripperDragStarted(BaseComponent*, const DragStartedEventArgs&);
    void OnGripperDragCompleted(BaseComponent*, const DragCompletedEventArgs&);
    void OnGripperDragDelta(BaseComponent*, const DragDeltaEventArgs&);
    void OnGripperDoubleClick(BaseComponent*, const MouseButtonEventArgs&);

    void RegisterGripperEvents();
    void UnregisterGripperEvents();

    void SetHeaderRowPresenter(GridViewHeaderRowPresenter* presenter);
    void OnHeaderRowPresenterChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& e);
    void OnHeaderRowPresenterDestroyed(DependencyObject* dob);

    void UpdateProperty(const DependencyProperty* headerDP,
        const DependencyProperty* columnDP, const DependencyProperty* presenterDP);

private:
    friend class GridViewHeaderRowPresenter;
    GridViewHeaderRowPresenter* mPresenter;
    GridViewColumn* mColumn;

    Ptr<Thumb> mHeaderGripper;

    float mStartDragWidth;

    NS_DECLARE_REFLECTION(GridViewColumnHeader, BaseButton)
};

NS_WARNING_POP

}

NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CONTROLS_API, Noesis::GridViewColumnHeaderRole)


#endif
