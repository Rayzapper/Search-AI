#ifndef RAYZAPPER_LINESEGMENT_H
#define RAYZAPPER_LINESEGMENT_H

#include <SFML/Graphics.hpp>

class LineSegment
{
public:
	LineSegment(sf::Vector2f start, sf::Vector2f end, float thickness);
	~LineSegment();
	void Render(sf::RenderWindow *window);
private:
	sf::VertexArray m_Array;
};

#endif