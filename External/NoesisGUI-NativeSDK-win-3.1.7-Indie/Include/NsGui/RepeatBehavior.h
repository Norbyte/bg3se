////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_REPEATBEHAVIOR_H__
#define __GUI_REPEATBEHAVIOR_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/TimeSpan.h>


namespace Noesis
{

class TimeSpan;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes how the RepeatBehavior calculates its repetitions
////////////////////////////////////////////////////////////////////////////////////////////////////
enum RepeatMode
{
    /// Specifies that a Timeline repeats n times
    RepeatMode_Count,
    /// Indicates that a Timeline repeats for a specific duration
    RepeatMode_Duration,
    /// Specifies an infinite number of repetitions
    RepeatMode_Forever
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes how a Timeline repeats its simple duration.
/// Examples:
///     RepeatBehavior="2x"             --->  Repeats 2 times
///     RepeatBehavior="1.2:30:15.500"  --->  [days.]hours:minutes:seconds[.fractionalSeconds]
///     RepeatBehavior="2.1:20"         --->  [days.]hours:minutes
///     RepeatBehavior="5"              --->  days
///     RepeatBehavior="Forever"        --->  Repeats forever
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API RepeatBehavior
{
public:
    /// Constructors
    //@{
    RepeatBehavior();
    RepeatBehavior(float count);
    RepeatBehavior(const TimeSpan& duration);
    //@}

    /// Creates a RepeatBehavior object that specifies an infinite number of repetitions
    static RepeatBehavior Forever();

    /// Generates a string representation of the point
    /// The string has the following form: "Forever", "2x", or a TimeSpan: "0:0:1"
    String ToString() const;

    /// Tries to parse a RepeatBehavior from a string
    static bool TryParse(const char* txt, RepeatBehavior& result);

    /// Gets repeat mode
    RepeatMode GetRepeatMode() const;

    /// Gets repeat count. Only valid for repeat mode *Count*
    float GetCount() const;

    /// Gets repeat time duration. Only valid for repeat mode *Duration*
    const TimeSpan& GetDuration() const;

    // Comparison operators
    //@{
    bool operator==(const RepeatBehavior& other) const;
    bool operator!=(const RepeatBehavior& other) const;
    //@}

private:
    RepeatMode mRepeatMode;
    float mCount;
    TimeSpan mDuration;

    NS_DECLARE_REFLECTION(RepeatBehavior, NoParent)
};

}


#endif
