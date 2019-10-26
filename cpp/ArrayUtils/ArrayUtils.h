#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_ARRAYUTILS_H
#define LYL_UTILS_ARRAYUTILS_H

#include <cstring>
#include <algorithm>
#include "../PersistentSegmentTree/PersistentSegmentTree.h"

using namespace std;

/**
 * 数组工具, 实现一些STL中没有但比较常用的功能
 * 非静态函数使用前需调用init初始化函数,会复制一份源序列的拷贝
 * @tparam NUM_TYPE
 * @tparam MAX_LEN
 */
template<typename NUM_TYPE>
class ArrayUtils {
public:
    /**
     * 初始化, 需要指定最大的处理长度
     * @param _array 输入数组指针
     * @param _len
     */
    explicit ArrayUtils(const NUM_TYPE *_array, size_t _len) {
        origin_array = _array;
        len = _len;
        utils = (void ***) malloc(sizeof(void **) * 64);
        memset(utils, 0, sizeof(void **) * 64);
        feature = 0;
    }

    ~ArrayUtils() {
        if (feature & DISCRETIZE) {
            free(utils[DISCRETIZE][0]);
            free(utils[DISCRETIZE]);
        }
        if (feature & QUERY_K_MIN) {
            delete (PersistentSegmentTree<size_t> *) utils[QUERY_K_MIN][0];
            delete (ArrayUtils<NUM_TYPE> *) utils[QUERY_K_MIN][1];
            free(utils[QUERY_K_MIN][2]);
            free(utils[QUERY_K_MIN][3]);
            free(utils[QUERY_K_MIN]);
        }
        free(utils);
    }

    /**
     * 离散化函数:
     * 设原序列去重后有cnt个元素,则该函数将原序列元素根据原序列大小关系
     * 映射到[1,cnt]区间上,并返回至res中,
     * 需先使用init函数
     * 时间复杂度:
     *      O(len * log(len))
     * 空间复杂度: len + O(1)
     * 注意:需使用init(_len)函数复制原序列并指定序列大小
     * @param src 原序列
     * @param res 返回序列
     * @return cnt 非重复元素个数
     */
    size_t discretize(NUM_TYPE *res) {
        if (!(feature & DISCRETIZE)) {
            feature |= DISCRETIZE;
            utils[DISCRETIZE] = (void **) malloc(sizeof(void *));
            utils[DISCRETIZE][0] = malloc(sizeof(NUM_TYPE) * (len + 1));
        }
        auto *array = (NUM_TYPE *) utils[DISCRETIZE][0];
        memcpy(array, origin_array, (len + 1) * sizeof(NUM_TYPE));
        NUM_TYPE *begin_index = array + 1, *end_index = array + 1 + len;
        sort(begin_index, end_index);
        auto cnt = (size_t) (unique(begin_index, end_index) - begin_index);
        for (size_t i = 1; i <= len; ++i) {
            res[i] = (int) (lower_bound(begin_index, begin_index + cnt, origin_array[i]) - begin_index) + 1;
        }
        return cnt;
    }

    /**
     * 用于获取离散化后的数字映射回原序列的数字
     * @param pos 离散化后的数字
     * @return 原序列对应的数字的值
     */
    NUM_TYPE get_undiscretize(size_t pos) const {
        if (!(feature & DISCRETIZE)) {
            return 0;
        }
        // return pos;
        return ((NUM_TYPE *) utils[DISCRETIZE][0])[pos];
    }

    /**
     * 询问区间[l,r]内第k小个元素的值
     * @param l
     * @param r
     * @param k
     * @return
     */
    NUM_TYPE query_k_min(size_t l, size_t r, size_t k) {
        if (!(feature & QUERY_K_MIN)) {
            feature |= QUERY_K_MIN;

            utils[QUERY_K_MIN] = (void **) malloc(sizeof(void *) * 4);
            utils[QUERY_K_MIN][0] = (void *)
                    new PersistentSegmentTree<size_t>(len);
            utils[QUERY_K_MIN][1] = (void *)
                    new ArrayUtils<NUM_TYPE>(origin_array, len);
            utils[QUERY_K_MIN][2] = malloc(sizeof(NUM_TYPE) * (len + 1));
            utils[QUERY_K_MIN][3] = malloc(sizeof(size_t));
            PersistentSegmentTree<size_t> &tree =
                    *(PersistentSegmentTree<size_t> *) utils[QUERY_K_MIN][0];
            ArrayUtils<NUM_TYPE> &array = *(ArrayUtils<NUM_TYPE> *) utils[QUERY_K_MIN][1];

            auto *discretized = (NUM_TYPE *) utils[QUERY_K_MIN][2];
            auto &cnt = *(size_t*)(utils[QUERY_K_MIN][3]);
            cnt= array.discretize(discretized);
            tree.init(cnt);
            for (size_t i = 1; i <= len; ++i) {
                tree.update((size_t) (i - 1), (size_t) (discretized[i]), 1);
                //printf("v%u:\n", i);
                //tree.print(i);
            }
        }
        PersistentSegmentTree<size_t> &tree =
                *(PersistentSegmentTree<size_t> *) utils[QUERY_K_MIN][0];
        ArrayUtils<NUM_TYPE> &array = *(ArrayUtils<NUM_TYPE> *) utils[QUERY_K_MIN][1];
        auto &cnt = *(size_t*)(utils[QUERY_K_MIN][3]);
        size_t left = 1, right = cnt, mid, node_num,
                old_one = tree.roots[l - 1], new_one = tree.roots[r];
        while (left < right) {
            auto nl = tree.node[tree.node[new_one].left_son].num,
                    nr = tree.node[tree.node[old_one].left_son].num;
            node_num = tree.node[tree.node[new_one].left_son].num -
                       tree.node[tree.node[old_one].left_son].num;
            mid = (left + right) >> 1;
            if (k <= node_num) {
                right = mid;
                new_one = tree.node[new_one].left_son;
                old_one = tree.node[old_one].left_son;
            } else {
                left = mid + 1;
                new_one = tree.node[new_one].right_son;
                old_one = tree.node[old_one].right_son;
                k -= node_num;
            }
        }
        return array.get_undiscretize(left);
    }

private:
    size_t len;
    /*
     * 根据具体需求选择的功能的二进制位数 feature
     * FEATURE枚举所提供的数进行二进制与运算, 如果不是0说明该对象
     * 拥有该功能
     */
    unsigned long long feature;
    // 2的幂次
    enum FEATURE {
        QUERY_K_MIN = 1,
        DISCRETIZE = 2,
        UNKNOWN = 4,
    };
    const NUM_TYPE *origin_array;
    void ***utils;
};


#endif //LYL_UTILS_ARRAYUTILS_H

#pragma clang diagnostic pop