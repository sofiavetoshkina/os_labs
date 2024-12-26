set(CMAKE_CXX_STANDARD 17)

add_executable(parent8_1 include/parent.hpp src/parent.cpp main.cpp)
target_include_directories(parent8_1 PRIVATE include)
target_link_libraries(parent8_1 PRIVATE rt pthread)

add_executable(child8_1 src/child.cpp)
target_link_libraries(child8_1 PRIVATE rt pthread)

