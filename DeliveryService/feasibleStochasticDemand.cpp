#include "feasibleStochasticDemand.h"
#include "defines.h"
#include "verify.h"
#include <vector>
#include <utility>
#include <algorithm>

FeasibleStochasticDemand::FeasibleStochasticDemand(const Demands & d, const Trips & t) :
	_p1(0.8),
	_p2(0.1),
	_p3(0.7),
	_pf(0.5),
	_demand(d),
	_trips(t),
	_min_max_c1(),
	_max_min_c1()
{
}

FeasibleStochasticDemand::~FeasibleStochasticDemand()
{
}

static double _nChoosek(double n, double k)
{
	if (k > n) return 0;
	if (k * 2 > n) k = n - k;
	if (k == 0) return 1;

	double result = n;
	for (int i = 2; i <= k; ++i)
	{
		result *= (n - i + 1);
		result /= i;
	}
	return result;
}

void FeasibleStochasticDemand::_initialize_cost_array()
{
	for (size_t k = 0; k < TASK; ++k)
	{
		std::vector<std::pair<double, size_t> > c;
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			c.push_back(std::make_pair(_demand.c1()[j][k], j));
		}
		std::sort(c.begin(), c.end());
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			_min_max_c1[j][k] = c[j].second;
		}
		std::reverse(c.begin(), c.end());
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			_max_min_c1[j][k] = c[j].second;
		}
	}
}

void FeasibleStochasticDemand::start()
{
	_initialize_cost_array();
	//for (size_t p = 0; p < POPULATION; ++p)
	//for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		size_t p = 0;
		size_t s = 1;
		printf("Start feasible stochastic demand %zu %zu\n", p, s);
		_start(p, s);
	}
}

void FeasibleStochasticDemand::_start(size_t p, size_t s)
{
	if (p == s)
	{
		return;
	}
	int X1 = _get_sum_x2(p, s);
	int N1 = _get_sum_y2(p, s) + X1;
	int X2 = _get_sum_x3(p, s);
	int N2 = _get_sum_y3(p, s) + X2;
	int X3 = _get_sum_x4(p, s);
	int N3 = _get_sum_y4(p, s) + X3;
	printf("%d %d %d %d %d %d\n", X1, N1, X2, N2, X3, N3);

	double p1 = _get_p1(X1, N1);
	double p2 = _get_p2(X2, N2);
	double p3 = _get_p3(X3, N3);
	int A1 = _get_a(N1, _p1);
	int A2 = _get_a(N2, _p2);
	int A3 = _get_a(N3, _p3);
	printf("%d %d %d\n", A1, A2, A3);

	size_t x2_t = 0;
	size_t x2_i = 0;
	size_t x2_j = 0;
	size_t x2_k = 0;

	size_t x3_t = 0;
	size_t x3_i = 0;
	size_t x3_m = 0;

	size_t x4_t = 0;
	size_t x4_i = 0;
	size_t x4_k = 0;

	size_t count = 0;
	while (p1 * p2 * p3 < _pf)
	{
		printf("%zu: %g * %g * %g = %g < %g\n", count, p1, p2, p3, p1*p2*p3, _pf);
		++count;
		if ((x2_t < DAY) && (X1 != A1))
		{
			printf("X1 %d -> ", X1);
			X1 = _update_X1(X1, A1, p, s, x2_t, x2_i, x2_j, x2_k);
			printf("%d\n", X1);
			p1 = _get_p1(X1, N1);
		}
		else if ((x3_t < DAY) && (X2 != A2))
		{
			printf("X2 %d -> ", X2);
			X2 = _update_X2(X2, A2, p, s, x3_t, x3_i, x3_m);
			printf("%d\n", X2);
			p2 = _get_p2(X2, N2);
		}
		else if ((x4_t < DAY) && (X3 != A3))
		{
			printf("X3 %d -> ", X3);
			X3 = _update_X3(X3, A3, p, s, x4_t, x4_i, x4_k);
			printf("%d\n", X3);
			p3 = _get_p3(X3, N3);
		}
		else
		{
			printf("Unable to match possibility\n");
			break;
		}
	}
	printf("final %g * %g * %g = %g < %g\n", p1, p2, p3, p1*p2*p3, _pf);

}

int FeasibleStochasticDemand::_update_X1(int X1, int A1, size_t p, size_t s, size_t& t, size_t& i, size_t& j, size_t& k)
{
	if (X1 == A1)
	{
		return X1;
	}
	for (; t < DAY; ++t)
	{
		for (; i < FLEET; ++i)
		{
			for (; j < DISTRICT; ++j)
			{
				for (; k < TASK; ++k)
				{
					bool add = X1 < A1;
					size_t jj = add ? _min_max_c1.at(j).at(k) : _max_min_c1.at(j).at(k);
					if (add)
					{
						++_trips.trips()[p][t].x2()[s][i][jj][k];
						--_trips.trips()[p][t].y2()[s][jj][k];
						++X1;
					}
					else
					{
						--_trips.trips()[p][t].x2()[s][i][jj][k];
						++_trips.trips()[p][t].y2()[s][jj][k];
						--X1;
					}
					Verify v(_demand, _trips);
					if (v.verify_x2(p, t, s, i, jj, k))
					{
						return X1;
					}
					if (add)
					{
						--_trips.trips()[p][t].x2()[s][i][jj][k];
						++_trips.trips()[p][t].y2()[s][jj][k];
						--X1;
					}
					else
					{
						++_trips.trips()[p][t].x2()[s][i][jj][k];
						--_trips.trips()[p][t].y2()[s][jj][k];
						++X1;
					}
				}
				k = 0;
			}
			j = 0;
		}
		i = 0;
	}
	return X1;
}

int FeasibleStochasticDemand::_update_X2(int X2, int A2, size_t p, size_t s, size_t& t, size_t& i, size_t& m)
{
	if (X2 == A2)
	{
		return X2;
	}
	for (; t < DAY; ++t)
	{
		for (; i < FLEET; ++i)
		{
			for (; m < STATION; ++m)
			{
				bool add = X2 < A2;
				if (add)
				{
					++_trips.trips()[p][t].x3()[s][i][m];
					--_trips.trips()[p][t].y3()[s][m];
					++X2;
				}
				else
				{
					--_trips.trips()[p][t].x3()[s][i][m];
					++_trips.trips()[p][t].y3()[s][m];
					--X2;
				}
				Verify v(_demand, _trips);
				if (v.verify_x3(p, t, s, i, m))
				{
					return X2;
				}
				if (add)
				{
					--_trips.trips()[p][t].x3()[s][i][m];
					++_trips.trips()[p][t].y3()[s][m];
					--X2;
				}
				else
				{
					++_trips.trips()[p][t].x3()[s][i][m];
					--_trips.trips()[p][t].y3()[s][m];
					++X2;
				}
			}
			m = 0;
		}
		i = 0;
	}
	return X2;
}

int FeasibleStochasticDemand::_update_X3(int X3, int A3, size_t p, size_t s, size_t& t, size_t& i, size_t& k)
{
	if (X3 == A3)
	{
		return X3;
	}
	for (; t < DAY; ++t)
	{
		for (; i < FLEET; ++i)
		{
			for (; k < STATION; ++k)
			{
				bool add = X3 < A3;
				if (add)
				{
					++_trips.trips()[p][t].x4()[s][i][k];
					--_trips.trips()[p][t].y4()[s][k];
					++X3;
				}
				else
				{
					--_trips.trips()[p][t].x4()[s][i][k];
					++_trips.trips()[p][t].y4()[s][k];
					--X3;
				}
				Verify v(_demand, _trips);
				if (v.verify_x4(p, t, s, i, k))
				{
					return X3;
				}
				if (add)
				{
					--_trips.trips()[p][t].x4()[s][i][k];
					++_trips.trips()[p][t].y4()[s][k];
					--X3;
				}
				else
				{
					++_trips.trips()[p][t].x4()[s][i][k];
					--_trips.trips()[p][t].y4()[s][k];
					++X3;
				}
			}
			k = 0;
		}
		i = 0;
	}
	return X3;
}

int FeasibleStochasticDemand::_get_a(int N, double p) const
{
	// (88)
	return static_cast<int>((N + 1) * p);
}

int FeasibleStochasticDemand::_get_sum_x2(size_t p, size_t s) const
{
	int sum_x2 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					sum_x2 += _trips.trips()[p][t].x2()[s][i][j][k];
				}
			}
		}
	}
	return sum_x2;
}

int FeasibleStochasticDemand::_get_sum_y2(size_t p, size_t s) const
{
	int sum_y2 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				sum_y2 += _trips.trips()[p][t].y2()[s][j][k];
			}
		}
	}
	return sum_y2;
}

int FeasibleStochasticDemand::_get_sum_x3(size_t p, size_t s) const
{
	int sum_x3 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				sum_x3 += _trips.trips()[p][t].x3()[s][i][m];
			}
		}
	}
	return sum_x3;
}

int FeasibleStochasticDemand::_get_sum_y3(size_t p, size_t s) const
{
	int sum_y3 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			sum_y3 += _trips.trips()[p][t].y3()[s][m];
		}
	}
	return sum_y3;
}

int FeasibleStochasticDemand::_get_sum_x4(size_t p, size_t s) const
{
	int sum_x4 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				sum_x4 += _trips.trips()[p][t].x4()[s][i][k];
			}
		}
	}
	return sum_x4;
}

int FeasibleStochasticDemand::_get_sum_y4(size_t p, size_t s) const
{
	int sum_y4 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			sum_y4 += _trips.trips()[p][t].y4()[s][k];
		}
	}
	return sum_y4;
}

double FeasibleStochasticDemand::_get_p1(int X1, int N1) const
{
	return (_nChoosek(N1, X1)) * std::pow(_p1, X1) * std::pow(1 - _p1, N1 - X1);
}

double FeasibleStochasticDemand::_get_p2(int X2, int N2) const
{
	return (_nChoosek(N2, X2)) * std::pow(_p2, X2) * std::pow(1 - _p2, N2 - X2);
}

double FeasibleStochasticDemand::_get_p3(int X3, int N3) const
{
	return (_nChoosek(N3, X3)) * std::pow(_p3, X3) * std::pow(1 - _p3, N3 - X3);
}

//double FeasibleStochasticDemand::_get_p1(size_t p, size_t s) const
//{
//	int x1 = 0;
//	for (size_t t = 0; t < DAY; ++t)
//	{
//		for (size_t i = 0; i < FLEET; ++i)
//		{
//			for (size_t j = 0; j < DISTRICT; ++j)
//			{
//				for (size_t k = 0; k < TASK; ++k)
//				{
//					x1 += _trips.trips()[p][t].x2()[s][i][j][k];
//				}
//			}
//		}
//	}
//
//	int n1 = 0;
//	for (size_t t = 0; t < DAY; ++t)
//	{
//		for (size_t j = 0; j < DISTRICT; ++j)
//		{
//			for (size_t k = 0; k < TASK; ++k)
//			{
//				n1 += _trips.trips()[p][t].y2()[s][j][k];
//			}
//		}
//	}
//
//	n1 += x1;
//
//	return ((double)_nChoosek(n1, x1)) * std::pow(_p1, x1) * std::pow(1 - _p1, n1 - x1);
//}

//double FeasibleStochasticDemand::_get_p2(size_t p, size_t s) const
//{
//	int x2 = 0;
//	for (size_t t = 0; t < DAY; ++t)
//	{
//		for (size_t i = 0; i < FLEET; ++i)
//		{
//			for (size_t m = 0; m < STATION; ++m)
//			{
//				x2 += _trips.trips()[p][t].x3()[s][i][m];
//			}
//		}
//	}
//
//	int n2 = 0;
//	for (size_t t = 0; t < DAY; ++t)
//	{
//		for (size_t m = 0; m < STATION; ++m)
//		{
//			n2 += _trips.trips()[p][t].y3()[s][m];
//		}
//	}
//
//	n2 += x2;
//
//	return ((double)_nChoosek(n2, x2)) * std::pow(_p2, x2) * std::pow(1 - _p2, n2 - x2);
//}

//double FeasibleStochasticDemand::_get_p3(size_t p, size_t s) const
//{
//	int x3 = 0;
//	for (size_t t = 0; t < DAY; ++t)
//	{
//		for (size_t i = 0; i < FLEET; ++i)
//		{
//			for (size_t k = 0; k < TASK; ++k)
//			{
//				x3 += _trips.trips()[p][t].x4()[s][i][k];
//			}
//		}
//	}
//
//	int n3 = 0;
//	for (size_t t = 0; t < DAY; ++t)
//	{
//		for (size_t k = 0; k < TASK; ++k)
//		{
//			n3 += _trips.trips()[p][t].y4()[s][k];
//		}
//	}
//
//	n3 += x3;
//
//	return ((double)_nChoosek(n3, x3)) * std::pow(_p3, x3) * std::pow(1 - _p3, n3 - x3);
//}

