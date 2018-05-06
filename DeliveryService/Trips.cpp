#include "Trips.h"
#include <iostream>
#include <fstream>
#include <gurobi_c++.h>

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

Trips::Trips() :
	_trips()
{
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
	if (!_read_x1("x1.txt"))
	{
		return false;
	}
	if (!_read_y1("y1.txt"))
	{
		return false;
	}
	if (!_read_v1("v1.txt"))
	{
		return false;
	}
	if (!_read_v2("v2.txt"))
	{
		return false;
	}
	if (!_read_v3("v3.txt"))
	{
		return false;
	}
	return true;
}

bool Trips::read_daily_trips()
{
	// to do: read x2, x3, x4, y2, y3, y4
	if (!_read_x2("x2.txt"))
	{
		return false;
	}
	if (!_read_x3("x3.txt"))
	{
		return false;
	}
	if (!_read_x4("x4.txt"))
	{
		return false;
	}
	if (!_read_y2("y2.txt"))
	{
		return false;
	}
	if (!_read_y3("y3.txt"))
	{
		return false;
	}
	if (!_read_y4("y4.txt"))
	{
		return false;
	}
	return true;
}

bool Trips::write_monthly_trips() const//呼叫write_x*看有沒有讀
{
	if (!_write_x1("x1.txt"))
	{
		return false;
	}
	if (!_write_y1("y1.txt"))
	{
		return false;
	}
	if (!_write_v1("v1.txt"))
	{
		return false;
	}
	if (!_write_v2("v2.txt"))
	{
		return false;
	}
	if (!_write_v3("v3.txt"))
	{
		return false;
	}
	return true;
}

bool Trips::write_daily_trips() const
{
	if (!_write_x2("x2.txt"))
	{
		return false;
	}
	if (!_write_x3("x3.txt"))
	{
		return false;
	}
	if (!_write_x4("x4.txt"))
	{
		return false;
	}
	if (!_write_y2("y2.txt"))
	{
		return false;
	}
	if (!_write_y3("y3.txt"))
	{
		return false;
	}
	if (!_write_y4("y4.txt"))
	{
		return false;
	}
	return true;
}

bool Trips::_read_x1(const std::string& x1)
{
	std::ifstream ifile(x1);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", x1.c_str());
		return false;
	}
	//for (int k = 0; k < TASK; ++k)
	//{
	//	for (int j = 0; j < DISTRICT; ++j)
	//	{
	//		for (int i = 0; i < FLEET; ++i)
	//		{
	//			for (int t = 0; t < DAY; ++t)
	//			{
	//				for (int p = 0; p < POPULATION; ++p)
	//				{
	//					ifile >> _trips[p][t].x1()[i][j][k];
	//				}
	//			}
	//		}
	//	}
	//}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int i = 0; i < FLEET; ++i)
			{
				for (int j = 0; j < DISTRICT; ++j)
				{
					for (int k = 0; k < TASK; ++k)
					{
						ifile >> _trips[p][t].x1()[i][j][k];
					}
				}
			}
		}
	}
	ifile.close();

	return true;
}

bool Trips::_read_y1(const std::string& y1)
{
	std::ifstream ifile(y1);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", y1.c_str());
		return false;
	}
	//for (int k = 0; k < TASK; ++k)
	//{
	//	for (int j = 0; j < DISTRICT; ++j)
	//	{
	//		for (int t = 0; t < DAY; ++t)
	//		{
	//			for (int p = 0; p < POPULATION; ++p)
	//			{
	//				ifile >> _trips[p][t].y1()[j][k];
	//			}
	//		}
	//	}
	//}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int j = 0; j < DISTRICT; ++j)
			{
				for (int k = 0; k < TASK; ++k)
				{
					ifile >> _trips[p][t].y1()[j][k];
				}
			}
		}
	}
	ifile.close();

	return true;
}

bool Trips::_read_v1(const std::string& v1)
{
	std::ifstream ifile(v1);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", v1.c_str());
		return false;
	}
	//for (int k = 0; k < TASK; ++k)
	//{
	//	for (int j = 0; j < DISTRICT; ++j)
	//	{
	//		for (int t = 0; t < DAY; ++t)
	//		{
	//			for (int p = 0; p < POPULATION; ++p)
	//			{
	//				ifile >> _trips[p][t].v1()[j][k];
	//			}
	//		}
	//	}
	//}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int j = 0; j < DISTRICT; ++j)
			{
				for (int k = 0; k < TASK; ++k)
				{
					ifile >> _trips[p][t].v1()[j][k];
				}
			}
		}
	}
	ifile.close();

	return true;
}

bool Trips::_read_v2(const std::string& v2)
{
	std::ifstream ifile(v2);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", v2.c_str());
		return false;
	}
	//for (int m = 0; m < STATION; ++m)
	//{
	//	for (int n = 0; n < CAR_TYPE; ++n)
	//	{
	//		for (int t = 0; t < DAY; ++t)
	//		{
	//			for (int p = 0; p < POPULATION; ++p)
	//			{
	//				ifile >> _trips[p][t].v2()[n][m];
	//			}
	//		}
	//	}
	//}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int n = 0; n < CAR_TYPE; ++n)
			{
				for (int m = 0; m < STATION; ++m)
				{
					ifile >> _trips[p][t].v2()[n][m];
				}
			}
		}
	}
	ifile.close();

	return true;
}

bool Trips::_read_v3(const std::string& v3)
{
	std::ifstream ifile(v3);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", v3.c_str());
		return false;
	}
	//for (int k = 0; k < TASK; ++k)
	//{
	//	for (int t = 0; t < DAY; ++t)
	//	{
	//		for (int p = 0; p < POPULATION; ++p)
	//		{
	//			ifile >> _trips[p][t].v3()[k];
	//		}
	//	}
	//}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int k = 0; k < TASK; ++k)
			{
				ifile >> _trips[p][t].v3()[k];
			}
		}
	}
	ifile.close();
	return true;
}
bool Trips::_read_x2(const std::string& x2)
{
	std::ifstream ifile(x2);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", x2.c_str());
		return false;
	}
	for (int k = 0; k < TASK; ++k)
	{
		for (int j = 0; j < DISTRICT; ++j) {

			for (int i = 0; i < FLEET; ++i) {

				for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
				{
					for (int t = 0; t < DAY; ++t)
					{
						for (int p = 0; p < POPULATION; ++p)
						{
							ifile >> _trips[p][t].x2()[s][i][j][k];
						}
					}
				}
			}
		}
	}

	ifile.close();
	return true;
}
bool Trips::_read_x3(const std::string& x3)
{
	std::ifstream ifile(x3);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", x3.c_str());
		return false;
	}
	for (int m = 0; m < STATION; ++m) {

		for (int i = 0; i < FLEET; ++i) {

			for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
			{
				for (int t = 0; t < DAY; ++t)
				{
					for (int p = 0; p < POPULATION; ++p)
					{
						ifile >> _trips[p][t].x3()[s][i][m];
					}
				}
			}
		}
	}
	ifile.close();
	return true;
}
bool Trips::_read_x4(const std::string& x4)
{
	std::ifstream ifile(x4);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", x4.c_str());
		return false;
	}
	for (int k = 0; k < TASK; ++k) {

		for (int i = 0; i < FLEET; ++i) {

			for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
			{
				for (int t = 0; t < DAY; ++t)
				{
					for (int p = 0; p < POPULATION; ++p)
					{
						ifile >> _trips[p][t].x4()[s][i][k];
					}
				}
			}
		}
	}
	ifile.close();
	return true;
}
bool Trips::_read_y2(const std::string& y2)
{
	std::ifstream ifile(y2);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", y2.c_str());
		return false;
	}
	for (int k = 0; k < TASK; ++k)
	{
		for (int j = 0; j < DISTRICT; ++j)
		{
			for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
			{
				for (int t = 0; t < DAY; ++t)
				{
					for (int p = 0; p < POPULATION; ++p)
					{
						ifile >> _trips[p][t].y2()[s][j][k];
					}
				}
			}
		}
	}
	ifile.close();
	return true;
}
bool Trips::_read_y3(const std::string& y3)
{
	std::ifstream ifile(y3);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", y3.c_str());
		return false;
	}
	for (int m = 0; m < STATION; ++m)
	{
		for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
		{
			for (int t = 0; t < DAY; ++t)
			{
				for (int p = 0; p < POPULATION; ++p)
				{
					ifile >> _trips[p][t].y3()[s][m];
				}
			}
		}
	}
	ifile.close();
	return true;
}
bool Trips::_read_y4(const std::string& y4)
{
	std::ifstream ifile(y4);
	if (ifile.fail())
	{
		printf("Unable to open%s\n", y4.c_str());
		return false;
	}
	for (int k = 0; k < TASK; ++k)
	{
		for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
		{
			for (int t = 0; t < DAY; ++t)
			{
				for (int p = 0; p < POPULATION; ++p)
				{
					ifile >> _trips[p][t].y4()[s][k];
				}
			}
		}
	}
	ifile.close();
	return true;
}
bool Trips::_write_x1(const std::string& file_name) const
{
	//std::ofstream ofile(file_name, std::ofstream::out | std::ofstream::trunc);
	std::ofstream ofile(file_name, std::ofstream::out);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			const Trip& trip = trips()[p][t];
			for (int i = 0; i < FLEET; ++i)
			{
				for (int j = 0; j < DISTRICT; ++j)
				{
					for (int k = 0; k < TASK; ++k)
					{
						ofile << trips()[p][t].x1()[i][j][k] << std::endl;
					}
				}
			}
		}
	}
	ofile.close();
	return true;
}

bool Trips::_write_y1(const std::string& file_name) const
{
	//std::ofstream ofile(file_name, std::ofstream::out | std::ofstream::trunc);
	std::ofstream ofile(file_name, std::ofstream::out);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int j = 0; j < DISTRICT; ++j)
			{
				for (int k = 0; k < TASK; ++k)
				{
					ofile << trips()[p][t].y1()[j][k] << std::endl;
				}
			}
		}
	}
	ofile.close();
	return true;
}

bool Trips::_write_v1(const std::string& file_name) const
{
	//std::ofstream ofile(file_name, std::ofstream::out | std::ofstream::trunc);
	std::ofstream ofile(file_name, std::ofstream::out);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int j = 0; j < DISTRICT; ++j)
			{
				for (int k = 0; k < TASK; ++k)
				{
					ofile << trips()[p][t].v1()[j][k] << std::endl;
				}
			}
		}
	}
	ofile.close();
	return true;
}

bool Trips::_write_v2(const std::string& file_name) const
{
	//std::ofstream ofile(file_name, std::ofstream::out | std::ofstream::trunc);
	std::ofstream ofile(file_name, std::ofstream::out);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int n = 0; n < CAR_TYPE; ++n)
			{
				for (int m = 0; m < STATION; ++m)
				{
					ofile << trips()[p][t].v2()[n][m] << std::endl;
				}
			}
		}
	}
	ofile.close();
	return true;
}

bool Trips::_write_v3(const std::string& file_name) const
{
	//std::ofstream ofile(file_name, std::ofstream::out | std::ofstream::trunc);
	std::ofstream ofile(file_name, std::ofstream::out);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int k = 0; k < TASK; ++k)
			{
				ofile << trips()[p][t].v3()[k] << std::endl;
			}
		}
	}
	ofile.close();
	return true;
}

bool Trips::_write_x2(const std::string& file_name) const
{
	//std::ofstream ofile(file_name, std::ofstream::out | std::ofstream::trunc);
	std::ofstream ofile(file_name, std::ofstream::out);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
			{
				for (int i = 0; i < FLEET; ++i)
				{
					for (int j = 0; j < DISTRICT; ++j)
					{
						for (int k = 0; k < TASK; ++k)
						{
							ofile << _trips[p][t].x2()[s][i][j][k];

						}

					}
				}
			}
		}
	}
	ofile.close();
	return true;
}
bool Trips::_write_x3(const std::string& file_name) const
{
	//std::ofstream ofile(file_name, std::ofstream::out | std::ofstream::trunc);
	std::ofstream ofile(file_name, std::ofstream::out);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}

	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
			{
				for (int i = 0; i < FLEET; ++i)
				{
					for (int m = 0; m < STATION; ++m)
					{
						ofile << _trips[p][t].x3()[s][i][m];
					}
				}
			}
		}
	}
	ofile.close();
	return true;
}
bool Trips::_write_x4(const std::string& file_name) const
{
	//std::ofstream ofile(file_name, std::ofstream::out | std::ofstream::trunc);
	std::ofstream ofile(file_name, std::ofstream::out);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}

	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
			{
				for (int i = 0; i < FLEET; ++i)
				{
					for (int k = 0; k < TASK; ++k)
					{
						ofile << _trips[p][t].x4()[s][i][k];
					}
				}
			}
		}
	}
	ofile.close();
	return true;
}
bool Trips::_write_y2(const std::string& file_name) const
{
	//std::ofstream ofile(file_name, std::ofstream::out | std::ofstream::trunc);
	std::ofstream ofile(file_name, std::ofstream::out);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
			{
				for (int j = 0; j < DISTRICT; ++j)
				{
					for (int k = 0; k < TASK; ++k)
					{
						ofile << _trips[p][t].y2()[s][j][k];
					}
				}
			}
		}
	}
	ofile.close();
	return true;
}
bool Trips::_write_y3(const std::string& file_name) const
{
	//std::ofstream ofile(file_name, std::ofstream::out | std::ofstream::trunc);
	std::ofstream ofile(file_name, std::ofstream::out);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
			{
				for (int m = 0; m < STATION; ++m)
				{

					ofile << _trips[p][t].y3()[s][m];

				}
			}
		}
	}
	ofile.close();
	return true;
}
bool Trips::_write_y4(const std::string& file_name) const
{
	//std::ofstream ofile(file_name, std::ofstream::out | std::ofstream::trunc);
	std::ofstream ofile(file_name, std::ofstream::out);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
			{
				for (int k = 0; k < TASK; ++k)
				{

					ofile << _trips[p][t].y4()[s][k];

				}
			}
		}
	}
	ofile.close();
	return true;
}