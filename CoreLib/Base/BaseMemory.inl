BEGIN_SE()

_Post_writable_byte_size_(size) void* GameAllocRaw(std::size_t size)
{
    return gCoreLibPlatformInterface.Alloc(size);
}

void GameFree(_Post_ptr_invalid_ void* ptr) noexcept
{
    gCoreLibPlatformInterface.Free(ptr);
}

END_SE()
