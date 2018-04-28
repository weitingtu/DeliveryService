#define _USE_MATH_DEFINES //產生隨機需求
#include "demands.h"
#include <iostream>
#include <fstream>
#include <cmath> 
#include <string>
#include <gurobi_c++.h>
#include <vector>

Demand::Demand() :
	_d1(),
	_d2(),
	_d3()
{
}


Demand::~Demand()
{
}

Demands::Demands() :
	_c1(),
	_c2(),
	_b1(),
	_b2(),
	_a1(),
	_c3({ { 27500, 51500 } }),
	_b3({ { 11000, 20600 } }),
	_c4({ { 10750, 10750 } }),
	_b4({ { 8600, 8600 } }),
	_a2({ { 8000, 15000 } }),
	_a3({ { 6200, 6200 } }),
	_load({ { 14, 20 } }),
	_u1(),
	_u2({ { 2, 4 } }),
	_u3({ { 1.11, 1.11 } }),
	_d1(),
	_d2(),
	_d3(),
	_demands(),
	_sum_task_demand(),
	_mean_task_demand(),
	_std_task_demand(),
	_sum_transfer_majorcustomer(),
	_mean_transfer_majorcustomer(),
	_std_transfer_majorcustomer(),
	_sum_transfer_demand(),
	_mean_transfer_demand(),
	_std_transfer_demand(),
	_pro({ { 1.0 / POPULATION } })
{
}


Demands::~Demands()
{
}

void Demands::read()
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

	_generate_stochastic_parameter();
	_generate_stochastic_demand();
}

bool Demands::_read_var_cost(const std::string& input_cost_varcost)
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
		// std::cout << j << " " <<k << " " << _c1[j][k] << std::endl;
		//ok std::cout << j << " " << k << " " << _c2[j][k] << std::endl;
		}
	}
	ifile.close();

	return true;

}

bool Demands::_read_outsourcing_cost(const std::string& input_cost_outsourcing17)
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
			//ok std::cout << j << " " << k << " " << _b1[j][k] << std::endl;
			//ok std::cout << j << " " << k << " " << _b2[j][k] << std::endl;
			//ok std::cout << j << " " << k << " " << _a1[j][k] << std::endl;
		}
	}
	ifile.close();

	return true;
}
bool Demands::_read_traveltime(const std::string& input_traveltime)
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
			// std::cout << j<< " " << k << " " << _u1[j][k] << std::endl;

		}
	}
	ifile.close();

	return true;
}
bool Demands::_read_d1(const std::string& input_demand_task_collection, const std::string& input_demand_task_sending)
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
					ifile1 >> _d1[0][t][j][k]; //第一組需求		stjk
					//ok std::cout << t << " " <<j<< " "<<k<<" " << _d1[0][t][j][k] << std::endl;
					ifile1 >> _demands[0][t].d1()[j][k];
				}
			}
		}
		else if (k == 1)
		{
			for (int j = 0; j < DISTRICT; ++j)
			{
				for (int t = 0; t < DAY; ++t)
				{
					ifile2 >> _d1[0][t][j][k]; //第一組需求	
					//ok  std::cout << t << " " <<j<< " "<<k<<" " << _d1[0][t][j][k] << std::endl;
					ifile2 >> _demands[0][t].d1()[j][k];
				}
			}
		}
	}

	ifile1.close();
	ifile2.close();

	return true;
}
bool Demands::_read_d2(const std::string& input_demand_transfer)
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
			ifile >> _d2[0][t][m]; //第一組需求
			//ok std::cout << t << " " << m << " " << _d2[0][t][m] << std::endl;
			ifile >> _demands[0][t].d2()[m];//第一組需求
		}
	}

	ifile.close();

	return true;
}
bool Demands::_read_d3(const std::string& input_demand_transfer_majorcoutomer)
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
			ifile >> _d3[0][t][k]; //第一組需求
			 //ok std::cout << t << " " << k << " " << _d3[0][t][k] << std::endl;
			ifile >> _demands[0][t].d3()[k]; //第一組需求
		}
	}

	ifile.close();

	return true;
}
void Demands::_generate_stochastic_parameter() //sum, mean, std
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
			//ok std::cout << j << " " << k << " " << _sum_task_demand[j][k] << std::endl;
			//ok std::cout << j << " " << k << " " << _mean_task_demand[j][k] << std::endl;
			//ok std::cout << j << " " << k << " " << _std_task_demand[j][k] << std::endl;
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
		//ok std::cout << i << " " << _sum_transfer_demand[i] << std::endl;
		//ok std::cout << i << " " << _mean_transfer_demand[i] << std::endl;
		//ok std::cout << i << " " << _std_transfer_demand[i] << std::endl;
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
		//ok std::cout << i << " " << _sum_transfer_majorcustomer[i] << std::endl;
		//ok std::cout << i << " " << _mean_transfer_majorcustomer[i] << std::endl;
		//std::cout << i << " " << _std_transfer_majorcustomer[i] << std::endl;
	}

}
void Demands::_generate_stochastic_demand()
{
	double r1, r2;
	for (int p = 1; p < POPULATION; ++p)
	{ //第二組需求開始
		for (int t = 0; t < DAY; ++t)
		{
			for (int j = 0; j < DISTRICT; ++j)
			{
				for (int k = 0; k < TASK; ++k)
				{
					r1 = rand() / (double)RAND_MAX;
					r2 = rand() / (double)RAND_MAX;
					_d1[p][t][j][k] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) * _std_task_demand[j][k] + _mean_task_demand[j][k];
					//std::cout <<"scenario" <<s << " " <<t << " " << j << " " << k<< " " << _d1[s][t][j][k] << std::endl;
					_demands[p][t].d1()[j][k] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) * _std_task_demand[j][k] + _mean_task_demand[j][k];
				}
			}
		}
	}

	for (int p = 1; p < POPULATION; ++p)
	{//第二組需求開始
		for (int t = 0; t < DAY; ++t)
		{
			for (int k = 0; k < TASK; ++k)
			{
				r1 = rand() / (double)RAND_MAX;
				r2 = rand() / (double)RAND_MAX;
				_d3[p][t][k] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) *_std_transfer_majorcustomer[k] + _mean_transfer_majorcustomer[k];
				//std::cout <<"scenario" <<s << " " << t << " " << k  << " " << _d3[s][t][k] << std::endl;
				_demands[p][t].d3()[k] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) *_std_transfer_majorcustomer[k] + _mean_transfer_majorcustomer[k];
			}
		}
	}

	for (int p = 1; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int m = 0; m < STATION; ++m)
			{
				r1 = rand() / (double)RAND_MAX;
				r2 = rand() / (double)RAND_MAX;
				_d2[p][t][m] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) *_std_transfer_demand[m] + _mean_transfer_demand[m];
				//std::cout <<"scenario" <<s << " " <<t << " " << m  << " " << _d2[s][t][m] << std::endl;
				_demands[p][t].d2()[m] = sqrt(-2.5 * log(r1)) * cos(2.5 * M_PI * r2) *_std_transfer_demand[m] + _mean_transfer_demand[m];
			}
		}
	}
}
