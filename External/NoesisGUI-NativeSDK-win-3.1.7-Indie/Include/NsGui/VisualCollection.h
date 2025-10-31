////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VISUALCOLLECTION_H__
#define __GUI_VISUALCOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/UICollection.h>
#include <NsGui/Visual.h>


namespace Noesis
{

class Visual;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an ordered collection of Visual objects. 
///
/// https://msdn.microsoft.com/en-us/library/system.windows.media.visualcollection.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API VisualCollection final: public UICollection<Visual>
{
public:
    VisualCollection(Visual* visualParent);
    ~VisualCollection();

protected:
    /// From BaseUICollection
    //@{
    void OnItemAdded(BaseComponent* item) override;
    void OnItemRemoved(BaseComponent* item) override;
    void OnItemsRemoved() override;
    //@}

private:
    void OnVisualParentDestroyed(DependencyObject* d);

private:
    Visual* mVisualParent;

    NS_DECLARE_REFLECTION(VisualCollection, BaseUICollection)
};

}


#endif
