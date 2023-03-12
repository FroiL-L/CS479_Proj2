#ifndef _IMAGEHELPERS_HPP_
#define _IMAGEHELPERS_HPP_

// Libraries
#include "ImageType.hpp"
#include <fstream>
#include <string.h>
#include <iostream>

/* Declarations */

/* readImageHeader():
 * 	Reads the header information in PPM, PGM, and PBM image files.
 * args:
 * 	@fsource: Path to file to read header from.
 * 	@rows: Location to output the number of rows as per the header.
 * 	@cols: Location to output the number of columns as per the header.
 * 	@maxLevel: Location to output the maximum value a pixel can have.
 * 	@type: The type of file that the header specifies it is.
 * return:
 * 	void
 */
void readImageHeader(char fsource[], int& rows, int& cols, int& maxLevel, bool& type);

/* readImage():
 * 	Input the pixel values contained within an image file.
 * args:
 * 	@fsource: Path to file to read pixel values from.
 * 	@dest: The location to store the pixel values to.
 * return:
 * 	void
 */
void readImage(char fsource[], ImageType& dest);

/* writeImage():
 * 	Output the image information to a given file location.
 * args:
 * 	@fdest: Path to file to output image information to.
 * 	@source: The location that stores the image information.
 */
void writeImage(char fdest[], ImageType& source);

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

/* Implementations*/

void readImageHeader(char fsource[], int& rows, int& cols, int& maxLevel, bool& type) {
	std::ifstream infile(fsource);

	// Test for invalid file access
	if(!infile.is_open()) return;

	// Variables
	std::string headerVals[4];

	// Iterate through the lines in the file
	std::string line;
	int metaValCount = 0;
	while(std::getline(infile, line) && metaValCount < 4) {
		char* token = strtok(&line[0], " ");
		// Iterate through tokens in a line
		while(token != NULL) {
			// Test for end of header
			if(metaValCount == 4) break;

			// Test for comment
			if(token[0] == '#') break;

			// save value
			headerVals[metaValCount] = token;
			metaValCount++;

			// Update token
			token = strtok(NULL, " ");
		}


	}

	// Test for invalid number of header values
	if(metaValCount != 4) return;

	// Save found values
	rows = std::stoi(headerVals[1]);
	cols = std::stoi(headerVals[2]);
	maxLevel = std::stoi(headerVals[3]);
	type = true; // TO BE UPDATED
}

void readImage(char fsource[], ImageType& dest) {
	// Variables
	int rows, cols, maxValue;
	int metaValCount = 0;
	std::string iterator;
	unsigned char* charImage;

	std::ifstream infile(fsource);

        // Test for invalid file access
        if(!infile.is_open()) return;

	// Skip through header
	while(metaValCount < 4 && std::getline(infile, iterator)) {
		char* token = strtok(&iterator[0], " ");
                // Iterate through tokens in a line
                while(token != NULL) {
                        // Test for end of header
                        if(metaValCount == 4) break;

                        // Test for comment
                        if(token[0] == '#') break;

                        // save value
                        metaValCount++;

                        // Update token
                        token = strtok(NULL, " ");
                }
	}

	// Setup location for saving the pixel values as unsigned char
	dest.getImageInfo(rows, cols, maxValue);
	charImage = (unsigned char*) new unsigned char[rows * cols];

	// Read in pixel values as unsigned char
	infile.read(reinterpret_cast<char*>(charImage), (rows * cols) * sizeof(unsigned char));

	// Test for invalid read operation
	if(infile.fail()) {
		std::cout << "Image " << fsource << " has wrong size" << std::endl;
		exit(1);
	}

	// Close file - finished reading
	infile.close();

	// Save pixel values as int
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			int val = (int)charImage[i * cols + j];
			dest.setVal(i, j, val);
		}
	}

	// Delete temporary pixel storage memory allocation
	delete(charImage);
}

void writeImage(char fdest[], ImageType& source) {
	// Variables
	int rows, cols, maxVal;
	unsigned char* charImage;

	std::ofstream outfile(fdest);

	// Test for invalid file opening
	if(!outfile.is_open()) return;

	// Write out header
	source.getImageInfo(rows, cols, maxVal);
	outfile << "P5" << std::endl
		<< rows << std::endl
		<< cols << std::endl
		<< maxVal << std::endl;

	// Get pixel values as int
	charImage = (unsigned char*) new unsigned char[rows * cols];
	int val;
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			source.getVal(i, j, val);
			charImage[i * cols + j] = (unsigned char)val;
		}
	}

	// Write out pixel values as unsigned char
	outfile.write(reinterpret_cast<char*>(charImage), (rows * cols) * sizeof(unsigned char));

	// Test for invalid write operation
	if(outfile.fail()) {
		std::cout << "Can't write image " << fdest << std::endl;
		exit(1);
	}

	// Close file
	outfile.close();

	// Delete temporary pixel stoarge memory allocation
	delete(charImage);
}

#endif
