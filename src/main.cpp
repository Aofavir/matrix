#include "../include/matrix.h"
#include <stdio.h>
#include <iostream>

int main() {
    Matrix A = create_matrix(2, 2);
    A.data[0][0] = 1; A.data[0][1] = 2;
    A.data[1][0] = 3; A.data[1][1] = 4;
    printf("Matrix A:\n");
    print_matrix(A);

    Matrix diagonal = matrix_get_diagonal(A);
    if (diagonal.error == MATRIX_SUCCESS) {
        std::cout << "Main diagonal:\n";
        print_matrix(diagonal);
    }
    else {
        std::cout << "Error: " << diagonal.error << '\n';
    }
    free_matrix(A);
 return 0;
}