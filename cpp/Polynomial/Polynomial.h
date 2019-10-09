#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_POLYNOMIAL_H
#define LYL_UTILS_POLYNOMIAL_H

#include <cstring>
#include <malloc.h>

/**
 * 数组表示多项式,次数连续连续,没有的项以系数0表示
 * 多项式系数由0~degree, degree为多项式的最高次幂
 * 各项对应系数为coe[i]     0 <= i <= degree
 * @tparam NUM_TYPE 系数的类型
 * @tparam MAX_TERMS 计算的最高次幂
 */
template<typename NUM_TYPE = int>
class ArrayPolynomial {
public:
    size_t degree;
    NUM_TYPE *coe;

    /**
     * 数组表示多项式构造函数,必须传入一个能够处理的最大长度参数,用于申请数组内存
     * @param max_process_len 能够处理的最大长度
     */
    explicit ArrayPolynomial(size_t max_process_len) {
        size_t max_size = max_process_len * sizeof(NUM_TYPE);
        coe = (NUM_TYPE *) malloc(max_size);
        memset(coe, 0, max_size);
        degree = 0;
    }

    ~ArrayPolynomial() {
        free(coe);
    }
};


#endif //LYL_UTILS_POLYNOMIAL_H

#pragma clang diagnostic pop