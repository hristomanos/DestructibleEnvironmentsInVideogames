#include "UserInterface.h"



UserInterface::UserInterface(Graphics* graphics, DrawableGameObject* drawableGameObject)
{
	m_pGraphics = graphics;
	m_pDrawableGameObject = drawableGameObject;
}


UserInterface::~UserInterface()
{
}


void UserInterface::Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	//Setup ImGui
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); // get input output
	ImGui_ImplWin32_Init(hWnd); // init win 32
	ImGui_ImplDX11_Init(device, deviceContext); //Init directx11
	ImGui::StyleColorsDark(); // style of gui
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 3.0f);
}



void UserInterface::Render(bool rayHit)
{
	//Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//Create Main ImGui window
	ImGui::Begin("Inspector");
	ImGui::Text("Mouse camera movement");
	ImGui::Separator();
	ImGui::Text("Enable : I");
	ImGui::Text("Disable : O");
	//ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
	ImGui::Separator();
	
	

	ShowOverlay();
	//ImGui::ShowDemoWindow(); //ImGui demo window
	ImGui::Checkbox("RayHit", &rayHit);


	if (ImGui::CollapsingHeader("Light"))
	{
		ImGui::PushID("Light");
		ImGui::Text("Position");
		ImGui::Separator();
		XMFLOAT4 tempLightPos = m_pGraphics->GetLightPosition(); // Get current light position
			if (ImGui::SliderFloat("X", (float*)& tempLightPos.x, -10, 10) || ImGui::SliderFloat("Y", (float*)& tempLightPos.y, -10, 10) || ImGui::SliderFloat("Z", (float*)&tempLightPos.z, -10, 10))
				m_pGraphics->SetLightPosition(tempLightPos);
			
			//Looks nicer
			//XMFLOAT4 tempLightPos = m_pGraphics->GetLightPosition(); // Get current light position
			//if (ImGui::DragFloat3("position", (float*)&tempLightPos, 0.1f, -15.0f, 15.0f))
			//	m_pGraphics->SetLightPosition(tempLightPos);

			float* tempColour = m_pGraphics->GetLightColour();
			ImGui::ColorEdit4("Diffuse colour", tempColour);
			m_pGraphics->SetLightColour(tempColour);

		ImGui::PopID();
	}

	if (ImGui::CollapsingHeader("Cube"))
	{
		ImGui::PushID("Cube");
		ImGui::Text("Position");
		ImGui::Separator();
		XMFLOAT3 tempCubePos = m_pDrawableGameObject->GetPosition();
		if (ImGui::SliderFloat("X", (float*)& tempCubePos.x, -10, 10) || ImGui::SliderFloat("Y", (float*)& tempCubePos.y, -10, 10) || ImGui::SliderFloat("Z", (float*)& tempCubePos.z, -10, 10))
			m_pDrawableGameObject->SetPosition(tempCubePos);

		ImGui::Text("Textures");
		ImGui::Separator();
		for (ID3D11ShaderResourceView* texture : m_pDrawableGameObject->g_pTextureArr)
		{
			ImGui::Image((void*)texture, ImVec2(128, 128));
			ImGui::SameLine();
		}
		ImGui::PopID();
	}





	ImGui::End();

	//Assemble together draw data
	ImGui::Render();
	//Render draw data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void UserInterface::LightWindow()
{	
	ImGui::Begin("Light");
	ImGui::Text("Position");

	XMFLOAT4 tempLightPos = m_pGraphics->GetLightPosition(); // Get current light position
	if (ImGui::SliderFloat("X", (float*)& tempLightPos.x, -10, 10) || ImGui::SliderFloat("Y", (float*)& tempLightPos.y, -10, 10) || ImGui::SliderFloat("Z", (float*)& tempLightPos.z, -10, 10))
		m_pGraphics->SetLightPosition(tempLightPos);

	ImGui::End();
}

void UserInterface::CubeWindow()
{
	ImGui::Begin("Cube");
	ImGui::Text("Position");
	XMFLOAT3 tempCubePos = m_pDrawableGameObject->GetPosition();
	if (ImGui::SliderFloat("X", (float*)&tempCubePos.x, -10, 10) || ImGui::SliderFloat("Y", (float*)&tempCubePos.y, -10, 10) || ImGui::SliderFloat("Z", (float*)&tempCubePos.z, -10, 10))
		m_pDrawableGameObject->SetPosition(tempCubePos);

	ImGui::Text("Textures");
	for(ID3D11ShaderResourceView * texture : m_pDrawableGameObject->g_pTextureArr)
	{
		ImGui::Image((void*)texture, ImVec2(64, 64));
		ImGui::SameLine();
	}

	ImGui::End();
}

// Demonstrate creating a simple static window with no decoration + a context-menu to choose which corner of the screen to use.
void UserInterface::ShowOverlay()
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
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Example: Simple overlay",nullptr, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::Text("Metrics");
		ImGui::Separator();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		//ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
		//ImGui::Text("%d active allocations", io.MetricsActiveAllocations);
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