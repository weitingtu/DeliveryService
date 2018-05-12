#pragma once

#define DAY 31         // t 規劃日期 
#define DISTRICT 4     // j 區域 
#define FLEET 8        // i 自有車數 
#define TASK 2         // k 任務:集貨&繼送 
#define CAR_TYPE 3     // n 套次1.2.3 
#define STATION 2      // m 其他轉運中心數  

#define POPULATION 50  // s 隨機情況 
//#define STOCHASTIC_DEMAND 50
#define STOCHASTIC_DEMAND 2
#define TRUCK 2        // 車型17ton truck & container truck

#define MAXWORKTIME 12.0 // 最大可工作時間 
#define WORKTIME 0.5     // 站所取送貨時間  hr

#define COST_TEMPORARY_OUTSOURCING17 2   // 外包17噸貨車臨時配送成本倍率
#define COST_TEMPORARY_17 2.5            // 自有車輛臨時配送成本倍率
#define COST_OUTSOURCING_CONTAINER 1.45  // 外包貨櫃車於區域任務之成本倍率(自有車*1.45) a1jk
