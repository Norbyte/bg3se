BEGIN_SE()

void* GameAllocRaw(std::size_t size)
{
	return GetStaticSymbols().ls__GlobalAllocator__Alloc(size, 5, 0, 8);
}

void GameFree(void* ptr)
{
	GetStaticSymbols().ls__GlobalAllocator__Free(ptr);
}

END_SE()
