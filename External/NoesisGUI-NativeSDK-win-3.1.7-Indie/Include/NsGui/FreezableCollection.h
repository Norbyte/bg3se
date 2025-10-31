////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_FREEZABLECOLLECTION_H__
#define __GUI_FREEZABLECOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BaseFreezableCollection.h>
#include <NsCore/ReflectionImplementEmpty.h>
#include <NsCore/IdOf.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of DependencyObject, Freezable, or Animatable objects.
/// FreezableCollection is itself an Animatable type.
///
/// This class is provided to enable the creation of custom collection types that fully support data
/// binding, change notification, property invalidation, and animation of its contents.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.freezablecollection-1
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
class FreezableCollection: public BaseFreezableCollection
{
public:
    /// Gets the element at the specified index
    T* Get(uint32_t index) const;

    /// Sets the element at the specified index
    void Set(uint32_t index, T* item);

    /// Adds an item to the collection. Returns The position into which the new element was
    /// inserted, or -1 to indicate that the item was not inserted into the collection
    int Add(T* item);

    /// Inserts an item to the collection at the specified index
    void Insert(uint32_t index, T* item);

    /// Determines whether the collection contains a specific value
    bool Contains(const T* item) const;

    /// Determines the index of a specific item in the collection. Returns -1 if not found
    int IndexOf(const T* item) const;

    /// Removes the first occurrence of a specific object from the collection. Returns true if item
    /// was removed, false to indicate that the item was not found in the collection
    bool Remove(const T* item);

    /// Creates a modifiable clone of this FreezableCollection and its contents, making deep copies
    Ptr<FreezableCollection<T>> Clone() const;

    /// Creates a modifiable copy of this FreezableCollection and its contents, making deep
    /// copies of this object's current values
    Ptr<FreezableCollection<T>> CloneCurrentValue() const;

protected:
    inline const TypeClass* GetItemType() const final;
    inline Ptr<Freezable> CreateInstanceCore() const override;

    NS_IMPLEMENT_INLINE_REFLECTION_(FreezableCollection<T>, BaseFreezableCollection,
        IdOf(IdOf<T>(), "Collection"))
};

}

#include <NsGui/FreezableCollection.inl>


#endif
