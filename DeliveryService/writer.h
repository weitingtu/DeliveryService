#pragma once
#include "defines.h"
#include "demands.h"
#include "trips.h"

class Writer
{
public:
	Writer(const Demands& d, const Trips& t);
	~Writer();

	bool write_trips(const std::string& file_name) const;

private:
	std::string _cmd_k(size_t k) const;
	double _cost_1(size_t p, size_t t) const;
	double _cost_2(size_t p, size_t t, size_t s) const;
	void _write_d1(std::ofstream& ofile, size_t p, size_t t, size_t j, size_t k) const;
	void _write_d2(std::ofstream& ofile, size_t p, size_t t, size_t m) const;
	void _write_d3(std::ofstream& ofile, size_t p, size_t t, size_t m) const;
	void _write_cost(std::ofstream& ofile, size_t p, size_t t) const;

	const Demands& _demands;
	const Trips&   _trips;
};

