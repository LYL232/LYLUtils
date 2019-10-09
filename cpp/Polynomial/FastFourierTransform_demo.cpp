/**
 * 使用示例:
 * 判题网址: https://www.luogu.org/problem/P1919
 * 题目描述
 * 给出两个n位10进制整数x和y，你需要计算x*y。
 * 输入格式
 * 第一行一个正整数n。 第二行描述一个位数为n的正整数x。 第三行描述一个位数为n的正整数y。
 * 输出格式
 * 输出一行，即x*y的结果。（注意判断前导0）
 * 输入输出样例
 * 输入
 * 1
 * 3
 * 4
 * 输出
 * 12
 * 说明/提示
 * 数据范围：
 * n<=60000
 */

#include<bits/stdc++.h>
#include "../Polynomial/Polynomial.h"
#include "../Complex/Complex.h"
#include "../IOUtils/IOUtils.h"
#include "../Polynomial/FastFourierTransformer.h"

#define MAX_N 200025

char s1[MAX_N], s2[MAX_N];
int n, cnt = 0;

ArrayPolynomial<int> a(MAX_N), b(MAX_N), ans(MAX_N);

int main() {
    n = IOUtils::read();
    scanf("%s%s", s1, s2);
    for (int i = n - 1; i >= 0; i--) {
        a.coe[cnt] = s1[i] - '0';
        b.coe[cnt++] = s2[i] - '0';
    }
    a.degree = b.degree = (size_t) n;
    FastFourierTransformer<int> fastFourierTransform(MAX_N);
    fastFourierTransform.transform(a, b, ans);
    for (int i = 0; i <= ans.degree; i++) {
        if (ans.coe[i] >= 10) {
            ans.coe[i + 1] += ans.coe[i] / 10;
            ans.coe[i] %= 10;
            ans.degree += (i == ans.degree);
        }
    }
    for (int i = ans.degree; i >= 0; --i) {
        printf("%d", ans.coe[i]);
    }
    putchar('\n');
    return 0;
}