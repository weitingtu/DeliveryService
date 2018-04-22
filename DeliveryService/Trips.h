#pragma once
#include "defines.h"
#include <array>
#include <vector>
#include <string>

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

	const std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT >, FLEET>, DAY>, POPULATION>& x2() const { return _x2; }
	const std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, POPULATION>& y2() const { return _y2; }
	const std::array<std::array<std::array<std::array<int, STATION>, FLEET>, DAY>, POPULATION>& x3() const { return _x3; }
	const std::array<std::array<std::array<int, STATION>, DAY>, POPULATION>& y3() const { return _y3; }
	const std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DAY>, POPULATION>& x4() const { return _x4; }
	const std::array<std::array<std::array<int, TASK>, DAY>, POPULATION>& y4() const { return _y4; }

	std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET >, DAY>, POPULATION>& x1() { return _x1; }
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, POPULATION>& y1() { return _y1; }
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, POPULATION>& v1() { return _v1; }
	std::array<std::array<std::array<std::array<int, STATION>, CAR_TYPE >, DAY>, POPULATION>& v2() { return _v2; }
	std::array<std::array<std::array<int, TASK>, DAY >, POPULATION>& v3() { return _v3; }

	std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET>, DAY>, POPULATION>& x2() { return _x2; }
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, POPULATION>& y2() { return _y2; }
	std::array<std::array<std::array<std::array<int, STATION>, FLEET>, DAY>, POPULATION>& x3() { return _x3; }
	std::array<std::array<std::array<int, STATION>, DAY>, POPULATION>& y3() { return _y3; }
	std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DAY>, POPULATION>& x4() { return _x4; }
	std::array<std::array<std::array<int, TASK>,  DAY>, POPULATION>& y4() { return _y4; }

private:
	/*50組隨機需求下，月趟次(x1,y1,v1,v2,v3) use other model to generate(totally have 50 set)*/

	std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET >, DAY>, POPULATION> _x1;
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, POPULATION> _y1;
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, POPULATION> _v1;
	std::array<std::array<std::array<std::array<int, STATION>, CAR_TYPE>, DAY>, POPULATION> _v2;
	std::array<std::array<std::array<int, TASK>, DAY >, POPULATION> _v3;

	std::array<std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, FLEET >, DAY>, POPULATION> _x2;
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, POPULATION> _y2;
	std::array<std::array<std::array<std::array<int, STATION>, FLEET>, DAY>, POPULATION> _x3;
	std::array<std::array<std::array<int, STATION>, DAY>, POPULATION> _y3;
	std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DAY>, POPULATION> _x4;
	std::array<std::array<std::array<int, TASK>, DAY>, POPULATION> _y4;
};
