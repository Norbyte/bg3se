////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASEBINDING_H__
#define __GUI_BASEBINDING_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/String.h>
#include <NsCore/Ptr.h>
#include <NsGui/CoreApi.h>
#include <NsGui/MarkupExtension.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines the common features for Binding.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.data.bindingbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseBinding: public MarkupExtension
{
public:
    BaseBinding();
    BaseBinding(const BaseBinding&) = delete;
    BaseBinding& operator=(const BaseBinding&) = delete;
    virtual ~BaseBinding() = 0;

    /// Gets or sets the amount of time, in milliseconds, to wait before updating the binding
    /// source after the value on the target changes
    //@{
    int GetDelay() const;
    void SetDelay(int delay);
    //@}

    /// Gets or sets the value to use when the binding is unable to return a value
    //@{
    BaseComponent* GetFallbackValue() const;
    void SetFallbackValue(BaseComponent* value);
    //@}

    /// Gets or sets the value to use when final target value is null
    //@{
    BaseComponent* GetTargetNullValue() const;
    void SetTargetNullValue(BaseComponent* value);
    //@}

    /// Gets or sets a string that specifies how to format the binding if it displays the bound 
    /// value as a string.
    ///
    /// Examples:
    ///
    /// .. code-block:: xml
    ///
    ///     <TextBlock Text="{Binding Amount, StringFormat=F2}" />
    ///     <TextBlock Text="{Binding Amount, StringFormat={}{0:F2}}" />
    ///     <TextBlock Text="{Binding Amount, StringFormat=Value is {0:F2} units}" />
    ///
    //@{
    const char* GetStringFormat() const;
    void SetStringFormat(const char* format);
    //@}

protected:
    bool CheckSealed() const;

protected:
    // Indicates if this binding is unmodifiable. The binding becomes sealed the first time a
    // binding expression is generated
    bool mSealed;

    // Indicates if this binding is part of a style or template trigger so it should solve element
    // names inside the template first
    bool mSolveNamesInTemplate;
    friend class DataTrigger;
    friend class MultiDataTrigger;
    friend class MultiBinding;

private:
    int mDelay;
    Ptr<BaseComponent> mFallbackValue;
    Ptr<BaseComponent> mTargetNullValue;
    String mStringFormat;

    NS_DECLARE_REFLECTION(BaseBinding, MarkupExtension)
};

NS_WARNING_POP

}


#endif
