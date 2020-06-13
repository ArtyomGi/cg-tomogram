#pragma once
#include <iostream>
#include <locale>
#include <fstream>
#include <stdio.h>
#include "qdebug.h"

class Data
{
	int width, height, depth;
	float scaleX, scaleY, scaleZ;

	short* arr;

	short min, max;
public:
	Data() :
		width(0), height(0), depth(0), scaleX(0), scaleY(0), scaleZ(0), min(0), max(0)
	{
		arr = nullptr;
	};

	~Data()
	{
		width = 0;
		height = 0;
		depth = 0;

		delete[] arr;
	}

	short getMin() { return min; }
	short getMax() { return max; }

	int getWidth() { return width; }
	int getHeight() { return height; }
	int getDepth() { return depth; }

	float getScaleX() { return scaleX; }
	float getScaleY() { return scaleY; }
	float getScaleZ() { return scaleZ; }

	void MinMax();
	int readFile(QString filename);

	short& operator[] (const int idx);
};