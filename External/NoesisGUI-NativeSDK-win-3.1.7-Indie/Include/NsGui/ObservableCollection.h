////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_OBSERVABLECOLLECTION_H__
#define __GUI_OBSERVABLECOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsCore/TypeOf.h>
#include <NsGui/BaseObservableCollection.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a dynamic data collection that provides notifications when items get added or
/// removed.
///
/// It is frequently used in data binding to display a collection of records within an
/// *ItemsControl* such as a *ListBox*, *ListView* or *TreeView*.
///
/// .. code-block:: xml
///
///     <ListBox Width="200"
///         ItemsSource="{Binding Source={StaticResource NameListData}}"
///         ItemTemplate="{StaticResource NameItemTemplate}" />
///
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class ObservableCollection: public BaseObservableCollection
{
public:
    /// Gets the element at the specified index
    inline T* Get(uint32_t index) const;

    /// Sets the element at the specified index
    inline void Set(uint32_t index, T* item);

    /// Adds an item to the collection. Returns The position into which the new element was
    /// inserted, or -1 to indicate that the item was not inserted into the collection
    inline int Add(T* item);

    /// Inserts an item to the collection at the specified index
    inline void Insert(uint32_t index, T* item);

    /// Determines whether the collection contains a specific value
    inline bool Contains(const T* item) const;

    /// Determines the index of a specific item in the collection. Returns -1 if not found
    inline int IndexOf(const T* item) const;

    /// Removes the first occurrence of a specific object from the collection. Returns true if item
    /// was removed, false to indicate that the item was not found in the collection
    inline bool Remove(const T* item);

private:
    inline const TypeClass* GetItemType() const override;

    NS_IMPLEMENT_INLINE_REFLECTION_(ObservableCollection<T>, BaseObservableCollection)
};

}

#include <NsGui/ObservableCollection.inl>


#endif
