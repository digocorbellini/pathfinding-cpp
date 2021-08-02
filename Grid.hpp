#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Grid
{
private:
	int **gridArr; // the grid
	int gridWidth; // the width of the grid
	int gridHeight; // the height of the grid
	int cellSize; // in pixels
	int outlineThickness; // in pixels

public:
	Grid(int width, int height, int cellSize);

	Grid(int width, int height, int cellSize, int outlineThickness);

	int getValueAt(int x, int y);

	Vector2f *gridToScreen(int x, int y);

	void drawGrid(RenderWindow *window);
	
	~Grid();

private:
	bool validCoords(int x, int y);
};

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
	gridArr = new int*[width];
	for (int i = 0; i < width; i++)
	{
		gridArr[i] = new int[height];
	}

	// initialize instance variables
	gridWidth = width;
	gridHeight = height;
	this->cellSize = cellSize;
	outlineThickness = 1;
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
	gridArr = new int* [width];
	for (int i = 0; i < width; i++)
	{
		gridArr[i] = new int[height];
	}

	// initialize instance variables
	gridWidth = width;
	gridHeight = height;
	this->cellSize = cellSize;
	this->outlineThickness = outlineThickness;
}

/// <summary>
/// Get the value at the given grid coordinates
/// </summary>
/// <param name="x">the row cooridnate</param>
/// <param name="y">the column coordinate</param>
/// <returns>the value at the given grid coordinates</returns>
int Grid::getValueAt(int x, int y)
{
	if (x < gridWidth && y < gridHeight)
	{
		return gridArr[x][y];
	}

	return -1;
}

/// <summary>
/// Convert the grid position to a screen position
/// </summary>
/// <param name="x">the x coordinate of the desired grid space</param>
/// <param name="y">the y coordinate of the dedired grid space</param>
/// <returns>a screen position in pixels as a Vector2f of the given cell
/// in the grid if the given coordinates are valid, otherwise returns NULL</returns>
Vector2f *Grid::gridToScreen(int x, int y)
{
	if (!validCoords(x, y))
	{
		return NULL;
	}

	float screenX = x * cellSize;
	float screenY = y * cellSize;

	return new Vector2f(screenX, screenY);
}

/// <summary>
/// Check to see if the given grid coordinates are valid
/// </summary>
/// <param name="x">the x coordinate of the desired cell</param>
/// <param name="y"the y coordinate of the desired cell></param>
/// <returns>true if the cell is valid and false otherwise</returns>
bool Grid::validCoords(int x, int y)
{
	return (x >= 0 && x <= gridWidth && y >= 0 && y <= gridHeight);
}

void Grid::drawGrid(RenderWindow *window)
{
	for (int x = 0; x < gridWidth; x++)
	{
		for (int y = 0; y < gridHeight; y++)
		{
			RectangleShape cell = RectangleShape(Vector2f(cellSize, cellSize));
			Vector2f* pos = gridToScreen(x, y);
			cell.setPosition(*pos);
			delete(pos);
			cell.setOutlineThickness(outlineThickness);
			cell.setFillColor(Color(0, 0, 0, 0));
			cell.setOutlineColor(Color(255, 255, 255, 255));

			window->draw(cell);
		}
	}
}

/// <summary>
/// Destructor for a grid object
/// </summary>
Grid::~Grid()
{
	for (int i = 0; i < gridWidth; i++)
	{
		delete gridArr[i];
	}

	delete gridArr;
}

#endif GRID_H

