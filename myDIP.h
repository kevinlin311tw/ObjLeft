/************************************************************************/
/* This code was created by Kevin (Ke-Yun) Lin at October, 2013.  
   myDIP includes many basic image processing tools
   feel free to contact me should you have any question!    
   email: kevinlin311.tw[at]gmail.com                                   */
/************************************************************************/
  
#ifndef _MY_DIP_H 
#define _MY_DIP_H 
  
#include <stdio.h> 
#include <vector> 
#include <list> 
#include <algorithm> 
#include <math.h> 
#include <iostream> 
#include <time.h> 
#include <limits.h> 
#include <omp.h> 
#include <stdexcept> 
  
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

void *** Memory_Allocate(int width, int height, int depth); 
void Memory_Free(unsigned char ***pixel_index,int width, int height); 
void * Memory_Allocate1D(int width, int height, int depth); 
void Memory_Free1D(unsigned char *pixel_index,int width, int height); 
  
struct myImage 
{ 
    //unsigned char *** imageData; 
    unsigned char * pixelData; 
    int width; 
    int height; 
    int depth; 
  
    myImage(int set_width, int set_height, int set_depth){ 
        width = set_width; 
        height = set_height; 
        depth = set_depth; 
        pixelData = (unsigned char *)Memory_Allocate1D( width, height, depth); 
        //imageData = (unsigned char***)Memory_Allocate( width, height, depth); 
    } 
  
public: 
    void myReleaseImage(){ 
        Memory_Free1D(pixelData, width, height); 
    } 
}; 
  
struct myColor 
{ 
    unsigned char B; unsigned char G;  unsigned char R; 
}; 
  
struct myFloatColor
{
	double B; double G;  double R;
};
void myInverse(myImage * input, myImage * output); 
void myBinary(myImage * input, myImage * output, int threshold); 
void myImageAND(myImage * input1, myImage * input2, myImage * output); 
void myRGB2Gray(myImage  * input, myImage * output); 
void myImageCopy(myImage * input, myImage * output); 
void myImageZero(myImage * input); 
myColor myGet2D(myImage * input, int x, int y); 
void mySet2D(myImage * input, myColor colors, int x, int y); 
  
void opencv_2_myImage(IplImage * input, myImage * output);//只是用來影像資料的轉換 
void myImage_2_opencv(myImage * input, IplImage * output);//只是用來影像資料的轉換 
  
void myResize( myImage * input, myImage * output); 
void myErode( myImage * input, myImage * output);
void myDilade( myImage * input, myImage * output);
  
myImage * myCreateImage(int width, int height, int depth); 
int myCountNonZero(myImage * input);
void myAbsDiff(myImage * input1, myImage * input2, myImage * output);
  
void myMedianBlur( myImage * input, myImage * output); 
void mymedianFilter(unsigned char* corrupted, unsigned char* smooth, int width, int height); 

void myredmask( myImage * input, myImage * mask );
void mygreenmask( myImage * input, myImage * mask );
void mybluemask( myImage * input, myImage * mask );

void myErodeitself( myImage * input,int n);
void myDiladeitself( myImage * input, int n);
  
#endif
