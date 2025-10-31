////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_STREAM_H__
#define __GUI_STREAM_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsGui/ProvidersApi.h>


namespace Noesis
{

template<class T> class Ptr;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a read-only generic view of a sequence of bytes
////////////////////////////////////////////////////////////////////////////////////////////////////
class Stream: public BaseComponent
{
public:
    /// Set the current position within the stream
    virtual void SetPosition(uint32_t pos) = 0;

    /// Returns the current position within the stream
    virtual uint32_t GetPosition() const = 0;

    /// Returns the length of the stream in bytes
    virtual uint32_t GetLength() const = 0;

    /// Reads data at the current position and advances it by the number of bytes read
    /// Returns the total number of bytes read. This can be less than the number of bytes requested
    virtual uint32_t Read(void* buffer, uint32_t size) = 0;

    /// Closes the current stream and releases any resources associated with the current stream
    virtual void Close() = 0;
};

NS_GUI_PROVIDERS_API Ptr<Stream> OpenFileStream(const char* filename);

}

#endif
