#ifndef _OBJECTDETAIL_H
#define _OBJECTDETAIL_H

class ObjectDetail
{
public:
	int GetCamID()			{return camID;};
	int GetObjectID()		{return objectID;};
	int GetTypeID()			{return typeID;};
	int GetOriTypeID()		{return oritypeID;};
	int GetArea()           {return area;};
	int GetX()				{return x;};
	int GetY()				{return y;};
	int GetWidth()			{return width;};
	int GetHeight()			{return height;};
	int GetLength()			{return length;};
	int GetSFrameID()		{return sFrameID;};
	int GetDirection()      {return direction;};
	long int GetFrameID()	{return frameID;};
	long int GetStartFrame(){return startFrame;};
	long int GetEndFrame()	{return endFrame;};

	void SetCamID(int value)			{camID = value;};
	void SetObjectID(int value)			{objectID = value;};
	void SetTypeID(int value)           {typeID = value;};
	void SetOriTypeID(int value)		{oritypeID = value;};
	void SetArea(int value)				{area = value;};
	void SetX(int value)				{x = value;};
	void SetY(int value)				{y = value;};
	void SetWidth(int value)			{width = value;};
	void SetHeight(int value)			{height = value;};
	void SetLength(int value)			{length = value;};
	void SetSFrameID(int value)			{sFrameID = value;};
	void SetDirection(int value)        {direction = value;};
	void SetFrameID(long int value)		{frameID = value;};
	void SetStartFrame(long int value)  {startFrame = value;};
	void SetEndFrame(long int value)	{endFrame = value;};

	int  offsetFrame;       
	long int playFrame;    

private:
	int camID;
	int objectID;
	int typeID;
	int oritypeID;
	int area;
	int x;
	int y;
	int width;
	int height;
	int length;
	int sFrameID;    
	int direction;
	long int frameID;
	long int startFrame;
	long int endFrame;
	
};


#endif
