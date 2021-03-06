#pragma once
#include "defines.h"
#include "demands.h"
#include "trips.h"
#include <array>
#include <vector>
#include <string>

class FeasibleStochasticDemand
{
public:
	FeasibleStochasticDemand(const Demands& d);
	~FeasibleStochasticDemand();

	bool start(FILE* fp, size_t count, size_t p, std::vector<Trip>& trips);
	void compute(FILE* fp, size_t count, size_t p, std::vector<Trip>& trips);
private:
	int _get_sum_x2(size_t s) const;
	int _get_sum_y2(size_t s) const;
	int _get_sum_x3(size_t s) const;
	int _get_sum_y3(size_t s) const;
	int _get_sum_x4(size_t s) const;
	int _get_sum_y4(size_t s) const;
	void _initialize_cost_array();

	bool _start(size_t count, size_t p, size_t s, std::vector<std::string>& results) const;
	double _get_p(int X, int N, double p) const;
	void _compute(size_t count, size_t p, size_t s, std::vector<std::string>& results) const;

	const double _p1; // 自有車駕駛對臨時營業所任務出車之機率。
	const double _p2; // 自有車駕駛對臨時轉運任務出車之機率。
	const double _p3; // 自有車駕駛對臨時大客戶任務出車之機率。
	const double _pf; // 宅配業者對臨時趟次欲達到的可行機率。

	const Demands& _demand;
	std::vector<Trip>* _trips;
	std::array<std::array<size_t, TASK>, DISTRICT> _min_max_c1;
	std::array<std::array<size_t, TASK>, DISTRICT> _max_min_c1;
};

