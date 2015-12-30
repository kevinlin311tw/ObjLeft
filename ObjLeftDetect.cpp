#include "ObjLeftDetect.h"
#include "parameter.h"
#include <omp.h>
//#include <Windows.h>
#include <time.h>

using namespace std;
using namespace cv;
#define WINDOW_NAME "Video"

ObjLeftDetect::ObjLeftDetect(myImage * input, int set_MOG_LearnFrame, int set_min_area, int set_buffer_len, myImage * mask)
{
	_CBM_model = new CBM_model(input, set_MOG_LearnFrame, set_min_area, set_buffer_len, INPUT_RESIZE, mask);	
	new_width = (int)(input->width*INPUT_RESIZE);
	new_height = (int)(input->height*INPUT_RESIZE);	
	initialize();
	alarmList.clear();
}
ObjLeftDetect::~ObjLeftDetect()
{
	delete _CBM_model;
	cvReleaseVideoWriter(&_writer1);
	cvReleaseVideoWriter(&_writer2);
}

void ObjLeftDetect::initialize()
{	
	object_detected = false;
	set_alarm = false;

	image = (int **)malloc(new_width*sizeof(int *));
	for (int k = 0; k < new_width; k++)
		image[k] = (int *)malloc(new_height*sizeof(int));	
	
	connect_colors = (myFloatColor *)malloc(10*sizeof(myFloatColor));	

	ObjLocation.clear();
	LeftLocation.clear();

 	myimg2 = myCreateImage(new_width,new_height,3);
	_ImgSynopsis = myCreateImage(new_width,new_height,3);

	A = cvCreateImage(cvSize(new_width,new_height),8,3);
	B = cvCreateImage(cvSize(new_width,new_height),8,3);
	cvZero(A);
	cvZero(B);
	
	_writer1 = cvCreateVideoWriter("static_to_track.avi",CV_FOURCC('X','V','I','D'),30,cvSize(new_width,new_height),1);
	_writer2 = cvCreateVideoWriter("summary.avi",CV_FOURCC('X','V','I','D'),30,cvSize(new_width,new_height),1);
}


bool ObjLeftDetect::process(myImage * input)
{
	IplImage * test; test = cvCreateImage(cvSize(new_width, new_height),8,3);
	set_alarm = false;
	myResize(input,myimg2);	
	object_detected = _CBM_model->Motion_Detection(myimg2);
	if (object_detected == true)
	{		
		ObjLocation = _CBM_model->GetDetectResult();	
		LeftLocation = _CBM_model->GetStaticForegourdResult();
			
		if (LeftLocation.size()>0)
		{
			myImageZero(_ImgSynopsis);

			for (int i = 0; i < _ImgSynopsis->width; i++)
			{
				for (int j = 0; j < _ImgSynopsis->height; j++)
				{
					myColor aaa = myGet2D(_CBM_model->my_imgStatic,i,j);
					myColor bbb; bbb.B = 200; bbb.G = 250; bbb.R = 10;
					if ((aaa.B==0)&&(aaa.G==200)&&(aaa.R==255))
					{
						mySet2D(_ImgSynopsis,bbb,i,j);
					}
				}
			}
			myImage_2_opencv(_ImgSynopsis,test);
			cvShowImage("summary",test);
			cvWaitKey(1);
			cvCopy(test,A);
			myImageZero(_ImgSynopsis);
			set_alarm = soft_validation3( _ImgSynopsis, LeftLocation);
			_CBM_model->System_Reset();
			LeftLocation.clear();			
		}
	}
	cvWriteFrame( _writer1, A);
	cvWriteFrame( _writer2, B);
	cvReleaseImage(&test);
	return set_alarm;
}


bool ObjLeftDetect::soft_validation3( myImage * ImgSynopsis, vector<Obj_info*> obj_left)
{
	bool _set_alarm = false;
	bool ** foreground;
	int temporal_rule = BUFFER_LENGTH;
	int retreval_time = temporal_rule/2 + temporal_rule/6;

	bool ** ForeSynopsis;
	ForeSynopsis = (bool **)malloc(new_width*sizeof(bool *));
	for (int i = 0; i < new_width; i++){
		ForeSynopsis[i] = (bool *)malloc(new_height*sizeof(bool));
	}
	for (int i = 0; i < new_width; i++){
		for (int j = 0; j < new_height; j++){
			ForeSynopsis[i][j] = false;
		}
	}

	int ** floodfillMap;
	floodfillMap = (int **)malloc(new_width*sizeof(int *));
	for (int i = 0; i < new_width; i++){
		floodfillMap[i] = (int *)malloc(new_height*sizeof(int));
	}
	for (int i = 0; i < new_width; i++){
		for (int j = 0; j < new_height; j++){
			floodfillMap[i][j] = 0;
		}
	}


	/************************************************************************/
	/*  capture the color information of the suspected owner               */
	/************************************************************************/
	foreground = _CBM_model->GetPrevious_nForeground(retreval_time-1);//the moment the bag dropped   (need to check!!)
	bool foreground_found = false;
	#pragma omp parallel for
	for (int j = 0; j < new_width; j++)
	{
		for (int k = 0; k < new_height; k++)
		{
			if (foreground[j][k] == true)
			{
				for (int n = 0; n < obj_left.size(); n++)
				{
					float owner_dist = point_dist( (float)j, (float)k, (float)obj_left.at(n)->x, (float)obj_left.at(n)->y);
					if (owner_dist<OWNER_SEARCH_ROI)//distance threshold
					{
						for (int w = -30; w < 30; w++)
						{
							//rgb histogram accumulated
							myColor color;
							color = myGet2D( _CBM_model->_GetPrevious_nFrame(retreval_time-1), j, k);
							//printf("get color: r = %lf, g = %lf, b = %lf\n",(float)color.R,(float)color.G, (float)color.B);
							obj_left.at(n)->Owner_B[(int)((float)color.B/255.0*10.0)] += 1.0;
							obj_left.at(n)->Owner_G[(int)((float)color.G/255.0*10.0)] += 1.0;
							obj_left.at(n)->Owner_R[(int)((float)color.R/255.0*10.0)] += 1.0;
						}
						foreground_found = true;
					}
				}
			}
		}
	}

	//if there is no foreground found near to the suspected left object, that is, we have a false alarm of object left detection
	//than return nothing!
	if (foreground_found==false)
		return false;

	//else if we found foregournd object, it must be the owner, than we extract the color information of further processing 

	myFloatColor ** obj_colors;
	obj_colors = (myFloatColor **)malloc(obj_left.size()*sizeof(myFloatColor *));
	for (int i = 0; i < obj_left.size(); i++)
		obj_colors[i] = (myFloatColor *)malloc(10*sizeof(myFloatColor));

	for (int i = 0; i < obj_left.size(); i++)
	{
		//normalizing the histogram accumulated information
		double total_r = 0.0, total_g = 0.0, total_b = 0.0;
		for (int j = 0; j < 10; j++)
		{
			total_r = total_r + obj_left.at(i)->Owner_R[j];
			total_g = total_g + obj_left.at(i)->Owner_G[j];
			total_b = total_b + obj_left.at(i)->Owner_B[j];
		}
		for (int j = 0; j < 10; j++)
		{
			obj_left.at(i)->Owner_R[j] = (obj_left.at(i)->Owner_R[j]/(total_r+0.001));
			obj_left.at(i)->Owner_G[j] = (obj_left.at(i)->Owner_G[j]/(total_g+0.001));
			obj_left.at(i)->Owner_B[j] = (obj_left.at(i)->Owner_B[j]/(total_b+0.001));

			obj_colors[i][j].R = obj_left.at(i)->Owner_R[j];
			obj_colors[i][j].G = obj_left.at(i)->Owner_G[j];
			obj_colors[i][j].B = obj_left.at(i)->Owner_B[j];
		}
	}

	/************************************************************************/
	/* generate the synopsis image                                          */
	/************************************************************************/
	//SYSTEMTIME s;
	//GetSystemTime(&s);
	for (int i = 1; i < retreval_time; i=i+1)//之前都是i=i+3
	{
		IplImage * img = cvCreateImage(cvSize(new_width,new_height),8,3);
		IplImage * fg = cvCreateImage(cvSize(new_width,new_height),8,1);
		foreground = _CBM_model->GetPrevious_nForeground(i);
		#pragma omp parallel for
		for (int j = 0; j < new_width; j++)
		{
			for (int k = 0; k < new_height; k++)
			{
				if (foreground[j][k] == true)
				{					
					myColor color;
					color = myGet2D( _CBM_model->_GetPrevious_nFrame(i), j, k);
					mySet2D(ImgSynopsis,color,j,k);
					ForeSynopsis[j][k] = true;

					CvScalar ppp;
					ppp.val[0] = color.B; ppp.val[1] = color.G; ppp.val[2] = color.R; 
					Set2D(img,k,j,ppp);
					CvScalar aaa; 
					aaa.val[0] = aaa.val[1] = aaa.val[2] = 255;
					Set2D(fg,k,j,aaa);
				}
				else
				{
					CvScalar ppp; 
					ppp.val[0] = ppp.val[1] = ppp.val[2] = 0;
					Set2D(img,k,j,ppp);
					CvScalar aaa; 
					aaa.val[0] = aaa.val[1] = aaa.val[2] = 0;
					Set2D(fg,k,j,aaa);
				}
			}
		}
		//char a[50], b[50];
		//sprintf(a,"temp\\mask%d%d%d%d%d%d_%d.bmp",s.wYear,s.wMonth,s.wDay,s.wHour,s.wMinute,s.wMilliseconds,i);
		//sprintf(b,"temp\\image%d%d%d%d%d%d_%d.bmp",s.wYear,s.wMonth,s.wDay,s.wHour,s.wMinute,s.wMilliseconds,i);
		//cvSaveImage(a,fg);
		//cvSaveImage(b,img);
		cvReleaseImage(&img);
		cvReleaseImage(&fg);
	}

	IplImage * temp = cvCreateImage(cvSize(ImgSynopsis->width,ImgSynopsis->height),8,3);
	myImage_2_opencv(ImgSynopsis,temp);
	cvShowImage("synopsis",temp);
	cvReleaseImage(&temp);

	cvZero(B);

	/************************************************************************/
	/* Pedestrian detector												   */
	/************************************************************************/
	for (int t = (retreval_time-1); t>=2; t--)
	{
		//_CBM_model->DetectPrevious_nForeground_HOG(t);
		//_CBM_model->DetectPrevious_nForeground_DPM(t);
	}


	/************************************************************************/
	/* use mounatin climbing algorithm to extract the candidate trajectory   */
	/************************************************************************/
	for (int n = 0; n < obj_left.size(); n++)
	{
		int _t_time = retreval_time-1;
		obj_left.at(n)->traj_label = n+1;
		
		for (int i = -OWNER_SEARCH_ROI/2; i < OWNER_SEARCH_ROI/2+1; i++)
		{
			for (int j = -OWNER_SEARCH_ROI/2; j < OWNER_SEARCH_ROI/2+1; j++)
			{
				Spatial_Temporal_Search(floodfillMap,obj_left.at(n)->x + i, obj_left.at(n)->y + j, obj_colors[n], _t_time, n+1);
			}
		}
	}

	/************************************************************************/
	/* Verifing the object left event by distance information              */
	/************************************************************************/
	IplImage * temp2 = cvCreateImage(cvSize(ImgSynopsis->width,ImgSynopsis->height),8,3);
	//myImage_2_opencv(ImgSynopsis,temp2);
	cvCopy(B,temp2);
	for (int n = 0; n < obj_left.size(); n++){
		bool inside = false, outside = false;
		int inside_count = 0, outside_count = 0;
		for (int i = 0; i < new_width; i++){
			for (int j = 0; j < new_height; j++){
				//if (floodfillMap[i][j] == obj_left.at(n)->traj_label){
				if (floodfillMap[i][j] == (n+1)){
					float dist = point_dist( (float)i, (float)j, (float)obj_left.at(n)->x, (float)obj_left.at(n)->y);
					
					if (dist>OWNER_SEARCH_ROI/2){
						inside_count++;
						//outside = true;
						//printf("outside dist = %lf,  i = %d, j = %d, objx = %d, objy = %d\n",dist,i,j,obj_left.at(n)->x,obj_left.at(n)->y);
					}
					else if (dist<OWNER_SEARCH_ROI/2){
						outside_count++;
						//inside = true;
						//printf("inside dist = %lf,  i = %d, j = %d, objx = %d, objy = %d\n",dist,i,j,obj_left.at(n)->x,obj_left.at(n)->y);
					}
				}
			}
		}
		if (inside_count>OWNER_SEARCH_ROI) inside = true;
		if (outside_count>OWNER_SEARCH_ROI) outside = true;

		if ((inside==true)&&(outside==true)){
 			printf("\a");
			printf("\a");
			_set_alarm = true;

			//cvCircle( temp2, cvPoint(obj_left.at(n)->x,obj_left.at(n)->y), OWNER_SEARCH_ROI/3, cvScalar(0,255,255),2,8,0);
		}
	}
	cvShowImage("synopsis",temp2);
	cvCopy(temp2,B);
	cvWaitKey(1);
	cvReleaseImage(&temp2);

	free(*ForeSynopsis);
	free(ForeSynopsis);
	free(*floodfillMap);
	free(floodfillMap);

	free(*obj_colors);
	free(obj_colors);

	return _set_alarm;
}

int ObjLeftDetect::Spatial_Temporal_Search( int ** Image, int i, int j, myFloatColor * colors, int time_stamp,int my_label)
{
    if ((j < 0) || (j >= new_height-1)) return 0; 
    if ((i < 0) || (i >= new_width-1)) return 0;	
	if(Image[i][j] == my_label) return 0;

	int object_x = i;
	int object_y = j;

	bool ** foreground;
	for (int t = time_stamp; t>=2; t--)
	{
		foreground = _CBM_model->GetPrevious_nForeground(t);
		//foreground = _CBM_model->GetPrevious_nForeground_DPM(t);
		//foreground = _CBM_model->GetPrevious_nForeground_HOG(t);

		if (foreground[object_x][object_y] == true)
		{
			for (int k = 0; k < new_width; k++)
				for (int l = 0; l < new_height; l++)
					image[k][l] = 0;

			spatial_flood( foreground, object_x, object_y);

			for (int k = 0; k < 10; k++)
			{
				connect_colors[k].B = 0.0;
				connect_colors[k].G = 0.0;
				connect_colors[k].R = 0.0;
			}
			int min_x = new_width, max_x = 0;
			int min_y = new_height, max_y = 0;
			for (int k = 0; k < new_width; k++)
			{
				for (int l = 0; l < new_height; l++)
				{
					if (image[k][l] == 1)
					{
						myColor color_temp;
						color_temp = myGet2D( _CBM_model->_GetPrevious_nFrame(t), k, l);
						connect_colors[(int)((double)color_temp.R/255.0*10.0)].R+=1.0;
						connect_colors[(int)((double)color_temp.G/255.0*10.0)].G+=1.0;
						connect_colors[(int)((double)color_temp.B/255.0*10.0)].B+=1.0;

						if (min_x > k) min_x = k;
						if (max_x < k) max_x = k;

						if (min_y > l) min_y = l;
						if (max_y < l) max_y = l;

					}
				}
			}

			object_x = min_x + (max_x-min_x)/2;
			object_y = min_y + (max_y-min_y)/2;
			
			//normalizing the histogram accumulated information
			double total_r = 0.0, total_g = 0.0, total_b = 0.0;
			for (int w = 0; w < 10; w++)
			{
				total_r = total_r + connect_colors[w].R;
				total_g = total_g + connect_colors[w].G;
				total_b = total_b + connect_colors[w].B;
			}
			for (int w = 0; w < 10; w++)
			{
				connect_colors[w].R = connect_colors[w].R/(total_r+0.001);
				connect_colors[w].G = connect_colors[w].G/(total_g+0.001);
				connect_colors[w].B = connect_colors[w].B/(total_b+0.001);
			}

			double color_prob_r = 0.0, color_prob_g = 0.0, color_prob_b = 0.0;
			for (int s = 0; s < 10; s++)
			{
				color_prob_r = color_prob_r + colors[s].R*connect_colors[s].R;
				color_prob_g = color_prob_g + colors[s].G*connect_colors[s].G;
				color_prob_b = color_prob_b + colors[s].B*connect_colors[s].B;
			}

			if ((color_prob_r>=0.0)&&(color_prob_g>=0.0)&&(color_prob_b>=0.0))
			{
				int foreground_num = 0, overlap_num = 0;
				for (int k = 0; k < new_width; k++)
				{
					for (int l = 0; l < new_height; l++)
					{
						if ((Image[k][l] == my_label)&&(image[k][l] == 1))
							overlap_num++;
					}
				}
				myColor color_temp, color_add;
				color_add = color_rainbow(time_stamp, t);
				for (int k = 0; k < new_width; k++)
				{
					for (int l = 0; l < new_height; l++)
					{
						if (image[k][l] == 1)
						{
							Image[k][l] = my_label;

							
							if (overlap_num < 1000)
							{			
								color_temp = myGet2D( _CBM_model->_GetPrevious_nFrame(t), k, l);
								CvScalar setcolor;
								setcolor.val[0] = color_temp.B + color_add.B;
								setcolor.val[1] = color_temp.G + color_add.G;
								setcolor.val[2] = color_temp.R + color_add.R;

								if (setcolor.val[0] > 255)	setcolor.val[0] = 255;
								if (setcolor.val[1] > 255)	setcolor.val[1] = 255;
								if (setcolor.val[2] > 255)	setcolor.val[2] = 255;

								Set2D(B,l,k,setcolor);
							}
						}
					}
				}							
			}
		}
	}

	return 0;
}
int ObjLeftDetect::spatial_flood(bool ** foreground, int i,int j)
{
    if ((j < 0) || (j >= new_height-1)) return 0; 
    if ((i < 0) || (i >= new_width-1)) return 0;
    if ((image[i][j] == 0)&&(foreground[i][j]==true)){
		image[i][j] = 1;
		spatial_flood(foreground,i+1,j);
		spatial_flood(foreground,i,j+1);
		spatial_flood(foreground,i-1,j);
		spatial_flood(foreground,i,j-1);
	}
	return 0;             
}

// bool ObjLeftDetect::strict_validation(myImage * img, vector<Obj_info*> obj_left)
// {
// 	bool _set_alarm = false;
// 	
// 	int static_foreground_num = obj_left.size();
// 
// 	IplImage * temp = cvCreateImage(cvSize(img->width,img->height),8,3);
// 	myImage_2_opencv(img,temp);
// 
// 	for(int k = 0; k < static_foreground_num; k++){
// 		int a, b;
// 		a = (obj_left.at(k)->x);
// 		b = (obj_left.at(k)->y);
// 		cvCircle( temp, cvPoint(a,b), 25, cvScalar(0,255,255),2,8,0); //show detection region
// 		printf("\a");printf("\a");
// 		_set_alarm = true;
// 	}
// 	cvShowImage("strict_validation",temp);
// 	cvWaitKey(0);
// 	cvReleaseImage(&temp);
// 	return _set_alarm;
// }

myColor ObjLeftDetect::color_rainbow(int total_time, int current_time)
{
	myColor color;
	double segment = (total_time+0.001)/6.0;
	int selection = (int)((double)current_time/segment);

	switch(selection){
		case 0:
			color.R = 100; color.G = 0; color.B = 200;
			break;
		case 1:
			color.R = 0; color.G = 0; color.B = 200;
			break;
		case 2:
			color.R = 0; color.G = 200; color.B = 0;
			break;
		case 3:
			color.R = 200; color.G = 200; color.B = 0;
			break;
		case 4:
			color.R = 200; color.G = 70; color.B = 0;
			break;
		case 5:
			color.R = 200; color.G = 0; color.B = 0;
			break;
		default:
			color.R = 0; color.G = 0; color.B = 0;
			break;
	}
	return color;
}
