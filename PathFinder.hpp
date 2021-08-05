#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include <stdio.h>
#include "SFML/Graphics.hpp"
#include "Grid.hpp"
#include "GridCellStates.hpp"
#include <vector>
#include <unordered_set>

using namespace std;
using namespace sf;

/// <summary>
/// A class for finding the shortest path between two cells in a grid
/// </summary>
class PathFinder
{
public:
	// node for each grid position
	struct GridNode
	{
		GridValue val; // value of this node
		Vector2i gridPos; // position of this node in the grid
		GridNode *parentNode; // node that came before this node in the path
		int gCost; // distance from starting node
		int hCost; // distance from end node

		int fCost()
		{
			return gCost + hCost;
		}
	};

private:
	// private instance variables
	Grid<GridNode> *grid;
	int outlineThickness;

	Vector2i* startPos;
	Vector2i* endPos;

	// custom hashing for GridNodes
	struct NodeHash
	{
		size_t operator()(const GridNode& node) const
		{
			return node.gCost + node.hCost + (int)node.val;
		}
	};

	int getDistance(GridNode* node1, GridNode* node2);

public:
	PathFinder(int width, int height, int cellSize);

	PathFinder(int width, int height, int cellSize, int outlineThickness);

	~PathFinder();

	Grid<GridNode> *getGrid();

	void drawGrid(RenderWindow *window);

	bool setValAt(int x, int y, GridValue val);

	bool setValAt(Vector2i pos, GridValue val);

	GridNode* getShortestPath();

private:
	void initializeNodes();

};

/// <summary>
/// Initialize the nodes in the grid
/// </summary>
void PathFinder::initializeNodes()
{
	for (int x = 0; x < grid->getGridWidth(); x++)
	{
		for (int y = 0; y < grid->getGridHeight(); y++)
		{
			GridNode* newNode = new GridNode();
			newNode->gridPos = Vector2i(x, y);
			grid->setValAt(x, y, newNode);
		}
	}
}

/// <summary>
/// Constructor for a new PathFinder object
/// </summary>
/// <param name="width">the width of the grid</param>
/// <param name="height">the length of the grid</param>
/// <param name="cellSize">the size of each grid cell</param>
PathFinder::PathFinder(int width, int height, int cellSize)
{
	grid = new Grid<GridNode>(width, height, cellSize);
	outlineThickness = 1;

	startPos = NULL;
	endPos = NULL;

	initializeNodes();
}

/// <summary>
/// Constructor for a new PathFinder object
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="cellSize"></param>
/// <param name="outlineThickness">the thickness of the outline of each grid square</param>
PathFinder::PathFinder(int width, int height, int cellSize, int outlineThickness)
{
	grid = new Grid<GridNode>(width, height, cellSize);
	this->outlineThickness = outlineThickness;

	startPos = NULL;
	endPos = NULL;

	initializeNodes();
}

/// <summary>
/// A destructor for a PathFinder object
/// </summary>
PathFinder::~PathFinder()
{
	cout << "startPos == null: " << (startPos == NULL) << endl;
	cout << "destPos == null: " << (endPos == NULL) << endl;

	for (int x = 0; x < grid->getGridWidth(); x++)
	{
		for (int y = 0; y < grid->getGridHeight(); y++)
		{
			GridNode* currNode = grid->getValueAt(x, y);
			delete(currNode);
		}
	}

	delete(grid);
}

/// <summary>
/// Get a reference to the internal grid object used for pathfinding
/// </summary>
/// <returns>a reference to the internal grid object used for pathfinding</returns>
Grid<PathFinder::GridNode>* PathFinder::getGrid()
{
	return grid;
}

/// <summary>
/// Draws this grid in the given window
/// </summary>
/// <param name="window">window to draw into</param>
void PathFinder::drawGrid(RenderWindow *window)
{
	int gridWidth = grid->getGridWidth();
	int gridHeight = grid->getGridHeight();
	int cellSize = grid->getCellSize();

	for (int x = 0; x < gridWidth; x++)
	{
		for (int y = 0; y < gridHeight; y++)
		{
			PathFinder::GridNode *node = grid->getValueAt(x, y);
			if (node != NULL)
			{
				RectangleShape cell = RectangleShape(Vector2f(cellSize, cellSize));
				Vector2f pos = grid->gridToScreen(x, y);
				cell.setPosition(pos);
				cell.setOutlineThickness(outlineThickness);

				GridStateColor color = valToColor(node->val);
				cell.setFillColor(Color((unsigned long)color));
				cell.setOutlineColor(Color::White);

				window->draw(cell);
			}				
		}
	}
}

/// <summary>
/// Set the value at the given grid coordinates
/// </summary>
/// <param name="x">the x coordinate of a cell in the grid</param>
/// <param name="y">the y coordinate of a cell in the grid</param>
/// <param name="val">the new value of the cell at (x, y) in the grid</param>
/// <returns>true if the coordinates are valid and false otherwise</returns>
bool PathFinder::setValAt(int x, int y, GridValue val)
{
	if (!grid->validCoords(x, y))
	{
		return false;
	}

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
			GridNode *oldStart = grid->getValueAt(startPos->x, startPos->y);
			oldStart->val = GridValue::UNOCCUPIED;

			// set new start pos
			startPos->x = x;
			startPos->y = y;
		}
	}
	else if (val == GridValue::DESTINATION)
	{
		// set old dest to unoccupied
		if (endPos == NULL)
		{
			endPos = new Vector2i(x, y);
		}
		else
		{
			// unoccupy old end pos
			GridNode* oldEnd = grid->getValueAt(endPos->x, endPos->y);
			oldEnd->val = GridValue::UNOCCUPIED;

			// set new dest pos
			endPos->x = x;
			endPos->y = y;
		}
	}
	else
	{
		// make sure that dest/start are removed if the cell is overwritten 
		GridNode *currNode = grid->getValueAt(x, y);
		if (startPos != NULL && currNode->val == GridValue::START)
		{
			// remove start pos
			delete(startPos);
			startPos = NULL;
		}
		else if (endPos != NULL && currNode->val == GridValue::DESTINATION)
		{
			// remove destination pos
			delete(endPos);
			endPos = NULL;
		}
	}

	// set the value at the cell
	grid->getValueAt(x, y)->val = val;

	return true;
}

/// <summary>
/// Set the value at the given screen pos
/// </summary>
/// <param name="pos">The screen position of the desired grid</param>
/// <param name="val">the new value of the cell at the given screen position</param>
/// <returns>true if the position is valid and false otherwise</returns>
bool PathFinder::setValAt(Vector2i pos, GridValue val)
{
	Vector2i gridPos = grid->screenToGrid(pos);
	return setValAt(gridPos.x, gridPos.y, val);
}

int PathFinder::getDistance(GridNode* node1, GridNode* node2)
{
	int xDist = abs(node1->gridPos.x - node2->gridPos.x);
	int yDist = abs(node1->gridPos.y - node2->gridPos.y);

	int numOfDiagonals = min(xDist, yDist);
	int numOfRegMoves = abs(xDist - yDist);
}

PathFinder::GridNode* PathFinder::getShortestPath()
{
	GridNode* startNode = grid->getValueAt(startPos->x, startPos->y);
	GridNode* endNode = grid->getValueAt(endPos->x, endPos->y);

	vector<GridNode*> openList;
	unordered_set<GridNode*, NodeHash> closedSet;

	openList.push_back(startNode);

	while (openList.size() > 0)
	{
		// find node with lowest fcost or lowest hcost if fcost are the same
		GridNode* lowestCostNode = openList[0];
		int pos = 0;
		for (int i = 1; i < openList.size(); i++)
		{
			GridNode* currNode = openList[i];
			if (currNode->fCost() < lowestCostNode->fCost()
					|| (currNode->fCost() == lowestCostNode->fCost()
					&& currNode->hCost <= lowestCostNode->hCost))
			{
				lowestCostNode = currNode;
				pos = i;
			}
		}

		// remove lowest cost node from open list
		vector<GridNode*>::iterator it = openList.begin() + pos;
		openList.erase(it);
		// add lowest cost node to closed set
		closedSet.insert(lowestCostNode);

		Vector2i lowestCostPos = lowestCostNode->gridPos;
		vector<GridNode*>* neighbours = grid->getNeighbours(lowestCostPos.x, lowestCostPos.y);
		for (int i = 0; i < neighbours->size(); i++)
		{
			GridNode* currNode = (*neighbours)[i];

			if (currNode->val == GridValue::OCCUPIED
				|| closedSet.find(currNode) != closedSet.end())
			{
				// go to next neighbour if this node is either occupied
				// or is in the closed set
				continue;
			}


		}

	}
}


#endif // !PATH_FINDER_H

