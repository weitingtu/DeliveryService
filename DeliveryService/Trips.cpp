#include "Trips.h"
#include <iostream>
#include <fstream>

Trip::Trip() :
	_x1(),
	_y1(),
	_v1(),
	_v2(),
	_v3(),
	_x2(),
	_y2(),
	_x3(),
	_y3(),
	_x4(),
	_y4()
{
}

Trip::~Trip()
{
}

Trips::Trips():
	_trips()
{
	//std::array<std::array<Trip, DAY>, POPULATION> _trips;
	_trips.resize(POPULATION);
	for (size_t p = 0; p < POPULATION; ++p)
	{
		_trips[p].resize(DAY);
	}
}

Trips::~Trips()
{
}

bool Trips::read_monthly_trips()
{
	if (!_read_num_x1("x1.txt"))
	{
		return false;
	}
	if (!_read_num_y1("y1.txt"))
	{
		return false;
	}
	if (!_read_num_v1("v1.txt"))
	{
		return false;
	}
	if (!_read_num_v2("v2.txt"))
	{
		return false;
	}
	if (!_read_num_v3("v3.txt"))
	{
		return false;
	}
	return true;
}

bool Trips::_read_num_x1(const std::string& x1)
{
	std::ifstream ifile(x1);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", x1.c_str());
		return false;
	}
	for (int k = 0; k < TASK; ++k)
	{
		for (int j = 0; j < DISTRICT; ++j)
		{
			for (int i = 0; i < FLEET; ++i)
			{
				for (int t = 0; t < DAY; ++t)
				{
					for (int p = 0; p < POPULATION; ++p)
					{
						ifile >> _trips[p][t].x1()[i][j][k];
						//std::cout << m << " " << l << " " << k << " " << j << " " << i << " " << _num_x1[m][l][k][j][i] << std::endl;
					}
				}
			}
		}
	}
	ifile.close();

	return true;
}

bool Trips::_read_num_y1(const std::string& y1)
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
				for (int p = 0; p < POPULATION; ++p)
				{
					ifile >> _trips[p][t].y1()[j][k];
					//std::cout <<  l << " " << k << " " << j << " " << i << " " << _num_y1[l][k][j][i] << std::endl;
				}
			}
		}
	}
	ifile.close();

	return true;
}

bool Trips::_read_num_v1(const std::string& v1)
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
				for (int p = 0; p < POPULATION; ++p)
				{
					ifile >> _trips[p][t].v1()[j][k];
					//std::cout <<  l << " " << k << " " << j << " " << i << " " << _num_v1[l][k][j][i] << std::endl;
				}
			}
		}
	}
	ifile.close();

	return true;
}

bool Trips::_read_num_v2(const std::string& v2)
{
	std::ifstream ifile(v2);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", v2.c_str());
		return false;
	}
	for (int m = 0; m < STATION; ++m)
	{
		for (int n = 0; n < CAR_TYPE; ++n)
		{
			for (int t = 0; t < DAY; ++t)
			{
				for (int p = 0; p < POPULATION; ++p)
				{
					ifile >> _trips[p][t].v2()[n][m];
					//std::cout <<  l << " " << k << " " << j << " " << i << " " << _num_v2[l][k][j][i] << std::endl;
				}
			}
		}
	}
	ifile.close();

	return true;
}

bool Trips::_read_num_v3(const std::string& v3)
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
			for (int p = 0; p < POPULATION; ++p)
			{
				ifile >> _trips[p][t].v3()[k];
				//std::cout <<k << " " << j << " " << i << " " << _num_v3[k][j][i] << std::endl;
			}
		}
	}
	ifile.close();
	return true;
}
