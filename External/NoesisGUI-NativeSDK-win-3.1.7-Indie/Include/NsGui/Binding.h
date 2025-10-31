////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BINDING_H__
#define __GUI_BINDING_H__


#include <NsCore/Noesis.h>
#include <NsCore/Ptr.h>
#include <NsCore/Symbol.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsGui/BaseBinding.h>
#include <NsGui/CoreApi.h>
#include <NsGui/UpdateSourceTrigger.h>
#include <NsGui/IUITreeNode.h>


namespace Noesis
{

class DependencyObject;
class DependencyProperty;
class PropertyPath;
class RelativeSource;
class FrameworkElement;
NS_INTERFACE IValueConverter;
enum BindingMode: int32_t;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides high-level access to the definition of a binding, which connects the properties
/// of binding target objects and any data source.
///
/// Example:
///
/// .. code-block:: xml
///
///     <TextBlock Text="{Binding Source={StaticResource myDataSource}, Path=PersonName}"/>
///
/// http://msdn.microsoft.com/en-us/library/system.windows.data.binding.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Binding: public BaseBinding
{
public:
    /// Default constructor
    Binding();

    /// Constructor with an initial property path
    Binding(const char* path);
    Binding(const DependencyProperty* path);

    /// Constructor with an initial property path and a source
    Binding(const char* path, BaseComponent* source);
    Binding(const DependencyProperty* path, BaseComponent* source);

    /// Constructor with an initial property path and an element name
    Binding(const char* path, const char* elementName);
    Binding(const DependencyProperty* path, const char* elementName);

    /// Constructor with an initial property path and a relative source
    Binding(const char* path, RelativeSource* relativeSource);
    Binding(const DependencyProperty* path, RelativeSource* relativeSource);

    /// Destructor
    ~Binding();

    /// Gets or sets the name of the element to use as the binding source object.
    /// Only one of the three properties, *ElementName*, *Source*, or *RelativeSource*, should be 
    /// set for each binding, or a conflict can occur. Setting this property shows an error if
    /// there is a binding source conflict
    //@{
    const char* GetElementName() const;
    void SetElementName(const char* elementName);
    //@}

    /// Gets or sets the object to use as the binding source.
    /// Only one of the three properties, *ElementName*, *Source*, or *RelativeSource*, should be 
    /// set for each binding, or a conflict can occur. Setting this property shows an error if
    /// there is a binding source conflict
    //@{
    BaseComponent* GetSource() const;
    void SetSource(BaseComponent* source);
    //@}

    /// Gets or sets the binding source by specifying its location relative to the position of
    /// the binding target.
    /// Only one of the three properties, *ElementName*, *Source*, or *RelativeSource*, should be 
    /// set for each binding, or a conflict can occur. Setting this property shows an error if
    /// there is a binding source conflict
    //@{
    RelativeSource* GetRelativeSource() const;
    void SetRelativeSource(RelativeSource* source);
    //@}

    /// Gets or sets the path to the binding source property
    //@{
    PropertyPath* GetPath() const;
    void SetPath(PropertyPath* path);
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

    /// Gets or sets the converter to use. If you set the Converter and StringFormat properties,
    /// the converter is applied to the data value first, and then the StringFormat is applied
    //@{
    IValueConverter* GetConverter() const;
    void SetConverter(IValueConverter* converter);
    //@}

    /// Gets or sets the parameter to pass to the Converter
    //@{
    BaseComponent* GetConverterParameter() const;
    void SetConverterParameter(BaseComponent* parameter);
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
    
    /// Find data from the appropriate source: Source, ElementName, RelativeSource or DataContext
    ObjectWithNameScope GetSourceObject(DependencyObject* target,
        const DependencyProperty* targetProperty) const;

    /// From MarkupExtension
    //@{
    /// Can return 0 when the source object is not resolvable (doesn't throw exception)
    Ptr<BaseComponent> ProvideValue(const ValueTargetProvider* provider) override;
    //@}

    // Item[] property is used to notify of changes on collections
    static Symbol ItemNotifyName();

    // Placeholder item to keep bindings working when item containers are recycled
    static BaseComponent* DisconnectedItem();

    /// A source property or a converter can return *Binding.DoNothing* to instruct the binding
    /// engine to do nothing when binding gets evaluated
    /// \prop
    static BaseComponent* GetDoNothing();

private:
    void RegisterSource();
    void UnregisterSource();

    void OnSourceDestroyed(DependencyObject* object);

    void SetSourceImpl(BaseComponent* source);

private:
    String mElementName;
    BaseComponent* mSource;
    Ptr<RelativeSource> mRelativeSource;

    Ptr<PropertyPath> mPath;

    BindingMode mMode;

    Ptr<IValueConverter> mConverter;
    Ptr<BaseComponent> mConverterParameter;

    UpdateSourceTrigger mUpdateSourceTrigger;

    NS_DECLARE_REFLECTION(Binding, BaseBinding)
};

NS_WARNING_POP

}


#endif
