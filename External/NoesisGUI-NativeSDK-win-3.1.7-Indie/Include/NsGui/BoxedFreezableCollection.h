////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BOXEDFREEZABLECOLLECTION_H__
#define __GUI_BOXEDFREEZABLECOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/BaseFreezableCollection.h>
#include <NsCore/ReflectionImplement.h>
#include <NsCore/CompilerTools.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of boxed objects (doubles, Points, ...).
/// BoxedFreezableCollection is itself an Animatable type.
////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
class BoxedFreezableCollection: public BaseFreezableCollection
{
public:
    static_assert(!IsDerived<T, BaseComponent>::Result, "T can't inherit from BaseComponent");

    /// Gets the element at the specified index
    inline const typename Param<T>::Type Get(uint32_t index) const;

    /// Sets the element at the specified index
    inline void Set(uint32_t index, const typename Param<T>::Type item);

    /// Adds an item to the collection. Returns The position into which the new element was
    /// inserted, or -1 to indicate that the item was not inserted into the collection
    inline int Add(const typename Param<T>::Type  item);

    /// Inserts an item to the collection at the specified index
    inline void Insert(uint32_t index, const typename Param<T>::Type  item);

    /// Determines whether the collection contains a specific value
    inline bool Contains(const typename Param<T>::Type  item) const;

    /// Determines the index of a specific item in the collection. Returns -1 if not found
    inline int IndexOf(const typename Param<T>::Type item) const;

    /// Removes the first occurrence of a specific object from the collection. Returns true if item
    /// was removed, false to indicate that the item was not found in the collection
    inline bool Remove(const typename Param<T>::Type  item);

    // Hides Freezable methods for convenience
    //@{
    Ptr<BoxedFreezableCollection<T>> Clone() const;
    Ptr<BoxedFreezableCollection<T>> CloneCurrentValue() const;
    //@}

protected:
    inline const TypeClass* GetItemType() const final;
    inline Ptr<Freezable> CreateInstanceCore() const override;

    NS_IMPLEMENT_INLINE_REFLECTION_(BoxedFreezableCollection<T>, BaseFreezableCollection,
        IdOf(IdOf<T>(), "Collection"))
};

}

#include <NsGui/BoxedFreezableCollection.inl>


#endif
