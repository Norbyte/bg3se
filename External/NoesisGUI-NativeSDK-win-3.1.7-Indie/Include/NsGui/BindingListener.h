////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BINDINGLISTENER_H__
#define __GUI_BINDINGLISTENER_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

class Type;
class BaseComponent;
class DependencyObject;
class BaseBinding;
class PropertyListener;
template<class T> class Ptr;
enum BindingMode: int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used by DataTriggers, MultiDataTriggers and MultiBindings to listen to Binding changes
////////////////////////////////////////////////////////////////////////////////////////////////////
class BindingListener
{
public:
    static void Shutdown();

    BindingListener();
    virtual ~BindingListener() = 0;

    /// Registers binding in the target
    void Register(BindingMode defaultMode);

    /// Removes binding from the target
    void Unregister();

    /// Gets source value
    Ptr<BaseComponent> GetSourceValue() const;

    /// Updates source value
    bool UpdateSource(BaseComponent* value) const;

protected:
    virtual DependencyObject* GetTarget() const = 0;
    virtual BaseBinding* GetBinding() const = 0;

    virtual void OnBindingChanged(BaseComponent* sourceValue, const Type* sourceType);

private:
    PropertyListener* mListener;
};
}


#endif
