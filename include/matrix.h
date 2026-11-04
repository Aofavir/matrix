
#ifndef MATRIX_H
#define MATRIX_H

// Коды ошибок
enum MatrixError {
    MATRIX_SUCCESS = 0,
    MATRIX_INVALID_SIZE = 1,
    MATRIX_INVALID_DATA = 2,
    MATRIX_MEMORY_ERROR = 3
};

// Структура матрицы
typedef struct {
    double** data;
    int rows;
    int cols;
    int error;
} Matrix;

// Базовые операции
Matrix create_matrix(int rows, int cols);
void free_matrix(Matrix& m);

Matrix matrix_add(const Matrix& A, const Matrix& B);
Matrix matrix_multiply(const Matrix& A, const Matrix& B);

// Вспомогательные функции
void print_matrix(const Matrix& m);
Matrix matrix_from_array(const double* data, int rows, int cols);

// Функция для задания по варианту
Matrix matrix_get_diagonal(Matrix m);
#endif
