#include <iostream>
#include <string>
#include <fstream>
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

void print3x3(int** Ary, int i, int j){
  int count =0;
  for(int a = -1; a < 2 ; a++){
    for(int b = -1; b < 2; b++){
      if(a == 0 && b==0){
        // Dont count middle pixel
      } else {
        if(Ary[i+a][j+b] > 0){count++;}
      }
    }
  }
}



// Prototypes
void zeroFrame(int** array, int rows, int cols);
void loadImage(ifstream& inFile, int** Ary, int rows, int cols);
void prettyPrint(int** Ary, ofstream& outFile, int rows, int cols);
void doThinning(int** firstAry, int** secondAry, int& changeFlag, int numRows, int numCols);
void northThinning(int** firstAry, int** secondAry, int& changeFlag, int numRows, int numCols);
void southThinning(int** firstAry, int** secondAry, int& changeFlag, int numRows, int numCols);
void westThinning(int** firstAry, int** secondAry, int& changeFlag, int numRows, int numCols);
void eastThinning(int** firstAry, int** secondAry, int& changeFlag, int numRows, int numCols);
void copyArys(int** firstAry, int** secondAry, int rows, int cols);
bool check3n4Conditions(int** Ary, int i, int j, int minNum);


int main(int argc, char* argv[]){
  // CLI files
  ifstream inFile(argv[1]);
  ofstream outFile1(argv[2]);
  ofstream outFile2(argv[3]);
  // Variables
  // 1
  int numRows, numCols, minVal, maxVal;
  int changeFlag, cycleCount = 1;
  inFile >> numRows;
  inFile >> numCols;
  inFile >> minVal;
  inFile >> maxVal;

  outFile1 << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;

  // allocating firstAry and secondAry
  int **firstAry = new int*[numRows+2];
  for(int i = 0; i < numRows+2; i++){
    firstAry[i] = new int[numCols+2];
  }
  int **secondAry = new int*[numRows+2];
  for(int i = 0; i < numRows+2; i++){
    secondAry[i] = new int[numCols+2];
  }

  // 2
  zeroFrame(firstAry, numRows+2, numCols+2);
  zeroFrame(secondAry, numRows+2, numCols+2);

  // 3
  loadImage(inFile, firstAry, numRows, numCols);

  // DEBUGSTUFF
  // printArrayDebug(firstAry, numRows+2, numCols+2);
  // print3x3(firstAry, 5, 11);

  // 4
  outFile2 << "prettyPrint of image before thinning: " << endl;
  prettyPrint(firstAry, outFile2, numRows, numCols);



  
  changeFlag = 1; // run atleast once

    // 5 - 7
  while(changeFlag > 0){
        changeFlag = 0;
    doThinning(firstAry, secondAry, changeFlag, numRows, numCols);
    outFile2 << "Result of thinning cycle " << cycleCount-1 << ": " << endl;
    prettyPrint(firstAry, outFile2, numRows, numCols);
    cycleCount++;
  }// 8
    
    
    // 9
    
    
    for(int i = 1; i < numRows+1; i++){
     for(int j = 1; j < numCols+1; j++){
         outFile1 << firstAry[i][j] << " ";
     }
        outFile1 << endl;
    }





  //10
  inFile.close();
  outFile1.close();
  outFile2.close();
}



// Functions

void copyArys(int** firstAry, int** secondAry, int rows, int cols){
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      firstAry[i][j] = secondAry[i][j];
    }
  }
}


bool check3n4Conditions(int** Ary, int i, int j, int minNum){
  // Condition 3
  int condition3Counter = 0;
  for(int a = -1; a < 2 ; a++){
    for(int b = -1; b < 2; b++){
      if(a == 0 && b==0){
        // Dont count middle pixel
      } else {
        if(Ary[i+a][j+b] > 0){condition3Counter++;}
      }
    }
  }
  // No need to move on to the next step if < 4 or < 3
  if(condition3Counter < minNum){return false;}

  // Condition 4
  // Checking horizontal and vertical
  if(Ary[i][j-1] == 0 && Ary[i][j+1]== 0){
    int topCounter = 0, bottomCounter = 0;
    for(int a = -1; a < 2; a++){
      if(Ary[i-1][j+a] > 0){topCounter++;}
      if(Ary[i+1][j+a] > 0){bottomCounter++;}
    }
    if(topCounter > 0 && bottomCounter>0){return false;}
  }

  if(Ary[i-1][j] == 0 && Ary[i+1][j]== 0){
    int leftCounter = 0, rightCounter = 0;
    for(int a = -1; a < 2; a++){
      if(Ary[i+a][j-1] > 0){leftCounter++;}
      if(Ary[i+a][j+1] > 0){rightCounter++;}
    }
    if(leftCounter > 0 && rightCounter > 0){return false;}
  }

  // Checking 4 corners
  if(Ary[i-1][j-1] > 0){
    if(Ary[i-1][j] == 0 && Ary[i][j-1] == 0) {return false;}
  }
  if(Ary[i-1][j+1] > 0){
    if(Ary[i-1][j] == 0 && Ary[i][j+1] == 0) {return false;}
  }
  if(Ary[i+1][j-1] > 0){
    if(Ary[i+1][j] == 0 && Ary[i][j-1] == 0) {return false;}
  }
  if(Ary[i+1][j+1] > 0){
    if(Ary[i+1][j] == 0 && Ary[i][j+1] == 0) {return false;}
  }
  return true;
}
void eastThinning(int** firstAry, int** secondAry, int& changeFlag, int numRows, int numCols){
  for(int i = 1; i < numRows+1; i++){
    for(int j = 1; j < numCols+1; j++){
      secondAry[i][j] = firstAry[i][j];

      if(firstAry[i][j] > 0 && firstAry[i][j+1] <= 0){
        if(check3n4Conditions(firstAry, i, j, 3) == true){
          secondAry[i][j] = 0;
          changeFlag++;
        }
      }
    }
  }
}

void westThinning(int** firstAry, int** secondAry, int& changeFlag, int numRows, int numCols){
  for(int i = 1; i < numRows+1; i++){
    for(int j = 1; j < numCols+1; j++){
      secondAry[i][j] = firstAry[i][j];

      if(firstAry[i][j] > 0 && firstAry[i][j-1] <= 0){
        if(check3n4Conditions(firstAry, i, j, 3) == true){
          secondAry[i][j] = 0;
          changeFlag++;
        }
      }
    }
  }
}

void southThinning(int** firstAry, int** secondAry, int& changeFlag, int numRows, int numCols){
  for(int i = 1; i < numRows+1; i++){
    for(int j = 1; j < numCols+1; j++){
      secondAry[i][j] = firstAry[i][j];

      if(firstAry[i][j] > 0 && firstAry[i+1][j] <= 0){
        if(check3n4Conditions(firstAry, i, j, 4) == true){
          secondAry[i][j] = 0;
          changeFlag++;
        }
      }
    }
  }
}


void northThinning(int** firstAry, int** secondAry, int& changeFlag, int numRows, int numCols){
  for(int i = 1; i < numRows+1; i++){
    for(int j = 1; j < numCols+1; j++){
      secondAry[i][j] = firstAry[i][j];

      if(firstAry[i][j] > 0 && firstAry[i-1][j] <= 0){
        if(check3n4Conditions(firstAry, i, j, 4) == true){
          secondAry[i][j] = 0;
          changeFlag++;
        }
      }
    }
  }
}


void doThinning(int** firstAry, int** secondAry, int& changeFlag, int numRows, int numCols){
  // 1
  northThinning(firstAry, secondAry, changeFlag, numRows, numCols);
  copyArys(firstAry, secondAry, numRows+2, numCols+2);
  // 2
  southThinning(firstAry, secondAry, changeFlag, numRows, numCols);
  copyArys(firstAry, secondAry, numRows+2, numCols+2);
  // 3
  westThinning(firstAry, secondAry, changeFlag, numRows, numCols);
  copyArys(firstAry, secondAry, numRows+2, numCols+2);
  // 4
  eastThinning(firstAry, secondAry, changeFlag, numRows, numCols);
  copyArys(firstAry, secondAry, numRows+2, numCols+2);
}

void prettyPrint(int** Ary, ofstream& outFile, int rows, int cols){
  for(int i = 1; i < rows+1; i++){
    for(int j = 1; j < cols+1; j++){
      if(Ary[i][j] > 0){
        outFile << Ary[i][j] << " ";
      } else {
        outFile << "  ";
      }
    }
    outFile << endl;
  }
}

void loadImage(ifstream& inFile, int** Ary, int rows, int cols){
  int value;
  for(int i = 1; i < rows+1; i++){
    for(int j = 1; j < cols+1; j++){
      inFile >> value;
      Ary[i][j] = value;
    }
  }
}


void zeroFrame(int** array, int rows, int cols){
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      array[i][j] = 0;
    }
  }
}
