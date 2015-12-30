#ifndef _VIDEO_DETAILS_H_
#define _VIDEO_DETAILS_H_

#include "opencvheader.h"

class VideoDetails
{
public:
	VideoDetails(char* filename);
	~VideoDetails();

	void VideoWriter_Initial(CvVideoWriter **writer ,char* filename, int fps);
	CvVideoWriter *writer_frame; 
	CvVideoWriter *writer_GMM; 
	CvVideoWriter *writer_Diff3; 
	CvVideoWriter *writer_Foreground; 

	bool writer_create;

	int _currentFrame;  //record current frame
	int _frameNum;      //record total frame number
	int _fps;
	CvCapture *_file;
	IplImage  *_frame; 
	char _videoName[50];
	double _width;
	double _height;

};
#endif
