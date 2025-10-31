////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_FRAMEWORKELEMENT_H__
#define __GUI_FRAMEWORKELEMENT_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsGui/UIElement.h>
#include <NsGui/IUITreeNode.h>
#include <NsGui/INameScope.h>


namespace Noesis
{

class ContextMenu;
class NameScope;
class FocusVisualLayer;
class ResourceDictionary;
class BaseBinding;
class BaseTrigger;
class Cursor;
class Style;
class BaseBindingExpression;
class BindingExpression;
class FrameworkTemplate;
class DataTemplate;
class TemplateLocalValueProvider;
template<class T> class Delegate;
struct SizeChangedEventArgs;
struct NotifyCollectionChangedEventArgs;
struct NotifyDictionaryChangedEventArgs;
struct Size;
struct Thickness;
NS_INTERFACE ITimeManager;
NS_INTERFACE IView;

template<class T> class UICollection;
typedef UICollection<BaseTrigger> TriggerCollection;

/// Helper macro to easily connect events to code behind functions when overriding ConnectEvent
#define NS_CONNECT_EVENT(type_, event_, handler_) \
    if (Noesis::StrEquals(event, #event_) && Noesis::StrEquals(handler, #handler_)) \
    { \
        ((type_*)source)->event_() += Noesis::MakeDelegate(this, &SelfClass::handler_); \
        return true; \
    }

/// Helper macro to easily connect attached events to code behind functions on ConnectEvent override
#define NS_CONNECT_ATTACHED_EVENT(type_, event_, handler_) \
    if (Noesis::StrEquals(event, #event_) && Noesis::StrEquals(handler, #handler_)) \
    { \
        ((Noesis::UIElement*)source)->AddHandler(type_::event_##Event, \
            Noesis::MakeDelegate(this, &SelfClass::handler_)); \
        return true; \
    }

////////////////////////////////////////////////////////////////////////////////////////////////////
enum PPAAMode
{
    /// Uses PPAA View's setting
    PPAAMode_Default,

    /// Disabled PPAA generation for this element subtree
    PPAAMode_Disabled
};

////////////////////////////////////////////////////////////////////////////////////////////////////
enum AncestorNameScopeChangeAction
{
    AncestorNameScopeChangeAction_Attach,
    AncestorNameScopeChangeAction_Detach,
    AncestorNameScopeChangeAction_Add,
    AncestorNameScopeChangeAction_Remove
};

////////////////////////////////////////////////////////////////////////////////////////////////////
struct AncestorNameScopeChangedArgs
{
    AncestorNameScopeChangeAction action;
    INameScope* nameScope;
    NameScopeChangedArgs changeArgs;
};

typedef Noesis::Delegate<void (FrameworkElement*, const AncestorNameScopeChangedArgs&)>
    AncestorNameScopeChangedDelegate;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for the context menu event. 
////////////////////////////////////////////////////////////////////////////////////////////////////
struct ContextMenuEventArgs: public RoutedEventArgs
{
    /// Gets the object that has the ContextMenu that will be opened
    mutable DependencyObject* targetElement = nullptr;

    /// Gets the horizontal position of the mouse
    float cursorLeft;

    /// Gets the vertical position of the mouse
    float cursorTop;

    ContextMenuEventArgs(BaseComponent* source, const RoutedEvent* event, float cursorLeft = -1.0f,
        float cursorTop = -1.0f);
};

typedef Delegate<void (BaseComponent*, const ContextMenuEventArgs&)> ContextMenuEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides event information for events that occur when a tooltip opens or closes.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct ToolTipEventArgs: public RoutedEventArgs
{
    ToolTipEventArgs(BaseComponent* source, const RoutedEvent* event);
};

typedef Delegate<void (BaseComponent*, const ToolTipEventArgs&)> ToolTipEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for the RequestBringIntoView routed event
////////////////////////////////////////////////////////////////////////////////////////////////////
struct RequestBringIntoViewEventArgs: public RoutedEventArgs
{
    /// Gets the object that should be made visible in response to the event. 
    DependencyObject* targetObject;

    /// Gets the rectangular region in the object's coordinate space which should be made visible. 
    Rect targetRect;

    RequestBringIntoViewEventArgs(BaseComponent* source, DependencyObject* object,
        const Rect& targetRect);
};

typedef Delegate<void (BaseComponent*, const RequestBringIntoViewEventArgs&)> RequestBringIntoViewEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data related to the SizeChanged event
////////////////////////////////////////////////////////////////////////////////////////////////////
struct SizeChangedEventArgs: public RoutedEventArgs
{
    /// Gets the new size of the object
    Size newSize;
    /// Gets the previous size of the object
    Size previousSize;
    /// Gets a value that indicates whether the *Width* component of the Size changed
    bool widthChanged;
    /// Gets a value that indicates whether the *Height* component of the Size changed
    bool heightChanged;

    SizeChangedEventArgs(BaseComponent* source, const RoutedEvent* event, const SizeChangedInfo& info);
};

typedef Delegate<void (BaseComponent*, const SizeChangedEventArgs&)> SizeChangedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef Delegate<void (FrameworkElement*)> AncestorChangedDelegate;
typedef Delegate<void(BaseComponent*, const NotifyDictionaryChangedEventArgs&)>
    AncestorResourcesChangedDelegate;

#ifdef NS_PLATFORM_WINDOWS
 #pragma push_macro("FindResource")
 #ifdef FindResource
  #undef FindResource
 #endif
#endif

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides GUI framework-level features for user interface elements.
///
/// FrameworkElement extends UIElement and adds the following capabilities:
///
/// * Layout system definition
/// * The logical tree
/// * Object lifetime events
/// * Support for data binding and dynamic resource references
/// * Styles
/// * More animation support
///
/// http://msdn.microsoft.com/en-us/library/system.windows.frameworkelement.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API FrameworkElement: public UIElement, public IUITreeNode
{
public:
    FrameworkElement();
    FrameworkElement(const FrameworkElement&) = delete;
    FrameworkElement& operator=(const FrameworkElement&) = delete;
    ~FrameworkElement();

    /// Gets the direction that text and other UI elements flow within any parent element that
    /// controls their layout
    static FlowDirection GetFlowDirection(const DependencyObject* d);

    /// Sets the direction that text and other UI elements flow within any parent element that
    /// controls their layout
    static void SetFlowDirection(DependencyObject* d, FlowDirection flowDirection);

    // Property accessors
    //@{

    /// Gets the rendered height of this element.
    float GetActualHeight() const;

    /// Gets the rendered width of this element.
    float GetActualWidth() const;

    /// Gets or sets a value indicating how the element's contents are mixed with the background
    //@{
    BlendingMode GetBlendingMode() const;
    void SetBlendingMode(BlendingMode mode);
    //@}

    /// Gets or sets the context menu element that should appear whenever the context menu is 
    /// requested through user interface (UI) from within this element.
    //@{
    ContextMenu* GetContextMenu() const;
    void SetContextMenu(ContextMenu* menu);
    //@}

    /// Gets or sets the cursor that displays when the mouse pointer is over this element.
    //@{
    Cursor* GetCursor() const;
    void SetCursor(Cursor* cursor);
    //@}

    /// Gets or sets the data context for an element when it participates in data binding
    //@{
    BaseComponent* GetDataContext() const;
    void SetDataContext(BaseComponent* context);
    //@}

    /// Gets or sets the key to use to reference the style for this control, when theme styles are
    /// used or defined
    //@{
    const Type* GetDefaultStyleKey() const;
    void SetDefaultStyleKey(const Type* key);
    //@}

    /// Gets or sets the direction that text and other UI elements flow within any parent element
    /// that controls their layout
    //@{
    FlowDirection GetFlowDirection() const;
    void SetFlowDirection(FlowDirection flowDirection);
    //@}

    /// Gets or sets a property that enables customization of appearance, effects, or other style 
    /// characteristics that will apply to this element when it captures keyboard focus
    //@{
    Style* GetFocusVisualStyle() const;
    void SetFocusVisualStyle(Style* style);
    //@}

    /// Gets or sets a value that indicates whether this FrameworkElement should force the UI to
    /// render the cursor as declared by this element's Cursor property.
    //@{
    bool GetForceCursor() const;
    void SetForceCursor(bool force);
    //@}

    /// Gets or sets the suggested height of the element.
    //@{
    float GetHeight() const;
    void SetHeight(float height);
    //@}

    /// Gets or sets the horizontal alignment characteristics applied to this element when it is 
    /// composed within a parent element, such as a panel or items control
    //@{
    HorizontalAlignment GetHorizontalAlignment() const;
    void SetHorizontalAlignment(HorizontalAlignment hAlign);
    //@}

    /// Gets or sets the context for input used by this element. The input scope, which modifies
    /// how input from alternative input methods is interpreted
    //@{
    InputScope GetInputScope() const;
    void SetInputScope(InputScope inputScope);
    //@}

    /// Gets or sets a graphics transformation that should apply to this element when layout is 
    /// performed
    //@{
    Transform* GetLayoutTransform() const;
    void SetLayoutTransform(Transform* transform);
    //@}

    /// Gets or sets the outer margin of an element.
    //@{
    const Thickness& GetMargin() const;
    void SetMargin(const Thickness& margin);
    //@}

    /// Gets or sets the maximum height constraint of the element.
    //@{
    float GetMaxHeight() const;
    void SetMaxHeight(float maxHeigth);
    //@}

    /// Gets or sets the maximum width constraint of the element. 
    //@{
    float GetMaxWidth() const;
    void SetMaxWidth(float maxWidth);
    //@}

    /// Gets or sets the minimum height constraint of the element.
    //@{
    float GetMinHeight() const;
    void SetMinHeight(float minHeight);
    //@}

    /// Gets or sets the minimum width constraint of the element.
    //@{
    float GetMinWidth() const;
    void SetMinWidth(float minWidth);
    //@}

    /// Gets or sets the identifying name of the element. The name provides a reference so that 
    /// code-behind, such as event handler code, can refer to a markup element after it is 
    /// constructed during processing by a XAML processor. 
    //@{
    const char* GetName() const;
    void SetName(const char* name);
    //@}

    /// Gets or sets a value that indicates whether this element incorporates style properties from 
    /// theme styles
    //@{
    bool GetOverridesDefaultStyle() const;
    void SetOverridesDefaultStyle(bool value);
    //@}

    /// Gets or sets a value that indicates whether antialiasing geometry is generated for this
    /// element. This property is inherited down by the visual tree
    //@{
    PPAAMode GetPPAAMode() const;
    void SetPPAAMode(PPAAMode mode);
    //@}

    /// Gets or sets the value of the contraction of each vertex along the normal for PPAA. This
    /// property is inherited down by the visual tree
    //@{
    float GetPPAAIn() const;
    void SetPPAAIn(float value);
    //@}

    /// Gets or sets the value of the extrusion of each vertex along the normal for PPAA. This
    /// property is inherited down by the visual tree
    //@{
    float GetPPAAOut() const;
    void SetPPAAOut(float value);
    //@}

    /// Gets or sets the style used by this element when it is rendered
    //@{
    Style* GetStyle() const;
    void SetStyle(Style* style);
    //@}

    /// Gets or sets an arbitrary object value that can be used to store custom information about 
    /// this element
    //@{
    BaseComponent* GetTag() const;
    void SetTag(BaseComponent* tag);
    void SetTag(const char* tag);
    //@}

    /// Gets or sets the tool-tip object that is displayed for this element in the user interface 
    //@{
    BaseComponent* GetToolTip() const;
    void SetToolTip(BaseComponent* tooltip);
    void SetToolTip(const char* tooltip);
    //@}

    /// Gets or sets a value that indicates whether layout rounding should be applied to this
    /// element's size and position during layout
    //@{
    bool GetUseLayoutRounding() const;
    void SetUseLayoutRounding(bool useLayoutRounding);
    //@}

    /// Gets or sets the vertical alignment characteristics applied to this element when it is 
    /// composed within a parent element such as a panel or items control.
    //@{
    VerticalAlignment GetVerticalAlignment() const;
    void SetVerticalAlignment(VerticalAlignment vAlign);
    //@}

    /// Gets or sets the width of the element
    //@{
    float GetWidth() const;
    void SetWidth(float width);
    //@}

    /// Get the trigger collection
    TriggerCollection* GetTriggers() const;

    //@}

    /// Returns a binding expression if the target property has an active binding; otherwise,
    /// returns null
    BindingExpression* GetBindingExpression(const DependencyProperty* dp);

    /// Attaches a binding to this element, based on the provided binding object
    /// \return The binding expression just set
    BaseBindingExpression* SetBinding(const DependencyProperty* dp, BaseBinding* binding);

    /// Attaches a binding to this element, based on the provided source property name as a path
    /// qualification to the data source
    /// \return The binding expression just set
    BindingExpression* SetBinding(const DependencyProperty* dp, const char* path);

    /// Gets a value that indicates whether this element has been loaded for presentation. 
    bool IsLoaded() const;

    /// Attempts to bring this element into view, within any scrollable regions it
    /// is contained within.
    void BringIntoView();

    /// Attempts to bring the provided region size of this element into view, within
    /// any scrollable regions it is contained within.
    void BringIntoView(const Rect& targetRectangle);

    /// Gets the TimeManager that controls current element
    ITimeManager* GetTimeManager() const;

    /// Gets the logical parent element of this element
    FrameworkElement* GetParent() const;

    /// Template management
    //@{

    /// Called on every Measure. Returns true if Visuals were added to the tree
    bool ApplyTemplate();

    /// Gets the logical parent or the templated parent when logical parent is not available
    FrameworkElement* GetParentOrTemplatedParent() const;

    /// Gets or sets the template parent of this element. This property is not relevant if the 
    /// element was not created through a template
    //@{
    FrameworkElement* GetTemplatedParent() const;
    void SetTemplatedParent(FrameworkElement* templatedParent, FrameworkTemplate* frameworkTemplate);
    //@}

    /// Looks for a named element in the applied template
    BaseComponent* GetTemplateChild(const char* name) const;
    template<class T> T* GetTemplateChild(const char* name) const;

    /// Gets the root element of the applied template
    FrameworkElement* GetTemplateRoot() const;

    /// Gets applied template on this element
    FrameworkTemplate* GetFrameworkTemplate() const;

    /// Clears current template
    void ClearFrameworkTemplate();

    /// Returns a clone of this element. Used when applying a template
    Ptr<FrameworkElement> Clone(FrameworkElement* parent, FrameworkElement* templatedParent,
        FrameworkTemplate* template_) const;

    //@}

    /// Gets root for VisualState management
    virtual FrameworkElement* GetStateGroupsRoot() const;

    /// Provides accessors that simplifies access to the NameScope registration methods
    //@{
    BaseComponent* FindName(const char* name) const;
    template<class T> T* FindName(const char* name) const;
    ObjectWithNameScope FindNameAndScope(const char* name) const;
    void RegisterName(const char* name, BaseComponent* object);
    void UnregisterName(const char* name);
    void UpdateName(const char* name, BaseComponent* object);
    INameScope::ChangedDelegate& NameScopeChanged();
    //@}

    /// Finds a resource looking in the logical parent chain
    //@{
    BaseComponent* FindResource(const char* key) const;
    template<class T> T* FindResource(const char* key) const;

  #ifdef NS_PLATFORM_WINDOWS
    BaseComponent* FindResourceW(const char* key) const;
    BaseComponent* FindResourceA(const char* key) const;
    template<class T> T* FindResourceW(const char* key) const;
    template<class T> T* FindResourceA(const char* key) const;
  #endif
    //@}

    /// Gets or sets the locally-defined resource dictionary
    //@{
    ResourceDictionary* GetResources() const;
    void SetResources(ResourceDictionary* resources);
    //@}

    /// Occurs just before any context menu on the element is closed
    UIElement::RoutedEvent_<ContextMenuEventHandler> ContextMenuClosing();
    /// Occurs when any context menu on the element is opened
    UIElement::RoutedEvent_<ContextMenuEventHandler> ContextMenuOpening();
    /// Occurs when the element is laid out, rendered, and ready for interaction
    UIElement::RoutedEvent_<RoutedEventHandler> Loaded();
    /// Occurs when this element xaml gets reloaded as a result of a hot-reload operation
    UIElement::RoutedEvent_<RoutedEventHandler> Reloaded();
    /// Occurs when BringIntoView is called on this element
    UIElement::RoutedEvent_<RequestBringIntoViewEventHandler> RequestBringIntoView();
    /// Occurs when either ActualHeight or ActualWidth properties change value on this element
    UIElement::RoutedEvent_<SizeChangedEventHandler> SizeChanged();
    /// Occurs just before any tooltip on the element is closed
    UIElement::RoutedEvent_<ToolTipEventHandler> ToolTipClosing();
    /// Occurs when any tooltip on the element is opened
    UIElement::RoutedEvent_<ToolTipEventHandler> ToolTipOpening();
    /// Occurs when the element is removed from within an element tree of loaded elements
    UIElement::RoutedEvent_<RoutedEventHandler> Unloaded();

    /// Occurs when the data context for this element changes
    UIElement::Event_<DependencyPropertyChangedEventHandler> DataContextChanged();
    /// Occurs when this element is initialized
    UIElement::Event_<EventHandler> Initialized();

    /// Notifies when an ancestor has changed its parent
    AncestorChangedDelegate& AncestorChanged();
    /// Notifies when a change occurred in the resources dictionary of this element or an ancestor
    AncestorResourcesChangedDelegate& AncestorResourcesChanged();
    /// Notifies when a NameScope in an ancestor has changed any of its items
    AncestorNameScopeChangedDelegate& AncestorNameScopeChanged();

    /// Attaches specified event to code-behind content
    virtual bool ConnectEvent(BaseComponent* source, const char* event, const char* handler);

    /// From UIElement
    //@{
    bool MoveFocus(const TraversalRequest& request) override;
    DependencyObject* PredictFocus(FocusNavigationDirection direction) override;
    //@}

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const override;
    void SetNodeParent(IUITreeNode* parent) override;
    BaseComponent* FindNodeResource(const char* key, bool fullElementSearch) const override;
    BaseComponent* FindNodeName(const char* name) const override;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const override;
    //@}}

    NS_IMPLEMENT_INTERFACE_FIXUP

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ActualHeightProperty;
    static const DependencyProperty* ActualWidthProperty;
    static const DependencyProperty* BlendingModeProperty;
    static const DependencyProperty* ContextMenuProperty;
    static const DependencyProperty* CursorProperty;
    static const DependencyProperty* DataContextProperty;
    static const DependencyProperty* DefaultStyleKeyProperty;
    static const DependencyProperty* FlowDirectionProperty;
    static const DependencyProperty* FocusVisualStyleProperty;
    static const DependencyProperty* ForceCursorProperty;
    static const DependencyProperty* HeightProperty;
    static const DependencyProperty* HorizontalAlignmentProperty;
    static const DependencyProperty* InputScopeProperty;
    static const DependencyProperty* LayoutTransformProperty;
    static const DependencyProperty* MarginProperty;
    static const DependencyProperty* MaxHeightProperty;
    static const DependencyProperty* MaxWidthProperty;
    static const DependencyProperty* MinHeightProperty;
    static const DependencyProperty* MinWidthProperty;
    static const DependencyProperty* NameProperty;
    static const DependencyProperty* OverridesDefaultStyleProperty;
    static const DependencyProperty* PPAAModeProperty;
    static const DependencyProperty* PPAAInProperty;
    static const DependencyProperty* PPAAOutProperty;
    static const DependencyProperty* StyleProperty;
    static const DependencyProperty* TagProperty;
    static const DependencyProperty* ToolTipProperty;
    static const DependencyProperty* UseLayoutRoundingProperty;
    static const DependencyProperty* VerticalAlignmentProperty;
    static const DependencyProperty* WidthProperty;
    //@}

    /// Dependency events
    //@{
    static const RoutedEvent* ContextMenuClosingEvent;
    static const RoutedEvent* ContextMenuOpeningEvent;
    static const RoutedEvent* LoadedEvent;
    static const RoutedEvent* ReloadedEvent;
    static const RoutedEvent* RequestBringIntoViewEvent;
    static const RoutedEvent* SizeChangedEvent;
    static const RoutedEvent* ToolTipClosingEvent;
    static const RoutedEvent* ToolTipOpeningEvent;
    static const RoutedEvent* UnloadedEvent;
    //@}

public:
    /// Invoked whenever an unhandled ContextMenuClosing routed event reaches this class in its
    /// route. Implement this method to add class handling for this event
    virtual void OnContextMenuClosing(const ContextMenuEventArgs& e);

    /// Invoked whenever an unhandled ContextMenuOpening routed event reaches this class in its
    /// route. Implement this method to add class handling for this event
    virtual void OnContextMenuOpening(const ContextMenuEventArgs& e);

    /// Invoked when the context menu changes
    virtual void OnContextMenuChanged(ContextMenu* oldMenu, ContextMenu* newMenu);

    /// Invoked when the style in use on this element changes, which will invalidate
    /// the layout.
    virtual void OnStyleChanged(Style* oldStyle, Style* newStyle);

    /// Invoked when focus style changes
    virtual void OnFocusStyleChanged(Style* oldStyle, Style* newStyle);

    /// Invoked whenever an unhandled ToolTipClosing routed event reaches this class in its route. 
    /// Implement this method to add class handling for this event.
    virtual void OnToolTipClosing(const ToolTipEventArgs& e);

    /// Invoked whenever the ToolTipOpening routed event reaches this class in its route. Implement 
    /// this method to add class handling for this event.
    virtual void OnToolTipOpening(const ToolTipEventArgs& e);

    /// Performs custom clone work for a derived class
    virtual void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const;

    /// Adds the provided object to the logical tree of this element
    void AddLogicalChild(BaseComponent* child);

    /// Removes the provided object from this element's logical tree. FrameworkElement updates the
    /// affected logical tree parent pointers to keep in sync with this deletion.
    void RemoveLogicalChild(BaseComponent* child);

    /// Gets the number of child logical elements for this element
    /// \remarks Each element implementation will decide how to store logical children
    virtual uint32_t GetLogicalChildrenCount() const;

    /// Returns the specified element in the parent Collection
    /// \remarks Each element implementation will decide how to store logical children
    virtual Ptr<BaseComponent> GetLogicalChild(uint32_t index) const;

    /// Indicates that logical parent has changed
    virtual void OnLogicalParentChanged(FrameworkElement* oldParent);

    /// Allows derived classes to use this visual as the default visual child
    //@{
    Visual* GetSingleVisualChild() const;
    void SetSingleVisualChild(Visual* child);
    //@}

    // Property value changed events
    //@{
    virtual void OnWidthChanged(float width);
    virtual void OnHeightChanged(float height);
    //@}

    /// Called by ApplyTemplate before it does work to generate the template tree.
    virtual void OnPreApplyTemplate();

    /// Override if you wish to get notified that the template tree has been created. Called after
    /// the template tree has been generated and it is invoked only if the call to ApplyTemplate
    /// actually caused the template tree to be generated
    virtual void OnApplyTemplate();

    /// Called by ApplyTemplate after it generates the template tree
    virtual void OnPostApplyTemplate();

    /// Sets a template on this element
    void SetFrameworkTemplate(FrameworkTemplate* frameworkTemplate);

    /// Looks for a DataTemplate in the UI tree that matches the specified item type
    static DataTemplate* FindTemplate(FrameworkElement* element, BaseComponent* item);

    /// Indicates if the property specified is set as a local value in the template
    bool IsTemplateLocalValue(const DependencyProperty* dp) const;

    /// Invoked when framework template changes
    virtual void OnTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkElement* oldRoot,
        FrameworkTemplate* newTemplate, FrameworkElement* newRoot);

    /// Invoked when templated parent changes
    virtual void OnTemplatedParentChanged(FrameworkElement* oldParent, FrameworkElement* newParent);

    /// When overridden in a derived class, measures the size in layout required for child elements
    /// and determines a size for the FrameworkElement-derived class
    /// \return The size that this element determines it needs during layout, based on its
    /// calculations of child element sizes
    ///
    /// \note Override MeasureOverride to implement custom layout sizing behavior for your element
    /// as it participates in the layout system. Your implementation should do the following:
    ///
    /// 1. Iterate your element's particular collection of children that are part of layout,
    ///    call Measure on each child element.
    /// 2. Immediately get DesiredSize on the child (this is set as a property after Measure
    ///    is called).
    /// 3. Compute the net desired size of the parent based upon the measurement of the
    /// child elements.
    ///
    /// The return value of MeasureOverride should be the element's own desired size, which
    /// then becomes the measure input for the parent element of the current element. This
    /// same process continues through the layout system until the root element is reached
    ///
    /// During this process, child elements might return a larger DesiredSize size than the
    /// initial availableSize to indicate that the child element wants more space. This might
    /// be handled in your own implementation by introducing a scrollable region, by resizing
    /// the parent control, by establishing some manner of stacked order, or any number of
    /// solutions for measuring or arranging content
    virtual Size MeasureOverride(const Size& availableSize);

    /// When overridden in a derived class, positions child elements and determines a size for
    /// a FrameworkElement derived class
    /// \param finalSize The final area within the parent that this element should use to arrange
    /// itself and its children
    /// \return The actual size used
    ///
    /// \note The implementation pattern should call Arrange on each visible child element, and
    /// pass the final desired size for each child element as the finalRect parameter. Parent
    /// elements should call Arrange on each child, otherwise the child elements will not be
    /// rendered
    virtual Size ArrangeOverride(const Size& finalSize);

    /// Calculates min and max limits depending on width, height and limit values
    void GetMinMax(float width, float height,
        float& minW, float& minH, float& maxW, float& maxH) const;

    /// Gets desired size before it gets clipped by size constraints and margins
    const Size& GetUnclippedSize() const;

    /// From BaseRefCounted
    //@{
    int32_t OnDestroy() override;
    //@}

    /// From DependencyObject
    //@{
    void OnInit() override;
    void OnPostInit() override;
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    bool OnSubPropertyChanged(const DependencyProperty* prop) override;
    void OnObjectValueSet(BaseComponent* oldValue, BaseComponent* newValue) override;
    void OnUncachedPropertySet(const DependencyProperty* dp) final override;
    ProviderValue GetProviderValue(const DependencyProperty* dp) const override;
    ProviderValue GetNonCachedProviderValue(const DependencyProperty* dp) const override;
    uint8_t GetNonCachedValueProvider(const DependencyProperty* dp) const override;
    //@}

    /// From Visual
    //@{
    uint32_t GetVisualChildrenCount() const override;
    Visual* GetVisualChild(uint32_t index) const override;
    void OnVisualParentChanged(Visual* oldParent) override;
    void OnConnectToView(IView* view) override;
    void OnConnectToViewChildren() override;
    void OnDisconnectFromView() override;
    DrawingCommands* GetDrawingCommands() const override;
    //@}

    /// From UIElement
    //@{
    Size MeasureCore(const Size& availableSize) override;
    void ArrangeCore(const Rect& finalRect) override;
    Ptr<Geometry> GetLayoutClip(const Size& layoutSlotSize) const override;
    UIElement* GetUIParentCore() const override;
    void OnGotFocus(const RoutedEventArgs& e) override;
    void OnRenderSizeChanged(const SizeChangedInfo& sizeInfo) override;
    //@}

public:
    bool IsStyleTriggerProviderEnabled() const;
    void StyleTriggerProviderEnable();
    void StyleTriggerProviderDisable();
    void ImplicitStyleProviderEnable();
    void ImplicitStyleProviderDisable();
    void DefaultStyleProviderEnable();
    void DefaultStyleProviderDisable();

    TemplateLocalValueProvider* GetTemplateLocalValues() const;

    Style* GetDefaultStyle() const;

    BaseComponent* FindResourceImpl(const char* key, FrameworkElement* boundary,
        bool themeSearch = true) const;
    BaseComponent* FindResourceInElement(const char* key, bool fullElementSearch) const;

    void OnFrameworkTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkTemplate* newTemplate);

    void RemoveTemplatedParent();

    FrameworkElement* FindInheritanceParent() const;
    bool IsSelfInheritanceParent() const;
    void SetInheritanceParentProps(FrameworkElement* inheritanceParent);
    void ConnectInheritanceParent();
    void ClearInheritanceParentProps();
    void DisconnectInheritanceParent();
    void UpdateInheritanceParentOnChildren(FrameworkElement* oldInheritanceParent,
        FrameworkElement* newInheritanceParent);
    void UpdateInheritanceParent(FrameworkElement* inheritanceParent);
    void UpdateInheritedProps(bool connect);
    void UpdateInheritedProp(const DependencyProperty* prop, bool isSubPropertyChange,
        const void* oldValue, const void* newValue);

    friend class View;
    void OnResourcesChanged(BaseComponent* sender,
        const NotifyDictionaryChangedEventArgs& args);
    void OnAncestorResourcesChanged(BaseComponent* sender,
        const NotifyDictionaryChangedEventArgs& args);
    void OnNameScopeChanged(INameScope* sender, const NameScopeChangedArgs& args);
    void OnAncestorNameScopeChanged(FrameworkElement* element,
        const AncestorNameScopeChangedArgs& args);

    INameScope* GetNameScope() const;
    void SetParent(FrameworkElement* parent);
    void CheckPropertyMetadata(const DependencyProperty* prop, const PropertyMetadata* metadata,
        const void* oldValue, const void* newValue, bool isSubPropertyChange);
    Point CalculateArrangeOffset(const Size& clientSize,
        const Size& renderSize, HorizontalAlignment hAlign,
        VerticalAlignment vAlign) const;

    static bool ShouldApplyMirrorTransform(FrameworkElement* element);

    /// Loaded/Unloaded events
    //@{
    friend class ViewLayout;
    void RaiseLoaded();
    void OnLoaded();
    void OnUnloaded();
    //@}

    /// Management of framework element flags
    //@{
    void SetElementFlag(uint32_t flag);
    void ClearElementFlag(uint32_t flag);
    bool CheckElementFlag(uint32_t flag) const;
    //@}

    void CloneRoutedEvents(FrameworkElement* clone) const;
    void CloneEvents(FrameworkElement* clone) const;
    void CloneResources(FrameworkElement* clone) const;
    void CloneInputBindings(FrameworkElement* clone) const;
    void CloneCommandBindings(FrameworkElement* clone) const;
    void CloneTriggers(FrameworkElement* clone) const;

    void AddClonedResources(const char* key, BaseComponent* value);

    void TransferAnimations(IView* view);

    struct TimeManagerRef;
    Ptr<TimeManagerRef> EnsureTimeManager() const;

    void EnsureResources();
    void ConnectResources();
    void DisconnectResources();

    void EnsureTriggers();
    void RegisterTriggers();
    void UnregisterTriggers();
    void RegisterTrigger(BaseComponent* trigger);
    void UnregisterTrigger(BaseComponent* trigger);
    void OnTriggersChanged(BaseComponent* sender, const NotifyCollectionChangedEventArgs& e);

    bool CancelLoadedRequest();

    EventHandler& GetDataContextChangedEvent();
    EventHandler& GetInitializedEvent();

public:
    friend class UIElementCollection;
    friend class ItemCollection;
    friend struct LogicalTreeHelper;
    friend class ContextMenu;
    friend struct ContextMenuService;
    friend class ToolTip;
    friend struct ToolTipService;
    friend class UserControl;
    friend class ContentControl;
    friend class Inline;
    friend class FocusVisualLayer;
    friend class DynamicResourceExpression;
    friend struct TemplatedParentTriggerProvider;
    friend class TemplateLocalValueProvider;
    friend struct ImplicitStyleProvider;
    friend struct BaseStyleTriggerProvider;
    friend struct StyleTriggerProvider;
    friend struct DefaultStyleSetterProvider;
    friend struct TemplateTriggerProvider;
    friend struct InheritedPropertyProvider;
    friend class FrameworkElementTest;
    friend class StoryboardTest;

    // Parent node in the XAML tree
    IUITreeNode* mOwner;

    // Logical parent of this element
    FrameworkElement* mParent;

    // Control that contains this element as part of its template visual tree
    FrameworkElement* mTemplatedParent;

    // Element storing inherited properties
    FrameworkElement* mInheritanceParent;

    // Prevents triggers to enter into an infinite recursion when invalidating
    Vector<const DependencyProperty*> mInvalidatedProperties;

    // Element resources dictionary
    Ptr<ResourceDictionary> mResources;

    // Trigger collection
    Ptr<TriggerCollection> mTriggers;

    // Default storage for a single visual child
    Ptr<Visual> mVisualChild;

    // Desired size without applying clipping
    Size mUnclippedSize;

    // FrameworkElement flags
    int32_t mElementFlags;

    // Element request to raise Loaded event when layout finalizes
    void* mLoadedRequest;

    // Delegates
    //@{
    AncestorChangedDelegate mAncestorChanged;
    AncestorResourcesChangedDelegate mAncestorResourcesChanged;
    AncestorNameScopeChangedDelegate mAncestorNameScopeChanged;
    //@}

    NS_DECLARE_REFLECTION(FrameworkElement, UIElement)
};

NS_WARNING_POP

}

NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CORE_API, Noesis::PPAAMode)

#include <NsGui/FrameworkElement.inl>

#ifdef NS_PLATFORM_WINDOWS
 #pragma pop_macro("FindResource")
#endif

#endif
