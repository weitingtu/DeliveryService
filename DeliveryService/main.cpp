#include "gurobi.h"
#include "demands.h"
#include "verify.h"
#include "feasibleStochasticDemand.h"

int main()
{
	Demands demands;
	demands.read();

	Trips trips;

	Gurobi gurobi(demands, trips);
	gurobi.monthly_trips();

	trips.write_monthly_trips();

	Trips t;
	t.read_monthly_trips();
	for (size_t p = 0; p < POPULATION; ++p)
	{
		printf("Verify population %zu\n", p);
		Verify v(demands, t);
		v.verify_momthly(p);
	}

	//Gurobi gurobi(demands, t);
	//gurobi.daily_trips();

	//FeasibleStochasticDemand f(demands, t);
	//f.start();

	return 0;
}