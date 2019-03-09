#include <iostream>
#include <SFML/Graphics.hpp>
#include "Mandelbrot.h"
#include "KeyFrame.h"
#include "Renderer.h"
#include <string>

constexpr int maxIterationsRender = 256;
//constexpr int widthRender = 4096;
//constexpr int heightRender = 2160;
constexpr int widthRender = 1920;
constexpr int heightRender = 1920;

constexpr int maxIterationsExploration = 256;
constexpr int widthExploration = 512;
constexpr int heightExploration = 512;

constexpr double movementSmoothing = 0.05;




int main()
{
	Renderer renderer;
	double lastRecordedZoom = -1;
	double lastRecordedoffsetX = - 1;
	double lastRecordedoffsetY = - 1;


	double speed = 0.05;
	double offsetX = -0.7;
	double offsetY = 0;
	double zoom = 0.4;

	sf::RenderWindow window(sf::VideoMode(widthExploration, heightExploration), "Mandelbrot set", sf::Style::Default);
	window.setFramerateLimit(60);


	sf::Texture texture;
	sf::Sprite sprite;

	bool stateChanged = false;

	sf::Clock renderTime; // For fractal rendering time
	sf::Clock clock; // For fps
	float lastTime = 0;

	Mandelbrot explorationBrot(maxIterationsExploration, widthExploration, heightExploration);
	sf::Image explorationImage;
	explorationImage.create(widthExploration, heightExploration, sf::Color::Green);


	Mandelbrot renderBrot(maxIterationsRender, widthRender, heightRender);
	sf::Image renderImage;
	renderImage.create(widthRender, heightRender);


	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::Resized:
				printf("New window width: %i, new window height %i\n", event.size.width, event.size.height);
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::S
					&& event.key.control)
				{
					renderTime.restart();

					renderBrot.UpdateImage(zoom, offsetX, offsetY, renderImage);

					renderImage.saveToFile("out.png");
					printf("Saved image of size %i * %i, %i iterations in %.4f seconds\n", widthRender, heightRender, maxIterationsRender, renderTime.getElapsedTime().asSeconds());
				}

				if (event.key.code == sf::Keyboard::R
					&& event.key.control && lastRecordedZoom != zoom && lastRecordedoffsetX != offsetX && lastRecordedoffsetY != offsetY)
				{
					renderer.RecordFrame(zoom, offsetX, offsetY);
					printf("recorded frame. zoom %.15f, x %.4f, y %.4f\n", zoom, offsetX, offsetY);
				}

				if (event.key.code == sf::Keyboard::E
					&& event.key.control)
				{
					std::string end = std::to_string((rand() % 10000));
					std::string fileName = "keyframe" + end;

					renderer.Serialize(fileName.c_str());
					std::cout << "saved current keyframes in file " << fileName << "\n";
				}

				if (event.key.code == sf::Keyboard::O
					&& event.key.control)
				{
					const std::string output = "C:\\Users\\Matis\\Desktop\\Mandelbrot\\Movie\\8";
					const int minImagesPerFrames = 120;

					renderTime.restart();

					renderer.RenderFramesAdaptive(minImagesPerFrames, renderBrot, output);
					std::cout << "Saved " << minImagesPerFrames * (renderer.FrameCount() - 1) << " images to folder " << output << " in " << renderTime.getElapsedTime().asSeconds() << " seconds\n";
				}

				if (event.key.code == sf::Keyboard::B
					&& event.key.control)
				{
					printf("");
				}

			case sf::Event::TextEntered:
				switch (event.text.unicode)
				{
				case 'w':
					stateChanged = true;
					offsetY -= speed * zoom;
					break;

				case 'a':
					stateChanged = true;
					offsetX -= speed * zoom;
					break;

				case 's':
					stateChanged = true;
					offsetY += speed * zoom;
					break;

				case 'd':
					stateChanged = true;
					offsetX += speed * zoom;
					break;

				case 'q':
					stateChanged = true;
					zoom *= 0.9;
					offsetX += zoom * movementSmoothing;
					offsetY += zoom * movementSmoothing;
					break;

				case 'e':
					stateChanged = true;
					zoom /= 0.9;
					offsetX -= zoom * movementSmoothing;
					offsetY -= zoom * movementSmoothing;
					break;

				case 'z':
					speed /= 0.9;
					break;

				case 'x':
					speed *= 0.9;
					break;


				default:
					break;
				}
				break;

			default:
				break;
			}

			if (event.type == event.Closed)
				window.close();
		}

		if (stateChanged)
		{
			renderTime.restart();
			stateChanged = false;

			explorationBrot.UpdateImage(zoom, offsetX, offsetY, explorationImage);


			texture.loadFromImage(explorationImage);
			sprite.setTexture(texture);

			printf("render took %.4f seconds\n", renderTime.getElapsedTime().asSeconds());

		}
		window.draw(sprite);
		window.display();




		float currentTime = clock.restart().asSeconds();
		int fps = (int)(1.f / (currentTime));

		window.setTitle("Mandelbrot set " + std::to_string(fps) + " fps, zoom: " + std::to_string(zoom));
	}

	return 0;
}
