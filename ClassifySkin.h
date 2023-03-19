/* ClassifyImage.h:
 * 	Header file for the respective implementation file. Contains
 * 	the declarations for functions.
 * author:
 * 	Froilan Luna-Lopez
 * 		University of Nevada, Reno
 * 		CS 479 - Pattern Recognition
 * date:
 * 	18 March 2023
 */

#ifndef CLASSIFYSKIN_H_
#define CLASSIFYSKIN_H_

/* classifyForPixel():
 * 	Classifies a pixel value as either skin or not skin.
 * args:
 * 	@pix: The pixel to classify.
 * 	@t: The threshold for classifying skin.
 * return:
 * 	true: if the pixel is classified as skin.
 * 	false: otherwise.
 */
bool classifyForPixel(RGB& pix, float t);

/* classifyForImage():
 * 	Classifies skin pixels within an image.
 * args:
 * 	@source: The image containing the pixels to classify.
 * 	@dest: The image to output the classified pixels.
 * 	@t: The threshold for classifying skin pixels.
 * return:
 * 	void
 */
void classifyForImage(ImageType& source, ImageType& dest, float t);

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
void getMisclass(ImageType& image, ImageType& ref, int& fp, int& fn);

#include "ClassifySkin.cpp"

#endif
