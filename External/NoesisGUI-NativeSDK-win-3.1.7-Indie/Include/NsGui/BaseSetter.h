////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASESETTER_H__
#define __GUI_BASESETTER_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>
#include <NsGui/IUITreeNode.h>


namespace Noesis
{

class BaseSetter;
template<class T> class UICollection;
typedef UICollection<BaseSetter> BaseSetterCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the base class for value setters.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.setterbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseSetter: public BaseComponent, public IUITreeNode
{
public:
    BaseSetter();
    BaseSetter(const BaseSetter&) = delete;
    BaseSetter& operator=(const BaseSetter&) = delete;
    virtual ~BaseSetter() = 0;

    // Seals a collection of setters
    static void SealSetters(BaseSetterCollection* setters);

    // Seals this setter
    virtual void Seal();

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const final;
    void SetNodeParent(IUITreeNode* parent) final;
    BaseComponent* FindNodeResource(const char* key, bool fullElementSearch) const final;
    BaseComponent* FindNodeName(const char* name) const final;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const final;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

protected:
    bool mIsSealed;

private:
    IUITreeNode* mOwner;

    NS_DECLARE_REFLECTION(BaseSetter, BaseComponent)
};

NS_WARNING_POP

}

#endif
