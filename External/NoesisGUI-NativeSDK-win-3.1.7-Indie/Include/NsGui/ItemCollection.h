////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ITEMCOLLECTION_H__
#define __GUI_ITEMCOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Delegate.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ICollectionView.h>
#include <NsGui/IList.h>
#include <NsGui/INotifyCollectionChanged.h>
#include <NsGui/IComponentInitializer.h>
#include <NsGui/IUITreeNode.h>


namespace Noesis
{

class DependencyObject;
class ItemsControl;
template<class T> class UICollection;
class CollectionView;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Holds the list of items that constitute the content of an ItemsControl.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.controls.itemcollection.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ItemCollection: public BaseComponent, public IList, public ICollectionView, 
    public INotifyCollectionChanged, public IComponentInitializer, 
    public IUITreeNode
{
public:
    ItemCollection(ItemsControl* itemsControl);
    ~ItemCollection();

    /// To be called from ItemsControl when the ItemsSource is assigned with a CollectionView
    void SetView(CollectionView* view);

    /// Check if the itemcollection is related to an external collection, and so it is readonly
    bool IsReadOnly() const;

    /// Gets the item at the specified index
    Ptr<BaseComponent> GetItemAt(uint32_t index) const;

    /// Sets the item at the specified index
    void Set(uint32_t index, BaseComponent* item);

    /// Adds an item to the collection. Returns The position into which the new element was
    /// inserted, or -1 to indicate that the item was not inserted into the collection
    int Add(BaseComponent* item);

    /// Inserts an item to the collection at the specified index
    void Insert(uint32_t index, BaseComponent* item);

    /// Determines the index of a specific item in the collection. Returns -1 if not found
    int IndexOf(const BaseComponent* item) const;

    /// Removes the first occurrence of a specific object from the collection. Returns true if item
    /// was removed, false to indicate that the item was not found in the collection
    bool Remove(const BaseComponent* item);

    /// Removes the collection item at the specified index
    void RemoveAt(uint32_t index);

    /// Removes all elements from the collection
    void Clear();

    /// From IList
    //@{
    Ptr<BaseComponent> GetComponent(uint32_t index) const final;
    void SetComponent(uint32_t index, BaseComponent* item) final;
    int AddComponent(BaseComponent* item) final;
    int IndexOfComponent(const BaseComponent* item) const final;
    int Count() const final;
    //@}

    /// From ICollectionView
    //@{
    bool CanFilter() const final;
    bool CanGroup() const final;
    bool CanSort() const final;
    bool Contains(const BaseComponent* item) const final;
    Ptr<BaseComponent> CurrentItem() const final;
    int CurrentPosition() const final;
    bool IsCurrentAfterLast() const final;
    bool IsCurrentBeforeFirst() const final;
    bool IsEmpty() const final;
    bool MoveCurrentTo(BaseComponent* item) final;
    bool MoveCurrentToFirst() final;
    bool MoveCurrentToLast() final;
    bool MoveCurrentToNext() final;
    bool MoveCurrentToPosition(int position) final;
    bool MoveCurrentToPrevious() final;
    void Refresh() final;
    EventHandler& CurrentChanged() final;
    //@}

    /// From INotifyCollectionChanged
    //@{
    NotifyCollectionChangedEventHandler& CollectionChanged() final;
    //@}

    /// From IComponentInitializer
    //@{
    bool IsInitialized() const override;
    void Init() override;
    void BeginInit() override;
    void EndInit() override;
    //@}

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const final;
    void SetNodeParent(IUITreeNode* parent) final;
    BaseComponent* FindNodeResource(const char* key, bool fullElementSearch) const final;
    BaseComponent* FindNodeName(const char* name) const final;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const final;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    bool CheckReadOnly() const;

    void ConnectChildren() const;
    void DisconnectChildren() const;

    void OnCollectionChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);
    void OnCurrentChanged(BaseComponent* sender, const EventArgs& args);

    void ConnectCollection();
    void DisconnectCollection();

    void RegisterDelegates();
    void UnregisterDelegates();

    void Reset(DependencyObject* d);

private:
    bool mIsInitialized;
    ItemsControl* mItemsControl;

    Ptr<UICollection<BaseComponent>> mLocalCollection;
    Ptr<CollectionView> mEffectiveCollectionView;

    EventHandler mCurrentChanged;
    NotifyCollectionChangedEventHandler mCollectionChanged;

    NS_DECLARE_REFLECTION(ItemCollection, BaseComponent)
};

NS_WARNING_POP

}


#endif
