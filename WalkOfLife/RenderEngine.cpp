//############################################################################################################################################################################################################################################################################################################
//###																																																																									   ###
//###																													WALK OF LIFE MAIN FILE																																							   ###	
//###																																																																									   ###	
//############################################################################################################################################################################################################################################################################################################



#include "RenderEngine.h"
#include <string.h>



namespace{
	RenderEngine* pRenderEngine; //pointer to the application
}

// MSG HANDLER FOR WINMAIN  ( Gives feedback �f program crashes)

LRESULT CALLBACK MainWindowProc(HWND hwindow, UINT msg, WPARAM wParam, LPARAM lParam){ //denna anv�nds endast f�r att komma �t RenderEnginens MsgProc() och skicka vidare meddelanden till den
	return pRenderEngine->MsgProc(hwindow, msg, wParam, lParam);
}

// CONSTRUCTOR

RenderEngine::RenderEngine(HINSTANCE hInstance, std::string name, UINT scrW, UINT scrH){
	this->hInstance = hInstance;
	applicationName = name;
	screen_Width = mainCamera.getWindowWidth();
	screen_Height = mainCamera.getWindowHeight();
	//screen_Width = scrW; //OLD
	//screen_Height = scrH; //OLD
	pRenderEngine = this;
	windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX;
	this->theQuadtree = new Quadtree(0, 0, 100, 100, 1, 6);
}

// DESTRUCTOR

RenderEngine::~RenderEngine(){ //destruktor

}


//######################################################################################################################################################
//###																																				 ###
//###										   INTITIALIZE FUNCTIONS IN : init()																	 ###	
//###																																				 ###	
//######################################################################################################################################################

// INITIALIZE CORE FUNCTIONS
bool RenderEngine::Init(){

	if (!InitWindow()){
		return false; //gick inte att skapa window
	}

	if (!InitDirect3D(hWindow)){
		return false; //gick inte att skapa Direct3D
	}
	// Sets and creates viewport
	SetViewport();

	//Initialize Shaders and triangle data
	Shaders();
	CreatePlaneData();
	TextureFunc();

	//Font
	Fonts();

	//Import
	
	ImportObj("Objects/testPlayer1.obj", "Objects/testPlayer1.mtl", gDevice, true);
	
	ImportObj("Objects/mapPart1.obj", "Objects/mapPart1.mtl", gDevice, false);
	ImportObj("Objects/mapPart2.obj", "Objects/mapPart2.mtl", gDevice, false);
	ImportObj("Objects/mapPart3.obj", "Objects/mapPart3.mtl", gDevice, false);
	ImportObj("Objects/mapPart4.obj", "Objects/mapPart4.mtl", gDevice, false);
	ImportObj("Objects/mapPart5.obj", "Objects/mapPart5.mtl", gDevice, false);
	ImportObj("Objects/mapPart6.obj", "Objects/mapPart6.mtl", gDevice, false);
	ImportObj("Objects/mapPart7.obj", "Objects/mapPart7.mtl", gDevice, false);
	ImportObj("Objects/mapPart7.obj", "Objects/mapPart7.mtl", gDevice, false);

	

	//LIGHT TEST ZONE BITCHES
	/*float l1Int = 1.0f;
	XMFLOAT3 l1Pos = XMFLOAT3(0.0f, 1.0f, -2.0f);
	XMFLOAT4 l1Amb = XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f);
	XMFLOAT4 l1Diff = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 l1Spec = XMFLOAT4(0.5f, 0.2f, 0.2f, 1.0f);
	XMFLOAT3 l1Dir = XMFLOAT3(0.0f, -50.0f, 30.0f);*/



	testLight[0] = LightClass(l_Directional, XMFLOAT3(1.0f, 1.0f, -5.0f), true, true);
	testLight[0].lightObject.Color = XMFLOAT4(Colors::White);
	/*testLight[0].ToggleActive();*/

	LightClass snoppe(l_Point, XMFLOAT3(1.0f, 1.0f, 0.0f), true, true);

	testLight[1] = snoppe;
	snoppe.lightObject.Type = 2;
	testLight[1].lightObject.Position = XMFLOAT4(-4.0f, 8.0f, 1.0f, 1.0f);
	testLight[1].lightObject.Color = XMFLOAT4(Colors::LightGoldenrodYellow);
	testLight[1].lightObject.AttConst = 1.0f;
	testLight[1].lightObject.AttLinear = 0.08f;
	testLight[1].lightObject.AttQuadratic = 0.00000f;
	//testLight[1].ToggleActive();
 	globalAmb = XMFLOAT4(Colors::Black);

	D3D11_BUFFER_DESC lbuffDesc;
	ZeroMemory(&lbuffDesc, sizeof(lbuffDesc));
	lbuffDesc.Usage = D3D11_USAGE_DEFAULT;
	lbuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lbuffDesc.CPUAccessFlags = 0;
	lbuffDesc.MiscFlags = 0;
	lbuffDesc.ByteWidth = sizeof(LightProperties);

	HRESULT hr = gDevice->CreateBuffer(&lbuffDesc, NULL, &lightConstBuff);


	// Material Buffers Init
	ZeroMemory(&lbuffDesc, sizeof(lbuffDesc));
	lbuffDesc.ByteWidth = sizeof(MaterialProperties);
	hr = gDevice->CreateBuffer(&lbuffDesc, NULL, &matConstBuff);



	return true; //om b�da funkade s� returnera true (y)
}

// INITIALIZE WINDOW

bool RenderEngine::InitWindow(){
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.hInstance = hInstance;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW; // hur den ska m�la ut allt, fast spelar nog ingen roll i v�rt fall
	wcex.lpfnWndProc = MainWindowProc;
	//wcex.hCursor = LoadCursor(NULL, IDC_NO); //skoj
	//wcex.hIcon = LoadIcon(NULL, IDI_ERROR); //skoj * 2
	//wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"RENDERENGINECLASS";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)){
		MessageBox(NULL, L"Kunde inte registrera window classen", NULL, NULL);
		return false;
	}

	RECT r = { 0, 0, screen_Width, screen_Height };
	AdjustWindowRect(&r, windowStyle, FALSE); //kommer inte kunna resiza skiten
	int width = r.right - r.left;
	int height = r.bottom - r.top;
	mainwname = L"Build: Walk_Of_Life.0008";
	hWindow = CreateWindow(L"RENDERENGINECLASS",
		L"Direct3D Projekt", //INTE S�KER P� DETTA, namnet p� applikationen blir typ i kinaspr�k s� venne, kan vara detta
		WS_OVERLAPPEDWINDOW,//Window handlers
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width, height, nullptr, nullptr, hInstance, nullptr);

	if (!hWindow){
		MessageBox(NULL, L"Failed to create window", NULL, NULL);
		return false;
	}


	return true;
}

// CREATE VIEWPORT

void RenderEngine::SetViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = screen_Width;
	vp.Height = screen_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gDeviceContext->RSSetViewports(1, &vp);
}

// FPS COUNTER

void RenderEngine::fpscounter()
{
	// Code computes the avarage frame time and frames per second

	static int framecount = 0;
	static float time = 0.0f;
	static float time2 = 0.0f;

	//Function is beeing initialiased every time a frame is made
	framecount++;

	// Compute averages over one second period
	if ((gTimer.TotalTime() - time) >= 0.25f)
	{
		float fps = (float)framecount * 4; // fps = framecount / 1
		fpsDisplay = fps;
		float mspf = 1000.0f / fps;
		float timer = gTimer.TotalTime();
		// Makes a String for the window handler
		std::wostringstream outs;
		
		outs.precision(6);
		outs << mainwname << L" "   
			<< L"        Frame Time: " << mspf << L" (ms)"
			<< L"        Time: " << timer << L" sec";
		
		//Prints the text in the window handler
		SetWindowText(hWindow, outs.str().c_str());

		// Reset for next fps.
		framecount = 0;
		time += 0.25f;

	
		
	}

	if ((gTimer.TotalTime() - time2) >= 0.20f && gCounter.theAge.years < 100){
		gCounter.setMonth(1);
		time2 += 0.20f;

	}
		

	
}

// CREATE TEXTURES

void RenderEngine::TextureFunc(){

	HRESULT texCheck;
	texCheck = CreateDDSTextureFromFile(gDevice, L"Textures/temp.dds", nullptr, &ddsTex1);

}

 // CREATE FONTS

void RenderEngine::Fonts(){

	spritefont.reset(new DirectX::SpriteFont(gDevice, L"Fonts/Arial.spritefont"));
	spriteBatch.reset(new DirectX::SpriteBatch(gDeviceContext));
}

// COMPILE SHADERS FROM .HLSL FILES

HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob)
{
	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}

// CREATE AND COMPILE SHADERS

void RenderEngine::Shaders(){
	// SHADER TESTER
	HRESULT ShaderTest;

	//DEAFULT SHADERS

	//create vertex shader
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorMessage = nullptr;
	ShaderTest = CompileShader(L"defaultVS.hlsl", "VS_main", "vs_5_0", &pVS);
	ShaderTest = gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShader);

	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	ShaderTest = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);
	
	//create pixel shader
	ID3DBlob* pPS = nullptr;
	ShaderTest = CompileShader(L"defaultPS.hlsl", "PS_main", "ps_5_0", &pPS);
	ShaderTest = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShader);

	// Realese shaders
	pVS->Release();
	pPS->Release();
}

// CREATE BUFFERS AND TEMP PLANE

void RenderEngine::CreatePlaneData(){

	struct PlaneVertex
	{
		float x, y, z;
		float ux, uy;
		float nx, ny, nz;
	}
	PlaneVertices[4] =
	{
		-1.8f, -1.8f, -0.9f, //v0 pos
		0.0f, 1.0f,
		0.0f, 0.0f, -1.0f, //n0

		-1.8f, 1.8f, -0.9f, //v1
		0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,  //n1

		1.8f, -1.8f, -0.9f, //v2
		1.0f, 1.0f,
		0.0f, 0.0f, -1.0f,  //n2

		1.8f, 1.8f, -0.9f, //v3
		1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,

	};

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(PlaneVertices);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = PlaneVertices;
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);


	// Rotatation And transform Buffer
	D3D11_BUFFER_DESC transformbuffer;
	memset(&transformbuffer, 0, sizeof(transformbuffer));
	transformbuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transformbuffer.Usage = D3D11_USAGE_DEFAULT;
	transformbuffer.ByteWidth = sizeof(World);

	HRESULT hr1112 = gDevice->CreateBuffer(&transformbuffer, NULL, &gWorld);
	hr1112 = gDevice->CreateBuffer(&transformbuffer, NULL, &cWorld);
}

// INITIALIZE DIRECTX OBJECT

bool RenderEngine::InitDirect3D(HWND hWindow){

	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWindow;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.SampleDesc.Count = 1; //anti-aliasing
	//scd.SampleDesc.Quality - kan vi mecka senare men �r lite saker som ska g�ras d�
	scd.Windowed = TRUE;
	//scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;

	// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext);

	if (SUCCEEDED(hr))
	{
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		gDevice->CreateRenderTargetView(pBackBuffer, NULL, &gBackRufferRenderTargetView);
		pBackBuffer->Release();

		//DepthBuffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
		depthStencilDesc.Width = screen_Width;
		depthStencilDesc.Height = screen_Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		HRESULT hr1 = gDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
		

		D3D11_DEPTH_STENCIL_DESC dsDesc;
		//Depth test settings
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		//Stencil tests
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;
		//Stencil operations - Pixel Front Facing
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		//Stencil operations - Pixel Back Facing
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr3 = gDevice->CreateDepthStencilState(&dsDesc, &gDepthStencilState);
		gDeviceContext->OMSetDepthStencilState(gDepthStencilState, 1);

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		descDSV.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		HRESULT hr2 = gDevice->CreateDepthStencilView(depthStencilBuffer, &descDSV, &gDepthStencilView);

		// set the render target as the back buffer
		gDeviceContext->OMSetRenderTargets(1, &gBackRufferRenderTargetView, gDepthStencilView);

		return true; //returnerar att den HAR klarat av att skapa device och swapchain
	}

	return false; //det gick inte att skapa device och swapchain, snyft :'(
}

// MESSAGE HANDLER

LRESULT RenderEngine::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg){
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CHAR: //en tangentbordsknapp har tryckts i
		switch (wParam){
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


//######################################################################################################################################################
//###																																				 ###
//###								Call functions that shall be updated frame by frame in :  Run() ,  Update()	        							 ###	
//###																																				 ###	
//######################################################################################################################################################

// RUN PROGRAM

int RenderEngine::Run(){

	//om allt funkat:
	ShowWindow(hWindow, SW_SHOW);
	MSG msg = { 0 }; //t�m alla platser i msg
	gTimer.Reset();
	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{ //applikationen �r fortfarande ig�ng
			gTimer.Tick();
			if ((gTimer.TotalTime() - time3) >= 0.01f)
			{

				Update(0.0f);
				Render();
				time3 = gTimer.TotalTime();
			}
			fpscounter();
		}
	}
	return static_cast<int>(msg.wParam);
}

// RENDER

void RenderEngine::Render(){
	static float rot = 0.00f;
	UINT32 vertexSize = sizeof(float) * 8;
	UINT32 offset = 0;
	rot += 0.01;
	float clearColor[] = { 0.15f,0.6f,1.0f, 0.2f };
	gDeviceContext->OMSetBlendState(0, 0, 0xffffffff);
	gDeviceContext->ClearRenderTargetView(gBackRufferRenderTargetView, clearColor);
	gDeviceContext->ClearDepthStencilView(gDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	mainCamera.setPlayerXPos(theCharacter->xPos);
	mainCamera.setPlayerYPos(theCharacter->yPos);

	mainCamera.updateCamera();
	//WORLD
	XMMATRIX YRotation = XMMatrixRotationY(rot);

	//The Camera Matrices are now defined in the camera class (mainCamera)

	XMMATRIX CamView = mainCamera.getCamView();
	XMMATRIX CamProjection = mainCamera.getCamProjection();
	XMMATRIX identityM = XMMatrixIdentity();
	XMMATRIX WorldInv = XMMatrixInverse(nullptr, XMMatrixIdentity());

	World perObjCBData;

	XMMATRIX WVP;
	WVP = identityM* CamView*CamProjection;


	XMStoreFloat4x4(&perObjCBData.WVP, XMMatrixTranspose(WVP));
	XMStoreFloat4x4(&perObjCBData.View, XMMatrixTranspose(CamView));
	XMStoreFloat4x4(&perObjCBData.Projection, XMMatrixTranspose(CamProjection));
	XMStoreFloat4x4(&perObjCBData.WorldSpace, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&perObjCBData.InvWorld, XMMatrixTranspose(WorldInv));

	
	gDeviceContext->UpdateSubresource(gWorld, 0, NULL, &perObjCBData, 0, 0);
	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);
	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);
	
	ID3D11Buffer *pShaderBuffers[2] = { matConstBuff, lightConstBuff };
	gDeviceContext->PSSetConstantBuffers(0, 2, pShaderBuffers);

	//RENDER OBJ FILES

	for each (GameObject var in gamePlatforms)
	{
			gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);
	
		gDeviceContext->IASetInputLayout(gVertexLayout);
		gDeviceContext->IASetVertexBuffers(0, 1, &var.vertexBuffer, &vertexSize, &offset);
		gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
		gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);
		
		var.CalculateWorld();
		var.material = MatPresets::Emerald;
		var.material.SpecPow = 38.0f;
		
		matProperties.Material = var.material;

		gDeviceContext->UpdateSubresource(matConstBuff, 0, nullptr, &matProperties, 0, 0);
		
		

		XMStoreFloat4x4(&perObjCBData.InvWorld, XMMatrixTranspose(XMMatrixInverse(nullptr, var.world)));
		XMStoreFloat4x4(&perObjCBData.WorldSpace, XMMatrixTranspose(var.world));
		WVP = XMMatrixIdentity();
		WVP = var.world * CamView *CamProjection;

		XMStoreFloat4x4(&perObjCBData.WVP, XMMatrixTranspose(WVP));


		gDeviceContext->UpdateSubresource(gWorld, 0, NULL, &perObjCBData, 0, 0);
		gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);

		gDeviceContext->Draw(var.nrElements * 3, 0);
		}



	gDeviceContext->PSSetShaderResources(0, 1, &ddsTex1);

	gDeviceContext->IASetInputLayout(gVertexLayout);
	gDeviceContext->IASetVertexBuffers(0, 1, &theCharacter->vertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);

	theCharacter->CalculateWorld();
	
	
	WVP = XMMatrixIdentity();
	WVP = theCharacter->world * CamView *CamProjection;
	XMStoreFloat4x4(&perObjCBData.InvWorld, XMMatrixTranspose(XMMatrixInverse(nullptr, theCharacter->world)));
	theCharacter->world = XMMatrixTranspose(theCharacter->world);

	XMStoreFloat4x4(&perObjCBData.WVP, XMMatrixTranspose(WVP));
	XMStoreFloat4x4(&perObjCBData.WorldSpace, theCharacter->world);

	XMStoreFloat4x4(&perObjCBData.InvWorld, XMMatrixTranspose(XMMatrixInverse(nullptr, theCharacter->world)));

	gDeviceContext->UpdateSubresource(gWorld, 0, NULL, &perObjCBData, 0, 0);
	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorld);



	gDeviceContext->Draw(theCharacter->nrElements * 3, 0);

	//////////////////////////////////////////////////////////////
	// Draw Text
	spriteBatch->Begin();

	std::wstring yearCount = std::to_wstring(gCounter.theAge.years);
	std::wstring monthCount = std::to_wstring(gCounter.theAge.months);
	std::wstring xPos = std::to_wstring(theCharacter->xPos);
	std::wstring yPos = std::to_wstring(theCharacter->yPos);
	std::wstring dass =std::to_wstring(fpsDisplay);
	std::wstring name(L"FPS: ");
	std::wstring year(L"\nYear: ");
	std::wstring month(L"\nMonth: ");
	std::wstring XXX(L"\nxPos: ");
	std::wstring YYY(L"\nyPos: ");
	std::wstring superutedass = name + dass + year + yearCount + month + monthCount + XXX + xPos + YYY + yPos;

	std::wstring Gameover(L"\nGAME OVER MOTHERFUCKER!! ");
	

	const wchar_t* AMAZING_SUPER_UTE_DASS = superutedass.c_str();

	if (gCounter.theAge.years == 100){

		 AMAZING_SUPER_UTE_DASS = Gameover.c_str();

	}

	spritefont->DrawString(spriteBatch.get(), AMAZING_SUPER_UTE_DASS, DirectX::SimpleMath::Vector2(0, 10));

	spriteBatch->End();
	gSwapChain->Present(0, 0); //v�xla back/front buffer
}

// UPDATES

void RenderEngine::Update(float dt){
		Input theInput;
		theInput.initInput(this->hInstance, hWindow);
		int input = 0;
		bool jump = false;
		input = theInput.detectInput(hWindow);
		jump = theInput.detectJump(hWindow);

		theCollision.TestCollision(gamePlatforms);

		if (input == 1 && theCollision.leftValid() == true)
		{
			this->theCharacter->Move(false); //left
		}

		else if (input == 2)
		{
			this->theCharacter->Move(true); //right

		}

		else
			this->theCharacter->momentum = 0;

		if (jump && theCollision.isGrounded() == true) //om grounded och man har klickat in jump
		{
			this->thePhysics.Jump(theCollision, theCharacter);
			thePhysics.onPlatform = false;
		}

		thePhysics.Gravitation(theCollision, theCharacter);
		theCharacter->CalculateWorld();
	

	// Update Lights
	camPos = XMFLOAT4(camxPos, 4.0f, -10.0f, 1.0f);
	lightProp01.CamPosition = camPos;
	lightProp01.GlobalAmbient = globalAmb;
	lightProp01.lights[0] = testLight[0].lightObject;
	lightProp01.lights[1] = testLight[1].lightObject;
	gDeviceContext->UpdateSubresource(lightConstBuff, 0, NULL, &lightProp01, 0, 0);

}

// REALESE AND CLEANUP

void RenderEngine::Release(){

	gDevice->Release();
	depthStencilBuffer->Release();

	gVertexBuffer->Release();
	gVertexLayout->Release();
	gVertexShader->Release();
	gPixelShader->Release();
	gDeviceContext->Release();

	//Kill Lights
	//delete testLight;
}

void RenderEngine::ImportObj(char* geometryFileName, char* materialFileName, ID3D11Device* gDev, bool player){// , bool isStatic, XMMATRIX startPosMatrix){
	static int gameObjectIndex = 0;
	OBJ objectTest(gDev);
	//Load obj
	objectTest.LoadObject(geometryFileName, materialFileName);

	//Test if filename is correct
	OutputDebugStringA(geometryFileName);
	OutputDebugStringA("\n");
	OutputDebugStringA(materialFileName);
	OutputDebugStringA("\n");
	if (player)
	{
		theCharacter = new PlayerObject(*objectTest.GetVertexBuffer(), XMFLOAT3(0, 9, 9), true, false, BoundingBox(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)));
		theCharacter->CreateBBOXVertexBuffer(gDevice);
		theCharacter->nrElements = objectTest.GetNrElements();
		Collision tempC(theCharacter);
		theCollision = tempC;
		//gameObjects.push_back(*theCharacter);
	}

	else
	{
		Platform testPlatform(false, objectTest.tempVerts, *objectTest.GetVertexBuffer(), XMFLOAT3(0, 0, 0), true, true, *objectTest.theBoundingBox);
		testPlatform.CreateBBOXVertexBuffer(gDevice);
		testPlatform.nrElements = objectTest.GetNrElements();
		theQuadtree->AddObject(&testPlatform);
		gamePlatforms.push_back(testPlatform);
	}

	//testObject.ObjName = objectTest.GetName();
	//Fill buffers
	//testObject.gameObjectIndex = gameObjectIndex; //anv�nds f�rtillf�llet vid frustum contains checken
	//gameObjectIndex++;
	//testObject.gVertexBuffer = *objectTest.GetVertexBuffer();


	//testObject.verteciesPos = objectTest.GetVertexPositions();
	//testObject.verteciesIndex = objectTest.GetIndecies();
	//testObject.verteciesPosX = objectTest.GetVerticiesX();
	//testObject.verteciesPosY = objectTest.GetVerticiesY();
	//testObject.verteciesPosZ = objectTest.GetVerticiesZ();




}

