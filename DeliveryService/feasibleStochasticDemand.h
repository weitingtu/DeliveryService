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
	FeasibleStochasticDemand(const Demands& d, const Trips& t);
	~FeasibleStochasticDemand();

	void start();
private:
	int _get_p1(size_t p) const;
	int _get_p2(size_t p) const;
	int _get_p3(size_t p) const;

	const double _p1; // �ۦ����r�p���{����~�ҥ��ȥX�������v�C
	const double _p2; // �ۦ����r�p���{����B���ȥX�������v�C
	const double _p3; // �ۦ����r�p���{�ɤj�Ȥ���ȥX�������v�C
	const double _pf; // �v�t�~�̹��{�ɽ릸���F�쪺�i����v�C

	const Demands _demand;
	Trips _trips;

};

