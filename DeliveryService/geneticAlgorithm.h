#pragma once
#include "defines.h"
#include "demands.h"
#include "trips.h"

class GeneticAlgorithm
{
public:
	GeneticAlgorithm(const Demands& d, const Trips& gurobi_trips, const Trips& low_cost_trips);
	~GeneticAlgorithm();

	void start();

	const Trips& trips() const { return _trips; }

private:
	void _create_prob_array(std::array<double, POPULATION * 2>& array);
	size_t _random_select(std::array<double, POPULATION * 2>& prob_array) const;
	const std::vector<Trip>& _get_trip(size_t index) const;
	void _get_exchange_index(size_t& t1, size_t& t2) const;
	void _mutant(std::vector<Trip>& pi_best, const std::vector<Trip>& pi_incumbent) const;
	bool _is_accept(double best_cost, double cost) const;
	void _start(size_t p);
	double _cost_1(const std::vector<Trip>& trip) const;
	double _cost_1(const Trip& trip) const;

	const Demands& _demands;
	const Trips& _gurobi_trips;
	const Trips& _low_cost_trips;
	Trips _trips;
	std::array<double, POPULATION * 2> _prob;

};

