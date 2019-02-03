cmake_minimum_required(VERSION 3.13)
project(ass2 C)

set(CMAKE_C_STANDARD 99)

add_executable(pagerank pagerank.c readData.c readData.h graph.c graph.h BSTree.c BSTree.h list.c list.h stack.h stack.c)
add_executable(inverted inverted.c readData.c readData.h graph.c graph.h BSTree.c BSTree.h list.c list.h stack.h stack.c)
add_executable(searchPagerank searchPagerank.c BStree_double.c BStree_double.h readData.c readData.h graph.c graph.h BSTree.c BSTree.h list.c list.h stack.h stack.c)
