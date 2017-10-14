#ifndef	RAYZAPPER_TILE_H
#define RAYZAPPER_TILE_H

#include "Entity.h"

enum SearchState
{
	CLEAR,
	OPEN,
	CLOSED
};

class Tile : public Entity
{
public:
	Tile(short ID);
	~Tile();
	virtual void Update(float dt, sf::Vector2i mousePosition);
	virtual void Render(sf::RenderWindow *window);
	void SetNeighbors(Tile *neighbors[8]);
	virtual void SetPosition(sf::Vector2f position);
	void SetSearchState(SearchState state);
	short GetID() const;
	bool GetMouseover();
	Tile* GetNeighbor(int direction);
	float GetMoveCost();
private:
	sf::RectangleShape m_Shape;
	sf::CircleShape m_TargetShape;
	sf::IntRect m_Hitbox;
	sf::Vector2i m_MousePosition;
	short m_ID;
	bool m_Mouseover = false;
	Tile *m_Neighbors[8];

	SearchState m_SearchState = CLEAR;
};

#endif