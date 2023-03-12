#include "ImageType.hpp"
#include "ImageHelpers.hpp"
#include <iostream>

int main(int argc, char** argv) {
	ImageType img;

	int r, c, maxval;
	bool type;

	readImageHeader((char*)("./sample.pgm"), r, c, maxval, type);

	img.setImageInfo(r, c, maxval);

	readImage((char*)("./sample.pgm"), img);

	writeImage((char*)("./out.pgm"), img);


	return 0;
}
