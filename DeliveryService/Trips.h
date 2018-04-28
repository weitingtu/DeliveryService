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

	const std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET >, DAY>, POPULATION>& x1() const { return _x1; }
	const std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, POPULATION>& y1() const { return _y1; }
	const std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, POPULATION>& v1() const { return _v1; }
	const std::array<std::array<std::array<std::array<int, STATION>, CAR_TYPE >, DAY>, POPULATION>& v2() const { return _v2; }
	const std::array<std::array<std::array<int, TASK>, DAY >, POPULATION>& v3() const { return _v3; }

	const std::array<std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT >, FLEET>, DAY>, STOCHASTIC_DEMAND>, POPULATION>& x2() const { return _x2; }
	const std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, STOCHASTIC_DEMAND>, POPULATION>& y2() const { return _y2; }
	const std::array<std::array<std::array<std::array<std::array<int, STATION>, FLEET>, DAY>, STOCHASTIC_DEMAND>, POPULATION>& x3() const { return _x3; }
	const std::array<std::array<std::array<std::array<int, STATION>, DAY>, STOCHASTIC_DEMAND>, POPULATION>& y3() const { return _y3; }
	const std::array<std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DAY>, STOCHASTIC_DEMAND>, POPULATION>& x4() const { return _x4; }
	const std::array<std::array<std::array<std::array<int, TASK>, DAY>, STOCHASTIC_DEMAND>, POPULATION>& y4() const { return _y4; }

	std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET >, DAY>, POPULATION>& x1() { return _x1; }
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, POPULATION>& y1() { return _y1; }
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, POPULATION>& v1() { return _v1; }
	std::array<std::array<std::array<std::array<int, STATION>, CAR_TYPE >, DAY>, POPULATION>& v2() { return _v2; }
	std::array<std::array<std::array<int, TASK>, DAY >, POPULATION>& v3() { return _v3; }

	std::array<std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET>, DAY>, STOCHASTIC_DEMAND>, POPULATION>& x2() { return _x2; }
	std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, STOCHASTIC_DEMAND>, POPULATION>& y2() { return _y2; }
	std::array<std::array<std::array<std::array<std::array<int, STATION>, FLEET>, DAY>, STOCHASTIC_DEMAND>, POPULATION>& x3() { return _x3; }
	std::array<std::array<std::array<std::array<int, STATION>, DAY>, STOCHASTIC_DEMAND>, POPULATION>& y3() { return _y3; }
	std::array<std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DAY>, STOCHASTIC_DEMAND>, POPULATION>& x4() { return _x4; }
	std::array<std::array<std::array<std::array<int, TASK>,  DAY>, STOCHASTIC_DEMAND>, POPULATION>& y4() { return _y4; }

private:
	/*50組隨機需求下，月趟次(x1,y1,v1,v2,v3) use other model to generate(totally have 50 set)*/

	std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET >, DAY>, POPULATION> _x1;
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, POPULATION> _y1;
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, POPULATION> _v1;
	std::array<std::array<std::array<std::array<int, STATION>, CAR_TYPE>, DAY>, POPULATION> _v2;
	std::array<std::array<std::array<int, TASK>, DAY >, POPULATION> _v3;

	std::array<std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET >, DAY>, STOCHASTIC_DEMAND>, POPULATION> _x2;
	std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, STOCHASTIC_DEMAND>, POPULATION> _y2;
	std::array<std::array<std::array<std::array<std::array<int, STATION>, FLEET>, DAY>, STOCHASTIC_DEMAND>, POPULATION> _x3;
	std::array<std::array<std::array<std::array<int, STATION>, DAY>, STOCHASTIC_DEMAND>, POPULATION> _y3;
	std::array<std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DAY>, STOCHASTIC_DEMAND>, POPULATION> _x4;
	std::array<std::array<std::array<std::array<int, TASK>, DAY>, STOCHASTIC_DEMAND>, POPULATION> _y4;
};