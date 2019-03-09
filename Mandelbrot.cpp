#include "Mandelbrot.h"
#include <thread>

Mandelbrot::Mandelbrot(int maxIterations, int width, int height)
{
	this->width = width;
	this->height = height;
	this->maxIterations = maxIterations;

	colors = std::vector<sf::Color>(maxIterations + 1);

	for (size_t i = 0; i < maxIterations + 1; i++)
	{
		colors[i] = GetColor(i);
	}
}

int Mandelbrot::MandelIterate(double px, double py, int maxIterations)
{

	double x = 0;
	double y = 0;

	double y2;
	double x2;

	int i = maxIterations;
	for (; i > 0; i--)
	{
		y2 = y * y;
		x2 = x * x;

		if (x2 + y2 > 4)
		{
			break;
		}

		y = 2 * x * y + py;
		x = x2 - y2 + px; // TODO This is slow, maybe x2 and y2 get moved out of the registers?


	}

	/*int i = maxIterations;
	for (; i > 0; i--)
	{
		y2 = y * y;
		x2 = x * x;

		if (x2 + y2 > 4)
		{
			break;
		}

		y = 2 * x * y + py;
		x = x2 - y2 + px; // TODO This is slow, maybe x2 and y2 get moved out of the registers?


	}*/

	return maxIterations - i;
}


sf::Color Mandelbrot::GetColor(int iterations)
{
	const double iterationLog = log(this->maxIterations - 1.0);

	
	if (iterations + 1 > maxIterations)
	{
		return sf::Color::Black;
	}
	else
	{
		double c = 3 * log(iterations) / iterationLog;

		if (c < 1)
		{
			return sf::Color(static_cast<sf::Uint8>(255 * c), 0, 0);
		}
		else if (c < 2)
		{
			return sf::Color(255, static_cast<sf::Uint8>(255 * (c - 1)), 0);
		}
		else
		{
			return sf::Color(255, 255, static_cast<sf::Uint8>(255 * (c - 2)));
		}
	}

	/*if (iterations > maxIterations)
	{
		return sf::Color::Black;
	}
	else
	{
		double c = 3 * log(iterations) / iterationLog;

		if (c < 1)
		{
			return sf::Color(static_cast<sf::Uint8>(255 * c), 0, 0);
		}
		else if (c < 2)
		{
			return sf::Color(255, static_cast<sf::Uint8>(255 * (c - 1)), 0);
		}
		else
		{
			return sf::Color(255, 255, static_cast<sf::Uint8>(255 * (c - 2)));
		}
	}*/
	
	/*
	if (iterations + 2 > maxIterations)
	{
		return sf::Color::Black;
	}
	else
	{
		double c = 3 * log(iterations) / iterationLog;

		if (c < 1)
		{
			return sf::Color(0, static_cast<sf::Uint8>(255 * c), 0);
		}
		else if (c < 2)
		{
			return sf::Color(0, static_cast<sf::Uint8>(255 * (c - 0)), 0);
		}
		else
		{
			return sf::Color(0, static_cast<sf::Uint8>(255 * (c - 1)), 0);
		}
	}*/
}


void Mandelbrot::UpdateImageSlice(double zoom, double offsetX, double offsetY, sf::Image& image, int minY, int maxY)
{/*
	double xCallS = 0 * zoom - width / 2.0 * zoom + offsetX;

	double yCallS = minY * zoom - height / 2.0 * zoom + offsetY;

	for (int x = 0; x < width; x++, xCallS += zoom)
	{
		double yCall = yCallS;
		for (int y = minY; y < maxY; y++, yCall += zoom)
		{
			int iterations = MandelIterate(xCallS, yCallS, maxIterations);
			image.setPixel(x, y, colors[iterations]);
		}
	}*/


	double screenScaleX = (1 / static_cast<float>(width)  * zoom);
	double screenScaleY = (1 / static_cast<float>(height)  * zoom);

	for (int x = 0; x < width; x++, offsetX += screenScaleX)
	{
		for (int y = minY; y < maxY; y++)
		{
			int iterations = MandelIterate(offsetX, offsetY + y * screenScaleY, maxIterations);

			/*
			if (x % 50 == 0)
			{

				//printf("x: %f\n", (x * screenScaleX));
				//printf("x: %f\n", (offsetX + (x * screenScaleX - 0.5) * zoom));

				printf("Iterations %i\n", iterations);
			}*/


			image.setPixel(x, y, colors[iterations]);
		}
	}
}

void Mandelbrot::UpdateImage(double zoom, double offsetX, double offsetY, sf::Image& image)
{
	const int step = height / std::thread::hardware_concurrency();

	std::vector<std::thread> threads;

	for (int i = 0; i < height; i += step)
	{
		threads.push_back(std::thread(&Mandelbrot::UpdateImageSlice, *this, zoom, offsetX, offsetY, std::ref(image), i, std::min(i + step, height)));
	}

	for (std::thread &thread : threads)
	{
		thread.join();
	}
}