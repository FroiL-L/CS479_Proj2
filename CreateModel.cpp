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
#include <time.h>

#include "Eigen/Dense"
#include "image.h"
#include "ReadImage.h"
#include "ReadImageHeader.h"
#include "WriteImage.h"
#include "rgb.h"
#include "point.h"


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
 * 	@type: The type of color scheme to use (1=RGB, 0=YCrCb).
 * return:
 * 	bool: 1 for success | 0 for failure
 */
bool learnForModel(char trainFName[], char refFName[], char modelFName[], bool type = true) {
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
				float r, g;
				if (type == true) {
					r = (float)val.r / (val.r + val.g + val.b);
					g = (float)val.g / (val.r + val.g + val.b);
				}
				else {
					r = (0.500 * val.r) - (0.419 * val.g) - (0.081 * val.b);
					g = - (0.169 * val.r) - (0.0332 * val.g) + (0.500 * val.b);
				}
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


/* estimate2DMean():
 * 	Estimates the mean for the two features in a set of data.
 * args:
 * 	@fName: Path to the file with data to work on.
 * 	@mu1: Location to store mean of first feature.
 * 	@mu2: location to store mean of second feature.
 * return:
 * 	void
 */
void estimate2DMean(char* fName, Eigen::Matrix<float, 2, 1>& mu1, Eigen::Matrix<float, 2, 1>& mu2) {
	// Variables
	std::ifstream inFile(fName);
	float sum11, sum12, sum21, sum22;
	float x, y, id;
	int samples1, samples2;

	// Test for proper file access
	if(!inFile.is_open()) {
		std::cout << "Error: Could not open file " << fName << std::endl;
		exit(1);
	}

	// Initialize variables for reading file
	sum11 = 0.0;
	sum12 = 0.0;
	sum21 = 0.0;
	sum22 = 0.0;
	samples1 = 0;
	samples2 = 0;

	// Begin reading samples
	while(inFile >> x >> y >> id) {
		if(id == 1.0) {
			sum11 += x;
			sum12 += y;
			samples1++;
		}
		else {
			sum21 += x;
			sum22 += y;
			samples2++;
		}
	}

	// Calculate means
	sum11 /= samples1 - 1;
	sum12 /= samples1 - 1;
	sum21 /= samples2 - 1;
	sum22 /= samples2 - 1;
	
	// Save means
	mu1 << sum11, sum12;
	mu2 << sum21, sum22;
}

/* estimate2DCov():
 * 	Estimate the covariance values in a covariance matrix for a
 * 	2D feature space.
 * args:
 * 	@fName: Path to the file with data to work on.
 * 	@mu1: Mean for the first feature.
 * 	@mu2: Mean for th second feature.
 * 	@cov11: Location to store the covariance for position 1,1.
 * 	@cov12: Location to store the covariance for position 1,2.
 * 	@cov22: Location to store the covariance for position 2,2.
 * return:
 * 	void
 */
void estimate2DCov(char* fName, 
		Eigen::Matrix<float, 2, 1>&  mu1, 
		Eigen::Matrix<float, 2, 1>& mu2, 
		Eigen::Matrix2f& covm1, 
		Eigen::Matrix2f& covm2) {
	// Variables
	std::ifstream inFile(fName);
	float x, y, id;
	float covm1_11, covm1_12, covm1_22;
	float covm2_11, covm2_12, covm2_22;
	float mu1x, mu1y, mu2x, mu2y;
	int samples1, samples2;

	// Test for proper file access
	if(!inFile.is_open()) {
		std::cout << "Error: Could not open file " << fName << std::endl;
		exit(1);
	}

	// Initialize values for covariance matrix
	covm1_11 = 0.0;
	covm1_12 = 0.0;
	covm1_22 = 0.0;
	covm2_11 = 0.0;
	covm2_12 = 0.0;
	covm2_22 = 0.0;
	samples1 = 0;
	samples2 = 0;
	mu1x = mu1(0, 0);
	mu1y = mu1(1, 0);
	mu2x = mu2(0, 0);
	mu2y = mu2(1, 0);

	// Begin reading from file
	while(inFile >> x >> y >> id) {
		if(id == 1.0) {
			covm1_11 += (x - mu1x) * (x - mu1x);
			covm1_12 += (x - mu1x) * (y - mu1y);
			covm1_22 += (y - mu1y) * (y - mu1y);
			samples1++;
		}
		else {
			covm2_11 += (x - mu2x) * (x - mu2x);
			covm2_12 += (x - mu2x) * (y - mu2y);
			covm2_22 += (y - mu2y) * (y - mu2y);
			samples2++;
		}
	}

	// Calculate covariances
	covm1_11 /= samples1 - 1;
	covm1_12 /= samples1 - 1;
	covm1_22 /= samples1 - 1;
	covm2_11 /= samples2 - 1;
	covm2_12 /= samples2 - 1;
	covm2_22 /= samples2 - 1;

	// Save covariances
	covm1 << covm1_11, covm1_12,
	      covm1_12, covm1_22;
	covm2 << covm2_11, covm2_12,
	      covm2_12, covm2_22;
}


/* randomDataSelect():
 * 	Randomly selects portions of data from a set and returns it.
 * args:
 * 	@fName: The name of the file with the data to select from.
 * 	@count1: The number of points to select from the first class.
 * 	@count2: The number of points to select from the second class.
 * 	@fDest: The location to store the selected data.
 * return:
 * 	void
 */
void randomDataSelect(char* fName, int count1, int count2, char* fDest) {
	// Variables
	float x, y, id;
	int pos1 = 0, pos2 = 0;
	point points1[60000];
	point points2[140000];
	std::ifstream inFile;
	std::ofstream outFile;

	// Open input file
	inFile.open(fName);
	if(!inFile.is_open()) {
		std::cout << "Error: Could not access file " << fName << std::endl;
	}

	// Read in points
	while(inFile >> x >> y >> id) {
		point p;
		p.x = x;
		p.y = y;
		p.id = id;

		if(id == 1.0) {
			points1[pos1] = p;
			pos1++;
		}
		else {
			points2[pos2] = p;
			pos2++;
		}
	}

	// Close input file
	inFile.close();

	// Open output file
	outFile.open(fDest);
	if(!outFile.is_open()) {
		std::cout << "Error: Could not access file " << fDest << std::endl;
	}

	// Randomly select data
	std::srand(time(NULL));
	for(int i = 0; i < count1; i++) {
		pos1 = std::rand() % 60001;
		point p = points1[pos1];
		outFile << p.x << " " << p.y << " " << p.id << std::endl;
	}

	for(int i = 0; i < count2; i++) {
		pos2 = std::rand() % 140001;
		point p = points2[pos2];
		outFile << p.x << " " << p.y << " " << p.id << std::endl;
	}

	// Close output file
	outFile.close();
	
}
