////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_UNIFORMGRID_H__
#define __GUI_UNIFORMGRID_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Panel.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a way to arrange content in a grid where all the cells in the grid have the same size.
///
/// .. code-block:: xml
///
///    <Grid xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
///        Height="200" Width="250">
///
///        <UniformGrid>
///            <Button Content="Button 1"/>
///            <Button Content="Button 2"/>
///            <Button Content="Button 3"/>
///            <Button Content="Button 4"/>
///        </UniformGrid>
///
///    </Grid>
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.uniformgrid.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API UniformGrid: public Panel
{
public:
    UniformGrid();
    ~UniformGrid();

    /// Gets or sets the number of leading blank cells in the first row of the grid
    //@{
    uint32_t GetFirstColumn() const;
    void SetFirstColumn(uint32_t firstColumn);
    //@}

    /// Gets or set the number of columns that are in the grid
    //@{
    uint32_t GetColumns() const;
    void SetColumns(uint32_t columns);
    //@}

    /// Gets or sets the number of rows that are in the grid
    //@{
    uint32_t GetRows() const;
    void SetRows(uint32_t rows);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* FirstColumnProperty;
    static const DependencyProperty* ColumnsProperty;
    static const DependencyProperty* RowsProperty;
    //@}

protected:
    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

private:
    void CalculateCells();

private:
    uint32_t mNumColumns;
    uint32_t mNumRows;

    NS_DECLARE_REFLECTION(UniformGrid, Panel)
};

NS_WARNING_POP

}


#endif
