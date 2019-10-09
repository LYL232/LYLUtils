#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_FASTFOURIERTRANSFROM_H
#define LYL_UTILS_FASTFOURIERTRANSFROM_H

#include <malloc.h>
#include <cmath>
#include "Polynomial.h"
#include "../Complex/Complex.h"

template<typename NUM_TYPE, typename COMPLEX_NUM_TYPE = double>
class FastFourierTransformer {
    typedef ArrayPolynomial<NUM_TYPE> CALCULATE_TYPE;
public:
    explicit FastFourierTransformer(size_t max_process_len) {
        size_t max_complex_size = max_process_len * sizeof(Complex<COMPLEX_NUM_TYPE>);
        pa = (Complex<COMPLEX_NUM_TYPE> *) malloc(max_complex_size);
        pb = (Complex<COMPLEX_NUM_TYPE> *) malloc(max_complex_size);
        bit_revers = (size_t *) malloc(max_process_len * sizeof(size_t));
        process_len = 0;
    }

    ~FastFourierTransformer() {
        free(pa);
        free(pb);
        free(bit_revers);
    }

    /**
     * 计算两个多项式a,b的卷积, 将结果保存在res中
     * @param a 多项式
     * @param b 多项式
     * @param res 保存结果的多项式
     */
    void transform(const CALCULATE_TYPE &a, const CALCULATE_TYPE &b, CALCULATE_TYPE &res) {
        register size_t i, l = 0, max_degree = a.degree > b.degree ? a.degree : b.degree;
        // 初始化点值表示法(复制系数表示法)
        for (i = 0; i <= max_degree; ++i) {
            pa[i].r = a.coe[i];
            pb[i].r = b.coe[i];
        }
        // 找大于max_degree的最近的2的n次方,即处理的长度
        process_len = 1;
        while (process_len < (max_degree << 1)) {
            process_len <<= 1;
            l++;// 用于求反转二进制的临时变量
        }
        for (i = 0; i <= process_len; ++i) {
            bit_revers[i] = (bit_revers[i >> 1] >> 1) | ((i & 1) << (l - 1));
        }
        // 系数表示转化为点值表示
        FFT(pa, 1);
        FFT(pb, 1);
        // 计算点值表示法的乘法
        for (i = 0; i <= process_len; i++) {
            pa[i] = pa[i] * pb[i];
        }
        // 点值表示转化为系数表示
        FFT(pa, -1);
        for (i = 0; i <= process_len; ++i) {
            res.coe[i] += (NUM_TYPE) (pa[i].r / process_len + 0.5);
        }
        while (!res.coe[process_len]) {
            process_len--;
        }
        res.degree = process_len;
    }

private:
    // 点值表示的多项式a 和 b
    Complex<COMPLEX_NUM_TYPE> *pa, *pb;
    // 临时数组用于存放反二进制
    size_t *bit_revers, process_len;

    /**
     * 将一个系数表示法的多项式表示为点值表示的多项式(正变换)
     * 或将一个点值表示法的多项式表示为系数表示的多项式(逆变换)
     * @param serial 处理的序列
     * @param flag 正变换为1, 逆变换为-1
     */
    void FFT(Complex<COMPLEX_NUM_TYPE> *serial, COMPLEX_NUM_TYPE flag) {
        register size_t i, j, k, l;
        const COMPLEX_NUM_TYPE pi = 3.14159265358;
        Complex<COMPLEX_NUM_TYPE> temp, iter;
        for (i = 0; i < process_len; i++) {
            if (i < bit_revers[i]) {
                temp.copy(serial[i]);
                serial[i].copy(serial[bit_revers[i]]);
                serial[bit_revers[i]].copy(temp);
            }
        }
        for (j = 1; j < process_len; j <<= 1) {
            temp.assign(cos(pi / j), flag * sin(pi / j));
            for (k = 0; k < process_len; k += (j << 1)) {
                iter.assign(1, 0);
                for (l = 0; l < j; l++, iter = iter * temp) {
                    Complex<COMPLEX_NUM_TYPE> Nx = serial[k + l], Ny = iter * serial[k + j + l];
                    serial[k + l] = Nx + Ny;
                    serial[k + j + l] = Nx - Ny;
                }
            }
        }
    }
};


#endif //LYL_UTILS_FASTFOURIERTRANSFROM_H

#pragma clang diagnostic pop