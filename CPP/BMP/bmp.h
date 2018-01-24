#pragma once
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <cstring>

// bit_count = 24
// 
// usage:
// unsigned char r[128][128], g[128][128], b[128][128];
// // fill your data here
// BMP bmp_file(128/*width*/, 128/*height*/);
// bmp_file.set_image_data((unsigned char*)r, (unsigned char*)g, (unsigned char*)b);
// bmp_file.save_image("bmp_img.bmp");
// 
// int width, height;
// unsigned char *r, *g, *b;
// // new r, g, b here
// BMP bmp_file2;
// bmp_file2.read_image("some_pic.bmp", width, height) // must be 24-bit bmp file !
// bmp_file2.resolve_image(r, g, b)
// // then you get your data with the size of r equals width * height
class BMP {
public:
	BMP();
	BMP(int width, int height);
	~BMP();
	BMP& operator=(BMP a);
	int get_width();
	int get_height();
	
	void set_image_size(int width, int height);
	
	// copy raw data to BMPD::image[]
	void set_raw_image_data(unsigned char* data);
	// start from top left corner
	// r/g/b[width][height] or will crash (no boundary check)
	// domain = [0, 255] or will be cut off
	void set_image_data(unsigned char *r, unsigned char *g, unsigned char *b);
	void set_image_data(unsigned char **r, unsigned char **g, unsigned char **b);
	void save_image(const char* file_path = "bmp_img.bmp");
	
	// read bmp file and get width and height
	void read_image(const char* file_path, int &w, int &h);
	// get rgb from an image
	void resolve_image(unsigned char *r, unsigned char *g, unsigned char *b);
	
private:
	void init();
	
	void set_width(int width);
	void set_height(int height);
	
	// must be called after set_width() & set_height()
	void set_size();
	
	// must be called after set_width() & set_height()
	void create_image();

	int width, height;
	int padding_per_line;
	int image_size;
	unsigned char* image;
	
	// ref: https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
	
	// Little-Endian
	
	///////////////////////
	// BITMAPFILEHEADER //
	//////////////////////
	unsigned char bitmap_file_header[14];
	///////////////////////
	// BITMAPINFOHEADER //
	//////////////////////
	unsigned char bitmap_info_header[40];
};
