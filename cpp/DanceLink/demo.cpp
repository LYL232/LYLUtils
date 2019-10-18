/**
 * 使用示例
 * 判题地址:https://www.luogu.org/problem/P4929
 * 题目描述
 * 给定一个N行M列的矩阵，矩阵中每个元素要么是1，要么是0
 * 你需要在矩阵中挑选出若干行，使得对于矩阵的每一列j，在你挑选的这些行中，有且仅有一行的第j个元素为1
 * 输入格式
 * 第一行两个数N,M
 * 接下来N行，每行M个数字0或1，表示这个矩阵
 * 输出格式
 * 一行输出若干个数表示答案，两个数之间用空格隔开，输出任一可行方案均可，顺序随意
 * 若无解，输出“No Solution!”（不包含引号）
 */
//#include "../IOUtils/IOUtils.h"
//#include "DanceLink.h"
#include <bits/stdc++.h>

template<typename NUM_TYPE>
class SparseMatrix {
public:

    struct PointValue {
        size_t row, col;
        NUM_TYPE value;

        PointValue() {
            row = col = 0;
            value = 0;
        }

        PointValue(size_t _row, size_t _col, NUM_TYPE _value) {
            row = _row;
            col = _col;
            value = _value;
        }

        PointValue(const PointValue &p) {
            row = p.row;
            col = p.col;
            value = p.value;
        }
    };

    class Iterator {
    public:

        Iterator(const Iterator &iterator) : matrix(iterator.matrix) {
            index = iterator.index;
        }

        inline PointValue operator*() const {
            return get();
        }

        inline bool operator!=(const Iterator &other) const {
            return index != other.index || (&matrix != &(other.matrix));
        }

        inline const Iterator &operator++() {
            ++index;
            return *this;
        }

        inline void to_next() {
            if (++index >= matrix.elements.size()) {
                index = matrix.elements.size();
            };
        }

        inline void to_previous() {
            if (--index < 0) {
                index = 0;
            }
        }

        inline PointValue get() const {
            if (index < 0 || index >= matrix.elements.size()) {
            }
            return matrix.elements[index];
        }

        inline bool has_next() const {
            return index < matrix.elements.size() - 1;
        }

        inline bool has_previous() const {
            return index > 0;
        }

    private:
        friend class SparseMatrix;

        size_t index;
        const SparseMatrix &matrix;

        explicit Iterator(const SparseMatrix &_matrix, size_t _index) : matrix(_matrix) {
            index = _index;
        }
    };


    /**
     *  开始迭代的点
     * @return
     */
    Iterator begin() const {
        return Iterator(*this, 0);
    }

    /**
     *  结束的迭代的点
     * @return
     */
    Iterator end() const {
        return Iterator(*this, elements.size());
    }

    SparseMatrix() {
        rows = cols = 0;
    };

    /**
     *  初始化, 指定矩阵行数和列数
     * @param _rows 行数
     * @param _cols 列数
     */
    SparseMatrix(size_t _rows, size_t _cols) {
        rows = _rows;
        cols = _cols;
    }

    SparseMatrix(const SparseMatrix &sparseMatrix) {
        rows = sparseMatrix.rows;
        cols = sparseMatrix.cols;
        elements = sparseMatrix.elements;
    }

    inline void set_rows(size_t _rows) {
        rows = _rows;
    }

    inline void set_cols(size_t _cols) {
        cols = _cols;
    }

    inline size_t get_rows() const {
        return rows;
    }

    inline size_t get_cols() const {
        return cols;
    }

    /**
     * 设置指定点为存在数
     * @param _row
     * @param _col
     */
    inline void set(size_t _row, size_t _col, NUM_TYPE _value) {
        rows = _row > rows ? _row : rows;
        cols = _col > cols ? _col : cols;
        elements.emplace_back(_row, _col, _value);
    }

    /**
     * 清空所有的已存在的点
     */
    inline void clear() {
        elements.clear();
    }

    /**
     * 返回所有的元素列表
     * @return 所有的元素列表
     */
    inline const std::vector<PointValue> get_elements() const {
        return elements;
    }

protected:
    // 行数和列数
    size_t rows, cols;
    //稀疏矩阵非0元素的点
    std::vector<PointValue> elements;
};


class DanceLinkX {
public:
    /**
     * 需指定最大处理规模, 最大处理规模等于矩阵元素个数加上列数, 即:
     * 对于n行m列矩阵, max_process_size = n*m + m
     * @param _max_process_size
     */
    explicit DanceLinkX(size_t _max_process_size) {
        num_of_selected = cnt = 0;
        found = false;
        max_process_size = _max_process_size + 1;
        nodes = (Node *) malloc(max_process_size * sizeof(Node));
        head = (int *) malloc(max_process_size * sizeof(int));
        num_of_nodes_of_col = (size_t *) malloc(max_process_size * sizeof(int));
        selected = (size_t *) malloc(max_process_size * sizeof(size_t));
    };

    ~DanceLinkX() {
        free(nodes);
        free(head);
        free(num_of_nodes_of_col);
        free(selected);
    }

    std::vector<size_t> dance(size_t rows, size_t cols, const SparseMatrix<bool> &sparse) {
        init(cols);
        for (auto i : sparse) {
            link(i.row, i.col);
        }
        found = false;
        _dance(0);
        std::vector<size_t> res;
        if (found) {
            for (register size_t i = 0; i < num_of_selected; i++) {
                res.push_back(selected[i]);
            }
        }
        return res;
    }


private:
    size_t max_process_size, cnt, num_of_selected;
    struct Node {
        //每个点的左右上下指针，所在行列
        int l, r, u, d, col, row;
    } *nodes;
    int *head;//每行的头结点
    bool found;
    size_t *num_of_nodes_of_col,//每列的节点数
            *selected;//选了哪几行

    /**
     * 搜索答案
     * @param deep 搜索深度
     */
    void _dance(size_t deep) {
        if (nodes[0].r == 0) {
            num_of_selected = deep;
            found = true;
            return;
        }
        register int i, j, c = nodes[0].r;
        for (i = nodes[0].r; i != 0; i = nodes[i].r) {
            if (num_of_nodes_of_col[i] < num_of_nodes_of_col[c]) {
                c = i;
            }
        }
        rm_col(c);
        for (i = nodes[c].d; i != c; i = nodes[i].d) {
            selected[deep] = (size_t) nodes[i].row;
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
    void link(size_t _row, size_t _col) {
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
            nodes[cnt].r = (size_t) head[_row];
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
    void init(size_t num) {
        found = false;
        num_of_selected = 0;
        for (register size_t i = 0; i <= num; i++) {
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


class IOUtils {
public:
    /**
     * 快速读取标准输入的整数值
     * @return 读取的整数值
     * demo:
     *      int x = IOUtils::read();
     */
    static inline int read() {
        register int x = 0, f = 1;
        auto s = (char) getchar();
        while (s < '0' || s > '9') {
            if (s == '-') {
                f = -1;
            }
            s = (char) getchar();
        }
        while (s >= '0' && s <= '9') {
            x = x * 10 + s - '0';
            s = (char) getchar();
        }
        return x * f;
    }
};

using namespace std;
const int MAX_N = 250525;
int n, m;

int main() {
    DanceLinkX danceLink(MAX_N);
    n = IOUtils::read();
    m = IOUtils::read();
    register size_t i, j;
    int f;
    SparseMatrix<bool> sparse;
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= m; j++) {
            f = IOUtils::read();
            if (f) {
                sparse.set(i, j, true);
            }
        }
    }
    vector<size_t> ans = danceLink.dance((size_t) n, (size_t) m, sparse);
    if (ans.empty()) {
        printf("No Solution!");
    } else {
        for (auto each : ans) {
            printf("%u ", each);
        }
        putchar('\n');
    }
    return 0;
}
