////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_FRAMEWORKPROPERTYMETADATA_H__
#define __CORE_FRAMEWORKPROPERTYMETADATA_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/UIPropertyMetadata.h>
#include <NsGui/UpdateSourceTrigger.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies the types of framework-level property behavior that pertain to a particular
/// dependency property in the property system.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum FrameworkPropertyMetadataOptions
{
    FrameworkPropertyMetadataOptions_None = 0x000,
    FrameworkPropertyMetadataOptions_AffectsMeasure = 0x001,
    FrameworkPropertyMetadataOptions_AffectsArrange = 0x002,
    FrameworkPropertyMetadataOptions_AffectsParentMeasure = 0x004,
    FrameworkPropertyMetadataOptions_AffectsParentArrange = 0x008,
    FrameworkPropertyMetadataOptions_AffectsRender = 0x010,
    FrameworkPropertyMetadataOptions_Inherits = 0x020,
    FrameworkPropertyMetadataOptions_OverridesInheritanceBehavior = 0x040,
    FrameworkPropertyMetadataOptions_NotDataBindable = 0x080,
    FrameworkPropertyMetadataOptions_BindsTwoWayByDefault = 0x100,
    FrameworkPropertyMetadataOptions_Journal = 0x400,
    FrameworkPropertyMetadataOptions_SubPropertiesDoNotAffectRender = 0x800
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Reports or applies metadata for a dependency property, specifically adding framework-specific
/// property system characteristics.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API FrameworkPropertyMetadata: public UIPropertyMetadata
{
public:
    /// Helper functions to create a new PropertyMetadata
    //@{
    static Ptr<FrameworkPropertyMetadata> Create();

    static Ptr<FrameworkPropertyMetadata> Create(const PropertyChangedCallback& changed);

    static Ptr<FrameworkPropertyMetadata> Create(const CoerceValueCallback& coerce);

    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue);

    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue,
        const PropertyChangedCallback& changed);

    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue,
        const CoerceValueCallback& coerce);

    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue, uint32_t options);

    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue, uint32_t options,
        UpdateSourceTrigger defaultTrigger);

    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue, uint32_t options,
        const PropertyChangedCallback& changed);

    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue, uint32_t options,
        const CoerceValueCallback& coerce);

    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue, uint32_t options,
        const PropertyChangedCallback& changed, const CoerceValueCallback& coerce);

    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue, uint32_t options,
        const PropertyChangedCallback& changed, const CoerceValueCallback& coerce,
        UpdateSourceTrigger defaultTrigger);
    //@}

    /// Indicates if the property affects measure layout pass
    inline bool AffectsMeasure() const;
    void SetAffectsMeasure(bool value);

    /// Indicates if the property affects arrange layout pass
    inline bool AffectsArrange() const;
    void SetAffectsArrange(bool value);

    /// Indicates if the property affects parent measure layout pass
    inline bool AffectsParentMeasure() const;
    void SetAffectsParentMeasure(bool value);

    /// Indicates if the property affects parent arrange layout pass
    inline bool AffectsParentArrange() const;
    void SetAffectsParentArrange(bool value);

    /// Indicates if the property affects render pass
    inline bool AffectsRender() const;
    void SetAffectsRender(bool value);

    /// Indicates if the property inherits its value from logical tree ancestors
    inline bool Inherits() const;
    void SetInherits(bool value);

    /// Indicates whether the property value inheritance evaluation should span across certain
    /// content boundaries in the logical tree of elements
    inline bool OverridesInheritanceBehavior() const;
    void SetOverridesInheritanceBehavior(bool value);

    /// Indicates whether the dependency property supports data binding
    inline bool IsNotDataBindable() const;
    void SetIsNotDataBindable(bool value);

    /// Indicates whether the property binds two-way by default
    inline bool BindsTwoWayByDefault() const;
    void SetBindsTwoWayByDefault(bool value);

    /// Indicates if the value of this property should be saved/restored when journaling by Uri
    inline bool Journal() const;
    void SetJournal(bool value);

    /// Indicates whether sub-properties of the dependency property do not affect the rendering of
    /// the containing object
    inline bool SubPropertiesDoNotAffectRender() const;
    void SetSubPropertiesDoNotAffectRender(bool value);

    /// Gets or sets the default for UpdateSourceTrigger to use when bindings for the property with
    ///  this metadata are applied, which have their UpdateSourceTrigger set to Default.
    inline UpdateSourceTrigger DefaultUpdateSourceTrigger() const;
    void SetDefaultUpdateSourceTrigger(UpdateSourceTrigger value);

    /// From PropertyMetadata
    //@{
    void ClearInheritedValues();
    void Merge(const PropertyMetadata* ancestor);
    //@}

protected:
    // Constructors
    //@{
    FrameworkPropertyMetadata();
    static Ptr<FrameworkPropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue);
    static Ptr<FrameworkPropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, const PropertyChangedCallback& changed);
    static Ptr<FrameworkPropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, const CoerceValueCallback& coerce);
    static Ptr<FrameworkPropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, uint32_t options);
    static Ptr<FrameworkPropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, uint32_t options, UpdateSourceTrigger defaultTrigger);
    static Ptr<FrameworkPropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, uint32_t options,
        const PropertyChangedCallback& changed);
    static Ptr<FrameworkPropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, uint32_t options,
        const CoerceValueCallback& coerce);
    static Ptr<FrameworkPropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, uint32_t options,
        const PropertyChangedCallback& changed, const CoerceValueCallback& coerce);
    static Ptr<FrameworkPropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, uint32_t options,
        const PropertyChangedCallback& changed, const CoerceValueCallback& coerce,
        UpdateSourceTrigger defaultTrigger);
    //@}

private:
    void SetOptions(uint32_t options, UpdateSourceTrigger defaultTrigger);
    void SetOptionValue(uint32_t flag, bool value);

private:
    uint32_t mLocalOptions;
    uint32_t mOptions;

    NS_DECLARE_REFLECTION(FrameworkPropertyMetadata, UIPropertyMetadata)
};

}

#include <NsGui/FrameworkPropertyMetadata.inl>

#endif
