#include <iostream>
#include <stdexcept>
#include <fstream>
using namespace std;

// bit_count = 24
// usage:
// int r[128][128], g[128][128], b[128][128];
// // fill your data here
// BMP bmp_file(128/*width*/, 128/*height*/);
// bmp_file.set_image_data((int**)r, (int**)g, (int**)b);
// bmp_file.save_image("bmp_img.bmp");
class BMP {
public:
	BMP();
	BMP(int width, int height);
	~BMP();
	void set_image_size(int width, int height);
	// start from top left corner
	// r/g/b[width][height] or will crash (no boundary check)
	// domain = [0, 255] or will be cut off
	void set_image_data(int **r, int **g, int **b);
	int get_width();
	int get_height();
	void save_image(const char* file_path = "bmp_img.bmp");
	
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
