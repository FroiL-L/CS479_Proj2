#ifndef WRITEIMAGE_H_
#define WRITEIMAGE_H_

#include "image.h"

/* writeImagePGM():
 *  Output the PGM image information to a given file location.
 * args:
 * 	@fname: Path to file to output image information to.
 * 	@image: The location that stores the image information.
 */
void writeImagePGM(char fname[], ImageType& image);


/* writeImagePPM():
 * 	Output the PPM image information to a given file location.
 * args:
 * 	@fname: Path to file to output image information to.
 * 	@image: The location that stores the image information.
 */
void writeImagePPM(char fname[], ImageType& image);

#include "WriteImage.cpp"

#endif
