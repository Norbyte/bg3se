////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_LOGICALTREEHELPER_H__
#define __GUI_LOGICALTREEHELPER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

class FrameworkElement;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Contains static methods useful for performing common tasks with nodes in a logical tree.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API LogicalTreeHelper final
{
    /// Returns the logical parent element of the specified element
    static FrameworkElement* GetParent(const FrameworkElement* element);

    /// Returns the number of children that a parent element contains
    static uint32_t GetChildrenCount(const FrameworkElement* element);

    /// Returns the child logical object of a parent at the specified index
    static Ptr<BaseComponent> GetChild(const FrameworkElement* element, uint32_t index);

    /// Search for an element with specified name down the logical tree
    static Ptr<FrameworkElement> FindLogicalNode(const FrameworkElement* element, const char* name);
};

}


#endif
