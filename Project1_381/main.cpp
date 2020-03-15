#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Prototypes
void ComputeHist(fstream& inFile, int* histAry);
void PrintHist(int* histAry, fstream& outFile1, int numRows, int numCols, int minVal, int maxVal);
void thresholdOp(fstream& inFile, int thresholdValue, fstream& outFile2);
void prettyPrint(fstream& outFile2, fstream& outFile3);

// MAIN
int main(int argc, char* argv[]){

	// CHANGE! TA SAID NO CONSOLE OUTPUT
	// cout << argv[1] << " is the input image file" << endl;
	// cout << argv[2] << " is the histogram file" << endl;
	// cout << argv[3] << " is the threshold image file" << endl;
	// cout << argv[4] << " is the prettyPrint thresold file" << endl;
	// Variables
	int numRows, numCols, minVal, maxVal, thresholdValue;
	fstream inFile(argv[1]);	 // Input File
	fstream outFile1(argv[2], fstream::out); // Histogram
	fstream outFile2(argv[3], fstream::out); // Threshold image
	fstream outFile3(argv[4], fstream::out); // Pretty Threshold image

	if (!inFile || !outFile1 || !outFile2 || !outFile3) {
		cout << "There is an error in opening the file" << endl;
		cout << "Exiting program" << endl;
		exit(1);
	}

	// Reading first 4 values AKA the 'header' AND threshold value
	inFile >> numRows;
	inFile >> numCols;
	inFile >> minVal;
	inFile >> maxVal;
	thresholdValue = stoi(argv[5]);

	// Printing values out to console
	// CHANGE! TA SAID NO CONSOLE OUTPUT
	// cout << "Rows: " << numRows << endl;
	// cout << "Cols: " << numCols << endl;
	// cout << "Min: " << minVal << endl;
	// cout << "Max: " << maxVal << endl;
	// cout << "Threshold: " << thresholdValue << endl;

	// Dynamically creatinng a 1-D array THEN filling them with zer0s
	int* histAry = new int[maxVal + 1];
	for(int i = 0; i < maxVal+1; i++){
		histAry[i] = 0;
	}

	// Fills histAry from inFile
	ComputeHist(inFile, histAry);
	// Prints histogram
	PrintHist(histAry, outFile1, numRows, numCols, minVal, maxVal);

	//Re-opening inFile
	inFile.close();
	inFile.open(argv[1]);

	// Prints thresold image w/ header
	thresholdOp(inFile,thresholdValue, outFile2);

	//Re-opening outFile2
	outFile2.close();
	outFile2.open(argv[3]);

	// Pretty print thresold image w/o header
	prettyPrint(outFile2, outFile3);

	// Closing all files
	inFile.close();
	outFile1.close();
	outFile2.close();
	outFile3.close();
	return 0;
}

void prettyPrint(fstream& outFile2, fstream& outFile3){
	int numRows, numCols, minVal, maxVal;
	outFile2 >> numRows;
	outFile2 >> numCols;
	outFile2 >> minVal;
	outFile2 >> maxVal;
	// Comparing only 1 and 0, no need for actual thresold value
	int pixel_val;
	for(int iRow = 0; iRow < numRows; iRow++){
		for(int iCol = 0; iCol < numCols; iCol++){
			outFile2 >> pixel_val;
			if (pixel_val > 0) {
				outFile3 << 1 << " ";
			} else {
				outFile3 << " " << " ";
			}
		}
		outFile3 << endl;
	}
}

void thresholdOp(fstream& inFile, int thresholdValue, fstream& outFile2){
	int numRows, numCols, minVal, maxVal;
	inFile >> numRows;
	inFile >> numCols;
	inFile >> minVal;
	inFile >> maxVal;
	// New header, with min-max as 0-1
	outFile2 << numRows << " " << numCols << " " << 0 << " "<< 1 << endl;
	// Write 1 if pixel value is greater than threshold value (We pick thresold)
	int pixel_val;
	for(int iRow = 0; iRow < numRows; iRow++){
		for(int iCol = 0; iCol < numCols; iCol++){
			inFile >> pixel_val;
			if (pixel_val >= thresholdValue) {
				outFile2 << 1 << " ";
			} else {
				outFile2 << 0 << " ";
			}
		}
		outFile2 << endl;
	}
}

void PrintHist(int* histAry, fstream& outFile1, int numRows, int numCols, int minVal, int maxVal){
	//Writing header
	outFile1 << numRows << " " << numCols << " " << minVal << " "<< maxVal << endl;
	// Writing histogram
	for(int i = 0; i < maxVal+1; i++){
		int totalDisplayed;
		outFile1 << i << "  (" << histAry[i] << "):";
		if( histAry[i] > 0) {
			if (histAry[i] > 60) { //Setting max + printed to be 60
				totalDisplayed = 60;
			} else {
				totalDisplayed = histAry[i];
			}
			for(int j = 0; j < totalDisplayed; j++) {
				outFile1 << "+";
			}
		}
		outFile1 << endl;
	}
	outFile1.close();
}

void ComputeHist(fstream& inFile, int* histAry) {
	int pixel_val;
	while(inFile >> pixel_val){
		histAry[pixel_val]++;
	}
}
