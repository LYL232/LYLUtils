/**
 * 使用示例: 迪杰斯特拉算法
 * 判题地址:https://www.luogu.org/problem/P4779
 * 题目描述
 * 给定一个 N 个点，M 条有向边的带非负权图，请你计算从 S 出发，到每个点的距离。
 * 数据保证你能从 S 出发到任意点。
 * 输入格式
 * 第一行为三个正整数 N, M, S。 第二行起 M 行，每行三个非负整数 ui, vi, wi表示从 ui到 vi有一条权值为 wi的边。
 * 输出格式
 * 输出一行 N 个空格分隔的非负整数，表示 S 到每个点的距离。
 */
#ifdef DEMO_DIJKSTRA
#include <cstdio>
#include "Dijkstra.h"
const int MAX_N = 100010, MAX_M = 500010;
size_t n, m, s;
int main() {
    Dijkstra<int, 0x7fffffff> dijkstra(MAX_N, MAX_M);
    scanf("%d%d%d", &n, &m, &s);
    while (m--) {
        register int u, v, d;
        scanf("%d%d%d", &u, &v, &d);
        dijkstra.add_edge((size_t) u, (size_t) v, d);
    }
    const int* res = dijkstra.solve(s);
    for (int i = 1; i <= n; i++) {
        printf("%d ", res[i]);
    }
    return 0;
}
#endif

/**
 * 使用示例: tarjan 算法求强联通分量
 * 题目描述
 * 在幻想乡，上白泽慧音是以知识渊博闻名的老师。
 * 春雪异变导致人间之里的很多道路都被大雪堵塞，
 * 使有的学生不能顺利地到达慧音所在的村庄。
 * 因此慧音决定换一个能够聚集最多人数的村庄作为新的教学地点。
 * 人间之里由N个村庄（编号为1..N）和M条道路组成，
 * 道路分为两种一种为单向通行的，一种为双向通行的，
 * 分别用1和2来标记。如果存在由村庄A到达村庄B的通路，
 * 那么我们认为可以从村庄A到达村庄B，记为(A,B)。
 * 当(A,B)和(B,A)同时满足时，我们认为A,B是绝对连通的，
 * 记为<A,B>。绝对连通区域是指一个村庄的集合，
 * 在这个集合中任意两个村庄X,Y都满足<X,Y>。
 * 现在你的任务是，找出最大的绝对连通区域，
 * 并将这个绝对连通区域的村庄按编号依次输出。
 * 若存在两个最大的，输出字典序最小的，比
 * 如当存在1,3,4和2,5,6这两个最大连通区域时，输出的是1,3,4。
 * 输入格式
 * 第1行：两个正整数N,M
 * 第2..M+1行：每行三个正整数a,b,t, t = 1表示存在从村庄a到b的单向道路，
 * t = 2表示村庄a,b之间存在双向通行的道路。
 * 保证每条道路只出现一次。
 * 输出格式
 * 第1行： 1个整数，表示最大的绝对连通区域包含的村庄个数。
 * 第2行：若干个整数，依次输出最大的绝对连通区域所包含的村庄编号。
 */
#ifdef DEMO_TARJAN
#include<bits/stdc++.h>
#include "Tarjan.h"
using namespace std;

const int MAX_N = 10025, MAX_M = 2 * (int)1e5 + 25;
int n, m;
size_t ans = 0, belongs_to[MAX_N];

int main() {
    Tarjan tarjan((size_t) MAX_N, (size_t) MAX_M);
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; i++) {
        int x, y, f;
        scanf("%d%d%d", &x, &y, &f);
        if (f == 1) {
            tarjan.add_edge((size_t)x, (size_t)y);
        }
        if (f == 2) {
            tarjan.add_edge((size_t)x, (size_t)y);
            tarjan.add_edge((size_t)y, (size_t)x);
        }
    }
    const vector<vector<size_t> > &result = tarjan.solve();
    for (size_t i = 0; i < result.size(); ++i) {
        ans = max(ans, result[i].size());
        for(auto each : result[i]) {
            belongs_to[each] = i;
        }
    }
    printf("%u\n", ans);
    for (size_t i = 1; i <= n; ++i) {
        if (result[belongs_to[i]].size() == ans) {
            size_t now = belongs_to[i];
            for (size_t j = i; j <= n; ++j) {
                if (belongs_to[j] == now) {
                    printf("%u ", j);
                }
            }
            return 0;
        }
    }
    return 0;
}
#endif
