#include "gurobi.h"
#include "demands.h"

int main()
{
	/*Gurobi gurobi;
	gurobi.read();
	gurobi.start(); */

	//gurobi.test();
	Demands demand;
	demand.read();

	Gurobi gurobi;
	gurobi.monthly_trips(demand);
	gurobi.daily_trips();

	return 0;
}