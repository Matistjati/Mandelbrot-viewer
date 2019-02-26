#include "Renderer.h"
#include "KeyFrame.h"
#include <iostream>
#include "Mandelbrot.h"

void Renderer::RecordFrame(KeyFrame frame)
{
	KeyFrame newFrame;
	newFrame.zoom = frame.zoom;
	newFrame.offsetX = frame.offsetX;
	newFrame.offsetY = frame.offsetY;
	keyFrames.push_back(newFrame);
}

void Renderer::RecordFrame(KeyFrame* frame)
{
	keyFrames.push_back(*frame);
}

void Renderer::RecordFrame(double zoom, double offsetX, double offsetY)
{
	KeyFrame frame;
	frame.zoom = zoom;
	frame.offsetX = offsetX;
	frame.offsetY = offsetY;
	keyFrames.push_back(frame);
}

double lerp(double value1, double value2, double amount)
{
	return (1 - amount) * value1 + amount * value2;
}

void Renderer::RenderFrames(int framesBetweenImages, Mandelbrot mandel, std::string outputFolder)
{
	if (keyFrames.size() < 2)
	{
		return;
	}

	int totalFileCount = (this->FrameCount() - 1) * framesBetweenImages;
	int currentFile = 1;
	sf::Image outPutImage;
	outPutImage.create(mandel.width, mandel.height, sf::Color::Green);

	for (int i = 1; i < keyFrames.size(); i++)
	{
		for (size_t j = 0; j < framesBetweenImages; j++)
		{
			double zoom = lerp(keyFrames[i - 1].zoom, keyFrames[i].zoom, j / (float)framesBetweenImages);
			double offsetX = lerp(keyFrames[i - 1].offsetX, keyFrames[i].offsetX, j / (float)framesBetweenImages);
			double offsetY = lerp(keyFrames[i - 1].offsetY, keyFrames[i].offsetY, j / (float)framesBetweenImages);

			mandel.UpdateImage(zoom, offsetX, offsetY, outPutImage);
			//std::cout << "zoom: " << zoom << ", x: " << offsetX << ", y: " << offsetY;


			std::string file = (outputFolder + "\\mandel" + std::to_string(currentFile) + ".png");

			std::cout << "saving file " << file << ", " << currentFile << "/" << totalFileCount << "\n";
			outPutImage.saveToFile(outputFolder + "\\mandel" + std::to_string(currentFile) + ".png");
			currentFile++;
		}
	}
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
	keyFrames[0] = *(new KeyFrame());

	FILE *file;

	fopen_s(&file, fileName.c_str(), "rb");

	KeyFrame* element = &this->keyFrames[0];
	fread((char *)element, sizeof(KeyFrame), fileSize / (sizeof(KeyFrame) / sizeof(double)) / sizeof(double), file); //Treats the object as if it is a char array

	fclose(file);
}

int Renderer::FrameCount()
{
	return keyFrames.size();
}
