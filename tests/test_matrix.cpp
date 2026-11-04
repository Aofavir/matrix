#include <gtest/gtest.h>
#include "matrix.h"
#include <stdexcept>
// 1. НОРМАЛЬНЫЕ СЛУЧАИ
// 1.1. Создание матрицы
TEST(MatrixTest, CreateMatrix_NormalCase) {
 Matrix A = create_matrix(3, 4);
 // Проверка размеров
 EXPECT_EQ(A.rows, 3); // ровно 3 строки
 EXPECT_EQ(A.cols, 4); // ровно 4 столбца
 // Проверка выделения памяти
 EXPECT_NE(A.data, nullptr);
 // Проверка инициализации элементов матрицы нулями
 for (int i = 0; i < 3; ++i) {
 for (int j = 0; j < 4; ++j) {
 EXPECT_DOUBLE_EQ(A.data[i][j], 0.0);
 }
 }
 // Освобождение памяти
 free_matrix(A);
}
// 1.2. Сложение матриц
TEST(MatrixTest, Addition_NormalCase) {
 Matrix A = create_matrix(2, 2);
 Matrix B = create_matrix(2, 2);
 A.data[0][0] = 1; A.data[0][1] = 2;
 A.data[1][0] = 3; A.data[1][1] = 4;
 B.data[0][0] = 5; B.data[0][1] = 6;
 B.data[1][0] = 7; B.data[1][1] = 8;
 Matrix C = matrix_add(A, B);
 EXPECT_DOUBLE_EQ(C.data[0][0], 6.0);
 EXPECT_DOUBLE_EQ(C.data[0][1], 8.0);
 EXPECT_DOUBLE_EQ(C.data[1][0], 10.0);
 EXPECT_DOUBLE_EQ(C.data[1][1], 12.0);
 // Освобождение
 free_matrix(A);
 free_matrix(B);
 free_matrix(C);
}
// 1.3. Умножение матриц
TEST(MatrixTest, Multiplication_NormalCase) {
 Matrix A = create_matrix(2, 2);
 Matrix B = create_matrix(2, 2);
 A.data[0][0] = 1; A.data[0][1] = 2;
 A.data[1][0] = 3; A.data[1][1] = 4;
 B.data[0][0] = 5; B.data[0][1] = 6;
 B.data[1][0] = 7; B.data[1][1] = 8;
 Matrix C = matrix_multiply(A, B);
 EXPECT_DOUBLE_EQ(C.data[0][0], 19.0);
 EXPECT_DOUBLE_EQ(C.data[0][1], 22.0);
 EXPECT_DOUBLE_EQ(C.data[1][0], 43.0);
 EXPECT_DOUBLE_EQ(C.data[1][1], 50.0);
 free_matrix(A);
 free_matrix(B);
 free_matrix(C);
}
// 1.4. Освобождение памяти — нормальный случай
TEST(MatrixTest, FreeMatrix_NormalCase) {
 Matrix A = create_matrix(3, 3); // создаём валидную матрицу
 // Ожидаем, что освобождение пройдёт без ошибок
 EXPECT_NO_THROW(free_matrix(A));
}
// 2. ГРАНИЧНЫЕ СЛУЧАИ
// 2.1. Минимальный размер матрицы — 1×1
TEST(MatrixTest, CreateMatrix_MinimumSize) {
 Matrix A = create_matrix(1, 1);
 EXPECT_EQ(A.rows, 1);
 EXPECT_EQ(A.cols, 1);
 EXPECT_DOUBLE_EQ(A.data[0][0], 0.0);
 free_matrix(A);
}
// 2.2. Умножение вектор-строка × вектор-столбец приводит к 1×1
// Вход: A (1×3), B (3×1)
// Условие: cols(A) == rows(B) → 3 == 3
// Ожидание: скалярное произведение — матрица 1×1
TEST(MatrixTest, Multiplication_RowTimesColumn_1x1) {
 Matrix A = create_matrix(1, 3); // вектор-строка
 Matrix B = create_matrix(3, 1); // вектор-столбец
 A.data[0][0] = 1; A.data[0][1] = 2; A.data[0][2] = 3;
 B.data[0][0] = 4;
 B.data[1][0] = 5;
 B.data[2][0] = 6;
 Matrix C = matrix_multiply(A, B);
 EXPECT_EQ(C.rows, 1);
 EXPECT_EQ(C.cols, 1);
 EXPECT_DOUBLE_EQ(C.data[0][0], 32.0);
 free_matrix(A);
 free_matrix(B);
 free_matrix(C);
}
// 3. НЕКОРРЕКТНЫЕ СЛУЧАИ
// 3.1. Создание матрицы с нулевыми размерами
TEST(MatrixTest, CreateMatrix_ZeroRows) {
 EXPECT_THROW(create_matrix(0, 3), std::invalid_argument);
}
// 3.2. Создание матрицы с отрицательным размером
TEST(MatrixTest, CreateMatrix_NegativeCols) {
 EXPECT_THROW(create_matrix(3, -1), std::invalid_argument);
}
// 3.3. Сложение матриц разных размеров
// Ожидание: std::invalid_argument
TEST(MatrixTest, Addition_SizeMismatch) {
 Matrix A = create_matrix(2, 2);
 Matrix B = create_matrix(3, 3);
 // Ожидание исключения при попытке сложить несовместимые
//матрицы
 Matrix C = matrix_add(A, B);

EXPECT_EQ(C.error, MATRIX_INVALID_SIZE);
EXPECT_EQ(C.data, nullptr);

free_matrix(A);
free_matrix(B);
 free_matrix(A);
 free_matrix(B);
}
// 3.4. Умножение матриц с несовместимыми размерами
TEST(MatrixTest, Multiplication_SizeMismatch) {
 Matrix A = create_matrix(2, 3);
 Matrix B = create_matrix(2, 2);
 Matrix C = matrix_multiply(A, B);

EXPECT_EQ(C.error, MATRIX_INVALID_SIZE);
EXPECT_EQ(C.data, nullptr);

free_matrix(A);
free_matrix(B);
 free_matrix(A);
 free_matrix(B);
}
// 3.5. Повторное освобождение пустой матрицы
TEST(MatrixTest, FreeMatrix_EmptyIsSafe) {
 Matrix empty{nullptr, 0, 0};
 // Первый вызов — без исключения
 EXPECT_NO_THROW(free_matrix(empty));
 // Повторный вызов — тоже безопасно (проверка на nullptr)
 EXPECT_NO_THROW(free_matrix(empty));
}