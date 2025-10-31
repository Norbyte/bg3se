////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DATATEMPLATESELECTOR_H__
#define __GUI_DATATEMPLATESELECTOR_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/IUITreeNode.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/BaseComponent.h>


namespace Noesis
{

class DataTemplate;
class DependencyObject;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a way to choose a DataTemplate based on the data object and the data-bound element.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.controls.datatemplateselector.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API DataTemplateSelector: public BaseComponent, public IUITreeNode
{
public:
    DataTemplateSelector();
    virtual ~DataTemplateSelector();

    /// When overridden in a derived class, returns a DataTemplate based on custom logic
    virtual DataTemplate* SelectTemplate(BaseComponent* item, DependencyObject* container);

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const final;
    void SetNodeParent(IUITreeNode* parent) final;
    BaseComponent* FindNodeResource(const char* key, bool fullElementSearch) const final;
    BaseComponent* FindNodeName(const char* name) const final;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const final;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    IUITreeNode* mOwner;

    NS_DECLARE_REFLECTION(DataTemplateSelector, BaseComponent)
};

NS_WARNING_POP

}

#endif
