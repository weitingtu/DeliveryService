#pragma once
#include "defines.h"
#include "demands.h"
#include "trips.h"

class FeasibleStochasticDemand;

class GeneticAlgorithm
{
public:
	GeneticAlgorithm(const Demands& d, const Trips& gurobi_trips, const Trips& low_cost_trips);
	~GeneticAlgorithm();

	void start();
	void start2();

	const Trips& trips() const { return _trips; }
	const std::vector<std::vector<Trip> >&  get_100_trips() const { return _100_trips; }

private:
	void _create_prob_array(std::array<double, POPULATION * 2>& array);
	size_t _random_select(std::array<double, POPULATION * 2>& prob_array) const;
	size_t _random_select(const std::vector<double>& prob_array) const;
	const std::vector<Trip>& _get_trip(size_t index) const;
	void _get_exchange_index(size_t& t1, size_t& t2) const;
	void _mutant(std::vector<Trip>& pi_best, const std::vector<Trip>& pi_incumbent) const;
	bool _is_accept(double best_cost, double cost) const;
	void _start(size_t p);
	double _cost_1(const std::vector<Trip>& trip) const;
	double _cost_1(const Trip& trip) const;

	std::vector<double> _generate_prob(const std::vector<std::vector<Trip>>& prev_trips) const;
	std::vector<std::vector<Trip> > _select_100(const std::vector<std::vector<Trip> >& prev_trips) const;
	std::vector<Trip> _mate(const std::vector<Trip>& trips1, const std::vector<Trip>& trips2) const;
	void _run_daily(std::vector<Trip>& new_trips) const;
	void _start2(FILE* pfp, size_t ite_count, FeasibleStochasticDemand& fs);


	const Demands& _demands;
	const Trips& _gurobi_trips;
	const Trips& _low_cost_trips;
	Trips _trips;
	std::array<double, POPULATION * 2> _prob;

	std::vector<std::vector<Trip>> _100_trips;

};

