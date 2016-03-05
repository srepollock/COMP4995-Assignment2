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
	DWORD dwNumMaterials = 0L;
	D3DMATERIAL9* pMeshMaterials = NULL;
	LPDIRECT3DTEXTURE9* pMeshTextures = NULL;

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
};

#endif