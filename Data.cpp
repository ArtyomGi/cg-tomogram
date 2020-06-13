#include "Data.h"

void Data::MinMax()
{
	min = max = arr[0];
	for (int i = 0; i < height * width * depth; i++)
	{
		if (min > arr[i])
			min = arr[i];
		if (max < arr[i])
			max = arr[i];
	}
}

int Data::readFile(QString fileName)
{
	std::ifstream file;
	file.open(fileName.toStdString(), std::ios::binary | std::ios::in);

	if (!file.is_open())
		qDebug() << "Your file isn't opened, please try another file" << endl;

	file.read((char*)&width, sizeof(int));
	file.read((char*)&height, sizeof(int));
	file.read((char*)&depth, sizeof(int));

	if (!width || !height || !depth)
	{
		qDebug() << "Data error: w, d, h are incorrect" << endl;
		file.close();
		return 2;
	}

	qDebug() << "width = " << width << ", height = " << height << ", depth = " << depth << endl;

	file.read((char*)&scaleX, sizeof(float));
	file.read((char*)&scaleY, sizeof(float));
	file.read((char*)&scaleZ, sizeof(float));

	qDebug() << "scaleX = " << scaleX << ", scaleY = " << scaleY << ", scaleZ = " << scaleZ << endl;

	long arrSize = width * height * depth;

	if (arr != nullptr) delete[] arr;

	arr = new short[arrSize];
	file.read((char*)arr, arrSize * sizeof(short));
	file.close();

	MinMax();

	return 0;
}

short& Data::operator[](const int idx)
{
	if (idx < 0 || idx >= width * height * depth) throw "Incorrect index";

	return arr[idx];
}