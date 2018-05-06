#include "gurobi.h"
#include "demands.h"
#include "verify.h"

int main()
{
	Demands demands;
	demands.read();

	//Trips trips;

	//Gurobi gurobi(demands, trips);
	//gurobi.monthly_trips();

	//trips.write_monthly_trips();

	Trips t;
	t.read_monthly_trips();
	for (size_t p = 0; p < POPULATION; ++p)
	{
		printf("Verify population %zu\n", p);
		Verify v(demands, t);
		v.verify_momthly(p);
	}

	//gurobi.daily_trips();

	return 0;
}