#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef LYL_UTILS_DANCELINK_H
#define LYL_UTILS_DANCELINK_H

#include<cstring>
#include "../Matrix/SparseMatrix.h"

class DanceLinkX {
    typedef unsigned int counter;
public:
    /**
     * 需指定最大处理规模, 最大处理规模等于矩阵元素个数加上列数, 即:
     * 对于n行m列矩阵, max_process_size = n*m + m
     * @param _max_process_size
     */
    explicit DanceLinkX(counter _max_process_size) {
        num_of_selected = cnt = 0;
        found = false;
        max_process_size = _max_process_size + 1;
        nodes = (Node *) malloc(max_process_size * sizeof(Node));
        head = (int *) malloc(max_process_size * sizeof(int));
        num_of_nodes_of_col = (counter *) malloc(max_process_size * sizeof(counter));
        selected = (counter *) malloc(max_process_size * sizeof(counter));
    };

    ~DanceLinkX() {
        free(nodes);
        free(head);
        free(num_of_nodes_of_col);
        free(selected);
    }

    std::vector<counter> dance(counter rows, counter cols, const SparseMatrix<bool> &sparse) {
        init(cols);
        for (auto i : sparse) {
            link(i.row, i.col);
        }
        found = false;
        _dance(0);
        std::vector<counter> res;
        if (found) {
            for (counter i = 0; i < num_of_selected; i++) {
                res.push_back(selected[i]);
            }
        }
        return res;
    }


private:
    counter max_process_size, cnt, num_of_selected;
    struct Node {
        //每个点的左右上下指针，所在行列
        int l, r, u, d, col, row;
    } *nodes;
    int *head;//每行的头结点
    bool found;
    counter *num_of_nodes_of_col,//每列的节点数
            *selected;//选了哪几行

    /**
     * 搜索答案
     * @param deep 搜索深度
     */
    void _dance(counter deep) {
        if (nodes[0].r == 0) {
            num_of_selected = deep;
            found = true;
            return;
        }
        int i, j, c = nodes[0].r;
        for (i = nodes[0].r; i != 0; i = nodes[i].r) {
            if (num_of_nodes_of_col[i] < num_of_nodes_of_col[c]) {
                c = i;
            }
        }
        rm_col(c);
        for (i = nodes[c].d; i != c; i = nodes[i].d) {
            selected[deep] = (counter) nodes[i].row;
            for (j = nodes[i].r; j != i; j = nodes[j].r) {
                rm_col(nodes[j].col);
            }
            _dance(deep + 1);
            if (found) {
                return;
            }
            for (j = nodes[i].l; j != i; j = nodes[j].l) {
                resume(nodes[j].col);
            }
        }
        resume(c);
    }

    /**
     * 往 _row行_col列加入值为1的点
     * @param _row
     * @param _col
     */
    void link(counter _row, counter _col) {
        num_of_nodes_of_col[_col]++;
        nodes[cnt].row = _row;
        nodes[cnt].col = _col;
        nodes[cnt].u = _col;
        nodes[cnt].d = nodes[_col].d;
        nodes[nodes[_col].d].u = cnt;
        nodes[_col].d = cnt;
        if (head[_row] < 0) {
            head[_row] = nodes[cnt].r = nodes[cnt].l = cnt;
        } else {
            nodes[cnt].r = (counter) head[_row];
            nodes[cnt].l = nodes[head[_row]].l;
            nodes[nodes[head[_row]].l].r = cnt;
            nodes[head[_row]].l = cnt;
        }
        cnt++;
    }

    /**
     * 为插入num个元素做初始化
     * num 等于需要处理的矩阵的列数
     * @param num
     */
    void init(counter num) {
        found = false;
        num_of_selected = 0;
        for (counter i = 0; i <= num; i++) {
            nodes[i].r = i + 1;
            nodes[i].l = i - 1;
            nodes[i].u = nodes[i].d = i;
        }
        nodes[num].r = 0;
        nodes[0].l = num;
        memset(head, -1, max_process_size * sizeof(int));
        memset(num_of_nodes_of_col, 0, max_process_size * sizeof(int));
        cnt = num + 1;
    }

    /**
     * 删除涉及_col列的集合
     * @param _col
     */
    void rm_col(int _col) {
        nodes[nodes[_col].l].r = nodes[_col].r, nodes[nodes[_col].r].l = nodes[_col].l;
        for (int i = nodes[_col].d; i != _col; i = nodes[i].d) {
            for (int j = nodes[i].r; j != i; j = nodes[j].r) {
                nodes[nodes[j].d].u = nodes[j].u;
                nodes[nodes[j].u].d = nodes[j].d;
                num_of_nodes_of_col[nodes[j].col]--;
            }
        }
    }

    /**
     * 恢复涉及_col列的集合
     * @param _col
     */
    void resume(int _col) {
        for (int i = nodes[_col].u; i != _col; i = nodes[i].u) {
            for (int j = nodes[i].l; j != i; j = nodes[j].l) {
                nodes[nodes[j].d].u = j;
                nodes[nodes[j].u].d = j;
                num_of_nodes_of_col[nodes[j].col]++;
            }
        }
        nodes[nodes[_col].l].r = _col;
        nodes[nodes[_col].r].l = _col;
    }

};

#endif //LYL_UTILS_DANCELINK_H

#pragma clang diagnostic pop