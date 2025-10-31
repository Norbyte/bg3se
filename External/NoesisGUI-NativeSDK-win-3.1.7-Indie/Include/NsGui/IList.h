////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ILIST_H__
#define __GUI_ILIST_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>


namespace Noesis
{

template<class T> class Ptr;
class BaseComponent;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a generic collection of objects that can be individually accessed by index.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IList: public Interface
{
    /// Gets the item at the specified index
    virtual Ptr<BaseComponent> GetComponent(uint32_t index) const = 0;

    /// Sets the item at the specified index
    virtual void SetComponent(uint32_t index, BaseComponent* item) = 0;

    /// Adds an item to the collection. Returns The position into which the new element was
    /// inserted, or -1 to indicate that the item was not inserted into the collection
    virtual int AddComponent(BaseComponent* item) = 0;

    /// Determines the index of a specific item in the collection. Returns -1 if not found
    virtual int IndexOfComponent(const BaseComponent* item) const = 0;

    /// Gets the number of items in the collection
    virtual int Count() const = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IList, Interface)
};

}


#endif
