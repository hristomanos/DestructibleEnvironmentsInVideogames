//--------------------------------------------------------------------------------------
// File: main.cpp
//
// This application demonstrates animation using matrix transformations
//
// http://msdn.microsoft.com/en-us/library/windows/apps/ff729722.aspx
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#define _XM_NO_INTRINSICS_

#include "main.h"

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
POINT g_LastMousePos;

#define DEFAULT_WINDOW_TITLE L"Framework"
#define MS_PER_UPDATE 16.66666666667
#define APPLICATION_WINDOW_WIDTH 1280;
#define APPLICATION_WINDOW_HEIGHT 720;

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	//Create an application instance
	Application* application = new Application(DEFAULT_WINDOW_TITLE, 1280, 720);
	if (FAILED(application->Init(hInstance, nCmdShow)))
		return -1;


	// Main message loop
	MSG msg = {0};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			
			application->Update(MS_PER_UPDATE / 1000.0f);
			application->Draw();

		}
	}
	application->Release();
	//delete application;
	application = nullptr;

	return (int)msg.wParam;
}








