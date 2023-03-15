/* WriteImage.cpp:
 * 	Contains functionality for outputting PGM or PPM image
 *  information to a file.
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

/* writeImagePGM():
 *  Output the PGM image information to a given file location.
 * args:
 * 	@fname: Path to file to output image information to.
 * 	@image: The location that stores the image information.
 */
void writeImagePGM(char fname[], ImageType& image)
{
 int i, j;
 int N, M, Q;
 unsigned char *charImage;
 ofstream ofp;

 image.getImageInfo(N, M, Q);

 charImage = (unsigned char *) new unsigned char [M*N];

 // convert the integer values to unsigned char

 int val;

 for(i=0; i<N; i++)
   for(j=0; j<M; j++) {
     image.getPixelVal(i, j, val);
     charImage[i*M+j]=(unsigned char)val;
   }

 ofp.open(fname, ios::out | ios::binary);

 if (!ofp) {
   cout << "Can't open file: " << fname << endl;
   exit(1);
 }

 ofp << "P5" << endl;
 ofp << M << " " << N << endl;
 ofp << Q << endl;

 ofp.write( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));

 if (ofp.fail()) {
   cout << "Can't write image " << fname << endl;
   exit(0);
 }

 ofp.close();

 delete [] charImage;

}


/* writeImagePPM():
 * 	Output the PPM image information to a given file location.
 * args:
 * 	@fname: Path to file to output image information to.
 * 	@image: The location that stores the image information.
 */
void writeImagePPM(char fname[], ImageType& image)
{
 int i, j;
 int N, M, Q;
 unsigned char *charImage;
 ofstream ofp;

 image.getImageInfo(N, M, Q);

 charImage = (unsigned char *) new unsigned char [3*M*N];

 RGB val;

 for(i=0; i<N; i++)
   for(j=0; j<3*M; j+=3) {
     image.getPixelVal(i, j/3, val);
     charImage[i*3*M+j]=(unsigned char)val.r;
     charImage[i*3*M+j+1]=(unsigned char)val.g;
     charImage[i*3*M+j+2]=(unsigned char)val.b;
   }

 ofp.open(fname, ios::out | ios::binary);

 if (!ofp) {
   cout << "Can't open file: " << fname << endl;
   exit(1);
 }

 ofp << "P6" << endl;
 ofp << M << " " << N << endl;
 ofp << Q << endl;

 ofp.write( reinterpret_cast<char *>(charImage), (3*M*N)*sizeof(unsigned char));

 if (ofp.fail()) {
   cout << "Can't write image " << fname << endl;
   exit(0);
 }

 ofp.close();

 delete [] charImage;
}
