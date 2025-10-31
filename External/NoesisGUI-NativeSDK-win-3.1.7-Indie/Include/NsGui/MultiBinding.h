////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MULTIBINDING_H__
#define __GUI_MULTIBINDING_H__


#include <NsCore/Noesis.h>
#include <NsCore/Ptr.h>
#include <NsCore/Vector.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsGui/BaseBinding.h>
#include <NsGui/CoreApi.h>
#include <NsGui/UpdateSourceTrigger.h>
#include <NsGui/UICollection.h>


namespace Noesis
{

class DependencyObject;
class DependencyProperty;
class PropertyPath;
class RelativeSource;
class FrameworkElement;
struct NotifyCollectionChangedEventArgs;
NS_INTERFACE IMultiValueConverter;
enum BindingMode: int32_t;

typedef UICollection<BaseBinding> BindingCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes a collection of Binding objects attached to a single binding target property.
///
/// Example:
///
/// .. code-block:: xml
///
///     <Border>
///       <Border.Width>
///         <MultiBinding Converter="{StaticResource AddConverter}">
///           <Binding ElementName="source1" Path="Width"/>
///           <Binding ElementName="source2" Path="Width"/>
///         </MultiBinding>
///       </Border.Width>
///     </Border>
///     <TextBlock>
///       <TextBlock.Text>
///         <MultiBinding StringFormat="{}{0} x {1}">
///           <Binding Path="Width" />
///           <Binding Path="Height" />
///         </MultiBinding>
///       </TextBlock.Text>
///     </TextBlock>
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.data.multibinding
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API MultiBinding: public BaseBinding
{
public:
    MultiBinding();
    ~MultiBinding();

    /// Gets the collection of Binding objects within this MultiBinding instance
    BindingCollection* GetBindings() const;

    /// Gets or sets the converter to use to convert the source values to or from the target value
    //@{
    IMultiValueConverter* GetConverter() const;
    void SetConverter(IMultiValueConverter* converter);
    //@}

    /// Gets or sets the parameter to pass to the Converter
    //@{
    BaseComponent* GetConverterParameter() const;
    void SetConverterParameter(BaseComponent* parameter);
    //@}

    /// Gets or sets the binding mode. It can be set to one of the following values of the
    /// *BindingMode* enumeration:
    ///
    /// * *OneWay*: The target is updated whenever the source changes.
    ///
    /// * *TwoWay*: A change to either the target or source updates the other.
    ///
    /// * *OneWayToSource*: The opposite of *OneWay*. The source is updated whenever the target 
    /// changes.
    ///
    /// * *OneTime*: This works just like *OneWay*, except changes to the source are not 
    /// reflected at the target. The target retains a snapshot of the source at the time the 
    /// *Binding* is initiated.
    //@{
    BindingMode GetMode() const;
    void SetMode(BindingMode mode);
    //@}

    /// Gets or sets a value that determines the timing of binding source updates:
    ///
    /// * *Default*: The default UpdateSourceTrigger value of the binding target property. The 
    /// default value for most dependency properties is *PropertyChanged*, while the *Text* 
    /// property has a default value of *LostFocus*.
    ///
    /// * *PropertyChanged*: The source is updated whenever the target property value changes.
    /// 
    /// * *LostFocus*: When the target property value changes, the source is only updated after 
    /// the target element loses focus.
    /// 
    /// * *Explicit*: The source is only updated when you make an explicit call to 
    /// *BindingExpression.UpdateSource*.
    //@{
    UpdateSourceTrigger GetUpdateSourceTrigger() const;
    void SetUpdateSourceTrigger(UpdateSourceTrigger value);
    //@}

    /// From MarkupExtension
    //@{
    /// Can return 0 when the source object is not resolvable (doesn't throw exception)
    Ptr<BaseComponent> ProvideValue(const ValueTargetProvider* provider) override;
    //@}

private:
    void OnBindingsChanged(BaseComponent* sender, const NotifyCollectionChangedEventArgs& e);

private:
    BindingCollection mBindings;
    Ptr<IMultiValueConverter> mConverter;
    Ptr<BaseComponent> mConverterParameter;
    BindingMode mMode;
    UpdateSourceTrigger mUpdateSourceTrigger;

    NS_DECLARE_REFLECTION(MultiBinding, BaseBinding)
};

NS_WARNING_POP

}


#endif
