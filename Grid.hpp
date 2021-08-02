#ifndef GRID_H
#define GRID_H

class Grid
{
private:
	int **gridArr;
	int gridWidth;
	int gridHeight;
	int gridSize; // in pixels

public:
	Grid(int width, int height, int gridSize);

	int getValueAt(int x, int y);
	
	~Grid();
};


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

// get the value at the given grid coordinates
int Grid::getValueAt(int x, int y)
{
	if (x < gridWidth && y < gridHeight)
	{
		return gridArr[x][y];
	}

	return -1;
}

Grid::~Grid()
{
	for (int i = 0; i < gridWidth; i++)
	{
		delete gridArr[i];
	}

	delete gridArr;
}
#endif GRID_H

