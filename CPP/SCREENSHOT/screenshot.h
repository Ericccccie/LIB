#pragma once
#include <Windows.h>
#include <cmath>
#include "bmp.h"

class SCREENSHOT {
public:
	SCREENSHOT();
	SCREENSHOT(int x0, int y0, int x1, int y1);
	~SCREENSHOT();
	static double get_scale_factor();
	// get rect area(x0, y0, x1, y1)
	// (default: the whole screen)
	// save result to bmp
	void get_screenshot(BMP &bmp);

private:
	int x0, y0;			// upper left (to bottom right)
	int width, height;
	HDC hScreenDC;
	HDC hMemoryDC;
	HBITMAP hOldBitmap;
	HBITMAP hBitmap;
	void init(int x0 = -1, int y0 = -1, int x1 = -1, int y1 = -1);
};