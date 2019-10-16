/**
 * 使用示例
 * 判题地址:https://www.luogu.org/problem/P4929
 * 题目描述
 * 给定一个N行M列的矩阵，矩阵中每个元素要么是1，要么是0
 * 你需要在矩阵中挑选出若干行，使得对于矩阵的每一列j，在你挑选的这些行中，有且仅有一行的第j个元素为1
 * 输入格式
 * 第一行两个数N,M
 * 接下来N行，每行M个数字0或1，表示这个矩阵
 * 输出格式
 * 一行输出若干个数表示答案，两个数之间用空格隔开，输出任一可行方案均可，顺序随意
 * 若无解，输出“No Solution!”（不包含引号）
 */
#include "../IOUtils/IOUtils.h"
#include "DanceLink.h"

using namespace std;
const int MAX_N = 250525;
int n, m;

int main() {
    DanceLinkX danceLink(MAX_N);
    n = IOUtils::read();
    m = IOUtils::read();
    register size_t i, j;
    int f;
    SparseMatrix<bool> sparse;
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= m; j++) {
            f = IOUtils::read();
            if (f) {
                sparse.set(i, j, true);
            }
        }
    }
    vector<size_t> ans = danceLink.dance((size_t) n, (size_t) m, sparse);
    if (ans.empty()) {
        printf("No Solution!");
    } else {
        for (auto each : ans) {
            printf("%u ", each);
        }
        putchar('\n');
    }
    return 0;
}
