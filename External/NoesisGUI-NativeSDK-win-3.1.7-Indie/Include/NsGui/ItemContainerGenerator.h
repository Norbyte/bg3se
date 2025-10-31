////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ITEMCONTAINERGENERATOR_H__
#define __GUI_ITEMCONTAINERGENERATOR_H__


#include <NsCore/Noesis.h>
#include <NsCore/Delegate.h>
#include <NsCore/BaseComponent.h>
#include <NsGui/CoreApi.h>
#include <NsGui/IRecyclingItemContainerGenerator.h>
#include <NsGui/Events.h>


namespace Noesis
{

class DependencyObject;
class DependencyProperty;
class ItemsControl;
class Panel;
class ItemContainerGeneratorTest;
struct NotifyCollectionChangedEventArgs;
enum NotifyCollectionChangedAction: int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// The ItemsChanged event is raised by an ItemContainerGenerator to inform layouts that the items
/// collection has changed.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct ItemsChangedEventArgs: public EventArgs
{
    NotifyCollectionChangedAction action;
    GeneratorPosition position;
    GeneratorPosition oldPosition;
    int itemCount;
    int itemUICount;

    ItemsChangedEventArgs(NotifyCollectionChangedAction action, const GeneratorPosition& position,
        const GeneratorPosition& oldPosition, int itemCount, int itemUICount);

    ItemsChangedEventArgs(NotifyCollectionChangedAction action, const GeneratorPosition& position,
        int itemCount, int itemUICount);
};

typedef Delegate<void(BaseComponent*, const ItemsChangedEventArgs&)> ItemsChangedEventHandler;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Generates the user interface on behalf of its host, such as an ItemsControl.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.itemcontainergenerator.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ItemContainerGenerator: public BaseComponent, 
    public IRecyclingItemContainerGenerator
{
public:
    // TODO: Expose Items as a ReadOnlyCollection<object>

    /// Gets the status of the generator
    GeneratorStatus GetStatus() const;

    /// Raised to inform controls that ItemContainerGenerator status has changed
    DelegateEvent_<EventHandler> StatusChanged();

    /// Raised by a ItemContainerGenerator to inform layouts that the items collection has changed
    DelegateEvent_<ItemsChangedEventHandler> ItemsChanged();

    /// Returns the container corresponding to the item at the given index within the Items
    DependencyObject* ContainerFromIndex(int index) const;

    /// Returns the container corresponding to the given item. Returns null if the item does not
    /// belong to the item collection, or if no container has been generated for it
    DependencyObject* ContainerFromItem(BaseComponent* item) const;

    /// Given a generated container, returns the index of the corresponding item within the Items
    int IndexFromContainer(DependencyObject* container) const;

    /// Returns the item corresponding to the given container. If the element was not generated as
    /// a container for this generator's host, the method returns DependencyProperty.UnsetValue
    BaseComponent* ItemFromContainer(DependencyObject* container) const;

    /// Starts a container generation batch
    void StartBatch();

    /// Stops current container generation batch
    void StopBatch();

    /// From IItemContainerGenerator
    //@{
    ItemContainerGenerator* GetItemContainerGeneratorForPanel(Panel* panel) const override;
    GeneratorPosition GeneratorPositionFromIndex(int itemIndex) const override;
    int IndexFromGeneratorPosition(const GeneratorPosition& position) const override;
    void StartAt(const GeneratorPosition& position, GeneratorDirection direction) override;
    void StartAt(const GeneratorPosition& position, GeneratorDirection direction,
        bool allowStartAtRealizedItem) override;
    Ptr<DependencyObject> GenerateNext() override;
    Ptr<DependencyObject> GenerateNext(bool& isNewlyRealized) override;
    void Stop() override;
    void PrepareItemContainer(DependencyObject* container) const override;
    void Remove(const GeneratorPosition& position, uint32_t count) override;
    void RemoveAll() override;
    //@}

    /// From IRecyclingItemContainerGenerator
    //@{
    void Recycle(const GeneratorPosition& position, uint32_t count) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    friend class ItemsControl;
    friend class VirtualizingStackPanelTestHelper;

    ItemContainerGenerator();
    ItemContainerGenerator(ItemsControl* host);

    void Refresh();

    void ResetLastNodeIndex() const;

    void SetStatus(GeneratorStatus status);

    Ptr<DependencyObject> GenerateNext(bool stopAtRealized, bool& isNewlyRealized);

    static BaseComponent* GetContainerItem(DependencyObject* container);
    static void SetContainerItem(DependencyObject* container, BaseComponent* item);

    void OnItemsChanged(BaseComponent* sender, const NotifyCollectionChangedEventArgs& e);
    void OnItemAdded(BaseComponent* item, int index);
    void OnItemRemoved(BaseComponent* item, int index);
    void OnItemReplaced(BaseComponent* oldItem, BaseComponent* newItem, int index);
    void OnItemMoved(BaseComponent* item, int oldIndex, int newIndex);
    void OnReset();

    void AttachContainerItem(DependencyObject* container, BaseComponent* item);
    void DetachContainerItem(DependencyObject* container, BaseComponent* item,
        bool setDisconnected);

    void UpdateAlternationCount(int oldValue, int newValue);

private:
    friend class ItemContainerGeneratorHelperTest;
    friend class ItemContainerGeneratorTest;

    // Host for generator items
    ItemsControl* mHost;

    // Stores info about one (realized) or several (unrealized) items
    struct ItemNode
    {
        enum Type { Unrealized, Realized };
        Type type;

        union
        {
            struct
            {
                uint32_t numItems;
            }
            unrealized;

            struct
            {
                BaseComponent* item;
                DependencyObject* container;
            }
            realized;
        };

        ItemNode(uint32_t n = 0);
        ItemNode(BaseComponent* i, DependencyObject* c);
        ItemNode(const ItemNode& node);
        ItemNode(ItemNode&& node);

        ~ItemNode();

        ItemNode& operator=(const ItemNode& node);
        ItemNode& operator=(ItemNode&& node);

        uint32_t NumItems() const;
        uint32_t NumContainers() const;

        BaseComponent* Item() const;
        DependencyObject* Container() const;

        bool MoveAllowed(bool allowMove) const;

        bool IsRealized() const;

        void Unrealize(uint32_t n);
        void Realize(BaseComponent* i, DependencyObject* c);

    private:
        void CopyFrom(const ItemNode& node);
        void MoveFrom(ItemNode& node);
        void UnrealizeInternal(uint32_t n);
        void RealizeInternal(BaseComponent* i, DependencyObject* c);
        void Release();
    };

    // Holds correspondence between Items and their Containers. Usually a group of unrealized
    // (virtualized) items, followed by some realized (visible) items, and a final group of
    // unrealized items:
    //
    //       ------------      --------------             --------------      ------------ 
    //      |     n      |    | 1 item*      |           | 1 item*      |    |     n      |
    //      | unrealized | -> | 1 container* | -> ... -> | 1 container* | -> | unrealized |
    //       ------------      --------------             --------------      ------------ 
    //
    typedef Vector<ItemNode> ItemNodes;
    ItemNodes mItemNodes;
    uint32_t mNumItems;

    void Remove(const GeneratorPosition& position, uint32_t count, bool recycleContainers,
        const char* actionName);
    void Remove(uint32_t startIt, int startIndex, uint32_t count, bool allowUnrealizedItems,
        bool recycleContainers, const char* actionName);
    int RemoveItem(uint32_t it);
    void CoalesceNodes(uint32_t prevIt, uint32_t nextIt);
    uint32_t FindNodeFromIndex(int index, int& itemIndex, int& offset);
    void MoveForward();
    void MoveForward(int offset, bool allowStartAtRealizedItem);
    void MoveBackward();
    void MoveBackward(int offset, bool allowStartAtRealizedItem);

    // Stores recycled containers to be used in future generations
    typedef Vector<Ptr<DependencyObject>> RecycledContainers;
    RecycledContainers mRecycledContainers;

    // Generator status
    GeneratorStatus mStatus;
    bool mBatching;

    // Generation state
    GeneratorDirection mDirection;
    uint32_t mNode; // node where next generation should occur
    int mOffset; // offset inside the current node where next generation should occur
    int mItemIndex; // index of next generated item

    // Events
    EventHandler mStatusChanged;
    ItemsChangedEventHandler mItemsChanged;

    // Stores a reference to the item in the container to speed up some searches
    static const DependencyProperty* ContainerItemProperty;

    // Optimization for ContainerFromIndex look-ups
    mutable uint32_t mLastNode;
    mutable int mLastIndex;

    NS_DECLARE_REFLECTION(ItemContainerGenerator, BaseComponent)
};

NS_WARNING_POP

}

#include <NsGui/ItemContainerGenerator.inl>

#endif
