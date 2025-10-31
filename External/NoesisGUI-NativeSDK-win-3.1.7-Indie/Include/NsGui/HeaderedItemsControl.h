////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_HEADEREDITEMSCONTROL_H__
#define __GUI_HEADEREDITEMSCONTROL_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ItemsControl.h>


namespace Noesis
{

class DataTemplate;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that contains multiple items and has a header.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.headereditemscontrol.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API HeaderedItemsControl: public ItemsControl
{
public:
    HeaderedItemsControl();
    ~HeaderedItemsControl();

    /// Gets a value that indicates whether this HeaderedItemsControl has a header
    bool GetHasHeader() const;

    /// Gets or sets the item that labels the control
    //@{
    BaseComponent* GetHeader() const;
    void SetHeader(BaseComponent* header);
    void SetHeader(const char* header);
    //@}

    /// Gets or sets a composite string that specifies how to format the Header property if it is
    /// displayed as a string.
    //@{
    const char* GetHeaderStringFormat() const;
    void SetHeaderStringFormat(const char* headerStringFormat);
    //@}

    /// Gets or sets the DataTemplate used to display the contents of the control's header
    //@{
    DataTemplate* GetHeaderTemplate() const;
    void SetHeaderTemplate(DataTemplate* value);
    //@}

    /// Gets or sets a DataTemplateSelector that provides custom logic for choosing the template
    /// used to display the header
    //@{
    DataTemplateSelector* GetHeaderTemplateSelector() const;
    void SetHeaderTemplateSelector(DataTemplateSelector* value);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* HasHeaderProperty;
    static const DependencyProperty* HeaderProperty;
    static const DependencyProperty* HeaderStringFormatProperty;
    static const DependencyProperty* HeaderTemplateProperty;
    static const DependencyProperty* HeaderTemplateSelectorProperty;
    //@}

protected:
    /// From BaseRefCounted
    //@{
    int32_t OnDestroy() override;
    //@}

    // From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    // From FrameworkElement
    //@{
    uint32_t GetLogicalChildrenCount() const override;
    Ptr<BaseComponent> GetLogicalChild(uint32_t index) const override;
    //@}

protected:
    virtual void OnHeaderChanged(BaseComponent* oldHeader, BaseComponent* newHeader);

private:
    friend class ItemsControl;
    void PrepareContainer(BaseComponent* item, ItemsControl* parent);
    void PrepareHierarchy(BaseComponent* item, ItemsControl* parent);
    void ClearContainer(BaseComponent* item);
    DataTemplate* FindHeaderTemplate(BaseComponent* item, ItemsControl* parent) const;

private:
    bool mHeaderAsLogicalChild;

    NS_DECLARE_REFLECTION(HeaderedItemsControl, ItemsControl)
};

}


#endif
