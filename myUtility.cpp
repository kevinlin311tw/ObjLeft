#include "myUtility.h"
#include <iostream>
#include <time.h>
#include <math.h>
#include <limits.h>
using namespace std;

int LeaderFollower1D(std::vector<float> &cluster, float &samples, float &thresh)
{
	return 0;
}

pair<int, int> path_Tag(vector<CvPoint> path, float reserveRatio)
{
	pair<int, int> tag;
	float drop_length = path_Length(path) * (1-reserveRatio) / 2;
	float d = 0.0;

	for (unsigned int i=0; i<path.size(); i++)
	{
		d += point_dist(path.at(i).x, path.at(i).y, path.at(i+1).x, path.at(i+1).y);

		if (d >= drop_length)
		{
			tag.first = i+1;
			d = 0.0;
			break;
		}
	}

	for (int i=path.size()-1; i>0; i--)
	{
		d += point_dist(path.at(i).x, path.at(i).y, path.at(i-1).x, path.at(i-1).y);

		if (d >= drop_length)
		{
			tag.second = i-1;
			d = 0.0;
			break;
		}
	}
	//cout<<"(start, end) = "<<"("<<tag.first<<", "<<tag.second<<")"<<endl;
	return tag;
}
float path_Length(vector<CvPoint> path)
{
	float distance = 0.0;
	for (unsigned int i=1; i<path.size(); i++)
	{
		distance += point_dist(static_cast<float>(path.at(i).x), path.at(i).y, path.at(i-1).x, path.at(i-1).y);
	}
	return distance;
}
float point_dist(float x1, float y1, float x2, float y2)
{
	float tempX = (x1 - x2);
	float tempY = (y1 - y2);

	return sqrt(tempX*tempX + tempY*tempY);
}
int otsu(int *hist, int grid)
{
	int threshold = 0;
	float total_data =0;
	double variance = 0;
	for (int i=0; i<grid; i++)
	{
		total_data += hist[i];
	}

	for(int i=0; i<grid; i++)
	{
		double p[2] = {0};
		double u[2] = {0};
		int k;
		for(k=0; k<=i; k++)
		{
			p[0] += hist[k];
			u[0] += k*hist[k];
		}
		if(p[0] != 0)
			u[0] /= p[0];
		p[0] /= total_data;

		while(k<grid)
		{
			p[1] += hist[k];
			u[1] += k*hist[k];
			k++;
		}
		if(p[1] != 0)
			u[1] /= p[1];
		p[1] /= total_data;

		double temp = p[0]*p[1]*pow(u[0]-u[1], 2); //max inter-variance
		cout<<p[0]<<" "<<p[1]<<" "<<u[0]<<" "<<u[1]<<endl;
		cout<<temp<<endl;
		if(temp > variance)
		{
			variance = temp;
			threshold = i;
		}
	}
	return threshold;
}

CvScalar Get2D(IplImage *img, int i, int j)
{
	CvScalar pt;
	pt.val[0] = ((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0];
	pt.val[1] = ((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1];
	pt.val[2] = ((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2];

	return pt;
}

void Set2D(IplImage* img ,int i, int j, CvScalar pt)
{
	((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0]=pt.val[0]; // B
	((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1]=pt.val[1]; // G
	((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2]=pt.val[2]; // R
}

void Set2D_ADD(IplImage* img ,int i, int j, CvScalar pt)
{

	if (((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0] + pt.val[0] >255)
	{
		((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0] = 255;
	}
	else
	{
		((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0]+=pt.val[0]; // B
	}

	if (((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1] + pt.val[1] >255)
	{
		((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1] = 255;
	}
	else
	{
		((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1]+=pt.val[1]; // G
	}

	if (((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2] + pt.val[2] >255)
	{
		((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2] = 255;
	}
	else
	{
		((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2]+=pt.val[2]; // R
	}
}

vector<int> path_resample_id(std::vector<CvPoint> path, float reserveRatio, int reLength)
{
	float path_length = path_Length(path);
	float I = (path_length * reserveRatio) / (reLength);
	float D = 0.0;

	int startIdx = -1;
	int endIdx = -1;
	pair<int, int> tag = path_Tag(path, reserveRatio);

	vector<int> resampleID;
	vector<CvPoint2D32f> resample;
	CvPoint2D32f firstPoint;
	firstPoint.x = cvPointTo32f(path.at(tag.first)).x;
	firstPoint.y = cvPointTo32f(path.at(tag.first)).y;
	resampleID.push_back(tag.first);
	resample.push_back(firstPoint);

	CvPoint2D32f o1, o2;
	float d;
	//for(int i=0; i<obj->GetLength(); i++)
	for(int i = tag.first; i<tag.second; i++)
	{
		if(D == 0.0)
		{
			o1 = resample.back();
		}
		else 
		{
			o1.x = cvPointTo32f(path.at(i-1)).x;
			o1.y = cvPointTo32f(path.at(i-1)).y;
		}
		o2.x = cvPointTo32f(path.at(i)).x;
		o2.y = cvPointTo32f(path.at(i)).y;

		d = point_dist(o1.x, o1.y, o2.x, o2.y);

		if ((D + d) >= I) {
			CvPoint2D32f newP;
			newP.x =  (o1.x + ((I - D) / d)
				* (o2.x- o1.x));
			newP.y = (o1.y + ((I - D) / d)
				* (o2.y - o1.y));

			resample.push_back(newP); // append new point 'q'
			resampleID.push_back(i);

			// insert 'q' at position i in points s.t.
			// 'q' will be the next i
			
			i--;
			D = 0.0;
		} else {
			D += d;
		}
	}

	if (resample.size() == reLength) {
		CvPoint2D32f p;
		p.x = cvPointTo32f(path.at(tag.second)).x;
		p.y = cvPointTo32f(path.at(tag.second)).y;
		resample.push_back(p);
		resampleID.push_back(tag.second);
	}

	return resampleID;
}


kmeansData kmeans(double** data, int row, int col, int k)
{
	//***************************//
	//*** initialize center  ***//
	//***************************//
	time_t t;
	srand((unsigned)time(&t));

	double **tempMeans = new double*[k];
	double *errD = new double[k];
	double *tempD = new double[k];
	int *count = new int[k];

	kmeansData kd;
	kd.index = new int[row];
	kd.means = new double *[k];
	for(int i=0; i<k; i++)
	{
		tempMeans[i] = new double[col];
		errD[i] = 0.0;
		tempD[i] = 0.0;
		count[i] = 0;

		int randID = rand() % (row+1);
		cout<<"rand "<<randID<<"\t";
		kd.means[i] = new double [col];
		for (int j=0; j<col; j++)
		{
			kd.means[i][j] = data[randID][j];
			//cout<<data[randID][j]<<"   "<<kd.means[i][j]<<endl;
			tempMeans[i][j] = 0.0;
		}
	}
	
	double err = 0.0;
	do 
	{
		for (int i=0; i<k; i++)
		{
			errD[i] = 0.0;
			tempD[i] = 0.0;
			count[i] = 0;
			for (int j=0; j<col; j++)
			{
				tempMeans[i][j] = 0.0;
			}
		}
		err = 0.0;
		
		for (int i=0; i<row; i++)
		{
			for (int c=0; c<k; c++)
			{
				tempD[c] = 0.0;
			}

			for (int j=0; j<col; j++)
			{
				for (int c=0; c<k; c++)
				{
					//cout<<data[i][j]-kd.means[c][j]<<"\t";
					tempD[c] += pow(data[i][j]-kd.means[c][j],2);
					//cout<<tempD[c]<<endl;
				}
			}
			//cout<<tempD[0]<<"  "<<tempD[1]<<"  "<<tempD[2]<<"  "<<endl;
			kd.index[i] = update_min_index(tempD, k);
			//cout<<"in row "<<i<<" inde x = "<<kd.index[i]<<endl;

			for (int j=0; j<col; j++)
			{
				tempMeans[kd.index[i]][j] += data[i][j];
			}
			count[kd.index[i]]++;
		}
		
		cout<<"count : "<<count[0]<<" "<<count[1]<<" "<<count[2]<<endl;
		for (int c=0; c<k; c++)
		{
			if (count[c] != 0)
			{
				for (int j=0; j<col; j++)
				{
					tempMeans[c][j] /= count[c];
					errD[c] += pow(tempMeans[c][j] - kd.means[c][j],2);
					kd.means[c][j] = tempMeans[c][j];
				}
			}else
			{
				int randID = rand() % (row+1);
				for (int j=0; j<col; j++)
				{
					tempMeans[c][j] /= count[c];
					errD[c] += pow(tempMeans[c][j] - kd.means[c][j],2);
					kd.means[c][j] = data[randID][j];
				}
			}
					
			errD[c] = sqrt(errD[c]);
			cout<<"errD "<<c<<" "<<errD[c]<<"\n";
			err += errD[c];
		}
		cout<<"error:"<<err<<endl;
	} while (err > 0.0000001);

	for (int c=0; c<k; c++)
	{
		delete [] tempMeans[c];
	}
	delete [] tempMeans;
	delete [] errD;
	delete [] count;
	delete [] tempD;

	return kd;
}

int nearestNeighbor(double* sample, double **data, int row, int col)
{
	double *d = new double[row];
	double temp = 0.0;
	double avg = 0.0;
	for (int i=0; i<row; i++)
	{
		temp = 0.0;
		for (int j=0; j<col; j++)
		{
			//cout<<sample[j]<<"  "<<data[i][j]<<"  ";
			temp += pow(sample[j] - data[i][j], 2);
		}
		d[i] = sqrt(temp);
		avg += d[i];
		cout<<d[i]<<" ";
	}
	cout<<endl<<"avg = "<<avg/row<<endl<<endl;
	int index = update_min_index(d, row);
	delete [] d;

	return index;
}

int update_min_index(double* data, int length)
{
	double min = DBL_MAX;
	int index = -1;

	for (int i=0; i<length; i++)
	{
		if (min > data[i])
		{
			min = data[i];
			index = i;
		}
	}
	//cout<<"min dist = "<<min<<endl;
	return index;

}
int myRound(double value)
{
	//cout<<"value = "<<value<<endl;
	if(value >= 0.0f)
	{
		return (static_cast<int>(value + 0.5f));
	}
	return static_cast<int>(value - 0.5f);
}
CvPoint cv_center(CvRect rect)
{
	CvPoint pt;
	pt.x = static_cast<int>(rect.x + 0.5 * rect.width);
	pt.y = static_cast<int>(rect.y + 0.5 * rect.height);
	return pt;
}
float findTheta(CvPoint p1, CvPoint p2)
{
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	double theta = atan2(dy, dx) * 180 / PI;

	if(theta <0)
		theta += 360;

	return static_cast<float>(theta);
}

void new_findMaxIdx(double *data, int dim, int * output)
{
	double tmp = 0.0;
	int idx = -1, count = 0, test = 0;

	if((data[0]>data[1])&&(data[0]>data[2]))
	{
		output[0] = 1; output[1] = 0; output[2] = 0;
	}
	else if ((data[1]>data[0])&&(data[1]>data[2]))
	{
		output[0] = 0; output[1] = 1; output[2] = 0;
	}
	else if ((data[2]>data[0])&&(data[2]>data[1]))
	{
		output[0] = 0; output[1] = 0; output[2] = 1;
	}
	else if ((data[0]>data[1])&&(data[0]==data[2]))
	{
		output[0] = 0; output[1] = 0; output[2] = 0;//101
	}
	else if ((data[0]>data[2])&&(data[0]==data[1]))
	{
		output[0] = 0; output[1] = 0; output[2] = 0;//110
	}
	else if ((data[1]>data[0])&&(data[1]==data[2]))
	{
		output[0] = 0; output[1] = 0; output[2] = 0;//011
	}
	else if ((data[1]==data[0])&&(data[1]==data[2]))
	{
		output[0] = 0; output[1] = 0; output[2] = 0;
	}
}
/*
int findMaxIdx(double *data, int dim)
{
	double tmp = 0.0;
	int idx = -1, count = 0, test = 0;

	if((data[0]>data[1])&&(data[0]>data[2]))
	{
		return 0;
	}
	else if ((data[1]>data[0])&&(data[1]>data[2]))
	{
		return 1;
	}
	else if ((data[2]>data[0])&&(data[2]>data[1]))
	{
		return 2;
	}
	else
		return -1;
}*/

int findMaxIdx(double *data, int dim)
{
	double tmp = 0.0;
	int idx = -1;
	for (int i=0; i<dim; i++)
	{
		if (data[i] >= tmp)
		{
			tmp = data[i];
			idx = i;
		}
	}
	return idx;
}
void myMemset(double *arr, double value, int dim)
{
	for (int i=0; i<dim; i++)
	{
		arr[i] = value;
	}
}
