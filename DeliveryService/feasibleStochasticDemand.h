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
	void _start(size_t p, size_t s);
	int _update_X1(int X1, int A1, size_t p, size_t s, size_t& t, size_t& i, size_t& j, size_t& k);
	int _update_X2(int X2, int A2, size_t p, size_t s, size_t& t, size_t& i, size_t& m);
	int _update_X3(int X3, int A3, size_t p, size_t s, size_t& t, size_t& i, size_t& k);
	int _get_a(int N, double p) const;
	int _get_sum_x2(size_t p, size_t s) const;
	int _get_sum_y2(size_t p, size_t s) const;
	int _get_sum_x3(size_t p, size_t s) const;
	int _get_sum_y3(size_t p, size_t s) const;
	int _get_sum_x4(size_t p, size_t s) const;
	int _get_sum_y4(size_t p, size_t s) const;
	double _get_p1(int X1, int N1) const;
	double _get_p2(int X2, int N2) const;
	double _get_p3(int X3, int N3) const;
	//double _get_p1(size_t p, size_t s) const;
	//double _get_p2(size_t p, size_t s) const;
	//double _get_p3(size_t p, size_t s) const;

	const double _p1; // �ۦ����r�p���{����~�ҥ��ȥX�������v�C
	const double _p2; // �ۦ����r�p���{����B���ȥX�������v�C
	const double _p3; // �ۦ����r�p���{�ɤj�Ȥ���ȥX�������v�C
	const double _pf; // �v�t�~�̹��{�ɽ릸���F�쪺�i����v�C

	const Demands& _demand;
	Trips _trips;

};

