#include "bmp.h"

BMP::BMP() {
	init();
	width = -1;
	height = -1;
	image = NULL;
}
BMP::BMP(int width, int height) {
	init();
	set_width(width);
	set_height(height);
	image = NULL;
}
BMP::~BMP() {
	if(image) {
		delete image;
		image = NULL;
	}
}
void BMP::set_image_size(int width, int height) {
	set_width(width);
	set_height(height);
}
// start from top left corner
// r/g/b[width][height] or will crash (no boundary check)
// domain = [0, 255] or will be cut off
void BMP::set_image_data(int **r, int **g, int **b) {
	set_size();
	create_image();
	for(int i = 0; i < height; i++) {
		// bmp is upside down
		int row = height - 1 - i;
		for(int j = 0; j < width; j++) {
			int col = j;
				// *((int*)r + width * i + j) = r[i][j]
			unsigned char red = (unsigned char)(*((int*)r + width * i + j));
			unsigned char green = (unsigned char)(*((int*)g + width * i + j));
			unsigned char blue = (unsigned char)(*((int*)b + width * i + j));
				// bmp seq: blue, green, red
			image[row * (width * 3 + padding_per_line) + col * 3 + 2] = red;
			image[row * (width * 3 + padding_per_line) + col * 3 + 1] = green;
			image[row * (width * 3 + padding_per_line) + col * 3] = blue;
		}
			// padding
		for(int j = 0; j < padding_per_line; j++)
			image[row * (width * 3 + padding_per_line) + width * 3 + j] = 0;

	}
}
int BMP::get_width() {
	return width;
}
int BMP::get_height() {
	return height;
}
void BMP::save_image(const char* file_path) {
	ofstream f;
	f.open(file_path, ios::binary);
	for(int i = 0; i < 14; i++)
		f << bitmap_file_header[i];
	for(int i = 0; i < 40; i++)
		f << bitmap_info_header[i];
	for(int i = 0; i < image_size; i++)
		f << image[i];
	f.close();
}
void BMP::init() {
	unsigned char _bitmap_file_header[14] = {
		'B', 'M',			// bfType (fixed)
		0, 0, 0, 0,			// bfSize
		0, 0, 0, 0,			// bfReserved (fixed)
		14 + 40, 0, 0, 0	// bfOffBits (fixed)
	};
	for(int i = 0; i < 14; i++)
		bitmap_file_header[i] = _bitmap_file_header[i];

	unsigned char _bitmap_info_header[40] = {
		40,	0, 0, 0,		// biSize (fixed)
		0, 0, 0, 0,			// biWidth
		0, 0, 0, 0,			// biHeight
		1, 0,				// biPlanes (fixed)
		24, 0,				// biBitCount (fixed)
		0, 0, 0, 0,			// biCompression (fixed)
		0, 0, 0, 0,			// biSizeImage (fixed)
		0, 0, 0, 0,			// biXPelsPerMeter (fixed)
		0, 0, 0, 0,			// biYPelsPerMeter (fixed)
		0, 0, 0, 0,			// biClrUsed (fixed)
		0, 0, 0, 0			// biClrImportant (fixed)
	};
	for(int i = 0; i < 40; i++)
		bitmap_info_header[i] = _bitmap_info_header[i];
}
void BMP::set_width(int width) {
	if(width < 0)
		throw logic_error("Width must not be less than 0");
	this->width = width;
		// high address is cut off when cast type
	bitmap_info_header[4] = (unsigned char)(width);
	bitmap_info_header[5] = (unsigned char)(width >> 8);
	bitmap_info_header[6] = (unsigned char)(width >> 16);
	bitmap_info_header[7] = (unsigned char)(width >> 24);
}
void BMP::set_height(int height) {
	if(height < 0)
		throw logic_error("Height must not be less than 0");
	this->height = height;
		// high address is cut off when cast type
	bitmap_info_header[8] = (unsigned char)(height);
	bitmap_info_header[9] = (unsigned char)(height >> 8);
	bitmap_info_header[10] = (unsigned char)(height >> 16);
	bitmap_info_header[11] = (unsigned char)(height >> 24);
}

	// must be called after set_width() & set_height()
void BMP::set_size() {
	if(width < 0 || height < 0)
		throw logic_error("Must set width and height first");
		// the number of pixels per line equals width
		// and one pixel needs 3(RGB) Byte, which is 24 bit
	padding_per_line = (4 - ((width * 3) % 4)) % 4;
		// header size + padded image size
	image_size = (width * 3 + padding_per_line) * height;
	int file_size = 54 + image_size;
		// high address is cut off when cast type
	bitmap_file_header[2] = (unsigned char)(file_size);
	bitmap_file_header[3] = (unsigned char)(file_size >> 8);
	bitmap_file_header[4] = (unsigned char)(file_size >> 16);
	bitmap_file_header[5] = (unsigned char)(file_size >> 24);
}

	// must be called after set_width() & set_height()
void BMP::create_image() {
	if(width < 0 || height < 0)
		throw logic_error("Must set width and height first");
	image = new unsigned char[image_size];
}
