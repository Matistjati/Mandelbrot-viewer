#include <iostream>
#include <SFML/Graphics.hpp>
#include "Mandelbrot.h"
#include "KeyFrame.h"
#include "Renderer.h"

constexpr int maxIterationsRender = 8192;
//constexpr int widthRender = 4096;
//constexpr int heightRender = 2160;
constexpr int widthRender = 1920;
constexpr int heightRender = 1920;

constexpr int maxIterationsExploration = 256;
constexpr int widthExploration = 512;
constexpr int heightExploration = 512;





int main()
{
	Renderer renderer;

	KeyFrame frame;
	frame.zoom = 1;
	frame.offsetX = 2;
	frame.offsetY = 3;
	
	renderer.RecordFrame(frame);

	KeyFrame frame2;

	frame2.zoom = 4;
	frame2.offsetX = 5;
	frame2.offsetY = 6;

	renderer.RecordFrame(frame2);

	KeyFrame frame3;
	frame3.zoom = 7;
	frame3.offsetX = 8;
	frame3.offsetY = 9;
	renderer.RecordFrame(frame3);


	frame.zoom = 6;
	frame.offsetX = 125;
	frame.offsetY = 69;

	renderer.Serialize("bb");

	Renderer renderer2;

	renderer2.Deserialize("bb");


	//KeyFrame ver;
	//ver.Deserialize("bb");
	//ver.Print();
	return 0;


	double speed = 0.5;
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
					printf("Saved image of size %i * %i, %i iterations in %.4f seconds", widthRender, heightRender, maxIterationsRender, renderTime.getElapsedTime().asSeconds());
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
					break;

				case 'e':
					stateChanged = true;
					zoom /= 0.9;
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

		window.setTitle("Mandelbrot set " + std::to_string(fps) + " fps");
	}

	return 0;
}
