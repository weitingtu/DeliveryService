#pragma once
#include "defines.h"
#include "demand.h"
#include "trips.h"
#include <array>
#include <vector>
#include <string>
#include <gurobi_c++.h>

class LowCostPriority
{
public:
	LowCostPriority();
	~LowCostPriority();

	void start(const Demand& demand);

private:

	Demand _demand;
	Trips  _trips;
};

