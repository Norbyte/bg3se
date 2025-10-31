////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_HIGHRESTIMER_H__
#define __CORE_HIGHRESTIMER_H__


#include <NsCore/Noesis.h>


namespace Noesis
{
namespace HighResTimer
{

/// Retrieves the current value of the high resolution timer in ticks (CPU dependent unit)
uint64_t Ticks();

/// Converts from ticks to seconds
double Seconds(uint64_t ticks);

}
}

#include <NsCore/HighResTimer.inl>

#endif
