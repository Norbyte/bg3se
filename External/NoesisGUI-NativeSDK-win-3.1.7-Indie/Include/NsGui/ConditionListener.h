////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CONDITIONLISTENER_H__
#define __GUI_CONDITIONLISTENER_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsCore/Delegate.h>
#include <NsCore/Vector.h>
#include <NsCore/String.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BindingListener.h>


namespace Noesis
{

class TypeProperty;
class Type;
class DependencyObject;
class DependencyProperty;
class FrameworkElement;
class CollectionView;
class BaseBinding;
struct PathElement;
struct DependencyPropertyChangedEventArgs;
struct AncestorNameScopeChangedArgs;
struct PropertyChangedEventArgs;
struct NotifyCollectionChangedEventArgs;
struct NotifyDictionaryChangedEventArgs;
struct EventArgs;


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used by DataTriggers and MultiDataTriggers to listen to Binding changes
////////////////////////////////////////////////////////////////////////////////////////////////////
class ConditionListener: public BindingListener
{
public:
    ConditionListener();
    virtual ~ConditionListener() = 0;

    /// Indicates if current binding value matches the trigger condition
    bool Matches() const;

protected:
    virtual BaseComponent* GetValue() const = 0;
    virtual void Invalidate(bool matches) const = 0;

    void OnBindingChanged(BaseComponent* sourceValue, const Type* sourceType) override;

    void UpdateValue(BaseComponent* sourceValue, const Type* sourceType);
    void Invalidate(BaseComponent* sourceValue);

private:
    const Type* mSourceType;
    Ptr<BaseComponent> mValue;
    bool mMatches;
};

}


#endif
