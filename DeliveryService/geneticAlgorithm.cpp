#include "geneticAlgorithm.h"
#include <stdlib.h>
#include "gurobi.h"
#include "lowCostPriority.h"
#include <functional>
#include <queue>



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

	size_t count = 0;
	while (count < 100)
	{
		printf("GA iteration: %zu\n", count);
		++count;
		_start2();
	}
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

void GeneticAlgorithm::_start2()
{
	std::vector<std::vector<Trip>> prev_trips = _select_100(_100_trips);

	std::vector<double> prob = _generate_prob(prev_trips);

	std::vector<std::vector<Trip>> all_trips;

	size_t count = 0;
	while (count < 90)
	{
		++count;
		size_t i_1 = _random_select(prob);
		size_t i_2 = _random_select(prob);
		if (i_1 == i_2)
		{
			all_trips.push_back(prev_trips.at(i_1));
			continue;
		}
		std::vector<Trip> new_trips = _mate(prev_trips.at(i_1), prev_trips.at(i_2));

		// gurobi for daily trip
		Trips daily_trips = _gurobi_trips;
		daily_trips.trips()[0] = new_trips;

		//Gurobi daily_runner(_demands, daily_trips);
		LowCostPriority daily_runner(_demands, daily_trips);
		daily_runner.daily_trip(0);

		new_trips = daily_trips.trips()[0];

		all_trips.push_back(new_trips);
	}

	while (count < 94)
	{
		size_t idx = (size_t)((rand() / (RAND_MAX + 1.0)) * 90);
		double rf = rand() / (RAND_MAX + 1.0);
		if (rf > 0.01)
		{
			continue;
		}
		++count;
		std::vector<Trip> new_trips = all_trips.at(idx);
		size_t index1 = 0;
		size_t index2 = 0;
		_get_exchange_index(index1, index2);
		std::swap(new_trips.at(index1), new_trips.at(index2));
		all_trips.push_back(new_trips);
	}


	std::priority_queue < std::pair<double, size_t>, std::vector<std::pair<double, size_t> >, std::greater<std::pair<double, size_t> >> q;
	for (size_t p = 0; p < prev_trips.size(); ++p)
	{
		double cost = _cost_1(prev_trips[p]);
		q.push(std::make_pair(cost, p));
	}

	while (count < 100)
	{
		++count;
		all_trips.push_back(prev_trips.at(q.top().second));
		q.pop();
	}
	_100_trips = all_trips;
}
