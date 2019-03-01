#include "Renderer.h"
#include "KeyFrame.h"
#include <iostream>
#include "Mandelbrot.h"
#include <cmath>

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

double lerp(double min, double max, double amount)
{
	return (1 - amount) * min + amount * max;
}

double PercentBetween(double start, double end, double value)
{
	return (value - start) / (end - start);
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
		std::cout << "lerping between " << keyFrames[i - 1].zoom << " and " << keyFrames[i].zoom;

		double zoom = keyFrames[i - 1].zoom;
		double zoomChange = std::pow((keyFrames[i].zoom / keyFrames[i - 1].zoom), 1.f / (framesBetweenImages - 1));

		double offsetX = keyFrames[i - 1].offsetX;
		//double offsetXChange = std::pow((keyFrames[i].offsetX / keyFrames[i - 1].offsetX), 1.f / (framesBetweenImages - 1));
		//double offsetXChange = ((keyFrames[i - 1].offsetX - keyFrames[i].offsetX) / (framesBetweenImages - 1));


		double offsetY = keyFrames[i - 1].offsetY;
		//double offsetYChange = std::pow((keyFrames[i].offsetY / keyFrames[i - 1].offsetY), 1.f / (framesBetweenImages - 1));
		//double offsetYChange = ((keyFrames[i - 1].offsetY - keyFrames[i].offsetY) / (framesBetweenImages - 1));








		double amount = 0;
		for (int j = 0; j < framesBetweenImages; j++, zoom *= zoomChange)
		{
			// How many percent zoom is between start and end
			double amount = PercentBetween(keyFrames[i].zoom, keyFrames[i - 1].zoom, zoom);

			double offsetX = lerp(keyFrames[i].offsetX, keyFrames[i - 1].offsetX, amount);
			double offsetY = lerp(keyFrames[i].offsetY, keyFrames[i - 1].offsetY, amount);




			mandel.UpdateImage(zoom, offsetX, offsetY, outPutImage);




			//std::cout << "% between start and finish: " << amount * 100 << "%\n";
			// I FINALLY GOT IT, turns out that my hunch about getting a constant relative difference was correct
			//std::cout << "zoom: " << zoom << ", relative difference: " << (lastZoom - zoom) / zoom << "\n";




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
