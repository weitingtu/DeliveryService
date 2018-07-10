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

void Trip::clear_daily()
{
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					_x2.at(s).at(i).at(j).at(k) = 0;
				}
			}
		}
	}
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				_y2.at(s).at(j).at(k) = 0;
			}
		}
	}
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				_x3.at(s).at(i).at(m) = 0;
			}
		}
	}
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			_y3.at(s).at(m) = 0;
		}
	}
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				_x4.at(s).at(i).at(k) = 0;
			}
		}
	}
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			_y4.at(s).at(k) = 0;
		}
	}
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

bool Trips::write_monthly_trips() const
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
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
			{
				for (int m = 0; m < STATION; ++m)
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
	for (int p = 0; p < POPULATION; ++p)
	{
		for (int t = 0; t < DAY; ++t)
		{
			for (int s = 0; s < STOCHASTIC_DEMAND; ++s)
			{
				for (int k = 0; k < TASK; ++k)
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
							ofile << _trips[p][t].x2()[s][i][j][k] << std::endl;
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
						ofile << _trips[p][t].x3()[s][i][m] << std::endl;
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
						ofile << _trips[p][t].x4()[s][i][k] << std::endl;
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
						ofile << _trips[p][t].y2()[s][j][k] << std::endl;
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
					ofile << _trips[p][t].y3()[s][m] << std::endl;
				}
			}
		}
	}
	ofile.close();
	return true;
}
bool Trips::_write_y4(const std::string& file_name) const
{
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
					ofile << _trips[p][t].y4()[s][k] << std::endl;
				}
			}
		}
	}
	ofile.close();
	return true;
}

bool Trips::write_trips(const std::string& file_name) const
{
	std::ofstream ofile(file_name, std::ofstream::out);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	for (size_t p = 0; p < POPULATION; ++p)
	{
		ofile << "Population " << p << std::endl;
		for (size_t t = 0; t < DAY; ++t)
		{
			ofile << "Day " << t << std::endl;
			_write_x1(ofile, p, t, 0);
			_write_x1(ofile, p, t, 1);
			_write_y1(ofile, p, t, 0);
			_write_y1(ofile, p, t, 1);
			_write_v1(ofile, p, t, 0);
			_write_v1(ofile, p, t, 1);
			_write_x2(ofile, p, t, 0);
			_write_x2(ofile, p, t, 1);
			_write_x3(ofile, p, t, 0);
			_write_x3(ofile, p, t, 1);
			_write_x4(ofile, p, t, 0);
			_write_x4(ofile, p, t, 1);
			_write_y2(ofile, p, t, 0);
			_write_y2(ofile, p, t, 1);
			_write_y3(ofile, p, t, 0);
			_write_y3(ofile, p, t, 1);
			_write_y4(ofile, p, t, 0);
			_write_y4(ofile, p, t, 1);
		}
	}
	return true;
}

std::string Trips::_cmd_k(size_t k) const
{
	if (0 == k)
	{
		return "collecting ";
	}
	return "sending ";
}

void Trips::_write_x1(std::ofstream& ofile, size_t p, size_t t, size_t k) const
{
	const Trip& trip = trips()[p][t];
	int total = 0;
	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			total += trip.x1()[i][j][k];
		}
	}
	std::string cmd("x1 self-run truck ");
	cmd += _cmd_k(k);
	ofile << cmd << total << std::endl;
}

void Trips::_write_y1(std::ofstream& ofile, size_t p, size_t t, size_t k) const
{
	const Trip& trip = trips()[p][t];
	int total = 0;
	for (int j = 0; j < DISTRICT; ++j)
	{
		total += trip.y1()[j][k];
	}

	std::string cmd("y1 outsourcing truck ");
	cmd += _cmd_k(k);
	ofile << cmd << total << std::endl;
}

void Trips::_write_v1(std::ofstream& ofile, size_t p, size_t t, size_t k) const
{
	const Trip& trip = trips()[p][t];
	int total = 0;
	for (int j = 0; j < DISTRICT; ++j)
	{
		total += trip.v1()[j][k];
	}

	std::string cmd("v1 outsourcing container truck ");
	cmd += _cmd_k(k);
	ofile << cmd << total << std::endl;
}

void Trips::_write_v2(std::ofstream& ofile, size_t p, size_t t, size_t n) const
{
	const Trip& trip = trips()[p][t];
	int total = 0;
	for (int m = 0; m < STATION; ++m)
	{
		total += trip.v2()[n][m];
	}

	std::string cmd("v2 outsourcing container truck type");
	cmd += std::to_string(n);
	cmd += " ";
	ofile << cmd << total << std::endl;
}

void Trips::_write_v3(std::ofstream& ofile, size_t p, size_t t, size_t k) const
{
	const Trip& trip = trips()[p][t];
	int total = 0;
	total += trip.v3()[k];

	std::string cmd("v3 outsourcing container truck MC ");
	cmd += _cmd_k(k);
	ofile << cmd << total << std::endl;
}

void Trips::_write_x2(std::ofstream& ofile, size_t p, size_t t, size_t k) const
{
	const Trip& trip = trips()[p][t];
	double total = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				total += trip.x2()[s][i][j][k];
			}
		}
	}
	total /= STOCHASTIC_DEMAND;
	std::string cmd("x2 self-run truck ");
	cmd += _cmd_k(k);
	ofile << cmd << total << std::endl;
}

void Trips::_write_x3(std::ofstream& ofile, size_t p, size_t t, size_t m) const
{
	const Trip& trip = trips()[p][t];
	double total = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			total += trip.x3()[s][i][m];
		}
	}
	total /= STOCHASTIC_DEMAND;
	std::string cmd("x3 self-run truck station");
	cmd += std::to_string(m);
	cmd += " ";
	ofile << cmd << total << std::endl;
}

void Trips::_write_x4(std::ofstream& ofile, size_t p, size_t t, size_t k) const
{
	const Trip& trip = trips()[p][t];
	double total = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			total += trip.x4()[s][i][k];
		}
	}
	total /= STOCHASTIC_DEMAND;
	std::string cmd("x4 self-run truck MC ");
	cmd += _cmd_k(k);
	ofile << cmd << total << std::endl;
}

void Trips::_write_y2(std::ofstream& ofile, size_t p, size_t t, size_t k) const
{
	const Trip& trip = trips()[p][t];
	double total = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			total += trip.y2()[s][j][k];
		}
	}
	total /= STOCHASTIC_DEMAND;
	std::string cmd("y2 outsourcing truck ");
	cmd += _cmd_k(k);
	ofile << cmd << total << std::endl;
}

void Trips::_write_y3(std::ofstream& ofile, size_t p, size_t t, size_t m) const
{
	const Trip& trip = trips()[p][t];
	double total = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		total += trip.y3()[s][m];
	}
	total /= STOCHASTIC_DEMAND;
	std::string cmd("y3 outsourcing truck station");
	cmd += std::to_string(m);
	cmd += " ";
	ofile << cmd << total << std::endl;
}

void Trips::_write_y4(std::ofstream& ofile, size_t p, size_t t, size_t k) const
{
	const Trip& trip = trips()[p][t];
	double total = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		total += trip.y4()[s][k];
	}
	total /= STOCHASTIC_DEMAND;
	std::string cmd("y4 self-run truck MC ");
	cmd += _cmd_k(k);
	ofile << cmd << total << std::endl;
}

