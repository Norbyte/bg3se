////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TRIGGER_H__
#define __GUI_TRIGGER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/String.h>
#include <NsGui/BaseTrigger.h>


namespace Noesis
{

class BaseSetter;
template<class T> class UICollection;
typedef UICollection<BaseSetter> BaseSetterCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a trigger that applies property values or performs actions conditionally.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.trigger.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Trigger: public BaseTrigger
{
public:
    Trigger();
    ~Trigger();

    /// Gets or sets the property that returns the value that is compared with the *Value* property
    /// of the trigger
    //@{
    const DependencyProperty* GetProperty() const;
    void SetProperty(const DependencyProperty* property);
    //@}

    /// Gets a collection of Setter objects, which describe the property values to apply when the
    /// specified condition has been met.
    //@{
    BaseSetterCollection* GetSetters() const;
    //@}

    /// Gets or sets the name of the object with the property that causes the associated setters 
    /// to be applied.
    //@{
    const char* GetSourceName() const;
    void SetSourceName(const char* name);
    //@}

    /// Gets or sets the value to be compared with the property value of the element. The
    /// comparison is a reference equality check.
    //@{
    BaseComponent* GetValue() const;
    void SetValue(BaseComponent* value);
    //@}

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

    BaseSetterCollection* InternalGetSetters() const; // can return null

private:
    bool Matches(FrameworkElement* target, FrameworkElement* nameScope, DependencyObject* object,
        const DependencyProperty* dp, bool skipSourceName, bool skipSelf) const;
    bool Matches(FrameworkElement* target, FrameworkElement* nameScope,
        FrameworkElement* changedObject, const DependencyProperty* changedProperty,
        const void* changedValue, bool skipSourceName) const;

    void EnsureSetters() const;

private:
    String mSourceName;
    const DependencyProperty* mProperty;
    Ptr<BaseComponent> mValue;
    mutable Ptr<BaseSetterCollection> mSetters;

    NS_DECLARE_REFLECTION(Trigger, BaseTrigger)
};

NS_WARNING_POP

}


#endif
