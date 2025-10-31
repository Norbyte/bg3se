////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ISEALABLE_H__
#define __GUI_ISEALABLE_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// This interface is shared across Freezable, Style and Template. A sealed object is free-threaded.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE ISealable: public Interface
{
    /// Indicates if the current instance can be sealed
    virtual bool CanSeal() const = 0;

    /// Indicates if the current instance is sealed
    virtual bool IsSealed() const = 0;

    /// Seals the current instance
    virtual void Seal() = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(ISealable, Interface)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void SealIfSealable(ISealable* sealable)
{
    if (sealable != 0 && !sealable->IsSealed() && sealable->CanSeal())
    {
        sealable->Seal();
    }
}

}

#endif
