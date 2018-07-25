#pragma once
#include "defines.h"
#include "demands.h"
#include "trips.h"

class WriterCsv
{
public:
	WriterCsv(const Demands& d, const std::vector<std::vector<Trip> >& t);
	~WriterCsv();

	bool write_trips(const std::string& file_name) const;

private:
	std::string _cmd_k(size_t k) const;
	double _day_cost(size_t p, size_t t) const;
	double _all_day_cost(size_t p, size_t t) const;
	double _total_district_cost(size_t p, size_t t) const;
	double _total_district_task_cost(size_t p, size_t t, size_t k) const;
	double _district_cost(size_t p, size_t t, size_t j) const;
	double _district_task_cost(size_t p, size_t t, size_t j, size_t k) const;
	double _trans_cost(size_t p, size_t t) const;
	double _trans_station_cost(size_t p, size_t t, size_t m) const;
	double _major_customer_cost(size_t p, size_t t) const;
	double _cost_2(size_t p, size_t t, size_t s) const;
	void _write_d1(std::ofstream& ofile, size_t p, size_t t, size_t j, size_t k) const;
	void _write_d2(std::ofstream& ofile, size_t p, size_t t, size_t m) const;
	void _write_d3(std::ofstream& ofile, size_t p, size_t t, size_t m) const;
	void _write_cost(std::ofstream& ofile, size_t p, size_t t) const;

	const Demands& _demands;
	const std::vector<std::vector<Trip> >&   _trips;
};

