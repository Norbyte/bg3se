////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASEOBSERVABLECOLLECTION_H__
#define __GUI_BASEOBSERVABLECOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BaseCollection.h>
#include <NsGui/INotifyCollectionChanged.h>
#include <NsGui/INotifyPropertyChanged.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Delegate.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for a dynamic data collection that provides notifications when items get added,
/// removed, or when the whole list is refreshed.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseObservableCollection: public BaseCollection,
    public INotifyCollectionChanged, public INotifyPropertyChanged
{
public:
    BaseObservableCollection();
    virtual ~BaseObservableCollection() = 0;

    /// Moves the item at the specified index to a new location in the collection
    void Move(uint32_t oldIndex, uint32_t newIndex);

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
    // Allows inheritors to perform specific actions when items are moved
    virtual void MoveItem(int oldIndex, int newIndex);

    /// From BaseCollection
    //@{
    void InsertItem(uint32_t index, BaseComponent* item) override;
    void SetItem(uint32_t index, BaseComponent* item) override;
    void RemoveItem(uint32_t index) override;
    void ClearItems() override;
    //@}

private:
    NotifyCollectionChangedEventHandler mCollectionChanged;
    PropertyChangedEventHandler mPropertyChanged;

    NS_DECLARE_REFLECTION(BaseObservableCollection, BaseCollection)
};

NS_WARNING_POP

}


#endif
