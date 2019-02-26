#include <vector>
#include <SFML/Graphics.hpp>

#pragma once
#ifndef MANDELBROT_H
#define MANDELBROT_H

class Mandelbrot
{
public:
	int width;
	int height;
	int maxIterations;
	std::vector<sf::Color> colors;
	Mandelbrot(int maxIterations, int width, int height);
	sf::Color GetColor(int iterations);

	void UpdateImage(double zoom, double offsetX, double offsetY, sf::Image& image);
	void UpdateImageSlice(double zoom, double offsetX, double offsetY, sf::Image& image, int minY, int maxY);
private:
	int MandelIterate(double px, double py, int maxIterations);
};


#endif
