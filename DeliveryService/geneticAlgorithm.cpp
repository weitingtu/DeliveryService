#include "geneticAlgorithm.h"
#include <stdlib.h>
#include "gurobi.h"



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
		prob_array[i] = 1 / cost;
		total_cost += 1 / cost;
	}
	for (size_t p = 0; p < POPULATION; ++p, ++i)
	{
		double cost = _cost_1(_gurobi_trips.trips()[p]);
		prob_array[i] = 1 / cost;
		total_cost += 1 / cost;
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

	_trips.trips()[p] = _gurobi_trips.trips()[p];
	std::vector<Trip> pi_best = _trips.trips()[p];
	double best_cost = _cost_1(pi_best);
	double init_cost = best_cost;

	size_t max_count = 1000;
	size_t max_non_improve_count = 100;
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