////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_SYMBOL_H__
#define __CORE_SYMBOL_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A Symbol is a shared string stored in a table and represented by an integer. As same strings
/// correspond to the same index, symbols can be compared very efficiently
////////////////////////////////////////////////////////////////////////////////////////////////////
class Symbol
{
public:
    Symbol();
    explicit Symbol(uint32_t index);
    explicit Symbol(const char* str);

    /// This is a special constructor that will not add the string to the iternal table
    struct NullIfNotFound {};
    Symbol(const char* str, NullIfNotFound);

    /// Gets symbol string
    const char* Str() const;

    /// Checks if symbol is empty string
    bool IsNull() const;

    /// Conversion to int
    operator uint32_t() const;

    /// Gets symbol for emptry string
    static Symbol Null();

    /// Returns a symbol created from a static string (character are not internally copied)
    static Symbol Static(const char* str);

private:
    uint32_t mIndex;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper macros for using symbols. The macro NS_DECLARE_SYMBOL implements a static symbol and
/// the NSS macro reuses that symbol. Symbol is created first time it is accessed
///
///  NS_DECLARE_SYMBOL(name)
///
///  void Test()
///  {
///      Symbol sym = NSS(name);
///  }
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_DECLARE_SYMBOL(id) static Noesis::Symbol __sSymbol##id;

#define NSS(id) (NS_LIKELY((__sSymbol##id) != 0) ? (__sSymbol##id): \
    ( __sSymbol##id = Noesis::Symbol(Noesis::SymbolManager::AddStaticString(#id)), __sSymbol##id) )

}

#include <NsCore/Symbol.inl>

#endif
