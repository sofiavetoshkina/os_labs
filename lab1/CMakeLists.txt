set(CMAKE_CXX_STANDARD 17)

add_executable(parent include/parent.hpp src/parent.cpp main.cpp)
target_include_directories(parent PRIVATE include)

add_executable(child src/child.cpp)
