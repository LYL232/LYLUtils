/**
 * 使用示例
 * 判题地址:https://www.luogu.org/problem/P3919
 * 题目描述
 * 如题，你需要维护这样的一个长度为 N 的数组，支持如下几种操作
 * 1.在某个历史版本上修改某一个位置上的值
 * 2.访问某个历史版本上的某一位置的值
 * 此外，每进行一次操作（对于操作2，即为生成一个完全一样的版本，不作任何改动），
 * 就会生成一个新的版本。版本编号即为当前操作的编号（从1开始编号，版本0表示初始状态数组）
 *输入格式
 * 输入的第一行包含两个正整数 N, M， 分别表示数组的长度和操作的个数。
 * 第二行包含N个整数，依次为初始状态下数组各位的值（依次为 ai，1≤i≤N）。
 * 接下来M行每行包含3或4个整数，代表两种操作之一（i为基于的历史版本号）：
 * 对于操作1，格式为vi  1  loc_i value_i，即为在版本vi的基础上，将 a_loc_i修改为 value_i
 * 对于操作2，格式为vi  2  loc_i ，即访问版本vi中的 a_loc_i的值，生成一样版本的对象应为vi
 * 输出格式
 * 输出包含若干行，依次为每个操作2的结果。
 */
#include<bits/stdc++.h>
#include "PersistentSegmentTree.h"
#include "../IOUtils/IOUtils.h"

const int MAX_N = (int) 1e6 + 25;

PersistentSegmentTree<int> persistentSegmentTree((size_t) MAX_N);

int n, m, a[MAX_N], ver, loc, value, op, ver_root[MAX_N];

int main() {
    n = IOUtils::read();
    m = IOUtils::read();
    for (int i = 1; i <= n; ++i) {
        a[i] = IOUtils::read();
    }
    ver_root[0] = persistentSegmentTree.init((size_t)n, a);
    for (int i = 1; i <= m; ++i) {
        ver = IOUtils::read();
        op = IOUtils::read();
        if (op == 1) {
            loc = IOUtils::read();
            value = IOUtils::read();
            ver_root[i] = persistentSegmentTree.update((size_t)ver_root[ver], (size_t)loc, value);
        } else {
            loc = IOUtils::read();
            printf("%d\n", persistentSegmentTree.query((size_t)ver_root[ver], (size_t)loc));
            ver_root[i] = ver_root[ver];
        }
    }
    return 0;
}

