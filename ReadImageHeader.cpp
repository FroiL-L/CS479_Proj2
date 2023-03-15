/* ReadImageHeader.cpp:
 * 	Contains functionality for inputting the header values contained
 * 	within PGM and PPM images into given variables.
 * Credit:
 * 	Dr. George Bebis
 * 		The source code was obtained via his website at
 * 		https://www.cse.unr.edu/~bebis/CS479/ .
 * 		It has been slightly modified (renaming functions,
 * 		adding comments, etc) for our intents and purposes.
 * Date:
 * 	15 March 2023
 */


// Libraries
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#include "image.h"

// Functions

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
void readImageHeader(char fname[], int& N, int& M, int& Q, bool& type)
{
 int i, j;
 unsigned char *charImage;
 char header [100], *ptr;
 ifstream ifp;

 ifp.open(fname, ios::in | ios::binary);

 if (!ifp) {
   cout << "Can't read image: " << fname << endl;
   exit(1);
 }

 // read header

 type = false; // PGM

 ifp.getline(header,100,'\n');
 if ( (header[0] == 80) &&  /* 'P' */
      (header[1]== 53) ) {  /* '5' */
      type = false;
 }
 else if ( (header[0] == 80) &&  /* 'P' */
      (header[1] == 54) ) {        /* '6' */
      type = true;
 } 
 else {
   cout << "Image " << fname << " is not PGM or PPM" << endl;
   exit(1);
 }

ifp.getline(header,100,'\n');
 while(header[0]=='#')
   ifp.getline(header,100,'\n');

 M=strtol(header,&ptr,0);
 N=atoi(ptr);

 ifp.getline(header,100,'\n');

 Q=strtol(header,&ptr,0);

 ifp.close();
}
