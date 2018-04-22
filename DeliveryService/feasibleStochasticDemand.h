#pragma once
#include "defines.h"
#include "demand.h"
#include "trips.h"
#include <array>
#include <vector>
#include <string>

class FeasibleStochasticDemand
{
public:
	FeasibleStochasticDemand(const Demand& d, const Trips& t);
	~FeasibleStochasticDemand();

	void start();
private:
	int _get_p1(size_t p) const;
	int _get_p2(size_t p) const;
	int _get_p3(size_t p) const;

	double p1; // 自有車駕駛對臨時營業所任務出車之機率。
	double p2; // 自有車駕駛對臨時轉運任務出車之機率。
	double p3; // 自有車駕駛對臨時大客戶任務出車之機率。
	double pf; // 宅配業者對臨時趟次欲達到的可行機率。

	const Demand _demand;
	Trips _trips;

};

