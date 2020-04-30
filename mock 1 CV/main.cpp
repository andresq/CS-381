#include <iostream> 
#include <string>
#include <fstream>
using namespace std;

class Image{
    public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;

    int** imageAry;

    void loadImageAry(ifstream& inFile){
        imageAry = new int*[numRows];
        for(int i = 0; i < numRows; i++){
            imageAry[i] = new int[numCols];
        }

        int input;

        for(int i = 0; i < numRows; i++){
            for(int j= 0; j < numCols; j++){
                inFile >> input;
                imageAry[i][j] = input;
                }
        }
    }


    void sumRows(ofstream& outFile1){
        int sum = 0;
        for(int row = 0; row < numRows; row++){
            outFile1 << row << ": ";
            for(int col = 0; col < numCols; col++){
                sum += imageAry[row][col];
            }
            outFile1 << sum << endl;
            sum = 0;
        }
    }


    void sumCols(ofstream& outFile2){
        int sum = 0;
        for(int col = 0; col < numCols; col++){
            outFile2 << col << ": ";
            for(int row = 0; row < numRows; row++){
                sum += imageAry[row][col];
            }
            outFile2 << sum << endl;
            sum = 0;
        }
    }




};


int main(int argc, char* argv[]){

    ifstream inFile(argv[1]);
    ofstream outFile1(argv[2]);
    ofstream outFile2(argv[3]);

    Image image;
    inFile >> image.numRows;
    inFile >> image.numCols;
    inFile >> image.minVal;
    inFile >> image.maxVal;

    image.loadImageAry(inFile);

    // Header output
    outFile1 << image.numRows << " " << image.numRows << " " << image.minVal << " " << image.maxVal << endl;
    outFile2 << image.numRows << " " << image.numRows << " " << image.minVal << " " << image.maxVal << endl;

    image.sumRows(outFile1);
    image.sumCols(outFile2);



    inFile.close();
    outFile1.close();
    outFile2.close();

}