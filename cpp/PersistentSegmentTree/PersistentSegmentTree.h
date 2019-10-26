#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_PERSISTENTSEGMENTTREE_H
#define LYL_UTILS_PERSISTENTSEGMENTTREE_H

#include <cstring>
#include <cstdlib>
#include <c++/cstdio>

/**
 * 持久化线段树:
 * 功能: 维护长度为len的数组,可以在某个历史版本上修改或访问某一个区间的值
 * 维护区间[1,len]
 * @tparam NUM_TYPE 数字类型
 */
template<typename NUM_TYPE>
class PersistentSegmentTree {
    template<typename > friend
    class ArrayUtils;

public:

    /**
     * 构造函数, 需要指定处理规模
     * @param _process_len
     */
    explicit PersistentSegmentTree(size_t _process_len) {
        cnt = len = versions = 0;
        node = (Node *) malloc((_process_len * 40) * sizeof(Node));
        roots = (size_t *) malloc((_process_len + 1) * sizeof(size_t));
        memset(node, 0, sizeof(Node) * (_process_len * 40));
    }

    ~PersistentSegmentTree() {
        free(roots);
        free(node);
    }

    /**
     * 清除函数:将数据全部清除
     */
    void clear() {
        memset(node, 0, sizeof(node));
        data = 0;
        cnt = len = 0;
    }

    /**
     * 树的初始化函数
     * 注意序列的下标从1开始
     * @param _len 序列长度
     * @param data 序列指针
     * @return 初始的树版本号(初始树根指针)
     */
    size_t init(size_t _len, const NUM_TYPE *_data = nullptr) {
        len = _len;
        data = _data;
        roots[0] = build(1, len);
        versions = 1;
        return roots[0];
    }

    /**
     * 根据某一旧版本的树根来更新下一个版本的树
     * @param old_root 旧版本树根编号
     * @param pos 更新的序列位置
     * @param num 更新操作数
     * @return 新版本树根编号
     */
    size_t update(size_t old_root, size_t pos, NUM_TYPE num) {
        //新版本线段树是基于版本old_root而来
        roots[versions] = update(roots[old_root], 1, len, pos, num);
        return roots[versions++];
    }

    /**
     * 查询某一版本下位置x的数
     * @param version 指定版本树根
     * @param x 序列位置
     * @return 查询结果
     */
    NUM_TYPE query(size_t version, size_t x) {
        //查询版本version下位置x的值
        return query(version, 1, len, x);
    }

    /**
     * 查询某一版本下区间[l,r]的数
     * @param root 指定版本树根
     * @param l 区间左端点
     * @param r 区间右端点
     * @return 查询结果
     */
    NUM_TYPE query(size_t root, int l, int r) {
        //查询版本version下位置x的值
        return query(root, l, r, 1, len);
    }

private:
    struct Node {
        size_t left_son, right_son;
        NUM_TYPE num;
    } *node;

    size_t cnt, len, *roots/*每个版本的根*/, versions/*版本数*/;

    const NUM_TYPE *data;

    /**
     * 树的构建函数
     * @param l 新节点所属区间左端点
     * @param r 新节点所属区间右端点
     * @return 新构建节点编号
     */
    size_t build(int l, int r) {
        size_t new_node = ++cnt;  //动态建树
        if (l == r) {
            node[new_node].left_son = node[new_node].right_son = 0;
            node[new_node].num = (data == nullptr) ? 0 : data[l];
            return new_node;
        }
        size_t mid = (size_t) (l + r) >> 1;
        node[new_node].left_son = build(l, mid);
        node[new_node].right_son = build(mid + 1, r);
        push_up(new_node);
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
    size_t update(size_t parent, int node_l, int node_r, size_t pos, NUM_TYPE num) {
        size_t new_node = ++cnt;  //动态建树
        //为了公用信息，先复制一份
        node[new_node] = node[parent];
        if (node_l == node_r) {
            // 可根据不同功能修改赋值语句 比如把=改成+=
            node[new_node].num += num;
            return new_node;
        }
        size_t mid = (size_t) (node_l + node_r) >> 1;
        if (pos <= mid) {
            node[new_node].left_son = update(node[parent].left_son, node_l, mid, pos, num);
        } else {
            node[new_node].right_son = update(node[parent].right_son, mid + 1, node_r, pos, num);
        }
        push_up(new_node);
        return new_node;
    }

    /**
     * 线段树向上更新函数, 可根据不同功能修改
     * @param node_id
     */
    inline void push_up(size_t node_id) {
        node[node_id].num = node[node[node_id].left_son].num + node[node[node_id].right_son].num;
    }

    /**
     * 查询node节点下,区间ql, qr的值
     * @param node_id 节点编号
     * @param l 当前区间左端点
     * @param r 当前区间右端点
     * @param ql 询问的区间右端点
     * @param qr 询问的区间右端点
     * @return 查询结果
     */
    NUM_TYPE query(size_t node_id, int ql, int qr, int l, int r) {
        //查询节点parent下位置x的值
        if (ql <= l && r <= qr) {
            return node[node_id].num;
        }
        size_t mid = (size_t) (l + r) >> 1;
        NUM_TYPE res = 0;
        if (ql <= mid) {
            res += query(node[node_id].left_son, ql, qr, l, mid);
        }
        if (qr > mid) {
            res += query(node[node_id].right_son, ql, qr, mid + 1, r);
        }
        return res;
    }

    /**
     * 查询node节点下,位置x的值
     * @param node_id 节点编号
     * @param l 区间左端点
     * @param r 区间右端点
     * @param x 位置
     * @return 查询结果
     */
    NUM_TYPE query(size_t node_id, int l, int r, size_t x) {
        //查询节点parent下位置x的值
        if (l == r) {
            return node[node_id].num;
        }
        size_t mid = (size_t) (l + r) >> 1;
        NUM_TYPE res;
        if (x <= mid) {
            res = query(node[node_id].left_son, l, mid, x);
        } else {
            res = query(node[node_id].right_son, mid + 1, r, x);
        }
        return res;
    }

    /**
     * 输出某个版本线段树的结构和数据
     * @param version
     */
    void print(size_t version) {
        print(0, roots[version], 1, len);
    }

    /**
     * 递归调用输出树的结构和数据函数
     * @param depth
     * @param root
     * @param l
     * @param r
     */
    void print(size_t depth, size_t root, size_t l, size_t r) {
        for(size_t i = 0; i < depth; ++i) {
            putchar(' ');
            putchar(' ');
        }
        printf("[%u, %u] = %u\n", l, r, node[root].num);
        if(l >= r) {
            return;
        }
        auto mid = (l + r) >> 1;
        print(depth + 1, node[root].left_son, l, mid);
        print(depth + 1, node[root].right_son, mid + 1, r);
    }

};

#endif //LYL_UTILS_PERSISTENTSEGMENTTREE_H

#pragma clang diagnostic pop