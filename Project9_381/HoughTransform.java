import java.util.*;
import java.io.*;
import java.lang.Math;



public class HoughTransform {
    

    xyCoord point = new xyCoord();
    int angleInDegree;
    double angleInRadians;

    int HoughDist;
    int HoughAngle = 180;
    int HoughMinVal;
    int HoughMaxVal;
    int[][] HoughAry;

    // Functions
    
    void buildHoughSpace(ImageProcessing image){
        for(int r = 0; r < image.numRows; r++){
            for(int c = 0; c < image.numCols; c++){

                if(image.imgAry[r][c] > 0){
                    point.x = c;
                    point.y = r;

                    


                    angleInDegree = 0;

                    while(angleInDegree <= 179){
                        angleInRadians = angleInDegree/180.00 * Math.PI;
                        double dist = computeDistance(point, angleInRadians);
                        int distInt = (int) dist;
                        HoughAry[distInt][angleInDegree]++;
                        angleInDegree++;
                    }
                }
            }
        }
    }

    double computeDistance(xyCoord point, double angleInRadians){
        double a = angleInRadians;
        double x = point.x ;
        double y = point.y ;

        double c = Math.atan(y/x);
        double t = a - c - (Math.PI/2);

        double distance = (   Math.sqrt(Math.pow(x, 2)+Math.pow(y, 2))   )   *  Math.cos(t) + (HoughDist/2);

        // System.out.println(distance);

        return distance;
    }

    void determineMinMax() {
        int max = 0;

        for(int i = 0; i < HoughDist; i++){
            for(int j = 0; j < HoughAngle; j++){
                if(HoughAry[i][j] > max){ max = HoughAry[i][j];}
            }
        }

        HoughMaxVal = max;
        int min = max;

        for(int i = 0; i < HoughDist; i++){
            for(int j = 0; j < HoughAngle; j++){
                if(HoughAry[i][j] < min){ min = HoughAry[i][j];}
            }
        }
        HoughMinVal = min;
    }


    void prettyPrint(PrintWriter outFile){
        for(int i = 0; i < HoughDist; i++){
            for(int j = 0; j < HoughAngle; j++){
                if(HoughAry[i][j] > 0) {outFile.print(HoughAry[i][j] + " ");}
                else {outFile.print("  ");}
            }
            outFile.println();
        }
    }

    void ary2File(PrintWriter outFile){
        for(int i = 0; i < HoughDist; i++){
            for(int j = 0; j < HoughAngle; j++){
                if(HoughAry[i][j] > 0) {outFile.print(HoughAry[i][j] + " ");}
                else {outFile.print("0 ");}
            }
            outFile.println();
        }
    }

}