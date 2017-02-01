/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   functions.h
 * Author: dcoomb
 *
 * Created on January 31, 2017, 12:11 AM
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

Rect getChangedRegion(Mat image1,Mat image2);

int scanForLine(Mat image,bool scanRight = true,int threshold = 5);


#endif /* FUNCTIONS_H */

