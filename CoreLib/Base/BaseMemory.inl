BEGIN_SE()

void* GameAllocRaw(std::size_t size)
{
	return gCoreLibPlatformInterface.Alloc(size);
}

void GameFree(void* ptr)
{
	gCoreLibPlatformInterface.Free(ptr);
}

END_SE()
