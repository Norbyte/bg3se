////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_UIELEMENTCOLLECTION_H__
#define __GUI_UIELEMENTCOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>
#include <NsGui/UICollection.h>
#include <NsGui/UIElement.h>


namespace Noesis
{

class FrameworkElement;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an ordered collection of UIElement child elements.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.controls.uielementcollection.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API UIElementCollection: public UICollection<UIElement>
{
public:
    UIElementCollection(UIElement* visualParent, FrameworkElement* logicalParent);
    ~UIElementCollection();

    /// Gets logical parent of the items stored in this collection
    FrameworkElement* GetLogicalParent() const;

protected:
    /// From BaseUICollection
    //@{
    void OnItemAdded(BaseComponent* item) override;
    void OnItemRemoved(BaseComponent* item) override;
    void OnItemsRemoved() override;
    //@}

private:
    void AddChild(UIElement* child);
    void RemoveChild(UIElement* child);
    void RemoveChildren();

    void InvalidateVisualParentMeasure();

    void RegisterParents();
    void UnregisterParents();

    void OnVisualParentDestroyed(DependencyObject* d);
    void OnLogicalParentDestroyed(DependencyObject* d);

private:
    UIElement* mVisualParent;
    FrameworkElement* mLogicalParent;

    NS_DECLARE_REFLECTION(UIElementCollection, BaseUICollection)
};

}


#endif
