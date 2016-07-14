cmake_minimum_required(VERSION 2.8)

find_package(Threads REQUIRED)

set(ex1 CreationAndTermination)
add_executable(${ex1} ${ex1}.c)
target_link_libraries(${ex1} ${CMAKE_THREAD_LIBS_INIT})

set(ex2 PassingArgument)
add_executable(${ex2} ${ex2}.c)
target_link_libraries(${ex2} ${CMAKE_THREAD_LIBS_INIT})

set(ex3 JoinThread)
add_executable(${ex3} ${ex3}.c)
target_link_libraries(${ex3} ${CMAKE_THREAD_LIBS_INIT} -lm)

set(ex4 StackManagement)
add_executable(${ex4} ${ex4}.c)
target_link_libraries(${ex4} ${CMAKE_THREAD_LIBS_INIT})

set(ex5 MutexExample)
add_executable(${ex5} ${ex5}.c)
target_link_libraries(${ex5} ${CMAKE_THREAD_LIBS_INIT})

set(ex6 CondVarExample)
add_executable(${ex6} ${ex6}.c)
target_link_libraries(${ex6} ${CMAKE_THREAD_LIBS_INIT})
