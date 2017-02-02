/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: dcoomb
 *
 * Created on January 29, 2017, 10:25 PM
 */
#include <iostream>
#include "functions.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

/*
 * 
 */
int main(int argc, char** argv) {
    
    
    if( argc != 3)
    {
     cout <<" Usage: windowDetector <image1> <image2>" << endl;
     return -1;
    }

    Mat image;
    Mat image2;
    Mat grey;
    Mat grey2;
    //Mat blurd;
    int thresh = 5;
    int squareSize = 20;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file
    image2 = imread(argv[2], CV_LOAD_IMAGE_COLOR); 
    if(! image.data || ! image2.data)                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    //GaussianBlur(image,blurd,Size(3,3),1);
    cv::cvtColor(image,grey,cv::COLOR_RGB2GRAY);
    cv::cvtColor(image2,grey2,cv::COLOR_RGB2GRAY);
    Rect menu = getChangedRegion(grey,grey2);
    int x = menu.x + menu.width/2-squareSize/2;
    int y = menu.y + menu.height/2-squareSize/2;
    Rect roi(x,y,squareSize,squareSize);
    Rect temp;
    int line = -1;
    int right=0,left=0,top=0,bottom=0;
    for(;roi.x<grey2.size[1];roi.x+=squareSize-3){
        line = scanForLine(grey2(roi),true);
        if(line>0){
            right = line+roi.x;
            break;
        }
    }
    
    temp = roi;
    for(;roi.y<grey2.size[0];roi.y+=squareSize-3){
        line = scanForLine(grey2(roi),true);
        if(line<0){
            bottom = roi.y+roi.height;
            break;
        }
    }
    roi = temp;
    for(;roi.y>0;roi.y-=squareSize-3){
        line = scanForLine(grey2(roi),true);
        if(line<0){
            top = roi.y;
            break;
        }
    }
    
    
    roi = Rect(x,y,squareSize,squareSize);
    for(;roi.x>0;roi.x-=squareSize-3){
        line = scanForLine(grey2(roi),false);
        if(line>0){
            left = line+roi.x;
            break;
        }
    }
    
    
    
    
    cv::rectangle(image2,Rect(left,top,right-left,bottom-top),Scalar(0,0,255));
    //cout<<" at "<<line<<endl;
    namedWindow("Line window", WINDOW_AUTOSIZE);
    imshow("Line window", image2);
    waitKey(0);
    //Mat cropped = grey(Rect(130,100,130,100));
    //int result = scanForLine(cropped,false);
    //cout<<"Hey, it's at "<<result<<endl;
    
    //GaussianBlur(bw,blurd,Size(3,3),2);
    return 0;
}

void codeDump(){
        Mat image;
    Mat grey;
    int thresh = 5;
    cv::cvtColor(image,grey,cv::COLOR_RGB2GRAY);
    
    int xsize = grey.size[0];
    int ysize = grey.size[1];
    Mat bw(xsize, ysize, CV_8UC1, Scalar(0));
    for(int x=0; x<xsize-1;x++){
        for(int y=0;y<ysize-1;y++){
            int diffx = (int)grey.at<uchar>(x+1,y)-(int)grey.at<uchar>(x,y);
            int diffy = (int)grey.at<uchar>(x,y+1)-(int)grey.at<uchar>(x,y);
            if(abs(diffx)>thresh){
                bw.at<uchar>(x+1,y)=255;
            }
            if(abs(diffy)>thresh){
                bw.at<uchar>(x,y+1)=255;
            }

        }
    }
    //GaussianBlur(bw,blurd,Size(3,3),2);
    namedWindow( "Blurred window", WINDOW_AUTOSIZE );// Create a window for display.
    namedWindow( "Line window", WINDOW_AUTOSIZE );
    imshow( "Blurred window", grey);                   // Show our image inside it.
    imshow( "Line window", bw);
}