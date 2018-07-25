#include "writerCsv.h"
#include <iostream>
#include <fstream>



WriterCsv::WriterCsv(const Demands& d, const std::vector<std::vector<Trip> >& t) :
	_demands(d),
	_trips(t)
{
}


WriterCsv::~WriterCsv()
{
}

std::string WriterCsv::_cmd_k(size_t k) const
{
	if (0 == k)
	{
		return "collecting ";
	}
	return "sending ";
}

// (7)
double WriterCsv::_day_cost(size_t p, size_t t) const
{
	const Trip& trip = _trips.at(p).at(t);

	double cost = 0;

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				cost += _demands.c1().at(j).at(k) * (double)trip.x1().at(i).at(j).at(k);
			}
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.b1().at(j).at(k) * (double)trip.y1().at(j).at(k);
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.a1().at(j).at(k) * (double)trip.v1().at(j).at(k);
		}
	}

	for (size_t n = 0; n < CAR_TYPE; ++n)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			cost += _demands.a2().at(m) * (double)trip.v2().at(n).at(m);
		}
	}

	for (size_t k = 0; k < TASK; ++k)
	{
		cost += _demands.a3().at(k) * (double)trip.v3().at(k);
	}

	return cost;
}

double WriterCsv::_all_day_cost(size_t p, size_t t) const
{
	double cost = _day_cost(p, t);
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		cost += _cost_2(p, t, s) / STOCHASTIC_DEMAND;
	}

	return cost;
}

double WriterCsv::_total_district_cost(size_t p, size_t t) const
{
	const Trip& trip = _trips.at(p).at(t);

	double cost = 0;

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				cost += _demands.c1().at(j).at(k) * (double)trip.x1().at(i).at(j).at(k);
			}
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.b1().at(j).at(k) * (double)trip.y1().at(j).at(k);
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.a1().at(j).at(k) * (double)trip.v1().at(j).at(k);
		}
	}

	return cost;
}

double WriterCsv::_total_district_task_cost(size_t p, size_t t, size_t k) const
{
	const Trip& trip = _trips.at(p).at(t);

	double cost = 0;

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			cost += _demands.c1().at(j).at(k) * (double)trip.x1().at(i).at(j).at(k);
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		cost += _demands.b1().at(j).at(k) * (double)trip.y1().at(j).at(k);
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		cost += _demands.a1().at(j).at(k) * (double)trip.v1().at(j).at(k);
	}

	return cost;
}

double WriterCsv::_district_cost(size_t p, size_t t, size_t j) const
{
	const Trip& trip = _trips.at(p).at(t);

	double cost = 0;

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.c1().at(j).at(k) * (double)trip.x1().at(i).at(j).at(k);
		}
	}

	for (size_t k = 0; k < TASK; ++k)
	{
		cost += _demands.b1().at(j).at(k) * (double)trip.y1().at(j).at(k);
	}

	for (size_t k = 0; k < TASK; ++k)
	{
		cost += _demands.a1().at(j).at(k) * (double)trip.v1().at(j).at(k);
	}

	return cost;
}

double WriterCsv::_district_task_cost(size_t p, size_t t, size_t j, size_t k) const
{
	const Trip& trip = _trips.at(p).at(t);

	double cost = 0;

	for (size_t i = 0; i < FLEET; ++i)
	{
		cost += _demands.c1().at(j).at(k) * (double)trip.x1().at(i).at(j).at(k);
	}

	cost += _demands.b1().at(j).at(k) * (double)trip.y1().at(j).at(k);

	cost += _demands.a1().at(j).at(k) * (double)trip.v1().at(j).at(k);

	return cost;
}

double WriterCsv::_trans_cost(size_t p, size_t t) const
{
	const Trip& trip = _trips.at(p).at(t);

	double cost = 0;

	for (size_t n = 0; n < CAR_TYPE; ++n)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			cost += _demands.a2().at(m) * (double)trip.v2().at(n).at(m);
		}
	}

	return cost;
}

double WriterCsv::_trans_station_cost(size_t p, size_t t, size_t m) const
{
	const Trip& trip = _trips.at(p).at(t);

	double cost = 0;

	for (size_t n = 0; n < CAR_TYPE; ++n)
	{
		cost += _demands.a2().at(m) * (double)trip.v2().at(n).at(m);
	}

	return cost;
}

double WriterCsv::_major_customer_cost(size_t p, size_t t) const
{
	const Trip& trip = _trips.at(p).at(t);

	double cost = 0;

	for (size_t k = 0; k < TASK; ++k)
	{
		cost += _demands.a3().at(k) * (double)trip.v3().at(k);
	}

	return cost;
}

// (15)
double WriterCsv::_cost_2(size_t p, size_t t, size_t s) const
{
	const Trip& trip = _trips.at(p).at(t);

	double cost = 0;

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				cost += _demands.c2().at(j).at(k) * (double)trip.x2().at(s).at(i).at(j).at(k);
			}
		}
	}

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			cost += _demands.c3().at(m) * (double)trip.x3().at(s).at(i).at(m);
		}
	}

	for (size_t i = 0; i < FLEET; ++i)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.c4().at(k) * (double)trip.x4().at(s).at(i).at(k);
		}
	}

	for (size_t j = 0; j < DISTRICT; ++j)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			cost += _demands.b2().at(j).at(k) * (double)trip.y2().at(s).at(j).at(k);
		}
	}

	for (size_t m = 0; m < STATION; ++m)
	{
		cost += _demands.b3().at(m) * (double)trip.y3().at(s).at(m);
	}

	for (size_t k = 0; k < TASK; ++k)
	{
		cost += _demands.b4().at(k) * (double)trip.y4().at(s).at(k);
	}

	return cost;
}

void WriterCsv::_write_d1(std::ofstream& ofile, size_t p, size_t t, size_t j, size_t k) const
{
	const Demand& demand = _demands.demands()[p][t];
	const Trip& trip = _trips.at(p).at(t);

	for (size_t i = 0; i < FLEET; ++i)
	{
		ofile << "x1 " << i << " " << trip.x1()[i][j][k] << " * 14 / " << _demands.c1()[j][k] << std::endl;
	}

	for (size_t i = 0; i < FLEET; ++i)
	{
		int x2 = 0;
		for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
		{
			x2 += trip.x2()[s][i][j][k];
		}
		ofile << "x2 " << i << " " << (double)x2 / STOCHASTIC_DEMAND << " * 14 / " << _demands.c2()[j][k] << std::endl;
	}

	ofile << "y1 " << trip.y1()[j][k] << " * 14 / " << _demands.b1()[j][k] << std::endl;
	int y2 = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		y2 += trip.y2()[s][j][k];
	}
	ofile << "y2 " << (double)y2 / STOCHASTIC_DEMAND << " * 14 / " << _demands.b2()[j][k] << std::endl;

	ofile << "v1 " << trip.v1()[j][k] << " * 20 / " << _demands.a1()[j][k] << std::endl;

	ofile << "d1 " << demand.d1()[j][k] << std::endl;
	double d1 = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		d1 += _demands.demands()[s][t].d1()[j][k];
	}
	ofile << "average d1 " << d1 / STOCHASTIC_DEMAND << std::endl;
}

void WriterCsv::_write_d2(std::ofstream& ofile, size_t p, size_t t, size_t m) const
{
	const Demand& demand = _demands.demands()[p][t];
	const Trip& trip = _trips.at(p).at(t);

	for (size_t i = 0; i < FLEET; ++i)
	{
		int x3 = 0;
		for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
		{
			x3 += trip.x3()[s][i][m];
		}
		ofile << "x3 " << i << " " << (double)x3 / STOCHASTIC_DEMAND << " * 14 / " << _demands.c3()[m] << std::endl;
	}

	int y3 = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		y3 += trip.y3()[s][m];
	}
	ofile << "y3 " << (double)y3 / STOCHASTIC_DEMAND << " * 14 / " << _demands.b3()[m] << std::endl;

	int v2 = 0;
	for (size_t n = 0; n < CAR_TYPE; ++n)
	{
		v2 += trip.v2()[n][m];
	}
	ofile << "v2 " << v2 << " * 20 / " << _demands.a2()[m] << std::endl;

	ofile << "d2 " << demand.d2()[m] << std::endl;
	double d2 = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		d2 += _demands.demands()[s][t].d2()[m];
	}
	ofile << "average d2 " << d2 / STOCHASTIC_DEMAND << std::endl;
}

void WriterCsv::_write_d3(std::ofstream& ofile, size_t p, size_t t, size_t k) const
{
	const Demand& demand = _demands.demands()[p][t];
	const Trip& trip = _trips.at(p).at(t);

	for (size_t i = 0; i < FLEET; ++i)
	{
		int x4 = 0;
		for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
		{
			x4 += trip.x4()[s][i][k];
		}
		ofile << "x4 " << i << " " << (double)x4 / STOCHASTIC_DEMAND << " * 14 / " << _demands.c4()[k] << std::endl;
	}

	int y4 = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		y4 += trip.y4()[s][k];
	}
	ofile << "y4 " << (double)y4 / STOCHASTIC_DEMAND << " * 14 / " << _demands.b4()[k] << std::endl;

	ofile << "v3 " << trip.v3()[k] << " * 20 / " << _demands.a3()[k] << std::endl;

	ofile << "d3 " << demand.d3()[k] << std::endl;
	double d3 = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		d3 += _demands.demands()[s][t].d3()[k];
	}
	ofile << "average d3 " << d3 / STOCHASTIC_DEMAND << std::endl;
}

void WriterCsv::_write_cost(std::ofstream& ofile, size_t p, size_t t) const
{
	ofile << "cost " << _day_cost(p, t) << std::endl;
	double cost = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		cost += _cost_2(p, t, s);
	}
	ofile << "average cost " << cost / STOCHASTIC_DEMAND << std::endl;
}

bool WriterCsv::write_trips(const std::string& file_name) const
{
	std::ofstream ofile(file_name, std::ofstream::out);
	if (ofile.fail())
	{
		printf("Unable to open %s\n", file_name.c_str());
		return false;
	}
	ofile
		<< "Population,"
		<< "Day,"
		<< "objective,"
		//		<< "solving time,"
		<< "district task cost,"
		<< "district task cost collecting,"
		<< "district task cost sending,"
		<< "district 1 cost,"
		<< "d1 cost collecting,"
		<< "d1 cost sending,"
		<< "district 2 cost,"
		<< "d2 cost collecting,"
		<< "d2 cost sending,"
		<< "district 3 cost,"
		<< "d3 cost collecting,"
		<< "d3 cost sending,"
		<< "district 4 cost,"
		<< "d4 cost collecting,"
		<< "d4 cost sending,"
		<< "transfer taks cost,"
		<< "transshipment center 1 cost,"
		<< "transshipment center 2 cost,"
		<< "major customer task cost,"
		<< std::endl;

	for (size_t p = 0; p < _trips.size(); ++p)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			ofile << p << ",";
			ofile << t << ",";
			ofile << _all_day_cost(p, t) << ",";
			ofile << _total_district_cost(p, t) << ",";
			for (size_t k = 0; k < TASK; ++k)
			{
				ofile << _total_district_task_cost(p, t, k) << ",";
			}
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				ofile << _district_cost(p, t, j) << ",";
				for (size_t k = 0; k < TASK; ++k)
				{
					ofile << _district_task_cost(p, t, j, k) << ",";
				}
			}
			ofile << _trans_cost(p, t) << ",";
			for (size_t m = 0; m < STATION; ++m)
			{
				ofile << _trans_station_cost(p, t, m) << ",";
			}
			ofile << _major_customer_cost(p, t) << ",";
			ofile << std::endl;
		}
	}
	return true;
}
