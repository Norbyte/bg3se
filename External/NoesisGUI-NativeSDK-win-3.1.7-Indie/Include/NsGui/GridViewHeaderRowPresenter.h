////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GRIDVIEWHEADERROWPRESENTER_H__
#define __GUI_GRIDVIEWHEADERROWPRESENTER_H__


#include <NsCore/Noesis.h>
#include <NsGui/ControlsApi.h>
#include <NsGui/BaseGridViewRowPresenter.h>


namespace Noesis
{

class ContextMenu;
class DataTemplate;
class DataTemplateSelector;
class Style;
class GridViewColumnHeader;
class ScrollViewer;
class Rectangle;
struct ScrollChangedEventArgs;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an object that is used to define the layout of a row of column headers. 
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.gridviewheaderrowpresenter.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CONTROLS_API GridViewHeaderRowPresenter: public BaseGridViewRowPresenter
{
public:
    GridViewHeaderRowPresenter();
    ~GridViewHeaderRowPresenter();

    /// Gets or sets a value that indicates whether columns can change positions
    //@{
    bool GetAllowsColumnReorder() const;
    void SetAllowsColumnReorder(bool value);
    //@}

    /// Gets or sets the Style to use for the column headers
    //@{
    Style* GetColumnHeaderContainerStyle() const;
    void SetColumnHeaderContainerStyle(Style* style);
    //@}

    /// Gets or sets a ContextMenu for the column headers
    //@{
    ContextMenu* GetColumnHeaderContextMenu() const;
    void SetColumnHeaderContextMenu(ContextMenu* menu);
    //@}

    /// Gets or sets a composite string that specifies how to format the column headers if they are 
    /// displayed as strings
    //@{
    const char* GetColumnHeaderStringFormat() const;
    void SetColumnHeaderStringFormat(const char* format);
    //@}

    /// Gets or sets the template to use to display the column headers
    //@{
    DataTemplate* GetColumnHeaderTemplate() const;
    void SetColumnHeaderTemplate(DataTemplate* dataTemplate);
    //@}

    /// Gets or sets a DataTemplateSelector that provides logic that selects the data template to 
    /// use to display a column header
    //@{
    DataTemplateSelector* GetColumnHeaderTemplateSelector() const;
    void SetColumnHeaderTemplateSelector(DataTemplateSelector* selector);
    //@}

    /// Gets or sets the content for a tooltip for the column header row
    //@{
    BaseComponent* GetColumnHeaderToolTip() const;
    void SetColumnHeaderToolTip(BaseComponent* tooltip);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* AllowsColumnReorderProperty;
    static const DependencyProperty* ColumnHeaderContainerStyleProperty;
    static const DependencyProperty* ColumnHeaderContextMenuProperty;
    static const DependencyProperty* ColumnHeaderStringFormatProperty;
    static const DependencyProperty* ColumnHeaderTemplateProperty;
    static const DependencyProperty* ColumnHeaderTemplateSelectorProperty;
    static const DependencyProperty* ColumnHeaderToolTipProperty;
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
    void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const override;
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

private:
    void OnScrollChanged(BaseComponent* sender, const ScrollChangedEventArgs& args);

    void UpdateColumnHeaders();
    bool IsValidDragPosition() const;
    uint32_t CalculateFloatingDestinationIndex() const;
    float CalculateFloatingDestinationOffset() const;

private:
    Ptr<GridViewColumnHeader> mPaddingHeader;
    Ptr<GridViewColumnHeader> mFloatingHeader;
    Ptr<Rectangle> mFloatingDestination;
    ScrollViewer* mHeaderScrollViewer;
    ScrollViewer* mItemsScrollViewer;

    GridViewColumnHeader* mDragHeader;
    Point mDragStart;
    Point mDragPos;
    bool mDragEnabled;
    bool mDragging;

    NS_DECLARE_REFLECTION(GridViewHeaderRowPresenter, BaseGridViewRowPresenter)
};

NS_WARNING_POP

}


#endif
