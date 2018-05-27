#pragma once
#include "defines.h"
#include "demands.h"
#include "trips.h"
#include <array>
#include <vector>
#include <string>
#include <gurobi_c++.h>

class LowCostPriority
{
public:
	LowCostPriority(const Demands& d, Trips& t);
	~LowCostPriority();

	void start();

private:
	void _initialize_cost_array();
	void _start(size_t p, size_t t);

	const Demands& _demands;
	Trips&  _trips;
	std::array<std::array<size_t, TASK>, DISTRICT> _min_max_c1;
	std::array<std::array<size_t, TASK>, DISTRICT> _max_min_c1;
};

