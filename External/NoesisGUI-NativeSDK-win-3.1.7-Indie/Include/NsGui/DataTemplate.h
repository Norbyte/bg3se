////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DATATEMPLATE_H__
#define __GUI_DATATEMPLATE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/FrameworkTemplate.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes the visual structure of a data object.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.datatemplate.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API DataTemplate: public FrameworkTemplate
{
public:
    DataTemplate();
    ~DataTemplate();

    /// Gets or sets the type for which this DataTemplate is intended
    //@{
    const Type* GetDataType() const;
    void SetDataType(const Type* dataType);
    //@}
    
    /// Gets a collection of triggers that apply property values or perform actions based on one or 
    /// more conditions.
    TriggerCollection* GetTriggers() const;

protected:
    /// From FrameworkTemplate
    //@{
    bool IsValidTemplatedParent(FrameworkElement* templatedParent) const;
    TriggerCollection* GetAvailableTriggers() const final;
    //@}

private:
    void EnsureTriggers() const;

private:
    const Type* mDataType;
    mutable Ptr<TriggerCollection> mTriggers;

    NS_DECLARE_REFLECTION(DataTemplate, FrameworkTemplate)
};

NS_WARNING_POP

}

#endif
