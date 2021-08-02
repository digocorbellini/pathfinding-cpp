#ifndef GRID_H
#define GRID_H

class Grid
{
private:
	int **gridArr; // the grid
	int gridWidth; // the width of the grid
	int gridHeight; // the height of the grid
	int gridSize; // in pixels

public:
	Grid(int width, int height, int gridSize);

	int getValueAt(int x, int y);
	
	~Grid();
};

/// <summary>
/// Create an instance of a grid
/// </summary>
/// <param name="width">the width of the grid</param>
/// <param name="height">the height of the grid</param>
/// <param name="gridSize">the size of each grid square in pixels</param>
Grid::Grid(int width, int height, int gridSize)
{
	this->gridSize = gridSize;
	// initialize grid
	gridArr = new int*[width];
	for (int i = 0; i < width; i++)
	{
		gridArr[i] = new int[height];
	}

	// initialize instance variables
	gridWidth = width;
	gridHeight = height;
	this->gridSize = gridSize;
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

