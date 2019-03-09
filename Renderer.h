#include <vector>
#include "KeyFrame.h"
#include <vector>
#include "Mandelbrot.h"

#pragma once
#ifndef RENDERER_H
#define RENDERER_H

class Renderer
{
public:
	void RecordFrame(KeyFrame frame);
	void RecordFrame(KeyFrame* frame);
	void RecordFrame(double zoom, double offsetX, double offsetY);
	double RenderFrames(int framesBetweenImages, Mandelbrot mandel, std::string outputFolder);
	double RenderFramesAdaptive(int minFramesBetweenImages, Mandelbrot mandel, std::string outputFolder);
	
	void Serialize(const char* fileName);
	void Deserialize(std::string fileName);

	int FrameCount();
private:
	int GetFrameCount(int minFrames, double start, double end, double referenceAmount);

	std::vector<KeyFrame> keyFrames;
};

#endif