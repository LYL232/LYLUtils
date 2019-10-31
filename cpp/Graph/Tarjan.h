//
// Created by 31832 on 2019/10/31.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_TARJAN_H
#define LYL_UTILS_TARJAN_H

#include <vector>
#include <malloc.h>
#include <memory.h>

using namespace std;

class Tarjan {
public:
    typedef vector<vector<size_t> > Result;

    Tarjan(size_t _node_num, size_t _max_edge_num) {
        node_num = _node_num;
        max_edge_num = _max_edge_num;
        edges = (Edge *) malloc(sizeof(Edge) * (max_edge_num + 1));
        nodes = (Node *) malloc(sizeof(Node) * (node_num + 1));
        node_stack = (size_t *) malloc(sizeof(size_t) * (node_num + 1));
        scc_node_num = (size_t *) malloc(sizeof(size_t) * (node_num + 1));
        memset(nodes, 0, sizeof(Node) * (node_num + 1));
        num_of_scc = timestamp = edge_count = stack_top = 0;
    }

    void add_edge(size_t from, size_t to) {
        edge_count++;
        edges[edge_count].to = to;
        edges[edge_count].next_edge = nodes[from].head;
        nodes[from].head = edge_count;
    }

    Result solve() {
        for (size_t i = 1; i <= node_num; i++) {
            if (!nodes[i].dfn) {
                tarjan(i);
            }
        }
        return result;
    }

    ~Tarjan() {
        free(edges);
        free(nodes);
        free(node_stack);
        free(scc_node_num);
    }

private:
    struct Edge {
        size_t to, next_edge;
    } *edges;

    struct Node {
        size_t head, dfn, low,
                belongs_to;//属于哪一个强连通分量
    } *nodes;

    Result result;

    size_t num_of_scc,// 强连通分量个数
            timestamp, edge_count,
            *node_stack, *scc_node_num/*强连通分量的个数*/,
            node_num, max_edge_num, stack_top;

    void tarjan(size_t node) {
        nodes[node].dfn = nodes[node].low = ++timestamp;
        node_stack[++stack_top] = node;
        for (size_t i = nodes[node].head; i; i = edges[i].next_edge) {
            size_t next_node = edges[i].to;
            if (!nodes[next_node].dfn) {
                tarjan(next_node);
                nodes[node].low = min(nodes[node].low, nodes[next_node].low);
            } else if (!nodes[next_node].belongs_to) {
                nodes[node].low = min(nodes[node].low, nodes[next_node].dfn);
            }
        }
        if (nodes[node].low == nodes[node].dfn) {
            result.emplace_back();
            size_t new_scc = result.size() - 1;
            nodes[node].belongs_to = new_scc;
            result[new_scc].push_back(node);
            while (node_stack[stack_top] != node) {
                ++scc_node_num[num_of_scc];
                size_t node_now = node_stack[stack_top];
                result[new_scc].push_back(node_now);
                nodes[node_now].belongs_to = new_scc;
                --stack_top;
            }
            --stack_top;
        }
    }


};

#endif //LYL_UTILS_TARJAN_H

#pragma clang diagnostic pop