#define _USE_MATH_DEFINES //產生隨機需求
#include "gurobi.h"
#include <iostream>
#include <fstream>
#include <cmath> 
#include <string>

Gurobi::Gurobi():
	_c1(),
	_c2(),
	_b1(),
	_b2(),
	_a1(),
	_c3({27500, 51500}),
	_b4({11000, 20600}),
	_c4({21500, 21500}),
	_b6({8600, 8600}),
	_a2({8000, 15000}),
	_a3({6200, 6200}),
	_load({14, 20}),
	_u1(),
	_u2({2, 4}),
	_u3({1.11}),
	_d1(),
	_d2(),
	_d3(),
	_sum_task_demand(),
	_mean_task_demand(),
	_std_task_demand(),
	_sum_transfer_majorcustomer(),
	_mean_transfer_majorcustomer(),
	_std_tranfer_majorcustomer(),
	_sum_transfer_demand(),
	_mean_transfer_demand(),
	_std_transfer_demand(),
	_pro({ 1.0 / SCENARIO }),
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
	_read_traveltime();
	_read_d1();
	_read_d2();
	_read_d3();
	_read_num_x1();
	_read_num_y1();
	_read_num_v1();
	_read_num_v2();
	_read_num_v3();
	_generate_stochastic_parameter();
	_generate_stochastic_demand();
}

bool Gurobi::_read_var_cost(const std::string& file_name)
{
	std::ifstream ifile(file_name);   // self truck (c1&c2)
	if (ifile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (int i = 0; i < DISTRICT; ++i) {
		for (int j = 0; j < TASK; ++j) {
			ifile >> _c1[i][j];
			_c2[i][j] = _c1[i][j] * COST_TEMPORARY_17; //2.5
		}
	}
	ifile.close();
	return true;
}

bool Gurobi::_read_outsourcing_cost(const std::string& file_name)
{
	std::ifstream ifile(file_name); //outsoucing truck and container (b1&b2&a1)
	if (ifile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (int i = 0; i < DISTRICT; ++i) {
		for (int j = 0; j < TASK; ++j) {
			ifile >> _b1[i][j];
			_b2[i][j] = _b1[i][j] * COST_TEMPORARY_OUTSOURCING17; //2
			_a1[i][j] = _b1[i][j] * COST_OUTSOURCING_CONTAINER; //1.45
		}
	}
	ifile.close();
	return true;
}

void Gurobi::_read_traveltime()
{
	std::ifstream ifile("input_traveltime.txt"); //district task
	for (int i = 0; i < DISTRICT; ++i) {
		for (int j = 0; j < TASK; ++j) {
			ifile >> _u1[i][j];
		}
	}
	ifile.close();
}

void Gurobi::_read_d1()
{
	//d1(district task)
	std::ifstream ifile1("input_demand_task_collection.txt");
	std::ifstream ifile2("input_demand_task_sending.txt");
	for (int i = 0; i < TASK; ++i) {
		if (i == 0) {
			for (int j = 0; j < DAY; ++j) {
				for (int k = 0; k < DISTRICT; ++k) {
					ifile1 >> _d1[0][j][k][i];//第一組需求		stjk

				}
			}

		}
		else if (i == 1) {
			for (int i = 0; i < 1; ++i) {//第一組
				for (int j = 0; j < DAY; ++j) {
					for (int k = 0; k < DISTRICT; ++k) {
						ifile2 >> _d1[0][j][k][i];//第一組需求					
					}
				}
			}
		}

	}

	ifile1.close();
	ifile2.close();
}

void Gurobi::_read_d2()
{
	//d2(transshipment task)
	std::ifstream ifile("input_demand_transfer.txt");
	for (int i = 0; i < DAY; ++i) {
		for (int j = 0; j < STATION; ++j) {
			ifile >> _d2[0][i][j];//第一組需求
		}
	}

	ifile.close();
}

void Gurobi::_read_d3()
{//d3(major customer task)
	std::ifstream ifile("input_demand_transfer_majorcoutomer.txt");
	for (int i = 0; i < DAY; ++i) {
		for (int j = 0; j < TASK; ++j) {
			ifile >> _d3[0][i][j];//第一組需求
		}

	}

	ifile.close();
}

void Gurobi::_generate_stochastic_parameter() //sum, mean, std
{    //district task
	int temp_var = 0;
	for (int i = 0; i < DAY; ++i) {
		for (int j = 0; j < DISTRICT; ++j) {
			for (int k = 0; k < TASK; ++k) {
				_sum_task_demand[j][k] = 0;
				_mean_task_demand[j][k] = 0;
				_std_task_demand[j][k] = 0;
				_sum_task_demand[j][k] = _sum_task_demand[j][k] + _d1[0][i][j][k];
				_mean_task_demand[j][k] = (_sum_task_demand[j][k] / DAY);
				temp_var = temp_var + pow((_d1[0][i][j][k] - _mean_task_demand[j][k]), 2);
				_std_task_demand[j][k] = pow((temp_var / DAY), 0.5);
			}
		}
	}
	//transshipment task
	for (int i = 0; i < DAY; ++i) {
		for (int j = 0; j < STATION; ++j) {
			_sum_transfer_demand[j] = 0;
			_mean_transfer_demand[j] = 0;
			_std_transfer_demand[j] = 0;
			_sum_transfer_demand[j] = _sum_transfer_demand[j] + _d2[0][i][j];
			_mean_transfer_demand[j] = (_sum_transfer_demand[j] / DAY);
			temp_var = 0;
			temp_var = temp_var + pow((_d2[0][i][j] - _mean_transfer_demand[j]), 2);
			_std_transfer_demand[j] = pow((temp_var / DAY), 0.5);

		}
	}
	//major customer
	for (int i = 0; i < DAY; ++i) {
		for (int j = 0; j < TASK; ++j) {
			_sum_transfer_majorcustomer[j] = 0;
			_mean_transfer_majorcustomer[j] = 0;
			_std_tranfer_majorcustomer[j] = 0;
			_sum_transfer_majorcustomer[j] = _sum_transfer_majorcustomer[j] + _d3[0][i][j];
			_mean_transfer_majorcustomer[j] = (_sum_transfer_majorcustomer[j] / DAY);
			temp_var = 0;
			temp_var = temp_var + pow((_d3[0][i][j] - _mean_transfer_majorcustomer[j]), 2);
			_std_tranfer_majorcustomer[j] = pow((temp_var / DAY), 0.5);

		}
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
					_d1[1][j][k][l] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) * _std_task_demand[j][k] + _mean_task_demand[j][k];

				}
			}
		}
	}

	for (int i = 1; i < SCENARIO; ++i) {//第二組需求開始
		for (int j = 0; j < DAY; ++j) {
			for (int k = 0; k < TASK; ++k) {
				r1 = rand() / (double)RAND_MAX;
				r2 = rand() / (double)RAND_MAX;
				_d3[1][j][k] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) *_std_tranfer_majorcustomer[j] + _mean_transfer_majorcustomer[j];
			}
		}
	}

	for (int i = 1; i < SCENARIO; ++i) {
		for (int j = 0; j < DAY; ++j) {
			for (int k = 0; k < STATION; ++k) {
				r1 = rand() / (double)RAND_MAX;
				r2 = rand() / (double)RAND_MAX;
				_d2[1][j][k] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) *_std_transfer_demand[j] + _mean_transfer_demand[j];
			}
		}
	}
}

void Gurobi::_read_num_x1()
{   // 50 sets of monthly trips
	std::ifstream ifile("x1.txt");
	for (int i = 0; i < SCENARIO; ++i) {
		for (int j = 0; j < DAY; ++j) {
			for (int k = 0; k < DISTRICT; ++k) {
				for (int l = 0; l < FLEET; ++l) {
					for (int m = 0; m < TASK; ++k) {
						ifile >> _num_x1[i][j][k][l][m];
					}
				}
			}
		}
	}
	ifile.close();
}

void Gurobi::_read_num_y1()
{
	std::ifstream ifile("y1.txt");
	for (int i = 0; i < SCENARIO; ++i) {
		for (int j = 0; j < DAY; ++j) {
			for (int k = 0; k < DISTRICT; ++k) {
				for (int l = 0; l < TASK; ++l) {
					ifile >> _num_y1[i][j][k][l];
				}
			}
		}
	}
	ifile.close();
}

void Gurobi::_read_num_v1()
{
	std::ifstream ifile("v1.txt");
	for (int i = 0; i < SCENARIO; ++i) {
		for (int j = 0; j < DAY; ++j) {
			for (int k = 0; k < DISTRICT; ++k) {
				for (int l = 0; l < TASK; ++l) {
					ifile >> _num_v1[i][j][k][l];
				}
			}
		}
	}
	ifile.close();
}

void Gurobi::_read_num_v2()
{
	std::ifstream ifile("v2.txt");
	for (int i = 0; i < SCENARIO; ++i) {
		for (int j = 0; j < DAY; ++j) {
			for (int k = 0; k < STATION; ++k) {
				for (int l = 0; l < CAR_TYPE; ++l) {
					ifile >> _num_v2[i][j][k][l];
				}
			}
		}
	}
	ifile.close();
}

void Gurobi::_read_num_v3()
{
	std::ifstream ifile("v3.txt");
	for (int i = 0; i < SCENARIO; ++i) {
		for (int j = 0; j < DAY; ++j) {
			for (int k = 0; k < TASK; ++k) {
				ifile >> _num_v3[i][j][k];
			}
		}
	}
	ifile.close();
}
