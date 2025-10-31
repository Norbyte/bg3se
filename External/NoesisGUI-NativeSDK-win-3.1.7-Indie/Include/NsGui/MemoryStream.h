////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __MEMORY_STREAM_H__
#define __MEMORY_STREAM_H__


#include <NsGui/Stream.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Stream implementation whose backing store is memory
////////////////////////////////////////////////////////////////////////////////////////////////////
class MemoryStream: public Stream
{
public:
    MemoryStream(const void *buffer, uint32_t size);

    /// From Stream
    //@{
    void SetPosition(uint32_t pos) override;
    uint32_t GetPosition() const override;
    uint32_t GetLength() const override;
    uint32_t Read(void* buffer, uint32_t size) override;
    void Close() override;
    //@}

private:
    const void* const mBuffer;
    const uint32_t mSize;
    uint32_t mOffset;
};

}

#include <NsGui/MemoryStream.inl>

#endif
