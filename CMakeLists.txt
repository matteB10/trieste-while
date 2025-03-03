cmake_minimum_required(VERSION 3.14.0)
project(while VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  trieste
  GIT_REPOSITORY https://github.com/EliasC/Trieste
  GIT_TAG d164f2ec62efe2a6bbf390fd785d63a42728d18e
  )

FetchContent_MakeAvailable(trieste)

add_executable(while
src/main.cc
src/parser.cc
src/passes/expressions.cc
src/passes/statements.cc
)

target_link_libraries(while
  CLI11::CLI11
  trieste::trieste
)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
