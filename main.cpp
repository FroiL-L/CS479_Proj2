#include <iostream>
#include <fstream>
#include "ReadImage.h"
#include "ReadImageHeader.h"
#include "WriteImage.h"
#include "CreateModel.h"
#include "ClassifySkin.h"

#include "image.h"

#define REF_PPM_1 "./data/ref1.ppm"
#define REF_PPM_2 "./data/ref3.ppm"
#define REF_PPM_3 "./data/ref6.ppm"
#define TRN_PPM_1 "./data/Training_1.ppm"
#define TRN_PPM_2 "./data/Training_3.ppm"
#define TRN_PPM_3 "./data/Training_6.ppm"
#define MOD_OUT "model.txt"
#define MOD_YCC "model_ycc.txt"

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

void genModel(bool isRGB = true) {
	std::cout << "Learning from " << REF_PPM_1 << std::endl;
	
	if (isRGB) {
		learnForModel((char*)TRN_PPM_1, (char*)REF_PPM_1, (char*)MOD_OUT, isRGB);
	}
	else {
		learnForModel((char*)TRN_PPM_1, (char*)REF_PPM_1, (char*)MOD_YCC, isRGB);
	}
	
}

void testClassifyPixel() {
	RGB val;
	val.r = 239;
	val.g = 187;
	val.b = 170;

	float result = classifyForPixel(val);

	std::cout << std::endl << "Pixel result: " << result << std::endl;
}

void calculatePriors() {
	float skinPrior;
	int skinCount, totalCount, tmpSkin, tmpTotal;
	skinCount = 0;
	totalCount = 0;

	estimatePriors((char*)REF_PPM_1, tmpSkin, tmpTotal);
	skinCount += tmpSkin;
	totalCount += tmpTotal;

	/*estimatePriors((char*)REF_PPM_2, tmpSkin, tmpTotal);
        skinCount += tmpSkin;
        totalCount += tmpTotal;

	estimatePriors((char*)REF_PPM_3, tmpSkin, tmpTotal);
        skinCount += tmpSkin;
        totalCount += tmpTotal;*/

	skinPrior = (float)skinCount / totalCount;

	std::cout << std::endl << "Prior Values:" << std::endl;
	std::cout << "==========================" << std::endl;
	std::cout << "Is skin: " << skinPrior << std::endl;
	std::cout << "is not skin: " << 1 - skinPrior << std::endl;
	std::cout << "Total pixels encountered: " << totalCount << std::endl;
}

void testSkinClassification(char* inFile, char* outFile, bool isRGB, float t) {
	ImageType image;
	ImageType outImage;

	getImage(inFile, image);
	outImage = image;

	classifyForImage(image, outImage, t, isRGB);

	writeImagePPM(outFile, outImage);
}

void testSkinMisclassification(float& fpRate, float& fnRate, float t, bool isRGB) {
	ImageType image, outImage, refImage;
	int fp, fn, rows, cols, levels, totalPix;

        getImage((char*)TRN_PPM_1, image);
	outImage = image;
	getImage((char*)REF_PPM_1, refImage);

	std::cout << std::endl << "Classifying image pixels..." << std::endl;
        classifyForImage(image, outImage, t, isRGB);

	std::cout << "Testing for misclassifications..." << std::endl;
        getMisclass(outImage, refImage, fp, fn);

	outImage.getImageInfo(rows, cols, levels);
	totalPix = rows * cols;

	fpRate = (float)fp / totalPix;
	fnRate = (float)fn / totalPix;

	std::cout << std::endl << "Misclassification: t = " << t << std::endl;
	std::cout << "================================" << std::endl;
	std::cout << "False positive count: " << fp << std::endl;
	std::cout << "False negative count: " << fn << std::endl;
	std::cout << "Total pixels tested:  " << totalPix << std::endl;
	std::cout << "False positive rate:  " << fpRate << std::endl;
	std::cout << "False negative rate:  " << fnRate << std::endl;
}

void getROCVals(bool isRGB) {
	char* fName;
	float fpRate, fnRate, minT, maxT, stride;
	int fp = 0, fn = 0, totalPix = 0;
	int fpTmp, fnTmp;
	int rows, cols, levels;
	ImageType image, refImage, outImage;

	// Configure for RGB
	if (isRGB) {
		fName = (char*)"roc.txt";
		minT = 0.00000;
		maxT = 7.46400;		// Max prob value = 7.10792; adust for 21 iters
		stride = 7.10792 / 20;
	}
	// Configure for YCrCb
	else {
		fName = (char*)"roc_ycc.txt";
		minT = -4.53314 * 2;		// Translate to match [0,maxT]
		maxT = -4.29900;		// Max prob value = -4.53314; adjust for 21 iters
		stride = 4.53314 / 20;
	}

	// Open file
	std::ofstream outFile(fName);

	// Test for inproper file access
	if(!outFile.is_open()) {
		std::cout << "Could not open file roc.txt" << std::endl;
		exit(1);
	}

	// Start ROC processes
	int iter = 1;
	for(float i = minT; i <= maxT; i += stride, iter++) {
		std::cout << "(" << iter << "/20)" << std::endl;

		// Redefine variables
		totalPix = 0;
		fn = 0;
		fp = 0;

		// Test on first image
		getImage((char*)TRN_PPM_2, image);
		outImage = image;
		getImage((char*)REF_PPM_2, refImage);

		classifyForImage(image, outImage, i, isRGB);

		getMisclass(outImage, refImage, fpTmp, fnTmp);
		
		outImage.getImageInfo(rows, cols, levels);
		totalPix += rows * cols;
		fp += fpTmp;
		fn += fnTmp;

		// Test on second image
		getImage((char*)TRN_PPM_3, image);
		outImage = image;
		getImage((char*)REF_PPM_3, refImage);

		classifyForImage(image, outImage, i, isRGB);

		getMisclass(outImage, refImage, fp, fn);
		
		outImage.getImageInfo(rows, cols, levels);
		totalPix += rows * cols;
		fp += fpTmp;
		fn += fnTmp;

		// Calculate misclassification rates
		fpRate = (float)fp / totalPix;
		fnRate = (float)fn / totalPix;
		outFile << i << "," << fpRate << "," << fnRate << std::endl;
	}

	outFile.close();
}

void calculateMeans(char fName[]) {
	float mur, mug;
	estimateRGMean((char*)fName, mur, mug);

	std::cout << std::endl << "Mean values:" << std::endl;
	std::cout << "=========================" << std::endl;
	std::cout << "r mean: " << mur << std::endl;
	std::cout << "g mean: " << mug << std::endl;
}

void calculateCov(char fName[]) {
	float mur, mug, covrr, covgg, covrg;

	// Get sample means
	estimateRGMean((char*)fName, mur, mug);
	
	// Calculate covariances
	estimateCovarianceRG((char*)fName, covrr, covgg, covrg, mur, mug);

	// Display results
	std::cout << std::endl << "Covariance Values" << std::endl;
	std::cout << "==============================" << std::endl;
	std::cout << "COV(R,G) = " << covrg << std::endl;
	std::cout << "COV(R,R) = " << covrr << std::endl;
	std::cout << "COV(G,G) = " << covgg << std::endl;
}


void runParameterEstimation(bool isRGB) {
	if (isRGB) {
		calculatePriors();
		calculateMeans((char*)MOD_OUT);
		calculateCov((char*)MOD_OUT);
	}
	else {
		calculatePriors();
                calculateMeans((char*)MOD_YCC);
                calculateCov((char*)MOD_YCC);
	}
}

void runClassifyERR(bool isRGB) {
	ImageType image;
	ImageType outImage;

	getImage((char*)TRN_PPM_1, image);
	outImage = image;

	classifyForImage(image, outImage, 6.75252, isRGB);

	writeImagePPM((char*)"Classified_ERR.ppm", outImage);

}

int main(int argc, char** argv) {
	testSkinClassification((char*)TRN_PPM_1, (char*)"Classified_RGB_2.ppm", true, 6.75252);
	testSkinClassification((char*)TRN_PPM_2, (char*)"Classified_RGB_3.ppm", true, 6.75252);
	testSkinClassification((char*)TRN_PPM_1, (char*)"Classified_YCC_2.ppm", false, -5.21311);
        testSkinClassification((char*)TRN_PPM_2, (char*)"Classified_YCC_3.ppm", false, -5.21311);
	return 0;
}
