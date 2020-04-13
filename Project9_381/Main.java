import java.util.*;
import java.io.*;
import java.lang.Math;

public class Main{
    public static void main(String args[]){
        ImageProcessing image = new ImageProcessing();
        HoughTransform hough = new HoughTransform();

        Scanner inFile = null;
        PrintWriter outFile1 = null;
        PrintWriter outFile2 = null;

        // 0
        try {
            inFile = new Scanner(new File(args[0]));
        } catch (FileNotFoundException err) {
            System.out.println("Error in opening input file from CLI: " + err);
        }

        try {
            outFile1 = new PrintWriter(args[1]);
            outFile2 = new PrintWriter(args[2]);
        } catch (FileNotFoundException err) {
            System.out.println("Error in opening output files from CLI: " + err);
        }

        image.numRows = inFile.nextInt();
        image.numCols = inFile.nextInt();
        image.minVal = inFile.nextInt();
        image.maxVal = inFile.nextInt();

        // dynamically allocating imgAry
        image.imgAry = new int[image.numRows][image.numCols];


        // HOUGH STUFF
        // a^2 + b^2 = c^2
        int diagonal = (int) Math.sqrt( Math.pow(image.numRows, 2) + Math.pow(image.numCols, 2)     );

        hough.HoughAngle = 180;
        hough.HoughDist = 2 * diagonal;
        hough.HoughAry = new int[hough.HoughDist][hough.HoughAngle];

        // 1
        image.loadImage(inFile);

        // 2
        hough.buildHoughSpace(image);

        // 3
        hough.prettyPrint(outFile1);

        // 4
        hough.determineMinMax();

        // 5
        outFile2.println(hough.HoughDist + " " + hough.HoughAngle + " " + hough.HoughMinVal + " " + hough.HoughMaxVal);

        // 6
        hough.ary2File(outFile2);

        // 7
        // Closing files
        inFile.close();
        outFile1.close();
        outFile2.close();
    }
}