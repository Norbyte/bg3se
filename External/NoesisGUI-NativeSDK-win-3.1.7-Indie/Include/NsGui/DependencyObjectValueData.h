////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DEPENDENCYOBJECTVALUEDATA_H__
#define __GUI_DEPENDENCYOBJECTVALUEDATA_H__


#include <NsCore/Noesis.h>
#include <NsGui/DependencySystemApi.h>
#include <NsCore/Ptr.h>
#include <NsCore/Vector.h>


namespace Noesis
{

class DependencyProperty;
class Expression;
typedef void* ValueStorage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Structure to store a dependency property value.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_DEPENDENCYSYSTEM_API StoredValue
{
    /// Stores the chain of values for a non-simple property
    struct ComplexValue
    {
        /// Stores the expression when the property is set using SetExpression or a 
        /// ValueProvider returned an expression
        Ptr<Expression> expression;
        /// Source value (using SetValue or returned by a ValueProvider), or if there is an
        /// expression stored above, this will contain the result of evaluating the expression
        ValueStorage base;
        /// Value set by animation using SetAnimate
        ValueStorage animated;
        /// If the property has a coerce callback in the metadata, the resulting value of 
        /// calling the animated or base value is stored here
        ValueStorage coerced;
    };

    union
    {
        /// Stores a simple value (of it's not an expression, nor is animated or coerced)
        ValueStorage simple;
        /// If the value is complex, stores a pointer to the ComplexValue struct
        ComplexValue* complex;
    } value;

    /// 8 bits for storing the priority of the provider which setted the value
    uint8_t provider;

    union
    {
        struct
        {
            // Indicate if the value is initialized
            bool isInitialized:1;
            // Indicate if the value union stores a simple or complex value
            bool isComplex:1;
            // The field ComplexValue::Expression is valid
            bool isExpression:1;
            // The field ComplexValue::Animated is valid
            bool isAnimated:1;
            // The field ComplesValue::Coerced is valid
            bool isCoerced:1;
            // The coerced value was set using SetCurrentValue
            bool isCoercedCurrentValue:1;
        } flags;

        // To quickly set all flags to 0
        uint8_t allFlags;
    };

    StoredValue();

    /// Create a complex struct, points value.complex to it and sets isComplex flag
    void CreateComplex();

    /// Passing void* storage, it returns the effective pointer to value
    static void* StorageToValue(const ValueStorage* storage, uint32_t size);

    /// Returns the void* member (simple, value, animated or coerced) containing the final value
    ValueStorage& GetFinalStorage() const;

    /// Returns a pointer to the final value
    void* GetFinalValue(uint32_t sizeOfT) const;

    /// Returns a pointer to the final value without the coerce
    void* GetUncoercedValue(uint32_t sizeOfT) const;

    /// Returns a pointer to the coerced value
    void* GetCoercedValue(uint32_t sizeOfT) const;

    /// Returns a pointer to the source value (value.simple or value.complex.base)
    void* GetBaseValue(uint32_t sizeOfT) const;

    /// Destroy all values (simple or complex)
    void DestroyAllValues(const DependencyProperty* dp);
};

}

#endif
