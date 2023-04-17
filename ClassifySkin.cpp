/* ClassifySkin.cpp:
 * 	Implementation file for respective header file. Contains the definitions for
 * 	classifying skin pixels.
 * authors:
 * 	Froilan Luna-Lopez
 * 		University of Nevada, Reno
 * 		CS 479 - Pattern Recognition
 * date:
 * 	18 March 2023
 */


// Libraries
#include "image.h"
#include "rgb.h"
#include "classification.hpp"
#include "Eigen/Dense"

// Functions

/* classifyForPixel():
 * 	Classifies a pixel value as either skin or not skin.
 * args:
 * 	@pix: The pixel to classify.
 * return:
 * 	true: if the pixel is classified as skin.
 * 	false: otherwise.
 */
float classifyForPixel(RGB& pix) {
	// Priors
	float pIsSkin = 0.0671455;

	// Sample mean
	Eigen::Vector2f mu;
	mu << 0.432223, 0.295772;

	// Sample covariance matrix
	Eigen::Matrix2f covm;
	covm << 0.00243317, -0.00111725,
	     -0.00111725, 0.000788423;

	int samples = 2;

	// Calculate pixel feature values
	float r, g;
	int rgbSum = pix.r + pix.g + pix.b;
	if(rgbSum == 0) {
		r = 0;
		g = 0;
	}
	else {
		r = (float)pix.r / rgbSum;
		g = (float)pix.g / rgbSum;
	}
	Eigen::Vector2f x(r, g);

	// Calculate conditional probability
	//Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
	
	float prob = (x.transpose() * (-0.5 * covm.inverse()) * x);
	prob += ((covm.inverse() * mu).transpose() * x);
	prob += (-0.5 * mu.transpose() * covm.inverse() * mu);
	prob += (-0.5 * log(covm.determinant()));
	
	return prob;
}


/* classifyForPixelYCC():
 * 	Classifies a pixel value as either skin or not skin.
 * args:
 * 	@pix: The pixel to classify.
 * return:
 * 	true: if the pixel is classified as skin.
 * 	false: otherwise.
 */
float classifyForPixelYCC(RGB& pix) {
	// Priors
	float pIsSkin = 0.0671455;

	// Sample mean
	Eigen::Vector2f mu;
	mu << 23.639, 22.2349;

	// Sample covariance matrix
	Eigen::Matrix2f covm;
	covm << 48.9502, -0.328258,
	     -0.328258, 176.884;

	int samples = 2;

	// Calculate pixel feature values
	float r, g;
	r = (0.500 * pix.r) - (0.419 * pix.g) - (0.081 * pix.b);
	g = - (0.169 * pix.r) - (0.0332 * pix.g) + (0.500 * pix.b);
	Eigen::Matrix<float, 2, 1> x(r, g);

	// Calculate conditional probability
	//Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
	
	float prob = (x.transpose() * (-0.5 * covm.inverse()) * x);
	prob += ((covm.inverse() * mu).transpose() * x);
	prob += (-0.5 * mu.transpose() * covm.inverse() * mu) + (-0.5 * log(covm.determinant()));
	
	return prob;
}

	
/* classifyForImage():
 * 	Classifies skin pixels within an image.
 * args:
 * 	@source: The image containing the pixels to classify.
 * 	@dest: The image to output the classified pixels.
 * 	@t: The threshold for classifying skin pixels.
 * 	@type: The type of color scheme to use (1=RGB, 0=YCrCb).
 * return:
 * 	void
 */
void classifyForImage(ImageType& source, ImageType& dest, float t, bool type = true) {
	// Variables
	int rows, cols, levels;
	RGB val;

	// Get image metadata
	source.getImageInfo(rows, cols, levels);

	// Loop through source image pixel rows
	for(int i = 0; i < rows; i++) {
		// Loop through source image pixel columns
		for(int j = 0; j < cols * 3; j += 3) {
			// Get RGB values for pixel
			source.getPixelVal(i, j/3, val);

			// Classify pixel
			float result;
			if (type == true) {
				result = classifyForPixel(val); 
			}
			else {
				result = classifyForPixelYCC(val);
			}

			// Output classification to destination image
			if(result > t) {
				val.r = 255;
				val.g = 255;
				val.b = 255;
				dest.setPixelVal(i, j/3, val);
			}
			else {
				val.r = 0;
				val.g = 0;
				val.b = 0;
				dest.setPixelVal(i, j/3, val);
			}
		}
	}
}


/* getMisclass():
 * 	Gets the number of pixels misclassified in an image.
 * args:
 * 	@image: The image to test.
 * 	@ref: The image to test against.
 * 	@fp: The number of false positives encountered.
 * 	@fn: The number of false negatives encountered.
 * return:
 * 	void
 */
void getMisclass(ImageType& image, ImageType& ref, int& fp, int& fn) {
	// Variables
	int rows, cols, levels, count;
	RGB imagePix, refPix;

	// Initialize variables
	count = 0;
	fp = 0;
	fn = 0;
	image.getImageInfo(rows, cols, levels);

	// Loop through image pixel rows
	for(int i = 0; i < rows; i++) {
		// Loop through image pixel columns
		for(int j = 0; j < cols * 3; j += 3) {
			// Get pixels
			image.getPixelVal(i, j/3, imagePix);
			ref.getPixelVal(i, j/3, refPix);

			// Convert red reference to white
			if(refPix.r == 252 && refPix.g == 3 && refPix.b == 3) {
				refPix.r = 255;
				refPix.g = 255;
				refPix.b = 255;
			}

			// Test for inequality
			if(imagePix.r != refPix.r || imagePix.g != refPix.g || imagePix.b != refPix.b) {
				//std::cout << std::endl << imagePix.r << " " << imagePix.g << " " << imagePix.b << std::endl;
				//std::cout << refPix.r << " " << refPix.g << " " << refPix.b << std::endl;
				// Test for false positive
				if(imagePix.r == 255 && imagePix.g == 255 && imagePix.b == 255) {
					fp++;
				}
				// Default to false negative
				else {
					fn++;
				}
			}
		}
	}
}
