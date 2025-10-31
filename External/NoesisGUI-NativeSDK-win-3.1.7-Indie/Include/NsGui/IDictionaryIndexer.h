////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IDICTIONARYINDEXER_H__
#define __GUI_IDICTIONARYINDEXER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>


namespace Noesis
{

template<class T> class Ptr;
class BaseComponent;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an indexer object that can be accessed with a string key.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IDictionaryIndexer: public Interface
{
    /// Gets the item associated with the specified key.
    /// Returns false if key is not found in the dictionary
    virtual bool TryGet(const char* key, Ptr<BaseComponent>& item) const = 0;

    /// Sets the item associated with the specified key.
    /// Returns false if key is not found in the dictionary
    virtual bool TrySet(const char* key, BaseComponent* item) = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IDictionaryIndexer, Interface)
};

}


#endif
