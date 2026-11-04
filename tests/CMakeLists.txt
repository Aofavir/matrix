include(FetchContent)
FetchContent_Declare(
 googletest
 GIT_REPOSITORY https://github.com/google/googletest.git
 GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
add_executable(matrix_tests test_matrix.cpp ../src/matrix.cpp)
target_link_libraries(matrix_tests PRIVATE gtest_main)
target_include_directories(matrix_tests PRIVATE
${CMAKE_SOURCE_DIR}/include)
add_test(NAME matrix_tests COMMAND matrix_tests)