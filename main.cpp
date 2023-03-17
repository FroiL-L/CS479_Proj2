#include <iostream>
#include "ReadImage.h"
#include "ReadImageHeader.h"
#include "WriteImage.h"
#include "CreateModel.h"

#define REF_PPM_1 "./data/ref1.ppm"
#define REF_PPM_2 "./data/ref3.ppm"
#define REF_PPM_3 "./data/ref6.ppm"
#define TRN_PPM_1 "./data/Training_1.ppm"
#define TRN_PPM_2 "./data/Training_3.ppm"
#define TRN_PPM_3 "./data/Training_6.ppm"
#define MOD_OUT "model.txt"

void basicCopyPaste() {
	int r, c, maxval;
	bool type;

	readImageHeader((char*)("./sample.ppm"), r, c, maxval, type);

	//img.setImageInfo(r, c, maxval);
	ImageType img(r, c, maxval);
	//img = ImageType(r, c*3, maxval);

	readImagePPM((char*)("./sample.ppm"), img);

	writeImagePPM((char*)("./out.ppm"), img);

}

void genModel() {
	std::cout << "Learning iteration 1..." << std::endl;
	learnForModel((char*)TRN_PPM_1, (char*)REF_PPM_1, (char*)MOD_OUT);
	std::cout << "Learning iteration 2..." << std::endl;
	learnForModel((char*)TRN_PPM_2, (char*)REF_PPM_2, (char*)MOD_OUT);
	std::cout << "Learning iteration 3..." << std::endl;
	learnForModel((char*)TRN_PPM_3, (char*)REF_PPM_3, (char*)MOD_OUT);
}

void calculateMeans() {
	float mur, mug;
	estimateRGMean((char*)MOD_OUT, mur, mug);
	std::cout << "r mean: " << mur << std::endl;
	std::cout << "g mean: " << mug << std::endl;
}

void calculateCov() {
	float mur, mug, covrr, covgg, covrg;

	// Get sample means
	estimateRGMean((char*)MOD_OUT, mur, mug);
	std::cout << "r mean: " << mur << std::endl;
	std::cout << "g mean: " << mug << std::endl;
	
	// Calculate covariances
	estimateCovarianceRG((char*)MOD_OUT, covrr, covgg, covrg, mur, mug);

	// Display results
	std::cout << std::endl << "Covariance Values" << std::endl;
	std::cout << "==============================" << std::endl;
	std::cout << "COV(R,G) = " << covrg << std::endl;
	std::cout << "COV(R,R) = " << covrr << std::endl;
	std::cout << "COV(G,G) = " << covgg << std::endl;
}

int main(int argc, char** argv) {
	calculateCov();

	return 0;
}
