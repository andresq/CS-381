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

    int** imgAry;
    int** CCAry;

    Image(ifstream& labelFile){
        labelFile >> numRows;
        labelFile >> numCols;
        labelFile >> minVal;
        labelFile >> maxVal;

        imgAry = new int*[numRows+2];
        for(int i = 0; i < numRows+2; i++){
            imgAry[i] = new int[numCols+2];
        }

        CCAry = new int*[numRows+2];
        for(int i = 0; i < numRows+2; i++){
            CCAry[i] = new int[numCols+2];
        }

        zeroFramed();
    }

    void zeroFramed(){
        for(int i = 0; i < numRows+2; i++){
            for(int j = 0; j < numCols+2; j++){
                imgAry[i][j] = 0;
                CCAry[i][j] = 0;
            }
        }
    }

    void loadImage(ifstream& inFile){
        int value;
        for(int i = 1; i < numRows+1; i++){
            for(int j = 1; j < numCols+1; j++){
                inFile >> value;
                imgAry[i][j] = value;
            }
        }
    }
};

class connectCC{
    public:
    int label;
    int numPixels;
    int minRow;
    int minCol;
    int maxRow;
    int maxCol;

    connectCC(int labelNum, int pixelNum, int rowMin, int colMin, int rowMax, int colMax){
        label = labelNum;
        numPixels = pixelNum;
        minRow = rowMin;
        minCol = colMin;
        maxRow = rowMax;
        maxCol = colMax;
    }

    void clearCC(int** CCAry, int numRows, int numCols){
        for(int i = 0; i < numRows+2; i++){
            for(int j = 0; j < numCols+2; j++){
                CCAry[i][j] = 0;
            }
        }
    }

    void loadCC(int** imgAry, int** CCAry){
        for(int i = minRow+1; i < maxRow+2; i++){
            for(int j= minCol+1; j < maxCol+2; j++){
                if(imgAry[i][j] > 0){
                    CCAry[i][j] = label;
                }
            }
        }
    }
};

class ChainCode{
    public:
    class Point{
        public:
        int row;
        int col;
        Point(int x, int y){
            row = x;
            col = y;
        }
    };

    Point neighborCoord[8] = {Point(-1,-1), Point(-1,-1), Point(-1,-1), Point(-1,-1), Point(-1,-1), Point(-1,-1), Point(-1,-1), Point(-1,-1) };
    Point startP = Point(-1,-1);
    Point currentP = Point(-1,-1);
    Point nextP = Point(-1,-1);
    int lastQ;
    int zeroTable[8] = {6,0,0,2,2,4,4,6};
    int nextDir;
    int pChainDir;

    ChainCode(){

    }

    void getChainCode(connectCC CC, int** CCAry, ofstream& ChainCodeFile, ofstream& debugFile){
        int label = CC.label;
        
        for(int iRow = CC.minRow+1; iRow < CC.maxRow+2; iRow++){
            for(int jCol = CC.minCol+1; jCol < CC.maxCol+2; jCol++){
                if(CCAry[iRow][jCol] == label){
                    ChainCodeFile << label << " " << iRow << " " << jCol << " " << CCAry[iRow][jCol];
                    debugFile     << label << " " << iRow << " " << jCol << " " << CCAry[iRow][jCol];
                    startP = Point(iRow, jCol);
                    currentP = Point(iRow, jCol);
                    lastQ = 4;
                }

                int nextQ = (lastQ+1) % 8;

                pChainDir = findNextP(currentP, nextQ, nextP);








            }
        }
    }

    int findNextP(Point currentP, int nextQ, Point nextP){
        loadNeighborCoord(currentP);
        int chainDir = -1;
        



        return -33;
    }

    void loadNeighborCoord(Point p){
        int i = p.row;
        int j = p.col;

        neighborCoord[0] = Point(i,j+1);
        neighborCoord[1] = Point(i-1,j+1);
        neighborCoord[2] = Point(i-1,j);
        neighborCoord[3] = Point(i-1,j-1);
        neighborCoord[4] = Point(i,j-1);
        neighborCoord[5] = Point(i+1,j-1);
        neighborCoord[6] = Point(i+1,j);
        neighborCoord[7] = Point(i+1,j+1);
    }

    void prettyPrint(ofstream& outFile){

    }


};

int main(int argc, char* argv[]){
    ifstream labelFile(argv[1]);
    ifstream propFile(argv[2]);
    ofstream ChainCodeFile(argv[3]);
    ofstream debugFile(argv[4]);

    Image image(labelFile);
    ChainCodeFile << image.numRows << " " << image.numCols << " " << image.minVal << " " << image.maxVal << endl;
    debugFile << image.numRows << " " << image.numCols << " " << image.minVal << " " << image.maxVal << endl;
    image.loadImage(labelFile);

    //DEBUGSTUFF
    // for(int i = 0; i < image.numRows+2; i++){
    //         for(int j = 0; j < image.numCols+2; j++){
    //             cout << image.CCAry[i][j] << " ";
    //         }
    //         cout << endl;
    //     }

    int dummyRead; // to get the correct spot
    propFile >> dummyRead; propFile >> dummyRead; propFile >> dummyRead; propFile >> dummyRead; // imageHeader
    int totalCC;
    propFile >> totalCC; // Is this used?

    int label, numPixels, minRow, minCol, maxRow, maxCol;

    // StartOfLoop


    

    propFile >> label; 
    propFile >> numPixels; 
    propFile >> minRow; 
    propFile >> minCol;
    propFile >> maxRow;
    propFile >> maxCol;
    connectCC CC(label, numPixels, minRow, minCol, maxRow, maxCol);
    // cout << CC.label << " " << CC.numPixels << " " << CC.minRow << " " << CC.minCol << " " << CC.maxRow << " " << CC.maxCol << endl;

    CC.clearCC(image.CCAry, image.numRows, image.numCols);
    CC.loadCC(image.imgAry, image.CCAry);

    //DEBUGSTUFF
    // for(int i = 0; i < image.numRows+2; i++){
    //         for(int j = 0; j < image.numCols+2; j++){
    //             cout << image.CCAry[i][j] << " ";
    //         }
    //         cout << endl;
    //     }














    labelFile.close();
    propFile.close();
    ChainCodeFile.close();
    debugFile.close();
}