#include "Includes.h"

Game g;

long CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	D3DLOCKED_RECT rect;
	DWORD* pData;
	short delta;
	switch (uMessage) {
	case WM_CREATE:
	{
		return 0;
	}
	case WM_PAINT:
	{
		ValidateRect(hWnd, NULL);//basically saying - yeah we took care of any paint msg without any overhead
		return 0;
	}
	case WM_CHAR: {
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);
		}
		return 0;
	}
	case WM_MOUSEWHEEL:
		delta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (delta > 0) {
			if (g.getCameraMove()) {
				g.rotateCameraX(-0.3f);
			}
			else if (g.getObj1Move()) {
				g.rotateObjectX(1, -0.3f);
			}
			else if (g.getObj2Move()) {
				g.rotateObjectX(2, -0.3f);
			}
		}
		else {
			if (g.getCameraMove()) {
				g.rotateCameraX(0.3f);
			}
			else if (g.getObj1Move()) {
				g.rotateObjectX(1, 0.3f);
			}
			else if (g.getObj2Move()) {
				g.rotateObjectX(2, 0.3f);
			}
		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case 0x30:
			// 0
			// move camera
			// set move camera bool
			g.setCameraMove(true);
			break;
		case 0x31:
			// 1
			//g.moveobj(1)
			g.setObj1Move(true);
			break;
		case 0x32:
			// 2
			//g.moveobj(2)
			g.setObj2Move(true);
			break;
	// moves current selection
		case 0x57:
			// w // move up
			if (g.getCameraMove()) {
				g.moveCamera(0.0f, 1.0f, 0.0f);
			}
			else if (g.getObj1Move()) {
				g.moveObject(1, 0.0f, 1.0f, 0.0f);
			}
			else if (g.getObj2Move()) {
				g.moveObject(2, 0.0f, 1.0f, 0.0f);
			}
			break;
		case 0x41:
			// a // move left
			if (g.getCameraMove()) {
				g.moveCamera(-1.0f, 0.0f, 0.0f);
			}
			else if (g.getObj1Move()) {
				g.moveObject(1, -1.0f, 0.0f, 0.0f);
			}
			else if (g.getObj2Move()) {
				g.moveObject(2, -1.0f, 0.0f, 0.0f);
			}
			break;
		case 0x53:
			// s // move down
			if (g.getCameraMove()) {
				g.moveCamera(0.0f, -1.0f, 0.0f);
			}
			else if (g.getObj1Move()) {
				g.moveObject(1, 0.0f, -1.0f, 0.0f);
			}
			else if (g.getObj2Move()) {
				g.moveObject(2, 0.0f, -1.0f, 0.0f);
			}
			break;
		case 0x44:
			// d // move right
			if (g.getCameraMove()) {
				g.moveCamera(1.0f, 0.0f, 0.0f);
			}
			else if (g.getObj1Move()) {
				g.moveObject(1, 1.0f, 0.0f, 0.0f);
			}
			else if (g.getObj2Move()) {
				g.moveObject(2, 1.0f, 0.0f, 0.0f);
			}
			break;
		case 0x58:
			// x // move farther
			if (g.getCameraMove()) {
				g.moveCamera(0.0f, 0.0f, 1.0f);
			}
			else if (g.getObj1Move()) {
				g.moveObject(1, 0.0f, 0.0f, 1.0f);
			}
			else if (g.getObj2Move()) {
				g.moveObject(2, 0.0f, 0.0f, 1.0f);
			}
			break;
		case 0x5A:
			// z // move closer
			if (g.getCameraMove()) {
				g.moveCamera(0.0f, 0.0f, -1.0f);
			}
			else if (g.getObj1Move()) {
				g.moveObject(1, 0.0f, 0.0f, -1.0f);
			}
			else if (g.getObj2Move()) {
				g.moveObject(2, 0.0f, 0.0f, -1.0f);
			}
			break;
		case 0x51:
			// q // rotate left
			if (g.getCameraMove()) {
				g.rotateCameraY(-0.3f);
			}
			else if (g.getObj1Move()) {
				g.rotateObjectY(1, -0.3f);
			}
			else if (g.getObj2Move()) {
				g.rotateObjectY(2, -0.3f);
			}
			break;
		case 0x45:
			// e // rotate right
			if (g.getCameraMove()) {
				g.rotateCameraY(0.3f);
			}
			else if (g.getObj1Move()) {
				g.rotateObjectY(1, 0.3f);
			}
			else if (g.getObj2Move()) {
				g.rotateObjectY(2, 0.3f);
			}
			break;
		case 0x50:
			// point lighting enabled
			// p
			g.SetLightingPoint();
			break;
		case 0x4F:
			// directional lighting enabled
			// o
			g.SetLightingDirectional();
			break;
		case 0x49:
			// spot lighting enabled
			// i
			g.SetLightingSpot();
			break;
		case 0x55:
			// ambient lighting enabled
			// u
			g.SetLightingAmbient();
			break;
		case 0x46:
			// f // rotate about z axis ccw
			if (g.getCameraMove()) {
				g.rotateCameraZ(0.3f);
			}
			else if (g.getObj1Move()) {
				g.rotateObjectZ(1, 0.3f);
			}
			else if (g.getObj2Move()) {
				g.rotateObjectZ(2, 0.3f);
			}
			break;
		case 0x47:
			// g // rotate about z axis cw
			if (g.getCameraMove()) {
				g.rotateCameraZ(-0.3f);
			}
			else if (g.getObj1Move()) {
				g.rotateObjectZ(1, -0.3f);
			}
			else if (g.getObj2Move()) {
				g.rotateObjectZ(2, -0.3f);
			}
			break;
		}
		return 0;
	case WM_LBUTTONDOWN:
		return 0;
	case WM_MOUSEMOVE:
		if (wParam == MK_LBUTTON) {
			// holding down the left button
		}
		return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pstrCmdLine, int iCmdShow) {
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wc;

	static TCHAR strAppName[] = _T("First Windows App, Zen Style");

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_HAND);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = strAppName;

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL,
		strAppName,
		strAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	//g_hWndMain = hWnd;//set our global window handle
	// FRAME HAS NOT BEEN SETUP!!!
	g = Game(hWnd);

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	if (FAILED(g.GameInit())) {
		//initialize Game
		SetError(_T("Initialization Failed"));
		g.GameShutdown();
		return E_FAIL;
	}

	while (TRUE) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			g.GameLoop();
		}
	}
	g.GameShutdown();// clean up the game
	return msg.wParam;
}
