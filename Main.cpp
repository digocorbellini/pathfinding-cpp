#include <iostream>
#include <SFML/Graphics.hpp>
#include "Grid.hpp"

using namespace sf;

// kinda wack to hardcode these values but whatever
int windowWidth = 1000;
int windowHeight = 1000;
int gridSize = 50;

int main()
{
	// make the window
	RenderWindow window(
			VideoMode(windowWidth, windowHeight), "Pathfinding");

	Grid grid(windowWidth / gridSize, windowHeight / gridSize, gridSize);
	

	// insert logic in here
	while (window.isOpen()) 
	{
		Event event;
		while (window.pollEvent(event)) 
		{
			// check for the closing of the window
			if (event.type == Event::Closed) 
			{
				// close window
				window.close();
			}
		}
		
		// clear previous frame
		window.clear();


		grid.drawGrid(&window);


		// display window
		window.display();
	}

}
