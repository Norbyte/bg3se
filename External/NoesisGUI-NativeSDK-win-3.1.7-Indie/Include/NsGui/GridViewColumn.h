////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GRIDVIEWCOLUMN_H__
#define __GUI_GRIDVIEWCOLUMN_H__


#include <NsCore/Noesis.h>
#include <NsGui/ControlsApi.h>
#include <NsGui/Animatable.h>


namespace Noesis
{

class Style;
class DataTemplate;
class DataTemplateSelector;
class BaseBindingExpression;
class GridViewColumn;

typedef Delegate<void (GridViewColumn* sender)> ActualWidthChangedDelegate;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a column that displays data.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.gridviewcolumn.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CONTROLS_API GridViewColumn: public Animatable
{
public:
    GridViewColumn();
    ~GridViewColumn();

    /// Gets the actual width of a GridViewColumn
    float GetActualWidth() const;
    void SetActualWidth(float width);

    /// Gets or sets the template to use to display the contents of a column cell
    //@{
    DataTemplate* GetCellTemplate() const;
    void SetCellTemplate(DataTemplate* dataTemplate);
    //@}

    /// Gets or sets a DataTemplateSelector that determines the template to use to display cells in
    /// a column
    //@{
    DataTemplateSelector* GetCellTemplateSelector() const;
    void SetCellTemplateSelector(DataTemplateSelector* selector);
    //@}

    /// Gets or sets the data item to bind to for this column
    //@{
    BaseBindingExpression* GetDisplayMemberBinding() const;
    void SetDisplayMemberBinding(BaseBindingExpression* binding);
    //@}
 
    /// Gets or sets the content of the header of a GridViewColumn
    //@{
    BaseComponent* GetHeader() const;
    void SetHeader(BaseComponent* header);
    //@}
 
    /// Gets or sets the style to use for the header of the GridViewColumn
    //@{
    Style* GetHeaderContainerStyle() const;
    void SetHeaderContainerStyle(Style* style);
    //@}
 
    /// Gets or sets a composite string that specifies how to format the Header property if it is 
    /// displayed as a string
    //@{
    const char* GetHeaderStringFormat() const;
    void SetHeaderStringFormat(const char* format);
    //@}
 
    /// Gets or sets the template to use to display the content of the column header
    //@{
    DataTemplate* GetHeaderTemplate() const;
    void SetHeaderTemplate(DataTemplate* headerTemplate);
    //@}

    /// Gets or sets the DataTemplateSelector that provides logic to select the template to use to
    /// display the column header
    //@{
    DataTemplateSelector* GetHeaderTemplateSelector() const;
    void SetHeaderTemplateSelector(DataTemplateSelector* selector);
    //@}
 
    /// Gets or sets the width of the column
    //@{
    float GetWidth() const;
    void SetWidth(float width);
    //@}

    /// Occurs when column *ActualWidth* changes
    ActualWidthChangedDelegate& ActualWidthChanged();

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* CellTemplateProperty;
    static const DependencyProperty* CellTemplateSelectorProperty;
    static const DependencyProperty* HeaderProperty;
    static const DependencyProperty* HeaderContainerStyleProperty;
    static const DependencyProperty* HeaderStringFormatProperty;
    static const DependencyProperty* HeaderTemplateProperty;
    static const DependencyProperty* HeaderTemplateSelectorProperty;
    static const DependencyProperty* WidthProperty;
    //@}

protected:
    /// From DependencyProperty
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    void CloneCommonCore(const Freezable* source) override;
    //@}

private:
    friend class GridViewHeaderRowPresenter;
    friend class GridViewRowPresenter;

    enum MeasureState
    {
        /// Measure has not been done yet on this column
        MeasureState_None,
        /// Header measure has been done
        MeasureState_Header,
        /// Items measure has been done
        MeasureState_Items,
        /// Header and items measure has been done, or fixed width was set
        MeasureState_Fixed
    };

    void SetMeasureState(MeasureState state);

    float GetDesiredWidth() const;
    float EnsureWidth(float width);

private:
    float mActualWidth;
    ActualWidthChangedDelegate mActualWidthChanged;

    Ptr<BaseBindingExpression> mDisplayMemberBinding;

    MeasureState mMeasureState;
    float mColumnDesiredWidth;

    NS_DECLARE_REFLECTION(GridViewColumn, Animatable)
};

NS_WARNING_POP

}


#endif
