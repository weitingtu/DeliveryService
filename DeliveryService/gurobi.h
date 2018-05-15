#pragma once
#include "defines.h"
#include "demands.h"
#include "trips.h"
#include <array>
#include <vector>
#include <string>
#include <gurobi_c++.h>

class Gurobi
{
public:
	Gurobi(const Demands& d, Trips& t);
	~Gurobi();

	void test() const;

	void monthly_trips();
	void daily_trips();

private:
	void _run_monthly_trips(size_t scenerio);
	void _run_daily_trips(size_t population, size_t s);

	bool _is_file_exist(const std::string& file_name) const;
	bool _delete_file(const std::string& file_name) const;
	bool _delete_files() const;
	std::string _var_name(std::string str, const std::vector<size_t>& idx) const;

	bool _write_x1(const std::vector<std::vector<std::vector<std::vector<GRBVar> > > >& x1, size_t population);
	bool _write_y1(const std::vector<std::vector<std::vector<GRBVar> > > & y1, size_t population);
	bool _write_v1(const std::vector<std::vector<std::vector<GRBVar> > > & v1, size_t population);
	bool _write_v2(const std::vector<std::vector<std::vector<GRBVar> > > & v2, size_t population);
	bool _write_v3(const std::vector<std::vector<GRBVar> >& v3, size_t population);

	void _write_x2(size_t p, size_t s, const std::vector<std::vector<std::vector<std::vector<GRBVar> > > >& x2);
	void _write_x3(size_t p, size_t s, const std::vector<std::vector<std::vector<GRBVar> > >& x3);
	void _write_x4(size_t p, size_t s, const std::vector<std::vector<std::vector<GRBVar> > >& x4);
	void _write_y2(size_t p, size_t s, const std::vector<std::vector<std::vector<GRBVar> > >& y2);
	void _write_y3(size_t p, size_t s, const std::vector<std::vector<GRBVar> >& y3);
	void _write_y4(size_t p, size_t s, const std::vector<std::vector<GRBVar> >& y4);

	const Demands& _demands;
	Trips&         _trips;

};