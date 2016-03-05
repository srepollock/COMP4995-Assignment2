#pragma once
#ifndef __BASICS
#define __BASICS
#define BITMAP_PICTURE "baboon.bmp"
#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
void SetError(TCHAR*, ...);
HRESULT RestoreGraphics();
#endif