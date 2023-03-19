/* CreateModel.cpp:
 * 	Contains the functionality for creating a model for skin detection
 * 	in images.
 * authors:
 * 	Froilan Luna-Lopez
 * 		University of Nevada, Reno
 * 		CS 479 - Pattern Recognition
 * date:
 * 	15 March 2023
 */


// Libraries
#include <fstream>
#include <stdlib.h>

#include "image.h"
#include "ReadImage.h"
#include "ReadImageHeader.h"
#include "WriteImage.h"
#include "rgb.h"


// Functions

/* getImage():
 * 	Gets the pixel values from an image and stores them in an
 * 	object of type ImageType.
 * args:
 * 	@fName: The path to the image to read.
 * 	@image: The object to store the image values to.
 * return:
 * 	void
 */
void getImage(char fName[], ImageType& image) {
	// Declare variables
	int hRows, hCols, hLevel;
	bool hType;

	// Get image header
	readImageHeader(fName, hRows, hCols, hLevel, hType);
	ImageType tmp(hRows, hCols, hLevel);
	image = tmp;

	// Get image pixel values
	readImagePPM(fName, image);
}


/* learnForModel():
 * 	Learns for a model by generating classified data from RGB values.
 * 	Training data with RGB values and reference data with PPM values
 * 	will be used to generate this data, and the true skin values
 * 	will be stored in a given file. If the file does not exist, 
 * 	it will be created. Otherwise, data will be appended to the
 * 	end of the file.
 * args:
 * 	@trainFName: The path to the file containing the image that 
 * 		will be used as training data.
 * 	@refFName: The path to the file containing the image that will
 * 		be used as reference data.
 * 	@modelFName: The path to the file that will be used as the
 * 		location to store the model values.
 * return:
 * 	bool: 1 for success | 0 for failure
 */
bool learnForModel(char trainFName[], char refFName[], char modelFName[]) {
	// Declare variables
	int hRows, hCols, hLevel;
	int* modelData;
	ImageType trainData, refData;
	std::ofstream modelFile;
	
	// Get training image data
	getImage(trainFName, trainData);

	// Get reference image data
	getImage(refFName, refData);

	// Open model file in appending mode
	modelFile.open(modelFName, std::ios_base::ate);
	if(!modelFile.is_open()) {
		std::cout << "Error: Couldn't open file "
			<< modelFName
			<< std::endl;
		exit(1);
	}

	// Loop through training data rows
	trainData.getImageInfo(hRows, hCols, hLevel);
	for(int i = 0; i < hRows; i++) {
		// Loop through training data columns
		for(int j = 0; j < hCols * 3; j += 3) {
			// Check if reference data determines that the
	 		// pixel is skin
			RGB val;
			refData.getPixelVal(i, j/3, val);
			if((val.r == 255 && val.g == 255 && val.b == 255)
					|| (val.r == 252 && val.g == 3 && val.b == 3)) {
				//Add training values to model
				trainData.getPixelVal(i, j/3, val);
				float r = (float)val.r / (val.r + val.g + val.b);
				float g = (float)val.g / (val.r + val.g + val.b);
				modelFile << r << " "
					<< g << std::endl;
			}
		}
	}

	// Close output file
	modelFile.close();

	return true;
}


/* estimateRGMean():
 *	Calculates the sample mean for red and green values contained
 *	within a given file.
 * args:
 * 	@fName: The path to the file containing the feature values.
 * 	@mur: The location to store the sample mean for reds.
 * 	@mug: The location to store the sample mean for greens.
 * return:
 * 	void
 */
void estimateRGMean(char fName[], float& mur, float& mug) {
	// Declare variables
	float rSum, gSum;
	int samples;
	char line[100];
	char* delimitPtr;
	ifstream inFile(fName);

	// Test if file not properly opened
	if(!inFile.is_open()) {
		std::cout << "Error: Could not open "
			<< fName
			<< std::endl;
		exit(1);
	}

	// Go through each line in the file
	samples = 0;
	rSum = 0;
	gSum = 0;
	while(inFile.getline(line, 100)) {
		rSum += std::strtof(line, &delimitPtr);
		gSum += std::strtof(delimitPtr, &delimitPtr);
		samples++;
	}

	// Finish reading from file
	inFile.close();

	// Calculate means
	mur = (float)rSum / samples;
	mug = (float)gSum / samples;
}

/* estimateCovarianceRG():
 * 	Calculates the sample covariances contained within the upper
 * 	triangular section of a covariance matrix for the red and
 * 	green features.
 * args:
 * 	@fName: The path to the file containing the samples.
 * 	@covrr: The covariance between red and red.
 * 	@covgg: The covariance between green and green.
 * 	@covrg: The covariance between red and green.
 */
void estimateCovarianceRG(char fName[], float& covrr, float& covgg, float& covrg, float mur, float mug) {
	// Declare variables
	float rrSum, ggSum, rgSum;
	int samples;
	char line[100];
	char* delimitPtr;
	ifstream inFile(fName);

	// Test if file not properly opened
	if(!inFile.is_open()) {
		std::cout << "Error: Could not open "
			<< fName
			<< std::endl;
		exit(1);
	}

	// Go through each line in the file
	samples = 0;
	rrSum = 0;
	ggSum = 0;
	rgSum = 0;
	while(inFile.getline(line, 100)) {
		// Get feature values
		float r = std::strtof(line, &delimitPtr);
		float g = std::strtof(delimitPtr, &delimitPtr);
		// Sum denormalized covariances
		rrSum += (r - mur) * (r - mur);
		ggSum += (g - mug) * (g - mug);
		rgSum += (r - mur) * (g - mug);

		// Update sample counter
		samples++;
	}

	// Finish reading from file
	inFile.close();

	// Calculate means
	covrr = rrSum / samples;
	covgg = ggSum / samples;
	covrg = rgSum / samples;
}


/* estimatePriors():
 * 	Calculates the priors for the classes skin and not skin contained
 * 	within the pixels of a given image.
 * args:
 * 	@fName: The path to the file containing the classified pixels.
 * 	@skinCount: The location to store the number of skin pixels
 * 		encountered.
 * 	@totalCount: The location to store the total number of pixels
 * 		encountered.
 * return:
 * 	void
 */
void estimatePriors(char fName[], int& skinCount, int& totalCount) {
	// Declare variables
	int hRows, hCols, hLevel;
	ImageType image;
	
	// Get reference image data
	getImage(fName, image);

	// Loop through image data rows
	image.getImageInfo(hRows, hCols, hLevel);
	skinCount = 0;
	totalCount = hRows * hCols;
	for(int i = 0; i < hRows; i++) {
		// Loop through image data columns
		for(int j = 0; j < hCols * 3; j += 3) {
			// Check if reference data determines that the
	 		// pixel is skin
			RGB val;
			image.getPixelVal(i, j/3, val);
			if((val.r == 255 && val.g == 255 && val.b == 255)
					|| (val.r == 252 && val.g == 3 && val.b == 3)) {
				skinCount++;
			}
		}
	}
}
