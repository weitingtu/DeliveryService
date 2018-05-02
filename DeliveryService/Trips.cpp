#include "Trips.h"

Trip::Trip() :
	_x1(),
	_y1(),
	_v1(),
	_v2(),
	_v3(),
	_x2(),
	_y2(),
	_x3(),
	_y3(),
	_x4(),
	_y4()
{
}

Trip::~Trip()
{
}

Trips::Trips():
	_trips()
{
	//std::array<std::array<Trip, DAY>, POPULATION> _trips;
	_trips.resize(POPULATION);
	for (size_t p = 0; p < POPULATION; ++p)
	{
		_trips[p].resize(DAY);
	}
}

Trips::~Trips()
{
}
