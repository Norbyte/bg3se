////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_HANDOFFBEHAVIOR_H__
#define __GUI_HANDOFFBEHAVIOR_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclareEnum.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies how new animations interact with any existing ones that are already applied to a 
/// property.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum HandoffBehavior: int32_t
{
    /// New animations replace any existing animations on the properties to which they are applied.
    HandoffBehavior_SnapshotAndReplace,
    /// New animations are combined with existing animations by appending the new animations to the 
    /// end of the composition chain.
    HandoffBehavior_Compose
};

}

NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CORE_API, Noesis::HandoffBehavior)


#endif
