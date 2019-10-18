#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_QUERYPERFORMANCETIMER_H
#define LYL_UTILS_QUERYPERFORMANCETIMER_H
#include <winbase.h>
/**
 * 基于WIN32API的高精度计时器
 */
class QueryPerformanceTimer {

public:

    QueryPerformanceTimer() {
        QueryPerformanceFrequency(&freq);
    }

    /**
     * 开始计时
     */
    void start() {
        QueryPerformanceCounter(&last);
    }

    /**
     * 返回与上一个start()调用的时间差, 单位微秒
     * @return 微秒时间差
     */
    __int64 count_in_micro() {
        QueryPerformanceCounter(&now);
        return (((now.QuadPart - last.QuadPart) * 1000000) / freq.QuadPart);
    }

private:
    LARGE_INTEGER freq, last, now;
};

#endif //LYL_UTILS_QUERYPERFORMANCETIMER_H

#pragma clang diagnostic pop