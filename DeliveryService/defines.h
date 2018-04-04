#pragma once

#define DAY 31         // t 規劃日期 
#define DISTRICT 4     // j 區域 
#define FLEET 8        // i 自有車數 
#define TASK 2         // k 任務:集貨&繼送 
#define CAR_TYPE 3     // n 套次1.2.3 
#define STATION 2      // m 其他轉運中心數  

#define SCENARIO 50    // s 隨機情況 
#define POLULATION 50
#define TRUCK 2        // 車型17ton truck & container truck

#define MAXWORKTIME 12 // 最大可工作時間 
#define WORKTIME 0.5   // 站所取送貨時間  hr

#define COST_TEMPORARY_OUTSOURCING17 2   // 外包17噸貨車臨時配送成本倍率
#define COST_TEMPORARY_17 2.5            // 自有車輛臨時配送成本倍率
#define COST_OUTSOURCING_CONTAINER 1.45  // 外包貨櫃車於區域任務之成本倍率(自有車*1.45) a1jk

#include <array>

template <class T, size_t I, size_t... J>
struct MultiDimArray
{
	using Nested = typename MultiDimArray<T, J...>::type;
	// typedef typename MultiDimArray<T, J...>::type Nested;
	using type = std::array<Nested, I>;
	// typedef std::array<Nested, I> type;
};

template <class T, size_t I>
struct MultiDimArray<T, I>
{
	using type = std::array<T, I>;
	// typedef std::array<T, I> type;
};