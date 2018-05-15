#include "gurobi.h"
#include "demands.h"
#include "verify.h"
#include "feasibleStochasticDemand.h"

int main()
{
	Demands demands;
	demands.read();

	//Trips trips;

	//Gurobi gurobi(demands, trips);
	//gurobi.monthly_trips();

	//trips.write_monthly_trips();

	//Trips trips;
	//trips.read_monthly_trips();
	//for (size_t p = 0; p < POPULATION; ++p)
	//{
	//	printf("Verify population %zu\n", p);
	//	Verify v(demands, t);
	//	v.verify_momthly(p);
	//}

	//Gurobi gurobi(demands, trips);
	//gurobi.daily_trips();
	//trips.write_daily_trips();

	Trips t;
	t.read_monthly_trips();
	t.read_daily_trips();
	//for (size_t p = 0; p < POPULATION; ++p)
	//{
	//	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	//	{
	//		printf("Verify population %zu stochastic demand %zu\n", p, s);
	//		Verify v(demands, t);
	//		v.verify_daily(p, s);
	//	}
	//}
	for (size_t p = 0; p < POPULATION; ++p)
	{
		Verify v(demands, t);
		double cost = v.cost(p);
		printf("Population %zu average cost %f\n", p, cost);
	}

	//FeasibleStochasticDemand f(demands, t);
	//f.start();

	return 0;
}