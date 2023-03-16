/* rgb.h:
 * 	Header file containing the declaration for RGB struct.
 * Credit:
 * 	Dr. George Bebis
 * 		The source code was obtained via his website at
 * 		https://www.cse.unr.edu/~bebis/CS479/ .
 * 		It has been slightly modified by adding comments
 * 		for our intents and purposes.
 * Date:
 * 	15 March 2023
 */

#ifndef RGB_H
#define RGB_H

// an example -- you would need to add more functions

struct RGB {
  RGB();
  RGB(int, int, int);
  RGB& operator=(RGB);
  int r, g, b;    // r: Red value; g: Green value; b: Blue value;
};

#endif
