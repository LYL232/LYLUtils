#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef UTILS_SEGMENTTREE_H
#define UTILS_SEGMENTTREE_H

#include <cstring>

/**
 *线段树:
 * 实现功能:根据指定序列父子区间关系维护序列区间性质
 * 区间范围[1,n]
 * 时间复杂度:
 *      修改:O(log(len))
 *      求区间和:O(log(len))
 *      初始化:O(len * log(len))
 *  空间复杂度: 8 * MAX_LEN + O(1)
 *  注意:常数较大
 * @tparam NUM_TYPE 数字类型
 * @tparam MAX_LEN 维护序列最大长度
 * @tparam INIT_NUM 序列每个点的初始值
 */
template<typename NUM_TYPE, int MAX_LEN, NUM_TYPE INIT_NUM = 0>
class SegmentTree {
public:

    /**
     * 清除树中所有数据,但不改变树的结构
     */
    inline void clear() {
        memset(tree, 0, sizeof(tree));
        memset(lazy, 0, sizeof(lazy));
    }

    /**
     * 构建树的结构, 当data为空时,树节点都初始化为0
     * @param _len 树维护的序列长度
     * @param data 树初始序列指针
     */
    void build(int _len, const NUM_TYPE *data = nullptr) {
        if (data == nullptr) {
            build_impl(1, 1, _len);
        } else {
            build_impl(1, 1, _len, data);
        }
        len = _len;
    }

    /**
     * 询问区间[l,r]值
     * @param l 区间左端点
     * @param r 区间右端点
     * @return 区间[l,r]的值
     */
    inline NUM_TYPE query(int l, int r) {
        if (l > r) {
            return 0;
        }
        return query_impl(l, r, 1, len, 1);
    }

    /**
     *  区间[l,r]修改
     * @param l 区间左端点
     * @param r 区间右端点
     * @param x 修改参数x
     */
    inline void update(int l, int r, NUM_TYPE x) {
        update_impl(l, r, 1, len, 1, x);
    }

    /**
     * 单点修改
     * @param pos 修改点
     * @param x 修改参数
     */
    inline void update(int pos, NUM_TYPE x) {
        update_impl(pos, pos, 1, len, 1, x);
    }

private:

    NUM_TYPE tree[MAX_LEN << 2], lazy[MAX_LEN << 2];

    int len;

    /**
     * 询问函数实现函数:询问[l,r]和[q_l,q_r]的交集的区间节点值
     * @param q_l 询问区间左端点
     * @param q_r 询问区间右端点
     * @param l 搜索区间左端点
     * @param r 搜索区间右端点
     * @param node 当前节点id
     * @return [l,r]和[q_l,q_r]的交集的区间节点值
     */
    NUM_TYPE query_impl(int q_l, int q_r, int l, int r, int node) {
        if (q_l <= l && r <= q_r) {
            return tree[node];
        }
        NUM_TYPE res = 0;
        int mid = (l + r) >> 1;
        push_down(node, l, r);
        if (q_l <= mid) {
            res += query_impl(q_l, q_r, l, mid, left_son(node));
        }
        if (q_r > mid) {
            res += query_impl(q_l, q_r, mid + 1, r, right_son(node));
        }
        return res;
    }

    /**
     * 带初始序列的构建函数实现函数
     * @param node 当前节点id
     * @param l 当前节点所属区间左端点
     * @param r 当前节点所属区间右端点
     * @param data 初始序列指针
     */
    void build_impl(int node, int l, int r, const NUM_TYPE *data) {
        lazy[node] = 0;
        if (l == r) {
            if (data == nullptr) {
                tree[node] = 0;
            } else {
                tree[node] = data[l];
            }
            return;
        }
        int mid = (l + r) >> 1;
        build_impl(left_son(node), l, mid, data);
        build_impl(right_son(node), mid + 1, r, data);
        push_up(node);
    }

    /**
     * 不带初始序列构建函数的实现函数
     * @param node 当前节点id
     * @param l 当前节点所属区间左端点
     * @param r 当前节点所属区间右端点
     */
    void build_impl(int node, int l, int r) {
        lazy[node] = 0;
        if (l == r) {
            tree[node] = INIT_NUM;
            return;
        }
        int mid = (l + r) >> 1;
        build_impl(left_son(node), l, mid);
        build_impl(right_son(node), mid + 1, r);
        push_up(node);
    }

    /**
     * 更新函数实现函数(区间更新)
     * @param l 修改区间左端点
     * @param r 修改区间右端点
     * @param node_l 当前节点区间左端点
     * @param node_r 当前节点区间右端点
     * @param node 当前节点id
     * @param x 修改操作数
     */
    inline void update_impl(int l, int r, int node_l, int node_r, int node, NUM_TYPE x) {
        if (l <= node_l && node_r <= r) {
            tree[node] += x * (node_r - node_l + 1);
            lazy[node] += x;
            return;
        }
        push_down(node, node_l, node_r);
        int mid = (node_l + node_r) >> 1;
        if (l <= mid) {
            update_impl(l, r, node_l, mid, left_son(node), x);
        }
        if (r > mid) {
            update_impl(l, r, mid + 1, node_r, right_son(node), x);
        }
        push_up(node);
    }

    /**
     * 更新函数实现函数(单点更新)
     * @param node_l 当前节点区间左端点
     * @param node_r 当前节点区间右端点
     * @param pos 修改点
     * @param node 当前节点id
     * @param x 修改操作数
     */
    inline void update_impl(int node_l, int node_r, int pos, int node, NUM_TYPE x) {
        if (node_l >= node_r) {
            tree[node] += x;
            return;
        }
        int mid = (node_l + node_r) >> 1;
        if (node_l <= mid) {
            update_impl(node_l, mid, pos, left_son(node), x);
        }
        if (node_r > mid) {
            update_impl(mid + 1, node_r, pos, right_son(node), x);
        }
        push_up(node);
    }

    /**
     * 获得节点左孩子id
     * @param node
     * @return 左孩子id
     */
    inline int left_son(int node) {
        return node << 1;
    }

    /**
     * 获得节点右孩子id
     * @param node
     * @return 右孩子id
     */
    inline int right_son(int node) {
        return node << 1 | 1;
    }

    /**
     * 向上更新函数:根据子节点的值更新父节点的值
     * 可根据需求修改
     * @param node 当前节点
     */
    inline void push_up(int node) {
        tree[node] = tree[left_son(node)] + tree[right_son(node)];
    }

    /**
     * 向下维护区间值和懒惰标记更新函数
     * @param node 当前节点id
     * @param l 区间左端点
     * @param r 区间右端点
     * @param k 区间修改数
     */
    inline void maintain(int node, int l, int r, NUM_TYPE k) {
        lazy[node] = lazy[node] + k;
        tree[node] = tree[node] + k * (r - l + 1);
    }

    /**
     * 向下维护函数
     * @param node 当前节点id
     * @param l 区间左端点
     * @param r 区间右端点
     */
    inline void push_down(int node, int l, int r) {
        int mid = (l + r) >> 1;
        maintain(left_son(node), l, mid, lazy[node]);
        maintain(right_son(node), mid + 1, r, lazy[node]);
        lazy[node] = 0;
    }

};

#endif //UTILS_SEGMENTTREE_H
/**
 * 使用示例:
 * 判题网址: https://www.luogu.org/problem/P3374
 */
/*
#include <cstdio>

const int MAX_N = (int) 1e5 + 25;
int n, m, x, y, op;
long long a[MAX_N], k;

SegmentTree<long long, MAX_N> seg_tree;

void demo1() {
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
}
#pragma clang diagnostic pop
*/
#pragma clang diagnostic pop