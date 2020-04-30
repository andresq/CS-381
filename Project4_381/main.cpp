#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//DEBUGSTUFF
void printArrayDebug(int** array, int rows, int cols){
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      cout << array[i][j] << " ";
    }
    cout << endl;
  }
}


// Prototypes
void zero2DAry(int** array, int rows, int cols);
void loadImage(fstream& imgFile, int** zeroFrameAry, int rows, int cols);
void loadImage(fstream& imgFile, int** zeroFrameAry, int row, int col, int rowFrameSize, int colFrameSize);
void prettyPrint(int** array, fstream& outFile, int rows, int cols);
void loadStruct(fstream& structFile, int** structAry, int rows, int cols);
void ComputeDilation(int** zeroFrameAry, int** morphAry, int rowFrameSize, int colFrameSize, int numStructRows, int numStructCols, int rowOrigin, int colOrigin);
void dilation(int i, int j, int** morphAry, int rowOrigin, int colOrigin);
void ComputeErosion(int** zeroFrameAry, int** morphAry, int** structAry, int rowFrameSize, int colFrameSize, int numImgRows, int numImgCols, int numStructRows,int numStructCols, int rowOrigin, int colOrigin);
void erosion(int i, int j, int** morphAry, int** structAry, int** zeroFrameAry, int rowOrigin, int colOrigin);
void AryToFile(int** array, fstream& outFile, int rows, int cols, int extraRows, int extraCols, int min, int max);
void ComputeOpening(int** zeroFrameAry, int** morphAry, int** structAry, int rowFrameSize, int colFrameSize, int numImgRows, int numImgCols,int numStructRows,int numStructCols, int rowOrigin, int colOrigin, int extraRows, int extraCols);
void ComputeClosing(int** zeroFrameAry, int** morphAry, int** structAry, int rowFrameSize, int colFrameSize, int numImgRows, int numImgCols,int numStructRows,int numStructCols, int rowOrigin, int colOrigin, int extraRows, int extraCols);


int main(int argc, char* argv[]) {
  fstream imgFile(argv[1]); // Image
  fstream structFile(argv[2]); // Structuring element
  fstream dilateOutFile(argv[3], fstream::out);
  fstream erodeOutFile(argv[4], fstream::out);
  fstream openingOutFile(argv[5], fstream::out);
  fstream closingOutFile(argv[6], fstream::out);
  fstream prettyPrintFile(argv[7], fstream::out);
  // Varibles
  int numImgRows, numImgCols, imgMin, imgMax;
  int numStructRows, numStructCols, structMin, structMax, rowOrigin, colOrigin;
  // Reading from imgFile
  imgFile >> numImgRows;
  imgFile >> numImgCols;
  imgFile >> imgMin;
  imgFile >> imgMax;
  // Reading from structFile
  structFile >> numStructRows;
  structFile >> numStructCols;
  structFile >> structMin;
  structFile >> structMax;
  structFile >> rowOrigin;
  structFile >> colOrigin;
  // Initiazing array variables
  int rowFrameSize = numStructRows/2, colFrameSize = numStructCols/2;
  int extraRows = rowFrameSize*2, extraCols = colFrameSize*2;


  int **zeroFrameAry = new int*[numImgRows+extraRows];
  for(int i = 0; i < numImgRows+extraRows; i++){
    zeroFrameAry[i] = new int[numImgCols+extraCols];
  }

  int **morphAry = new int*[numImgRows+extraRows];
  for(int i = 0; i < numImgRows+extraRows; i++){
    morphAry[i] = new int[numImgCols+extraCols];
  }

  int **structAry = new int*[numStructRows];
  for(int i = 0; i < numStructRows; i++){
    structAry[i] = new int[numStructCols];
  }



  // Loading Image
  zero2DAry(zeroFrameAry, numImgRows+extraRows, numImgCols+extraCols);
  loadImage(imgFile, zeroFrameAry, numImgRows+extraRows, numImgCols+extraCols, rowFrameSize, colFrameSize);
  prettyPrint(zeroFrameAry, prettyPrintFile, numImgRows+extraRows, numImgCols+extraCols);
  // Loading Structuring Element
  zero2DAry(structAry, numStructRows, numStructCols);
  loadStruct(structFile, structAry, numStructRows, numStructCols);
  prettyPrint(structAry, prettyPrintFile, numStructRows, numStructCols);
  // Dilation
  zero2DAry(morphAry, numImgRows+extraRows, numImgCols+extraCols);
  ComputeDilation(zeroFrameAry, morphAry,  rowFrameSize, colFrameSize, numImgRows, numImgCols, rowOrigin, colOrigin);
  AryToFile(morphAry, dilateOutFile,  numImgRows,  numImgCols, extraRows, extraCols, imgMin,  imgMax);
  prettyPrint(morphAry, prettyPrintFile, numImgRows+extraRows, numImgCols+extraCols);
  // Erosion
  zero2DAry(morphAry, numImgRows+extraRows, numImgCols+extraCols);
  ComputeErosion(zeroFrameAry, morphAry, structAry, rowFrameSize, colFrameSize, numImgRows, numImgCols, numStructRows, numStructCols, rowOrigin, colOrigin);
  AryToFile(morphAry, erodeOutFile,  numImgRows,  numImgCols, extraRows, extraCols, imgMin,  imgMax);
  prettyPrint(morphAry, prettyPrintFile, numImgRows+extraRows, numImgCols+extraCols);
  // Opening
  zero2DAry(morphAry, numImgRows+extraRows, numImgCols+extraCols);
  ComputeOpening(zeroFrameAry, morphAry, structAry, rowFrameSize, colFrameSize, numImgRows, numImgCols, numStructRows, numStructCols, rowOrigin, colOrigin, extraRows, extraCols);
  AryToFile(morphAry, openingOutFile,  numImgRows,  numImgCols, extraRows, extraCols, imgMin,  imgMax);
  prettyPrint(morphAry, prettyPrintFile, numImgRows+extraRows, numImgCols+extraCols);
  // Closing
  zero2DAry(morphAry, numImgRows+extraRows, numImgCols+extraCols);
  ComputeClosing(zeroFrameAry, morphAry, structAry, rowFrameSize, colFrameSize, numImgRows, numImgCols, numStructRows, numStructCols, rowOrigin, colOrigin, extraRows, extraCols);
  AryToFile(morphAry, closingOutFile,  numImgRows,  numImgCols, extraRows, extraCols, imgMin,  imgMax);
  prettyPrint(morphAry, prettyPrintFile, numImgRows+extraRows, numImgCols+extraCols);


  //Closing all files
  imgFile.close();
  structFile.close();
  dilateOutFile.close();
  erodeOutFile.close();
  openingOutFile.close();
  closingOutFile.close();
  prettyPrintFile.close();
}

void ComputeClosing(int** zeroFrameAry, int** morphAry, int** structAry, int rowFrameSize, int colFrameSize, int numImgRows, int numImgCols,int numStructRows,int numStructCols, int rowOrigin, int colOrigin, int extraRows, int extraCols){
  // Dilation then Erosion
  //creating a tempArray
  int **tempArray = new int*[numImgRows+extraRows];
  for(int i = 0; i < numImgRows+extraRows; i++){
    tempArray[i] = new int[numImgCols+extraCols];
  }
  zero2DAry(tempArray, numImgRows+extraRows, numImgCols+extraCols);

  ComputeDilation(zeroFrameAry, morphAry, rowFrameSize, colFrameSize, numImgRows, numImgCols, rowOrigin, colOrigin);


  ComputeErosion(morphAry, tempArray, structAry, rowFrameSize, colFrameSize, numImgRows, numImgCols, numStructRows, numStructCols, rowOrigin, colOrigin);


  // Copy tempArray back to morphAry
  for(int i = 0; i < numImgRows+extraRows; i++){
    for(int j = 0; j < numImgCols+extraCols; j++){
      morphAry[i][j] = tempArray[i][j];
    }
  }

  // deallocate tempArray
  for(int i = 0; i < numImgCols+extraCols; i++) { delete[] tempArray[i];}
  delete[] tempArray;
}

void ComputeOpening(int** zeroFrameAry, int** morphAry, int** structAry, int rowFrameSize, int colFrameSize, int numImgRows, int numImgCols,int numStructRows,int numStructCols, int rowOrigin, int colOrigin, int extraRows, int extraCols){
  // Erosion then Dilation
  //creating a tempArray
  int **tempArray = new int*[numImgRows+extraRows];
  for(int i = 0; i < numImgRows+extraRows; i++){
    tempArray[i] = new int[numImgCols+extraCols];
  }
  zero2DAry(tempArray, numImgRows+extraRows, numImgCols+extraCols);


  ComputeErosion(zeroFrameAry, morphAry, structAry, rowFrameSize, colFrameSize, numImgRows, numImgCols, numStructRows, numStructCols, rowOrigin, colOrigin);

  ComputeDilation(morphAry, tempArray, rowFrameSize, colFrameSize, numImgRows, numImgCols, rowOrigin, colOrigin);

  // Copy tempArray back to morphAry
  for(int i = 0; i < numImgRows+extraRows; i++){
    for(int j = 0; j < numImgCols+extraCols; j++){
      morphAry[i][j] = tempArray[i][j];
    }
  }

  // deallocate tempArray
  for(int i = 0; i < numImgCols+extraCols; i++) { delete[] tempArray[i];}
  delete[] tempArray;
}

// Functions
void AryToFile(int** array, fstream& outFile, int rows, int cols, int extraRows, int extraCols, int min, int max){
  outFile << rows << " " << cols << " " << min << " " << max << endl;
  int rowPadding = extraRows/2;
  int colPadding = extraCols/2;
  for(int i = rowPadding; i < rows+rowPadding; i++){
    for(int j = colPadding; j < cols+colPadding; j++){
      if(array[i][j] == 0){
        outFile << "0 ";
      } else {
        outFile << array[i][j] << " ";
      }
    }
    outFile << endl;
  }
}

void erosion(int i, int j, int** morphAry, int** structAry, int** zeroFrameAry, int numStructRows, int numStructCols, int rowOrigin, int colOrigin){
  bool match = true;
  int rOffset = i - rowOrigin;
  int cOffset = j - colOrigin;
  int r , c;

  r = 0;

  while(r < numStructRows && match){
    c = 0;
    while (c < numStructCols && match){
      if(structAry[r][c] > 0 && zeroFrameAry[rOffset+r][cOffset+c] == 0)  { match = false;}
      c++;
    }
    r++;
  }

  if(match){ morphAry[i][j] = 1;}
}


void dilation(int i, int j, int** morphAry, int rowOrigin, int colOrigin){
  for(int a = 0; a < rowOrigin+1; a++){
    for(int b = 0; b < colOrigin+1; b++){
      morphAry[i-a][j-b] = 1;
    }
  }
  //before i,j
  for(int a = 0; a < rowOrigin+1; a++){
    for(int b = 0; b < colOrigin+1; b++){
      morphAry[i+a][j+b] = 1;
    }
  }


}


void ComputeErosion(int** zeroFrameAry, int** morphAry, int** structAry, int rowFrameSize, int colFrameSize, int numImgRows, int numImgCols,int numStructRows,int numStructCols, int rowOrigin, int colOrigin){
  int i, j;
  i = rowFrameSize;
  while(i<numImgRows+rowFrameSize){
    j = colFrameSize;
    while(j<numImgCols+colFrameSize){
      if(zeroFrameAry[i][j] > 0) {erosion(i, j, morphAry, structAry, zeroFrameAry, numStructRows, numStructCols, rowOrigin, colOrigin);}
      j++;
    }
    i++;
  }
}


void ComputeDilation(int** zeroFrameAry, int** morphAry, int rowFrameSize, int colFrameSize, int numImgRows, int numImgCols, int rowOrigin, int colOrigin){
  int i, j;
  i = rowFrameSize;
  while(i<numImgRows+rowFrameSize){
    j = colFrameSize;
    while(j<numImgCols+colFrameSize){
      if(zeroFrameAry[i][j] > 0) {dilation(i, j, morphAry, rowOrigin, colOrigin);}
      j++;
    }
    i++;
  }
}


void loadStruct(fstream& structFile, int** structAry, int rows, int cols){
  int value;
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      structFile >> value;
      structAry[i][j] = value;
    }
  }
}

void prettyPrint(int** array, fstream& outFile, int rows, int cols){
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      if(array[i][j] == 0){
        outFile << ". ";
      } else {
        outFile << array[i][j] << " ";
      }
    }
    outFile << endl;
  }

  outFile << endl;
}

void loadImage(fstream& imgFile, int** zeroFrameAry, int rows, int cols, int rowFrameSize, int colFrameSize){
  int value;
  for(int i = rowFrameSize; i < rows-rowFrameSize; i++){
    for(int j = colFrameSize; j < cols-colFrameSize; j++){
      imgFile >> value;
      zeroFrameAry[i][j] = value;
    }
  }
}

void zero2DAry(int** array, int rows, int cols){
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      array[i][j] = 0;
    }
  }
}
