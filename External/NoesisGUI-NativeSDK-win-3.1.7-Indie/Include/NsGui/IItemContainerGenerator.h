////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IITEMCONTAINERGENERATOR_H__
#define __GUI_IITEMCONTAINERGENERATOR_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class DependencyObject;
class ItemContainerGenerator;
class Panel;
template<class T> class Ptr;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// This enum is used by the ItemContainerGenerator to indicate its status.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum GeneratorStatus
{
    /// The generator has not tried to generate content
    GeneratorStatus_NotStarted,
    /// The generator is generating containers
    GeneratorStatus_GeneratingContainers,
    /// The generator has finished generating containers
    GeneratorStatus_ContainersGenerated,
    /// The generator has finished generating containers, but encountered one or more errors
    GeneratorStatus_Error
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies the direction in which item generation will occur.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum GeneratorDirection
{
    /// Specifies to generate items in a forward direction
    GeneratorDirection_Forward,
    /// Specifies to generate items in a backward direction
    GeneratorDirection_Backward
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to describe the position of an item that is managed by ItemContainerGenerator.
/// Examples:
///  - To start generating forward from the beginning of the item list, specify position (-1, 0)
///     and direction Forward.
///  - To start generating backward from the end of the list, specify position (-1, 0) and
///     direction Backward.
///  - To generate the items after the element with index k, specify position (k, 0) and
///     direction Forward.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GeneratorPosition
{
    /// Index, with respect to realized elements.  The special value -1 refers to a fictitious
    /// element at the beginning or end of the the list
    int index;

    /// Offset, with respect to unrealized items near the indexed element. An offset of 0 refers to
    /// the indexed element itself, an offset of 1 refers to the next (unrealized) item, and an
    /// offset of -1 refers to the previous item
    int offset;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Interface through which a layout element (such as a panel) marked as an ItemsHost communicates
/// with the ItemContainerGenerator of its items owner.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IItemContainerGenerator: public Interface
{
    /// Return the ItemContainerGenerator appropriate for use by the given panel
    virtual ItemContainerGenerator* GetItemContainerGeneratorForPanel(Panel* panel) const = 0;

    /// Returns the GeneratorPosition object that maps to the item at the specified index
    virtual GeneratorPosition GeneratorPositionFromIndex(int itemIndex) const = 0;

    /// Returns the index that maps to the specified GeneratorPosition
    virtual int IndexFromGeneratorPosition(const GeneratorPosition& position) const = 0;

    /// Prepare the generator to generate, starting at the given position and direction. This method
    /// must be called before calling *GenerateNext()*. This method sets the generator's status to
    /// *GeneratingContainers*; when generation is stopped, the status changes to
    /// *ContainersGenerated* or *Error*, as appropriate
    virtual void StartAt(const GeneratorPosition& position, GeneratorDirection direction) = 0;

    /// Prepare the generator to generate, starting at the given position and direction. This method
    /// must be called before calling *GenerateNext()*. This method sets the generator's status to
    /// *GeneratingContainers*; when generation is stopped, the status changes to
    /// *ContainersGenerated* or *Error*, as appropriate. The parameter *allowStartAtRealizedItem*
    /// specifies whether to start at a generated item or not
    virtual void StartAt(const GeneratorPosition& position, GeneratorDirection direction,
        bool allowStartAtRealizedItem) = 0;

    /// Returns the container element used to display the next item
    virtual Ptr<DependencyObject> GenerateNext() = 0;

    /// Returns the container element used to display the next item. The parameter *isNewlyRealized*
    /// will return true if the returned container is newly generated (realized); otherwise, false
    virtual Ptr<DependencyObject> GenerateNext(bool& isNewlyRealized) = 0;

    /// Stops generation of containers
    virtual void Stop() = 0;

    /// Prepares the specified element as the container for the corresponding item
    virtual void PrepareItemContainer(DependencyObject* container) const = 0;

    /// Removes one or more generated (realized) items. The *position* must refer to a previously
    /// generated item, which means its position offset must be 0. The parameter *count* indicates
    /// the number of elements to remove, starting at specified *position*
    virtual void Remove(const GeneratorPosition& position, uint32_t count) = 0;

    /// Removes all generated (realized) items
    virtual void RemoveAll() = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IItemContainerGenerator, Interface)
};

}

#endif
