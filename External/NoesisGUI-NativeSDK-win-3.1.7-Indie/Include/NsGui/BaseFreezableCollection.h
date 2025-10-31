////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASEFREEZABLECOLLECTION_H__
#define __GUI_BASEFREEZABLECOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Animatable.h>
#include <NsGui/IList.h>
#include <NsGui/INotifyCollectionChanged.h>
#include <NsGui/INotifyPropertyChanged.h>
#include <NsCore/Vector.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class implementation for a FreezableCollection.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseFreezableCollection: public Animatable, public IList,
    public INotifyCollectionChanged, public INotifyPropertyChanged
{
public:
    BaseFreezableCollection();
    virtual ~BaseFreezableCollection() = 0;

    /// Removes the item at the specified index
    void RemoveAt(uint32_t index);

    /// Removes all items from the collection
    void Clear();

    /// From IList
    //@{
    Ptr<BaseComponent> GetComponent(uint32_t index) const final;
    void SetComponent(uint32_t index, BaseComponent* item) final;
    int AddComponent(BaseComponent* item) final;
    int IndexOfComponent(const BaseComponent* item) const final;
    int Count() const final;
    //@}

    /// From INotifyCollectionChanged
    //@{
    NotifyCollectionChangedEventHandler& CollectionChanged() final;
    //@}

    /// From INotifyPropertyChanged
    //@{
    PropertyChangedEventHandler& PropertyChanged() final;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

protected:
    void Set(uint32_t index, BaseComponent* item);
    int Add(BaseComponent* item);
    void Insert(uint32_t index, BaseComponent* item);
    bool Remove(const BaseComponent* item);

    /// Allows inheritors to perform specific actions when items are added or removed
    //@{
    virtual void InsertItem(uint32_t index, BaseComponent* item);
    virtual void SetItem(uint32_t index, BaseComponent* item);
    virtual void RemoveItem(uint32_t index);
    virtual void ClearItems();
    //@}

    /// Gets the permitted type of items for this collection
    virtual const TypeClass* GetItemType() const = 0;

    /// From Freezable
    //@{
    bool FreezeCore(bool isChecking) override;
    void CloneCore(const Freezable* source) override;
    void CloneCurrentValueCore(const Freezable* source) override;
    void GetAsFrozenCore(const Freezable* source) override;
    void GetCurrentValueAsFrozenCore(const Freezable* source) override;
    //@}

    /// From DependencyObject
    //@{
    void OnInit() override;
    //@}

protected:
    Vector<Ptr<BaseComponent>> mItems;

private:
    bool CheckFrozen();
    bool CheckNull(BaseComponent* item) const;
    bool CheckType(BaseComponent* item) const;

    void DisconnectItems();

private:
    NotifyCollectionChangedEventHandler mCollectionChanged;
    PropertyChangedEventHandler mPropertyChanged;

    friend class VisualTreeInspectorHelper;
    NS_DECLARE_REFLECTION(BaseFreezableCollection, Animatable)
};

NS_WARNING_POP

}


#endif
