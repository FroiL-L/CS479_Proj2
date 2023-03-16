#ifndef READIMAGE_H_
#define READIMAGE_H_

#include "image.h"

/* readImagePGM:
 * 	Inputs the pixel values contained within a PGM image into
 * 	a given storage location.
 * args:
 * 	@fname: Path to file to read pixel values from.
 * 	@image: The location to store the pixel values to.
 * return:
 * 	void
 */
void readImagePGM(char fname[], ImageType& image);


/* readImagePPM():
 * 	Inputs the pixel values contained within a PPM image into
 * 	a given storage location.
 * args:
 * 	@fname: Path to file to read pixel values from.
 * 	@image: The location to store the pixel values to.
 * return:
 * 	void
 */
void readImagePPM(char fname[], ImageType& image);

#endif
