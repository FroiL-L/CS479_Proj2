#ifndef _IMAGETYPE_HPP_
#define _IMAGETYPE_HPP_

// Libraries
#include <vector>

/* ImageType:
 * 	Declaration for class that handles image interactions.
 */
class ImageType {
	private:
		int r, c; 			// Rows, Columns
		int maxPixVal;			// Max pixel value
		std::vector<int> pixelValue;	// List of pixel values
	public:
		/* Constructors / Destructors */
		ImageType();
		~ImageType();

		/* Public Getter Methods */
		void getImageInfo(int& rows, int& cols, int& maxLevel);
		void getVal(int row, int col, int& value);

		/* Public Setter Methods */
		void setImageInfo(int rows, int cols, int maxLevel);
		void setVal(int row, int col, int value);

};

// ImageType:
// 	Implementation for ImageType class.

/* ImageType():
 * 	Default constructor for ImageType. Initializes all values to zero.
 */
ImageType::ImageType() :
	r(0), c(0), maxPixVal(0) {
}

/* ~ImageType():
 * 	Default destructor for ImageType.
 */
ImageType::~ImageType() {
}

/* getImageInfo():
 * 	Gets the metadata information for the contained image.
 * args:
 * 	@rows: Location to output the number of rows in the image.
 * 	@cols: Location to output the number of columns in the image.
 * 	@maxLevel: Location to output the maximum pixel value allowed.
 * return:
 * 	void
 */
void ImageType::getImageInfo(int& rows, int& cols, int& maxLevel) {
	rows = this->r;
	cols = this->c;
	maxLevel = this->maxPixVal;
}

/* getVal():
 * 	Get the pixel value at a given location.
 * args:
 * 	@row: The row to look for the pixel value.
 * 	@col: The column to look for the pixel value.
 * 	@value: Location to store the pixel value.
 * return:
 * 	void
 */
void ImageType::getVal(int row, int col, int& value) {
	// Test for invalid row number
	if(row >= this->r) return;

	// Test for invalid column number
	if(col >= this->c) return;

	value = this->pixelValue[(row * this->c) + col];
	//value = this->pixelValue[row][col];
}

/* setImageInfo():
 * 	Sets the metadata information for the contained image.
 * args:
 * 	@rows: The number of rows to assign to image.
 * 	@cols: The number of columns to assign to image.
 * 	@maxLevel: The maximum pixel value allowed.
 * return:
 * 	void
 */
void ImageType::setImageInfo(int rows, int cols, int maxLevel) {
	// Update attributes
	this->r = rows;
	this->c = cols;
	this->maxPixVal = maxLevel;

	// Update pixel storage
	this->pixelValue.resize(rows * cols);
}

/* setVal():
 * 	Sets the pixel value at a given location.
 * args:
 * 	@row: The row to access.
 * 	@col: The column to access.
 * 	@value: The value to set the pixel value as.
 * return:
 * 	void
 */
void ImageType::setVal(int row, int col, int value) {
	// Test for invalid row
	if(row > this->r) return;

	// Test for invalid column
	if(col > this->c) return;

	// Test if value exceeds maximum
	if(value > this->maxPixVal) {
		value = this->maxPixVal;
	}

	this->pixelValue[(row * this->c) + col] = value;
}

#endif
