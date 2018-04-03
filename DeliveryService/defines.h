#pragma once

#define day 31         // t 規劃日期 
#define district 4     // j 區域 
#define fleet 8        // i 自有車數 
#define task 2         // k 任務:集貨&繼送 
#define type 3         // n 套次1.2.3 
#define station 2      // m 其他轉運中心數  

#define scenario 50    // s 隨機情況 
#define population 50
#define truck 2        // 車型17ton truck & container truck

#define maxworktime 12 // 最大可工作時間 
#define worktime 0.5   // 站所取送貨時間  hr

#define cost_temporary_outsourcing17 2   // 外包17噸貨車臨時配送成本倍率
#define cost_temporary_17 2.5            // 自有車輛臨時配送成本倍率
#define cost_outsourcing_container 1.45  // 外包貨櫃車於區域任務之成本倍率(自有車*1.45) a1jk