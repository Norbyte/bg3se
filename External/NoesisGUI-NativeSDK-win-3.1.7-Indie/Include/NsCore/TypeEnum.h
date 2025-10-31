////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPEENUM_H__
#define __CORE_TYPEENUM_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>
#include <NsCore/Symbol.h>
#include <NsCore/TypeMeta.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Vector.h>
#include <NsCore/Pair.h>


namespace Noesis
{

template<typename T> class ArrayRef;
class BoxedValue;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class providing reflection for enumerations
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API TypeEnum: public TypeMeta
{
public:
    TypeEnum(Symbol name);

    /// Gets the value corresponding to the given enumeration name
    bool HasName(Symbol name, uint64_t& value) const;

    /// Gets the name corresponding to the given enumeration value
    bool HasValue(uint64_t value, Symbol& name) const;
    
    /// Gets a boxed value for the given enumeration name
    virtual bool GetValueObject(Symbol name, Ptr<BoxedValue>& value) const = 0;

    /// Adds a new enumeration value
    void AddValue(Symbol name, uint64_t value);

    /// Gets all values
    typedef Pair<Symbol, uint64_t> Value;
    ArrayRef<Value> GetValues() const;

public:
    typedef Vector<Value> Values;
    Values mValues;

    NS_DECLARE_REFLECTION(TypeEnum, TypeMeta)
};

NS_WARNING_POP

}

#endif
