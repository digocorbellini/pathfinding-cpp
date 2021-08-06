#include <iostream>
#include <SFML/Graphics.hpp>
#include "PathFinder.hpp"
#include "Button.hpp"

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

// buttons
const int NUM_OF_BUTTONS = 4;
Button* pathButton;
Button* diagonalToggleBtn;

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

void UILogic()
{
	// check to see if path buton is being pressed
	if (pathButton->isButtonPressed(window))
	{
		// draw shortest path if the button is being pressed
		pathFinder->drawShortestPath(window, includeDiagonals);
	}

	// check to see if the diagonal toggle button is being pressed
	if (diagonalToggleBtn->isButtonPressedDown(window))
	{
		// flip the bool for including diagonals and change the button
		// fill color accordingly
		includeDiagonals = !includeDiagonals;
		Color btnColor = (includeDiagonals) ? Color::Green : Color::Red;
		diagonalToggleBtn->setFillColor(btnColor);
	}
}

void drawUI()
{
	// draw all buttons
	pathButton->drawButton(window);
	diagonalToggleBtn->drawButton(window);
}

int main()
{
	// make the window
	window = new RenderWindow(
			VideoMode(windowWidth, windowHeight + extraUIHeight), "Pathfinding");
	// instantiate grid
	pathFinder = new PathFinder(windowWidth / gridSize, windowHeight / gridSize, gridSize);	
	grid = pathFinder->getGrid();

	// make buttons
	// size of all buttons
	Vector2f buttonSize(windowWidth / NUM_OF_BUTTONS, extraUIHeight);
	// the leftmost position for UI buttons
	Vector2f btnPosLeft = grid->gridToScreen(0, grid->getGridHeight());

	RectangleShape pathBtnShape(buttonSize);
	pathButton = new Button(&pathBtnShape, Color::Cyan);
	pathButton->setPosition(btnPosLeft);

	RectangleShape diagonalToggle(buttonSize);
	diagonalToggleBtn = new Button(&diagonalToggle);
	diagonalToggleBtn->setPosition(btnPosLeft + Vector2f(buttonSize.x, 0));
	diagonalToggleBtn->setFillColor(Color::Green);

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
		UILogic();

		// draw objects
		pathFinder->drawGrid(window);
		playerCursor();
		drawUI();

		// display window
		window->display();
	}

	delete(pathFinder);
}
