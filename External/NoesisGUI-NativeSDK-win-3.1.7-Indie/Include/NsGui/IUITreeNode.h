////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IUITREENODE_H__
#define __GUI_IUITREENODE_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsGui/CoreApi.h>

namespace Noesis
{

class BaseComponent;
class FrameworkElement;
NS_INTERFACE INameScope;

struct ObjectWithNameScope
{
    BaseComponent* object;
    INameScope* scope;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents nodes in a UI tree.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IUITreeNode: public Interface
{
    /// Gets or sets node parent
    //@{
    virtual IUITreeNode* GetNodeParent() const = 0;
    virtual void SetNodeParent(IUITreeNode* parent) = 0;
    //@}

    /// Looks for a UI resource upwards in the tree.
    /// Returns *DependencyProperty.UnsetValue* if resource is not found
    virtual BaseComponent* FindNodeResource(const char* key, bool fullElementSearch) const = 0;

    /// Looks for a named node upwards in the tree.
    /// Returns null if node is not found
    virtual BaseComponent* FindNodeName(const char* name) const = 0;
    
    /// Looks for a named node upwards in the tree, returning also the scope in which it was found.
    /// Returns null in both fields if node is not found
    virtual ObjectWithNameScope FindNodeNameAndScope(const char* name) const = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IUITreeNode, Interface)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper function that finds the first FrameworkElement ancestor for the given UI tree node
NS_GUI_CORE_API FrameworkElement* FindTreeElement(IUITreeNode* node);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper function to obtain the root of the UI Tree from the specified node
NS_GUI_CORE_API IUITreeNode* GetNodeRoot(IUITreeNode* node);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper functions to connect/disconnect nodes to/from the UI tree
//@{
NS_GUI_CORE_API void ConnectNode(IUITreeNode* node, const IUITreeNode* parent);
NS_GUI_CORE_API void DisconnectNode(IUITreeNode* node, const IUITreeNode* parent);
//@}

}


#endif
