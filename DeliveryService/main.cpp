#include "gurobi.h"
#include "demand.h"

int main()
{
	/*Gurobi gurobi;
	gurobi.read();
	gurobi.start(); */

	//gurobi.test();
	Demand demand;
	demand.read();

	Gurobi gurobi;
	gurobi.monthly_trips(demand);

	return 0;
}