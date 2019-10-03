/**
 * 使用示例:
 * 判题网址: https://www.luogu.org/problem/P3372
 * 题目描述
 * 如题，已知一个数列，你需要进行下面两种操作：
 * 1.将某区间每一个数加上x
 * 2.求出某区间每一个数的和
 * 输入格式
 * 第一行包含两个整数N、M，分别表示该数列数字的个数和操作的总个数。
 * 第二行包含N个用空格分隔的整数，其中第i个数字表示数列第i项的初始值。
 * 接下来M行每行包含3或4个整数，表示一个操作，具体如下：
 * 操作1： 格式：1 x y k 含义：将区间[x,y]内每个数加上k
 * 操作2： 格式：2 x y 含义：输出区间[x,y]内每个数的和
 * 输出格式
 * 输出包含若干行整数，即为所有操作2的结果。
 */
#include <cstdio>
#include"SegmentTree.h"

const int MAX_N = (int) 1e5 + 25;
int n, m, x, y, op;
long long a[MAX_N], k;

SegmentTree<long long, MAX_N> seg_tree;

int main() {
   scanf("%d %d", &n, &m);
   for(int i = 1 ; i <= n; ++i) {
       scanf("%lld", a + i);
   }
   seg_tree.build(n, a);
   while(m--) {
       scanf("%d", &op);
       if(op == 1) {
           scanf("%d %d %lld", &x, &y, &k);
           if(x==y) {
               seg_tree.update(x, k);
           } else {
               seg_tree.update(x, y, k);
           }
       } else {
           scanf("%d %d", &x, &y);
           printf("%lld\n", seg_tree.query(x, y));
       }
   }
   seg_tree.clear(); //多组数据时用clear函数
    return 0;
}
