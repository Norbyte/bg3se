////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline ItemsChangedEventArgs::ItemsChangedEventArgs(NotifyCollectionChangedAction action_,
    const GeneratorPosition& position_, const GeneratorPosition& oldPosition_, int itemCount_,
    int itemUICount_): action(action_), position(position_), oldPosition(oldPosition_),
    itemCount(itemCount_), itemUICount(itemUICount_) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline ItemsChangedEventArgs::ItemsChangedEventArgs(NotifyCollectionChangedAction action_,
    const GeneratorPosition& position_, int itemCount_, int itemUICount_): action(action_),
    position(position_), oldPosition({ -1, 0 }), itemCount(itemCount_),
    itemUICount(itemUICount_) {}

}
