#ifndef READIMAGEHEADER_H
#define READIMAGEHEADER_H

/* readImageHeader():
 *  Reads the header information for PPM and PGM image files.
 * args:
 * 	@fname: Path to file to read header from.
 * 	@N: Location to output the number of rows as per the header.
 * 	@M: Location to output the number of columns as per the header.
 * 	@Q: Location to output the maximum value a pixel can have.
 * 	@type: The type of file that the header specifies it is.
 * return:
 * 	void
*/
void readImageHeader(char fname[], int& N, int& M, int& Q, bool& type);

#include "ReadImageHeader.cpp"

#endif
