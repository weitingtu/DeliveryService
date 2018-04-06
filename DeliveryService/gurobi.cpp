#define _USE_MATH_DEFINES //產生隨機需求
#include "gurobi.h"
#include <iostream>
#include <fstream>
#include <cmath> 
#include <string>
#include <gurobi_c++.h>

Gurobi::Gurobi() :
	_c1(),
	_c2(),
	_b1(),
	_b2(),
	_a1(),
	_c3({ { 27500, 51500 } }),
	_b4({ { 11000, 20600 } }),
	_c4({ { 21500, 21500 } }),
	_b6({ { 8600, 8600 } }),
	_a2({ { 8000, 15000 } }),
	_a3({ { 6200, 6200 } }),
	_load({ { 14, 20 } }),
	_u1(),
	_u2({ { 2, 4 } }),
	_u3({ {1.11} }),
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

void Gurobi::start()
{
	// add gurobi to solve lp here
}

bool Gurobi::_read_var_cost(const std::string& input_cost_varcost)
{
	std::ifstream ifile(input_cost_varcost);   // self truck (c1&c2)
	if (ifile.fail())
	{
		printf("Unable to open %s\n", input_cost_varcost.c_str());
		return false;
	}
	for (int i = 0; i < DISTRICT; ++i) {
		for (int j = 0; j < TASK; ++j) {
			ifile >> _c1[i][j];
			_c2[i][j] = _c1[i][j] * COST_TEMPORARY_17; //2.5
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
	for (int i = 0; i < DISTRICT; ++i) {
		for (int j = 0; j < TASK; ++j) {
			ifile >> _b1[i][j];
			_b2[i][j] = _b1[i][j] * COST_TEMPORARY_OUTSOURCING17; //2
			_a1[i][j] = _b1[i][j] * COST_OUTSOURCING_CONTAINER; //1.45
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
	for (int i = 0; i < DISTRICT; ++i) {
		for (int j = 0; j < TASK; ++j) {
			ifile >> _u1[i][j];
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
	for (int i = 0; i < TASK; ++i) {
		if (i == 0) {
			for (int j = 0; j < DISTRICT; ++j) {
				for (int k = 0; k < DAY; ++k) {
					ifile1 >> _d1[0][k][j][i];//第一組需求		stjk
				//std::cout << k << " " <<j<< " "<<i<<" " << _d1[0][k][j][i] << std::endl;
				}
			}

		}
		else if (i == 1) {
			for (int j = 0; j < DISTRICT; ++j) {
				for (int k = 0; k < DAY; ++k) {
					ifile2 >> _d1[0][k][j][i];//第一組需求	
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
	for (int i = 0; i < STATION; ++i) {
		for (int j = 0; j < DAY; ++j) {
			ifile >> _d2[0][j][i];//第一組需求
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
	for (int i = 0; i < TASK; ++i) {
		for (int j = 0; j < DAY; ++j) {
			ifile >> _d3[0][j][i];//第一組需求
		//std::cout << j << " " << i << " " << _d3[0][j][i] << std::endl;

		}

	}

	ifile.close();

	return true;
}

void Gurobi::_generate_stochastic_parameter() //sum, mean, std
{    //district task
	int temp_var = 0;
	for (int i = 0; i < TASK; ++i) {
		for (int j = 0; j < DISTRICT; ++j) {
			_sum_task_demand[j][i] = 0;
			_mean_task_demand[j][i] = 0;
			_std_task_demand[j][i] = 0;
			temp_var = 0;

			for (int k = 0; k < DAY; ++k) {
				_sum_task_demand[j][i] = _sum_task_demand[j][i] + _d1[0][k][j][i];
				_mean_task_demand[j][i] = (_sum_task_demand[j][i] / DAY);
			}
			for (int k = 0; k < DAY; ++k) {
				temp_var = temp_var + pow((_d1[0][k][j][i] - _mean_task_demand[j][i]), 2);
			}

			_std_task_demand[j][i] = pow((temp_var / DAY), 0.5);
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
		temp_var = 0;

		for (int j = 0; j < DAY; ++j) {
			_sum_transfer_demand[i] = _sum_transfer_demand[i] + _d2[0][j][i];
			_mean_transfer_demand[i] = (_sum_transfer_demand[i] / DAY);
		}
		for (int k = 0; k < DAY; ++k) {
			temp_var = temp_var + pow((_d2[0][k][i] - _mean_transfer_demand[i]), 2);
		}
		_std_transfer_demand[i] = pow((temp_var / DAY), 0.5);
		//std::cout << i << " " << _sum_transfer_demand[i] << std::endl;
		//std::cout << _sum_transfer_demand[i] << std::endl;
		//std::cout << i << " " << _mean_transfer_demand[i] << std::endl;
		//std::cout << i << " " << _std_transfer_demand[i] << std::endl;
	}
	//major customer
	for (int i = 0; i < TASK; ++i) {
		_sum_transfer_majorcustomer[i] = 0;
		_mean_transfer_majorcustomer[i] = 0;
		_std_transfer_majorcustomer[i] = 0;
		temp_var = 0;
		for (int j = 0; j < DAY; ++j) {
			_sum_transfer_majorcustomer[i] = _sum_transfer_majorcustomer[i] + _d3[0][j][i];
			_mean_transfer_majorcustomer[i] = (_sum_transfer_majorcustomer[i] / DAY);
		}
		for (int k = 0; k < DAY; ++k) {
			temp_var = temp_var + pow((_d3[0][k][i] - _mean_transfer_majorcustomer[i]), 2);
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
	for (int i = 1; i < SCENARIO; ++i) { //第二組需求開始
		for (int j = 0; j < DAY; ++j) {
			for (int k = 0; k < DISTRICT; ++k) {
				for (int l = 0; l < TASK; ++l) {
					r1 = rand() / (double)RAND_MAX;
					r2 = rand() / (double)RAND_MAX;
					_d1[i][j][k][l] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) * _std_task_demand[k][l] + _mean_task_demand[k][l];
					//std::cout <<"scenario" <<i << " " << j << " " << k << " " << l << " " << _d1[i][j][k][l] << std::endl;
				}
			}
		}
	}

	for (int i = 1; i < SCENARIO; ++i) {//第二組需求開始
		for (int j = 0; j < DAY; ++j) {
			for (int k = 0; k < TASK; ++k) {
				r1 = rand() / (double)RAND_MAX;
				r2 = rand() / (double)RAND_MAX;
				_d3[i][j][k] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) *_std_transfer_majorcustomer[k] + _mean_transfer_majorcustomer[k];
				//std::cout <<"scenario" <<i << " " << j << " " << k  << " " << _d3[i][j][k] << std::endl;
			}
		}
	}

	for (int i = 1; i < SCENARIO; ++i) {
		for (int j = 0; j < DAY; ++j) {
			for (int k = 0; k < STATION; ++k) {
				r1 = rand() / (double)RAND_MAX;
				r2 = rand() / (double)RAND_MAX;
				_d2[i][j][k] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) *_std_transfer_demand[k] + _mean_transfer_demand[k];
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
	for (int i = 0; i < TASK; ++i) {
		for (int j = 0; j < FLEET; ++j) {
			for (int k = 0; k < DISTRICT; ++k) {
				for (int l = 0; l < DAY; ++l) {
					for (int m = 0; m < SCENARIO; ++m) {
						ifile >> _num_x1[m][l][k][j][i];
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
	for (int i = 0; i < TASK; ++i) {
		for (int j = 0; j < DISTRICT; ++j) {
			for (int k = 0; k < DAY; ++k) {
				for (int l = 0; l < SCENARIO; ++l) {
					ifile >> _num_y1[l][k][j][i];
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
	for (int i = 0; i < TASK; ++i) {
		for (int j = 0; j < DISTRICT; ++j) {
			for (int k = 0; k < DAY; ++k) {
				for (int l = 0; l < SCENARIO; ++l) {
					ifile >> _num_v1[l][k][j][i];
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
	for (int i = 0; i < CAR_TYPE; ++i) {
		for (int j = 0; j < STATION; ++j) {
			for (int k = 0; k < DAY; ++k) {
				for (int l = 0; l < SCENARIO; ++l) {
					ifile >> _num_v2[l][k][j][i];
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
	for (int i = 0; i < TASK; ++i) {
		for (int j = 0; j < DAY; ++j) {
			for (int k = 0; k < SCENARIO; ++k) {
				ifile >> _num_v3[k][j][i];
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
