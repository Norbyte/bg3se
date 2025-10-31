////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CONTENTCONTROL_H__
#define __GUI_CONTENTCONTROL_H__


#include <NsCore/Noesis.h>
#include <NsGui/Control.h>


namespace Noesis
{

class DataTemplate;
class DataTemplateSelector;
class ItemsControl;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control with a single piece of content.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.contentcontrol.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ContentControl: public Control
{
public:
    ContentControl();
    ~ContentControl();

    /// Gets or sets the content of a ContentControl
    //@{
    BaseComponent* GetContent() const;
    void SetContent(BaseComponent* content);
    void SetContent(const char* content);
    //@}

    /// Gets or sets the format used to display the content of the control as a string.
    //@{
    const char* GetContentStringFormat() const;
    void SetContentStringFormat(const char* value);
    //@}

    /// Gets or sets the template used to display the content of the ContentControl
    //@{
    DataTemplate* GetContentTemplate() const;
    void SetContentTemplate(DataTemplate* dataTemplate);
    //@}

    /// Gets or sets a DataTemplateSelector that provides custom logic for choosing the template
    /// used to display the content
    //@{
    DataTemplateSelector* GetContentTemplateSelector() const;
    void SetContentTemplateSelector(DataTemplateSelector* selector);
    //@}

    /// Gets a value that indicates whether the ContentControl contains content
    bool GetHasContent() const;

    /// From BaseObject
    //@{
    String ToString() const override;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ContentProperty;
    static const DependencyProperty* ContentStringFormatProperty;
    static const DependencyProperty* ContentTemplateProperty;
    static const DependencyProperty* ContentTemplateSelectorProperty;
    static const DependencyProperty* HasContentProperty;
    //@}

protected:
    // Content property value changed event
    virtual void OnContentChanged(BaseComponent* oldContent, BaseComponent* newContent);

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
    bool mContentAsLogicalChild;

private:
    friend class ItemsControl;
    void PrepareContainer(BaseComponent* item, DataTemplate* itemTemplate,
        DataTemplateSelector* itemTemplateSelector);
    void ClearContainer(BaseComponent* item);

    NS_DECLARE_REFLECTION(ContentControl, Control)
};

}

#endif
