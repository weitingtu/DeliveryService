#pragma once
#include "defines.h"
#include <array>

class Gurobi
{
public:
	Gurobi();
	~Gurobi();

	void read();

private:
	bool _read_var_cost(const std::string& file_name);
	bool _read_outsourcing_cost(const std::string & file_name);
	void _read_traveltime();
	void _read_d1();
	void _read_d2();
	void _read_d3();
	void _read_num_x1();
	void _read_num_y1();
	void _read_num_v1();
	void _read_num_v2();
	void _read_num_v3();
	void _generate_stochastic_parameter();
	void _generate_stochastic_demand();


	/*cost*/
	std::array<std::array<double, TASK>, DISTRICT> _c1; // c1[j][k] monthly cost of self truck   (file: input_cost_varcost)
	std::array<std::array<double, TASK>, DISTRICT> _c2; // c2=c1*2.5 (stochastic cost)
	std::array<std::array<double, TASK>, DISTRICT> _b1; // monthlycost of outsourcing 17 truck   (file: input_cost_outsourcing17)
	std::array<std::array<double, TASK>, DISTRICT> _b2; // b1*2 (stochastic cost)
	std::array<std::array<double, TASK>, DISTRICT> _a1; // monthly cost of outsourcing container truck   (b1*1.45)



	/* stochastic cost */
	std::array<double, STATION> _c3 = { 27500, 51500 };
	std::array<double, STATION> _b4 = { 11000, 20600 };
	std::array<double, TASK> _c4 = { 21500, 21500 };
	std::array<double, TASK> _b6 = { 8600, 8600 };
	std::array<double, STATION> _a2 = { 8000, 15000 }; // monthly cost of outsourcing container truck to another transshipment center  
	std::array<double, TASK> _a3 = { 6200, 6200 };     // 外包貨櫃車配送大型客戶 之任務 k 一趟次之費用

	/* capacity*/
	std::array<double, TRUCK> load = { 14, 20 }; // 17ton&container的容量


	/*travel time*/
	std::array<std::array<double, TASK>, DISTRICT> _u1; // travel time and operating time for serving district j and task k  (file:input_traveltime)
	std::array<double, STATION> _u2 = { 2, 4 };         // travel time and operating time for serving another transshipment center m    
	std::array<double, 1> _u3 = { 1.11 };               // travel time and operating time for serving task k for major customer


	/* 運量*/
	std::array<std::array<std::array<std::array<double, TASK>, DISTRICT>, DAY>, SCENARIO> _d1; //在scenerio s下,第 t day, 區域j中,任務k的配送量 
	std::array<std::array<std::array<double, STATION>, DAY>, SCENARIO> _d2; // 在scenerio s下,第 t day,與其他轉運中心m的配送量
	std::array<std::array<std::array<double, TASK>, DAY>, SCENARIO> _d3;    // 在scenerio s下,第 t day,大客戶任務k的配送量

	/*產生隨機需求*/
	std::array < std::array<double, TASK>, DISTRICT> sum_task_demand;  //district task
	std::array<std::array<double, TASK>, DISTRICT> mean_task_demand;
	std::array<std::array<double, TASK>, DISTRICT> std_task_demand;

	std::array<double, TASK> sum_transfer_majorcustomer;
	std::array<double, TASK> mean_transfer_majorcustomer;
	std::array<double, TASK> std_tranfer_majorcustomer;

	std::array<double, STATION> sum_transfer_demand;
	std::array<double, STATION> mean_transfer_demand;
	std::array<double, STATION> std_transfer_demand;


	std::array<double, 1> pro = { 1.0 / SCENARIO }; // probability (日趟次期望值)

	/*50組隨機需求下，月趟次(x1,y1,v1,v2,v3) use other model to generate(totally have 50 set)*/

	std::array<std::array<std::array<std::array<std::array<int, TASK>, FLEET>, DISTRICT>, DAY>, SCENARIO> num_x1; // (file:X1)
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, SCENARIO> num_y1;                    // (file:Y1)
	std::array<std::array<std::array<std::array<int, TASK>, DISTRICT>, DAY>, SCENARIO> num_v1;                    // (file:V1)
	std::array<std::array<std::array<std::array<int, CAR_TYPE>, STATION>, DAY>, SCENARIO> num_v2;                 // (file:v2)
	std::array<std::array<std::array<int, TASK>, DAY >, SCENARIO> num_v3;                                         // (file:v3)

};