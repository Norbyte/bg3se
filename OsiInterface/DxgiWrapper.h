#pragma once

class DxgiWrapper
{
public:
	DxgiWrapper();
	~DxgiWrapper();

	HRESULT CreateDXGIFactory(REFIID riid, _Out_ void **ppFactory);
	HRESULT CreateDXGIFactory1(REFIID riid, _Out_ void **ppFactory);
	HRESULT CreateDXGIFactory2(UINT Flags, REFIID riid, _Out_ void **ppFactory);

private:
	typedef HRESULT(WINAPI * CreateDXGIFactoryProc)(REFIID riid, _Out_ void **ppFactory);
	typedef HRESULT(WINAPI * CreateDXGIFactory2Proc)(UINT Flags, REFIID riid, _Out_ void **ppFactory);

	HMODULE dxgiHandle;
	CreateDXGIFactoryProc createDXGIFactory;
	CreateDXGIFactoryProc createDXGIFactory1;
	CreateDXGIFactory2Proc createDXGIFactory2;
};

extern std::unique_ptr<DxgiWrapper> gDxgiWrapper;