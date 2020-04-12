#include "Application.h"

HRESULT Application::Init(HINSTANCE hInstance, int nCmdShow)
{
	HRESULT hr;

	//Initialize the application window
	hr = InitWindow(hInstance, nCmdShow);
	if (FAILED(hr))
	{
		MessageBox(0, L"Window Initialisation - Failed",
			L"Error", MB_OK);
		return hr;
	}


	// Initialize core graphics
	m_pGraphics = new Graphics();
	hr = m_pGraphics->Init(m_hWnd);
	if (FAILED(hr))
	{
		MessageBox(0, L"Graphics Initialisation - Failed",
			L"Error", MB_OK);
		return hr;
	}

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	m_windowWidth = rc.right - rc.left;
	m_windowHeight = rc.bottom - rc.top;

	//Initialise camera
	m_pCamera = new Camera(m_windowWidth, m_windowHeight);

	//Initialise direct input
	m_pDirectInput = new DirectInput();
	if (!m_pDirectInput->InitDirectInput(hInstance, m_hWnd))
	{
		MessageBox(0, L"Direct Input Initialization - Failed",
			L"Error", MB_OK);
		return 0;
	}

	
	//Initialise object
	m_pGameObject = new DrawableGameObject();
	hr = m_pGameObject->Init(m_pGraphics->Getd3dDevice(), m_pGraphics->GetImmediateContext());
	if (FAILED(hr))
		return hr;

	//for (int x = 0; x < 2; x++)
	//{
	//	for (int y = 0; y < 2; y++)
	//	{
	//		for (int z = 0; z < 2; z++)
	//		{
	//			m_VoxelMap[x][y][z] = true;
	//			m_Cubes[x][y][z] = new DrawableGameObject();
	//			m_Cubes[x][y][z]->Init(m_pGraphics->Getd3dDevice(), m_pGraphics->GetImmediateContext());
	//			m_Cubes[x][y][z]->setPosition(XMFLOAT3(x * 2 , y * 2 , z * 2 ));
	//		}
	//	}
	//}


	m_pChunk = new Chunk();
	m_pChunk->CreateMesh(m_pGraphics->Getd3dDevice(), m_pGraphics->GetImmediateContext());

	//Initialise user interface
	m_pUserInterface = new UserInterface(m_pGraphics,m_pGameObject);
	m_pUserInterface->Init(m_hWnd,m_pGraphics->Getd3dDevice(),m_pGraphics->GetImmediateContext());

	return S_OK;
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


void Application::Update(float deltaTime)
{
	static float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;

	XMMATRIX* mGO = m_pGameObject->GetTransform();
	std::vector<XMFLOAT3> vertPosArray = m_pGameObject->GetVertPosArray();
	std::vector<WORD> indexPosArray = m_pGameObject->GetIndexPosArray();


	m_pDirectInput->DetectInput(deltaTime,m_pCamera, m_hWnd, vertPosArray, indexPosArray,*mGO);

	m_pGameObject->Update(t);

	//Object Update

	//for (int x = 0; x < 2; x++)
	//{
	//	for (int y = 0; y < 2; y++)
	//	{
	//		for (int z = 0; z < 2; z++)
	//		{
	//			m_Cubes[x][y][z]->Update(t);
	//		}
	//	}
	//}

	m_pChunk->UpdateMesh(t);


	if (GetAsyncKeyState(VK_ESCAPE))
	{
		exit(0);
	}
}

void Application::Draw()
{
	m_pGraphics->Draw();

	//Draw cube1
	XMMATRIX *mGO = m_pGameObject->GetTransform();
	cb1.mWorld = XMMatrixTranspose(*mGO);
	cb1.mView = XMMatrixTranspose(m_pCamera->GetViewMatrix());
	cb1.mProjection = XMMatrixTranspose(m_pCamera->GetProjectionMatrix()); 
	m_pGraphics->GetImmediateContext()->UpdateSubresource(m_pGraphics->GetConstantBuffer(), 0, nullptr, &cb1, 0, 0);

	m_pGameObject->Draw(m_pGraphics->GetImmediateContext());

	//for (int x = 0; x < 2; x++)
	//{
	//	for (int y = 0; y < 2; y++)
	//	{
	//		for (int z = 0; z < 2; z++)
	//		{

	//			mGO = m_Cubes[x][y][z]->getTransform();
	//			cb1.mWorld = XMMatrixTranspose(*mGO);
	//			cb1.mView = XMMatrixTranspose(m_pCamera->GetViewMatrix());
	//			cb1.mProjection = XMMatrixTranspose(m_pCamera->GetProjectionMatrix());
	//			m_pGraphics->GetImmediateContext()->UpdateSubresource(m_pGraphics->GetConstantBuffer(), 0, nullptr, &cb1, 0, 0);

	//			m_Cubes[x][y][z]->Draw(m_pGraphics->GetImmediateContext());
	//		}
	//	}
	//}

	m_pChunk->RenderMesh(cb1,m_pCamera,m_pGraphics);

	//Draw ImGui
	m_pUserInterface->Render(m_pDirectInput->rayHit);

	//Draw on screen
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

	if (m_pUserInterface)
			m_pUserInterface->~UserInterface();
}




