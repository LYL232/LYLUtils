#include<bits/stdc++.h>
#include "ArrayUtils.h"

const int MAX_N = (int) 1e5 + 25;

/**
 * 使用示例:离散化数组, 下标从1开始
 */
void demo_discretize() {
    int init[] = {0, 1, 12, 13, 4, 6, 7, 6, 6, 12, 2, 2, 3, 4, 5}, *res = new int[15];
    ArrayUtils<int> arrayUtils(init, 14);
    arrayUtils.discretize(res);
    for (int i = 1; i <= 14; ++i) {
        printf("%d ", res[i]);
    }
    putchar('\n');
    delete (res);
}

/**
 * 使用示例: 静态 查询区间第k小,下标从1开始
 * 判题地址:https://www.luogu.org/problem/P3834
 */
void demo_query_k_min() {
    int init[] = {0, 1, 12, 13, 4, 6, 7, 6, 6, 12, 2, 2, 3, 4, 5};
    ArrayUtils<int> arrayUtil(init, 14);
    printf("%d\n", arrayUtil.query_k_min(1, 3, 1));
    printf("%d\n", arrayUtil.query_k_min(5, 10, 4));
}

int main() {
    demo_discretize();
    demo_query_k_min();
    return 0;
}

