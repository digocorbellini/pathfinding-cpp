#include <iostream>
#include <SFML/Graphics.hpp>
#include "Grid.hpp"

using namespace sf;

// kinda wack to hardcode these values but whatever
int windowWidth = 1000;
int windowHeight = 1000;
int gridSize = 50;

RenderWindow *window;
Grid *grid;

Vector2f selectedGridPos = Vector2f(0, 0);

void playerCursor()
{
	// get mouse position
	Vector2i mousePos = Mouse::getPosition(*window);

	// set selected cell
	selectedGridPos = grid->centerScreenCoord(mousePos);
	Vector2f cellSize(grid->getCellSize(), grid->getCellSize());
	RectangleShape selectedSquare(cellSize);
	selectedSquare.setFillColor(Color((unsigned long)valToColor(GridValue::SELECTED)));
	selectedSquare.setPosition(selectedGridPos);
	window->draw(selectedSquare);
}

void playerController()
{
	// get mouse position
	Vector2i mousePos = Mouse::getPosition(*window);

	// player inputs
	if (Mouse::isButtonPressed(Mouse::Button::Left))
	{
		// pressed left mouse button
		if (Keyboard::isKeyPressed(Keyboard::Key::LShift))
		{
			// unoccupy cell
			grid->setValAt(mousePos, GridValue::UNOCCUPIED);
		}
		else
		{
			// occupy cell
			grid->setValAt(mousePos, GridValue::OCCUPIED);
		}	
	}
	else if (Mouse::isButtonPressed(Mouse::Button::Right))
	{
		// pressed right mouse button
		// place destination cell
		grid->setValAt(mousePos, GridValue::DESTINATION);
	}
	else if (Mouse::isButtonPressed(Mouse::Button::Middle) 
			|| Keyboard::isKeyPressed(Keyboard::Key::Space))
	{
		// middle mouse button
		// place starting cell
		grid->setValAt(mousePos, GridValue::START);
	}
}

int main()
{
	// make the window
	window = new RenderWindow(
			VideoMode(windowWidth, windowHeight), "Pathfinding");
	// instantiate grid
	grid = new Grid(windowWidth / gridSize, windowHeight / gridSize, gridSize);	

	// insert logic in here
	while (window->isOpen()) 
	{
		Event event;
		while (window->pollEvent(event)) 
		{
			// check for the closing of the window
			if (event.type == Event::Closed) 
			{
				// close window
				window->close();
			}
		}
		
		// clear previous frame
		window->clear();

		// logic
		playerController();

		// draw objects
		grid->drawGrid(window);
		playerCursor();

		// display window
		window->display();
	}
}
