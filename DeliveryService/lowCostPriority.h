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

	void monthly_trips();
	void daily_trips();
	static void daily_trip(size_t p, const Demands& demands, std::vector<Trip>& trips);

private:
	void _initialize_cost_array();
	void _monthly_trips(size_t p, size_t t);
	void _run_daily_trips(size_t p, size_t s);
	void _daily_trips(size_t p, size_t s, size_t t);

	static void _daily_trips(const Demands& demands,
		const std::array<std::array<size_t, TASK>, DISTRICT>& min_max_c1,
		const std::array<std::array<size_t, TASK>, DISTRICT>& max_min_c1,
		const Demand& demand, Trip& trip, size_t s);
	static void _initialize_cost_array(const Demands& demands,
		std::array<std::array<size_t, TASK>, DISTRICT>& min_max_c1,
		std::array<std::array<size_t, TASK>, DISTRICT>& max_min_c1);

	const Demands& _demands;
	Trips&  _trips;
	std::array<std::array<size_t, TASK>, DISTRICT> _min_max_c1;
	std::array<std::array<size_t, TASK>, DISTRICT> _max_min_c1;
};

