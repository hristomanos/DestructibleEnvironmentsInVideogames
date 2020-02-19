#include "Application.h"

	DirectX::XMFLOAT4 g_EyePosition(0.0f, 2.0f, -5, 1.0f);
HRESULT Application::Init(HINSTANCE hInstance, int nCmdShow)
{
	HRESULT hr;

	//Initialize the application window
	hr = InitWindow(hInstance, nCmdShow);
	if (FAILED(hr))
		return hr;


	// Initialize core graphics
	m_pGraphics = new Graphics();
	hr = m_pGraphics->Init(m_hWnd);
	if (FAILED(hr))
		return hr;

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	m_windowWidth = rc.right - rc.left;
	m_windowHeight = rc.bottom - rc.top;


	//	// Initialize the world matrix
	g_World1 = XMMatrixIdentity();

	// Initialize the view matrix
	XMVECTOR Eye = XMLoadFloat4(&g_EyePosition);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, m_windowWidth / (FLOAT)m_windowHeight, 0.01f, 100.0f);


	m_pCamera = new Camera(m_windowWidth, m_windowHeight);

	//Initialise direct input
	m_pDirectInput = new DirectInput();
	if (!m_pDirectInput->InitDirectInput(hInstance, m_hWnd))
	{
		MessageBox(0, L"Direct Input Initialization - Failed",
			L"Error", MB_OK);
		return 0;
	}

	//Initialise camera
	m_Camera2.SetPosition(0.0f, 0.0f, -5.0f);
	m_Camera2.SetProjectionValues(90.0f, static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight), 0.01f, 1000.0f);

	//Initialise object
	m_pGameObject = new DrawableGameObject();
	hr = m_pGameObject->Init(m_pGraphics->Getd3dDevice(), m_pGraphics->GetImmediateContext());
	if (FAILED(hr))
		return hr;




	//Initialise mgui

	return S_OK;
}

void Application::Update(float deltaTime)
{
	m_pDirectInput->DetectInput(deltaTime,m_pCamera, m_hWnd);

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
	
	//m_Camera->Update(deltaTime);
	//m_pCamera->HandleInput();

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		exit(0);
	}
}

void Application::Draw()
{
	m_pGraphics->Draw();

	//Draw Camera

	//Clear our back buffer to the updated color
	//float bgColor[4] = { m_Red,m_Green,m_Blue };
	//m_pGraphics->GetImmediateContext()->ClearRenderTargetView(m_pGraphics->GetRenderTargetView(), bgColor);

	XMMATRIX *mGO = m_pGameObject->getTransform();
	
	ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose(*mGO);
	cb1.mView = XMMatrixTranspose(m_pCamera->camView); //(m_Camera.GetViewMatrix()); g_View
	cb1.mProjection = XMMatrixTranspose(m_pCamera->camProjection); //g_Projection
	m_pGraphics->GetImmediateContext()->UpdateSubresource(m_pGraphics->GetConstantBuffer(), 0, nullptr, &cb1, 0, 0);


	m_pGameObject->Draw(m_pGraphics->GetImmediateContext());
	m_pGraphics->GetSwapChain()->Present(0, 0);
}

void Application::Release()
{
	if (m_pGraphics)
		m_pGraphics->Release();

	if (m_pGameObject)
			m_pGameObject->Release();

	if (m_pDirectInput)
		m_pDirectInput->~DirectInput();

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
	wcex.lpszClassName = L"FrameworkWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	m_hInst = hInstance;
	RECT rc = { 0, 0, 1280, 720 };


	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindow(L"FrameworkWindowClass", 
						  L"Direct3D 11 Framework",
						  WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
						  CW_USEDEFAULT, 
						  CW_USEDEFAULT, 
						  rc.right - rc.left, 
						  rc.bottom - rc.top, 
						  nullptr, 
						  nullptr, 
						  hInstance,
						  nullptr);

	if (!m_hWnd)
		return E_FAIL;

	ShowWindow(m_hWnd, nCmdShow);

	return S_OK;
}

