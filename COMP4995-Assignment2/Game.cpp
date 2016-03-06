#include "Includes.h"

Game::Game() {
	
}

Game::Game(HWND hWnd) {
	this->hWndMain = hWnd;
	
}

Game::~Game() {

}

int Game::GameInit() {
	HRESULT r = 0;//return values
	D3DSURFACE_DESC desc;
	LPDIRECT3DSURFACE9 pSurface = 0;
	LPD3DXBUFFER pD3DXMtrlBuffer, pD3DXMtrlBuffer2;

	this->pD3D = Direct3DCreate9(D3D_SDK_VERSION);//COM object
	if (this->pD3D == NULL) {
		SetError(_T("Could not create IDirect3D9 object"));
		return E_FAIL;
	}
												  // FALSE = full screen TRUE = windowed
	r = InitDirect3DDevice(this->hWndMain, 640, 480, TRUE, D3DFMT_X8R8G8B8, this->pD3D, &this->pDevice);
	if (FAILED(r)) {//FAILED is a macro that returns false if return value is a failure - safer than using value itself
		SetError(_T("Initialization of the device failed"));
		return E_FAIL;
	}

	// This is where I setup the surface
	r = this->pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface);
	if (FAILED(r)) {
		SetError(_T("Couldn't get backbuffer"));
	}
	pSurface->GetDesc(&desc);

	r = this->pDevice->CreateOffscreenPlainSurface(desc.Width, desc.Height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &this->BitmapSurface, NULL);

	r = LoadBitmapToSurface(_T(BITMAP_PICTURE), &pSurface, this->pDevice);
	if (FAILED(r))
		SetError(_T("Error loading bitmap"));

	r = D3DXLoadSurfaceFromSurface(this->BitmapSurface, NULL, NULL, pSurface, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
	if (FAILED(r))
		SetError(_T("did not copy surface"));

	frameController = Frame(pDevice);

	frameController.LoadAlphabet(_T("Alphabet vSmall.bmp"), 8, 16);
	frameController.InitTiming();

	// Load in the objects
	if (FAILED(D3DXLoadMeshFromX(_T("tiger.x"), D3DXMESH_SYSTEMMEM,
		pDevice, NULL,
		&pD3DXMtrlBuffer, NULL, 
		&dwNumMaterials,
		&pMesh)))
	{
		// If model is not in current folder, try parent folder
		if (FAILED(D3DXLoadMeshFromX(_T("..\\tiger.x"), D3DXMESH_SYSTEMMEM,
			pDevice, NULL,
			&pD3DXMtrlBuffer, NULL, &dwNumMaterials,
			&pMesh)))
		{
			MessageBox(NULL, _T("Could not find tiger.x"), _T("Meshes.exe"), MB_OK);
			return E_FAIL;
		}
	}

	// Load in the objects
	if (FAILED(D3DXLoadMeshFromX(_T("Monkey.x"), D3DXMESH_SYSTEMMEM,
		pDevice, NULL,
		&pD3DXMtrlBuffer2, NULL,
		&dwNumMaterials2,
		&pMesh2)))
	{
		// If model is not in current folder, try parent folder
		if (FAILED(D3DXLoadMeshFromX(_T("..\\Monkey.x"), D3DXMESH_SYSTEMMEM,
			pDevice, NULL,
			&pD3DXMtrlBuffer2, NULL, &dwNumMaterials2,
			&pMesh2)))
		{
			MessageBox(NULL, _T("Could not find Monkey.x"), _T("Meshes.exe"), MB_OK);
			return E_FAIL;
		}
	}

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];
	D3DXMATERIAL* d3dxMaterials2 = (D3DXMATERIAL*)pD3DXMtrlBuffer2->GetBufferPointer();
	pMeshMaterials2 = new D3DMATERIAL9[dwNumMaterials2];
	pMeshTextures2 = new LPDIRECT3DTEXTURE9[dwNumMaterials2];

	for (DWORD i = 0; i<dwNumMaterials; i++)
	{
		// Copy the material
		pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;

		pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// Create the texture
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				d3dxMaterials[i].pTextureFilename,
				&pMeshTextures[i])))
			{
				// If texture is not in current folder, try parent folder
				const TCHAR* strPrefix = TEXT("..\\");
				const int lenPrefix = lstrlen(strPrefix);
				TCHAR strTexture[MAX_PATH];
				lstrcpyn(strTexture, strPrefix, MAX_PATH);
				lstrcpyn(strTexture + lenPrefix, d3dxMaterials[i].pTextureFilename, MAX_PATH - lenPrefix);
				// If texture is not in current folder, try parent folder
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					strTexture,
					&pMeshTextures[i])))
				{
					MessageBox(NULL, _T("Could not find texture map"), _T("Meshes.exe"), MB_OK);
				}
			}
		}
	}

	// Create normals for the tiger
	pMesh->CloneMeshFVF(D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, pDevice, &pMesh);
	if (FAILED(D3DXComputeNormals(pMesh, NULL))) {
		return E_FAIL;
	}

	for (DWORD i = 0; i<dwNumMaterials2; i++)
	{
		// Copy the material
		pMeshMaterials2[i] = d3dxMaterials2[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		pMeshMaterials2[i].Ambient = pMeshMaterials2[i].Diffuse;

		pMeshTextures2[i] = NULL;
		if (d3dxMaterials2[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials2[i].pTextureFilename) > 0)
		{
			// Create the texture
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				d3dxMaterials2[i].pTextureFilename,
				&pMeshTextures2[i])))
			{
				// If texture is not in current folder, try parent folder
				const TCHAR* strPrefix = TEXT("..\\");
				const int lenPrefix = lstrlen(strPrefix);
				TCHAR strTexture[MAX_PATH];
				lstrcpyn(strTexture, strPrefix, MAX_PATH);
				lstrcpyn(strTexture + lenPrefix, d3dxMaterials2[i].pTextureFilename, MAX_PATH - lenPrefix);
				// If texture is not in current folder, try parent folder
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					strTexture,
					&pMeshTextures2[i])))
				{
					MessageBox(NULL, _T("Could not find texture map"), _T("Meshes.exe"), MB_OK);
				}
			}
		}
	}

	SetLighting();
	pDevice->SetMaterial(&material);
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);

	// for loop to load in the objects
	SetupMatrices();
	D3DXMatrixTranslation(&matObj1, 0, 0, 0);
	D3DXMatrixTranslation(&matObj2, 0, 0, 0);

	// Done with the material buffer
	pD3DXMtrlBuffer->Release();
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

int Game::GameLoop() {
	frameController.FrameCount();
	this->Render();

	if (GetAsyncKeyState(VK_ESCAPE))
		PostQuitMessage(0);

	return S_OK;
}

int Game::GameShutdown() {

	frameController.UnloadAlphabet();

	if (this->pVB)
		this->pVB->Release();

	//release resources. First display adapter because COM object created it, then COM object
	if (this->pBackSurface)
		if (this->pDevice)
			this->pDevice->Release();

	if (this->pD3D)
		this->pD3D->Release();

	return S_OK;
}

int Game::LoadBitmapToSurface(TCHAR* PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice) {
	HRESULT r;
	HBITMAP hBitmap;
	BITMAP Bitmap;

	hBitmap = (HBITMAP)LoadImage(NULL, PathName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap == NULL) {
		SetError(_T("Unable to load bitmap"));
		return E_FAIL;
	}

	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	DeleteObject(hBitmap);//we only needed it for the header info to create a D3D surface

						  //create surface for bitmap
	r = pDevice->CreateOffscreenPlainSurface(Bitmap.bmWidth, Bitmap.bmHeight, D3DFMT_X8R8G8B8, D3DPOOL_SCRATCH, ppSurface, NULL);

	if (FAILED(r)) {
		SetError(_T("Unable to create surface for bitmap load"));
		return E_FAIL;
	}
	//load bitmap onto surface
	r = D3DXLoadSurfaceFromFile(*ppSurface, NULL, NULL, PathName, NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(r)) {
		SetError(_T("Unable to laod file to surface"));
		return E_FAIL;
	}

	return S_OK;
}

int Game::InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice) {
	D3DPRESENT_PARAMETERS d3dpp;//rendering info
	D3DDISPLAYMODE d3ddm;//current display mode info
	HRESULT r = 0;

	if (*ppDevice)
		(*ppDevice)->Release();

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	r = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if (FAILED(r)) {
		SetError(_T("Could not get display adapter information"));
		return E_FAIL;
	}

	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = bWindowed ? d3ddm.Format : FullScreenFormat;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//D3DSWAPEFFECT_COPY;
	d3dpp.hDeviceWindow = hWndTarget;
	d3dpp.Windowed = bWindowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = 0;//default refresh rate
	d3dpp.PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	//	d3dpp.FullScreen_PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	r = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, ppDevice);
	if (FAILED(r)) {
		SetError(_T("Could not create the render device"));
		return E_FAIL;
	}

	this->DeviceHeight = Height;
	this->DeviceWidth = Width;

	this->SavedPresParams = d3dpp;
	return S_OK;
}

int Game::Render() {
	HRESULT r;
	this->pBackSurface = 0;

	if (!this->pDevice) {
		SetError(_T("Cannot render because there is no device"));
		return E_FAIL;
	}
	this->pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 55), 1.0f, 0);
	r = ValidateDevice();
	if (FAILED(r)) {
		return E_FAIL;
	}
	r = this->pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->pBackSurface);
	if (FAILED(r)) {
		SetError(_T("Couldn't get backbuffer"));
	}
	// NULL for fullscreen
	r = this->pDevice->UpdateSurface(this->BitmapSurface, NULL, this->pBackSurface, NULL);
	if (FAILED(r)) {
		SetError(_T("Error setting surface to back surface"));
	}

	D3DLOCKED_RECT Locked;
	this->pBackSurface->LockRect(&Locked, 0, 0);
	frameController.PrintFrameRate(40, 50, TRUE, D3DCOLOR_ARGB(255, 255, 0, 255), (DWORD*)Locked.pBits, Locked.Pitch);	
	this->pBackSurface->UnlockRect();

	pDevice->SetFVF(DRVERTEX_FLAGS);

	//Start to render in 3D
	this->pDevice->BeginScene();

	pDevice->SetTransform(D3DTS_WORLD, &matObj1);
	// Meshes are divided into subsets, one for each material. Render them in
	// a loop
	for (DWORD i = 0; i<dwNumMaterials; i++)
	{
		// Set the material and texture for this subset
		pDevice->SetMaterial(&pMeshMaterials[i]);
		pDevice->SetTexture(0, pMeshTextures[i]);

		// Draw the mesh subset
		pMesh->DrawSubset(i);
	}
	pDevice->SetTransform(D3DTS_WORLD, &matObj2);
	for (DWORD i = 0; i<dwNumMaterials2; i++)
	{
		// Set the material and texture for this subset
		pDevice->SetMaterial(&pMeshMaterials2[i]);
		pDevice->SetTexture(0, pMeshTextures2[i]);

		// Draw the mesh subset
		pMesh2->DrawSubset(i);
	}

	//finish rendering
	this->pDevice->EndScene();

	pDevice->SetTransform(D3DTS_VIEW, &matView); // updates view

	//console work
	this->pDevice->Present(NULL, NULL, NULL, NULL);//swap over buffer to primary surface
	return S_OK;
}

HRESULT Game::ValidateDevice() {
	HRESULT r = 0;
	//Test current state of device
	r = this->pDevice->TestCooperativeLevel();
	if (FAILED(r)) {
		//if device is lost then return failure
		if (r == D3DERR_DEVICELOST)
			return E_FAIL;
		//if device is ready to be reset then try
		if (r == D3DERR_DEVICENOTRESET) {
			//release back surface
			this->pBackSurface->Release();
			//reset device
			r = this->pDevice->Reset(&this->SavedPresParams);
			if (FAILED(r)) {
				//device was not ready to be reset
				SetError(_T("Could not reset device"));
				PostQuitMessage(E_FAIL);
				return E_FAIL;
			}
			//reacquire a pointer to new back buffer
			r = this->pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->pBackSurface);
			if (FAILED(r)) {
				SetError(_T("Unable to reacquire back buffer"));
				PostQuitMessage(0);
				return E_FAIL;
			}
			this->pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
			this->RestoreGraphics();
		}
	}
	return S_OK;
}

HRESULT Game::RestoreGraphics() {
	return S_OK;
}

void Game::SetupMatrices()
{
	// For our world matrix, we will just leave it as the identity
	D3DXMATRIXA16 matWorld;
	D3DXMatrixRotationY(&matWorld, 50.0f);
	pDevice->SetTransform(D3DTS_WORLD, &matWorld); // Takes whatever is in the matrix, and sets it to the world
			// ** This is how we will move objects and rotate later **

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the 
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -10.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	pDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	float screenaspect = (float)DeviceWidth / (float)DeviceHeight;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, screenaspect, 1.0f, 100.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}
void Game::moveCamera(float x, float y, float z) {
	D3DXMATRIX newM;
	D3DXMatrixTranslation(&newM, x, y, z);
	D3DXMatrixMultiply(&matView, &matView, &newM);
}
void Game::rotateCamera(float r) {
	// rotate around y
	D3DXMATRIX newM;
	D3DXMatrixRotationY(&newM, r);
	D3DXMatrixMultiply(&matView, &matView, &newM);
}

// This needs to be fixed. I am rotating about the y. I need to rotate around the y of my coords
void Game::moveObject(int objNum, float x, float y, float z) {
	D3DXMATRIX newM;
	if (objNum == 1) {
		D3DXMatrixTranslation(&newM, x, y, z);
		D3DXMatrixMultiply(&matObj1, &matObj1, &newM);
	}
	else {
		D3DXMatrixTranslation(&newM, x, y, z);
		D3DXMatrixMultiply(&matObj2, &matObj2, &newM);
	}
}
void Game::rotateObject(int objNum, float r) {
	// rotate around y
	D3DXMATRIX newM, matRot, backM;
	float x, y, z;
	if (objNum == 1) {
		D3DXMatrixRotationY(&matRot, r);
		x = matObj1._41;
		y = matObj1._42;
		z = matObj1._43;
		D3DXMatrixTranslation(&newM, -x, -y, -z);
		D3DXMatrixMultiply(&newM, &newM, &matRot);
		D3DXMatrixTranslation(&backM, x, y, z);
		D3DXMatrixMultiply(&newM, &newM, &backM);
		D3DXMatrixMultiply(&matObj1, &matObj1, &newM);
	}
	else {
		x = matObj2._41;
		y = matObj2._42;
		z = matObj2._43;
		D3DXMatrixTranslation(&newM, -x, -y, -z);

		D3DXMatrixRotationY(&matRot, r);

		D3DXMatrixMultiply(&newM, &newM, &matRot);

		D3DXMatrixTranslation(&backM, x, y, z);

		D3DXMatrixMultiply(&newM, &newM, &backM);

		D3DXMatrixMultiply(&matObj2, &matObj2, &newM);
	}
}
bool Game::getCameraMove() {
	return CameraMove;
}
bool Game::getObj1Move() {
	return Obj1Move;
}
bool Game::getObj2Move() {
	return Obj2Move;
}
void Game::setCameraMove(bool b) {
	CameraMove = b;
	Obj1Move = false;
	Obj2Move = false;
}
void Game::setObj1Move(bool b) {
	Obj1Move = b;
	CameraMove = false;
	Obj2Move = false;
}
void Game::setObj2Move(bool b) {
	Obj2Move = b;
	CameraMove = false;
	Obj1Move = false;
}
void Game::SetLighting() {
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse.r = 1.0f;
	material.Diffuse.g = 1.0f;
	material.Diffuse.b = 1.0f;
	material.Diffuse.a = 1.0f;
	material.Diffuse.r = 1.0f;
	material.Ambient.r = 1.0f;
	material.Ambient.g = 1.0f;
	material.Ambient.b = 1.0f;
	material.Ambient.a = 1.0f;

	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXVECTOR3 vecDir;
	vecDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
}