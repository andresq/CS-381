import java.util.*;

public class ImageProcessing {
    int numRows;
    int numCols;
    int minVal;
    int maxVal;

    int[][] imgAry;

    ImageProcessing(){
        numRows = 0;
        numCols = 0;
        minVal = 0;
        maxVal = 0;
    }

    void loadImage(Scanner inFile){
        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numCols; j++){
                imgAry[i][j] = inFile.nextInt();
            }
        }
    }




} // END OF MAIN



// DEBUGSTUFF

// for(int i = 0; i < image.numRows; i++){
//     for(int j = 0; j < image.numCols; j++){
//         System.out.print(image.imgAry[i][j]);
//     }
//     System.out.println();
// }