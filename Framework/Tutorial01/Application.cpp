#include "Application.h"

HRESULT Application::Init(HINSTANCE hInstance, int nCmdShow)
{
	HRESULT hr;

	//Initialize the application window
	hr = InitWindow(hInstance, nCmdShow);
	if (FAILED(hr))
		return hr;

	// Initialize core graphics
	hr = m_pGraphics->Init(m_hWnd);
	if (FAILED(hr))
		return hr;

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	m_windowWidth = rc.right - rc.left;
	m_windowHeight = rc.bottom - rc.top;
	
	hr = m_pGameObject->Init(m_pGraphics->Getd3dDevice(), m_pGraphics->GetImmediateContext());
	if (FAILED(hr))
		return hr;

	//Initialise camera
	m_Camera.SetPosition(0.0f, 0.0f, -2.0f);
	m_Camera.SetProjectionValues(90.0f, static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight), 0.01f, 1000.0f);

	//Initialise objects
	//Initialise mgui
}

void Application::Draw()
{
	m_pGraphics->Draw();

	//Set the render buffer back to the back buffer
	

	ConstantBuffer cb1;
	//cb1.mWorld = XMMatrixTranspose(*mGO);
	cb1.mView = XMMatrixTranspose(m_Camera.GetViewMatrix());			 
	cb1.mProjection = XMMatrixTranspose(m_Camera.GetProjectionMatrix()); 
	m_pGraphics->GetImmediateContext()->UpdateSubresource(m_pGraphics->GetConstantBuffer(), 0, nullptr, &cb1, 0, 0);


	//gameobject->Draw
	m_pGameObject->Draw(m_pGraphics->GetImmediateContext());
	m_pGraphics->GetSwapChain()->Present(1, 0);
}

void Application::Update(float deltaTime)
{
	// Update our time

		//static float t = 0.0f;
	//if (m_driverType == D3D_DRIVER_TYPE_REFERENCE)
	//{
	//	t += (float)XM_PI * 0.0125f;
	//}
	//else
	//{
	//	static ULONGLONG timeStart = 0;
	//	ULONGLONG timeCur = GetTickCount64();
	//	if (timeStart == 0)
	//		timeStart = timeCur;
	//	t = (timeCur - timeStart) / 1000.0f;
	//}

	//HandleKeyboardInput(t);

	//Object Update
	m_pGameObject->Update(deltaTime);

	//Camera Input handling
	//m_pCamera->Update(deltaTime);
	//m_pCamera->HandleInput();

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		exit(0);
	}
}

void Application::Release()
{
	m_pGraphics->Release();
	

}


HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Helper::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	m_hInst = hInstance;
	RECT rc = { 0, 0, 1280, 720 };

	//g_LastMousePos.x = 0;
	//g_LastMousePos.y = 0;

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindow(L"TutorialWindowClass", L"Direct3D 11 Tutorial 5",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!m_hWnd)
		return E_FAIL;

	ShowWindow(m_hWnd, nCmdShow);

	return S_OK;
}
