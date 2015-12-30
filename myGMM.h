/************************************************************************/
/* Acknowledgements: This GMM code was slightly modified by Kevin (Ke-Yun) Lin
   based on the source code from Gurpinder Singh Sandhu
   The original version can be found here 
   https://github.com/gpsinghsandhu/Background-Subtraction-using-GMM    */
/************************************************************************/

// /*IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
// 
// By downloading, copying, installing or using the software you agree to this license.
// If you do not agree to this license, do not download, install,
// copy or use the software.
// 
// 
// License Agreement
// 
// Copyright (C) 2013, Gurpinder Singh Sandhu, all rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// * Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// * The name of the copyright holders may not be used to endorse or promote products
// derived from this software without specific prior written permission.
// 
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall Gurpinder Singh Sandhu be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.*/



#define USE_OPENCV  //LOOK LOOK LOOK HERE: If you don't want to use opencv library, just comment this declaration! "USE_OPENCV" 

#ifndef _MYGMM_H_
#define _MYGMM_H_

#include "myDIP.h"
//#include <conio.h>//for MS

#ifdef USE_OPENCV
	#include <opencv2/core/core.hpp>
	#include <opencv2/imgproc/imgproc.hpp>
	#include <opencv2/highgui/highgui.hpp>
	#include <opencv2/features2d/features2d.hpp>
	using namespace cv;
#endif

//Structure used for saving various components for each pixel
struct gaussian
{								
	double mean[3], covariance;
	double weight;	// Represents the measure to which a particular component defines the pixel value
	gaussian* Next;
	gaussian* Previous;

	gaussian()
	{
		mean[0] = 0.0;mean[1] = 0.0;mean[2] = 0.0;covariance = 0.0;weight = 0.0;
	};
};

struct Node
{
	gaussian* pixel_s;
	gaussian* pixel_r;
	int no_of_components;
	Node* Next;
};

class myGMM 
{
public:
	gaussian  *ptr, *start, *rear, *g_temp, *save, *next, *previous, *nptr, *temp_ptr;
	Node  *N_ptr,*N_rear, *N_start;//, 

	//Some constants for the algorithm
	double pi;
	double cthr;
	double alpha;
	double cT;
	double covariance0;
	double cf;
	double cfbar;
	double temp_thr;
	double prune;
	double alpha_bar;
	//Temperory variable
	int overall;
	
	double del[3], mal_dist;
	double sum;
	double sum1;
	int count;
	bool close;
	int background;
	double mult;
	double duration,duration1,duration2,duration3;
	double temp_cov;
	double weight;
	double var;
	double muR,muG,muB,dR,dG,dB,rVal,gVal,bVal;

	unsigned char * r_ptr;
	unsigned char * b_ptr;
	
	//Some function associated with the structure management
	Node* Create_Node(double info1, double info2, double info3);
	void Insert_End_Node(Node* np);
	gaussian* Create_gaussian(double info1, double info2, double info3);
	void Insert_End_gaussian(gaussian* nptr);
	gaussian* Delete_gaussian(gaussian* nptr);

	//main founction of GMM
	myGMM(double LearningRate);
	~myGMM();
	void process(myImage * inputRGB, myImage * outputBIN);
	void initial(myImage * inputRGB);
	void ChangeLearningRate( float new_learn_rate);

#ifdef USE_OPENCV
	void process(Mat orig_img, Mat bin_img);//this function is for opencv user
	void initial(Mat orig_img);//this function is for opencv user
#endif

private:
};

#endif
