////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COLLECTIONVIEW_H__
#define __GUI_COLLECTIONVIEW_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Vector.h>
#include <NsCore/Delegate.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ICollectionView.h>
#include <NsGui/INotifyCollectionChanged.h>
#include <NsGui/IComponentInitializer.h>
#include <NsGui/IUITreeNode.h>
#include <NsGui/Events.h>


namespace Noesis
{

NS_INTERFACE IList;
class CollectionViewTest;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a view for grouping, sorting, filtering, and navigating a data collection.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.data.collectionview.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API CollectionView: public BaseComponent, public ICollectionView,
    public INotifyCollectionChanged, public IComponentInitializer, public IUITreeNode
{
public:
    CollectionView(IList* collection);
    ~CollectionView();

    /// Returns the underlying unfiltered collection
    IList* GetSourceCollection() const;

    /// Gets the number of records in the view
    /// \prop
    int Count() const;

    /// Retrieves the item at the specified zero-based index in the view
    Ptr<BaseComponent> GetItemAt(uint32_t index) const;

    /// Returns the index at which the specified item is located; -1 if the item is unknown
    int IndexOf(const BaseComponent* item) const;

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
    void InternalCollectionChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);

    void ConnectCollection();

    void SetCurrentPosition(int pos);

private:
    friend class CollectionViewTest;

    bool mIsInitialized;
    IUITreeNode* mOwner;

    bool mIsDynamic;
    int mCurrentPosition;

    Ptr<IList> mCollection;

    EventHandler mCurrentChanged;
    NotifyCollectionChangedEventHandler mCollectionChanged;

    NS_DECLARE_REFLECTION(CollectionView, BaseComponent)
};

NS_WARNING_POP

}


#endif
