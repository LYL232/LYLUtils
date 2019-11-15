#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_MATRIX_H
#define LYL_UTILS_MATRIX_H

#include <malloc.h>
#include <cstring>

/**
 * 简单的矩阵表示
 * @tparam NUM_TYPE
 */
template<typename NUM_TYPE>
class Matrix {
    typedef unsigned int counter;
public:
    NUM_TYPE **elements;

    Matrix() {
        elements = nullptr;
        row = col = 0;
    }

    /**
     *  初始化, 下标从0开始
     * @param _rows 行数
     * @param _cols 列数
     */
    Matrix(size_t _rows, size_t _cols) {
        row = _rows;
        col = _cols;
        apply_mem();
    }

    Matrix(const Matrix &matrix) {
        row = matrix.row;
        col = matrix.col;
        apply_mem();
        const size_t col_size = sizeof(NUM_TYPE) * col;
        for (counter i = 0; i < row; ++i) {
            memcpy(elements[i], matrix.elements[i], col_size);
        }
    }

    ~Matrix() {
        free_mem();
    }

    /**
     * 重新调整矩阵的大小
     * @param _rows 行数
     * @param _cols 列数
     * @param set_zero 是否置0
     */
    void resize(size_t _rows, size_t _cols, bool set_zero = false) {
        free_mem();
        row = _rows;
        col = _cols;
        apply_mem();
        if (set_zero) {
            const size_t col_size = col * sizeof(NUM_TYPE);
            for (counter i = 0; i < row; ++i) {
                memset(elements[i], 0, col_size);
            }
        }
    }

private:
    size_t row, col;

    void apply_mem() {
        elements = (NUM_TYPE **) malloc(row * sizeof(NUM_TYPE *));
        const size_t col_size = col * sizeof(NUM_TYPE);
        for (counter i = 0; i < row; ++i) {
            elements[i] = (NUM_TYPE *) malloc(col_size);
        }
    }

    void free_mem() {
        for (counter i = 0; i < row; ++i) {
            free(elements[i]);
        }
        free(elements);
    }

};

#endif //LYL_UTILS_MATRIX_H

#pragma clang diagnostic pop