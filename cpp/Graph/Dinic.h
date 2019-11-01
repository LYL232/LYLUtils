#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_DINIC_H
#define LYL_UTILS_DINIC_H

#include <memory.h>
#include <queue>
#include <malloc.h>

/**
 * @tparam EDGE_VALUE_TYPE
 * @tparam MAX_EDGE_TYPE
 */
template<typename EDGE_VALUE_TYPE, EDGE_VALUE_TYPE MAX_EDGE_TYPE>
class Dinic {
public:
    Dinic(size_t _node_num, size_t _max_edge_num) {
        node_num = _node_num;
        max_edge_num = _max_edge_num;
        edges = (Edge *) malloc(sizeof(Edge) * (max_edge_num + 1) * 2);
        head = (size_t *) malloc(sizeof(size_t) * (node_num + 1));
        memset(head, -1, sizeof(size_t) * (node_num + 1));
        current = (size_t *) malloc(sizeof(size_t) * (node_num + 1));
        deep = (size_t *) malloc(sizeof(size_t) * (node_num + 1));
    }

    ~Dinic() {
        free(edges);
        free(head);
        free(current);
        free(deep);
    }

    void add_edge(size_t from, size_t to, EDGE_VALUE_TYPE dis) {
        add_single_edge(from, to, dis);
        add_single_edge(to, from, 0);
    }

    EDGE_VALUE_TYPE solve(size_t source, size_t destination) {
        EDGE_VALUE_TYPE max_flow = 0;
        while (BFS(source, destination)) {
            for (size_t i = 1; i <= node_num; ++i) {
                current[i] = head[i];
            }
            max_flow += DFS(source, destination, MAX_EDGE_TYPE);
        }
        return max_flow;
    }

private:
    struct Edge {
        size_t to, next;
        EDGE_VALUE_TYPE value;
    } *edges;
    size_t node_num, max_edge_num;
    size_t edge_count = (size_t) -1, *head, *current, *deep;

    void add_single_edge(size_t from, size_t to, EDGE_VALUE_TYPE dis) {
        edges[++edge_count].to = to;
        edges[edge_count].value = dis;
        edges[edge_count].next = head[from];
        head[from] = edge_count;
    }

    EDGE_VALUE_TYPE DFS(size_t from, size_t destination, EDGE_VALUE_TYPE flow_limit) {
        if (from == destination) {
            return flow_limit;
        }
        EDGE_VALUE_TYPE flow = 0, reverse_flow;
        for (size_t &current_edge = current[from];
             current_edge != (size_t) -1; current_edge = edges[current_edge].next) {
            size_t next_node = edges[current_edge].to;
            if (deep[next_node] == deep[from] + 1 && edges[current_edge].value > 0) {
                reverse_flow = DFS(next_node, destination, std::min(flow_limit, edges[current_edge].value));
                flow_limit -= reverse_flow;
                edges[current_edge].value -= reverse_flow;
                flow += reverse_flow;
                // 反向边:从0开始,偶数为正向边,奇数为反向边,一条边异或以后即可得到它的对应边
                edges[current_edge ^ 1].value += reverse_flow;
                if (!flow_limit) {
                    break;
                }
            }
        }
        if (!flow) {
            deep[from] = (size_t)-1;
        }
        return flow;
    }

    bool BFS(size_t source, size_t destination) {
        memset(deep, -1, sizeof(size_t) * (node_num + 1));
        std::queue<size_t> bfs_queue;
        bfs_queue.push(source);
        deep[source] = 0;
        size_t node, next_edge;
        while (!bfs_queue.empty()) {
            node = bfs_queue.front();
            bfs_queue.pop();
            for (size_t edge = head[node]; edge != (size_t)-1; edge = edges[edge].next) {
                next_edge = edges[edge].to;
                if (deep[next_edge] == (size_t)-1 && edges[edge].value > 0) {
                    deep[next_edge] = deep[node] + 1;
                    bfs_queue.push(next_edge);
                }
            }
        }
        return deep[destination] != (size_t)-1;
    }
};

#endif //LYL_UTILS_DINIC_H

#pragma clang diagnostic pop