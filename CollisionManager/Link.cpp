#include "Link.h"
//class Verlet{}s;
Link::Link(Circle* c1, Circle* c2)
{
	m_c1 = c1;
	m_c2 = c2;
	m_targetDist = m_c1->m_radius + m_c2->m_radius;
}

void Link::Update()
{
	const sf::Vector2f dir = m_c1->m_currentPos - m_c2->m_currentPos;
	const float dist = Verlet::VectorDistance(m_c1->m_currentPos, m_c2->m_currentPos);
	const sf::Vector2f dirNormalized = dir / dist;
	const float overlap = m_targetDist - dist;
	if (m_c1-> m_isFixed)
		m_c2->m_currentPos -= dirNormalized * overlap;
	else if ((m_c2->m_isFixed))
	{
		m_c1->m_currentPos +=  dirNormalized * overlap;

	}
	else
	{
		m_c1->m_currentPos += 0.5f * dirNormalized * overlap;
		m_c2->m_currentPos -= 0.5f * dirNormalized * overlap;
	}
}