#include <iostream>
#include <SFML/Graphics.hpp>
#include "PathFinder.hpp"

using namespace sf;

// kinda wack to hardcode these values but whatever
int windowWidth = 1000;
int windowHeight = 1000;
int extraUIHeight = 100;
int gridSize = 25;

RenderWindow *window;
PathFinder *pathFinder;
Grid<PathFinder::GridNode>* grid;
bool includeDiagonals = true;

Vector2f selectedGridPos = Vector2f(0, 0);

/// <summary>
/// Handle the logic for the player cursor on the grid
/// </summary>
void playerCursor()
{
	// get mouse position
	Vector2i mousePos = Mouse::getPosition(*window);

	// set selected cell
	Vector2i gridPos = grid->screenToGrid(mousePos);
	if (grid->validCoords(gridPos.x, gridPos.y))
	{
		selectedGridPos = grid->centerScreenCoord(mousePos);
		Vector2f cellSize(grid->getCellSize(), grid->getCellSize());
		RectangleShape selectedSquare(cellSize);
		selectedSquare.setFillColor(Color((unsigned long)valToColor(GridValue::SELECTED)));
		selectedSquare.setPosition(selectedGridPos);
		window->draw(selectedSquare);
	}	
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
	else if (Keyboard::isKeyPressed(Keyboard::Key::Tab))
	{
		if (!pathFinder->drawShortestPath(window, includeDiagonals))
		{
			cout << "missing start/end" << endl;
		}
	}
}

bool isShapePressed(RectangleShape shape)
{
	if(Mouse::isButtonPressed(Mouse::Button::Left))
	{
		Vector2i mousePos = Mouse::getPosition(*window);
		Vector2f mouseWorldPos(mousePos);
		if (shape.getGlobalBounds().contains(mouseWorldPos))
		{
			return true;
		}
	}
	return false;
}

void drawUI()
{
	// draw button to start shortest path
	// draw button to turn on and off diagonal search
	const int NUM_OF_BUTTONS = 4;

	// size of all buttons
	Vector2f buttonSize(windowWidth / NUM_OF_BUTTONS, extraUIHeight);
	// the leftmost position for UI buttons
	Vector2f btnPosLeft = grid->gridToScreen(0, grid->getGridHeight());

	RectangleShape pathFindingButton(buttonSize);
	pathFindingButton.setFillColor(Color::Cyan);
	pathFindingButton.setPosition(btnPosLeft);
	window->draw(pathFindingButton);

	if (isShapePressed(pathFindingButton))
	{
		pathFinder->drawShortestPath(window, includeDiagonals);
	}

	RectangleShape diagonalToggle(buttonSize);
	Color diagonalToggleColor = (includeDiagonals) ? Color::Green : Color::Red;
	diagonalToggle.setFillColor(diagonalToggleColor);
	diagonalToggle.setPosition(btnPosLeft + Vector2f(buttonSize.x, 0));
	window->draw(diagonalToggle);

	if (isShapePressed(diagonalToggle))
	{
		includeDiagonals = !includeDiagonals;
	}

}

int main()
{
	// make the window
	window = new RenderWindow(
			VideoMode(windowWidth, windowHeight + extraUIHeight), "Pathfinding");
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
		drawUI();
		pathFinder->drawGrid(window);
		playerCursor();

		// display window
		window->display();
	}

	delete(pathFinder);
}
