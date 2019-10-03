#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef LYL_UTILS_BINARYINDEXTREE_H
#define LYL_UTILS_BINARYINDEXTREE_H


#include <cstring>

/**
 * 树状数组:
 * 实现功能:动态修改序列,并维护序列区间和
 * 区间范围:[1, len]
 * 时间复杂度:
 *      修改:O(log(len))
 *      求区间和:O(log(len))
 *      初始化:O(MAX_LEN)
 *  空间复杂度: MAX_LEN + O(1)
 * @tparam NUM_TYPE 运算数类型
 * @tparam MAX_LEN 序列最长的长度
 */
template<typename NUM_TYPE, int MAX_LEN>
class BinaryIndexTree {
public:
    /**
     * 清除树中所有的数据,包括可处理序列长度
     */
    void clear() {
        memset(tree, 0, sizeof(tree));
        len = 0;
    }

    /**
     * 清除序列中的所有的长度,并设置序列长度
     * @param _len 需要处理的序列长度
     */
    void init(int _len) {
        clear();
        len = _len;
    }

    /**
     * 向序列中的位置x 插入数k
     * 要求 1 <= x <= len
     * @param x
     * @param k
     */
    void insert(int x, NUM_TYPE k) {
        while (x <= len) {
            tree[x] = k;
            x += low_bit(x);
        }
    }

    /**
     * 令数中位置为x的数加上k
     * 要求 1 <= x <= len
     * @param x
     * @param k
     */
    void add(int x, NUM_TYPE k) {
        while (x <= len) {
            tree[x] += k;
            x += low_bit(x);
        }
    }

    /**
     * 求区间和:[a, b]
     * 要求 1 <= a <= b <= len
     * @param a
     * @param b
     * @return 区间和:[a, b]
     */
    NUM_TYPE sum(int a, int b) {
        return sum(b) - sum(a - 1);
    }

private:
    NUM_TYPE tree[MAX_LEN + 1];
    int len;

    int low_bit(int x) {
        return x & (-x);
    }

    /**
     * 求出[1, x]的区间和
     * @param x
     * @return [1, x]的区间和
     */
    NUM_TYPE sum(int x) {
        int res = 0;
        while (x) {
            res += tree[x];
            x -= low_bit(x);
        }
        return res;
    }
};

#endif //LYL_UTILS_BINARYINDEXTREE_H

#pragma clang diagnostic pop