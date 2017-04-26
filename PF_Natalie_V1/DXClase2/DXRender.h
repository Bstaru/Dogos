#ifndef _DXRender_
#define _DXRender_

#include "sonido.h"
#include "Constantes.h"
#include "Camara.h"
#include "MeshPrimitives.h"

class DXRender
{
private:
	//Global Declarations//
	IDXGISwapChain* SwapChain;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;

	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 DirectInput;

	D3DXMATRIX View;
	D3DXMATRIX Projection;

	MPrimitives *esfera1;
	MPrimitives *esfera2;
	MPrimitives *esfera3;
	MPrimitives *esferass[50];

	D3DXVECTOR3 from[50];
	D3DXVECTOR3 to[50], Position;

	//modelos
	MPrimitives *gato, *gatito,*gatito2, *gatito3, *gatito4, *gatito5, *dogo, *doge,
				*nave,*nave2,
				*bala, *pla1, *pla2, *pla3, *pla4;
	//billboard
	MPrimitives *water, *arbol1, *arbol2, *arbol3, *arbol4, *arbol5, *arbol6, *arbol7, *arbol8, *arbol9, *arbol10,
				*carne, *poio, *pescao, *Start,
		        *interiornave, *nada ,*enter, *teclas, *volver;//gui
	//demas
	MPrimitives *skydomo0, *skydomo, *skydomo2;
	MPrimitives *terreno, *terreno2;

	Camara *camara, *skyCamara;
	Sonido *sonido;

	D3DXVECTOR3 BoundingSphereTranslation,navecol, colcat1, colcat2, colcat3, colcat4, colcat5;
	D3DXVECTOR3 DummyBoundyEvents;
	D3DXVECTOR3 LastBoundingPosition;

	D3DXVECTOR3 Right, Forward, DefaultRight, DefaultForward, Up;

	float rotPerFrame, rotPerFrameSky, rot, rot2, soundTolerance, movYcosas,
		 balaMove, maxSoundTolerance;
	
	bool movCY, balaGo, inside,cat1,cat2,cat3,cat4,cat5;

public:
	int level, naveSelected;
	float  speed, timer, timerBala;
	DXRender()
	{
		level = 0;
		naveSelected = 1;
		maxSoundTolerance = 1.0f;
		timer = timerBala = 0;
		rotPerFrame = rotPerFrameSky = movYcosas = 0.0f;
		movCY = balaGo = inside =  false;
		cat1 = cat2 = cat3 = cat4 = cat5 = true;
		balaMove = 0.05f;

		soundTolerance = maxSoundTolerance;

		BoundingSphereTranslation = D3DXVECTOR3(0, 0, 0);
		navecol = D3DXVECTOR3(0, 0, 0);
		colcat2 = D3DXVECTOR3(0, 0, 0);
		colcat3 = D3DXVECTOR3(0, 0, 0);
		colcat4 = D3DXVECTOR3(0, 0, 0);
		colcat5 = D3DXVECTOR3(0, 0, 0);

		DefaultForward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		DefaultRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		Forward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}
	

	bool InitDevice(HWND hwnd, int Width, int Height)
	{
		//Describe our Buffer
		DXGI_MODE_DESC bufferDesc;

		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

		bufferDesc.Width = Width;
		bufferDesc.Height = Height;
		bufferDesc.RefreshRate.Numerator = 60;
		bufferDesc.RefreshRate.Denominator = 1;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		//Describe our SwapChain
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		//Create our SwapChain
		D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
			D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

		//Create our BackBuffer
		ID3D11Texture2D* BackBuffer;
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

		//Create our Render Target
		d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
		BackBuffer->Release();

		//Crea la descripcion del raster, el cual determina como se dibujaran los poligonos
		D3D11_RASTERIZER_DESC rasterDesc;
		
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_NONE;//se puede manipular para transparencias, cambiar a D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0;

		d3d11Device->CreateRasterizerState(&rasterDesc, &rasterState);
		d3d11DevCon->RSSetState(rasterState);

		//Describe our Depth/Stencil Buffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ID3D11Texture2D* depthStencilBuffer;

		depthStencilDesc.Width     = Width;
		depthStencilDesc.Height    = Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0; 
		depthStencilDesc.MiscFlags      = 0;

		//Create the Depth/Stencil View
		d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
		d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

		//Set our Render Target
		d3d11DevCon->OMSetRenderTargets( 1, &renderTargetView, depthStencilView );
		Todo();

		sonido = new Sonido();
		sonido->Initialize(hwnd);
		sonido->Play("Sound/redv_rr.wav");

		return true;
	}

	void Todo() {

	/*	esfera1 = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 1.0f, L"Texturas/escudo.jpg", PS_MULTITEXTURE);
		esfera1->setNormalMap(L"Texturas/arena.jpg");
		esfera2 = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 1.0f, L"Texturas/escudo.jpg", PS_SPECULAR);
		esfera3 = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 1.0f, L"Texturas/escudo.jpg", PS_NORMALMAP);
		esfera3->setNormalMap(L"Texturas/escudoNormal.jpg");*/
		
		skydomo = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 1000.0f, L"Texturas/sky.jpg", PS_DIFFUSE);
		enter = new MPrimitives(d3d11Device, d3d11DevCon, 1, 1, L"Texturas/enter.png", PS_DIFFUSE, camara);
		teclas = new MPrimitives(d3d11Device, d3d11DevCon, 1, 1, L"Texturas/teclas.png", PS_DIFFUSE, camara);
		pla1 = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 2.0f, L"Texturas/jupiter.jpg", PS_SPECULAR);
		pla2 = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 2.0f, L"Texturas/saturn.jpg", PS_SPECULAR);
		pla3 = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 2.0f, L"Texturas/planeta.png", PS_SPECULAR);
		pla4 = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 2.0f, L"Texturas/planeta2.png", PS_SPECULAR);

		nave = new MPrimitives(d3d11Device, d3d11DevCon, "Models/nave.txt", L"Texturas/nave.png", PS_SPECULAR);
		nave2 = new MPrimitives(d3d11Device, d3d11DevCon, "Models/gato.txt", L"Texturas/gato.png", PS_DIFFUSE);

		gato = new MPrimitives(d3d11Device, d3d11DevCon, "Models/gato.txt", L"Texturas/gato.png", PS_DIFFUSE);
		gatito = new MPrimitives(d3d11Device, d3d11DevCon, "Models/gato.txt", L"Texturas/gato2.png", PS_DIFFUSE);
		gatito2 = new MPrimitives(d3d11Device, d3d11DevCon, "Models/gato.txt", L"Texturas/gato.png", PS_DIFFUSE);
		gatito3 = new MPrimitives(d3d11Device, d3d11DevCon, "Models/gato.txt", L"Texturas/gato2.png", PS_DIFFUSE);
		gatito4 = new MPrimitives(d3d11Device, d3d11DevCon, "Models/gato.txt", L"Texturas/gato.png", PS_DIFFUSE);
		gatito5 = new MPrimitives(d3d11Device, d3d11DevCon, "Models/gato.txt", L"Texturas/gato2.png", PS_DIFFUSE);
		dogo = new MPrimitives(d3d11Device, d3d11DevCon, "Models/dog.txt", L"Texturas/dogo.png", PS_DIFFUSE);
		doge = new MPrimitives(d3d11Device, d3d11DevCon, "Models/dog.txt", L"Texturas/dogo2.png", PS_DIFFUSE);
		bala = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 1.0f, L"Texturas/Bullet.jpg", PS_SPECULAR);
		water = new MPrimitives(d3d11Device, d3d11DevCon, 300.0f, 300.0f, false, L"Texturas/water.jpg", PS_DIFFUSE);
		arbol1 = new MPrimitives(d3d11Device, d3d11DevCon, 4.31f, 4.13, true, L"Texturas/tree1.png", PS_DIFFUSE);
		arbol2 = new MPrimitives(d3d11Device, d3d11DevCon, 4.66f, 5.82f, true, L"Texturas/tree2.png", PS_DIFFUSE);
		arbol3 = new MPrimitives(d3d11Device, d3d11DevCon, 4.98f, 6.88f, true, L"Texturas/tree3.png", PS_DIFFUSE);
		arbol4 = new MPrimitives(d3d11Device, d3d11DevCon, 4.31f, 4.13, true, L"Texturas/tree1.png", PS_DIFFUSE);
		arbol5 = new MPrimitives(d3d11Device, d3d11DevCon, 4.66f, 5.82f, true, L"Texturas/tree2.png", PS_DIFFUSE);
		arbol6 = new MPrimitives(d3d11Device, d3d11DevCon, 4.31f, 4.13, true, L"Texturas/tree1.png", PS_DIFFUSE);
		arbol7 = new MPrimitives(d3d11Device, d3d11DevCon, 4.66f, 5.82f, true, L"Texturas/tree2.png", PS_DIFFUSE);
		arbol8 = new MPrimitives(d3d11Device, d3d11DevCon, 4.98f, 6.88f, true, L"Texturas/tree3.png", PS_DIFFUSE);
		arbol9 = new MPrimitives(d3d11Device, d3d11DevCon, 4.98f, 6.88f, true, L"Texturas/tree2.png", PS_DIFFUSE);
		arbol10 = new MPrimitives(d3d11Device, d3d11DevCon, 4.98f, 6.88f, true, L"Texturas/tree3.png", PS_DIFFUSE);
		carne = new MPrimitives(d3d11Device, d3d11DevCon, 2.5475f, 2.175f, true, L"Texturas/carne.png", PS_DIFFUSE);
		poio = new MPrimitives(d3d11Device, d3d11DevCon, 2.5475f, 2.175f, true, L"Texturas/pollo.png", PS_DIFFUSE);
		pescao = new MPrimitives(d3d11Device, d3d11DevCon, 2.5475f, 2.175f, true, L"Texturas/pez.png", PS_DIFFUSE);
		interiornave = new MPrimitives(d3d11Device, d3d11DevCon, 1, 1, L"Texturas/naveinterior.png", PS_DIFFUSE, camara);
		volver = new MPrimitives(d3d11Device, d3d11DevCon, 1, 1, L"Texturas/backspace.png", PS_DIFFUSE, camara);
		nada = new MPrimitives(d3d11Device, d3d11DevCon, 1, 1, L"Texturas/nada.png", PS_DIFFUSE, camara);
		skydomo2 = new MPrimitives(d3d11Device, d3d11DevCon, 32, 32, 1000.0f, L"Texturas/space.jpg", PS_DIFFUSE);
		terreno = new MPrimitives(d3d11Device, d3d11DevCon, 100.0f, L"Texturas/sand.jpg", "Texturas/Heightmap01.bmp", PS_MULTITEXTURE);
		terreno->setNormalMap(L"Texturas/grass.jpg");

		terreno2 = new MPrimitives(d3d11Device, d3d11DevCon, 100.0f, L"Texturas/grass.jpg", "Texturas/Heightmap.bmp", PS_MULTITEXTURE);
		terreno2->setNormalMap(L"Texturas/sand.jpg");
	}

	bool InitDirectInput(HINSTANCE hInstance, HWND hwnd)
	{
		DirectInput8Create(hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&DirectInput,
			NULL); 

		DirectInput->CreateDevice(GUID_SysKeyboard,
			&DIKeyboard,
			NULL);

		DirectInput->CreateDevice(GUID_SysMouse,
			&DIMouse,
			NULL);

		DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
		DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		DIMouse->SetDataFormat(&c_dfDIMouse);
		DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

		return true;
	}

	void DetectInput()
	{
		DIMOUSESTATE mouseCurrState;
		BYTE keyboardState[256];

		DIKeyboard->Acquire();
		DIMouse->Acquire();
		DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
		DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

		if (level < 1)
			speed = 0.75f;
		else
		{
			if (timer > 0)
				speed = 0.625f;
			else
				speed = 0.125f;
		}

		float speed = 0.2f;
		float moveLeftRight = 0;
		float moveBackForward = 0;
		float camYaw = 0;
		float camPitch = 0;	
		float camZoom = 0;
		float smoothness = 0.001f;


		if (keyboardState[DIK_RETURN])
		{
			this->level = 1;
		}

		if (keyboardState[DIK_BACKSPACE])
		{
			this->level = 0;
		}

		if (soundTolerance > maxSoundTolerance)
		{
			if (keyboardState[DIK_M]){
				soundTolerance = 0;
				sonido->Play("Sound/laser.wav");
			}
		}

		if(keyboardState[DIK_A])
		{
			moveLeftRight -= speed;
		}
		if(keyboardState[DIK_D])
		{
			moveLeftRight += speed;
		}
		if(keyboardState[DIK_W])
		{
			moveBackForward += speed;
		}
		if(keyboardState[DIK_S])
		{
			moveBackForward -= speed;
		}

		/*if (keyboardState[DIK_LEFT])
		{
			DummyBoundyEvents.x = -.01f;
		}
		if (keyboardState[DIK_RIGHT])
		{
			DummyBoundyEvents.x = .01f;
		}
		if (keyboardState[DIK_UP])
		{
			DummyBoundyEvents.y = .01f;
		}
		if (keyboardState[DIK_DOWN])
		{
			DummyBoundyEvents.y = -.01f;
		}
		if (keyboardState[DIK_Q])
		{
			DummyBoundyEvents.z = .01f;
		}
		if (keyboardState[DIK_E])
		{
			DummyBoundyEvents.z = -.01f;
		}*/
	
		if (keyboardState[DIK_C])
		{
			inside = true;
		}
		if (keyboardState[DIK_V])
		{
			inside = false;
		}
		if (keyboardState[DIK_Y])
		{
			level = 2;
		}
		if((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
		{
			camYaw = mouseCurrState.lX * smoothness;
			camPitch = mouseCurrState.lY * smoothness;
			mouseLastState = mouseCurrState;
		}
		if (soundTolerance > maxSoundTolerance)
		{
			if (mouseCurrState.rgbButtons[0] & 0x80)
			{
				soundTolerance = 0;
				sonido->Play("Sound/laser.wav");

				if (balaGo == false)
				{
					balaGo = true;
					timerBala = 15;

					Position = nave->getPosition();

					D3DXMATRIX RotationMatrix;
					D3DXMatrixRotationYawPitchRoll(&RotationMatrix, camara->Yaw, camara->Pitch, 0);
					D3DXVec3TransformCoord(&Right, &DefaultRight, &RotationMatrix);
					D3DXVec3TransformCoord(&Forward, &DefaultForward, &RotationMatrix);
					D3DXVec3Cross(&Up, &Forward, &Right);
				}
			}
		}

		camara->setDistance(camara->distance + (mouseCurrState.lZ * 0.001f));

		if (inside == false) {
			camara->UpdateCameraTPS(moveBackForward, moveLeftRight, camYaw, camPitch);
			skyCamara->setSkyCamara(camara);
		}
		
		else if (inside == true) {
			camara->UpdateCameraFree(moveBackForward, moveLeftRight, camYaw, camPitch);
			skyCamara->setSkyCamara2(camara);
		}

		if (keyboardState[DIK_J])
		{
			naveSelected = 1;
		}
		if (keyboardState[DIK_K])
		{
			naveSelected = 2;
		}
	}

	void ReleaseObjects()
	{
		//Release the COM Objects we created
		SwapChain->Release();
		d3d11Device->Release();
		d3d11DevCon->Release();
		renderTargetView->Release();
		depthStencilView->Release();
	}

	bool InitScene(int Width, int Height)
	{
		//Create the Viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = Width;
		viewport.Height = Height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		//Set the Viewport
		d3d11DevCon->RSSetViewports(1, &viewport);

		//Create the camera
		D3DXVECTOR3 Position = D3DXVECTOR3(115.0f,0.0f, 205.0f);
		D3DXVECTOR3 Target = D3DXVECTOR3(115.0f, 5.0f, 205.0f);
		D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		camara = new Camara(Position,Target,Up, Width, Height);
		skyCamara = new Camara(Position,Target,Up, Width, Height);

		return true;
	}

	void UpdateScene() {
		Movs();
		switch (this->level)
		{
		case 0:
			Update0();
			break;
		case 1:
			Update1();
			break;
		case 2:
			Update2();
			break;
		}
	}
	void DrawScene() {
		//Clear our backbuffer to the updated color
		D3DXCOLOR bgColor(0.0f, 0.0f, 0.25f, 1.0f);
		d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
		//Refresh the Depth/Stencil view
		d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		d3d11DevCon->RSSetState(rasterState);

		switch (this->level)
		{
		case 0:
			Draw0();
			break;
		case 1:
			Draw1();
			break;		
		case 2:
			Draw2();
			break;
		}
	
		//Present the backbuffer to the screen
		SwapChain->Present(0, 0);
	}

	void Update0()
	{
		LastBoundingPosition = camara->Target;
		//BoundingSphereTranslation += DummyBoundyEvents;

		rotPerFrame += 0.001f;
		rotPerFrameSky += 0.01f;

		soundTolerance += 0.01f;

		rotPerFrame += 0.1f;
		rot = 90.0f;

		D3DXMATRIX  escala, scale, rotX, rotMatY, westMat, eastMat, northMat, southMat,
				movplaneta, movplaneta2, movplaneta3, movplaneta4;

		D3DXMatrixRotationY(&rotMatY, D3DXToRadian(rotPerFrame));
		D3DXMatrixRotationX(&rotX, D3DXToRadian(rot));
		D3DXMatrixTranslation(&eastMat, 3, 0, 0);
		D3DXMatrixTranslation(&westMat, -3, 0, 0);
		D3DXMatrixTranslation(&northMat, 0, 0, 3);
		D3DXMatrixTranslation(&southMat, 0, 0, -3);

		D3DXMatrixTranslation(&movplaneta, 115.0f, 0.0f, 205.0f);
		D3DXMatrixTranslation(&movplaneta2, 115.0f, 5.0f, 205.0f);
		D3DXMatrixTranslation(&movplaneta3, 115.0f, 10.0f, 205.0f);
		D3DXMatrixTranslation(&movplaneta4, 115.0f, -5.0f, 205.0f);
		
		pla1->UpdateScene(rotX*westMat*westMat*westMat*westMat*rotMatY*movplaneta);
		pla2->UpdateScene(rotX*eastMat*eastMat*eastMat*eastMat*rotMatY*movplaneta2);
		pla3->UpdateScene(rotX*northMat*northMat*northMat*northMat*northMat*rotMatY*movplaneta3);
		pla4->UpdateScene(rotX*southMat*southMat*southMat*southMat*southMat*rotMatY*movplaneta4);

		//skydomo
		D3DXMATRIX skyRotMat, skyRotMatZ;
		D3DXMatrixRotationX(&skyRotMat, D3DXToRadian(-90));
		D3DXMatrixRotationZ(&skyRotMatZ, D3DXToRadian(rotPerFrameSky));
		skydomo2->UpdateScene(skyRotMatZ * skyRotMat);

	}
	void Draw0()
	{
		//Clear our backbuffer to the updated color
		D3DXCOLOR bgColor(0.0f, 0.0f, 0.25f, 1.0f);
		d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
		//Refresh the Depth/Stencil view
		d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		d3d11DevCon->RSSetState(rasterState);

		pla1->DrawScene(camara);
		pla2->DrawScene(camara);
		pla3->DrawScene(camara);
		pla4->DrawScene(camara);
		skydomo2->DrawScene(skyCamara);

		enter->DrawBitmap(camara, D3DXVECTOR2(.3, .7), D3DXVECTOR2(0,-.65));
		teclas->DrawBitmap(camara, D3DXVECTOR2(1.5, 1.5), D3DXVECTOR2(0, .2));

		//Present the backbuffer to the screen
		SwapChain->Present(0, 0);
	}

	void Update1()
	{
		LastBoundingPosition = BoundingSphereTranslation;
		BoundingSphereTranslation += DummyBoundyEvents;

		rotPerFrame += 0.1f;
		rotPerFrameSky += 0.01f;

		///gatos saltarines :V
		if (movYcosas >= 5.0f) {
			movCY = true;
		}
		if (movYcosas <= 0.0f) {
			movCY = false;
		}
		if (movCY) {
			movYcosas -= 0.3f;
		}
		else {
			movYcosas += 0.3f;
		}
		///gatos saltarines :V fin

		soundTolerance += 0.01f;

		rotPerFrame += 0.1f;
		rot = 90.0f;
		rot2 = -90.0f;

		D3DXMATRIX  escala, scale, rotX, rotY, rotZ, rotMatY, rotMatX, rotMatZ,
			northMat, westMat, eastMat, southMat, norte, sur, este, oeste,
			palFondo, palFondo2;
		D3DXMatrixRotationY(&rotMatY, D3DXToRadian(rotPerFrame));
		D3DXMatrixRotationX(&rotMatX, D3DXToRadian(rotPerFrame));
		D3DXMatrixRotationZ(&rotMatZ, D3DXToRadian(rotPerFrame));
		D3DXMatrixRotationY(&rotY, D3DXToRadian(rot));
		D3DXMatrixRotationX(&rotX, D3DXToRadian(rot));
		D3DXMatrixRotationZ(&rotZ, D3DXToRadian(rot));
		D3DXMatrixTranslation(&northMat, 0, 3, 0);
		D3DXMatrixTranslation(&norte, 0, 1, 0);
		D3DXMatrixTranslation(&southMat, 0, -3, 0);
		D3DXMatrixTranslation(&sur, 0, -1, 0);
		D3DXMatrixTranslation(&eastMat, 3, 0, 0);
		D3DXMatrixTranslation(&este, 1, 0, 0);
		D3DXMatrixTranslation(&westMat, -3, 0, 0);
		D3DXMatrixTranslation(&oeste, -1, 0, 0);
		D3DXMatrixTranslation(&palFondo, 0, 0, 3);
		D3DXMatrixTranslation(&palFondo2, 0, 0, 1);
		D3DXMatrixScaling(&scale, 0.03f, 0.03f, 0.03f);
		D3DXMatrixScaling(&escala, 0.5f, 0.5f, 0.53f);

		gato->UpdateScene(westMat*westMat*westMat*northMat*escala*escala);
		dogo->UpdateScene(westMat*westMat*westMat*southMat*escala*escala);

		///gatos saltarines :V
		D3DXMATRIX movCosaY, gato1Pos, gato2Pos, gato3Pos, gato4Pos, gato5Pos, escalaG;
		D3DXMatrixTranslation(&movCosaY, 0, movYcosas, 0);
		D3DXMatrixScaling(&escalaG, 0.15f, 0.15f, 0.15f);

		D3DXMatrixTranslation(&gato1Pos, 215.0f, 27.0f, 205.0f);
		D3DXMatrixTranslation(&gato2Pos, 1400.0f, 80.0f, 260.0f);
		D3DXMatrixTranslation(&gato3Pos, 1410.0f, 65.5f, 1395.0f);
		D3DXMatrixTranslation(&gato4Pos, 270.0f, 100.5f, 1355.0f);
		D3DXMatrixTranslation(&gato5Pos, 750.0f, 90.0f, 970.0f);

		gatito->UpdateScene(movCosaY*gato1Pos*escalaG);
		gatito2->UpdateScene(movCosaY*gato2Pos*escalaG);
		gatito3->UpdateScene(movCosaY*gato3Pos*escalaG);
		gatito4->UpdateScene(movCosaY*gato4Pos*escalaG);
		gatito5->UpdateScene(movCosaY*gato5Pos*escalaG);	


		if (nave->getPosition().z >= 128)
			nave->Position.z = 128;
		if (nave->getPosition().z <= -128)
			nave->Position.z = -128;
		if (nave->getPosition().x >= 128)
			nave->Position.x = 128;
		if (nave->getPosition().x <= -128)
			nave->Position.x = -128;
		if (nave->getPosition().y >= 128)
			nave->Position.y = 128;
		if (nave->getPosition().y <= -128)
			nave->Position.y = -128;

	/*	nave->PrimitiveBoundings->PositionM = nave->getPosition();
		nave->PrimitiveBoundings->Radius = 30.0f;

		gatito->PrimitiveBoundings->PositionM = gatito->getPosition();
		gatito->PrimitiveBoundings->Radius = 5.0f;
		
		if (CheckBoundingSphereCollision(
			nave->PrimitiveBoundings->Radius,
			nave->PrimitiveBoundings->PositionM, 
			nave->getWorldMatrix(),
			gatito->PrimitiveBoundings->Radius,
			gatito->PrimitiveBoundings->PositionM, 
			gatito->getWorldMatrix())) {
			cat1 = false;
		}*/
		//setColision(nave, gatito);
		//doges peques :v
		doge->UpdateScene(eastMat*eastMat*eastMat*southMat*escala*escala*escala);

		//nave
		D3DXMATRIX objT,objP, NavScale, camTras, camRot, lugarCol;	
		D3DXMatrixTranslation(&objT, 0,-5, 150);
		D3DXMatrixTranslation(&objP, 0, -5, -150);
		D3DXMatrixScaling(&NavScale, 0.05f, 0.05f, 0.05f);
		D3DXMatrixRotationYawPitchRoll(&camRot, camara->Yaw, camara->Pitch, 0);
		D3DXMatrixTranslation(&lugarCol, navecol.x, navecol.y, navecol.z);

		switch (naveSelected)
		{
		case 1:
			if (inside == false) {
				D3DXMatrixTranslation(&camTras, camara->Target.x, camara->Target.y, camara->Target.z);
				nave->UpdateScene(objT*NavScale*camRot*camTras);
			}
			else if (inside == true) {
				D3DXMatrixTranslation(&camTras, camara->Position.x, camara->Position.y, camara->Position.z);
				nave->UpdateScene(objP*NavScale*camRot*camTras);
			}
			break;
		case 2:
			if (inside == false) {
				D3DXMatrixTranslation(&camTras, camara->Target.x, camara->Target.y, camara->Target.z);
				nave2->UpdateScene(objT*NavScale*camRot*camTras);
			}
			else if (inside == true) {
				D3DXMatrixTranslation(&camTras, camara->Position.x, camara->Position.y, camara->Position.z);
				nave2->UpdateScene(objP*NavScale*camRot*camTras);
			}
			break;
		}

		if (balaGo == true)
		{
			D3DXMATRIX moverBala;
			Position += balaMove*Forward;
			D3DXMatrixTranslation(&moverBala, Position.x, Position.y, Position.z);
			bala->UpdateScene(moverBala);

			bala->PrimitiveBoundings->PositionM = D3DXVECTOR3(Position.x, Position.y, Position.z);
			bala->PrimitiveBoundings->Radius = 2.0f;
		}

		//aguita
		D3DXMATRIX agualoc;
		D3DXMatrixTranslation(&agualoc, 128, 0, 128);
		water->UpdateScene(rotX*norte*norte*agualoc);

		//arboles
		D3DXMATRIX postree1,postree2, postree3, postree4, postree5, postree6, postree7, postree8, postree9, postree10;
		D3DXMatrixTranslation(&postree1, 210.0f, 14.0f, 35.0f); 
		D3DXMatrixTranslation(&postree2, 210.0f, 11.0f, 200.0f);
		D3DXMatrixTranslation(&postree3, 130.0f, 6.0f, 26.0f);
		D3DXMatrixTranslation(&postree4, 45.0f, 17.0f, 200.0f);
		D3DXMatrixTranslation(&postree5, 45.0f, 16.0f, 35.0f);
		D3DXMatrixTranslation(&postree6, 120, 15.0f, 120);
		D3DXMatrixTranslation(&postree7, 130, 16.0f, 130);
		D3DXMatrixTranslation(&postree8, 128, 16.0f, 140);
		D3DXMatrixTranslation(&postree9, 110, 16.0f, 128);
		D3DXMatrixTranslation(&postree10, 128, 16.0f, 145);

		arbol1->UpdateScene(postree1);
		arbol2->UpdateScene(postree2);
		arbol3->UpdateScene(postree3);
		arbol4->UpdateScene(postree4);
		arbol5->UpdateScene(postree5);
		arbol6->UpdateScene(postree6);
		arbol7->UpdateScene(postree7);
		arbol8->UpdateScene(postree8);
		arbol9->UpdateScene(postree9);
		arbol10->UpdateScene(postree10);

		//skydomo
		D3DXMATRIX skyRotMat, skyRotMatZ;
		D3DXMatrixRotationX(&skyRotMat, D3DXToRadian(-90));
		D3DXMatrixRotationZ(&skyRotMatZ, D3DXToRadian(rotPerFrameSky));
		skydomo->UpdateScene(skyRotMatZ * skyRotMat);

		//COLISIONES DEL TERRENO
		if (camara->Target.y <= terreno->getTerrainCollision(camara->Target)) {

			camara->Target.y = terreno->getTerrainCollision(camara->Target) + 1; //colision terreno 3ra
		}
		if (camara->Position.y <= terreno->getTerrainCollision(camara->Position)) {

			camara->Position.y = terreno->getTerrainCollision(camara->Position) + 5; //colision terreno 1ra
		}		

		//colision esferas ejemplo																	
	/*	D3DXMATRIX Bounding1, Bounding2, BoundinMatTranslation, MatAcomodo;
		D3DXMatrixTranslation(&MatAcomodo, 50, 3, 50);
		D3DXMatrixTranslation(&Bounding1, 3, 6, 0);
		D3DXMatrixTranslation(&Bounding2, -3, 6, 0);
		D3DXMatrixTranslation(&BoundinMatTranslation, BoundingSphereTranslation.x, BoundingSphereTranslation.y, BoundingSphereTranslation.z);
		
		esfera1->UpdateScene(rotMatY);
		esfera2->UpdateScene(Bounding1 * BoundinMatTranslation);
		esfera3->UpdateScene(Bounding2);
		setColision(esfera1, esfera2);
		setColision(esfera1, esfera3);

		DummyBoundyEvents.x = 0;
		DummyBoundyEvents.y = 0;
		DummyBoundyEvents.z = 0;*/

	}
	void Draw1()
	{
		//Clear our backbuffer to the updated color
		D3DXCOLOR bgColor(0.0f, 0.0f, 0.25f, 1.0f);
		d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
		//Refresh the Depth/Stencil view
		d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		d3d11DevCon->RSSetState(rasterState);
		switch (naveSelected)
		{
		case 1:
			nave->DrawScene(camara);
			break;
		case 2:
			nave2->DrawScene(camara);
			break;
		}

		/*esfera1->DrawScene(camara);
		esfera2->DrawScene(camara);
		esfera3->DrawScene(camara);*/

		//if (camara->Target == D3DXVECTOR3(215.0f, 27.0f, 205.0f) || camara->Position == D3DXVECTOR3(215.0f, 27.0f, 205.0f)) {
		//	cat1 = false;
		//}

		if (cat1 == true) {
			gatito->DrawScene(camara);
		}
		if (cat2 == true) {
			gatito2->DrawScene(camara);
		}if (cat3 == true) {
			gatito3->DrawScene(camara);
		}if (cat4 == true) {
			gatito4->DrawScene(camara);
		}if (cat5 == true) {
			gatito5->DrawScene(camara);
		}

		water->DrawScene(camara);

		arbol1->DrawScene(camara);
		arbol2->DrawScene(camara);
		arbol3->DrawScene(camara);
		arbol4->DrawScene(camara);
		arbol5->DrawScene(camara);
		arbol6->DrawScene(camara);
		arbol7->DrawScene(camara);
		arbol8->DrawScene(camara);
		arbol9->DrawScene(camara);
		arbol10->DrawScene(camara);
		terreno->DrawScene(camara);
		skydomo->DrawScene(skyCamara);

		if (inside == false)
		{
			nada->DrawBitmap(camara, D3DXVECTOR2(2, .5), D3DXVECTOR2(0, -.75));
		}
		else if (inside == true)
		{
			interiornave->DrawBitmap(camara, D3DXVECTOR2(2, .5), D3DXVECTOR2(0, -.75));
		}		

		if (balaGo == true)
			bala->DrawScene(camara);

		volver->DrawBitmap(camara, D3DXVECTOR2(.3, .5), D3DXVECTOR2(.75,.75));

		//Present the backbuffer to the screen
		SwapChain->Present(0, 0);
	}

	void Update2()
	{
		LastBoundingPosition = BoundingSphereTranslation;
		BoundingSphereTranslation += DummyBoundyEvents;

		rotPerFrame += 0.1f;
		rotPerFrameSky += 0.01f;

		///gatos saltarines :V
		if (movYcosas >= 5.0f) {
			movCY = true;
		}
		if (movYcosas <= 0.0f) {
			movCY = false;
		}
		if (movCY) {
			movYcosas -= 0.3f;
		}
		else {
			movYcosas += 0.3f;
		}
		///gatos saltarines :V fin

		soundTolerance += 0.01f;

		rotPerFrame += 0.1f;
		rot = 90.0f;
		rot2 = -90.0f;

		D3DXMATRIX  escala, scale, rotX, rotY, rotZ, rotMatY, rotMatX, rotMatZ,
			northMat, westMat, eastMat, southMat, norte, sur, este, oeste,
			palFondo, palFondo2;
		D3DXMatrixRotationY(&rotMatY, D3DXToRadian(rotPerFrame));
		D3DXMatrixRotationX(&rotMatX, D3DXToRadian(rotPerFrame));
		D3DXMatrixRotationZ(&rotMatZ, D3DXToRadian(rotPerFrame));
		D3DXMatrixRotationY(&rotY, D3DXToRadian(rot));
		D3DXMatrixRotationX(&rotX, D3DXToRadian(rot));
		D3DXMatrixRotationZ(&rotZ, D3DXToRadian(rot));
		D3DXMatrixTranslation(&northMat, 0, 3, 0);
		D3DXMatrixTranslation(&norte, 0, 1, 0);
		D3DXMatrixTranslation(&southMat, 0, -3, 0);
		D3DXMatrixTranslation(&sur, 0, -1, 0);
		D3DXMatrixTranslation(&eastMat, 3, 0, 0);
		D3DXMatrixTranslation(&este, 1, 0, 0);
		D3DXMatrixTranslation(&westMat, -3, 0, 0);
		D3DXMatrixTranslation(&oeste, -1, 0, 0);
		D3DXMatrixTranslation(&palFondo, 0, 0, 3);
		D3DXMatrixTranslation(&palFondo2, 0, 0, 1);
		D3DXMatrixScaling(&scale, 0.03f, 0.03f, 0.03f);
		D3DXMatrixScaling(&escala, 0.5f, 0.5f, 0.53f);



		if (nave->getPosition().z >= 128)
			nave->Position.z = 128;
		if (nave->getPosition().z <= -128)
			nave->Position.z = -128;
		if (nave->getPosition().x >= 128)
			nave->Position.x = 128;
		if (nave->getPosition().x <= -128)
			nave->Position.x = -128;
		if (nave->getPosition().y >= 128)
			nave->Position.y = 128;
		if (nave->getPosition().y <= -128)
			nave->Position.y = -128;


		//nave
		D3DXMATRIX objT, objP, NavScale, camTras, camRot, lugarCol;
		D3DXMatrixTranslation(&objT, 0, -5, 150);
		D3DXMatrixTranslation(&objP, 0, -5, -150);
		D3DXMatrixScaling(&NavScale, 0.05f, 0.05f, 0.05f);
		D3DXMatrixRotationYawPitchRoll(&camRot, camara->Yaw, camara->Pitch, 0);
		D3DXMatrixTranslation(&lugarCol, navecol.x, navecol.y, navecol.z);

		switch (naveSelected)
		{
		case 1:
			if (inside == false) {
				D3DXMatrixTranslation(&camTras, camara->Target.x, camara->Target.y, camara->Target.z);
				nave->UpdateScene(objT*NavScale*camRot*camTras);
			}
			else if (inside == true) {
				D3DXMatrixTranslation(&camTras, camara->Position.x, camara->Position.y, camara->Position.z);
				nave->UpdateScene(objP*NavScale*camRot*camTras);
			}
			break;
		case 2:
			if (inside == false) {
				D3DXMatrixTranslation(&camTras, camara->Target.x, camara->Target.y, camara->Target.z);
				nave2->UpdateScene(objT*NavScale*camRot*camTras);
			}
			else if (inside == true) {
				D3DXMatrixTranslation(&camTras, camara->Position.x, camara->Position.y, camara->Position.z);
				nave2->UpdateScene(objP*NavScale*camRot*camTras);
			}
			break;
		}

		if (balaGo == true)
		{
			D3DXMATRIX moverBala;
			Position += balaMove*Forward;
			D3DXMatrixTranslation(&moverBala, Position.x, Position.y, Position.z);
			bala->UpdateScene(moverBala);

			bala->PrimitiveBoundings->PositionM = D3DXVECTOR3(Position.x, Position.y, Position.z);
			bala->PrimitiveBoundings->Radius = 2.0f;
		}


		//skydomo
		D3DXMATRIX skyRotMat, skyRotMatZ;
		D3DXMatrixRotationX(&skyRotMat, D3DXToRadian(-90));
		D3DXMatrixRotationZ(&skyRotMatZ, D3DXToRadian(rotPerFrameSky));
		skydomo2->UpdateScene(skyRotMatZ * skyRotMat);

		//COLISIONES DEL TERRENO
		if (camara->Target.y <= terreno->getTerrainCollision(camara->Target)) {

			camara->Target.y = terreno->getTerrainCollision(camara->Target) + 1; //colision terreno 3ra
		}
		if (camara->Position.y <= terreno->getTerrainCollision(camara->Position)) {

			camara->Position.y = terreno->getTerrainCollision(camara->Position) + 5; //colision terreno 1ra
		}

	}
	void Draw2()
	{
		//Clear our backbuffer to the updated color
		D3DXCOLOR bgColor(0.0f, 0.0f, 0.25f, 1.0f);
		d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
		//Refresh the Depth/Stencil view
		d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		d3d11DevCon->RSSetState(rasterState);
		switch (naveSelected)
		{
		case 1:
			nave->DrawScene(camara);
			break;
		case 2:
			nave2->DrawScene(camara);
			break;
		}
		if (inside == false)
		{
			nada->DrawBitmap(camara, D3DXVECTOR2(2, .5), D3DXVECTOR2(0, -.75));
		}
		else if (inside == true)
		{
			interiornave->DrawBitmap(camara, D3DXVECTOR2(2, .5), D3DXVECTOR2(0, -.75));
		}

		if (balaGo == true)
			bala->DrawScene(camara);

		volver->DrawBitmap(camara, D3DXVECTOR2(.3, .5), D3DXVECTOR2(.75, .75));
		terreno2->DrawScene(camara);

		//Present the backbuffer to the screen
		SwapChain->Present(0, 0);
	}

	void Movs() {

		switch (this->level)
		{
		case 0:
		case 1:
			//RotElement += 0.1f;
			if (timer > 0)
				timer -= 0.002f;
			if (timerBala > 0)
			{
				timerBala -= 0.02f;
				balaMove += 0.0002;/////////////////////////////////////////////////////////////
			}
			else
			{
				balaGo = false;
				balaMove = 0.05;
			}

			break;

		}
	}

	void setColision(MPrimitives *obj1, MPrimitives *obj2)
	{
		if (CheckBoundingSphereCollision(

			obj1->PrimitiveBoundings->Radius,
			obj1->PrimitiveBoundings->PositionM, obj1->getWorldMatrix(),
			obj2->PrimitiveBoundings->Radius,
			obj2->PrimitiveBoundings->PositionM, obj2->getWorldMatrix())){

			camara->Target = LastBoundingPosition;
		}
	}

	bool CheckBoundingSphereCollision(float firstSphereRadius,
		D3DXVECTOR3 firstSpherePos,
		D3DXMATRIX firstObjWorldSpace,
		float secondSphereRadius,
		D3DXVECTOR3 secondSpherePos,
		D3DXMATRIX secondObjWorldSpace)
	{
		//Declare local variables
		D3DXVECTOR3 world_1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 world_2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float objectsDistance = 0.0f;

		//Transform the objects world space to objects REAL center in world space

		D3DXVec3TransformCoord(&world_1, &firstSpherePos, &firstObjWorldSpace);
		D3DXVec3TransformCoord(&world_2, &secondSpherePos, &secondObjWorldSpace);

		// Create Vector to verify distance
		world_1 -= world_2;

		//Get the distance between the two objects
		objectsDistance = D3DXVec3Length(&world_1);

		//If the distance between the two objects is less than the sum of their bounding spheres...
		if (objectsDistance <= (firstSphereRadius + secondSphereRadius))
			return true;

		//If the bounding spheres are not colliding, return false
		return false;
	}
};


#endif