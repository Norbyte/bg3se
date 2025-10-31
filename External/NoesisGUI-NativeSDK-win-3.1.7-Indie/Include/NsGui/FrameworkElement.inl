////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsGui/ContextMenuService.h>
#include <NsGui/SizeChangedInfo.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline ContextMenuEventArgs::ContextMenuEventArgs(BaseComponent* s, const RoutedEvent* e,
    float left, float top) : RoutedEventArgs(s, e), cursorLeft(left), cursorTop(top)
{
    NS_ASSERT(e == ContextMenuService::ContextMenuOpeningEvent ||
        e == ContextMenuService::ContextMenuClosingEvent);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline ToolTipEventArgs::ToolTipEventArgs(BaseComponent* s, const RoutedEvent* e):
    RoutedEventArgs(s, e) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline RequestBringIntoViewEventArgs::RequestBringIntoViewEventArgs(BaseComponent* s,
    DependencyObject* object, const Rect& rect): RoutedEventArgs(s,
    FrameworkElement::RequestBringIntoViewEvent), targetObject(object), targetRect(rect) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline SizeChangedEventArgs::SizeChangedEventArgs(BaseComponent* s, const RoutedEvent* e,
    const SizeChangedInfo& info): RoutedEventArgs(s, e), newSize(info.newSize),
    previousSize(info.previousSize), widthChanged(info.widthChanged),
    heightChanged(info.heightChanged) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* FrameworkElement::GetTemplateChild(const char* name) const
{
    BaseComponent* child = GetTemplateChild(name);
    NS_CHECK(child == 0 || DynamicCast<T*>(child) != 0, "Invalid cast");
    return static_cast<T*>(child);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* FrameworkElement::FindName(const char* name) const
{
    BaseComponent* resource = FindName(name);
    NS_CHECK(resource == 0 || DynamicCast<T*>(resource) != 0, "Invalid cast");
    return static_cast<T*>(resource);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* FrameworkElement::FindResource(const char* key) const
{
    BaseComponent* resource = FindResource(key);
    NS_CHECK(resource == 0 || DynamicCast<T*>(resource) != 0, "Invalid cast");
    return static_cast<T*>(resource);
}

#ifdef NS_PLATFORM_WINDOWS
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* FrameworkElement::FindResourceW(const char* key) const
{
    BaseComponent* resource = FindResourceW(key);
    NS_CHECK(resource == 0 || DynamicCast<T*>(resource) != 0, "Invalid cast");
    return static_cast<T*>(resource);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* FrameworkElement::FindResourceA(const char* key) const
{
    BaseComponent* resource = FindResourceA(key);
    NS_CHECK(resource == 0 || DynamicCast<T*>(resource) != 0, "Invalid cast");
    return static_cast<T*>(resource);
}
#endif

}
