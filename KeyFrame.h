#include <iostream>

#pragma once
#ifndef KEYFRAME_H
#define KEYFRAME_H

struct KeyFrame
{
	double zoom;
	double offsetX;
	double offsetY;

public:
	void Serialize(const char* fileName) 
	{
		FILE *file;

		fopen_s(&file, fileName, "wb");

		fwrite((char *)this, sizeof(*this), 1, file); //Treats the object as if it is a char array
		fclose(file);
	}

	void Deserialize(const char* fileName) 
	{
		FILE *file;

		fopen_s(&file, fileName, "rb");

		fread((char *)this, sizeof(*this), 1, file); //Treats the object as if it is a char array
		fclose(file);
	}

	void Print()
	{
		printf("%.4f %.4f %.4f\n", zoom, offsetX, offsetY);
	}
};

#endif
