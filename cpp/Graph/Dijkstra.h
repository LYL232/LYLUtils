//
// Created by LYL232 on 2019/10/27.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_DIJKSTRA_H
#define LYL_UTILS_DIJKSTRA_H

#include <memory.h>
#include <c++/queue>
#include <c++/cstdlib>

template<typename EDGE_VALUE_TYPE, EDGE_VALUE_TYPE max_edge_value>
class Dijkstra {
public:
    Dijkstra(size_t _node_num, size_t max_edge_num) {
        node_num = _node_num;
        edge_list = (EdgeLinkList *) malloc(sizeof(EdgeLinkList) * max_edge_num);
        list_head = (size_t *) malloc(sizeof(size_t) * (node_num + 1));
        memset(list_head, 0, sizeof(size_t) * (node_num + 1));
        is_node_vis = (bool *) malloc(sizeof(bool) * (node_num + 1));
        node_distance = (EDGE_VALUE_TYPE *) malloc(sizeof(EDGE_VALUE_TYPE) * (node_num + 1));
        memset(is_node_vis, 0, sizeof(bool) * (node_num + 1));

        edge_count = 0;
    }

    inline void add_edge(size_t from, size_t to, EDGE_VALUE_TYPE value) {
        edge_list[++edge_count].value = value;
        edge_list[edge_count].to = to;
        edge_list[edge_count].next_edge = list_head[from];
        list_head[from] = edge_count;
    }

    inline const EDGE_VALUE_TYPE *solve(size_t source) {
        for (int i = 1; i <= node_num; ++i) {
            node_distance[i] = max_edge_value;
        }
        node_distance[source] = 0;
        node_queue.push((DistanceNode) {0, source});
        while (!node_queue.empty()) {
            DistanceNode tmp = node_queue.top();
            node_queue.pop();
            size_t node_now = tmp.pos;
            if (is_node_vis[node_now]) {
                continue;
            }
            is_node_vis[node_now] = true;
            for (size_t i = list_head[node_now]; i; i = edge_list[i].next_edge) {
                size_t next = edge_list[i].to;
                EDGE_VALUE_TYPE try_value = node_distance[node_now] + edge_list[i].value;
                if (node_distance[next] > try_value) {
                    node_distance[next] = try_value;
                    if (!is_node_vis[next]) {
                        node_queue.push((DistanceNode) {node_distance[next], next});
                    }
                }
            }
        }
        return node_distance;
    }

    ~Dijkstra() {
        free(edge_list);
        free(list_head);
        free(is_node_vis);
        free(node_distance);
    }


private:
    struct EdgeLinkList {
        size_t to, next_edge;
        EDGE_VALUE_TYPE value;
    } *edge_list;
    size_t *list_head/*list_head[x]:x节点为起点的第一条边*/
    , edge_count, node_num;

    bool *is_node_vis;

    EDGE_VALUE_TYPE *node_distance;

    struct DistanceNode {
        EDGE_VALUE_TYPE value;
        size_t pos;

        bool operator<(const DistanceNode &x) const {
            return x.value < value;
        }
    };

    std::priority_queue<DistanceNode> node_queue;

};


#endif //LYL_UTILS_DIJKSTRA_H

#pragma clang diagnostic pop