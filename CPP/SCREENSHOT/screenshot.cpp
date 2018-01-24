#include "screenshot.h"

SCREENSHOT::SCREENSHOT() {
	init();
}

SCREENSHOT::SCREENSHOT(int x0, int y0, int x1, int y1) {
	init(x0, y0, x1, y1);
}

SCREENSHOT::~SCREENSHOT() {
	DeleteDC(hMemoryDC);
	DeleteDC(hScreenDC);
}

void SCREENSHOT::init(int x0, int y0, int x1, int y1) {
	if (x0 == -1) {
		x0 = 0;
		y0 = 0;
		x1 = GetSystemMetrics(SM_CXSCREEN);
		y1 = GetSystemMetrics(SM_CYSCREEN);
	}
	this->x0 = x0;
	this->y0 = y0;
	this->width = x1 - x0;
	this->height = y1 - y0;
	// get the device context of the screen
	this->hScreenDC = GetDC(NULL);
	// and a device context to put it in
	this->hMemoryDC = CreateCompatibleDC(hScreenDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
	// get a new bitmap
	this->hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);
}

void SCREENSHOT::get_screenshot(BMP &bmp) {
	hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
	this->hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);
	BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, x0, y0, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

	// Get the BITMAP from the HBITMAP
	BITMAP bmpScreen;
	GetObject(hBitmap, sizeof(BITMAP), &bmpScreen);

	//BITMAPFILEHEADER   bmfHeader;
	BITMAPINFOHEADER   bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmpScreen.bmWidth;
	bi.biHeight = bmpScreen.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	DWORD dwBmpSize = ((width * bi.biBitCount + 31) / 32) * 4 * height;

	// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
	// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
	// have greater overhead than HeapAlloc.
	HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
	char *lpbitmap = (char *)GlobalLock(hDIB);

	// Gets the "bits" from the bitmap and copies them into a buffer 
	// which is pointed to by lpbitmap.
	GetDIBits(hMemoryDC, hBitmap, 0,
		(UINT)height,
		lpbitmap,
		(BITMAPINFO *)&bi, DIB_RGB_COLORS);

	bmp.set_image_size(width, height);
	bmp.set_raw_image_data((unsigned char*)lpbitmap);

	//Unlock and Free the DIB from the heap
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);
}

double SCREENSHOT::get_scale_factor() {
	HDC hScreenDC = GetDC(NULL);
	int horizontalDPI = GetDeviceCaps(hScreenDC, LOGPIXELSX);
	int verticalDPI = GetDeviceCaps(hScreenDC, LOGPIXELSY);
	double scale_factor = 0;
	switch (horizontalDPI) {
	case 96: scale_factor = 1.0; break;
	case 120: scale_factor = 1.25; break;
	case 144: scale_factor = 1.5; break;
	case 192: scale_factor = 2.0; break;
	default: break;
	}
	return scale_factor;
}

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////                  Private                             ////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
