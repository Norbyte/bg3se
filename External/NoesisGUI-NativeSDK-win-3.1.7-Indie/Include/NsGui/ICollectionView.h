////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ICOLLECTIONVIEW_H__
#define __GUI_ICOLLECTIONVIEW_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsGui/Events.h>


namespace Noesis
{

class BaseComponent;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Enables collections to have the functionalities of current record management, custom sorting,
/// filtering, and grouping. 
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE ICollectionView: public Interface
{
    /// Gets a value that indicates whether this view supports filtering via the Filter property
    /// \prop
    virtual bool CanFilter() const = 0;

    /// Gets a value that indicates whether this view supports grouping via GroupDescriptions 
    /// \prop
    virtual bool CanGroup() const = 0;

    /// Gets a value that indicates whether this view supports sorting via SortDescriptions 
    /// \prop
    virtual bool CanSort() const = 0;

    /// Returns a value that indicates whether a given item belongs to this collection view
    virtual bool Contains(const BaseComponent* item) const = 0;

    /// Gets the current item in the view
    /// \prop
    virtual Ptr<BaseComponent> CurrentItem() const = 0;

    /// Gets the ordinal position of the CurrentItem within the view
    /// \prop
    virtual int CurrentPosition() const = 0;

    /// Gets a value that indicates whether the CurrentItem of the view is beyond the end of the 
    /// collection
    /// \prop
    virtual bool IsCurrentAfterLast() const = 0;

    /// Gets a value that indicates whether the CurrentItem of the view is beyond the beginning of 
    /// the collection
    /// \prop
    virtual bool IsCurrentBeforeFirst() const = 0;

    /// Returns a value that indicates whether the resulting view is empty.
    /// \prop
    virtual bool IsEmpty() const = 0;

    /// Sets the specified item to be the CurrentItem in the view
    virtual bool MoveCurrentTo(BaseComponent* item) = 0;

    /// Sets the first item in the view as the CurrentItem
    virtual bool MoveCurrentToFirst() = 0;

    /// Sets the last item in the view as the CurrentItem
    virtual bool MoveCurrentToLast() = 0;

    /// Sets the item after the CurrentItem in the view as the CurrentItem
    virtual bool MoveCurrentToNext() = 0;

    /// Sets the item at the specified index to be the CurrentItem in the view
    virtual bool MoveCurrentToPosition(int position) = 0;

    /// Sets the item before the CurrentItem in the view as the CurrentItem
    virtual bool MoveCurrentToPrevious() = 0;

    /// Recreates the view
    virtual void Refresh() = 0;

    /// Occurs after the CurrentItem has changed
    virtual EventHandler& CurrentChanged() = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(ICollectionView, Interface)
};

}

#endif
