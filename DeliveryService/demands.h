#pragma once
#include "defines.h"
#include <array>
#include <vector>
#include <string>

class Demand
{
public:
	Demand();
	~Demand();

	/* 運量*/
	const std::array<std::array<double, TASK>, DISTRICT>& d1() const { return _d1; } // 在scenerio s下,第 t day, 區域j中,任務k的配送量 
	const std::array<double, STATION>& d2() const { return _d2; }                    // 在scenerio s下,第 t day,與其他轉運中心m的配送量
	const std::array<double, TASK>& d3() const { return _d3; }                       // 在scenerio s下,第 t day,大客戶任務k的配送量
	std::array<std::array<double, TASK>, DISTRICT>& d1() { return _d1; } // 在scenerio s下,第 t day, 區域j中,任務k的配送量 
	std::array<double, STATION>& d2() { return _d2; }                    // 在scenerio s下,第 t day,與其他轉運中心m的配送量
	std::array<double, TASK>& d3() { return _d3; }                       // 在scenerio s下,第 t day,大客戶任務k的配送量

	friend class Demands;
private:
	/* 運量*/
	std::array<std::array<double, TASK>, DISTRICT> _d1; // 在scenerio s下,第 t day, 區域j中,任務k的配送量 
	std::array<double, STATION> _d2;                    // 在scenerio s下,第 t day,與其他轉運中心m的配送量
	std::array<double, TASK> _d3;                       // 在scenerio s下,第 t day,大客戶任務k的配送量
};

class Demands
{
public:
	Demands();
	~Demands();


	void read();

	/*cost*/
	const std::array<std::array<double, TASK>, DISTRICT>& c1() const { return _c1; } // c1[j][k] monthly cost of self truck   (file: input_cost_varcost)
	const std::array<std::array<double, TASK>, DISTRICT>& c2() const { return _c2; } // c2=c1*2.5 (stochastic cost)
	const std::array<std::array<double, TASK>, DISTRICT>& b1() const { return _b1; } // monthlycost of outsourcing 17 truck   (file: input_cost_outsourcing17)
	const std::array<std::array<double, TASK>, DISTRICT>& b2() const { return _b2; } // b1*2 (stochastic cost)
	const std::array<std::array<double, TASK>, DISTRICT>& a1() const { return _a1; } // monthly cost of outsourcing container truck   (b1*1.45)

	/* stochastic cost */
	const std::array<double, STATION>& c3() const { return _c3; }
	const std::array<double, STATION>& b3() const { return _b3; }
	const std::array<double, TASK>   & c4() const { return _c4; }
	const std::array<double, TASK>   & b4() const { return _b4; }
	const std::array<double, STATION>& a2() const { return _a2; } // monthly cost of outsourcing container truck to another transshipment center  
	const std::array<double, TASK>   & a3() const { return _a3; } // 外包貨櫃車配送大型客戶 之任務 k 一趟次之費用

	/* capacity*/
	const std::array<double, TRUCK>& load() const { return _load; } // 17ton&container的容量


	/*travel time*/
	const std::array<std::array<double, TASK>, DISTRICT>& u1() const { return _u1; } // travel time and operating time for serving district j and task k  (file:input_traveltime)
	const std::array<double, STATION>& u2() const { return _u2; }                    // travel time and operating time for serving another transshipment center m    
	const std::array<double, TASK>& u3() const{ return _u3; }                        // travel time and operating time for serving task k for major customer


	/* 運量*/
	const std::vector<std::vector<Demand> >& demands() const { return _demands; }

	/*產生隨機需求*/
	const std::array<std::array<double, TASK>, DISTRICT>& sum_task_demand()  const { return _sum_task_demand; } //district task
	const std::array<std::array<double, TASK>, DISTRICT>& mean_task_demand() const { return _mean_task_demand; }
	const std::array<std::array<double, TASK>, DISTRICT>& std_task_demand()  const { return _std_task_demand; }

	const std::array<double, TASK>& sum_transfer_majorcustomer()  const { return _sum_transfer_majorcustomer; }
	const std::array<double, TASK>& mean_transfer_majorcustomer() const { return _mean_transfer_majorcustomer; }
	const std::array<double, TASK>& std_transfer_majorcustomer()  const { return _std_transfer_majorcustomer; }

	const std::array<double, STATION>& sum_transfer_demand()  const { return _sum_transfer_demand; }
	const std::array<double, STATION>& mean_transfer_demand() const { return _mean_transfer_demand; }
	const std::array<double, STATION>& std_transfer_demand()  const { return _std_transfer_demand; }


	const std::array<double, 1>& pro() const { return _pro; }
	
private:
	
	bool _read_var_cost(const std::string& input_cost_varcost);
	bool _read_outsourcing_cost(const std::string & input_cost_outsourcing17);
	bool _read_traveltime(const std::string& input_traveltime);
	bool _read_d1(const std::string& input_demand_task_collection, const std::string& input_demand_task_sending);
	bool _read_d2(const std::string& input_demand_transfer);
	bool _read_d3(const std::string& input_demand_transfer_majorcoutomer);
	void _generate_stochastic_parameter();
	void _generate_stochastic_demand();
	

	/*cost*/
	std::array<std::array<double, TASK>, DISTRICT> _c1; // c1[j][k] monthly cost of self truck   (file: input_cost_varcost)
	std::array<std::array<double, TASK>, DISTRICT> _c2; // c2=c1*2.5 (stochastic cost)
	std::array<std::array<double, TASK>, DISTRICT> _b1; // monthlycost of outsourcing 17 truck   (file: input_cost_outsourcing17)
	std::array<std::array<double, TASK>, DISTRICT> _b2; // b1*2 (stochastic cost)
	std::array<std::array<double, TASK>, DISTRICT> _a1; // monthly cost of outsourcing container truck   (b1*1.45)

	/* stochastic cost */
	std::array<double, STATION> _c3;
	std::array<double, STATION> _b3;
	std::array<double, TASK> _c4;
	std::array<double, TASK> _b4;
	std::array<double, STATION> _a2; // monthly cost of outsourcing container truck to another transshipment center  
	std::array<double, TASK> _a3;    // 外包貨櫃車配送大型客戶 之任務 k 一趟次之費用
									 /* capacity*/
	std::array<double, TRUCK> _load;  // 17ton&container的容量


									  /*travel time*/
	std::array<std::array<double, TASK>, DISTRICT> _u1; // travel time and operating time for serving district j and task k  (file:input_traveltime)
	std::array<double, STATION> _u2;                    // travel time and operating time for serving another transshipment center m    
	std::array<double, TASK> _u3;                       // travel time and operating time for serving task k for major customer


	/* 運量*/
	std::vector<std::vector<Demand> > _demands; 
																			/*產生隨機需求*/
	std::array<std::array<double, TASK>, DISTRICT> _sum_task_demand;  //district task
	std::array<std::array<double, TASK>, DISTRICT> _mean_task_demand;
	std::array<std::array<double, TASK>, DISTRICT> _std_task_demand;

	std::array<double, TASK> _sum_transfer_majorcustomer;
	std::array<double, TASK> _mean_transfer_majorcustomer;
	std::array<double, TASK> _std_transfer_majorcustomer;

	std::array<double, STATION> _sum_transfer_demand;
	std::array<double, STATION> _mean_transfer_demand;
	std::array<double, STATION> _std_transfer_demand;


	std::array<double, 1> _pro;
};

