#ifndef _OBJLEFTDETECT_H_
#define _OBJLEFTDETECT_H_
#include "myDIP.h"
#include "CBM_model.h"
#include "ObjectDetail.h"
#include "myUtility.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits.h>
#include <time.h>
//#include <windows.h>
//#include <process.h>
//#include <io.h>
#include <math.h>
#include <string>
#define PI  3.1415926535897932384626433832795

class ObjLeftDetect
{
public:
	ObjLeftDetect(myImage * input, int set_MOG_LearnFrame, int set_min_area, int set_buffer_len, myImage * mask);
	~ObjLeftDetect();
	bool process(myImage * input);
	CBM_model *_CBM_model;
	CvVideoWriter *_writer1, *_writer2;
	IplImage *A, *B;

	int ** image;
	myFloatColor * connect_colors;

private:
	//SYSTEMTIME st;
	VideoDetails *_video, *_video2;
	
	char _statement[200]; 	
	void initialize();
	void do_detection( int validation_method);
	bool soft_validation3( myImage * ImgSynopsis, vector<Obj_info*> obj_left);
	
	//show result //
	inline void WorkBegin();
	inline void WorkEnd();
	inline string WorkFps();

	int new_width, new_height; 
	vector<Obj_info *> alarmList;
	myImage * myimg1, * myimg2;
	IplImage * ImgSynopsis;
	myImage * _ImgSynopsis;

	int Spatial_Temporal_Search( int ** Image, int i, int j, myFloatColor * colors, int time_stamp,int my_label);
	int spatial_flood( bool ** foreground, int i, int j);

	myColor color_rainbow(int total_time, int current_time);
	bool object_detected;
	vector<Obj_info*> ObjLocation;
	vector<Obj_info*> LeftLocation;
	bool set_alarm;

};

#endif
