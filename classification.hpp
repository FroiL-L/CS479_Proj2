/* classification.hpp:
 * 	A library that contains tools for classification.
 * authors:
 *	Froilan Luna-Lopez
 *  Aaron Ramirez
 *		University of Nevada, Reno
 *		CS 479 - Pattern Recognition
 * date:
 * 	27 February 2023
 */

#ifndef _CLASSIFICATION_H_
#define _CLASSIFICATION_H_
//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include "Eigen/Dense"

// Classifier.cpp

void bayesCaseOne(Eigen::Matrix<float, 2, 1> muOne, Eigen::Matrix<float, 2, 1> muTwo, float varianceOne, float varianceTwo, float priorOne, float priorTwo, const std::string& sourceFile, const std::string& destFile) {
    std::ifstream inFile;
    inFile.open(sourceFile.c_str());

    // Open file for writing classifications to
    std::ofstream outFile;
    outFile.open(destFile.c_str());

    float xf, yf, _f;
    while (inFile >> xf >> yf >> _f) {
        Eigen::Matrix<float, 2, 1> x;
        x << xf, yf;

        float discrimOne = (((1.0 / varianceOne) * muOne).transpose() * x) - (1.0 / (2 * varianceOne)) * pow(muOne.norm(), 2);
        float discrimTwo = (((1.0 / varianceTwo) * muTwo).transpose() * x) - (1.0 / (2 * varianceTwo)) * pow(muTwo.norm(), 2);

        if (priorOne != priorTwo) {
            discrimOne += log(priorOne);
            discrimTwo += log(priorTwo);
        }

        // Save choice
        outFile << xf << " " << yf << " ";
        if (discrimOne < discrimTwo) {
            outFile << "2";
        } else {
            outFile << "1";
        }
        outFile << std::endl;
    }

    inFile.close();
    outFile.close();

}


void bayesCaseTwo(const Eigen::Matrix<float, 2, 1>& muOne, const Eigen::Matrix<float, 2, 1>& muTwo, const Eigen::Matrix2f& sigmaOne, const Eigen::Matrix2f& sigmaTwo, float priorOne, float priorTwo, const std::string& sourceFile, const std::string& destFile) {
    std::ifstream inFile;
    inFile.open(sourceFile);

    // Open file for writing classifications to
    std::ofstream outFile;
    outFile.open(destFile);

    float xf, yf, _f;
    while(inFile >> xf >> yf >> _f) {
        Eigen::Vector2f x(xf, yf);
        float discrimOne = ((sigmaOne.inverse() * muOne).transpose() * x)(0) - (0.5 * muOne.transpose() * sigmaOne.inverse() * muOne);
        float discrimTwo = ((sigmaTwo.inverse() * muTwo).transpose() * x)(0) - (0.5 * muTwo.transpose() * sigmaTwo.inverse() * muTwo);

        if (priorOne != priorTwo) {
            discrimOne += log(priorOne);
            discrimTwo += log(priorTwo);
        }

        // Save choice
        outFile << xf << " " << yf << " ";
        if(discrimOne < discrimTwo) {
            outFile << "1";
        }
        else {
            outFile << "2";
        }
        outFile << std::endl;
    }

    // Close files
    inFile.close();
    outFile.close();

}


void bayesCaseThree(const Eigen::Vector2f muOne, const Eigen::Vector2f muTwo, 
    const Eigen::Matrix2f sigmaOne, const Eigen::Matrix2f sigmaTwo, float priorOne, float priorTwo,
    const std::string& sourceFile, const std::string& destFile) {

    std::ifstream inFile;
    inFile.open(sourceFile);

    // Open file for writing classifications to
    std::ofstream outFile;
    outFile.open(destFile);

    float xf, yf, _f;
    while (inFile >> xf >> yf >> _f) {
        Eigen::Vector2f x(xf, yf);
        float discrimOne = (x.transpose() * (-0.5 * sigmaOne.inverse()) * x) + ((sigmaOne.inverse() * muOne).transpose() * x)(0) +(-0.5 * muOne.transpose() * sigmaOne.inverse() * muOne) + (-0.5 * log(sigmaOne.determinant()));
        float discrimTwo = (x.transpose() * (-0.5 * sigmaTwo.inverse()) * x) + ((sigmaTwo.inverse() * muTwo).transpose() * x)(0) +(-0.5 * muTwo.transpose() * sigmaTwo.inverse() * muTwo) + (-0.5 * log(sigmaTwo.determinant()));

        if (priorOne != priorTwo) {
            discrimOne += log(priorOne);
            discrimTwo += log(priorTwo);
        }

        // Save choice
        outFile << xf << " " << yf << " ";
        if (discrimOne < discrimTwo) {
            outFile << "2";
        }
        else {
            outFile << "1";
        }
        outFile << std::endl;
    }

    // Close files
    inFile.close();
    outFile.close();
}

/* classifyEuclidean():
 * 	Classifies points within a file between two classes, 1 and 2.
 * 	Classification is based on minimizing the Euclidean distance
 * 	with the feature and mean values.
 * args:
 * 	@means1: Set of mean values for features in class 1.
 * 	@means2: Set of mean vluaes for features in class 2.
 * 	@sourceFile: Path to the file with the data to classify.
 * 	@destFile: Path to the file to write the classifications to.
 * return:
 * 	@destFile
 */
void classifyEuclidean(Eigen::Matrix<float, 2, 1> means1,
	Eigen::Matrix<float, 2, 1> means2,
	std::string sourceFile,
	std::string destFile) {
	// Open file for reading feature sets
	std::ifstream inFile;
	inFile.open(sourceFile);

	// Open file for writing classifications to
	std::ofstream outFile;
	outFile.open(destFile);

	// Read features from source file
	float xf, yf, _f;
	while(inFile >> xf >> yf >> _f) {
		// Compute distance from first mean
		float dist1 = std::pow((xf - means1(0, 0)), 2)
			+ std::pow((yf - means1(1, 0)), 2);

		// Compute distance from second mean
		float dist2 = std::pow((xf - means2(0, 0)), 2)
			+ std::pow((yf - means2(1, 0)), 2);

		// Save choice
		outFile << xf << " " << yf << " ";
		if(dist1 < dist2) {
			outFile << "1";
		}
		else {
			outFile << "2";
		}
		outFile << std::endl;
	}

	// Close files
	inFile.close();
	outFile.close();
}

/* misclassifyCount():
 * 	Counts the number of misclassifications between a file containing
 * 	the true classes of data points and another that contains the
 * 	expected classification. This function only supports classes of
 * 	type 1 or 2 and points must be one-to-one in each file to work
 * 	properly.
 * args:
 * 	@trueSrc: The path to the file containing the true classifications.
 * 	@classSrc: The path to the file containing the expected
 * 		classifications.
 * 	@counts: The location to store misclassification counts.
 * return:
 * 	@counts.
 */
void misclassifyCount(std::string trueSrc, std::string classSrc, std::vector<int>& counts) {
	// Open files for reading
	std::ifstream trueFile;
	std::ifstream classFile;
	
	trueFile.open(trueSrc);
	classFile.open(classSrc);

	// Iterate through values in files and count misclassifications
	float trueVal, classVal;
	while(trueFile >> trueVal >> trueVal >> trueVal
			&& classFile >> classVal >> classVal >> classVal) {
		if(trueVal != classVal) {
			if(trueVal == 1) {
				counts[0] += 1;
			}
			else if(trueVal == 2) {
				counts[1] += 1;
			}
		}
	}

	// Close files
	trueFile.close();
	classFile.close();

}


#endif
