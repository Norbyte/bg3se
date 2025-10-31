
////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/DynamicCast.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> Ptr<T> GUI::LoadXaml(const Uri& uri)
{
    Ptr<BaseComponent> xaml = LoadXaml(uri);
    NS_CHECK(xaml == 0 || DynamicPtrCast<T>(xaml) != 0, "Invalid cast");
    return StaticPtrCast<T>(xaml);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> Ptr<T> GUI::LoadXaml(const char* filename)
{
    Ptr<BaseComponent> xaml = LoadXaml(filename);
    NS_CHECK(xaml == 0 || DynamicPtrCast<T>(xaml) != 0, "Invalid cast");
    return StaticPtrCast<T>(xaml);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> Ptr<T> GUI::ParseXaml(const char* xamlText)
{
    Ptr<BaseComponent> xaml = ParseXaml(xamlText);
    NS_CHECK(xaml == 0 || DynamicPtrCast<T>(xaml) != 0, "Invalid cast");
    return StaticPtrCast<T>(xaml);
}

}
