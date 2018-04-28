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
	LowCostPriority();
	~LowCostPriority();

	void start(const Demands& demand);

private:

	Demands _demand;
	Trips  _trips;
};

