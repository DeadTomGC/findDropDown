/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "functions.h"

Rect getChangedRegion(Mat image1, Mat image2) {
    Mat output;
    cv::subtract(image1, image2, output);
    cv::threshold(output, output, 10,255,cv::THRESH_BINARY);
    int xsize = output.size[1];
    int ysize = output.size[0];
    bool first = true;
    Rect lb(xsize-1,ysize-1,0,0);
    for (int y = 0; y < ysize; y++) {
        for (int x = 0; x < xsize; x++) {
            if (output.at<uchar>(y, x ) ==255) {
                if(lb.x>x){
                    lb.x=x;
                }
                if(lb.y>y){
                    lb.y=y;
                }
                if(lb.x+lb.width<x){
                    lb.width = x-lb.x;
                }
                if(lb.y+lb.height<y){
                    lb.height = y-lb.y;
                }
            }

        }
    }
    
    //namedWindow("Line window", WINDOW_AUTOSIZE);
    //imshow("Line window", output(lb));
    //waitKey(0);
    return lb;
}
//--------------------------
//image must be in greyscale
//--------------------------

int scanForLine(Mat image, bool scanRight, int threshold,float lineConfidence) {
    int xsize = image.size[1];
    int ysize = image.size[0];
    int dir = (scanRight) ? 1 : -1;
    int start = (scanRight) ? 1 : xsize - 2;
    int end = (scanRight) ? xsize - 1 : -2;

    Mat bw(ysize, xsize, CV_8UC1, Scalar(0));
    for (int x = 0; x < xsize - 1; x++) {
        for (int y = 0; y < ysize; y++) {
            int diffx = (int) image.at<uchar>(y, x + 1)-(int) image.at<uchar>(y, x);

            if (abs(diffx) > threshold) {
                bw.at<uchar>(y, x + 1) = 255;
            }

        }
    }
    //namedWindow("Line window", WINDOW_AUTOSIZE);



    for (int x = start; x != end; x = x + dir) {
        int sum = 0;
        float avg = 0;
        float pixel = 0;
        for (int y = 0; y < ysize; y++) {
            pixel = 0.5 * bw.at<uchar>(y, x - 1) + 0.5 * bw.at<uchar>(y, x + 1) + bw.at<uchar>(y, x);
            if (pixel > 255){ pixel = 255;}
            sum += pixel;
        }
        avg = sum / ((float) ysize);
        if (avg > 255 * lineConfidence) {
            for (int y = 0; y < ysize; y++) {
                bw.at<uchar>(y, x) = 125;
            }
            //imshow("Line window", bw);
            //waitKey(0);
            return x;
        }
    }
    //imshow("Line window", bw);
    //waitKey(0);
    return -1;
}

Rect quickFindDropDown(Mat image,int squareSize,int x,int y){
    Rect roi(x,y,squareSize,squareSize);
    Rect temp;
    int line = -1,tempLine = -1;
    int right=0,left=0,top=0,bottom=0;
    for(;roi.x+roi.width<image.size[1];roi.x+=squareSize-3){
        line = scanForLine(image(roi),true);
        if(line>0){
            right = line+roi.x;
            break;
        }
        
    }
    
    temp = roi;
    for(;roi.y+roi.height<image.size[0];roi.y+=squareSize-3){
        tempLine = scanForLine(image(roi),true);
        if(abs(tempLine-line)>3){
            bottom = roi.y+roi.height;
            break;
        }
        
    }
    roi = temp;
    for(;roi.y>0;roi.y-=squareSize-3){
        tempLine = scanForLine(image(roi),true);
        if(abs(tempLine-line)>3){
            top = roi.y;
            break;
        }
        
    }
    
    
    roi = Rect(x,y,squareSize,squareSize);
    for(;roi.x>0;roi.x-=squareSize-3){
        line = scanForLine(image(roi),false);
        if(line>0){
            left = line+roi.x;
            break;
        }
    }
    
    temp = roi;
    for(;roi.y+roi.height<image.size[0];roi.y+=squareSize-3){

        tempLine = scanForLine(image(roi),false);
        if(abs(tempLine-line)>3){
            if(bottom>roi.y+roi.height){
                bottom = roi.y+roi.height;
            }
            break;
        }
    }

    roi = temp;
    for(;roi.y>0;roi.y-=squareSize-3){
        
        tempLine = scanForLine(image(roi),false);
        if(abs(tempLine-line)>3){
            if(top<roi.y){
                top = roi.y;
            }
            break;
        }
    }
    
    return Rect(left,top,right-left,bottom-top);
}