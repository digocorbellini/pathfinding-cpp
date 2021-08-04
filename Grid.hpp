#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>
#include "GridCellStates.hpp"
#include "math.h"
#include <stdio.h>

using namespace std;
using namespace sf;

class Grid
{
private:
	GridValue **gridArr; // the grid
	int gridWidth; // the width of the grid
	int gridHeight; // the height of the grid
	int cellSize; // in pixels
	int outlineThickness; // in pixels

	Vector2i *startPos;
	Vector2i *destPos;

public:
	Grid(int width, int height, int cellSize);

	Grid(int width, int height, int cellSize, int outlineThickness);

	GridValue getValueAt(int x, int y);

	Vector2f gridToScreen(int x, int y);

	Vector2i screenToGrid(Vector2i pos);

	Vector2f centerScreenCoord(Vector2i pos);

	void drawGrid(RenderWindow *window);

	bool setValAt(int x, int y, GridValue val);

	bool setValAt(Vector2i pos, GridValue val);

	int getCellSize();
	
	~Grid();

private:
	bool validCoords(int x, int y);
};

/// <summary>
/// Get the cell size
/// </summary>
/// <returns>the cell size in pixels</returns>
int Grid::getCellSize()
{
	return cellSize;
}

/// <summary>
/// Return the center screen position of the cell closest to the givest 
/// screen position
/// </summary>
/// <param name="pos">A position on the screen</param>
/// <returns>the center screen position of the cell closest to the givest 
/// screen position</returns>
Vector2f Grid::centerScreenCoord(Vector2i pos)
{
	int posX = (int)floor(pos.x / cellSize) * cellSize;
	int posY = (int)floor(pos.y / cellSize) * cellSize;
	return Vector2f(posX, posY);
}

/// <summary>
/// Create an instance of a grid
/// </summary>
/// <param name="width">the width of the grid</param>
/// <param name="height">the height of the grid</param>
/// <param name="cellSize">the size of each grid square in pixels</param>
Grid::Grid(int width, int height, int cellSize)
{
	this->cellSize = cellSize;
	// initialize grid
	gridArr = new GridValue *[width];
	for (int i = 0; i < width; i++)
	{
		gridArr[i] = new GridValue[height];
	}

	// initialize instance variables
	gridWidth = width;
	gridHeight = height;
	this->cellSize = cellSize;
	outlineThickness = 1;

	startPos = NULL;
	destPos = NULL;
}

/// <summary>
/// Create an instance of a grid
/// </summary>
/// <param name="width">the width of the grid</param>
/// <param name="height">the height of the grid</param>
/// <param name="cellSize">the size of each grid square in pixels</param>
Grid::Grid(int width, int height, int cellSize, int outlineThickness)
{
	this->cellSize = cellSize;
	// initialize grid
	gridArr = new GridValue* [width];
	for (int i = 0; i < width; i++)
	{
		gridArr[i] = new GridValue[height];
	}

	// initialize instance variables
	gridWidth = width;
	gridHeight = height;
	this->cellSize = cellSize;
	this->outlineThickness = outlineThickness;

	startPos = NULL;
	destPos = NULL;
}

/// <summary>
/// Get the value at the given grid coordinates
/// </summary>
/// <param name="x">the row cooridnate</param>
/// <param name="y">the column coordinate</param>
/// <returns>the value at the given grid coordinates</returns>
GridValue Grid::getValueAt(int x, int y)
{
	if (x < gridWidth && y < gridHeight)
	{
		return gridArr[x][y];
	}

	return GridValue::INVALID;
}

/// <summary>
/// Convert the grid position to a screen position
/// </summary>
/// <param name="x">the x coordinate of the desired grid space</param>
/// <param name="y">the y coordinate of the dedired grid space</param>
/// <returns>a screen position in pixels as a Vector2f of the given cell
/// in the grid if the given coordinates are valid, otherwise returns NULL</returns>
Vector2f Grid::gridToScreen(int x, int y)
{
	float screenX = x * cellSize;
	float screenY = y * cellSize;

	return Vector2f(screenX, screenY);
}

Vector2i Grid::screenToGrid(Vector2i pos)
{
	int xPos = (int)floor(pos.x / cellSize);
	int yPos = (int)floor(pos.y / cellSize);
	return Vector2i(xPos, yPos);
}

/// <summary>
/// Check to see if the given grid coordinates are valid
/// </summary>
/// <param name="x">the x coordinate of the desired cell</param>
/// <param name="y"the y coordinate of the desired cell></param>
/// <returns>true if the cell is valid and false otherwise</returns>
bool Grid::validCoords(int x, int y)
{
	return (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight);
}

/// <summary>
/// Draws this grid in the given window
/// </summary>
/// <param name="window">window to draw into</param>
void Grid::drawGrid(RenderWindow *window)
{
	for (int x = 0; x < gridWidth; x++)
	{
		for (int y = 0; y < gridHeight; y++)
		{
			RectangleShape cell = RectangleShape(Vector2f(cellSize, cellSize));
			Vector2f pos = gridToScreen(x, y);
			cell.setPosition(pos);
			cell.setOutlineThickness(outlineThickness);

			GridStateColor color = valToColor(gridArr[x][y]);
			cell.setFillColor(Color((unsigned long)color));
			cell.setOutlineColor(Color::White);

			window->draw(cell);
		}
	}
}

/// <summary>
/// Set the value of the cell at the given coordinates
/// </summary>
/// <param name="x">the x coordinate of the cell</param>
/// <param name="y">the y coordinate of the cell</param>
/// <param name="val">the new value of the given cell</param>
/// <returns>true if the cell is valid and false otherwise</returns>
bool Grid::setValAt(int x, int y, GridValue val)
{
	if (validCoords(x, y))
	{
		// only allow one start and dest cell
		if (val == GridValue::START)
		{
			// set old start to unoccupied
			if (startPos == NULL)
			{
				startPos = new Vector2i(x, y);
			}
			else
			{
				// unoccupy old start pos
				gridArr[startPos->x][startPos->y] = GridValue::UNOCCUPIED;

				// set new start pos
				startPos->x = x;
				startPos->y = y;
			}
		}
		else if (val == GridValue::DESTINATION)
		{
			// set old dest to unoccupied
			if (destPos == NULL)
			{
				destPos = new Vector2i(x, y);
			}
			else
			{
				// unoccupy old dest pos
				gridArr[destPos->x][destPos->y] = GridValue::UNOCCUPIED;

				// set new dest pos
				destPos->x = x;
				destPos->y = y;
			}
		}
		else
		{
			// make sure that dest/start are removed if the cell is overwritten 
			GridValue currVal = getValueAt(x, y);
			if (startPos != NULL && currVal == GridValue::START)
			{
				// remove start pos
				delete(startPos);
				startPos = NULL;
			}
			else if (destPos != NULL && currVal == GridValue::DESTINATION)
			{
				// remove destination pos
				delete(destPos);
				destPos = NULL;
			}
		}

		// set the value at the cell
		gridArr[x][y] = val;

		return true;
	}

	return false;

}

/// <summary>
/// Set the value of the cell at the given screen position
/// </summary>
/// <param name="pos">the screen position of the cell</param>
/// <param name="val">the new value of the cell</param>
/// <returns>true if the cell is valid and false otherwise</returns>
bool Grid::setValAt(Vector2i pos, GridValue val)
{
	Vector2i gridCoords(screenToGrid(pos));
	return setValAt(gridCoords.x, gridCoords.y, val);
}

/// <summary>
/// Destructor for a grid object
/// </summary>
Grid::~Grid()
{
	cout << "startPos == null: " << (startPos == NULL) << endl;
	cout << "destPos == null: " << (destPos == NULL) << endl;

	// free the grid array
	for (int i = 0; i < gridWidth; i++)
	{
		delete gridArr[i];
	}

	delete gridArr;

	// free the start and dest positions
	if (startPos != NULL)
	{
		delete(startPos);
	}
	if (destPos != NULL)
	{
		delete(destPos);
	}
}

#endif

