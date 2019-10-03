/**
 * 使用示例:
 * 判题网址: https://www.luogu.org/problem/P3374
 *如题，已知一个数列，你需要进行下面两种操作：
 * 1.将某一个数加上x
 * 2.求出某区间每一个数的和
 * 输入格式
 * 第一行包含两个整数N、M，分别表示该数列数字的个数和操作的总个数。
 * 第二行包含N个用空格分隔的整数，其中第i个数字表示数列第i项的初始值。
 * 接下来M行每行包含3个整数，表示一个操作，具体如下：
 * 操作1： 格式：1 x k 含义：将第x个数加上k
 * 操作2： 格式：2 x y 含义：输出区间[x,y]内每个数的和
 * 输出格式
 * 输出包含若干行整数，即为所有操作2的结果。
 */
#include <cstdio>
#include "BinaryIndexTree.h"

const int MAX_N = (int) 5e5 + 25;
int n, m, x, y, k;
BinaryIndexTree<long long, MAX_N> binaryIndexTree;

int main() {
   scanf("%d %d", &n, &m);
   binaryIndexTree.init(n);
   for (int i = 1; i <= n; ++i) {
       scanf("%d", &x);
       binaryIndexTree.insert(i, x);
   }
   while (m--) {
       scanf("%d %d %d", &k, &x, &y);
       if (k == 1) {
           binaryIndexTree.add(x, y);
       } else {
           printf("%lld\n", binaryIndexTree.sum(x, y));
       }
   }
   return 0;
};