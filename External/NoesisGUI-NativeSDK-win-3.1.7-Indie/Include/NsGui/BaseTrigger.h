////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASETRIGGER_H__
#define __GUI_BASETRIGGER_H__


#include <NsCore/Noesis.h>
#include <NsGui/DependencyObject.h>
#include <NsGui/IUITreeNode.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

class FrameworkElement;
class BaseSetter;
class BaseTrigger;
class TriggerAction;

template<class T> class UICollection;
typedef UICollection<BaseSetter> BaseSetterCollection;
typedef UICollection<BaseTrigger> TriggerCollection;
typedef UICollection<TriggerAction> TriggerActionCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the base class for specifying a conditional value within a Style object.
///
/// The styling and templating model allows you to specify triggers within your Style,
/// ControlTemplate, and DataTemplate. Essentially, TriggerBase is the base class for
/// objects that allow you to apply changes when certain conditions (such as when a certain
/// property value becomes true, or when an event occurs) are satisfied.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.triggerbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseTrigger: public DependencyObject, public IUITreeNode
{
public:
    BaseTrigger();
    BaseTrigger(const BaseTrigger&) = delete;
    BaseTrigger& operator=(const BaseTrigger&) = delete;
    virtual ~BaseTrigger() = 0;

    static void RegisterTriggers(TriggerCollection* triggers, FrameworkElement* element,
        FrameworkElement* nameScope, bool skipSourceName, bool skipTargetName, uint8_t priority);
    static void UnregisterTriggers(TriggerCollection* triggers, FrameworkElement* element,
        bool skipSourceName);
    static void SealTriggers(TriggerCollection* triggers);

    /// Gets a collection of TriggerAction objects to apply when the trigger object becomes active.
    /// This property does not apply to the EventTrigger class
    TriggerActionCollection* GetEnterActions() const;

    /// Gets a collection of TriggerAction objects to apply when the trigger object becomes
    /// inactive. This property does not apply to the EventTrigger class
    TriggerActionCollection* GetExitActions() const;

    // Invoke enter actions
    void FireEnterActions(FrameworkElement* target, FrameworkElement* nameScope);

    // Invoke exit actions
    void FireExitActions(FrameworkElement* target, FrameworkElement* nameScope);

    // Register against binding changes
    virtual void RegisterBindings(FrameworkElement* target, FrameworkElement* nameScope,
        bool skipTargetName, uint8_t priority);

    // Unregister from binding changes
    virtual void UnregisterBindings(FrameworkElement* target);

    // Registers against events
    virtual void RegisterEvents(FrameworkElement* target, FrameworkElement* nameScope,
        bool skipSourceName);

    // Unregisters from events
    virtual void UnregisterEvents(FrameworkElement* target, bool skipSourceName);

    // Tests if this trigger will be active for the specified object and looks for a setter value
    // that can be applied to the specified object and property
    // Returns UnsetValue if trigger can't be applied or a valid setter was not found
    virtual BaseComponent* FindValue(FrameworkElement* target, FrameworkElement* nameScope,
        DependencyObject* object, const DependencyProperty* dp, bool skipSourceName,
        bool skipTargetName);

    // Applies setters if trigger conditions are fulfilled
    virtual void Invalidate(FrameworkElement* target, FrameworkElement* nameScope,
        bool skipSourceName, bool skipTargetName, uint8_t priority);

    // Applies setters if trigger changes its state after the specified property change
    virtual void Invalidate(FrameworkElement* target, FrameworkElement* nameScope,
        FrameworkElement* changedObject, const DependencyProperty* changedProperty,
        const void* oldValue, const void* newValue, bool skipSourceName, bool skipTargetName,
        uint8_t priority);

    // Seals this trigger
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
    static BaseComponent* FindValueInSetters(BaseSetterCollection* setters,
        FrameworkElement* target, FrameworkElement* nameScope, DependencyObject* object,
        const DependencyProperty* dp, bool skipTargetName);
    static void InvalidateSetters(BaseSetterCollection* setters, FrameworkElement* target,
        FrameworkElement* nameScope, bool skipTargetName, uint8_t priority);

    bool HasEnterActions() const;
    bool HasExitActions() const;

    /// From DependencyObject
    //@{
    void OnObjectValueSet(BaseComponent* oldValue, BaseComponent* newValue) override;
    //@}

protected:
    bool mIsSealed;

private:
    void EnsureEnterActions() const;
    void EnsureExitActions() const;

private:
    IUITreeNode* mOwner;

    mutable Ptr<TriggerActionCollection> mEnterActions;
    mutable Ptr<TriggerActionCollection> mExitActions;

    NS_DECLARE_REFLECTION(BaseTrigger, DependencyObject)
};

NS_WARNING_POP

}

#endif
