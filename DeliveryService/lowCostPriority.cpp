#include "lowCostPriority.h"
#include "verify.h"
#include <math.h>
#include <numeric>

LowCostPriority::LowCostPriority(const Demands& d, Trips& t) :
	_demands(d),
	_trips(t),
	_min_max_c1(),
	_max_min_c1()
{
	_initialize_cost_array();
}


LowCostPriority::~LowCostPriority()
{
}

void LowCostPriority::_initialize_cost_array()
{
	for (size_t k = 0; k < TASK; ++k)
	{
		std::vector<std::pair<double, size_t> > c;
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			c.push_back(std::make_pair(_demands.c1()[j][k], j));
		}
		std::sort(c.begin(), c.end());
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			_min_max_c1[j][k] = c[j].second;
		}
		std::reverse(c.begin(), c.end());
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			_max_min_c1[j][k] = c[j].second;
		}
	}
}

void LowCostPriority::start()
{
	for (size_t p = 0; p < POPULATION; ++p)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			_start(p, t);
		}
		Verify v(_demands, _trips);
		v.verify_momthly(p);
	}
}

void LowCostPriority::_start(size_t p, size_t t)
{
	const Demand& demand = _demands.demands()[p][t];
	Trip& trip = _trips.trips()[p][t];

	std::vector<int> v2(STATION);
	std::vector<int> v3(TASK);

	// step 1
	for (size_t m = 0; m < STATION; ++m)
	{
		v2[m] = (int)std::ceil(demand.d2()[m] / _demands.load()[1]);
	}

	int v2_total = std::accumulate(v2.begin(), v2.end(), 0);

	for (size_t k = 0; k < TASK; ++k)
	{
		v3[k] = (int)std::ceil(demand.d3()[k] / _demands.load()[1]);
	}

	int v3_total = std::accumulate(v3.begin(), v3.end(), 0);

	if (v3_total > v2_total)
	{
		v2[0] += v3_total - v2_total;
		v2_total += v3_total - v2_total;
	}

	// step 2
	for (size_t k = 0; k < TASK; ++k)
	{
		size_t n = k == 0 ? 1 : 2;
		// k 0: collecting
		// k 1: sending
		// n 1: station collecting, district sending
		// n 2: station sending, district collecting
		int v3_count = v3[k];
		for (size_t m = 0; m < STATION; ++m)
		{
			if (0 == v3_count)
			{
				break;;
			}
			if (0 == v2[m])
			{
				continue;
			}
			int count = (v3_count <= v2[m]) ? v3_count : v2[m];
			trip.v2()[n][m] = count;
			v3_count -= count;
			v2[m] -= count;
		}
		trip.v3()[k] = v3[k] - v3_count;
	}

	for (size_t m = 0; m < STATION; ++m)
	{
		trip.v2()[0][m] = v2[m];
	}

	// step 3
	std::vector<std::vector<double> > d1(DISTRICT);

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		d1[j].resize(TASK);
		for (size_t k = 0; k < TASK; ++k)
		{
			d1[j][k] = demand.d1()[j][k];
		}
	}

	for (size_t k = 0; k < TASK; ++k)
	{
		// k 0: collecting
		// k 1: sending
		// n 1: station collecting, district sending
		// n 2: station sending, district collecting
		// k 0 -> use v2[0][m] and v2[2][m]
		// k 1 -> use v2[0][m] and v2[1][m]
		int v2_count = 0;
		size_t n = k == 0 ? 2 : 1;
		for (size_t m = 0; m < STATION; ++m)
		{
			v2_count += trip.v2()[0][m];
			v2_count += trip.v2()[n][m];
		}
		for (size_t jj = 0; jj < DISTRICT; ++jj)
		{
			size_t j = _max_min_c1[jj][k];
			if (0 == v2_count)
			{
				break;;
			}
			if (0 == d1[j][k])
			{
				continue;
			}
			int d1_count = (int)std::ceil(d1[j][k] / _demands.load()[1]);
			if (v2_count >= d1_count)
			{
				d1[j][k] = 0.0;
				v2_count -= d1_count;
				trip.v1()[j][k] = d1_count;
			}
			else
			{
				d1[j][k] -= v2_count * _demands.load()[1];
				trip.v1()[j][k] = v2_count;
				v2_count = 0;
			}
		}
	}

	// step 4, x1
	size_t i = 0; // FLEET
	double total_worktime = MAXWORKTIME;

	for (size_t k = 0; k < TASK; ++k)
	{
		if (i >= FLEET)
		{
			break;
		}
		for (size_t jj = 0; jj < DISTRICT; ++jj)
		{
			if (i >= FLEET)
			{
				break;
			}
			size_t j = _min_max_c1[jj][k];
			double work_time = _demands.u1()[j][k] * 2 + WORKTIME;
			if (work_time > MAXWORKTIME)
			{
				// impossible to send/collect
				continue;
			}
			while (d1[j][k] > 0)
			{
				if (total_worktime < work_time)
				{
					// change car
					++i;
					total_worktime = MAXWORKTIME;
					if (i >= FLEET)
					{
						break;
					}
				}
				++trip.x1()[i][j][k];
				total_worktime -= work_time;
				d1[j][k] -= _demands.load()[0];
				if (d1[j][k] < 0)
				{
					d1[j][k] = 0;
				}
			}
		}
	}

	// step 5, y1
	for (size_t k = 0; k < TASK; ++k)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			trip.y1()[j][k] = (int)std::ceil(d1[j][k] / _demands.load()[0]);
		}
	}
}
