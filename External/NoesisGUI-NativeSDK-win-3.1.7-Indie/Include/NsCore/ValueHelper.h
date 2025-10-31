////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_VALUEHELPER_H__
#define __CORE_VALUEHELPER_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>
#include <NsCore/String.h>


namespace Noesis
{

/// Returns a string that represents the passed value
template<class T> String ToString(const T& value);

/// Comparison helper with special cases for NaN values
template<class T> bool Equals(const T& left, const T& right);
bool Equals(float left, float right);
bool Equals(double left, double right);

}

// Inline include
#include <NsCore/ValueHelper.inl>


#endif
