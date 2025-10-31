////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_KEYTIME_H__
#define __GUI_KEYTIME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/TimeSpan.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the different types of behavior for a KeyTime
////////////////////////////////////////////////////////////////////////////////////////////////////
enum KeyTimeType
{
    /// Specifies that the allotted total time for an animation sequence is divided evenly amongst 
    /// each of the key frames.
    KeyTimeType_Uniform,
    /// Specifies that each KeyTime value is expressed as a percentage of the total time allotted 
    /// for a given animation sequence.
    KeyTimeType_Percent,
    /// Specifies that each KeyTime is expressed as a TimeSpan value relative to the BeginTime of 
    /// an animation sequence.
    KeyTimeType_TimeSpan,
    /// Specifies that adjacent KeyFrames are each allotted a slice of time proportional to their 
    /// length, respectively. The overall goal is to produce a length value that keeps the pace of 
    /// the animation sequence constant.
    KeyTimeType_Paced
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies the precise timing when a particular key frame should take place
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API KeyTime
{
public:
    KeyTime();

    /// Creates a new KeyTime instance, with the KeyTimeType property initialized to the value of 
    /// the specified parameter
    static KeyTime FromPercent(float value);
    
    /// Creates a new KeyTime instance, with the KeyTimeType property initialized to the value of 
    /// the specified parameter
    static KeyTime FromTimeSpan(const TimeSpan& timeSpan);
    
    /// Creates a Paced KeyTime which creates timing behavior resulting in an animation that 
    /// interpolates at a constant rate
    static KeyTime Paced();

    /// Creates a Uniform KeyTime which divides the allotted time of the animation evenly between 
    /// key frames
    static KeyTime Uniform();

    /// Generates a string representation of the point
    /// The string has the following form: "Uniform", "Paced", "21.5%", or a TimeSpan: "0:0:1"
    String ToString() const;

    /// Tries to parse a KeyTime from a string
    static bool TryParse(const char* str, KeyTime& result);
    
    /// Gets the KeyTime type
    inline KeyTimeType GetType() const;
    
    /// Returns the TimeSpan value for a KeyTime of type KeyTimeType_TimeSpan
    inline const TimeSpan& GetTimeSpan() const;
    
    /// Returns the percent value for a KeyTime of type KeyTimeType_Percent
    inline float GetPercent() const;

    /// Compares two KetTime objects
    //@{
    bool operator==(const KeyTime& other) const;
    bool operator!=(const KeyTime& other) const;
    //@}

private:
    KeyTime(float percent);
    KeyTime(const TimeSpan& timeSpan);

private:
    KeyTimeType mType;
    float mPercent;
    TimeSpan mTimeSpan;
    
    NS_DECLARE_REFLECTION(KeyTime, NoParent)
};

}

#include <NsGui/KeyTime.inl>


#endif
