////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_RESOURCEDICTIONARY_H__
#define __GUI_RESOURCEDICTIONARY_H__


#include <NsCore/Noesis.h>
#include <NsCore/HashMap.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BaseDictionary.h>
#include <NsGui/INameScope.h>
#include <NsGui/IUITreeNode.h>
#include <NsGui/Uri.h>


namespace Noesis
{

class ResourceDictionary;
struct NotifyCollectionChangedEventArgs;

template<class T> class UICollection;
typedef UICollection<ResourceDictionary> ResourceDictionaryCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a hash table implementation that contains resources used by the UI.
///
/// ResourceDictionaries are implicitly created when using the *Resources* property of tree
/// elements. But a resource dictionary can also be declared as a single XAML file for later runtime
/// load or for merging it into the Application or a FrameworkElement.
///
/// .. code-block:: xml
///
///    <ResourceDictionary
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml">
///      <SolidColorBrush x:Key="NormalBrush" Color="Silver"/>
///      <Style TargetType="Button">
///        <Setter Property="FontSize" Value="20"/>
///        <Setter Property="Background" Value="Crimson"/>
///      </Style>
///    </ResourceDictionary>
///
/// http://msdn.microsoft.com/en-us/library/system.windows.resourcedictionary.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ResourceDictionary: public BaseDictionary, public IUITreeNode
{
public:
    ResourceDictionary();
    ~ResourceDictionary();

    /// Indicates if this dictionary is read-only
    bool IsReadOnly() const;

    /// Indicates if this dictionary or any of the merged dictionaries contains a resource
    bool HasResourcesDefined() const;

    /// Gets the collection of merged dictionaries
    ResourceDictionaryCollection* GetMergedDictionaries() const;

    /// Get or set the source file for this dictionary
    //@{
    const Uri& GetSource() const;
    void SetSource(const Uri& source);
    //@}

    /// Returns the number of entries of the base dictionary (excluding merged dictionaries)
    uint32_t Count() const;

    /// Gets the value stored for the specified key
    BaseComponent* Get(const char* key) const;
    template<class T> T* Get(const char* key) const;

    /// Determines whether the dictionary contains an element with the specified key
    bool Contains(const char* key) const;

    /// Removes the element with the specified key from the dictionary
    void Remove(const char* key);

    /// Removes all elements from the dictionary
    void Clear();

    /// Enumerates the entries of the base dictionary (excluding merged dictionaries)
    typedef Delegate<void (const char*, BaseComponent*)> EnumCallback;
    void EnumKeyValues(const EnumCallback& callback) const;

    /// Destroyed delegate is raised when object is going to be destroyed
    typedef Delegate<void(ResourceDictionary*)> DestroyedDelegate;
    DestroyedDelegate& Destroyed();

    /// From IDictionary
    //@{
    void Add(const char* key, BaseComponent* value) override;
    void Set(const char* key, BaseComponent* value) override;
    bool Find(const char* key, Ptr<BaseComponent>& resource) const override;
    //@}

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const override;
    void SetNodeParent(IUITreeNode* parent) override;
    BaseComponent* FindNodeResource(const char* key, bool fullElementSearch) const override;
    BaseComponent* FindNodeName(const char* name) const override;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

protected:
    // From BaseRefCounted
    //@{
    int32_t OnDestroy() override;
    //@}

private:
    friend struct UI;
    friend class Style;
    friend class FrameworkTemplate;
    friend class ResourceDictionaryTest;

    /// Makes this dictionary (and merged dictionaries) read-only
    void SetReadOnly();

    /// Makes this dictionary part of the Application resources
    void SetAppDictionary();

    /// Seals all values of this dictionary and merged dictionaries
    void SealValues();

    /// Item management
    void OnItemAdded(BaseComponent* item);
    void OnItemRemoved(BaseComponent* item);
    void OnAllItemsRemoved();

    /// Clears all dictionary elements, including merged dictionaries
    void Reset();

    /// Load source dictionary resource
    void LoadSource();

    /// Called when a dictionary is added to be merged
    void OnMergedDictionariesChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);
    void OnAddMergedDictionary(ResourceDictionary* dict);
    void OnRemoveMergedDictionary(ResourceDictionary* dict);
    void OnRemoveMergedDictionaryForMerged(ResourceDictionary* dict);

    void EnsureMergedDictionaries(bool registerNotifications);

private:
    IUITreeNode* mOwner;
    Uri mSource;
    bool mIsReadOnly;
    bool mIsAppDictionary;

    Ptr<ResourceDictionaryCollection> mMergedDictionaries;

    typedef HashMap<String, Ptr<BaseComponent>> Resources;
    Resources mResources;

    DestroyedDelegate mDestroyedDelegate;

    NS_DECLARE_REFLECTION(ResourceDictionary, BaseDictionary)
};

NS_WARNING_POP

}

#include <NsGui/ResourceDictionary.inl>


#endif
