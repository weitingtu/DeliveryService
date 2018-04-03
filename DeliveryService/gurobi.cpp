#include "gurobi.h"
#include <iostream>
#include <fstream>

Gurobi::Gurobi()
{
}


Gurobi::~Gurobi()
{
}

void Gurobi::read()
{
	_read_cost();
}

void Gurobi::_read_cost()
{
	std::ifstream ifile("input_cost_varcost.txt");   //self truck 
	for (int i = 0; i < district; ++i) {
		for (int j = 0; j < task; ++j) {
			ifile >> _c1[i][j];
			_c2[i][j] = _c1[i][j] * cost_temporary_17; //2.5
		}
	}
	ifile.close();
}