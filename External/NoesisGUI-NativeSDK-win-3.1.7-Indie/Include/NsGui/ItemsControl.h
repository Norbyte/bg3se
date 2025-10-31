////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ITEMSCONTROL_H__
#define __GUI_ITEMSCONTROL_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Control.h>


namespace Noesis
{

class DataTemplateSelector;
class DataTemplate;
class ItemContainerGenerator;
class ItemsPanelTemplate;
class ItemCollection;
class ItemsPresenter;
class Panel;
class ItemsControlTest;
struct NotifyCollectionChangedEventArgs;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that can be used to present a collection of items.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.itemscontrol.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ItemsControl: public Control
{
public:
    ItemsControl();
    ~ItemsControl();

    /// Gets or sets a value that controls the range of values assigned to the AlternationIndex
    /// property attached to each generated container.
    ///  * The default value 0 means "do not set AlternationIndex"
    ///  * A positive value means "assign AlternationIndex in the range [0, AlternationCount) so
    ///    that adjacent containers receive different values"
    //@{
    int GetAlternationCount() const;
    void SetAlternationCount(int value);
    //@}

    /// Gets the index set on generated containers, when the ItemsControl's AlternationCount
    /// property is positive. The AlternationIndex lies in the range [0, AlternationCount), and
    /// adjacent containers always get assigned different values
    static int GetAlternationIndex(const DependencyObject* element);

    /// Gets or sets a path to a value on the source object to serve as the visual representation 
    /// of the object
    //@{
    const char* GetDisplayMemberPath() const;
    void SetDisplayMemberPath(const char* path);
    //@}
    
    /// Gets a value that indicates whether the ItemsControl contains items
    bool GetHasItems() const;

    /// Gets the ItemContainerGenerator associated with this control
    ItemContainerGenerator* GetItemContainerGenerator() const;

    /// Gets or sets the Style that is applied to the container element generated for each item. 
    //@{
    Style* GetItemContainerStyle() const;
    void SetItemContainerStyle(Style* style);
    //@}

    /// Gets or sets the ItemsPanelTemplate that defines the Panel that controls the layout of items
    //@{
    ItemsPanelTemplate* GetItemsPanel() const;
    void SetItemsPanel(ItemsPanelTemplate* value);
    //@}

    /// Returns a new instance of the default items panel for this ItemsControl
    virtual Ptr<Panel> GetDefaultItemsPanel() const;

    /// Gets or sets a collection used to generate the content of the ItemsControl. When the
    /// ItemsSource property is set, the Items collection is made read-only and fixed-size
    //@{
    BaseComponent* GetItemsSource() const;
    void SetItemsSource(BaseComponent* items);
    //@}

    /// Gets or sets the DataTemplate used to display each item.
    //@{
    DataTemplate* GetItemTemplate() const;
    void SetItemTemplate(DataTemplate* value);
    //@}

    /// Gets or sets a DataTemplateSelector that provides custom logic for choosing the template
    /// used to display the header
    //@{
    DataTemplateSelector* GetItemTemplateSelector() const;
    void SetItemTemplateSelector(DataTemplateSelector* selector);
    //@}

    /// Gets the collection used to generate the content of the ItemsControl. When the ItemsSource
    /// property is set, the Items collection is made read-only and fixed-size
    ItemCollection* GetItems() const;

    /// Returns the ItemsControl that owns the specified container element.
    static ItemsControl* ItemsControlFromItemContainer(DependencyObject* container);

    /// Returns the container that owns the given element. If *itemsControl* is not null, returns a
    /// container that belongs to the given ItemsControl. If *itemsControl* is null, returns the
    /// closest container belonging to any ItemsControl. Returns null if no such container exists
    static DependencyObject* ContainerFromElement(ItemsControl* itemsControl,
        DependencyObject* element);

    /// Returns the container belonging to the current ItemsControl that owns the given container
    /// element. Returns null if no such container exists
    DependencyObject* ContainerFromElement(DependencyObject* element);

    /// Returns the ItemsControl that the specified element hosts items for. If IsItemsHost is set 
    /// to true on element in a style or if element is a panel created by the ItemsPresenter for an 
    /// ItemsControl, the ItemsControl is returned; otherwise, a null reference
    static ItemsControl* GetItemsOwner(DependencyObject* itemsHost);

    /// Determines if the specified item is (or is eligible to be) its own container
    bool IsItemItsOwnContainer(BaseComponent* item) const;

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* AlternationCountProperty;
    static const DependencyProperty* AlternationIndexProperty; // attached property
    static const DependencyProperty* DisplayMemberPathProperty;
    static const DependencyProperty* HasItemsProperty;
    static const DependencyProperty* ItemContainerStyleProperty;
    static const DependencyProperty* ItemsPanelProperty;
    static const DependencyProperty* ItemsSourceProperty;
    static const DependencyProperty* ItemTemplateProperty;
    static const DependencyProperty* ItemTemplateSelectorProperty;
    //@}
    
protected:
    // Updates items presenter and items host
    void RefreshItems();

    // Gets the panel which is marked by the template to visually host the items
    Panel* GetItemsHost() const;

    // Creates or identifies the element that is used to display the given item.
    virtual Ptr<DependencyObject> GetContainerForItemOverride() const;

    // Determines if the specified item is (or is eligible to be) its own container.
    virtual bool IsItemItsOwnContainerOverride(BaseComponent* item) const;

    // Configures container
    virtual void PrepareContainerForItemOverride(DependencyObject* container,
        BaseComponent* item) const;

    // Clears container attachments
    virtual void ClearContainerForItemOverride(DependencyObject* container,
        BaseComponent* item) const;

    // Applies the appropriate style to item container
    virtual void ApplyItemContainerStyle(DependencyObject* container) const;
    static void OverrideContainerStyle(DependencyObject* container, const char* key);

    // Invoked when the Items property changes
    virtual void OnItemsChanged(const NotifyCollectionChangedEventArgs& args);

    /// Invoked when the current item changes
    virtual void OnCurrentItemChanged();

    // Invoked when item containers are generated/destroyed for this ItemsControl
    virtual void OnContainersGenerated();

    // Invoked when Item template changes
    virtual void OnItemTemplateChanged(DataTemplate* oldTemplate, DataTemplate* newTemplate);

    // Invoked when Item template selector changes
    virtual void OnItemTemplateSelectorChanged(DataTemplateSelector* oldSelector,
        DataTemplateSelector* newSelector);

    // Tries to bring item into view
    void OnBringItemIntoView(BaseComponent* item);

    // Finds next available container for navigation in the specified direction
    DependencyObject* NextContainer(int startIndex, int direction, bool allowCycle);

    // Gets the number of items visible in the viewport. Returns 0 when no viewport is available.
    //@{
    int GetVerticalPageSize() const;
    int GetHorizontalPageSize() const;
    //@}

    // From DependencyObject
    //@{
    void OnInit() override;
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    // From UIElement
    //@{
    void OnKeyDown(const KeyEventArgs& e) override;
    //@}

    // From FrameworkElement
    //@{
    void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const override;
    uint32_t GetLogicalChildrenCount() const override;
    Ptr<BaseComponent> GetLogicalChild(uint32_t index) const override;
    void OnTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkElement* oldRoot,
        FrameworkTemplate* newTemplate, FrameworkElement* newRoot) override;
    Size MeasureOverride(const Size& availableSize) override;
    //@}

    // Called when ItemsSource has changed
    virtual void OnItemsSourceChanged(BaseComponent* oldSource,
        BaseComponent* newSource);

    // ItemsControl flag management
    //@{
    void SetItemsFlag(uint32_t flag);
    void ClearItemsFlag(uint32_t flag);
    bool CheckItemsFlag(uint32_t flag) const;

    struct ScopeItemsFlag
    {
        ScopeItemsFlag(ItemsControl* ic, uint32_t f);
        ~ScopeItemsFlag();
    private:
        ItemsControl* itemsControl;
        uint32_t flag;
    };

    enum
    {
        UsesExternalCollectionView = 1,
        RemovingExternalCollectionView = UsesExternalCollectionView * 2,
        NeedsRefreshItems = RemovingExternalCollectionView * 2,
        ItemsHaveChanged = NeedsRefreshItems * 2,
        ContainersWereGenerated = ItemsHaveChanged * 2,

        LastFlag = ContainersWereGenerated
    };
    //@}

    void PrepareContainer(BaseComponent* item, ItemsControl* parent);

private:
    friend class ItemContainerGenerator;
    friend class ItemCollection;
    friend class ItemsPresenter;
    friend class Panel;
    friend class VirtualizingPanel;
    friend class ItemsControlTest;

    // ItemContainerGenerator host interface
    //@{
    Ptr<DependencyObject> GetContainerForItem(BaseComponent* item) const;
    void PrepareItemContainer(DependencyObject* container, BaseComponent* item) const;
    void ClearContainerForItem(DependencyObject* container, BaseComponent* item) const;
    bool IsHostForItemContainer(DependencyObject* container) const;
    //@}

    static bool IsContainerForItemsControl(DependencyObject* element,
        ItemsControl* itemsControl);

    void OnCollectionViewSourceChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);
    void OnItemsChangedStart(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);
    void OnItemsChangedEnd(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);
    void OnCurrentItemChangedInternal(BaseComponent* c, const EventArgs& args);
    void OnGeneratorChanged(BaseComponent* sender, const EventArgs& e);

    // Allows ItemsPresenter to update items presenter
    void SetItemsPresenter(ItemsPresenter* itemsPresenter);

    // Allows Panel to update items host
    void SetItemsHost(Panel* itemsHost);
    Orientation GetItemsHostOrientation() const;

    bool FocusNext(int direction, bool allowCycle);
    bool FocusContainer(DependencyObject* container);

    void EnsureItems();
    void ConnectItems();
    void DisconnectItems();

    void InvalidateItemsPresenter();
    void InvalidateItems();

    bool CheckTemplateProperties();

    static void SetAlternationIndex(DependencyObject* element, int value);
    static void ClearAlternationIndex(DependencyObject* element);

    virtual void OnAlternationCountChanged(int oldValue, int newValue);

private:
    Ptr<ItemCollection> mItems;
    Ptr<ItemContainerGenerator> mGenerator;
    Ptr<ItemsPresenter> mItemsPresenter;
    Ptr<Panel> mItemsHost;
    Ptr<DataTemplate> mDisplayMemberTemplate;

    uint32_t mItemsFlags;

    NS_DECLARE_REFLECTION(ItemsControl, Control)
};

NS_WARNING_POP

}


#endif
