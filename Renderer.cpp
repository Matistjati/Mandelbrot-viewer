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

inline double lerp(double min, double max, double amount)
{
	return (1 - amount) * min + amount * max;
}

inline double PercentBetween(double start, double end, double value)
{
	return (value - start) / (end - start);
}


double Renderer::RenderFrames(int framesBetweenImages, Mandelbrot mandel, std::string outputFolder)
{
	if (keyFrames.size() < 2)
	{
		return -1;
	}

	int totalFileCount = (this->FrameCount() - 1) * framesBetweenImages;
	int currentFile = 1;
	sf::Image outPutImage;
	outPutImage.create(mandel.width, mandel.height, sf::Color::Green);

	sf::Clock renderTime;
	double totalTime = 0;

	std::cout << "saving " << totalFileCount << " files to folder " << outputFolder;

	for (size_t i = 1; i < keyFrames.size(); i++)
	{
		//std::cout << "lerping between " << keyFrames[i - 1].zoom << " and " << keyFrames[i].zoom;

		std::cout << "At keyframe " << i << "/" << FrameCount() - 1 << "\n";

		double zoom = keyFrames[i - 1].zoom;
		double zoomChange = std::pow((keyFrames[i].zoom / keyFrames[i - 1].zoom), 1.f / (framesBetweenImages - 1));

		double offsetX = keyFrames[i - 1].offsetX;
		double offsetY = keyFrames[i - 1].offsetY;
		double amount = 0;

		for (int j = 0; j < framesBetweenImages; j++, zoom *= zoomChange, currentFile++)
		{
			// How many percent zoom is between start and end
			amount = PercentBetween(keyFrames[i].zoom, keyFrames[i - 1].zoom, zoom);

			double offsetX = lerp(keyFrames[i].offsetX, keyFrames[i - 1].offsetX, amount);
			double offsetY = lerp(keyFrames[i].offsetY, keyFrames[i - 1].offsetY, amount);

			mandel.UpdateImage(zoom, offsetX, offsetY, outPutImage);


			//std::cout << "% between start and finish: " << amount * 100 << "%\n";
			// I FINALLY GOT IT, turns out that my hunch about getting a constant relative difference was correct
			//std::cout << "zoom: " << zoom << ", relative difference: " << (lastZoom - zoom) / zoom << "\n";


			std::string file = (outputFolder + "\\mandel" + std::to_string(currentFile) + ".png");

			outPutImage.saveToFile(outputFolder + "\\mandel" + std::to_string(currentFile) + ".png");
			std::cout << "Saving file " << ("mandel" + std::to_string(currentFile) + ".png") <<", " <<
				currentFile << "/" << totalFileCount <<
				", took " << renderTime.restart().asSeconds() << "seconds\n";
		}
	}
	return totalTime / totalFileCount;
}

int Renderer::GetFrameCount(int minFrames, double start, double end, double referenceAmount)
{
	return (int)std::ceil(minFrames * ((start / end) / referenceAmount));
}

double Renderer::RenderFramesAdaptive(int minFramesBetweenImages, Mandelbrot mandel, std::string outputFolder)
{
	if (keyFrames.size() < 2)
	{
		return -1;
	}
	
	// The average
	double referenceAmount = 0;
	for (size_t i = 1; i < keyFrames.size(); i++)
	{
		referenceAmount += keyFrames[i - 1].zoom / keyFrames[i].zoom;
	}
	referenceAmount /= keyFrames.size();

	std::vector<int> frameAmounts(keyFrames.size());
	int totalFileCount = 0;

	for (size_t i = 1; i < keyFrames.size(); i++)
	{
		int frameAmount = GetFrameCount(minFramesBetweenImages, keyFrames[i - 1].zoom, keyFrames[i].zoom, referenceAmount);
		totalFileCount += frameAmount;
		frameAmounts[i - 1] = frameAmount;
	}


	int currentFile = 1;
	sf::Image outPutImage;
	outPutImage.create(mandel.width, mandel.height, sf::Color::Green);

	sf::Clock renderTime;
	double totalTime = 0;


	std::cout << "saving " << totalFileCount << " files to folder " << outputFolder << "\n";

	for (size_t i = 1; i < keyFrames.size(); i++)
	{
		int currentImageBetweenFrame = frameAmounts[i - 1];
		std::cout << "At keyframe " << i << "/" << FrameCount() - 1 << ", " << currentImageBetweenFrame << " images for this frame\n";
		
		double zoom = keyFrames[i - 1].zoom;
		double zoomChange = std::pow((keyFrames[i].zoom / keyFrames[i - 1].zoom), 1.f / (currentImageBetweenFrame - 1));

		double offsetX = keyFrames[i - 1].offsetX;
		double offsetY = keyFrames[i - 1].offsetY;
		double amount = 0;

		for (int _ = 0; _ < currentImageBetweenFrame; _++, zoom *= zoomChange, currentFile++)
		{
			// How many percent zoom is between start and end
			amount = PercentBetween(keyFrames[i].zoom, keyFrames[i - 1].zoom, zoom);

			double offsetX = lerp(keyFrames[i].offsetX, keyFrames[i - 1].offsetX, amount);
			double offsetY = lerp(keyFrames[i].offsetY, keyFrames[i - 1].offsetY, amount);

			mandel.UpdateImage(zoom, offsetX, offsetY, outPutImage);

			std::string file = (outputFolder + "\\mandel" + std::to_string(currentFile) + ".png");

			outPutImage.saveToFile(outputFolder + "\\mandel" + std::to_string(currentFile) + ".png");

			std::cout << "Saving file " << ("mandel" + std::to_string(currentFile) + ".png") << ", " <<
				currentFile << "/" << totalFileCount <<
				", took " << renderTime.restart().asSeconds() << "seconds\n";
		}
	}
	return totalTime / totalFileCount;
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
