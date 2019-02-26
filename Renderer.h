#include <vector>
#include "KeyFrame.h"
#include <vector>

#pragma once
#ifndef RENDERER_H
#define RENDERER_H

class Renderer
{
public:
	void RecordFrame(KeyFrame frame);
	void RenderFrames(int framesBetweenImages, int width, int height, int maxIterations);
	
	void Serialize(const char* fileName);
	void Deserialize(std::string fileName);

private:
	std::vector<KeyFrame> keyFrames;
};

#endif