#include <iostream>
#include <SFML/Graphics.hpp>
#include "PathFinder.hpp"

using namespace sf;

// kinda wack to hardcode these values but whatever
int windowWidth = 1000;
int windowHeight = 1000;
int gridSize = 50;

RenderWindow *window;
PathFinder *pathFinder;
Grid<PathFinder::GridNode>* grid;

Vector2f selectedGridPos = Vector2f(0, 0);

/// <summary>
/// Handle the logic for the player cursor on the grid
/// </summary>
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

/// <summary>
/// Handle the player's ability to color the grid
/// </summary>
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
			pathFinder->setValAt(mousePos, GridValue::UNOCCUPIED);
		}
		else
		{
			// occupy cell
			pathFinder->setValAt(mousePos, GridValue::OCCUPIED);
		}	
	}
	else if (Mouse::isButtonPressed(Mouse::Button::Right))
	{
		// pressed right mouse button
		// place destination cell
		pathFinder->setValAt(mousePos, GridValue::DESTINATION);
	}
	else if (Mouse::isButtonPressed(Mouse::Button::Middle) || Keyboard::isKeyPressed(Keyboard::Key::Space))
	{
		// middle mouse button
		// place starting cell
		pathFinder->setValAt(mousePos, GridValue::START);
	}
}

int main()
{
	// make the window
	window = new RenderWindow(
			VideoMode(windowWidth, windowHeight), "Pathfinding");
	// instantiate grid
	pathFinder = new PathFinder(windowWidth / gridSize, windowHeight / gridSize, gridSize);	
	grid = pathFinder->getGrid();

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
		pathFinder->drawGrid(window);
		playerCursor();

		// display window
		window->display();
	}

	delete(pathFinder);
}
