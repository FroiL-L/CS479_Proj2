/* image.cpp:
 * 	Implementation file for image.h.
 * Credit:
 * 	Dr. George Bebis
 * 		The source code was obtained via his website at
 * 		https://www.cse.unr.edu/~bebis/CS479/ .
 * 		It has been slightly modified by adding comments
 * 		for our intents and purposes.
 * Date:
 * 	15 March 2023
 */


// Libraries
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#include "image.h"
#include "rgb.h"

#include <iostream>
// Functions

/* ImageType():
 * 	Default constructor for ImageType. Initializes all values to zero.
 */
ImageType::ImageType()
{
 N = 0;
 M = 0;
 Q = 0;

 pixelValue = NULL;
}


/* ImageType():
 * 	Constructor for ImageType. Assigns values to self through
 *  parameters.
 * args:
 *  @tmpN: Number of rows.
 *  @tmpM: Number of columns.
 *  @tmpQ: Max value possible for pixel values.
 */
ImageType::ImageType(int tmpN, int tmpM, int tmpQ)
{
 int i, j;

 N = tmpN;
 M = tmpM;
 Q = tmpQ;

 pixelValue = new int* [N];
 for(i=0; i<N; i++) {
   pixelValue[i] = new int[M*3];
   for(j=0; j<M; j++)
     pixelValue[i][j] = 0;
 }
}


/* ~ImageType():
 * 	Destructor for ImageType.
 */
ImageType::~ImageType() {
 for(int i = 0; i < N; i++) {
	delete[] pixelValue[i];
 }
 delete[] pixelValue;
}


/* getImageInfo():
 * 	Gets the metadata information for the contained image.
 * args:
 * 	@rows: Location to output the number of rows in the image.
 * 	@cols: Location to output the number of columns in the image.
 * 	@levels: Location to output the maximum pixel value allowed.
 * return:
 * 	void
 */
void ImageType::getImageInfo(int& rows, int& cols, int& levels)
{
 rows = N;
 cols = M;
 levels = Q;
} 


/* setImageInfo():
 * 	Sets the metadata information for the contained image.
 * args:
 * 	@rows: The number of rows to assign to image.
 * 	@cols: The number of columns to assign to image.
 * 	@levels: The maximum pixel value allowed.
 * return:
 * 	void
 */
void ImageType::setImageInfo(int rows, int cols, int levels)
{
 N= rows;
 M= cols;
 Q= levels;

 pixelValue = new int*[N];
 for(int i = 0; i < M; i++) {
	pixelValue[i] = new int[M];
 }
}


/* setPixelVal():
 * 	Sets the pixel value at a given location.
 * args:
 * 	@i: The row to access.
 * 	@j: The column to access.
 * 	@val: The value to set the pixel value as.
 * return:
 * 	void
 */
void ImageType::setPixelVal(int i, int j, int val)
{
 pixelValue[i][j] = val;
}


/* setPixelVal():
 * 	Sets the pixel value at a given location.
 * args:
 * 	@i: The row to access.
 * 	@j: The column to access.
 * 	@val: The value to set the pixel value as.
 * return:
 * 	void
 */
void ImageType::setPixelVal(int i, int j, RGB& val)
{
 pixelValue[i][j*3] = val.r;
 pixelValue[i][j*3+1] = val.g;
 pixelValue[i][j*3+2] = val.b;
}


/* getPixelVal():
 * 	Get the pixel value at a given location.
 * args:
 * 	@i: The row to look for the pixel value.
 * 	@j: The column to look for the pixel value.
 * 	@val: Location to store the pixel value.
 * return:
 * 	void
 */
void ImageType::getPixelVal(int i, int j, int& val)
{
 val = pixelValue[i][j];
}


/* getPixelVal():
 * 	Get the pixel value at a given location.
 * args:
 * 	@i: The row to look for the pixel value.
 * 	@j: The column to look for the pixel value.
 * 	@val: Location to store the pixel value.
 * return:
 * 	void
 */
void ImageType::getPixelVal(int i, int j, RGB& val)
{
 val.r = pixelValue[i][j*3];
 val.g = pixelValue[i][j*3+1];
 val.b = pixelValue[i][j*3+2];
}


/* operator=():
 * 	Modifies the left-hand object (self) by reassigning its values
 * 	to match that of the right-hand object.
 * args:
 * 	@image: The source to copy values from.
 * return:
 * 	(*)this
 */
ImageType& ImageType::operator=(ImageType& image) {
	// Remove old pixel values
	for(int i = 0; i < N; i++) {
		delete[] pixelValue[i];
	}
	if(pixelValue != NULL)
		delete[] pixelValue;

	// Reassign basic variables
	image.getImageInfo(N, M, Q);

	// Allocate and store new pixel values
	pixelValue = new int*[N];
	for(int i = 0; i < N; i++) {
		pixelValue[i] = new int[M*3];
		for(int j = 0; j < M; j++) {
			int val;
			image.getPixelVal(i, j, val);
			pixelValue[i][j] = val;
		}
	}

	return *this;
}
