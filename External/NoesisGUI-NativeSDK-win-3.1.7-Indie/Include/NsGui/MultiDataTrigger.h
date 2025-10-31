////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MULTIDATATRIGGER_H__
#define __GUI_MULTIDATATRIGGER_H__


#include <NsCore/Noesis.h>
#include <NsCore/HashMap.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BaseTrigger.h>


namespace Noesis
{

class BaseSetter;
class Condition;

template<class T> class UICollection;
typedef UICollection<BaseSetter> BaseSetterCollection;
typedef UICollection<Condition> ConditionCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a trigger that applies property values or performs actions when the bound data meets
/// a set of conditions.
///
/// A MultiDataTrigger object is similar to a MultiTrigger, except that the conditions of a
/// MultiDataTrigger are based on property values of bound data instead of those of a UIElement.
/// In a MultiDataTrigger, a condition is met when the property value of the data item matches the
/// specified Value. You can then use setters or the *EnterActions* and *ExitActions* properties to
/// apply changes or start actions when all of the conditions are met.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.multidatatrigger.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API MultiDataTrigger: public BaseTrigger
{
public:
    MultiDataTrigger();
    ~MultiDataTrigger();

    /// Gets a collection of Condition objects. Changes to property values are applied when all of
    /// the conditions in the collection are met.
    ConditionCollection* GetConditions() const;

    /// Gets a collection of Setter objects, which describe the property values to apply when all
    /// of the conditions of the MultiTrigger are met.
    BaseSetterCollection* GetSetters() const;

    /// From BaseTrigger
    //@{
    void RegisterBindings(FrameworkElement* target, FrameworkElement* nameScope,
        bool skipTargetName, uint8_t priority) final;
    void UnregisterBindings(FrameworkElement* target) final;
    BaseComponent* FindValue(FrameworkElement* target, FrameworkElement* nameScope,
        DependencyObject* object, const DependencyProperty* dp, bool skipSourceName,
        bool skipTargetName) final;
    void Invalidate(FrameworkElement* target, FrameworkElement* nameScope, bool skipSourceName,
        bool skipTargetName, uint8_t priority) final;
    void Seal() override;
    //@}

    ConditionCollection* InternalGetConditions() const; // can return null
    BaseSetterCollection* InternalGetSetters() const; // can return null

    struct Listener;

private:
    mutable Ptr<ConditionCollection> mConditions;
    mutable Ptr<BaseSetterCollection> mSetters;

    struct ListenerHashKeyInfo;
    typedef HashSet<Listener*, 0, HashBucket_K<Listener*, ListenerHashKeyInfo>> Listeners;
    Listeners mListeners;

    NS_DECLARE_REFLECTION(MultiDataTrigger, BaseTrigger)
};

NS_WARNING_POP

}


#endif
