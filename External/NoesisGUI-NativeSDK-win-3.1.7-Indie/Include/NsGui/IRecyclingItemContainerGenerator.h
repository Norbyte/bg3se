////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IRECYCLINGITEMCONTAINERGENERATOR_H__
#define __GUI_IRECYCLINGITEMCONTAINERGENERATOR_H__


#include <NsCore/Noesis.h>
#include <NsGui/IItemContainerGenerator.h>


namespace Noesis
{

class DependencyObject;
class ItemContainerGenerator;
class Panel;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Interface through which a layout element (such as a panel) marked as an ItemsHost communicates
/// with the ItemContainerGenerator of its items owner. This interface adds the notion of recycling
/// a container and is used for virtualizing panels.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IRecyclingItemContainerGenerator: public IItemContainerGenerator
{
    /// Recycle generated elements. Equivalent to *Remove()* except that the generator retains this
    /// container in a list. This container will be handed back in a future call to
    /// *GenerateNext()*. The *position* must refer to a previously generated item, which means its
    /// position offset must be 0. The parameter *count* is the number of elements to recycle,
    /// starting at *position*
    virtual void Recycle(const GeneratorPosition& position, uint32_t count) = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IRecyclingItemContainerGenerator, IItemContainerGenerator)
};

}


#endif
