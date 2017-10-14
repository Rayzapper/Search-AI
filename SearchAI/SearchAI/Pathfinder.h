#ifndef	RAYZAPPER_PATHFINDER_H
#define RAYZAPPER_PATHFINDER_H

#include "Tile.h"
#include <stack>
#include <queue>

using namespace std;

enum SearchOrder
{
	CLOCK,
	COUNTER,
	RANDOM
};

struct Node
{
	Tile *tile;
	Node *parent;
};

class Pathfinder
{
public:
	Pathfinder(vector<vector<Tile*>> *tiles);
	~Pathfinder();
	void SetSearchOrder(SearchOrder order);
	void ClearData();
	Node* DeepSearch(Tile *start, Tile *end);
	Node* BroadSearch(Tile *start, Tile *end);
	Node* CostSearch(Tile *start, Tile *end);
	Node* StarSearch(Tile *start, Tile *end);
private:
	vector<int> GetRandomOrder();

	vector<vector<Tile*>> *m_TileMap;
	SearchOrder m_SearchOrder = CLOCK;

	vector<Node*> m_NodeList;
};

#endif