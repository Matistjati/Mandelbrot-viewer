#include "Renderer.h"
#include "KeyFrame.h"
#include <iostream>

void Renderer::RecordFrame(KeyFrame frame)
{
	KeyFrame newFrame;
	newFrame.zoom = frame.zoom;
	newFrame.offsetX = frame.offsetX;
	newFrame.offsetY = frame.offsetY;
	keyFrames.push_back(newFrame);
}

void Renderer::RenderFrames(int framesBetweenImages, int width, int height, int maxIterations)
{

}

void Renderer::Serialize(const char * fileName)
{
	FILE *file;

	fopen_s(&file, fileName, "wb");
	KeyFrame* element = &this->keyFrames[0];
	fwrite((char *)element, sizeof(KeyFrame), this->keyFrames.size(), file); //Treats the object as if it is a char array

	fclose(file);
}

void Renderer::Deserialize(std::string fileName)
{
	struct stat stat_buf;
	int rc = stat(fileName.c_str(), &stat_buf);
	int fileSize = stat_buf.st_size;

	keyFrames.resize(fileSize / (sizeof(KeyFrame) / sizeof(double)) / sizeof(double));

	FILE *file;

	fopen_s(&file, fileName.c_str(), "rb");

	KeyFrame* element = &this->keyFrames.front();
	fread((char *)element, sizeof(KeyFrame), fileSize / (sizeof(KeyFrame) / sizeof(double)) / sizeof(double), file); //Treats the object as if it is a char array

	fclose(file);
}
