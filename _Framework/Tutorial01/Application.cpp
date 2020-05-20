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

	m_pChunk = new Chunk();
	m_pChunk->CreateMesh(m_pGraphics->Getd3dDevice(), m_pGraphics->GetImmediateContext());

	//Initialise ImGui
	InitImGui(m_hWnd, m_pGraphics->Getd3dDevice(), m_pGraphics->GetImmediateContext());

	return S_OK;
}

void Application::InitImGui(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); // get input output
	ImGui_ImplWin32_Init(hWnd); // init win 32
	ImGui_ImplDX11_Init(device, deviceContext); //Init directx11
	ImGui::StyleColorsDark(); // style of gui
}

void Application::RenderUI()
{
	//Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//Create ImGui window
	ImGui::Begin("Inspector");
	ImGui::Text("Mouse camera movement");
	ImGui::Text("Enable : I");
	ImGui::Text("Disable : O");

	bool enableWireFrame = m_pGraphics->GetEnableWireFrame();
	if (ImGui::Checkbox("Wireframe", (bool*)&enableWireFrame))
	{
		m_pGraphics->SetEnableWireFrame(enableWireFrame);
	}

	ShowOverlay();
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Camera"))
	{
		XMFLOAT3* camPos = new XMFLOAT3(0, 0, 0);
		XMStoreFloat3(camPos, m_pCamera->GetCamPosition());

		ImGui::PushID("Camera");
		ImGui::Text("Position");
		ImGui::DragFloat("X", &camPos->x, 0.1f, -50000.0f, 50000.0f, "%.2f");
		ImGui::DragFloat("Y", &camPos->y, 0.1f, -50000.0f, 50000.0f, "%.2f");
		ImGui::DragFloat("Z", &camPos->z, 0.1f, -50000.0f, 50000.0f, "%.2f");

		ImGui::Separator();
		ImGui::DragFloat("Speed", &m_pCamera->m_MovementSpeed, 0.1f, 0.0f, 100.0f, "%.2f");
		ImGui::NewLine();
		ImGui::PopID();
	}

	//UI
	m_pGraphics->UI();
	

	ImGui::End();
	//Assemble together draw data
	ImGui::Render();
	//Render draw data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

// Demonstrate creating a simple static window with no decoration + a context-menu to choose which corner of the screen to use.
void Application::ShowOverlay()
{
	const float DISTANCE = 10.0f;
	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	if (corner != -1)
	{
		ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	}
	ImGui::SetNextWindowBgAlpha(0.35f);
	if (ImGui::Begin("Example: Simple overlay", nullptr, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::Text("Metrics");
		ImGui::Separator();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("(right-click to change position)");
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;

			ImGui::EndPopup();
		}
	}
	ImGui::End();
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


void Application::Update(double deltaTime)
{
	m_pDirectInput->DetectInput(deltaTime, m_pCamera, m_hWnd);

	m_pChunk->UpdateMesh(deltaTime);
}

void Application::Draw()
{
	m_pGraphics->Draw();

	m_pChunk->RenderMesh(cb1,m_pCamera,m_pGraphics);

	RenderUI();

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
}




