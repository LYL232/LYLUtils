#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_IOUTILS_H
#define LYL_UTILS_IOUTILS_H

#include <cstdio>
#include <iostream>
using namespace std;

class IOUtils {
public:
    /**
     * 快速读取标准输入的整数值
     * @return 读取的整数值
     * demo:
     *      int x = IOUtils::read();
     */
    static inline int read() {
        register int x = 0, f = 1;
        auto s = (char) getchar();
        while (s < '0' || s > '9') {
            if (s == '-') {
                f = -1;
            }
            s = (char) getchar();
        }
        while (s >= '0' && s <= '9') {
            x = x * 10 + s - '0';
            s = (char) getchar();
        }
        return x * f;
    }

    static inline void cpp_io_tie() {
        ios::sync_with_stdio(false);
        cin.tie();
        cout.tie();
    }
};

#endif //LYL_UTILS_IOUTILS_H

#pragma clang diagnostic pop