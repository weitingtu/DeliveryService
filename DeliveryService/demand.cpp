#include "demand.h"

Demand::Demand():
_c1(),
_c2(),
_b1(),
_b2(),
_a1(),
_c3({ { 27500, 51500 } }),
_b3({ { 11000, 20600 } }),
_c4({ { 21500, 21500 } }),
_b4({ { 8600, 8600 } }),
_a2({ { 8000, 15000 } }),
_a3({ { 6200, 6200 } }),
_load({ { 14, 20 } }),
_u1(),
_u2({ { 2, 4 } }),
_u3({ { 1.11, 1.11 } }),
_d1(),
_d2(),
_d3(),
_sum_task_demand(),
_mean_task_demand(),
_std_task_demand(),
_sum_transfer_majorcustomer(),
_mean_transfer_majorcustomer(),
_std_transfer_majorcustomer(),
_sum_transfer_demand(),
_mean_transfer_demand(),
_std_transfer_demand(),
_pro({ { 1.0 / SCENARIO } })
{
}


Demand::~Demand()
{
}
