////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TIMESPAN_H__
#define __GUI_TIMESPAN_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/StringFwd.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a time interval.
///
/// The interval is expressed in days, hours, minutes, seconds and milliseconds, or in ticks
/// (100-nanosecond units). TimeSpan is generally used from XAML files, parsing with the following
/// signature: ``[ws][-]{ d | [d.]hh:mm[:ss[.ff]] }[ws]``
///
/// .. code-block:: c
///
///     ws: optional white space
///     -: optional minus sign indicating a negative TimeSpan 
///     d: days, ranging from 0 to 10675199
///     hh: hours, ranging from 0 to 23
///     mm: minutes, ranging from 0 to 59 
///     ss: optional seconds, ranging from 0 to 59 
///     ff: optional fractional seconds, consisting of 1 to 7 decimal digits
///
/// .. code-block:: c
///
///     "1" and "1.0:0" represents 1 day
///     "0:1" represents 1 minute
///     "0:0:1" represents 1 second
///     "0:0:0.1" represents 1 millisecond
///     "-0:0:3" represents 3 seconds negative
///
/// https://msdn.microsoft.com/en-us/library/system.timespan(v=vs.110).aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API TimeSpan
{
public:
    inline TimeSpan();
    inline TimeSpan(int64_t ticks);
    inline TimeSpan(double seconds);
    TimeSpan(int hours, int minutes, int seconds);
    TimeSpan(int days, int hours, int minutes, int seconds);
    TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds);

    /// Tries to parse a TimeSpan from a string
    static bool TryParse(const char* str, TimeSpan& result);

    /// Returns a string representation of the TimeSpan instance
    String ToString() const;

    /// Gets Zero Timespan
    static inline TimeSpan Zero();
    
    /// Get the days component of the time interval
    /// \prop
    inline int GetDays() const;
    
    /// Get the hours component of the time interval
    /// \prop
    inline int GetHours() const;
    
    /// Get the minutes component of the time interval
    /// \prop
    inline int GetMinutes() const;
    
    /// Get the seconds component of the time interval
    /// \prop
    inline int GetSeconds() const;
    
    /// Get the milliseconds component of the time interval
    /// \prop
    inline int GetMilliseconds() const;
    
    /// Get total TimeSpan value in Ticks
    /// \prop
    inline int64_t GetTicks() const;
    
    /// Get total TimeSpan value in seconds
    inline double GetTimeInSeconds() const;

    // Operators
    //@{
    inline bool operator==(const TimeSpan& other) const;
    inline bool operator!=(const TimeSpan& other) const;
    inline TimeSpan& operator+=(const TimeSpan& other);
    inline TimeSpan& operator-=(const TimeSpan& other);
    inline TimeSpan& operator*=(float value);
    //@}

private:
    int64_t mTicks;

    NS_DECLARE_REFLECTION(TimeSpan, NoParent)
};

/// TimeSpan Binary operators
//@{
inline TimeSpan operator+(const TimeSpan& t0, const TimeSpan& t1);
inline TimeSpan operator-(const TimeSpan& t0, const TimeSpan& t1);

inline TimeSpan operator*(const TimeSpan& t, float value);
inline TimeSpan operator*(float value, const TimeSpan& t);

inline float operator/(const TimeSpan& t0, const TimeSpan& t1);
inline TimeSpan operator/(const TimeSpan& t, float value);

inline bool operator<(const TimeSpan& t0, const TimeSpan& t1);
inline bool operator<=(const TimeSpan& t0, const TimeSpan& t1);
inline bool operator>(const TimeSpan& t0, const TimeSpan& t1);
inline bool operator>=(const TimeSpan& t0, const TimeSpan& t1);
//@}

}

#include <NsGui/TimeSpan.inl>


#endif
