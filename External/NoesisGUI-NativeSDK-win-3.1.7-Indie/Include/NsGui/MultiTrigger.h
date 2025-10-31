////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MULTITRIGGER_H__
#define __GUI_MULTITRIGGER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BaseTrigger.h>


namespace Noesis
{

class BaseSetter;
class Setter;
class Condition;

template<class T> class UICollection;
typedef UICollection<BaseSetter> BaseSetterCollection;
typedef UICollection<Condition> ConditionCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a trigger that applies property values or performs actions when a set of conditions
/// are satisfied.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.multitrigger.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API MultiTrigger: public BaseTrigger
{
public:
    MultiTrigger();
    ~MultiTrigger();

    /// Gets a collection of Condition objects. Changes to property values are applied when all of 
    /// the conditions in the collection are met.
    ConditionCollection* GetConditions() const;

    /// Gets a collection of Setter objects, which describe the property values to apply when all 
    /// of the conditions of the MultiTrigger are met.
    BaseSetterCollection* GetSetters() const;

    /// From BaseTrigger
    //@{
    BaseComponent* FindValue(FrameworkElement* target, FrameworkElement* nameScope,
        DependencyObject* object, const DependencyProperty* dp, bool skipSourceName,
        bool skipTargetName) final;
    void Invalidate(FrameworkElement* target, FrameworkElement* nameScope, bool skipSourceName,
        bool skipTargetName, uint8_t priority) final;
    void Invalidate(FrameworkElement* target, FrameworkElement* nameScope,
        FrameworkElement* changedObject, const DependencyProperty* changedProperty,
        const void* oldValue, const void* newValue, bool skipSourceName, bool skipTargetName,
        uint8_t priority) final;
    void Seal() override;
    //@}

    ConditionCollection* InternalGetConditions() const; // can return null
    BaseSetterCollection* InternalGetSetters() const; // can return null

private:
    bool Matches(FrameworkElement* target, FrameworkElement* nameScope, DependencyObject* object,
        const DependencyProperty* dp, bool skipSourceName, bool skipSelf) const;
    bool Matches(FrameworkElement* target, FrameworkElement* nameScope,
        FrameworkElement* changedObject, const DependencyProperty* changedProperty,
        const void* changedValue, bool skipSourceName) const;

    void EnsureConditions() const;
    void EnsureSetters() const;

    void CheckConditions(ConditionCollection* conditions) const;

private:
    mutable Ptr<ConditionCollection> mConditions;
    mutable Ptr<BaseSetterCollection> mSetters;

    NS_DECLARE_REFLECTION(MultiTrigger, BaseTrigger)
};

NS_WARNING_POP

}

#endif
