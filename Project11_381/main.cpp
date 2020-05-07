#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// Data stuctures
class imagePP{
    public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;

    class box{
        public:
        int minR;
        int minC;
        int maxR;
        int maxC;
    };

    class boxNode{
        public:
        int boxType; // 1=doc box, 2=paragraph, 3= textLine
        box BBOX;
        boxNode* next;

        boxNode(){
            boxType = -99;
            next = NULL;
        }
    };

    class boxQ{
        public:
        boxNode* Qfront = new boxNode();
        boxNode* Qback = Qfront;
        void insert(boxNode* newBoxNode){
            Qback->next = newBoxNode;
            Qback = newBoxNode;
        }
    } Queue;

    int** imgAry;
    int thresholdVal;

    int* HPP;
    int* VPP;
    
    int* HPPbin;
    int* VPPbin;

    int structElement [3] = {1,1,1};
    int* HPPMorph;
    int* VPPMorph;

    int HPPruns;
    int VPPruns;

    bool isHorizontal = false;

    // Methods

    void zeroFrame(){
        for(int row = 0; row < numRows+2; row++){
            imgAry[row][0] = 0;
            imgAry[row][numCols+1] = 0;
        }

        for(int col = 0; col < numCols+2; col++){
            imgAry[0][col] = 0;
            imgAry[numRows+1][col] = 0;
        }
    }

    void loadImage(ifstream& inFile){
        int value;
        
        for(int i = 1; i < numRows+1; i++){
            for(int j= 1; j < numCols+1; j++){
                inFile >> value;
                imgAry[i][j] = value;
            }
        }
    }

    void computeHPP(){
        int sum = 0;
        for(int row = 0; row < numRows+2; row++){
            for(int col = 0 ; col < numCols+2; col++){
                if(imgAry[row][col] > 0){
                    sum++;
                }
            }
            HPP[row] = sum;
            sum = 0;
        }
    }

    void computeVPP(){
        int sum = 0;
        for(int col = 0; col < numCols+2; col++){
            for(int row = 0; row < numRows+2; row++){
                if(imgAry[row][col] > 0){
                    sum++;
                }
            }
            VPP[col] = sum;
            sum = 0;
        }
    }

    void threshold(){
        // HPPbin
        for(int i = 0; i < numRows+2; i++){
            if(HPP[i] < thresholdVal){
                HPPbin[i] = 0;
            } else {
                HPPbin[i] = 1;
            }
        }


        // VPPbin
        for(int i = 0; i < numCols+2; i++){
            if(VPP[i] < thresholdVal){
                VPPbin[i] = 0;
            } else {
                VPPbin[i] = 1;
            }
        }
    }


    // Dilation then Erosion
    void closingHPPbin(){
        int* temp = new int[numRows+2];
        // Dilation
        for(int i = 1; i < numRows+1; i++){
            if(HPPbin[i] == 1){
                temp[i-1] = structElement[0];
                temp[i] = structElement[1];
                temp[i+1] = structElement[2];
            }
        }

        // Erosion
        for(int i = 1; i < numRows+1; i++){
            if(temp[i] == structElement[1]){
                if((temp[i-1] == structElement[0]) && (temp[i+1] == structElement[2])){
                    HPPMorph[i] = 1;
                }
            }
        }

        delete[] temp;
    }

    void closingVPPbin(){
        int* temp = new int[numCols+2];
        // Dilation
        for(int i = 1; i < numCols+1; i++){
            if(VPPbin[i] == 1){
                temp[i-1] = structElement[0];
                temp[i] = structElement[1];
                temp[i+1] = structElement[2];
            }
        }

        // Erosion
        for(int i = 1; i < numCols+1; i++){
            if(temp[i] == structElement[1]){
                if((temp[i-1] == structElement[0]) && (temp[i+1] == structElement[2])){
                    VPPMorph[i] = 1;
                }
            }
        }
        delete[] temp;
    }

    int computeRuns(int* Ary, int sizeOfAry){
        int count = 1;
        int currentVal = Ary[0];

        for(int i = 1; i < sizeOfAry; i++){
            if(Ary[i] != currentVal){
                count++;
                currentVal = Ary[i];
            }
        }
        return count;
    }

    string computeDirection(){
        if(HPPruns >= 3 * VPPruns){
            isHorizontal = true;
            return "horizontal";
        } else if(VPPruns >= 3 * HPPruns){
            isHorizontal = false;
            return "vertical";
        } else {
            cout << "Can not determine the reading direction" << endl;
            exit(1);
        }
    }

    void createPageBoundingBox(){
        boxNode* pageBox = new boxNode();
        pageBox->boxType = 1;

        int startHPP, startVPP;
        int endHPP, endVPP;
        
        // Finding StartHPP and StartVPP (first 1 in each array);
        for(int i = 0; i < numRows+2; i++){
            if(HPPMorph[i] ==1){
                startHPP = i;
                break;
            }
        }

        
        for(int i = 0; i < numCols+2; i++){
            if(VPPMorph[i] ==1){
                startVPP = i;
                break;
            }
        }

        // Finding endHPP, endVPP (last 1 in each array)
        for(int i = numRows+1; i >= 0; i--){
            if(HPPMorph[i] == 1){
                endHPP = i;
                break;
            }
        }

             
        for(int i = numCols+1; i >= 0; i--){
            if(VPPMorph[i] == 1){
                endVPP = i;
                break;
            }
        }

        

        if(isHorizontal){
            pageBox->BBOX.minR = startHPP;
            pageBox->BBOX.minC = startVPP;
            pageBox->BBOX.maxR = endHPP;
            pageBox->BBOX.maxC = endVPP;
        } else {
            pageBox->BBOX.minR = startVPP;
            pageBox->BBOX.minC = startHPP;
            pageBox->BBOX.maxR = endVPP;
            pageBox->BBOX.maxC = endHPP;
        }


        Queue.insert(pageBox);

    }

    void findLineBoxes(){
        int reader = 0;
        int startIndex = -1;
        int endIndex = -1;

        if(isHorizontal){
            int startVPP, endVPP;
            for(int i = 0; i < numCols+2; i++){
                if(VPPMorph[i] ==1){
                    startVPP = i;
                    break;
                }
            }
            for(int i = numCols+1; i >= 0; i--){
                if(VPPMorph[i] == 1){
                    endVPP = i;
                    break;
                }
            }   
            while (reader < numRows+2 ){
                startIndex = findStart(HPPMorph, reader, numRows+2);
                endIndex = findEnd(HPPMorph, startIndex, numRows+2);

                if(startIndex != endIndex){
                    boxNode* newNode = new boxNode();
                    newNode->boxType = 3;
                    newNode->BBOX.minR = startIndex;
                    newNode->BBOX.minC = startVPP;
                    newNode->BBOX.maxR = endIndex;
                    newNode->BBOX.maxC = endVPP;

                    Queue.insert(newNode);
                }
                

                reader = endIndex+1;
            }


            boxNode* newNode = new boxNode();
                    newNode->boxType = 3;
                    newNode->BBOX.minR = startIndex;
                    newNode->BBOX.minC = startVPP;
                    newNode->BBOX.maxR = endIndex;
                    newNode->BBOX.maxC = endVPP;

                    Queue.insert(newNode);


        } else {
            int startHPP, endHPP;
            for(int i = 0; i < numRows+2; i++){
                if(HPPMorph[i] == 1){
                    startHPP = i;
                    break;
                }
            }
            for(int i = numRows+1; i >= 0; i--){
                if(HPPMorph[i] == 1){
                    endHPP = i;
                    break;
                }
            }  


            while (reader < numCols+2 ){
                startIndex = findStart(VPPMorph, reader, numCols+2);
                endIndex = findEnd(VPPMorph, startIndex, numCols+2);

                if(startIndex != endIndex){
                    boxNode* newNode = new boxNode();
                    newNode->boxType = 3;
                    newNode->BBOX.minR = startHPP;
                    newNode->BBOX.minC = startIndex;;
                    newNode->BBOX.maxR = endHPP;
                    newNode->BBOX.maxC = endIndex;;

                    Queue.insert(newNode);
                }
                

                reader = endIndex+1;
            }

            boxNode* newNode = new boxNode();
                    newNode->boxType = 3;
                    newNode->BBOX.minR = startHPP;
                    newNode->BBOX.minC = startIndex;;
                    newNode->BBOX.maxR = endHPP;
                    newNode->BBOX.maxC = endIndex;;

                    Queue.insert(newNode);



        }
    }


    int findStart(int* Ary, int reader, int size){
        int index = reader;
        for(int i = reader; i < size; i++){
            if(Ary[i] == 1){
                return i;
            }
        }

        return size-1;
    }

    int findEnd(int* Ary, int startIndex, int size){
        int lastOne = startIndex;
        for(int i = startIndex+1; i < size; i++){
            if(Ary[i] == 0){
                return lastOne;
            } else {
                lastOne++;
            }
        }

        return size-1;
    }


    void drawBoxes(){
        boxNode* spot = Queue.Qfront->next;

        while(spot->next != NULL){
            // offset needed
            int startR = spot->BBOX.minR - 2;
            int startC = spot->BBOX.minC - 2;
            int endR = spot->BBOX.maxR - 2;
            int endC = spot->BBOX.maxC - 2;
            int boxType = spot->boxType;

            // Only write if its a ZERO
            // for(int i = startR; i < endR; i++){
            //     if(imgAry[i][startC] == 0)imgAry[i][startC] = boxType;
            //     if(imgAry[i][endC] == 0)  imgAry[i][endC] = boxType;
            // }

            // for(int i = startC; i < endC; i++){
            //     if(imgAry[startR][i] == 0) imgAry[startR][i] = boxType;
            //     if(imgAry[endR][i] == 0) imgAry[endR][i] = boxType;
            // }
            
            //Overwrites no matter what
            for(int i = startR; i < endR; i++){
                imgAry[i][startC] = boxType;
                imgAry[i][endC] = boxType;
            }

            for(int i = startC; i < endC; i++){
                imgAry[startR][i] = boxType;
                imgAry[endR][i] = boxType;
            }
                spot = spot->next;
        }

    }

    void prettyPrint(ofstream& outFile){
        for(int i = 0; i < numRows+2; i++){
            for( int j = 0; j < numCols+2; j++){
                if(imgAry[i][j] != 0){
                    outFile << imgAry[i][j] << " ";
                } else {
                    outFile << "  ";
                }
            }
            outFile << endl;
        }
    }

    void printQueue(ofstream& outFile){
        boxNode* spot = Queue.Qfront->next;
        while(spot->next != NULL){
            outFile << spot->boxType << endl;
            outFile << spot->BBOX.minR << " " << spot->BBOX.minC << " " << spot->BBOX.maxR << " " << spot->BBOX.maxC << endl;
            spot = spot->next;

        }
    }

};



int main(int argc, char* argv[]){
    ifstream inFile(argv[1]);
    int inputThreshold = stoi(argv[2]);
    ofstream outFile1(argv[3]);
    ofstream outFile2(argv[4]);


    imagePP image;
    inFile >> image.numRows;
    inFile >> image.numCols;
    inFile >> image.minVal;
    inFile >> image.maxVal;

    image.imgAry = new int*[image.numRows+2];
    for(int i = 0; i < image.numRows+2;i++){
        image.imgAry[i] = new int[image.numCols+2];
    }

    image.zeroFrame();

    image.thresholdVal = inputThreshold;

    // Size to numRows+2, numCols+2
    image.HPP = new int[image.numRows+2];
    image.VPP = new int[image.numCols+2];

    image.HPPbin = new int[image.numRows+2];
    image.VPPbin = new int[image.numCols+2];

    image.HPPMorph = new int[image.numRows+2];
    image.VPPMorph = new int[image.numCols+2];

    // Init to zero
    for(int i = 0; i < image.numRows+2; i++){
        image.HPP[i] = 0;
        image.HPPbin[i] = 0;
        image.HPPMorph[i] = 0;
    }
    for(int i = 0; i < image.numCols+2; i++){
        image.VPP[i] = 0;
        image.VPPbin[i] = 0;
        image.VPPMorph[i] = 0;
    }

    // Load image to imageAry
    image.loadImage(inFile);


    // Compute HPP
    image.computeHPP();

    // Compute VPP
    image.computeVPP();


    // Output HPP, VPP to outFile2 with captions
    outFile2 << "HPP:" << endl;
    for(int i = 0; i < image.numRows+2;i++){
        outFile2 << image.HPP[i] << " ";
    }
    outFile2 << endl;
    outFile2 << "VPP:" << endl;
    for(int i = 0; i < image.numCols+2;i++){
        outFile2 << image.VPP[i] << " ";
    }
    outFile2 << endl;

    // Thesholding to create HPPbin & VPPbin
    image.threshold();


    // Output HPPbin, VPPbin to outFile2 with captions
    outFile2 << "HPPbin:" << endl;
    for(int i = 0; i < image.numRows+2;i++){
        outFile2 << image.HPPbin[i] << " ";
    }
    outFile2 << endl;
    outFile2 << "VPPbin:" << endl;
    for(int i = 0; i < image.numCols+2;i++){
        outFile2 << image.VPPbin[i] << " ";
    }
    outFile2 << endl;


    // Creating HPPMorph
    image.closingHPPbin();

    // Creating VPPMorph
    image.closingVPPbin();

    // Compute HPPruns;
    image.HPPruns = image.computeRuns(image.HPPMorph, image.numRows+2);
    // Compute VPPruns;
    image.VPPruns = image.computeRuns(image.VPPMorph, image.numCols+2);

    // Output reading direction to outFile2
    outFile2 << "Reading Direction: " << endl;
    outFile2 << image.computeDirection() << endl;


    // BBox around whole page boxType = 1
    image.createPageBoundingBox();

    // Text - Line Bouding boxes boxType = 3
    image.findLineBoxes();

    // Draw boxes overwrite last thing
    image.drawBoxes();

    // prettyPrint to outFile1
    image.prettyPrint(outFile1);

    // prints to outFile2
    image.printQueue(outFile2);

    inFile.close();
    outFile1.close();
    outFile2.close();
    return 0;
}