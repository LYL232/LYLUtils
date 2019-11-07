#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef LYL_UTILS_RANGESEGMENTTREE_H
#define LYL_UTILS_RANGESEGMENTTREE_H

#include <malloc.h>
#include <c++/cstdio>

/**
 * 值域线段树(权值线段树)：
 * 每个节点表示值域为[L,R]之间的性质，可以表示很大的范围，
 * 空间复杂度 O(max_num_count * log2(range_r - range_l)
 *  时间复杂度 O(max_num_count * log2(range_r - range_l)
 * @tparam NUM_TYPE
 */
template<typename NUM_TYPE>
class RangeSegmentTree {
    struct Node {
        NUM_TYPE value;
        size_t sons[2];
    } *nodes;
    size_t node_now, root;
    NUM_TYPE range_l, range_r, query_l, query_r;
public:
    RangeSegmentTree(size_t _max_num_count, NUM_TYPE rl, NUM_TYPE rr) {
        if (rl > rr) {
            NUM_TYPE temp = rl;
            rl = rr;
            rr = temp;
        }
        NUM_TYPE rge = rr - rl;
        size_t log_rang = 1;
        // 求出最大范围值域需要的空间
        while (rge > 1) {
            rge >>= 1;
            ++log_rang;
        }
        size_t max_node = _max_num_count * log_rang;
        nodes = (Node *) malloc(sizeof(Node) * max_node);
        range_l = rl;
        range_r = rr;
        root = node_now = 0;
    }

    ~RangeSegmentTree() {
        free(nodes);
    }

    /**
     * 向树中插入一个数
     * @param value
     */
    void insert(NUM_TYPE value) {
        insert_to_root(root, range_l, range_r, value);
    }

    /**
     * 询问在[l,r]之间的数的性质
     * @param l
     * @param r
     * @return
     */
    NUM_TYPE query(NUM_TYPE l, NUM_TYPE r) {
        query_l = l;
        query_r = r;
        return query(root, range_l, range_r);
    }

    /**
     * 打印树
     */
    void print() const {
        print_node(root, range_l, range_r);
    }

private:
    void print_node(size_t node, NUM_TYPE l, NUM_TYPE r) const {
        if (!node) {
            return;
        }
        printf("%u:[%lld, %lld](%lld, %u, %u)\n", node, l, r, nodes[node].value, nodes[node].sons[0],
               nodes[node].sons[1]);
        NUM_TYPE mid = (l + r) >> 1;
        print_node(nodes[node].sons[0], l, mid);
        print_node(nodes[node].sons[1], mid + 1, r);
    }

    void insert_to_root(size_t &node, NUM_TYPE l, NUM_TYPE r, NUM_TYPE v) {
        if (!node) {
            node = ++node_now;
            nodes[node].value = 0;
            nodes[node].sons[0] = nodes[node].sons[1] = 0;
        }
        nodes[node].value += 1;
        if (l == r) {
            return;
        }
        NUM_TYPE mid = (l + r) >> 1;
        if (v <= mid) {
            insert_to_root(nodes[node].sons[0], l, mid, v);
        } else {
            insert_to_root(nodes[node].sons[1], mid + 1, r, v);
        }
    }

    NUM_TYPE query(size_t parent, NUM_TYPE l, NUM_TYPE r) const {
        if (!parent) {
            return 0;
        }
        if (query_l <= l && r <= query_r) {
            return nodes[parent].value;
        }
        NUM_TYPE mid = (l + r) >> 1, res = 0;
        if (query_l <= mid) {
            res += query(nodes[parent].sons[0], l, mid);
        }
        if (query_r > mid) {
            res += query(nodes[parent].sons[1], mid + 1, r);
        }
        return res;
    }
};

#endif //LYL_UTILS_RANGESEGMENTTREE_H

#pragma clang diagnostic pop