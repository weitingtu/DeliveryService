#include "gurobi.h"
#include "demands.h"
#include "verify.h"
#include "feasibleStochasticDemand.h"
#include "lowCostPriority.h"
#include "writer.h"
#include "writerCsv.h"
#include "geneticAlgorithm.h"

int main()
{
	Demands demands;
	demands.read();

	//Trips trips;

	//LowCostPriority l(demands, trips);
	//l.monthly_trips();
	//l.daily_trips();

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

	//Trips t;
	//t.read_monthly_trips();
	//t.read_daily_trips();
	//for (size_t p = 0; p < POPULATION; ++p)
	//{
	//	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	//	{
	//		printf("Verify population %zu stochastic demand %zu\n", p, s);
	//		Verify v(demands, t);
	//		v.verify_daily(p, s);
	//	}
	//}
	//for (size_t p = 0; p < POPULATION; ++p)
	//{
	//	Verify v(demands, t);
	//	double cost = v.cost(p);
	//	printf("Population %zu average cost %f\n", p, cost);
	//}

	//FeasibleStochasticDemand f(demands, t);
	//f.start2();
	//Writer w(demands, t);
	//w.write_trips("result.txt");

	Trips gurobi_trips;
	//Gurobi gurobi(demands, gurobi_trips);
	//gurobi.monthly_trips();
	//gurobi.daily_trips();
	gurobi_trips.read_monthly_trips();
	gurobi_trips.read_daily_trips();

	Trips low_cost_trips;
	LowCostPriority l(demands, low_cost_trips);
	l.monthly_trips();
	l.daily_trips();

	GeneticAlgorithm ga(demands, gurobi_trips, low_cost_trips);
	ga.start2();

	//Writer w(demands, ga.trips());
	//w.write_trips("ga_result.txt");

	//WriterCsv w1(demands, gurobi_trips);
	//w1.write_trips("gurobi.csv");

	//WriterCsv w2(demands, low_cost_trips);
	//w2.write_trips("low_cost.csv");

	WriterCsv w3(demands, ga.get_100_trips());
	w3.write_trips("ga.csv");

	return 0;
}