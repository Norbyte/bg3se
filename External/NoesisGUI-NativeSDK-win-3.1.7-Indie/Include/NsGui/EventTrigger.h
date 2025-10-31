////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EVENTTRIGGER_H__
#define __GUI_EVENTTRIGGER_H__


#include <NsCore/Noesis.h>
#include <NsCore/HashMap.h>
#include <NsCore/String.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/BaseTrigger.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

class RoutedEvent;
class FrameworkElement;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a trigger that applies a set of actions in response to an event.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.eventtrigger.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API EventTrigger: public BaseTrigger
{
public:
    EventTrigger();
    ~EventTrigger();
    
    /// Gets the collection of BeginStoryboard objects that this EventTrigger maintains
    TriggerActionCollection* GetActions() const;

    /// Gets or sets the name of the event that initiates the trigger
    //@{
    const RoutedEvent* GetRoutedEvent() const;
    void SetRoutedEvent(const RoutedEvent* event);
    //@}

    /// Gets or sets the name of the object with the event that activates this trigger. This is 
    /// only used by element triggers or template triggers. 
    //@{
    const char* GetSourceName() const;
    void SetSourceName(const char* name);
    //@}

    /// From BaseTrigger
    //@{
    void RegisterEvents(FrameworkElement* target, FrameworkElement* nameScope,
        bool skipSourceName) final;
    void UnregisterEvents(FrameworkElement* target, bool skipSourceName) final;
    void Seal() override;
    //@}

    struct Listener;

private:
    String mSourceName;
    const RoutedEvent* mRoutedEvent;
    mutable Ptr<TriggerActionCollection> mActions;

    struct ListenerHashKeyInfo;
    typedef HashSet<Listener*, 0, HashBucket_K<Listener*, ListenerHashKeyInfo>> Listeners;
    Listeners mListeners;

    NS_DECLARE_REFLECTION(EventTrigger, BaseTrigger)
};

NS_WARNING_POP

}

#endif
