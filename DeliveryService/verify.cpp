#include "verify.h"



Verify::Verify(const Demands& d, const Trips& t) :
	_demands(d),
	_trips(t)
{
}


Verify::~Verify()
{
}

bool Verify::verify(size_t p) const
{
	return false;
}

// (7)
double Verify::_cost_1(size_t p, size_t t) const
{
	const Trip& trip = _trips.trips().at(p).at(t);
	//	const Demand& demand = _demands.demands().at(p).at(t);

	double cost = 0;

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				cost += _demands.c1().at(j).at(k) * (double)trip.x1().at(i).at(j).at(k);
			}
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.b1().at(j).at(k) * (double)trip.y1().at(j).at(k);
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.a1().at(j).at(k) * (double)trip.v1().at(j).at(k);
		}
	}

	for (size_t n = 0; n < CAR_TYPE; ++n)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			cost += _demands.a2().at(m) * (double)trip.v2().at(n).at(m);
		}
	}

	for (size_t k = 0; k < TASK; ++k)
	{
		cost += _demands.a3().at(k) * (double)trip.v3().at(k);
	}

	return cost;
}

// verify (8)
bool Verify::_verify_v1_v2(size_t p, size_t t, size_t k) const
{
	const Trip& trip = _trips.trips().at(p).at(t);

	int sum_v1 = 0;
	for (size_t j = 0; j < DISTRICT; ++j)
	{
		sum_v1 += trip.v1().at(j).at(k);
	}

	int sum_v2 = 0;
	for (size_t n = 0; n < CAR_TYPE; ++n)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			sum_v2 += trip.v1().at(n).at(m);
		}
	}

	return sum_v1 >= sum_v2;
}

// verify (9)
bool Verify::_verify_v3_v2(size_t p, size_t t, size_t k) const
{
	const Trip& trip = _trips.trips().at(p).at(t);

	int v3 = trip.v3().at(k);

	int sum_v2 = 0;
	for (size_t n = 0; n < CAR_TYPE; ++n)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			sum_v2 += trip.v1().at(n).at(m);
		}
	}

	return v3 >= sum_v2;
}

// verify (10), (11), (12), (13), (14)
bool Verify::_verify_x1_y1_v1_v2_v3(size_t p, size_t t) const
{
	const Trip& trip = _trips.trips().at(p).at(t);

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				if (trip.x1().at(i).at(j).at(k) < 0)
				{
					return false;
				}
			}
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			if (trip.y1().at(j).at(k) < 0)
			{
				return false;
			}
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			if (trip.v1().at(j).at(k) < 0)
			{
				return false;
			}
		}
	}

	for (size_t n = 0; n < CAR_TYPE; ++n)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			if (trip.v2().at(n).at(m) < 0)
			{
				return false;
			}
		}
	}

	for (size_t k = 0; k < TASK; ++k)
	{
		if (trip.v3().at(k) < 0)
		{
			return false;
		}
	}

	return true;
}

// (15)
double Verify::_cost_2(size_t p, size_t t, size_t s) const
{
	const Trip& trip = _trips.trips().at(p).at(t);
	//	const Demand& demand = _demands.demands().at(p).at(t);

	double cost = 0;

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				cost += _demands.c2().at(j).at(k) * (double)trip.x2().at(s).at(i).at(j).at(k);
			}
		}
	}

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			cost += _demands.c3().at(m) * (double)trip.x3().at(s).at(i).at(m);
		}
	}

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.c4().at(k) * (double)trip.x4().at(s).at(i).at(k);
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.b2().at(j).at(k) * (double)trip.y2().at(s).at(j).at(k);
		}
	}

	for (size_t m = 0; m < STATION; ++m)
	{
		cost += _demands.b3().at(m) * (double)trip.y3().at(s).at(m);
	}

	for (size_t k = 0; k < TASK; ++k)
	{
		cost += _demands.b4().at(k) * (double)trip.y4().at(s).at(k);
	}

	return cost;
}

// verify (16)
bool Verify::_verify_x1_y1_x2_y2(size_t p, size_t t, size_t j, size_t k, size_t s) const
{
	const Trip& trip = _trips.trips().at(p).at(t);
	const Demand& demand = _demands.demands().at(s).at(t);

	int sum_x1 = 0;
	for (size_t i = 0; i < FLEET; ++i)
	{
		sum_x1 += trip.x1().at(i).at(j).at(k);
	}

	int y1 = trip.y1().at(j).at(k);

	int sum_x2 = 0;
	for (size_t i = 0; i < FLEET; ++i)
	{
		sum_x2 += trip.x2().at(s).at(i).at(j).at(k);
	}

	int y2 = trip.y2().at(s).at(j).at(k);

	int v1 = trip.v1().at(j).at(k);

	double h1 = _demands.load().at(0);
	double h2 = _demands.load().at(1);

	return h1 * (sum_x1 + y1 + sum_x2 + y2) + h2 * v1 >= demand.d1().at(j).at(k);
}

// verify (17)
bool Verify::_verify_x3_y3(size_t p, size_t t, size_t m, size_t s) const
{
	const Trip& trip = _trips.trips().at(p).at(t);
	const Demand& demand = _demands.demands().at(s).at(t);

	int sum_x3 = 0;
	for (size_t i = 0; i < FLEET; ++i)
	{
		sum_x3 += trip.x3().at(s).at(i).at(m);
	}

	int y3 = trip.y3().at(s).at(m);

	int sum_v2 = 0;
	for (size_t n = 0; n < CAR_TYPE; ++n)
	{
		sum_v2 += trip.v2().at(n).at(m);
	}

	double h1 = _demands.load().at(0);
	double h2 = _demands.load().at(1);

	return h1 * (sum_x3 + y3) + h2 * sum_v2 >= demand.d2().at(m);
}

// verify (18)
bool Verify::_verify_x4_y4(size_t p, size_t t, size_t k, size_t s) const
{
	const Trip& trip = _trips.trips().at(p).at(t);
	const Demand& demand = _demands.demands().at(s).at(t);

	int sum_x4 = 0;
	for (size_t i = 0; i < FLEET; ++i)
	{
		sum_x4 += trip.x4().at(s).at(i).at(k);
	}

	int y4 = trip.y4().at(s).at(k);

	int v3 = trip.v3().at(k);

	double h1 = _demands.load().at(0);
	double h2 = _demands.load().at(1);

	return h1 * (sum_x4 + y4) + h2 * v3 >= demand.d3().at(k);
}

// verify (19)
bool Verify::_verify_x1_x2_x4(size_t p, size_t t, size_t i, size_t k, size_t s) const
{
	const Trip& trip = _trips.trips().at(p).at(t);
	const Demand& demand = _demands.demands().at(s).at(t);

	double sum_1 = 0;
	for (size_t j = 0; j < DISTRICT; ++j)
	{
		int x1 = trip.x1().at(i).at(j).at(k);
		int x2 = trip.x2().at(s).at(i).at(j).at(k);
		sum_1 += _demands.u1().at(j).at(k) * (double)(x1 + x2);
	}

	int x4 = trip.x4().at(s).at(i).at(k);
	double u3 = _demands.u3().at(k);

	return sum_1 + u3 * (double)x4 <= MAXWORKTIME;
}

// verify (20)
bool Verify::_verify_x3(size_t p, size_t t, size_t i, size_t s) const
{
	const Trip& trip = _trips.trips().at(p).at(t);
	const Demand& demand = _demands.demands().at(s).at(t);

	double sum = 0;
	for (size_t m = 0; m < STATION; ++m)
	{
		double u2 = _demands.u2().at(m);
		int x3 = trip.x3().at(s).at(i).at(m);
		sum += u2 * (double)x3;
	}

	return sum <= MAXWORKTIME;
}
