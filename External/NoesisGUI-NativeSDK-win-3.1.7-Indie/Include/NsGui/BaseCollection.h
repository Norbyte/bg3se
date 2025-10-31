////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASECOLLECTION_H__
#define __GUI_BASECOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Vector.h>
#include <NsGui/CoreApi.h>
#include <NsGui/IList.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides the base class for a generic collection.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseCollection: public BaseComponent, public IList
{
public:
    BaseCollection();
    virtual ~BaseCollection() = 0;

    /// Removes the item at the specified index
    void RemoveAt(uint32_t index);

    /// Removes all items from the collection
    void Clear();

    /// From IList
    //@{
    Ptr<BaseComponent> GetComponent(uint32_t index) const;
    void SetComponent(uint32_t index, BaseComponent* item);
    int AddComponent(BaseComponent* item);
    int IndexOfComponent(const BaseComponent* item) const;
    int Count() const;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

public:
    void Set(uint32_t index, BaseComponent* item);
    int Add(BaseComponent* item);
    void Insert(uint32_t index, BaseComponent* item);
    bool Remove(const BaseComponent* item);

    // Allows inheritors to perform specific actions when items are added or removed
    //@{
    virtual void InsertItem(uint32_t index, BaseComponent* item);
    virtual void SetItem(uint32_t index, BaseComponent* item);
    virtual void RemoveItem(uint32_t index);
    virtual void ClearItems();
    //@}

    // Gets the permitted type of items for this collection. Default type is BaseComponent
    virtual const TypeClass* GetItemType() const;

protected:
    Vector<Ptr<BaseComponent>> mItems;

private:
    bool CheckType(BaseComponent* item) const;

    NS_DECLARE_REFLECTION(BaseCollection, BaseComponent)
};

NS_WARNING_POP

}


#endif
