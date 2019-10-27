#include <cstdio>

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
#include "Dijkstra.h"
const int MAX_N = 100010, MAX_M = 500010;
size_t n, m, s;
void demo_dijkstra() {
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
}

int main() {
    demo_dijkstra();
    return 0;
}

