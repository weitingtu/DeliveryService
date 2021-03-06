#include "geneticAlgorithm.h"
#include <stdlib.h>
#include "gurobi.h"
#include "lowCostPriority.h"
#include "feasibleStochasticDemand.h"
#include <functional>
#include <queue>
#include <time.h>

GeneticAlgorithm::GeneticAlgorithm(const Demands& d, const Trips& gurobi_trips, const Trips& low_cost_trips) :
	_demands(d),
	_gurobi_trips(gurobi_trips),
	_low_cost_trips(low_cost_trips),
	_trips(),
	_prob()
{
}

GeneticAlgorithm::~GeneticAlgorithm()
{
}

void GeneticAlgorithm::_create_prob_array(std::array<double, POPULATION * 2>& prob_array)
{
	double total_cost = 0.0;
	size_t i = 0;
	for (size_t p = 0; p < POPULATION; ++p, ++i)
	{
		double cost = _cost_1(_gurobi_trips.trips()[p]);
		prob_array[i] = 1 / (cost * cost);
		total_cost += 1 / (cost * cost);
	}
	for (size_t p = 0; p < POPULATION; ++p, ++i)
	{
		double cost = _cost_1(_low_cost_trips.trips()[p]);
		prob_array[i] = 1 / (cost * cost);
		total_cost += 1 / (cost * cost);
	}

	prob_array[0] = prob_array[0] / total_cost;
	for (i = 1; i < prob_array.size(); ++i)
	{
		double p = prob_array[i] / total_cost;
		prob_array[i] = prob_array[i - 1] + p;
	}
}

size_t GeneticAlgorithm::_random_select(std::array<double, POPULATION * 2>& prob_array) const
{
	double rf = rand() / (RAND_MAX + 1.0);
	size_t i = 0;
	for (; i < prob_array.size(); ++i)
	{
		if (rf <= prob_array[i])
		{
			break;
		}
	}

	return i;
}

size_t GeneticAlgorithm::_random_select(const std::vector<double>& prob_array) const
{
	double rf = rand() / (RAND_MAX + 1.0);
	size_t i = 0;
	for (; i < prob_array.size(); ++i)
	{
		if (rf <= prob_array[i])
		{
			break;
		}
	}

	return i;
}

const std::vector<Trip>& GeneticAlgorithm::_get_trip(size_t index) const
{
	if (index < POPULATION)
		return _gurobi_trips.trips()[index];
	return _low_cost_trips.trips()[index - POPULATION];
}

void GeneticAlgorithm::_get_exchange_index(size_t& t1, size_t& t2) const
{
	t1 = (size_t)((rand() / (RAND_MAX + 1.0)) * DAY);
	t2 = (size_t)((rand() / (RAND_MAX + 1.0)) * DAY);
	if (t1 > t2)
	{
		std::swap(t1, t2);
	}
}

void GeneticAlgorithm::_mutant(std::vector<Trip>& pi_best, const std::vector<Trip>& pi_incumbent) const
{
	size_t index1 = 0;
	size_t index2 = 0;
	_get_exchange_index(index1, index2);
	for (size_t t = 0; t < DAY; ++t)
	{
		if (t > index1 && t <= index2)
		{
			pi_best[t] = pi_incumbent[t];
		}
	}
}

bool GeneticAlgorithm::_is_accept(double best_cost, double cost) const
{
	if (cost == best_cost)
	{
		return false;
	}
	if (cost < best_cost)
	{
		return true;
	}
	double ap = std::exp(-((cost - best_cost) / best_cost) * 300);
	double alpha = rand() / (RAND_MAX + 1.0);

	return ap >= alpha;
}

void GeneticAlgorithm::start()
{
	srand(0);
	for (size_t p = 0; p < POPULATION; ++p)
	{
		_start(p);
	}
}

void GeneticAlgorithm::_start(size_t p)
{
	std::array<double, POPULATION * 2> prob_array;
	_create_prob_array(prob_array);

	double gurobi_cost = _cost_1(_gurobi_trips.trips()[p]);
	double low_cost_cost = _cost_1(_low_cost_trips.trips()[p]);

	if (gurobi_cost < low_cost_cost)
	{
		_trips.trips()[p] = _gurobi_trips.trips()[p];
	}
	else
	{
		_trips.trips()[p] = _low_cost_trips.trips()[p];
	}

	std::vector<Trip> pi_best = _trips.trips()[p];
	double best_cost = _cost_1(pi_best);
	double init_cost = best_cost;

	size_t max_count = 100;
	size_t max_non_improve_count = 10;
	size_t count = 0;
	size_t non_improve_count = 0;
	while (count < max_count && non_improve_count < max_non_improve_count)
	{
		size_t index = _random_select(prob_array);
		const std::vector<Trip>& pi_incumbent = _get_trip(index);

		_mutant(pi_best, pi_incumbent);

		// gurobi for daily trip
		std::vector<Trip> pi_orig = _trips.trips()[p];
		_trips.trips()[p] = pi_best;

		Gurobi gurobi(_demands, _trips);
		gurobi.daily_trip(p);

		pi_best = _trips.trips()[p];
		_trips.trips()[p] = pi_orig;

		double cost = _cost_1(pi_best);
		printf("p %zu, count %zu, non-improve count %zu, cost %f -> %f\n", p, count, non_improve_count, best_cost, cost);
		if (_is_accept(best_cost, cost))
		{
			_trips.trips()[p] = pi_best;
			best_cost = cost;
			non_improve_count = 0;
		}
		else
		{
			pi_best = _trips.trips()[p];
			++non_improve_count;
		}
		++count;
	}
	printf("p %zu, count %zu, non-improve count %zu, cost %f -> %f\n", p, count, non_improve_count, init_cost, best_cost);
}

double GeneticAlgorithm::_cost_1(const std::vector<Trip>& trip) const
{
	double cost = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		cost += _cost_1(trip[t]);
		for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
		{
			cost += _cost_2(trip[t], s) / STOCHASTIC_DEMAND;
		}
	}

	return cost;
}

// (7)
double GeneticAlgorithm::_cost_1(const Trip& trip) const
{
	double cost = 0;

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				cost += _demands.c1().at(j).at(k) * (double)trip.x1().at(i).at(j).at(k);
			}
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.b1().at(j).at(k) * (double)trip.y1().at(j).at(k);
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.a1().at(j).at(k) * (double)trip.v1().at(j).at(k);
		}
	}

	for (size_t n = 0; n < CAR_TYPE; ++n)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			cost += _demands.a2().at(m) * (double)trip.v2().at(n).at(m);
		}
	}

	for (size_t k = 0; k < TASK; ++k)
	{
		cost += _demands.a3().at(k) * (double)trip.v3().at(k);
	}

	return cost;
}

double GeneticAlgorithm::_cost_2(const Trip& trip, size_t s) const
{
	double cost = 0;

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				cost += _demands.c2().at(j).at(k) * (double)trip.x2().at(s).at(i).at(j).at(k);
			}
		}
	}

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			cost += _demands.c3().at(m) * (double)trip.x3().at(s).at(i).at(m);
		}
	}

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.c4().at(k) * (double)trip.x4().at(s).at(i).at(k);
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.b2().at(j).at(k) * (double)trip.y2().at(s).at(j).at(k);
		}
	}

	for (size_t m = 0; m < STATION; ++m)
	{
		cost += _demands.b3().at(m) * (double)trip.y3().at(s).at(m);
	}

	for (size_t k = 0; k < TASK; ++k)
	{
		cost += _demands.b4().at(k) * (double)trip.y4().at(s).at(k);
	}

	return cost;
}

void GeneticAlgorithm::start2()
{
	for (size_t p = 0; p < _gurobi_trips.trips().size(); ++p)
	{
		_100_trips.push_back(_gurobi_trips.trips()[p]);
	}
	for (size_t p = 0; p < _low_cost_trips.trips().size(); ++p)
	{
		_100_trips.push_back(_low_cost_trips.trips()[p]);
	}

	FILE* fp = nullptr;
	errno_t err;
	if ((err = fopen_s(&fp, "GA_runtime.csv", "w")) != 0)
	{
		printf("Error, unable to open file GA_runtime.csv\n");
		return;
	}
	fprintf(fp, "iteration, running time, accumulated time\n");

	FILE* pfp = nullptr;
	if ((err = fopen_s(&pfp, "probability.csv", "w")) != 0)
	{
		printf("Error, unable to open file probability.csv\n");
		return;
	}
	fprintf(pfp, "iteration, p, s, N1, X1, p(X>x1), N2, X1, p(X>x2), N3, X3, p(X>x3), pf = p(X>x1)*p(X>x2)*p(X>x3)\n");

	double total_runtime = 0.0;
	size_t ite_count = 0;
	FeasibleStochasticDemand fs(_demands);
	while (ite_count < 1)
	{
		printf("GA iteration: %zu\n", ite_count);
		time_t start_t = clock();
		_start2(pfp, ite_count, fs);
		double runtime = (double)(clock() - start_t) / CLOCKS_PER_SEC;
		total_runtime += runtime;
		printf("run time (total): %.2fs (%.2fs)\n", runtime, total_runtime);
		fprintf(fp, "%zu, %.2fs, %.2fs\n", ite_count, runtime, total_runtime);

		++ite_count;
	}
	fclose(fp);
	fclose(pfp);

	_write_best(_100_trips);
}

std::vector<double> GeneticAlgorithm::_generate_prob(const std::vector<std::vector<Trip>>& prev_trips) const
{
	std::vector<double> prob(prev_trips.size(), 0.0);
	double total_cost = 0.0;
	for (size_t p = 0; p < prev_trips.size(); ++p)
	{
		double cost = _cost_1(prev_trips[p]);
		prob[p] = 1 / (cost * cost);
		total_cost += 1 / (cost * cost);
	}

	prob[0] = prob[0] / total_cost;
	for (size_t i = 1; i < prob.size(); ++i)
	{
		double p = prob[i] / total_cost;
		prob[i] = prob[i - 1] + p;
	}

	return prob;
}

std::vector<std::vector<Trip>> GeneticAlgorithm::_select_100(const std::vector<std::vector<Trip> >& prev_trips) const
{
	std::vector<std::vector<Trip>> trips;
	std::vector<double> prob = _generate_prob(prev_trips);

	for (size_t p = 0; p < prev_trips.size(); ++p)
	{
		size_t i = _random_select(prob);
		trips.push_back(prev_trips[p]);
	}

	return trips;
}

std::vector<Trip> GeneticAlgorithm::_mate(const std::vector<Trip>& trips1, const std::vector<Trip>& trips2) const
{
	std::vector<Trip> trips;

	const double prob_threshold = 0.6;
	bool start_exchange = false;
	bool finish_exchange = false;

	for (size_t i = 0; i < trips1.size(); ++i)
	{
		double rf = rand() / (RAND_MAX + 1.0);
		if (rf >= prob_threshold)
		{
			if (!start_exchange)
			{
				start_exchange = true;
			}
			else if (!finish_exchange)
			{
				finish_exchange = true;
			}
		}

		if (start_exchange && !finish_exchange)
		{
			trips.push_back(trips2.at(i));
		}
		else
		{
			trips.push_back(trips1.at(i));
		}
	}

	return trips;
}

void GeneticAlgorithm::_run_daily(std::vector<Trip>& new_trips) const
{
	for (size_t i = 0; i < new_trips.size(); ++i)
	{
		new_trips.at(i).clear_daily();
	}
	// low cost for daily trip
	LowCostPriority::daily_trip(_demands, new_trips);
	Gurobi::solve_x(_demands, new_trips);

	// gurobi for daily trip
	//Trips daily_trips = _gurobi_trips;
	//daily_trips.trips()[0] = new_trips;

	//Gurobi daily_runner(_demands, daily_trips);
	//daily_runner.daily_trip(0);

	//new_trips = daily_trips.trips()[0];
}

void GeneticAlgorithm::_start2(FILE* pfp, size_t ite_count, FeasibleStochasticDemand& fs)
{
	std::vector<std::vector<Trip>> prev_trips = _select_100(_100_trips);

	std::vector<double> prob = _generate_prob(prev_trips);

	std::vector<std::vector<Trip>> all_trips;

	time_t start_t = clock();
	double total_runtime = 0.0;
	size_t count = 0;
	while (count < 90)
	{
		size_t i_1 = _random_select(prob);
		size_t i_2 = _random_select(prob);
		if (i_1 == i_2)
		{
			all_trips.push_back(prev_trips.at(i_1));
			continue;
		}
		std::vector<Trip> new_trips = _mate(prev_trips.at(i_1), prev_trips.at(i_2));

		_run_daily(new_trips);
		fs.compute(pfp, ite_count, count, new_trips);
		//if (fs.start(pfp, ite_count, count, new_trips))
		{
			time_t end_t = clock();
			double runtime = (double)(end_t - start_t) / CLOCKS_PER_SEC;
			total_runtime += runtime;
			printf("%zu, %zu cost %f run time %f %f\n", ite_count, count, _cost_1(new_trips), runtime, total_runtime);
			start_t = end_t;
			++count;
			all_trips.push_back(std::move(new_trips));
		}
	}

	while (count < 94)
	{
		size_t idx = (size_t)((rand() / (RAND_MAX + 1.0)) * 90);
		double rf = rand() / (RAND_MAX + 1.0);
		if (rf > 0.01)
		{
			continue;
		}
		std::vector<Trip> new_trips = all_trips.at(idx);
		size_t index1 = 0;
		size_t index2 = 0;
		_get_exchange_index(index1, index2);
		std::swap(new_trips.at(index1), new_trips.at(index2));

		_run_daily(new_trips);
		fs.compute(pfp, ite_count, count, new_trips);
		//if (fs.start(pfp, ite_count, count, new_trips))
		{
			time_t end_t = clock();
			double runtime = (double)(end_t - start_t) / CLOCKS_PER_SEC;
			total_runtime += runtime;
			printf("%zu, %zu cost %f run time %f %f\n", ite_count, count, _cost_1(new_trips), runtime, total_runtime);
			start_t = end_t;
			++count;
			all_trips.push_back(std::move(new_trips));
		}
	}


	std::priority_queue < std::pair<double, size_t>, std::vector<std::pair<double, size_t> >, std::greater<std::pair<double, size_t> >> q;
	for (size_t p = 0; p < prev_trips.size(); ++p)
	{
		double cost = _cost_1(prev_trips[p]);
		q.push(std::make_pair(cost, p));
	}

	while (count < 100)
	{
		std::vector<Trip> new_trips = prev_trips.at(q.top().second);
		_run_daily(new_trips);
		fs.compute(pfp, ite_count, count, new_trips);
		//if (fs.start(pfp, ite_count, count, new_trips))
		{
			time_t end_t = clock();
			double runtime = (double)(end_t - start_t) / CLOCKS_PER_SEC;
			total_runtime += runtime;
			printf("%zu, %zu cost %f run time %f %f\n", ite_count, count, _cost_1(new_trips), runtime, total_runtime);
			start_t = end_t;
			++count;
			all_trips.push_back(std::move(new_trips));
		}
		q.pop();
	}
	_100_trips = all_trips;

	double min_cost = std::numeric_limits<double>::max();
	for (size_t p = 0; p < all_trips.size(); ++p)
	{
		double cost = _cost_1(all_trips[p]);
		if (cost < min_cost)
		{
			min_cost = cost;
		}
	}
	printf("min cost %f\n", min_cost);
}

void GeneticAlgorithm::_write_best(const std::vector<std::vector<Trip> >& all_trips) const
{
	if (all_trips.empty())
	{
		printf("error, unable to write best solution. Solution is empty\n");
		return;
	}

	std::priority_queue < std::pair<double, size_t>, std::vector<std::pair<double, size_t> >, std::greater<std::pair<double, size_t> >> q;
	for (size_t p = 0; p < all_trips.size(); ++p)
	{
		double cost = _cost_1(all_trips[p]);
		q.push(std::make_pair(cost, p));
	}

	int sum_x1 = 0;
	int sum_y1 = 0;
	int sum_v1 = 0;
	int sum_v2 = 0;
	int sum_v3 = 0;
	int sum_x2 = 0;
	int sum_y2 = 0;
	int sum_x3 = 0;
	int sum_y3 = 0;
	int sum_x4 = 0;
	int sum_y4 = 0;

	const std::vector<Trip>& trips = all_trips.at(q.top().second);
	for (size_t t = 0; t < trips.size(); ++t)
	{
		const Trip& trip = trips.at(t);
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					sum_x1 += trip.x1().at(i).at(j).at(k);
				}
			}
		}
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				sum_y1 += trip.y1().at(j).at(k);
			}
		}
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				sum_v1 += trip.v1().at(j).at(k);
			}
		}
		for (size_t n = 0; n < CAR_TYPE; ++n)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				sum_v2 += trip.v2().at(n).at(m);
			}
		}
		for (size_t k = 0; k < TASK; ++k)
		{
			sum_v3 += trip.v3().at(k);
		}

		for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t j = 0; j < DISTRICT; ++j)
				{
					for (size_t k = 0; k < TASK; ++k)
					{
						sum_x2 += trip.x2().at(s).at(i).at(j).at(k);
					}
				}
			}
		}
		for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					sum_y2 += trip.y2().at(s).at(j).at(k);
				}
			}
		}
		for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t m = 0; m < STATION; ++m)
				{
					sum_x3 += trip.x3().at(s).at(i).at(m);
				}
			}
		}
		for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				sum_y3 += trip.y3().at(s).at(m);
			}
		}
		for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					sum_x4 += trip.x4().at(s).at(i).at(k);
				}
			}
		}
		for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				sum_y4 += trip.y4().at(s).at(k);
			}
		}
	}

	FILE* fp = nullptr;
	errno_t err;
	if ((err = fopen_s(&fp, "best_sol.csv", "w")) != 0)
	{
		printf("Error, unable to open file best_sol.csv\n");
		return;
	}
	fprintf(fp, "x1, y1, v1, v2, v3, x2, y2, x3, y3, x4, y4\n");
	fprintf(fp, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
		sum_x1, sum_y1, sum_v1, sum_v2, sum_v3, sum_x2, sum_y2, sum_x3, sum_y3, sum_x4, sum_y4);

	fclose(fp);
}
