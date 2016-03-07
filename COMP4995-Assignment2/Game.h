#pragma once
#ifndef __GAME
#define __GAME
#define DRVERTEX_FLAGS (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
struct DRIVERTEX {
	float x, y, z;
	float nx, ny, nz;
};

class Game {
private:
	HWND hWndMain;
	Frame frameController;
	LPDIRECT3D9 pD3D = 0;
	LPDIRECT3DDEVICE9 pDevice = 0;
	PDIRECT3DSURFACE9 pBackSurface = 0;
	PDIRECT3DSURFACE9 BitmapSurface = 0;
	D3DPRESENT_PARAMETERS SavedPresParams;
	LPDIRECT3DVERTEXBUFFER9 pVB = 0;
	int DeviceHeight, DeviceWidth;
	LPD3DXMESH pMesh = NULL;
	LPD3DXMESH pMesh2 = NULL;
	DWORD dwNumMaterials = 0L;
	D3DMATERIAL9* pMeshMaterials = NULL;
	LPDIRECT3DTEXTURE9* pMeshTextures = NULL;
	DWORD dwNumMaterials2 = 0L;
	D3DMATERIAL9* pMeshMaterials2 = NULL;
	LPDIRECT3DTEXTURE9* pMeshTextures2 = NULL;
	D3DXMATRIX matView;
	D3DXMATRIX matObj1;
	D3DXMATRIX matObj2;
	D3DMATERIAL9 material;
	D3DLIGHT9 light;
	bool CameraMove = true; // default move camera
	bool Obj1Move = false;
	bool Obj2Move = false;

public:
	Game();
	Game(HWND);
	~Game();
	int GameInit();
	int GameLoop();
	int GameShutdown();
	int LoadBitmapToSurface(TCHAR*, LPDIRECT3DSURFACE9*, LPDIRECT3DDEVICE9);
	int InitDirect3DDevice(HWND, int, int, BOOL, D3DFORMAT, LPDIRECT3D9, LPDIRECT3DDEVICE9*);
	int Render();
	void SetupMatrices();
	HRESULT ValidateDevice();
	HRESULT RestoreGraphics();
	void moveCamera(float, float, float);
	void rotateCameraX(float);
	void rotateCameraY(float);
	void rotateCameraZ(float);
	void moveObject(int, float, float, float);
	void rotateObjectX(int, float);
	void rotateObjectY(int, float);
	void rotateObjectZ(int, float);
	bool getCameraMove();
	bool getObj1Move();
	bool getObj2Move();
	void setCameraMove(bool);
	void setObj1Move(bool);
	void setObj2Move(bool);
	void SetLightingDirectional();
	void SetLightingAmbient();
	void SetLightingSpot();
	void SetLightingPoint();
};

#endif