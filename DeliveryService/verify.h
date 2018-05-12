#pragma once
#include "defines.h"
#include "demands.h"
#include "trips.h"
#include <array>
#include <vector>
#include <string>
#include <gurobi_c++.h>

class Verify
{
public:
	Verify(const Demands& d, const Trips& t);
	~Verify();

	bool verify_momthly(size_t p) const;
	bool verify_daily(size_t p, size_t s) const;
	bool verify_x2(size_t p, size_t t, size_t s, size_t i, size_t j, size_t k) const;
	bool verify_x3(size_t p, size_t t, size_t s, size_t i, size_t m) const;
	bool verify_x4(size_t p, size_t t, size_t s, size_t i, size_t k) const;

private:
	double _cost_1(size_t p, size_t t) const;
	bool _verify_v1_v2(size_t p, size_t t, size_t k) const;
	bool _verify_v3_v2(size_t p, size_t t, size_t k) const;
	bool _verify_x1_y1_v1_v2_v3(size_t p, size_t t) const;

	double _cost_2(size_t p, size_t t, size_t s) const;
	bool _verify_x1_y1_x2_y2(size_t p, size_t t, size_t j, size_t k, size_t s) const;
	bool _verify_x3_y3(size_t p, size_t t, size_t m, size_t s) const;
	bool _verify_x4_y4(size_t p, size_t t, size_t k, size_t s) const;
	bool _verify_x1_x2_x4(size_t p, size_t t, size_t i, size_t k, size_t s) const;
	bool _verify_x3(size_t p, size_t t, size_t i, size_t s) const;
	bool _verify_x2_x3_x4_y2_y3_y4(size_t p, size_t t, size_t s) const;

	const Demands& _demands;
	const Trips&   _trips;

};

