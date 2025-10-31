////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GRIDVIEW_H__
#define __GUI_GRIDVIEW_H__


#include <NsCore/Noesis.h>
#include <NsGui/ControlsApi.h>
#include <NsGui/BaseView.h>


namespace Noesis
{

class ContextMenu;
class DataTemplate;
class DataTemplateSelector;
class GridViewColumn;
class Style;
struct NotifyCollectionChangedEventArgs;

template<class T> class FreezableCollection;
typedef Noesis::FreezableCollection<Noesis::GridViewColumn> GridViewColumnCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a view mode that displays data items in columns for a ListView control.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.gridview.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CONTROLS_API GridView: public BaseView
{
public:
    GridView();
    ~GridView();

    /// Gets or sets whether columns in a GridView can be reordered by a drag-and-drop operation
    //@{
    bool GetAllowsColumnReorder() const;
    void SetAllowsColumnReorder(bool value);
    //@}

    /// Gets or sets the style to apply to column headers
    //@{
    Style* GetColumnHeaderContainerStyle() const;
    void SetColumnHeaderContainerStyle(Style* style);
    //@}

    /// Gets or sets a ContextMenu for the GridView
    //@{
    ContextMenu* GetColumnHeaderContextMenu() const;
    void SetColumnHeaderContextMenu(ContextMenu* menu);
    //@}

    /// Gets or sets a composite string that specifies how to format the column headers of the 
    /// GridView if they are displayed as strings
    //@{
    const char* GetColumnHeaderStringFormat() const;
    void SetColumnHeaderStringFormat(const char* format);
    //@}

    /// Gets or sets a template to use to display the column headers
    //@{
    DataTemplate* GetColumnHeaderTemplate() const;
    void SetColumnHeaderTemplate(DataTemplate* dataTemplate);
    //@}

    /// Gets or sets the selector object that provides logic for selecting a template to use for 
    /// each column header
    //@{
    DataTemplateSelector* GetColumnHeaderTemplateSelector() const;
    void SetColumnHeaderTemplateSelector(DataTemplateSelector* selector);
    //@}

    /// Gets or sets the content of a tooltip that appears when the mouse pointer pauses over one 
    /// of the column headers
    //@{
    BaseComponent* GetColumnHeaderToolTip() const;
    void SetColumnHeaderToolTip(BaseComponent* tooltip);
    //@}
    
    /// Gets the collection of GridViewColumn objects that is defined for this GridView
    GridViewColumnCollection* GetColumns() const;

    /// Gets or sets the attached property that contains the GridViewColumnCollection
    //@{
    static GridViewColumnCollection* GetColumnCollection(const DependencyObject* element);
    static void SetColumnCollection(DependencyObject* element, GridViewColumnCollection* value);
    //@}

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* AllowsColumnReorderProperty;
    static const DependencyProperty* ColumnHeaderContainerStyleProperty;
    static const DependencyProperty* ColumnHeaderContextMenuProperty;
    static const DependencyProperty* ColumnHeaderStringFormatProperty;
    static const DependencyProperty* ColumnHeaderTemplateProperty;
    static const DependencyProperty* ColumnHeaderTemplateSelectorProperty;
    static const DependencyProperty* ColumnHeaderToolTipProperty;
    static const DependencyProperty* ColumnCollectionProperty; // Attached

protected:
    /// From DependencyObject
    //@{
    void OnInit() override;
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    bool FreezeCore(bool isChecking) override;
    void CloneCore(const Freezable* source) override;
    void CloneCurrentValueCore(const Freezable* source) override;
    void GetAsFrozenCore(const Freezable* source) override;
    void GetCurrentValueAsFrozenCore(const Freezable* source) override;
    //@}

    /// From BaseView
    //@{
    void ClearItem(ListViewItem* item) override;
    void PrepareItem(ListViewItem* item) override;
    //@}

private:
    void EnsureColumns() const;

private:
    mutable Ptr<GridViewColumnCollection> mColumns;

    NS_DECLARE_REFLECTION(GridView, BaseView)
};

NS_WARNING_POP

}


#endif
