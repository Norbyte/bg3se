////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_BOXING_H__
#define __CORE_BOXING_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/CompilerTools.h>


namespace Noesis
{

class Type;
template<class T> class Ptr;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for boxed objects
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API BoxedValue: public BaseComponent
{
public:
    /// Returns the type of the boxed value
    virtual const Type* GetValueType() const = 0;

    /// Returns the address of the boxed value
    virtual const void* GetValuePtr() const = 0;

    NS_DECLARE_REFLECTION(BoxedValue, BaseComponent)
};

template<class T> class Boxed;

namespace Boxing
{

/// Converts a value type to BaseComponent
template<class T> EnableIf<IsBestByCopy<T>::Result, Ptr<BoxedValue>> Box(T value);
template<class T> EnableIf<!IsBestByCopy<T>::Result, Ptr<BoxedValue>> Box(const T& value);
inline Ptr<BoxedValue> Box(bool value);
inline Ptr<BoxedValue> Box(const char* text);
inline Ptr<BoxedValue> Box(char* text);

/// Returns whether the given object can be unboxed
template<class T> bool CanUnbox(BaseComponent* object);

/// Extracts a value type from BaseComponent
template<class T> class Boxer;
template<class T> typename Boxer<T>::UnboxType Unbox(BaseComponent* object);

}
}

#include <NsCore/Boxing.inl>

#endif
