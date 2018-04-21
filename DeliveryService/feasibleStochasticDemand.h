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
	double p1; // �ۦ����r�p���{����~�ҥ��ȥX�������v�C
	double p2; // �ۦ����r�p���{����B���ȥX�������v�C
	double p3; // �ۦ����r�p���{�ɤj�Ȥ���ȥX�������v�C
	double pf; // �v�t�~�̹��{�ɽ릸���F�쪺�i����v�C

	const Demand _demand;
	Trips _trips;

};

