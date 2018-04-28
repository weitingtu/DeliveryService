#include "lowCostPriority.h"



LowCostPriority::LowCostPriority() :
	_demand(),
	_trips()
{
}


LowCostPriority::~LowCostPriority()
{
}

void LowCostPriority::start(const Demands& demand)
{
	_demand = demand;
}
