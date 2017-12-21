#include "Pathfinder.h"

Pathfinder::Pathfinder(vector<vector<Tile*>> *tiles)
	: m_TileMap(tiles)
{

}

Pathfinder::~Pathfinder()
{

}

void Pathfinder::SetSearchOrder(SearchOrder order)
{
	m_SearchOrder = order;
}

void Pathfinder::ClearData()
{
	for each (Node *n in m_NodeList)
	{
		n->tile->SetSearchState(CLEAR);
		delete n;
	}
	m_NodeList.clear();
}

Node* Pathfinder::DeepSearch(Tile *start, Tile *end)
{
	ClearData();

	stack<Node*> open, closed;

	Node *startNode = new Node;
	startNode->tile = start;
	startNode->parent = nullptr;

	open.push(startNode);
	startNode->tile->SetSearchState(OPEN);
	m_NodeList.push_back(startNode);

	bool loop = true;

	vector<int> order;
	if (m_SearchOrder == COUNTER)
	{
		for (int i = 0; i < 8; i++)
		{
			order.push_back(i);
		}
	}
	else if (m_SearchOrder == CLOCK)
	{
		for (int i = 0; i < 8; i++)
		{
			order.push_back(7 - i);
		}
	}

	Node *current = startNode;
	while (loop)
	{
		current = open.top();
		open.pop();
		closed.push(current);
		current->tile->SetSearchState(CLOSED);

		// Examination
		if (current->tile == end)
		{
			loop = false;
			break;
		}

		// Expansion
		if (m_SearchOrder == RANDOM) order = GetRandomOrder();
		for (size_t i = 0; i < 8; i++)
		{
			Node *newNode = new Node;

			newNode->tile = current->tile->GetNeighbor(order[i]);

			newNode->parent = current;

			bool add = true;

			if (newNode->tile == nullptr) add = false;
			else if (newNode->tile->GetID() == 1 || !newNode->tile->GetActive()) add = false;
			else
			{
				for each (Node *n in m_NodeList)
				{
					if (newNode->tile == n->tile)
					{
						add = false;
						break;
					}
				}
			}

			if (add)
			{
				open.push(newNode);
				newNode->tile->SetSearchState(OPEN);
				m_NodeList.push_back(newNode);
			}
			else delete newNode;
		}
	}

	return current;
}

Node* Pathfinder::BroadSearch(Tile *start, Tile *end)
{
	ClearData();

	queue<Node*> open, closed;

	Node *startNode = new Node;
	startNode->tile = start;
	startNode->parent = nullptr;

	open.push(startNode);
	startNode->tile->SetSearchState(OPEN);
	m_NodeList.push_back(startNode);

	bool loop = true;

	vector<int> order;
	if (m_SearchOrder == COUNTER)
	{
		for (int i = 0; i < 8; i++)
		{
			order.push_back(i);
		}
	}
	else if (m_SearchOrder == CLOCK)
	{
		for (int i = 0; i < 8; i++)
		{
			order.push_back(7 - i);
		}
	}

	Node *current = startNode;
	while (loop)
	{
		current = open.front();
		open.pop();
		closed.push(current);
		current->tile->SetSearchState(CLOSED);

		// Examination
		if (current->tile == end)
		{
			loop = false;
			break;
		}

		// Expansion
		if (m_SearchOrder == RANDOM) order = GetRandomOrder();
		for (size_t i = 0; i < 8; i++)
		{
			Node *newNode = new Node;

			newNode->tile = current->tile->GetNeighbor(order[i]);

			newNode->parent = current;

			bool add = true;

			if (newNode->tile == nullptr) add = false;
			else if (newNode->tile->GetID() == 1 || !newNode->tile->GetActive()) add = false;
			else
			{
				for each (Node *n in m_NodeList)
				{
					if (newNode->tile == n->tile)
					{
						add = false;
						break;
					}
				}
			}

			if (add)
			{
				open.push(newNode);
				newNode->tile->SetSearchState(OPEN);
				m_NodeList.push_back(newNode);
			}
			else delete newNode;
		}
	}

	return current;
}

Node* Pathfinder::CostSearch(Tile *start, Tile *end)
{
	ClearData();

	struct CostNode
	{
		Node* node;
		float cost;
	};

	vector<CostNode> open, closed;

	CostNode startNode;
	startNode.node = new Node;
	startNode.node->tile = start;
	startNode.node->parent = nullptr;
	startNode.cost = 0;

	open.push_back(startNode);
	startNode.node->tile->SetSearchState(OPEN);
	m_NodeList.push_back(startNode.node);

	bool loop = true;

	vector<int> order;
	if (m_SearchOrder == COUNTER)
	{
		for (int i = 0; i < 8; i++)
		{
			order.push_back(i);
		}
	}
	else if (m_SearchOrder == CLOCK)
	{
		for (int i = 0; i < 8; i++)
		{
			order.push_back(7 - i);
		}
	}

	CostNode current = startNode;
	while (loop)
	{
		current = open[0];
		closed.push_back(current);
		open.erase(open.begin());
		current.node->tile->SetSearchState(CLOSED);

		// Examination
		if (current.node->tile == end)
		{
			loop = false;
			break;
		}

		// Expansion
		if (m_SearchOrder == RANDOM) order = GetRandomOrder();
		for (size_t i = 0; i < 8; i++)
		{
			CostNode newNode;
			newNode.node = new Node;
			bool diagonal = false;
			if (order[i] % 2 == 1) diagonal = true;

			newNode.node->tile = current.node->tile->GetNeighbor(order[i]);

			newNode.node->parent = current.node;

			newNode.cost = current.cost;

			bool add = true;

			if (newNode.node->tile == nullptr) add = false;
			else if (newNode.node->tile->GetID() == 1 || !newNode.node->tile->GetActive()) add = false;
			else
			{
				for each (Node *n in m_NodeList)
				{
					if (newNode.node->tile == n->tile)
					{
						add = false;
						break;
					}
				}
			}

			if (add)
			{
				if (diagonal) newNode.cost += ((newNode.node->tile->GetMoveCost() + current.node->tile->GetMoveCost()) / 2) * 1.4;
				else newNode.cost += ((newNode.node->tile->GetMoveCost() + current.node->tile->GetMoveCost()) / 2);
				bool added = false;
				size_t j = 0;
				while (!added && j < open.size())
				{
					if (open[j].cost > newNode.cost)
					{
						open.insert(open.begin() + j, newNode);
						added = true;
					}
					j++;
				}
				if (!added) open.push_back(newNode);
				newNode.node->tile->SetSearchState(OPEN);
				m_NodeList.push_back(newNode.node);
			}
			else delete newNode.node;
		}
	}

	return current.node;
}

float GetEndCost(Tile *start, Tile *end)
{
	float xDifference, yDifference, distance;

	xDifference = abs(start->GetPosition().x - end->GetPosition().x);
	yDifference = abs(start->GetPosition().y - end->GetPosition().y);
	xDifference /= 40;
	yDifference /= 40;

	if (xDifference >= yDifference)
	{
		distance = yDifference * 1.4;
		xDifference -= yDifference;
		distance += xDifference;
	}
	else
	{
		distance = xDifference * 1.4;
		yDifference -= xDifference;
		distance += yDifference;
	}

	return distance;
}

Node* Pathfinder::StarSearch(Tile *start, Tile *end)
{
	ClearData();

	struct StarNode
	{
		Node* node;
		float startCost, endCost;
	};

	vector<StarNode> open, closed;

	StarNode startNode;
	startNode.node = new Node;
	startNode.node->tile = start;
	startNode.node->parent = nullptr;
	startNode.startCost = 0;
	startNode.endCost = GetEndCost(start, end);

	open.push_back(startNode);
	startNode.node->tile->SetSearchState(OPEN);
	m_NodeList.push_back(startNode.node);

	bool loop = true;

	vector<int> order;
	if (m_SearchOrder == COUNTER)
	{
		for (int i = 0; i < 8; i++)
		{
			order.push_back(i);
		}
	}
	else if (m_SearchOrder == CLOCK)
	{
		for (int i = 0; i < 8; i++)
		{
			order.push_back(7 - i);
		}
	}

	StarNode current = startNode;
	while (loop)
	{
		current = open[0];
		closed.push_back(current);
		open.erase(open.begin());
		current.node->tile->SetSearchState(CLOSED);

		// Examination
		if (current.node->tile == end)
		{
			loop = false;
			break;
		}

		// Expansion
		if (m_SearchOrder == RANDOM) order = GetRandomOrder();
		for (size_t i = 0; i < 8; i++)
		{
			StarNode newNode;
			newNode.node = new Node;
			bool diagonal = false;
			if (order[i] % 2 == 1) diagonal = true;

			newNode.node->tile = current.node->tile->GetNeighbor(order[i]);

			newNode.node->parent = current.node;

			newNode.startCost = current.startCost;

			bool add = true;

			if (newNode.node->tile == nullptr) add = false;
			else if (newNode.node->tile->GetID() == 1 || !newNode.node->tile->GetActive()) add = false;
			else
			{
				for each (Node *n in m_NodeList)
				{
					if (newNode.node->tile == n->tile)
					{
						add = false;
						break;
					}
				}
			}

			if (add)
			{
				newNode.endCost = GetEndCost(newNode.node->tile, end);

				if (diagonal) newNode.startCost += ((newNode.node->tile->GetMoveCost() + current.node->tile->GetMoveCost()) / 2) * 1.4;
				else newNode.startCost += ((newNode.node->tile->GetMoveCost() + current.node->tile->GetMoveCost()) / 2);
				bool added = false;
				size_t j = 0;
				while (!added && j < open.size())
				{
					if (open[j].startCost + open[j].endCost > newNode.startCost + newNode.endCost)
					{
						open.insert(open.begin() + j, newNode);
						added = true;
					}
					j++;
				}
				if (!added) open.push_back(newNode);
				newNode.node->tile->SetSearchState(OPEN);
				m_NodeList.push_back(newNode.node);
			}
			else delete newNode.node;
		}
	}

	return current.node;
}

vector<int> Pathfinder::GetRandomOrder()
{
	vector<int> order;

	for (int i = 0; i < 8; i++)
	{
		order.push_back(i);
	}

	random_shuffle(order.begin(), order.end());

	return order;
}
