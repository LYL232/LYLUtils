#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_SPARSEMATRIX_H
#define LYL_UTILS_SPARSEMATRIX_H

#include <vector>
#include "../Iterator/BaseIterator.h"


/**
 * 简单版本的稀疏矩阵,目前只作简单的表示工具,没有任何检查和优化
 * @tparam NUM_TYPE
 */
template<typename NUM_TYPE>
class SparseMatrix {
    typedef unsigned int counter;
public:

    struct PointValue {
        counter row, col;
        NUM_TYPE value;

        PointValue() {
            row = col = 0;
            value = 0;
        }

        PointValue(counter _row, counter _col, NUM_TYPE _value) {
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
                throw IteratorException(IteratorException::TypeCode::OutOfBound);
            };
        }

        inline void to_previous() {
            if (--index < 0) {
                index = 0;
                throw IteratorException(IteratorException::TypeCode::OutOfBound);
            }
        }

        inline PointValue get() const {
            if (index < 0 || index >= matrix.elements.size()) {
                throw IteratorException(IteratorException::TypeCode::InvalidIndex);
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
    SparseMatrix(counter _rows, counter _cols) {
        rows = _rows;
        cols = _cols;
    }

    SparseMatrix(const SparseMatrix &sparseMatrix) {
        rows = sparseMatrix.rows;
        cols = sparseMatrix.cols;
        elements = sparseMatrix.elements;
    }

    inline void set_rows(counter _rows) {
        rows = _rows;
    }

    inline void set_cols(counter _cols) {
        cols = _cols;
    }

    inline counter get_rows() const {
        return rows;
    }

    inline counter get_cols() const {
        return cols;
    }

    /**
     * 设置指定点为存在数
     * @param _row
     * @param _col
     */
    inline void set(counter _row, counter _col, NUM_TYPE _value) {
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
    counter rows, cols;
    //稀疏矩阵非0元素的点
    std::vector<PointValue> elements;
};

#endif //LYL_UTILS_SPARSEMATRIX_H

#pragma clang diagnostic pop