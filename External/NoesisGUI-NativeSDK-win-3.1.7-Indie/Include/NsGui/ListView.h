////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_LISTVIEW_H__
#define __GUI_LISTVIEW_H__


#include <NsCore/Noesis.h>
#include <NsGui/ControlsApi.h>
#include <NsGui/ListBox.h>


namespace Noesis
{

class BaseView;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that displays a list of data items.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.listview.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CONTROLS_API ListView: public ListBox
{
public:
    ListView();
    ~ListView();

    /// Gets or sets an object that defines how the data is styled and organized in a ListView
    /// control
    //@{
    BaseView* GetView() const;
    void SetView(BaseView* view);
    //@}

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* ViewProperty;
    //@}

protected:
    /// From ItemsControl
    //@{
    Ptr<DependencyObject> GetContainerForItemOverride() const override;
    void PrepareContainerForItemOverride(DependencyObject* element,
        BaseComponent* item) const override;
    bool IsItemItsOwnContainerOverride(BaseComponent* item) const override;
    //@}

    NS_DECLARE_REFLECTION(ListView, ListBox)
};

}


#endif
