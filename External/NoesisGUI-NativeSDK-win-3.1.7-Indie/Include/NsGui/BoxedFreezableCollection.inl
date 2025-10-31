////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
const typename Param<T>::Type  BoxedFreezableCollection<T>::Get(uint32_t index) const
{
    return Boxing::Unbox<T>(BaseFreezableCollection::GetComponent(index));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void BoxedFreezableCollection<T>::Set(uint32_t index, const typename Param<T>::Type item)
{
    BaseFreezableCollection::Set(index, Boxing::Box<T>(item));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
int BoxedFreezableCollection<T>::Add(const typename Param<T>::Type item)
{
    return BaseFreezableCollection::Add(Boxing::Box<T>(item));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void BoxedFreezableCollection<T>::Insert(uint32_t index, const typename Param<T>::Type  item)
{
    BaseFreezableCollection::Insert(index, Boxing::Box<T>(item));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
bool BoxedFreezableCollection<T>::Contains(const typename Param<T>::Type  item) const
{
    return BaseFreezableCollection::IndexOfComponent(Boxing::Box<T>(item)) >= 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
int BoxedFreezableCollection<T>::IndexOf(const typename Param<T>::Type  item) const
{
    return BaseFreezableCollection::IndexOfComponent(Boxing::Box<T>(item));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
bool BoxedFreezableCollection<T>::Remove(const typename Param<T>::Type  item)
{
    return BaseFreezableCollection::Remove(Boxing::Box<T>(item));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
const TypeClass* BoxedFreezableCollection<T>::GetItemType() const
{
    return TypeOf<Boxed<T>>();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
Ptr<BoxedFreezableCollection<T>> BoxedFreezableCollection<T>::Clone() const
{
    return StaticPtrCast<BoxedFreezableCollection<T>>(Freezable::Clone());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
Ptr<BoxedFreezableCollection<T>> BoxedFreezableCollection<T>::CloneCurrentValue() const
{
    return StaticPtrCast<BoxedFreezableCollection<T>>(Freezable::CloneCurrentValue());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
Ptr<Freezable> BoxedFreezableCollection<T>::CreateInstanceCore() const
{
    return *new BoxedFreezableCollection<T>();
}

}
