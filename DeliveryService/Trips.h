#pragma once
#include "defines.h"
#include <array>
#include <vector>
#include <string>

class Trip
{
public:
	Trip();
	~Trip();

	const std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET >& x1() const { return _x1; }
	const std::array<std::array<int, TASK>, DISTRICT>& y1() const { return _y1; }
	const std::array<std::array<int, TASK>, DISTRICT>& v1() const { return _v1; }
	const std::array<std::array<int, STATION>, CAR_TYPE >& v2() const { return _v2; }
	const std::array<int, TASK>& v3() const { return _v3; }

	const std::array<std::array<std::array<std::array<int, TASK>, DISTRICT >, FLEET>, STOCHASTIC_DEMAND>& x2() const { return _x2; }
	const std::array<std::array<std::array<int, TASK>, DISTRICT>, STOCHASTIC_DEMAND>& y2() const { return _y2; }
	const std::array<std::array<std::array<int, STATION>, FLEET>, STOCHASTIC_DEMAND>& x3() const { return _x3; }
	const std::array<std::array<int, STATION>, STOCHASTIC_DEMAND>& y3() const { return _y3; }
	const std::array<std::array<std::array<int, TASK>, FLEET>, STOCHASTIC_DEMAND>& x4() const { return _x4; }
	const std::array<std::array<int, TASK>, STOCHASTIC_DEMAND>& y4() const { return _y4; }

	std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET >& x1() { return _x1; }
	std::array<std::array<int, TASK>, DISTRICT>& y1() { return _y1; }
	std::array<std::array<int, TASK>, DISTRICT>& v1() { return _v1; }
	std::array<std::array<int, STATION>, CAR_TYPE >& v2() { return _v2; }
	std::array<int, TASK>& v3() { return _v3; }

	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET>, STOCHASTIC_DEMAND>& x2() { return _x2; }
	std::array<std::array<std::array<int, TASK>, DISTRICT>, STOCHASTIC_DEMAND>& y2() { return _y2; }
	std::array<std::array<std::array<int, STATION>, FLEET>, STOCHASTIC_DEMAND>& x3() { return _x3; }
	std::array<std::array<int, STATION>, STOCHASTIC_DEMAND>& y3() { return _y3; }
	std::array<std::array<std::array<int, TASK>, FLEET>, STOCHASTIC_DEMAND>& x4() { return _x4; }
	std::array<std::array<int, TASK>, STOCHASTIC_DEMAND>& y4() { return _y4; }

private:
	/*50組隨機需求下，月趟次(x1,y1,v1,v2,v3) use other model to generate(totally have 50 set)*/

	std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET > _x1;
	std::array<std::array<int, TASK>, DISTRICT> _y1;
	std::array<std::array<int, TASK>, DISTRICT>  _v1;
	std::array<std::array<int, STATION>, CAR_TYPE> _v2;
	std::array<int, TASK> _v3;

	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET >, STOCHASTIC_DEMAND> _x2;
	std::array<std::array<std::array<int, TASK>, DISTRICT>, STOCHASTIC_DEMAND> _y2;
	std::array<std::array<std::array<int, STATION>, FLEET>, STOCHASTIC_DEMAND> _x3;
	std::array<std::array<int, STATION>, STOCHASTIC_DEMAND> _y3;
	std::array<std::array<std::array<int, TASK>, FLEET>, STOCHASTIC_DEMAND> _x4;
	std::array<std::array<int, TASK>, STOCHASTIC_DEMAND> _y4;
};

class Trips
{
public:
	Trips();
	~Trips();

	const std::vector<std::vector<Trip> >& trips() const { return _trips; }
	std::vector<std::vector<Trip> >& trips() { return _trips; }

	bool read_monthly_trips();

private:
	bool _read_num_x1(const std::string& x1);
	bool _read_num_y1(const std::string& y1);
	bool _read_num_v1(const std::string& v1);
	bool _read_num_v2(const std::string& v2);
	bool _read_num_v3(const std::string& v3);

	std::vector<std::vector<Trip> > _trips;
};