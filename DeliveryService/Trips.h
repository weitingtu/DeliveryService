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

	const std::array<std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DISTRICT>, DAY>, SCENARIO>& x1() const { return _x1; }
	const std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, SCENARIO>& y1() const { return _y1; }
	const std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, SCENARIO>& v1() const { return _v1; }
	const std::array<std::array<std::array<std::array<int, CAR_TYPE>, STATION>, DAY>, SCENARIO>& v2() const { return _v2; }
	const std::array<std::array<std::array<int, TASK>, DAY >, SCENARIO>& v3() const { return _v3; }

	const std::array<std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DISTRICT>, DAY>, SCENARIO>& x2() const { return _x2; }
	const std::array<std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DISTRICT>, DAY>, SCENARIO>& y2() const { return _y2; }
	const std::array<std::array<std::array<std::array<int, CAR_TYPE>, STATION>, DAY>, SCENARIO>& x3() const { return _x3; }
	const std::array<std::array<std::array<std::array<int, CAR_TYPE>, STATION>, DAY>, SCENARIO>& y3() const { return _y3; }
	const std::array<std::array<std::array<int, TASK>, DAY >, SCENARIO>& x4() const { return _x4; }
	const std::array<std::array<std::array<int, TASK>, DAY >, SCENARIO>& y4() const { return _y4; }

	std::array<std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DISTRICT>, DAY>, SCENARIO>& x1() { return _x1; }
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, SCENARIO>& y1() { return _y1; }
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, SCENARIO>& v1() { return _v1; }
	std::array<std::array<std::array<std::array<int, CAR_TYPE>, STATION>, DAY>, SCENARIO>& v2() { return _v2; }
	std::array<std::array<std::array<int, TASK>, DAY >, SCENARIO>& v3() { return _v3; }

	std::array<std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DISTRICT>, DAY>, SCENARIO>& x2() { return _x2; }
	std::array<std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DISTRICT>, DAY>, SCENARIO>& y2() { return _y2; }
	std::array<std::array<std::array<std::array<int, CAR_TYPE>, STATION>, DAY>, SCENARIO>& x3() { return _x3; }
	std::array<std::array<std::array<std::array<int, CAR_TYPE>, STATION>, DAY>, SCENARIO>& y3() { return _y3; }
	std::array<std::array<std::array<int, TASK>, DAY >, SCENARIO>& x4() { return _x4; }
	std::array<std::array<std::array<int, TASK>, DAY >, SCENARIO>& y4() { return _y4; }

private:
	/*50組隨機需求下，月趟次(x1,y1,v1,v2,v3) use other model to generate(totally have 50 set)*/

	std::array<std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DISTRICT>, DAY>, SCENARIO> _x1; 
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, SCENARIO> _y1;                   
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, SCENARIO> _v1;                   
	std::array<std::array<std::array<std::array<int, CAR_TYPE>, STATION>, DAY>, SCENARIO> _v2;                
	std::array<std::array<std::array<int, TASK>, DAY >, SCENARIO> _v3;                                        

	std::array<std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DISTRICT>, DAY>, SCENARIO> _x2; 
	std::array<std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DISTRICT>, DAY>, SCENARIO> _y2; 
	std::array<std::array<std::array<std::array<int, CAR_TYPE>, STATION>, DAY>, SCENARIO> _x3;                
	std::array<std::array<std::array<std::array<int, CAR_TYPE>, STATION>, DAY>, SCENARIO> _y3;                
	std::array<std::array<std::array<int, TASK>, DAY >, SCENARIO> _x4;                                        
	std::array<std::array<std::array<int, TASK>, DAY >, SCENARIO> _y4;                                        
};
