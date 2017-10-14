#include "LineSegment.h"

LineSegment::LineSegment(sf::Vector2f start, sf::Vector2f end, float thickness)
{
	m_Array.setPrimitiveType(sf::PrimitiveType::TrianglesFan);

	sf::Vertex point1, point2, point3, point4;
	point1.color = sf::Color::Black;
	point2.color = sf::Color::Black;
	point3.color = sf::Color::Black;
	point4.color = sf::Color::Black;

	sf::Vector2f normal;
	normal = end - start;
	float length = sqrtf((normal.x * normal.x) + (normal.y * normal.y));
	normal /= length;

	if (normal.x == 0 || normal.y == 0)
	{
		float temp = normal.x;
		normal.x = normal.y;
		normal.y = temp;
	}
	else normal.x *= -1;
	normal *= (thickness / 2);

	point1.position = start + normal;
	point2.position = end + normal;
	point3.position = end - normal;
	point4.position = start - normal;

	m_Array.append(point1);
	m_Array.append(point2);
	m_Array.append(point3);
	m_Array.append(point4);
}

LineSegment::~LineSegment()
{
	m_Array.clear();
}

void LineSegment::Render(sf::RenderWindow *window)
{
	window->draw(m_Array);
}