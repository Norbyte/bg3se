////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_ILAYERMANAGER_H__
#define __CORE_ILAYERMANAGER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

class Visual;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Manages a list of visual elements that will be rendered over the tree.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE ILayerManager: public Interface
{
    /// Adds a visual to a new layer
    virtual void AddLayer(Visual* layerRoot) = 0;

    /// Removes a visual layer
    virtual void RemoveLayer(Visual* layerRoot) = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(ILayerManager, Interface)
};

}


#endif
