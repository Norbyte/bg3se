////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DURATION_H__
#define __GUI_DURATION_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/TimeSpan.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
enum DurationType
{
    /// Identifies a Duration value that is automatically determined
    DurationType_Automatic,

    /// Indicates this Duration represents a TimeSpan value
    DurationType_TimeSpan,

    /// Identifies a Duration value that represents an infinite interval
    DurationType_Forever
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the duration of time that a Timeline is active.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.duration.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Duration
{
public:
    Duration();
    Duration(const TimeSpan& timeSpan);

    /// Creates a new Duration of *Automatic* type
    static Duration Automatic();

    /// Creates a new Duration of *Forever* type
    static Duration Forever();

    /// Tries to parse a Duration from a string
    static bool TryParse(const char* str, Duration& result);

    /// Gets duration time
    inline const TimeSpan& GetTimeSpan() const;

    /// Gets duration type
    inline DurationType GetDurationType() const;

    /// Gets duration as a string
    String ToString() const;

    // Operators
    //@{
    Duration& operator=(const TimeSpan& other);
    bool operator==(const Duration& other) const;
    bool operator!=(const Duration& other) const;
    //@}

private:
    DurationType mDurationType;
    TimeSpan mTimeSpan;

    NS_DECLARE_REFLECTION(Duration, NoParent)
};

}

#include <NsGui/Duration.inl>


#endif
