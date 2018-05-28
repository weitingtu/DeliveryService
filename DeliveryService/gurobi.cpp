#define _USE_MATH_DEFINES //產生隨機需求
#include "gurobi.h"
#include "verify.h"
#include <iostream>
#include <fstream>
#include <cmath> 
#include <string>
#include <gurobi_c++.h>
#include <vector>

Gurobi::Gurobi(const Demands& d, Trips& t) :
	_demands(d),
	_trips(t)
{
}

Gurobi::~Gurobi()
{
}

void Gurobi::monthly_trips()
{
	for (size_t p = 0; p < POPULATION; ++p)
	{
		printf("Run population %zu\n", p);
		_run_monthly_trips(p);
		Verify v(_demands, _trips);
		v.verify_momthly(p);
	}
}

void Gurobi::_run_monthly_trips(size_t p)
{
	// add gurobi to solve lp here
	try {
		GRBEnv env = GRBEnv();

		GRBModel model = GRBModel(env);

		// Create variables
		std::vector<std::vector<std::vector<std::vector<GRBVar>>>> x1(DAY); //tijk
		std::vector<std::vector<std::vector<GRBVar>>> y1(DAY); //tjk
		std::vector<std::vector<std::vector<GRBVar>>> v1(DAY); //tjk
		std::vector<std::vector<std::vector<GRBVar>>> v2(DAY); //tnm
		std::vector<std::vector<GRBVar>> v3(DAY); //tk

		for (size_t t = 0; t < DAY; ++t)
		{
			x1[t].resize(FLEET);
			for (size_t i = 0; i < FLEET; ++i)
			{
				x1[t][i].resize(DISTRICT);
				for (size_t j = 0; j < DISTRICT; ++j)
				{
					x1[t][i][j].resize(TASK);
					for (size_t k = 0; k < TASK; ++k)
					{
						x1[t][i][j][k] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, _var_name("x1", { t, i, j, k }));
					}
				}
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			y1[t].resize(DISTRICT);
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				y1[t][j].resize(TASK);
				for (size_t k = 0; k < TASK; ++k)
				{
					y1[t][j][k] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, _var_name("y1", { t, j, k }));
				}
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			v1[t].resize(DISTRICT);
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				v1[t][j].resize(TASK);
				for (size_t k = 0; k < TASK; ++k)
				{
					v1[t][j][k] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, _var_name("v1", { t, j, k }));
				}
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			v2[t].resize(CAR_TYPE);
			for (size_t n = 0; n < CAR_TYPE; ++n)
			{
				v2[t][n].resize(STATION);
				for (size_t m = 0; m < STATION; ++m)
				{
					v2[t][n][m] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, _var_name("v2", { t, n, m }));
				}
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			v3[t].resize(TASK);
			for (size_t k = 0; k < TASK; ++k)
			{
				v3[t][k] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, _var_name("v3", { t, k }));
			}
		}

		model.update();
		// Set objective: 
		model.set(GRB_IntAttr_ModelSense, 1);
		GRBLinExpr obj = 0.0;
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					for (size_t j = 0; j < DISTRICT; ++j)
					{
						double c1 = _demands.c1()[j][k];
						obj += c1 * x1[t][i][j][k];
					}
				}
			}
		}
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					double b1 = _demands.b1()[j][k];
					obj += b1 * y1[t][j][k];
				}
			}
		}
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					double a1 = _demands.a1()[j][k];
					obj += a1 * v1[t][j][k];
				}
			}
		}
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t n = 0; n < CAR_TYPE; ++n)
			{
				for (size_t m = 0; m < STATION; ++m)
				{
					double a2 = _demands.a2()[m];
					obj += a2 * v2[t][n][m];
				}
			}
		}
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				double a3 = _demands.a3()[k];
				obj += a3 * v3[t][k];
			}
		}
		model.setObjective(obj, GRB_MINIMIZE);
		// Add constraint: 
		int constr_count = 0;
		GRBLinExpr constr = 0.0;

		// (8)
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				constr.clear();
				for (size_t j = 0; j < DISTRICT; ++j)
				{
					constr += v1[t][j][k];
				}

				// Nk: N1 = {1, 3}, N2 = {1, 2}
				for (size_t n = 0; n < CAR_TYPE; ++n)
				{
					if (0 == k && 1 == n)
					{
						continue;
					}
					else if (1 == k && 2 == n)
					{
						continue;
					}
					for (size_t m = 0; m < STATION; ++m)
					{
						constr -= v2[t][n][m];
					}
				}
				model.addConstr(constr >= 0, "c" + std::to_string(constr_count++));
			}
		}

		// (9)
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				constr.clear();
				constr += v3[t][k];

				// Nk: N1 = {2}, N2 = {3}
				size_t n = k == 0 ? 1 : 2;
				//for (size_t n = 0; n < CAR_TYPE; ++n)
				{
					for (size_t m = 0; m < STATION; ++m)
					{
						constr -= v2[t][n][m];
					}
				}
				model.addConstr(constr >= 0, "c" + std::to_string(constr_count++));
			}
		}

		// (16) without daily trips
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					constr.clear();

					for (size_t i = 0; i < FLEET; ++i)
					{
						constr += x1[t][i][j][k];
					}
					constr += y1[t][j][k];
					constr *= _demands.load()[0];
					constr += _demands.load()[1] * v1[t][j][k];

					double d1 = _demands.demands()[p][t].d1()[j][k];
					model.addConstr(constr >= d1, "c" + std::to_string(constr_count++));
				}
			}
		}
		// (17) without daily trips
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				constr.clear();
				for (size_t n = 0; n < CAR_TYPE; ++n)
				{
					constr += v2[t][n][m];
				}


				constr *= _demands.load()[1];

				double d2 = _demands.demands()[p][t].d2()[m];
				model.addConstr(constr >= d2, "c" + std::to_string(constr_count++));
			}
		}
		// (18) without daily trips
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				constr.clear();
				constr += v3[t][k];
				constr *= _demands.load()[1];

				double d3 = _demands.demands()[p][t].d3()[k];
				model.addConstr(constr >= d3, "c" + std::to_string(constr_count++));
			}
		}
		// (19) without daily trips
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					constr.clear();
					for (size_t j = 0; j < DISTRICT; ++j)
					{
						double u1 = _demands.u1()[j][k];

						constr += ((u1 * 2) + WORKTIME)*x1[t][i][j][k];
					}
					model.addConstr(constr <= MAXWORKTIME, "c" + std::to_string(constr_count++));
				}
			}
		}
		// Optimize model

		model.getEnv().set(GRB_DoubleParam_MIPGap, 0.03);
		model.optimize();
		model.write("out.lp");
		printf("finish optimization\n");

		std::cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;

		// save result in _x1[population], _y1[population], _v1[population] _v2[population] _v3[population]

		if (!_write_x1( x1, p))
		{
			return;
		}
		if (!_write_y1( y1, p))
		{
			return;
		}
		if (!_write_v1( v1, p))
		{
			return;
		}
		if (!_write_v2( v2, p))
		{
			return;
		}
		if (!_write_v3( v3, p))
		{
			return;
		}

	}
	catch (GRBException e) {
		std::cout << "Error code = " << e.getErrorCode() << std::endl;
		std::cout << e.getMessage() << std::endl;
	}
	catch (...) {
		std::cout << "Exception during optimization" << std::endl;
	}
}

bool Gurobi::_write_x1(const std::vector<std::vector<std::vector<std::vector<GRBVar> > > >& x1, size_t p)
{
	for (size_t t = 0; t < DAY; ++t)
	{
		Trip& trip = _trips.trips()[p][t];
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					trip.x1()[i][j][k] = (int)x1[t][i][j][k].get(GRB_DoubleAttr_X);
				}
			}
		}
	}
	return true;
}

bool Gurobi::_write_y1( const std::vector<std::vector<std::vector<GRBVar> > >& y1, size_t p)
{
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				_trips.trips()[p][t].y1()[j][k] = (int)y1[t][j][k].get(GRB_DoubleAttr_X);
			}
		}
	}
	return true;
}

bool Gurobi::_write_v1(const std::vector<std::vector<std::vector<GRBVar> > >& v1, size_t p)
{
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				_trips.trips()[p][t].v1()[j][k] = (int)v1[t][j][k].get(GRB_DoubleAttr_X);
			}
		}
	}
	return true;
}

bool Gurobi::_write_v2(const std::vector<std::vector<std::vector<GRBVar> >>& v2, size_t p)
{
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t n = 0; n < CAR_TYPE; ++n)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				_trips.trips()[p][t].v2()[n][m] = (int)v2[t][n][m].get(GRB_DoubleAttr_X);
			}
		}
	}
	return true;
}

bool Gurobi::_write_v3(const std::vector<std::vector<GRBVar> >& v3, size_t p)
{
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			_trips.trips()[p][t].v3()[k] = (int)v3[t][k].get(GRB_DoubleAttr_X);
		}
	}
	return true;
}

void Gurobi::daily_trips()
{
	for (size_t p = 0; p < POPULATION; ++p)
	{
		for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
		{
			printf("Run population %zu stochastic demand %zu\n", p, s);
			_run_daily_trips(p, s);
			Verify v(_demands, _trips);
			v.verify_daily(p, s);
		}
	}
}

void Gurobi::_run_daily_trips(size_t p, size_t s)
{
	// add gurobi to solve lp here
	try {

		GRBEnv env = GRBEnv();

		GRBModel model = GRBModel(env);

		// Create variables

		std::vector<std::vector<std::vector<std::vector<GRBVar> > > > x2(DAY); //tijk
		std::vector<std::vector<std::vector<GRBVar> > > x3(DAY); //tim
		std::vector<std::vector<std::vector<GRBVar> > > x4(DAY); //tik
		std::vector<std::vector<std::vector<GRBVar> > > y2(DAY); //tjk
		std::vector<std::vector<GRBVar> > y3(DAY); //tm
		std::vector<std::vector<GRBVar> > y4(DAY); //tk

		for (size_t t = 0; t < DAY; ++t)
		{
			x2[t].resize(FLEET);
			for (size_t i = 0; i < FLEET; ++i)
			{
				x2[t][i].resize(DISTRICT);
				for (size_t j = 0; j < DISTRICT; ++j)
				{
					x2[t][i][j].resize(TASK);
					for (size_t k = 0; k < TASK; ++k)
					{
						x2[t][i][j][k] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, _var_name("x2", { t, i, j, k }));
					}
				}
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			x3[t].resize(FLEET);
			for (size_t i = 0; i < FLEET; ++i)
			{
				x3[t][i].resize(STATION);
				for (size_t m = 0; m < STATION; ++m)
				{
					x3[t][i][m] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, _var_name("x3", { t, i, m }));
				}
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			x4[t].resize(FLEET);
			for (size_t i = 0; i < FLEET; ++i)
			{
				x4[t][i].resize(TASK);
				for (size_t k = 0; k < TASK; ++k)
				{
					x4[t][i][k] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, _var_name("x4", { t, i, k }));
				}
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			y2[t].resize(DISTRICT);
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				y2[t][j].resize(TASK);
				for (size_t k = 0; k < TASK; ++k)
				{
					y2[t][j][k] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, _var_name("y2", { t, j, k }));
				}
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			y3[t].resize(STATION);
			for (size_t m = 0; m < STATION; ++m)
			{
				y3[t][m] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, _var_name("y3", { t, m }));
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			y4[t].resize(TASK);
			for (size_t k = 0; k < TASK; ++k)
			{
				y4[t][k] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, _var_name("y4", { t, k }));
			}
		}

		model.update();

		// Set objective: 
		model.set(GRB_IntAttr_ModelSense, 1);

		// (15)
		GRBLinExpr obj = 0.0;
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t j = 0; j < DISTRICT; ++j)
				{
					for (size_t k = 0; k < TASK; ++k)
					{
						double c2 = _demands.c2()[j][k];
						obj += c2 * x2[t][i][j][k];
					}
				}
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t m = 0; m < STATION; ++m)
				{
					double c3 = _demands.c3()[m];
					obj += c3 * x3[t][i][m];
				}
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					double c4 = _demands.c4()[k];
					obj += c4 * x4[t][i][k];
				}
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					double b2 = _demands.b2()[j][k];
					obj += b2 * y2[t][j][k];
				}
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				double b3 = _demands.b3()[m];
				obj += b3 * y3[t][m];
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				double b4 = _demands.b4()[k];
				obj += b4 * y4[t][k];
			}
		}

		model.setObjective(obj, GRB_MINIMIZE);

		// Add constraint: 

		int constr_count = 0;
		GRBLinExpr constr = 0.0;

		// (16)
		for (size_t t = 0; t < DAY; ++t)
		{
			const Trip& trip = _trips.trips()[p][t];
			const Demand& demand = _demands.demands()[s][t];

			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					constr.clear();
					int sum_x1 = 0;
					for (size_t i = 0; i < FLEET; ++i)
					{
						sum_x1 += trip.x1()[i][j][k];
					}
					int y1 = trip.y1()[j][k];
					int v1 = trip.v1()[j][k];

					constr += sum_x1;
					constr += y1;

					for (size_t i = 0; i < FLEET; ++i)
					{
						constr += x2[t][i][j][k];
					}
					constr += y2[t][j][k];
					constr *= _demands.load()[0];
					constr += _demands.load()[1] * v1;

					double d1 = demand.d1()[j][k];
					model.addConstr(constr >= d1, "c" + std::to_string(constr_count++));
				}
			}
		}

		// (17)
		for (size_t t = 0; t < DAY; ++t)
		{
			const Trip& trip = _trips.trips()[p][t];
			const Demand& demand = _demands.demands()[s][t];

			for (size_t m = 0; m < STATION; ++m)
			{
				constr.clear();

				int sum_v2 = 0;
				for (size_t n = 0; n < CAR_TYPE; ++n)
				{
					sum_v2 += trip.v2()[n][m];
				}
				for (size_t i = 0; i < FLEET; ++i)
				{
					constr += x3[t][i][m];
				}
				constr += y3[t][m];
				constr *= _demands.load()[0];
				constr += _demands.load()[1] * sum_v2;

				double d2 = demand.d2()[m];
				model.addConstr(constr >= d2, "c" + std::to_string(constr_count++));
			}
		}

		// (18)
		for (size_t t = 0; t < DAY; ++t)
		{
			const Trip& trip = _trips.trips()[p][t];
			const Demand& demand = _demands.demands()[s][t];

			for (size_t k = 0; k < TASK; ++k)
			{
				constr.clear();

				int v3 = trip.v3()[k];
				for (size_t i = 0; i < FLEET; ++i)
				{
					constr += x4[t][i][k];
				}
				constr += y4[t][k];
				constr *= _demands.load()[0];
				constr += _demands.load()[1] * v3;

				double d3 = demand.d3()[k];
				model.addConstr(constr >= d3, "c" + std::to_string(constr_count++));
			}
		}

		// (19)
		for (size_t t = 0; t < DAY; ++t)
		{
			const Trip& trip = _trips.trips()[p][t];
			const Demand& demand = _demands.demands()[s][t];

			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					constr.clear();
					double sum_u1_x1 = 0.0;
					for (size_t j = 0; j < DISTRICT; ++j)
					{
						double u1 = _demands.u1()[j][k];
						u1 *= 2;
						u1 += 0.5;
						int x1 = trip.x1()[i][j][k];
						constr += u1 * x1;
						constr += u1 * x2[t][i][j][k];

						sum_u1_x1 += u1 * x1;
					}

					double u3 = _demands.u3()[k];
					u3 *= 2;
					u3 += 0.5;

					constr += u3 * x4[t][i][k];
					model.addConstr(constr <= MAXWORKTIME, "c" + std::to_string(constr_count++));
				}
			}
		}

		// (20)
		for (size_t t = 0; t < DAY; ++t)
		{
			const Trip& trip = _trips.trips()[p][t];
			const Demand& demand = _demands.demands()[s][t];

			for (size_t i = 0; i < FLEET; ++i)
			{
				constr.clear();
				for (size_t m = 0; m < STATION; ++m)
				{
					double u2 = _demands.u2()[m];
					u2 *= 2;
					u2 += 0.5;
					constr += u2 * x3[t][i][m];
				}
				// ??? t''
				model.addConstr(constr <= MAXWORKTIME, "c" + std::to_string(constr_count++));
			}
		}

		// Optimize model

		model.getEnv().set(GRB_DoubleParam_MIPGap, 0.03);
		model.optimize();
		model.write("out.lp");
		printf("finish optimization\n");

		std::cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;

		_write_x2(p, s, x2);
		_write_x3(p, s, x3);
		_write_x4(p, s, x4);
		_write_y2(p, s, y2);
		_write_y3(p, s, y3);
		_write_y4(p, s, y4);
	}
	catch (GRBException e) {
		std::cout << "Error code = " << e.getErrorCode() << std::endl;
		std::cout << e.getMessage() << std::endl;
	}
	catch (...) {
		std::cout << "Exception during optimization" << std::endl;
	}
}

void Gurobi::_write_x2(size_t p, size_t s, const std::vector<std::vector<std::vector<std::vector<GRBVar> > > >& x2)
{
	for (size_t t = 0; t < DAY; ++t)
	{
		Trip& trip = _trips.trips()[p][t];
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					trip.x2()[s][i][j][k] = (int)x2[t][i][j][k].get(GRB_DoubleAttr_X);
				}
			}
		}
	}
}

void Gurobi::_write_x3(size_t p, size_t s, const std::vector<std::vector<std::vector<GRBVar> > >& x3)
{
	for (size_t t = 0; t < DAY; ++t)
	{
		Trip& trip = _trips.trips()[p][t];
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				trip.x3()[s][i][m] = (int)x3[t][i][m].get(GRB_DoubleAttr_X);
			}
		}
	}
}

void Gurobi::_write_x4(size_t p, size_t s, const std::vector<std::vector<std::vector<GRBVar> > >& x4)
{
	for (size_t t = 0; t < DAY; ++t)
	{
		Trip& trip = _trips.trips()[p][t];
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				trip.x4()[s][i][k] = (int)x4[t][i][k].get(GRB_DoubleAttr_X);
			}
		}
	}
}

void Gurobi::_write_y2(size_t p, size_t s, const std::vector<std::vector<std::vector<GRBVar> > >& y2)
{
	for (size_t t = 0; t < DAY; ++t)
	{
		Trip& trip = _trips.trips()[p][t];
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				trip.y2()[s][j][k] = (int)y2[t][j][k].get(GRB_DoubleAttr_X);
			}
		}
	}
}

void Gurobi::_write_y3(size_t p, size_t s, const std::vector<std::vector<GRBVar> >& y3)
{
	for (size_t t = 0; t < DAY; ++t)
	{
		Trip& trip = _trips.trips()[p][t];
		for (size_t m = 0; m < STATION; ++m)
		{
			trip.y3()[s][m] = (int)y3[t][m].get(GRB_DoubleAttr_X);
		}
	}
}

void Gurobi::_write_y4(size_t p, size_t s, const std::vector<std::vector<GRBVar> >& y4)
{
	for (size_t t = 0; t < DAY; ++t)
	{
		Trip& trip = _trips.trips()[p][t];
		for (size_t k = 0; k < TASK; ++k)
		{
			trip.y4()[s][k] = (int)y4[t][k].get(GRB_DoubleAttr_X);
		}
	}
}

std::string Gurobi::_var_name(std::string str, const std::vector<size_t>& idx) const
{
	for (size_t i : idx)
	{
		str += "_";
		str += std::to_string(i);
	}
	return str;
}

bool Gurobi::_is_file_exist(const std::string& file_name) const
{
	struct stat buffer;
	return (stat(file_name.c_str(), &buffer) == 0);
}

bool Gurobi::_delete_file(const std::string& file_name) const
{
	if (!_is_file_exist(file_name))
	{
		return true;
	}
	if (remove(file_name.c_str()) != 0)
	{
		printf("Error deleting file %s\n", file_name.c_str());
		return false;
	}
	return true;
}

bool Gurobi::_delete_files() const
{
	if (!_delete_file("x1.txt"))
	{
		return false;
	}
	if (!_delete_file("y1.txt"))
	{
		return false;
	}
	if (!_delete_file("v1.txt"))
	{
		return false;
	}
	if (!_delete_file("v2.txt"))
	{
		return false;
	}
	if (!_delete_file("v3.txt"))
	{
		return false;
	}
	if (!_delete_file("x2.txt"))
	{
		return false;
	}
	if (!_delete_file("x3.txt"))
	{
		return false;
	}
	if (!_delete_file("x4.txt"))
	{
		return false;
	}
	if (!_delete_file("y2.txt"))
	{
		return false;
	}
	if (!_delete_file("y3.txt"))
	{
		return false;
	}
	if (!_delete_file("y4.txt"))
	{
		return false;
	}
	return true;
}

void Gurobi::test() const
{
	try {
		GRBEnv env = GRBEnv();

		GRBModel model = GRBModel(env);

		// Create variables

		GRBVar x = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x");
		GRBVar y = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y");
		GRBVar z = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "z");

		// Set objective: maximize x + y + 2 z

		model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);

		// Add constraint: x + 2 y + 3 z <= 4

		model.addConstr(x + 2 * y + 3 * z <= 4, "c0");

		// Add constraint: x + y >= 1

		model.addConstr(x + y >= 1, "c1");

		// Optimize model

		model.optimize();

		std::cout << x.get(GRB_StringAttr_VarName) << " "
			<< x.get(GRB_DoubleAttr_X) << std::endl;
		std::cout << y.get(GRB_StringAttr_VarName) << " "
			<< y.get(GRB_DoubleAttr_X) << std::endl;
		std::cout << z.get(GRB_StringAttr_VarName) << " "
			<< z.get(GRB_DoubleAttr_X) << std::endl;

		std::cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;

	}
	catch (GRBException e) {
		std::cout << "Error code = " << e.getErrorCode() << std::endl;
		std::cout << e.getMessage() << std::endl;
	}
	catch (...) {
		std::cout << "Exception during optimization" << std::endl;
	}
}
