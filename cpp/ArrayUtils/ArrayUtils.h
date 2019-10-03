#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef ACM_PRATICE_ARRAYUTILS_H
#define ACM_PRATICE_ARRAYUTILS_H

#include <cstring>
#include <algorithm>

using namespace std;
/**
 * 数组工具, 实现一些STL中没有但比较常用的功能
 * 非静态函数使用前需调用init初始化函数,会复制一份源序列的拷贝
 * @tparam NUM_TYPE
 * @tparam MAX_LEN
 */
template<typename NUM_TYPE, int MAX_LEN>
class ArrayUtils {
public:
    /**
     * 初始化序列长度,并复制一份输入序列
     * 注意:下标从1开始
     * @param data 输入序列
     * @param _len 序列长度
     */
    void init(const NUM_TYPE *data, int _len) {
        len = _len;
        memcpy(array, data, (len + 1) * sizeof(NUM_TYPE));
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
    int discretize(const NUM_TYPE *src, NUM_TYPE *res) {
        NUM_TYPE *begin_index = array + 1, *end_index = array + 1 + len;
        sort(begin_index, end_index);
        auto cnt = (int) (unique(begin_index, end_index) - begin_index);
        for (int i = 1; i <= len; ++i) {
            res[i] = (int) (lower_bound(begin_index, begin_index + cnt, src[i]) - begin_index) + 1;
        }
        return cnt;
    }


private:
    int len;
    NUM_TYPE array[MAX_LEN + 1];
};

#endif //ACM_PRATICE_ARRAYUTILS_H

#pragma clang diagnostic pop