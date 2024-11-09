set(CMAKE_CXX_STANDARD 17)

add_executable(lab2 include/filter.h src/filter.cpp main.cpp)
target_include_directories(lab2 PRIVATE include)
target_link_libraries(lab2 PRIVATE Threads::Threads)