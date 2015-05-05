#pragma once
#include <windows.h>
#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>
#include <DirectXMathVector.inl>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <DirectXCollision.h>

//INLCUDE H FILES HERE


//#include "GameObject.h"
#include "Physics.h"
#include "Collision.h"
#include "Input.h"
#include "Platform.h"
#include "PlayerObject.h"
#include "GameTimer.h"
#include "Counters.h"
#include "Obj.h"
#include "Extra DirectXLibs\Inc\DDSTextureLoader.h"
#include "Extra DirectXLibs\Inc\SpriteFont.h"
#include "Extra DirectXLibs\Inc\SpriteBatch.h"
#include "Extra DirectXLibs\Inc\SimpleMath.h"

using namespace DirectX;



#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")



class RenderEngine{ //DENNA KLASSEN ?R ABSTRAKT - g?r inte instantiera

public:
	HWND hWindow;
	RenderEngine(HINSTANCE hInstance, std::string name, UINT scrW, UINT scrH);
	virtual ~RenderEngine();

	int Run();
	int bajs = 2;
	
	void SetViewport();
	virtual bool Init();
	void Update(float dt);
	void Render();
	virtual void Release();
	void Fonts();
	void fpscounter();
	
	OBJ theOBJ;
	Physics thePhysics;
	Collision theCollision;
	GameTimer gTimer;
	Counters gCounter;
	PlayerObject* theCharacter;
	std::wstring mainwname;

	//Import Functions
	void ImportObj(char* geometryFileName, char* materialFileName, ID3D11Device* gDev, bool player);// , bool isStatic, XMMATRIX startPosMatrix);
	//Struct for objLoader
	struct GameObjects{
		int gameObjectIndex;
		ID3D11Buffer* gVertexBuffer;
		//ID3D11Buffer* gIndexBuffer;
		UINT nrElements = 0;
		BoundingOrientedBox bbox;

		char* ObjName;
		std::vector<XMFLOAT3> verteciesPos;
		std::vector<float> verteciesPosX;
		std::vector<float> verteciesPosY;
		std::vector<float> verteciesPosZ;
		std::vector<int> verteciesIndex;
	
	};
	

	
	std::vector<Platform> gamePlatforms;
	std::vector<GameObject> gameObjects;

	//Tempor?r geometry
	void CreatePlaneData();
	void Shaders();
	void TextureFunc();

	// Message handler
	LRESULT MsgProc(HWND hwindow, UINT msg, WPARAM wParam, LPARAM lParam);


	//FONTS AND BATCHES
	std::unique_ptr<DirectX::SpriteFont> spritefont;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;

	struct World
	{
		XMFLOAT4X4 View;
		XMFLOAT4X4 Projection;
		XMFLOAT4X4 WorldSpace;
		XMFLOAT4X4 InvWorld;
	};

	
protected:
	 //handle till f?nstret
	HINSTANCE hInstance;

	std::string applicationName;
	UINT screen_Width;
	UINT screen_Height;
	UINT fpsDisplay=0;
	DWORD windowStyle; //se konstruktorn

	bool InitWindow();
	bool InitDirect3D(HWND hWindow);

	ID3D11Device* gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	ID3D11RenderTargetView* gBackRufferRenderTargetView = nullptr;
	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11DepthStencilView* gdepthStencilView = nullptr;
	ID3D11Texture2D* depthStencilBuffer = nullptr;

	ID3D11Buffer* gVertexBuffer = nullptr;
	ID3D11Buffer* gVertexBuffer2 = nullptr;
	ID3D11Buffer* gWorld;
	ID3D11Buffer* cWorld;

	ID3D11ShaderResourceView* gTextureView = nullptr;
	ID3D11ShaderResourceView* ddsTex1 = nullptr;

	ID3D11InputLayout* gVertexLayout = nullptr;
	ID3D11VertexShader* gVertexShader = nullptr;
	ID3D11PixelShader* gPixelShader = nullptr;
	ID3D11VertexShader* gWireFrameVertexShader = nullptr;
	ID3D11PixelShader* gWireFramePixelShader = nullptr;
	ID3D11InputLayout* gWireFrameLayout = nullptr;

	ID3D11GeometryShader* gGeometryShader = nullptr;

};