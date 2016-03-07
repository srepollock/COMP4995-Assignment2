#pragma once
#ifndef __BASICS
#define __BASICS
#define BITMAP_PICTURE "baboon.bmp"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
void SetError(TCHAR*, ...);
HRESULT RestoreGraphics();
#endif