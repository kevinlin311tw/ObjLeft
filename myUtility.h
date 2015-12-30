#ifndef _MY_UTILITY_H
#define _MY_UTILITY_H

#include <vector>
#include <utility>
#include <opencv2/opencv.hpp>

#define LF_ALPHA 0.3    //learning rate of leader follower
#define  PI 3.1415926

struct kmeansData
{
	double **means;
	int *index;
};
int LeaderFollower1D(std::vector<float> &cluster, float &samples, float &thresh);

int otsu(int *hist, int grid);

kmeansData kmeans(double** data, int row, int col, int k);

int nearestNeighbor(double* sample, double **data, int row, int col);

float point_dist(float x1, float y1, float x2, float y2);

float path_Length(std::vector<CvPoint> path);

std::pair<int, int> path_Tag(std::vector<CvPoint> path, float reserveRatio);

std::vector<int> path_resample_id(std::vector<CvPoint> path, float reserveRatio, int reLength);

CvScalar Get2D(IplImage *img, int i, int j);

void Set2D(IplImage* img ,int i, int j, CvScalar pt);

void Set2D_ADD(IplImage* img ,int i, int j, CvScalar pt);

int update_min_index(double*, int length);

int myRound(double value);

CvPoint cv_center(CvRect rect);

float findTheta(CvPoint, CvPoint);

int findMaxIdx(double *, int dim);

void new_findMaxIdx(double *data, int dim, int * output);

void myMemset(double *arr, double value, int dim);


#endif
