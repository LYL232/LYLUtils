#ifndef LYLUTILS_BIT_H
#define LYLUTILS_BIT_H
/*
 * 树状数组模板
 *  标签: 树状数组
 * */
#include<bits/stdc++.h>
using namespace std;

// 最大能处理数组大小
const int BIT_MAX_LEN = (int)1e5 +25;
class BIT {
public:
    //清除树中所有数据
    void clear() {
        memset(_tree, 0, sizeof(_tree));
        _n = 0;
    }

    // 初始化树: n:需要维护的数组大小
    void init(int __n) {
        clear();
        _n = __n;
    }

    //向位置x中插入数字k
    // 1 <= x <= n
    void insert(int _x,int _k) {
        while(_x <= _n) {
            _tree[_x] = _k;
            _x += low_bit(_x);
        }
    }

    //向位置x的数字追加数字k;
    // 1 <= x <= n
    void add(int _x,int _k) {
        while(_x <= _n) {
            _tree[_x] += _k;
            _x += low_bit(_x);
        }
    }

    //求出[a, b] 的区间和 a <= b 可以用于求某个点的数字值
    int seg_sum(int _a, int _b) {
        return sum(_b) - sum(_a - 1);
    }

private:
    // 树状数组(binary index tree)
    // 功能: 维护数组区间和
    int _tree[BIT_MAX_LEN + 1], _n;
    int low_bit(int x) {
        return x & (-x);
    }

    // 求出区间[1, x]的所有已插入数字的和
    int sum(int _x) {
        int _ans=0;
        while(_x) {
            _ans += _tree[_x];
            _x -= low_bit(_x);
        }
        return _ans;
    }
};
#endif //LYLUTILS_BIT_H
