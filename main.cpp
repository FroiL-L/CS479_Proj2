#include <iostream>
#include "ReadImage.h"
#include "ReadImageHeader.h"
#include "WriteImage.h"


int main(int argc, char** argv) {
	//ImageType img;

	int r, c, maxval;
	bool type;

	readImageHeader((char*)("./sample.ppm"), r, c, maxval, type);

	//img.setImageInfo(r, c, maxval);
	ImageType img(r, c, maxval);
	//img = ImageType(r, c*3, maxval);

	readImagePPM((char*)("./sample.ppm"), img);

	writeImagePPM((char*)("./out.ppm"), img);


	return 0;
}
