////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPEENUMHELPER_H__
#define __CORE_TYPEENUMHELPER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Reflection.h>
#include <NsCore/TypeEnumCreator.h>


namespace Noesis
{

class TypeEnum;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper to create reflection for enums.
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class TypeEnumHelper
{
public:
    static const TypeEnum* GetType(const char* name)
    {
        static const TypeEnum* type;
        
        if (NS_UNLIKELY(type == 0))
        {
            type = (const TypeEnum*)(Reflection::RegisterType(name, TypeEnumCreator<T>::Create,
                TypeEnumCreator<T>::Fill));
        }
        
        return type;
    }
};

}

#endif
