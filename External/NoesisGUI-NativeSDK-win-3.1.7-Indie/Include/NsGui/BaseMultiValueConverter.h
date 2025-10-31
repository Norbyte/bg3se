////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASEMULTIVALUECONVERTER_H__
#define __GUI_BASEMULTIVALUECONVERTER_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>
#include <NsGui/IMultiValueConverter.h>
#include <NsGui/IUITreeNode.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for value converters used in MultiBindings.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseMultiValueConverter: public BaseComponent, public IMultiValueConverter, 
    public IUITreeNode
{
public:
    BaseMultiValueConverter();
    ~BaseMultiValueConverter();

    /// From IMultiValueConverter
    //@{
    bool TryConvert(ArrayRef<BaseComponent*> values, const Type* targetType,
        BaseComponent* parameter, Ptr<BaseComponent>& result) override;
    bool TryConvertBack(BaseComponent* value, ArrayRef<const Type*> targetTypes,
        BaseComponent* parameter, BaseVector<Ptr<BaseComponent>>& results) override;
    //@}

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

    NS_DECLARE_REFLECTION(BaseMultiValueConverter, BaseComponent)
};

NS_WARNING_POP

}

#endif
