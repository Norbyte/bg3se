////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COLLECTION_H__
#define __GUI_COLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BaseCollection.h>
#include <NsCore/ReflectionImplementEmpty.h>
#include <NsCore/TypeOf.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a generic collection of objects that can be individually accessed by index.
////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
class Collection: public BaseCollection
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

protected:
    inline const TypeClass* GetItemType() const final;

    NS_IMPLEMENT_INLINE_REFLECTION_(Collection<T>, BaseCollection)
};

}

#include <NsGui/Collection.inl>


#endif
