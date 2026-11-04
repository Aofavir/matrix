#include "matrix.h"
#include <new>
#include <stdexcept>
#include <iostream>
#include <iomanip> // Для std::setw, std::setprecision
#include <algorithm>


Matrix matrix_multiply(const Matrix& A, const Matrix& B)
{
    Matrix C = {nullptr, 0, 0, MATRIX_SUCCESS};

    // Проверка размерностей
    if (A.rows <= 0 || A.cols <= 0 ||
        B.rows <= 0 || B.cols <= 0 ||
        A.cols != B.rows) {
        C.error = MATRIX_INVALID_SIZE;
        return C;
    }

    // Проверка указателей
    if (A.data == nullptr || B.data == nullptr) {
        C.error = MATRIX_INVALID_DATA;
        return C;
    }

    for (int i = 0; i < A.rows; ++i) {
        if (A.data[i] == nullptr) {
            C.error = MATRIX_INVALID_DATA;
            return C;
        }
    }

    for (int i = 0; i < B.rows; ++i) {
        if (B.data[i] == nullptr) {
            C.error = MATRIX_INVALID_DATA;
            return C;
        }
    }

    // Выделение памяти
    C.data = new (std::nothrow) double*[A.rows];

    if (C.data == nullptr) {
        C.error = MATRIX_MEMORY_ERROR;
        return C;
    }

    C.rows = A.rows;
    C.cols = B.cols;

    int allocated = 0;

    for (int i = 0; i < C.rows; ++i) {
        C.data[i] = new (std::nothrow) double[C.cols];

        if (C.data[i] == nullptr) {
            for (int j = 0; j < allocated; ++j)
                delete[] C.data[j];

            delete[] C.data;

            C.data = nullptr;
            C.rows = C.cols = 0;
            C.error = MATRIX_MEMORY_ERROR;

            return C;
        }

        ++allocated;
    }

    // Инициализация результата
    for (int i = 0; i < C.rows; ++i) {
        for (int j = 0; j < C.cols; ++j) {
            C.data[i][j] = 0.0;
        }
    }

    // Умножение матриц: оптимизированный порядок i-k-j
    for (int i = 0; i < A.rows; ++i) {
        for (int k = 0; k < A.cols; ++k) {
            double temp = A.data[i][k];

            for (int j = 0; j < B.cols; ++j) {
                C.data[i][j] += temp * B.data[k][j];
            }
        }
    }

    return C;
}

Matrix matrix_add(const Matrix& A, const Matrix& B)
{
    Matrix C = {nullptr, 0, 0, MATRIX_SUCCESS};

    // Проверка размерностей
    if (A.rows <= 0 || A.cols <= 0 ||
        B.rows <= 0 || B.cols <= 0 ||
        A.rows != B.rows || A.cols != B.cols) {
        C.error = MATRIX_INVALID_SIZE;
        return C;
    }

    // Проверка указателей
    if (A.data == nullptr || B.data == nullptr) {
        C.error = MATRIX_INVALID_DATA;
        return C;
    }

    for (int i = 0; i < A.rows; ++i) {
        if (A.data[i] == nullptr || B.data[i] == nullptr) {
            C.error = MATRIX_INVALID_DATA;
            return C;
        }
    }

    // Выделение памяти
    C.data = new (std::nothrow) double*[A.rows];

    if (C.data == nullptr) {
        C.error = MATRIX_MEMORY_ERROR;
        return C;
    }

    C.rows = A.rows;
    C.cols = A.cols;

    int allocated = 0;

    for (int i = 0; i < C.rows; ++i) {
        C.data[i] = new (std::nothrow) double[C.cols];

        if (C.data[i] == nullptr) {
            for (int j = 0; j < allocated; ++j)
                delete[] C.data[j];

            delete[] C.data;

            C.data = nullptr;
            C.rows = C.cols = 0;
            C.error = MATRIX_MEMORY_ERROR;

            return C;
        }

        ++allocated;
    }

    // Сложение матриц
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            C.data[i][j] = A.data[i][j] + B.data[i][j];
        }
    }

    return C;
}

Matrix create_matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument(
            "Matrix dimensions must be positive"
        );
    }

    Matrix mat = {nullptr, rows, cols, MATRIX_SUCCESS};

    mat.data = new double*[rows]();

    try {
        for (int i = 0; i < rows; ++i) {
            mat.data[i] = new double[cols]();
        }
    }
    catch (...) {
        for (int i = 0; i < rows; ++i) {
            delete[] mat.data[i];
        }

        delete[] mat.data;
        throw;
    }

    return mat;
}

void free_matrix(Matrix& m) {
    // 1. Проверка на nullptr - защита от двойного освобождения
    if (m.data == nullptr) {
        return;
    }
    // 2. Освобождение памяти каждой строки
    for (int i = 0; i < m.rows; i++) {
        if (m.data[i] != nullptr) {
            delete[] m.data[i]; // Освобождаем строку
            m.data[i] = nullptr; // Обнуляем указатель
        }
    }
    // 3. Освобождение массива указателей
    delete[] m.data;
    // 4. Обнуление структуры (защита от use-after-free)
    m.data = nullptr;
    m.rows = 0;
    m.cols = 0;
}

void print_matrix(const Matrix& m) {
    // 1. Проверка валидности матрицы
    if (m.data == nullptr || m.rows <= 0 || m.cols <= 0) {
        std::cout << "[Empty matrix]" << std::endl;
        return;
    }
    // 2. Настройка формата вывода
    std::cout << std::fixed << std::setprecision(2);
    // 3. Проверка необходимости сокращения вывода
    const int MAX_ROWS_TO_SHOW = 10;
    const int MAX_COLS_TO_SHOW = 8;
    bool show_summary = (m.rows > MAX_ROWS_TO_SHOW || m.cols > MAX_COLS_TO_SHOW);
    int rows_to_show = show_summary ? MAX_ROWS_TO_SHOW : m.rows;
    int cols_to_show = show_summary ? MAX_COLS_TO_SHOW : m.cols;
    // 4. Вывод матрицы
    for (int i = 0; i < rows_to_show; i++) {
        // Пропуск средних строк для больших матриц
        if (show_summary && i == MAX_ROWS_TO_SHOW / 2 && m.rows > MAX_ROWS_TO_SHOW) {
            std::cout << "[...";
            for (int j = 0; j < cols_to_show - 2; j++) {
                std::cout << " "; // 6 пробелов под число
            }
            std::cout << "...]" << std::endl;
            continue;
        }

        std::cout << "["; // Открывающая скобка
        // Вывод элементов строки
        for (int j = 0; j < cols_to_show; j++) {
            // Пропуск средних столбцов для больших матриц
            if (show_summary && j == MAX_COLS_TO_SHOW / 2 && m.cols >
            MAX_COLS_TO_SHOW) {
                std::cout << " ... ";
                continue;
            }
            std::cout << std::setw(6) << m.data[i][j]; // Выравнивание

            if (j < cols_to_show - 1) {
                std::cout << " "; // Пробел между элементами
            }
        }
        std::cout << "]"; // Закрывающая скобка
        // Добавление многоточия если пропущены столбцы
        if (show_summary && m.cols > MAX_COLS_TO_SHOW) {
            std::cout << " ...";
        }

        std::cout << std::endl;
    }
    // 5. Добавление многоточия если пропущены строки
    if (show_summary && m.rows > MAX_ROWS_TO_SHOW) {
        std::cout << "[...";
        for (int j = 0; j < std::min(cols_to_show, m.cols); j++) {
            std::cout << " ... ";
        }
        std::cout << "...]" << std::endl;
    }
    // 6. Информация о размере если матрица большая
    if (show_summary) {
        std::cout << "Matrix " << m.rows << "×" << m.cols
        << " (showing first " << rows_to_show << "×" << cols_to_show << ")"
        << std::endl;
    }
}

Matrix matrix_from_array(const double* data, int rows, int cols)
{
    Matrix M = {nullptr, 0, 0, MATRIX_SUCCESS};

    if (data == nullptr || rows <= 0 || cols <= 0) {
        M.error = MATRIX_INVALID_DATA;
        return M;
    }

    M.data = new (std::nothrow) double*[rows];

    if (M.data == nullptr) {
        M.error = MATRIX_MEMORY_ERROR;
        return M;
    }

    M.rows = rows;
    M.cols = cols;

    int allocated = 0;

    for (int i = 0; i < rows; i++) {
        M.data[i] = new (std::nothrow) double[cols];

        if (M.data[i] == nullptr) {
            for (int j = 0; j < allocated; j++)
                delete[] M.data[j];

            delete[] M.data;

            M.data = nullptr;
            M.rows = M.cols = 0;
            M.error = MATRIX_MEMORY_ERROR;

            return M;
        }

        allocated++;

        for (int j = 0; j < cols; j++) {
            M.data[i][j] = data[i * cols + j];
        }
    }

    return M;
}

Matrix matrix_get_diagonal(Matrix m)
{
    Matrix result = {nullptr, 0, 0, MATRIX_SUCCESS};
    if (m.rows <= 0 || m.cols <= 0) {
        result.error = MATRIX_INVALID_SIZE;
        return result;
    }
    if (m.data == nullptr) {
        result.error = MATRIX_INVALID_DATA;
        return result;
    }
    for (int i = 0; i < m.rows; ++i) {
        if (m.data[i] == nullptr) {
            result.error = MATRIX_INVALID_DATA;
            return result;
        }
    }
    int size = (m.rows < m.cols) ? m.rows : m.cols;
    result.data = new (std::nothrow) double*[size];
    if (result.data == nullptr) {
        result.error = MATRIX_MEMORY_ERROR;
        return result;
    }
    result.rows = size;
    result.cols = 1;
    for (int i = 0; i < size; ++i) {
        result.data[i] = new (std::nothrow) double[1];

        if (result.data[i] == nullptr) {
            for (int j = 0; j < i; ++j) {
                delete[] result.data[j];
            }
            delete[] result.data;
            result.data = nullptr;
            result.rows = result.cols = 0;
            result.error = MATRIX_MEMORY_ERROR;
            return result;
        }
        result.data[i][0] = m.data[i][i];
    }
    return result;
}