#pragma once
#include "Circle.h"
#include "Verlet.h"


class Link
{
public:
	Circle* m_c1;
	Circle* m_c2;
	float m_targetDist;
	Link(Circle*, Circle*);
	void Update();

};