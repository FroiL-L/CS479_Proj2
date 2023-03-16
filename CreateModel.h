#ifndef CREATEMODEL_H_
#define CREATEMODEL_H_

/* getImage():
 * 	Gets the pixel values from an image and stores them in an
 * 	object of type ImageType.
 * args:
 * 	@fName: The path to the image to read.
 * 	@image: The object to store the image values to.
 * return:
 * 	void
 */
void getImage(char fName[], ImageType& image);


/* learnForModel():
 * 	Learns for a model by generating classified data from RGB values.
 * 	Training data with RGB values and reference data with PPM values
 * 	will be used to generate this data, and the true skin values
 * 	will be stored in a given file. If the file does not exist, 
 * 	it will be created. Otherwise, data will be appended to the
 * 	end of the file.
 * args:
 * 	@trainFName: The path to the file containing the image that 
 * 		will be used as training data.
 * 	@refFName: The path to the file containing the image that will
 * 		be used as reference data.
 * 	@modelFName: The path to the file that will be used as the
 * 		location to store the model values.
 * return:
 * 	bool: 1 for success | 0 for failure
 */
bool learnForModel(char trainFName[], char refFName[], char modelFName[]);

#endif
