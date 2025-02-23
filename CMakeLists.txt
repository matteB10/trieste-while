cmake_minimum_required(VERSION 3.14.0)
project(while VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  trieste
  GIT_REPOSITORY https://github.com/microsoft/Trieste
  GIT_TAG 5858239198315bd8fbe6b29e089cd1e65a286a4d
  )

FetchContent_MakeAvailable(trieste)

add_executable(while
src/main.cc
src/parser.cc
src/passes/addsub.cc
src/passes/assignments.cc
src/passes/atoms.cc
src/passes/conditionals.cc
src/passes/grouping.cc
src/passes/loops.cc
src/passes/multiplication.cc
)

target_link_libraries(while
  CLI11::CLI11
  trieste::trieste
)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
