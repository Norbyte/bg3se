////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Noesis
{

namespace SymbolManager
{

/// Add a string to the symbol table and return the index identifying the string
/// This function copies the string to an internal buffer
NS_CORE_KERNEL_API uint32_t AddString(const char* str);

/// Add a static string to the symbol table and return the index identifying the string
/// The string is not copied internally and the pointer is suppored to remain valid
NS_CORE_KERNEL_API uint32_t AddStaticString(const char* str);

/// Find a string in the symbol table
NS_CORE_KERNEL_API uint32_t FindString(const char* str);

/// Get the string associated to a symbol index
NS_CORE_KERNEL_API const char* GetString(uint32_t index);

/// This function must be invoked before any symbol is created
void Init();

/// Clears all the symbols and deallocates all internal memory
void Shutdown();

}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol::Symbol(): mIndex(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol::Symbol(uint32_t index): mIndex(index)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol::Symbol(const char* str): mIndex(SymbolManager::AddString(str))
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol::Symbol(const char* str, NullIfNotFound): mIndex(SymbolManager::FindString(str))
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char* Symbol::Str() const
{
    return SymbolManager::GetString(mIndex);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Symbol::IsNull() const
{
    return mIndex == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol::operator uint32_t() const
{
    return mIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol Symbol::Null()
{
    return Symbol(uint32_t(0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol Symbol::Static(const char* str)
{
    return Symbol(SymbolManager::AddStaticString(str));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> struct HashKeyInfo;
template<> struct HashKeyInfo<Symbol>
{
    static bool IsEmpty(Symbol key) { return key == 0xffffffff; }
    static void MarkEmpty(Symbol& key) { *(uint32_t*)&key = 0xffffffff; }
    static uint32_t HashValue(Symbol key) { return key; }
    static bool IsEqual(Symbol lhs, Symbol rhs) { return lhs == rhs; }
};

}
