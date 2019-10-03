#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_PERSISTENTSEGMENTTREE_H
#define LYL_UTILS_PERSISTENTSEGMENTTREE_H

#include <cstring>

/**
 * 持久化线段树:
 * 功能: 维护长度为len的数组,可以在某个历史版本上修改或访问某一个位置的值
 * 维护区间[1,len]
 * @tparam NUM_TYPE 数字类型
 * @tparam MAX_LEN 最大长度
 */

template<typename NUM_TYPE, int MAX_LEN>
class PersistentSegmentTree {
    typedef int size_type;
public:
    /**
     * 清除函数:将数据全部清除
     */
    void clear() {
        memset(tree, 0, sizeof(tree));
        data = nullptr;
        cnt = len = 0;
    }

    /**
     * 树的初始化函数
     * 注意序列的下标从1开始
     * @param _len 序列长度
     * @param data 序列指针
     * @return 初始的树版本号(初始树根指针)
     */
    size_type init(size_type _len, const NUM_TYPE *_data = nullptr) {
        len = _len;
        data = _data;
        return build(1, len);
    }

    /**
     * 根据某一旧版本的树根来更新下一个版本的树
     * @param old_root 旧版本树根编号
     * @param pos 更新的序列位置
     * @param num 更新操作数
     * @return 新版本树根编号
     */
    size_type update(size_type old_root, size_type pos, NUM_TYPE num) {
        //新版本线段树是基于版本old_root而来
        return update(old_root, 1, len, pos, num);
    }

    /**
     * 查询某一版本下位置x的数
     * @param version 指定版本树根
     * @param x 序列位置
     * @return 查询结果
     */
    NUM_TYPE query(size_type version, size_type x) {
        //查询版本version下位置x的值
        return query(version, 1, len, x);
    }

private:
    struct Node {
        size_type left_son, right_son;
        NUM_TYPE num;
    } tree[MAX_LEN << 6];

    size_type cnt, len;

    const NUM_TYPE *data;

    /**
     * 树的构建函数
     * @param l 新节点所属区间左端点
     * @param r 新节点所属区间右端点
     * @return 新构建节点编号
     */
    size_type build(int l, int r) {
        size_type new_node = ++cnt;  //动态建树
        if (l == r) {
            tree[new_node].left_son = tree[new_node].right_son = 0;
            tree[new_node].num = (data == nullptr) ? 0 : data[l];
            return new_node;
        }
        size_type mid = (l + r) >> 1;
        tree[new_node].left_son = build(l, mid);
        tree[new_node].right_son = build(mid + 1, r);
        return new_node;
    }

    //这个其实是插入函数，因为公用结点的缘故并不能修改版本信息导致其他版本受影响
    /**
     * 更新函数,根据在parent下,更新pos的数为num
     * @param parent 父节点编号
     * @param node_l 将要生成的子节点的区间左端点
     * @param node_r 将要生成的子节点的区间右端点
     * @param pos 更新位置
     * @param num 更新操作数
     * @return 新生成的子节点编号
     */
    size_type update(size_type parent, int node_l, int node_r, size_type pos, NUM_TYPE num) {
        size_type new_node = ++cnt;  //动态建树
        if (node_l == node_r) {
            tree[new_node].num = num;
            return new_node;
        }
        size_type mid = (node_l + node_r) >> 1;
        //为了公用信息，先复制一份
        tree[new_node] = tree[parent];
        if (pos <= mid) {
            tree[new_node].left_son = update(tree[parent].left_son, node_l, mid, pos, num);
        } else {
            tree[new_node].right_son = update(tree[parent].right_son, mid + 1, node_r, pos, num);
        }
        return new_node;
    }

    /**
     * 查询node节点下,位置x的值
     * @param node 节点编号
     * @param l 区间左端点
     * @param r 区间右端点
     * @param x 位置
     * @return 查询结果
     */
    NUM_TYPE query(size_type node, int l, int r, size_type x) {
        //查询节点parent下位置x的值
        if (l == r) {
            return tree[node].num;
        }
        size_type mid = (l + r) >> 1;
        NUM_TYPE res;
        if (x <= mid) {
            res = query(tree[node].left_son, l, mid, x);
        } else {
            res = query(tree[node].right_son, mid + 1, r, x);
        }
        return res;
    }

};

#endif //LYL_UTILS_PERSISTENTSEGMENTTREE_H

#pragma clang diagnostic pop