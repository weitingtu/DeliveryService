#define _USE_MATH_DEFINES //產生隨機需求
#include "gurobi.h"
#include <iostream>
#include <fstream>
#include <cmath> 
#include <string>
#include <gurobi_c++.h>
#include <vector>

Gurobi::Gurobi() :
	_c1(),
	_c2(),
	_b1(),
	_b2(),
	_a1(),
	_c3({ { 27500, 51500 } }),
	_b3({ { 11000, 20600 } }),
	_c4({ { 21500, 21500 } }),
	_b4({ { 8600, 8600 } }),
	_a2({ { 8000, 15000 } }),
	_a3({ { 6200, 6200 } }),
	_load({ { 14, 20 } }),
	_u1(),
	_u2({ { 2, 4 } }),
	_u3({ {1.11, 1.11} }),
	_d1(),
	_d2(),
	_d3(),
	_sum_task_demand(),
	_mean_task_demand(),
	_std_task_demand(),
	_sum_transfer_majorcustomer(),
	_mean_transfer_majorcustomer(),
	_std_transfer_majorcustomer(),
	_sum_transfer_demand(),
	_mean_transfer_demand(),
	_std_transfer_demand(),
	_pro({ { 1.0 / SCENARIO } }),
	_num_x1(),
	_num_y1(),
	_num_v1(),
	_num_v2(),
	_num_v3()
{
}


Gurobi::~Gurobi()
{
}

void Gurobi::read()
{
	if (!_read_var_cost("input_cost_varcost.txt"))
	{
		return;
	}
	if (!_read_outsourcing_cost("input_cost_outsourcing17.txt"))
	{
		return;
	}
	if (!_read_traveltime("input_traveltime.txt"))
	{
		return;
	}
	if (!_read_d1("input_demand_task_collection.txt", "input_demand_task_sending.txt"))
	{
		return;
	}
	if (!_read_d2("input_demand_transfer.txt"))
	{
		return;
	}
	if (!_read_d3("input_demand_transfer_majorcoutomer.txt"))
	{
		return;
	}
	if (!_read_num_x1("x1.txt"))
	{
		return;
	}
	if (!_read_num_y1("y1.txt"))
	{
		return;
	}
	if (!_read_num_v1("v1.txt"))
	{
		return;
	}
	if (!_read_num_v2("v2.txt"))
	{
		return;
	}
	if (!_read_num_v3("v3.txt"))
	{
		return;
	}
	_generate_stochastic_parameter();
	_generate_stochastic_demand();
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

void Gurobi::start()
{
	// add gurobi to solve lp here
	try {

		size_t scenerio = 0;

		GRBEnv env = GRBEnv();

		GRBModel model = GRBModel(env);

		// Create variables

		std::vector<std::vector<std::vector<std::vector<GRBVar> > > > x2(DAY);
		std::vector<std::vector<std::vector<GRBVar> > > x3(DAY);
		std::vector<std::vector<std::vector<GRBVar> > > x4(DAY);
		std::vector<std::vector<std::vector<GRBVar> > > y2(DAY);
		std::vector<std::vector<GRBVar> > y3(DAY);
		std::vector<std::vector<GRBVar> > y4(DAY);

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
						double c2 = _c2[j][k];
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
					double c3 = _c3[m];
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
					double c4 = _c4[k];
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
					double b2 = _b2[j][k];
					obj += b2 * y2[t][j][k];
				}
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				double b3 = _b3[m];
				obj += b3 * y3[t][m];
			}
		}

		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				double b4 = _b4[k];
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
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					constr.clear();
					int sum_x1 = 0;
					for (size_t i = 0; i < FLEET; ++i)
					{
						sum_x1 += _num_x1[scenerio][t][j][i][k];
					}
					int y1 = _num_y1[scenerio][t][j][k];
					int v1 = _num_v1[scenerio][t][j][k];

					constr += sum_x1;
					constr += y1;

					for (size_t i = 0; i < FLEET; ++i)
					{
						constr += x2[t][i][j][k];
					}
					constr += y2[t][j][k];
					constr *= _load[0];
					constr += _load[1] * v1;

					double d1 = _d1[scenerio][t][j][k];
					model.addConstr(constr >= d1, "c" + std::to_string(constr_count++));
				}
			}
		}

		// (17)
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				constr.clear();

				int sum_v2 = 0;
				for (size_t n = 0; n < CAR_TYPE; ++n)
				{
					sum_v2 += _num_v2[scenerio][t][m][n];
				}
				for (size_t i = 0; i < FLEET; ++i)
				{
					constr += x3[t][i][m];
				}
				constr += y3[t][m];
				constr *= _load[0];
				constr += _load[1] * sum_v2;

				double d2 = _d2[scenerio][t][m];
				model.addConstr(constr >= d2, "c" + std::to_string(constr_count++));
			}
		}

		// (18)
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				constr.clear();

				int v3 = _num_v3[scenerio][t][k];
				for (size_t i = 0; i < FLEET; ++i)
				{
					constr += x4[t][i][k];
				}
				constr += y4[t][k];
				constr *= _load[0];
				constr += _load[1] * v3;

				double d3 = _d3[scenerio][t][k];
				model.addConstr(constr >= d3, "c" + std::to_string(constr_count++));
			}
		}

		// (19)
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					constr.clear();
					double sum_u1_x1 = 0.0;
					for (size_t j = 0; j < DISTRICT; ++j)
					{
						double u1 = _u1[j][k];
						int x1 = _num_x1[scenerio][t][j][i][k];
						constr += u1 * x1;
						constr += u1 * x2[t][i][j][k];

						sum_u1_x1 += u1 * x1;
					}

					double u3 = _u3[k];

					double worktime = MAXWORKTIME;
					constr += u3 * x4[t][i][k];
					if (sum_u1_x1 >= MAXWORKTIME)
					{
						worktime = sum_u1_x1;
					}
					model.addConstr(constr <= worktime, "c" + std::to_string(constr_count++));
				}
			}
		}

		// (20)
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				constr.clear();
				for (size_t m = 0; m < STATION; ++m)
				{
					double u2 = _u2[m];
					constr += u2 * x3[t][i][m];
				}
				// ??? t''
				model.addConstr(constr <= MAXWORKTIME, "c" + std::to_string(constr_count++));
			}
		}

		// Optimize model

		model.optimize();
		model.write("out.lp");
		printf("finish optimization\n");

		std::cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;

		if (!_write_x2("x2.txt", x2))
		{
			return;
		}
		if (!_write_x3("x3.txt", x3))
		{
			return;
		}
		if (!_write_x4("x4.txt", x4))
		{
			return;
		}
		if (!_write_y2("y2.txt", y2))
		{
			return;
		}
		if (!_write_y3("y3.txt", y3))
		{
			return;
		}
		if (!_write_y4("y4.txt", y4))
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

bool Gurobi::_write_x2(const std::string& file_name, const std::vector<std::vector<std::vector<std::vector<GRBVar> > > >& x2) const
{
	std::ofstream ofile(file_name);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					ofile << x2[t][i][j][k].get(GRB_DoubleAttr_X) << std::endl;
				}
			}
		}
	}
	ofile.close();
	return true;
}

bool Gurobi::_write_x3(const std::string& file_name, const std::vector<std::vector<std::vector<GRBVar> > >& x3) const
{
	std::ofstream ofile(file_name);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				ofile << x3[t][i][m].get(GRB_DoubleAttr_X) << std::endl;
			}
		}
	}
	ofile.close();
	return true;
}

bool Gurobi::_write_x4(const std::string& file_name, const std::vector<std::vector<std::vector<GRBVar> > >& x4) const
{
	std::ofstream ofile(file_name);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				ofile << x4[t][i][k].get(GRB_DoubleAttr_X) << std::endl;
			}
		}
	}
	ofile.close();
	return true;
}

bool Gurobi::_write_y2(const std::string& file_name, const std::vector<std::vector<std::vector<GRBVar> > >& y2) const
{
	std::ofstream ofile(file_name);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				ofile << y2[t][j][k].get(GRB_DoubleAttr_X) << std::endl;
			}
		}
	}
	ofile.close();
	return true;
}

bool Gurobi::_write_y3(const std::string& file_name, const std::vector<std::vector<GRBVar> >& y3) const
{
	std::ofstream ofile(file_name);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			ofile << y3[t][m].get(GRB_DoubleAttr_X) << std::endl;
		}
	}
	ofile.close();
	return true;
}

bool Gurobi::_write_y4(const std::string& file_name, const std::vector<std::vector<GRBVar> >& y4) const
{
	std::ofstream ofile(file_name);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			ofile << y4[t][k].get(GRB_DoubleAttr_X) << std::endl;
		}
	}
	ofile.close();
	return true;
}

bool Gurobi::_read_var_cost(const std::string& input_cost_varcost)
{
	std::ifstream ifile(input_cost_varcost);   // self truck (c1&c2)
	if (ifile.fail())
	{
		printf("Unable to open %s\n", input_cost_varcost.c_str());
		return false;
	}
	for (int j = 0; j < DISTRICT; ++j)
	{
		for (int k = 0; k < TASK; ++k)
		{
			ifile >> _c1[j][k];
			_c2[j][k] = _c1[j][k] * COST_TEMPORARY_17; //2.5
			//std::cout << i << " " << j << " " << _c1[i][j] << std::endl;
			//std::cout << i << " " << j << " " << _c2[i][j] << std::endl;
		}
	}
	ifile.close();

	return true;
}

bool Gurobi::_read_outsourcing_cost(const std::string& input_cost_outsourcing17)
{
	std::ifstream ifile(input_cost_outsourcing17); //outsoucing truck and container (b1&b2&a1)
	if (ifile.fail())
	{
		printf("Unable to open %s\n", input_cost_outsourcing17.c_str());
		return false;
	}
	for (int j = 0; j < DISTRICT; ++j)
	{
		for (int k = 0; k < TASK; ++k)
		{
			ifile >> _b1[j][k];
			_b2[j][k] = _b1[j][k] * COST_TEMPORARY_OUTSOURCING17; //2
			_a1[j][k] = _b1[j][k] * COST_OUTSOURCING_CONTAINER; //1.45
			//std::cout << i << " " << j << " " << _b1[i][j] << std::endl;
			//std::cout << i << " " << j << " " << _b2[i][j] << std::endl;
			//std::cout << i << " " << j << " " << _a1[i][j] << std::endl;
		}
	}
	ifile.close();

	return true;
}

bool Gurobi::_read_traveltime(const std::string& input_traveltime)
{
	std::ifstream ifile(input_traveltime);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", input_traveltime.c_str());
		return false;
	}
	for (int j = 0; j < DISTRICT; ++j)
	{
		for (int k = 0; k < TASK; ++k)
		{
			ifile >> _u1[j][k];
			//std::cout << i << " " << j << " " << _u1[i][j] << std::endl;

		}
	}
	ifile.close();

	return true;
}

bool Gurobi::_read_d1(const std::string& input_demand_task_collection, const std::string& input_demand_task_sending)
{
	std::ifstream ifile1(input_demand_task_collection);
	std::ifstream ifile2(input_demand_task_sending);
	if (ifile1.fail())
	{
		printf("Unable to open%s\n", input_demand_task_collection.c_str());
		return false;
	}
	if (ifile2.fail())
	{
		printf("Unable to open%s\n", input_demand_task_sending.c_str());
		return false;
	}
	for (int k = 0; k < TASK; ++k)
	{
		if (k == 0)
		{
			for (int j = 0; j < DISTRICT; ++j)
			{
				for (int t = 0; t < DAY; ++t)
				{
					ifile1 >> _d1[0][t][j][k];//第一組需求		stjk
				//std::cout << k << " " <<j<< " "<<i<<" " << _d1[0][k][j][i] << std::endl;
				}
			}
		}
		else if (k == 1)
		{
			for (int j = 0; j < DISTRICT; ++j)
			{
				for (int t = 0; t < DAY; ++t)
				{
					ifile2 >> _d1[0][t][j][k];//第一組需求	
			  //std::cout << k << " " <<j<< " "<<i<<" " << _d1[0][k][j][i] << std::endl;
				}
			}
		}
	}

	ifile1.close();
	ifile2.close();

	return true;
}

bool Gurobi::_read_d2(const std::string& input_demand_transfer)
{
	std::ifstream ifile(input_demand_transfer);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", input_demand_transfer.c_str());
		return false;
	}
	for (int m = 0; m < STATION; ++m)
	{
		for (int t = 0; t < DAY; ++t)
		{
			ifile >> _d2[0][t][m];//第一組需求
			//std::cout << j << " " << i << " " << _d2[0][j][i] << std::endl;
		}
	}

	ifile.close();

	return true;
}

bool Gurobi::_read_d3(const std::string& input_demand_transfer_majorcoutomer)
{
	std::ifstream ifile(input_demand_transfer_majorcoutomer);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", input_demand_transfer_majorcoutomer.c_str());
		return false;
	}
	for (int k = 0; k < TASK; ++k)
	{
		for (int t = 0; t < DAY; ++t)
		{
			ifile >> _d3[0][t][k];//第一組需求
		//std::cout << j << " " << i << " " << _d3[0][j][i] << std::endl;

		}

	}

	ifile.close();

	return true;
}

void Gurobi::_generate_stochastic_parameter() //sum, mean, std
{    //district task
	for (int k = 0; k < TASK; ++k)
	{
		for (int j = 0; j < DISTRICT; ++j)
		{
			_sum_task_demand[j][k] = 0;
			_mean_task_demand[j][k] = 0;
			_std_task_demand[j][k] = 0;
			int temp_var = 0;

			for (int t = 0; t < DAY; ++t)
			{
				_sum_task_demand[j][k] += _d1[0][t][j][k];
			}

			_mean_task_demand[j][k] = (_sum_task_demand[j][k] / DAY);

			for (int t = 0; t < DAY; ++t)
			{
				temp_var += pow((_d1[0][t][j][k] - _mean_task_demand[j][k]), 2);
			}

			_std_task_demand[j][k] = pow((temp_var / DAY), 0.5);
			//std::cout << j << " " << i << " " << _sum_task_demand[j][i] << std::endl;
			//std::cout << _sum_task_demand[j][i] << std::endl;
			//std::cout << j << " " << i << " " << _mean_task_demand[j][i] << std::endl;
			//std::cout << j << " " << i << " " << _std_task_demand[j][i] << std::endl;

		}
	}

	//transshipment task
	for (int i = 0; i < STATION; ++i) {
		_sum_transfer_demand[i] = 0;
		_mean_transfer_demand[i] = 0;
		_std_transfer_demand[i] = 0;
		int temp_var = 0;

		for (int j = 0; j < DAY; ++j)
		{
			_sum_transfer_demand[i] += _d2[0][j][i];
		}

		_mean_transfer_demand[i] = (_sum_transfer_demand[i] / DAY);

		for (int k = 0; k < DAY; ++k)
		{
			temp_var += pow((_d2[0][k][i] - _mean_transfer_demand[i]), 2);
		}
		_std_transfer_demand[i] = pow((temp_var / DAY), 0.5);
		//std::cout << i << " " << _sum_transfer_demand[i] << std::endl;
		//std::cout << _sum_transfer_demand[i] << std::endl;
		//std::cout << i << " " << _mean_transfer_demand[i] << std::endl;
		//std::cout << i << " " << _std_transfer_demand[i] << std::endl;
	}

	//major customer
	for (int i = 0; i < TASK; ++i)
	{
		_sum_transfer_majorcustomer[i] = 0;
		_mean_transfer_majorcustomer[i] = 0;
		_std_transfer_majorcustomer[i] = 0;
		int temp_var = 0;

		for (int j = 0; j < DAY; ++j)
		{
			_sum_transfer_majorcustomer[i] += _d3[0][j][i];
		}
		_mean_transfer_majorcustomer[i] = (_sum_transfer_majorcustomer[i] / DAY);

		for (int k = 0; k < DAY; ++k)
		{
			temp_var += pow((_d3[0][k][i] - _mean_transfer_majorcustomer[i]), 2);
		}
		_std_transfer_majorcustomer[i] = pow((temp_var / DAY), 0.5);
		//std::cout << i << " " << _sum_transfer_majorcustomer[i] << std::endl;
		//std::cout << _sum_transfer_majorcustomer[i] << std::endl;
		//std::cout << i << " " << _mean_transfer_majorcustomer[i] << std::endl;
		//std::cout << i << " " << _std_transfer_majorcustomer[i] << std::endl;
	}
}

void Gurobi::_generate_stochastic_demand()
{
	double r1, r2;
	for (int s = 1; s < SCENARIO; ++s)
	{ //第二組需求開始
		for (int t = 0; t < DAY; ++t)
		{
			for (int j = 0; j < DISTRICT; ++j)
			{
				for (int k = 0; k < TASK; ++k)
				{
					r1 = rand() / (double)RAND_MAX;
					r2 = rand() / (double)RAND_MAX;
					_d1[s][t][j][k] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) * _std_task_demand[j][k] + _mean_task_demand[j][k];
					//std::cout <<"scenario" <<i << " " << j << " " << k << " " << l << " " << _d1[i][j][k][l] << std::endl;
				}
			}
		}
	}

	for (int s = 1; s < SCENARIO; ++s)
	{//第二組需求開始
		for (int t = 0; t < DAY; ++t)
		{
			for (int k = 0; k < TASK; ++k)
			{
				r1 = rand() / (double)RAND_MAX;
				r2 = rand() / (double)RAND_MAX;
				_d3[s][t][k] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) *_std_transfer_majorcustomer[k] + _mean_transfer_majorcustomer[k];
				//std::cout <<"scenario" <<i << " " << j << " " << k  << " " << _d3[i][j][k] << std::endl;
			}
		}
	}

	for (int s = 1; s < SCENARIO; ++s)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int m = 0; m < STATION; ++m)
			{
				r1 = rand() / (double)RAND_MAX;
				r2 = rand() / (double)RAND_MAX;
				_d2[s][t][m] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) *_std_transfer_demand[m] + _mean_transfer_demand[m];
				//std::cout <<"scenario" <<i << " " << j << " " << k  << " " << _d2[i][j][k] << std::endl;
			}
		}
	}

}

bool Gurobi::_read_num_x1(const std::string& x1)
{
	std::ifstream ifile(x1);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", x1.c_str());
		return false;
	}
	for (int k = 0; k < TASK; ++k)
	{
		for (int i = 0; i < FLEET; ++i)
		{
			for (int j = 0; j < DISTRICT; ++j)
			{
				for (int t = 0; t < DAY; ++t)
				{
					for (int s = 0; s < SCENARIO; ++s)
					{
						ifile >> _num_x1[s][t][j][i][k];
						//std::cout << m << " " << l << " " << k << " " << j << " " << i << " " << _num_x1[m][l][k][j][i] << std::endl;
					}
				}
			}
		}
	}
	ifile.close();

	return true;
}

bool Gurobi::_read_num_y1(const std::string& y1)
{
	std::ifstream ifile(y1);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", y1.c_str());
		return false;
	}
	for (int k = 0; k < TASK; ++k)
	{
		for (int j = 0; j < DISTRICT; ++j)
		{
			for (int t = 0; t < DAY; ++t)
			{
				for (int s = 0; s < SCENARIO; ++s)
				{
					ifile >> _num_y1[s][t][j][k];
					//std::cout <<  l << " " << k << " " << j << " " << i << " " << _num_y1[l][k][j][i] << std::endl;
				}
			}
		}
	}
	ifile.close();

	return true;
}

bool Gurobi::_read_num_v1(const std::string& v1)
{
	std::ifstream ifile(v1);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", v1.c_str());
		return false;
	}
	for (int k = 0; k < TASK; ++k)
	{
		for (int j = 0; j < DISTRICT; ++j)
		{
			for (int t = 0; t < DAY; ++t)
			{
				for (int s = 0; s < SCENARIO; ++s)
				{
					ifile >> _num_v1[s][t][j][k];
					//std::cout <<  l << " " << k << " " << j << " " << i << " " << _num_v1[l][k][j][i] << std::endl;
				}
			}
		}
	}
	ifile.close();

	return true;
}

bool Gurobi::_read_num_v2(const std::string& v2)
{
	std::ifstream ifile(v2);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", v2.c_str());
		return false;
	}
	for (int n = 0; n < CAR_TYPE; ++n)
	{
		for (int m = 0; m < STATION; ++m)
		{
			for (int t = 0; t < DAY; ++t)
			{
				for (int s = 0; s < SCENARIO; ++s)
				{
					ifile >> _num_v2[s][t][m][n];
					//std::cout <<  l << " " << k << " " << j << " " << i << " " << _num_v2[l][k][j][i] << std::endl;
				}
			}
		}
	}
	ifile.close();

	return true;
}

bool Gurobi::_read_num_v3(const std::string& v3)
{
	std::ifstream ifile(v3);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", v3.c_str());
		return false;
	}
	for (int k = 0; k < TASK; ++k)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int s = 0; s < SCENARIO; ++s)
			{
				ifile >> _num_v3[s][t][k];
				//std::cout <<k << " " << j << " " << i << " " << _num_v3[k][j][i] << std::endl;
			}
		}
	}
	ifile.close();
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
