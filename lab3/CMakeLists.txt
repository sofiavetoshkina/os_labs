set(CMAKE_CXX_STANDARD 17)

add_executable(parent1 include/parent.hpp src/parent.cpp main.cpp)
target_include_directories(parent1 PRIVATE include)
target_link_libraries(parent1 PRIVATE rt pthread)

add_executable(child1 src/child.cpp)
target_link_libraries(child1 PRIVATE rt pthread)

