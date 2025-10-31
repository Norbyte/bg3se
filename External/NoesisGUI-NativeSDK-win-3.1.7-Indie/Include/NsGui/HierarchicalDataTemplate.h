////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_HIERARCHICALDATATEMPLATE_H__
#define __GUI_HIERARCHICALDATATEMPLATE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/DataTemplate.h>


namespace Noesis
{

class Style;
class BaseBinding;
class DataTemplateSelector;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a DataTemplate that supports HeaderedItemsControl, such as TreeViewItem or MenuItem.
///
/// .. code-block:: xml
///
///    <Grid
///        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml">
///
///        <Grid.Resources>
///            <LeagueList x:Key="items" />
///
///            <HierarchicalDataTemplate DataType="League" ItemsSource="{Binding Path=Divisions}">
///                <TextBlock Text="{Binding Path=Name}"/>
///            </HierarchicalDataTemplate>
///
///            <HierarchicalDataTemplate DataType="Division" ItemsSource="{Binding Path=Teams}">
///                <TextBlock Text="{Binding Path=Name}"/>
///            </HierarchicalDataTemplate>
///
///            <DataTemplate DataType="Team">
///                <TextBlock Text="{Binding Path=Name}"/>
///            </DataTemplate>
///        </Grid.Resources>
///
///        <TreeView DataContext="{StaticResource items}">
///            <TreeViewItem ItemsSource="{Binding Leagues}" Header="My Soccer Leagues" />
///        </TreeView>
///
///    </Grid>
///
/// http://msdn.microsoft.com/en-us/library/system.windows.hierarchicaldatatemplate.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API HierarchicalDataTemplate: public DataTemplate
{
public:
    HierarchicalDataTemplate();
    ~HierarchicalDataTemplate();

    /// Gets or sets the Style that is applied to the item container for each child item
    //@{
    Style* GetItemContainerStyle() const;
    void SetItemContainerStyle(Style* style);
    //@}

    /// Gets or sets the binding for this data template, which indicates where to find the
    /// collection that represents the next level in the data hierarchy
    //@{
    BaseBinding* GetItemsSource() const;
    void SetItemsSource(BaseBinding* source);
    //@}

    /// Gets or sets the DataTemplate to apply to the ItemTemplate property on a generated
    /// HeaderedItemsControl (such as a MenuItem or a TreeViewItem), to indicate how to display
    /// items from the next level in the data hierarchy
    //@{
    DataTemplate* GetItemTemplate() const;
    void SetItemTemplate(DataTemplate* templat);
    //@}

    /// Gets or sets the DataTemplateSelector to apply to the ItemTemplateSelector property on a
    /// generated HeaderedItemsControl (such as a MenuItem or a TreeViewItem), to indicate how to
    /// select a template to display items from the next level in the data hierarchy
    //@{
    DataTemplateSelector* GetItemTemplateSelector() const;
    void SetItemTemplateSelector(DataTemplateSelector* selector);
    //@}

private:
    Ptr<Style> mItemContainerStyle;
    Ptr<BaseBinding> mItemsSource;
    Ptr<DataTemplate> mItemTemplate;
    Ptr<DataTemplateSelector> mItemTemplateSelector;

    NS_DECLARE_REFLECTION(HierarchicalDataTemplate, DataTemplate)
};

NS_WARNING_POP

}


#endif
