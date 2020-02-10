#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>


class Helper
{
public:
	static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	static HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
};

