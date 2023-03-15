/* image.h:
 * 	Header containing the declarations for the ImageType class.
 * Credit:
 * 	Dr. George Bebis
 * 		The source code was obtained via his website at
 * 		https://www.cse.unr.edu/~bebis/CS479/ .
 * 		It has been slightly modified by adding comments
 * 		for our intents and purposes.
 * Date:
 * 	15 March 2023
 */

#ifndef IMAGE_H
#define IMAGE_H

// a simple example - you would need to add more funtions

class ImageType {
 public:
   ImageType();
   ImageType(int, int, int);
   void getImageInfo(int&, int&, int&);
   void setImageInfo(int, int, int);
   void setPixelVal(int, int, int);
   void getPixelVal(int, int, int&);
 private:
   int N, M, Q;       // N: Rows; M: Columns; Q: Max. pixel value;
   int **pixelValue;  // pixelValue: List of pixel values.
};

#endif
