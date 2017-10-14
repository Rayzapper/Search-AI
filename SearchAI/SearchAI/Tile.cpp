#include "Tile.h"

Tile::Tile(short ID)
	: m_ID(ID)
{
	m_Shape.setOutlineColor(sf::Color::Black);
	m_Shape.setOutlineThickness(-0.5);

	m_TargetShape.setRadius(12);
	m_TargetShape.setOutlineThickness(-5);
	m_TargetShape.setOutlineColor(sf::Color::Black);

	if (ID == 0)
		m_Shape.setFillColor(sf::Color(255, 255, 255, 255));
	else if (ID == 1)
		m_Shape.setFillColor(sf::Color(31, 31, 31, 255));
	else if (ID == 2)
		m_Shape.setFillColor(sf::Color(127, 127, 127, 255));
	else if (ID == 3)
		m_Shape.setFillColor(sf::Color(0, 0, 255, 255));

	m_Shape.setSize(sf::Vector2f(40, 40));
	m_Hitbox = sf::IntRect(0, 0, m_Shape.getSize().x, m_Shape.getSize().y);
}

Tile::~Tile()
{
	
}

void Tile::Update(float dt, sf::Vector2i mousePosition)
{
	m_MousePosition = sf::Vector2i(mousePosition);
	m_Mouseover = m_Hitbox.contains(m_MousePosition);
}

void Tile::Render(sf::RenderWindow *window)
{
	window->draw(m_Shape);
	if (m_SearchState != CLEAR)
	{
		if (m_SearchState == CLOSED) m_TargetShape.setFillColor(sf::Color::Black);
		else m_TargetShape.setFillColor(sf::Color::Transparent);

		window->draw(m_TargetShape);
	}
}

void Tile::SetID(short ID)
{
	m_ID = ID;

	if (ID == 0)
		m_Shape.setFillColor(sf::Color(255, 255, 255, 255));
	else if (ID == 1)
		m_Shape.setFillColor(sf::Color(31, 31, 31, 255));
	else if (ID == 2)
		m_Shape.setFillColor(sf::Color(127, 127, 127, 255));
	else if (ID == 3)
		m_Shape.setFillColor(sf::Color(0, 0, 255, 255));
}

void Tile::SetNeighbors(Tile *neighbors[8])
{
	for (size_t i = 0; i < 8; i++)
	{
		m_Neighbors[i] = neighbors[i];
	}
}

void Tile::SetPosition(sf::Vector2f position)
{
	m_Position = position;
	m_Shape.setPosition(position);
	m_TargetShape.setPosition(position + sf::Vector2f(8, 8));
	m_Hitbox.left = position.x;
	m_Hitbox.top = position.y;
}

void Tile::SetSearchState(SearchState state)
{
	m_SearchState = state;
}

void Tile::SetActive(bool active)
{
	m_Active = active;
}

short Tile::GetID() const
{
	return m_ID;
}

bool Tile::GetMouseover()
{
	return m_Mouseover;
}

Tile* Tile::GetNeighbor(int direction)
{
	return m_Neighbors[direction];
}

float Tile::GetMoveCost()
{
	if (m_ID == 0) return 1.f;
	else if (m_ID == 2) return 4.f;
	else if (m_ID == 3) return 8.f;
	else return 100.f;
}

bool Tile::GetActive() const
{
	return m_Active;
}