set(CMAKE_CXX_STANDARD 17)

add_library(implementation1 SHARED src/implementation1.cpp)
set_target_properties(implementation1 PROPERTIES OUTPUT_NAME "implementation1" PREFIX "")
target_include_directories(implementation1 PRIVATE include)

add_library(implementation2 SHARED src/implementation2.cpp)
set_target_properties(implementation2 PROPERTIES OUTPUT_NAME "implementation2" PREFIX "")
target_include_directories(implementation2 PRIVATE include)

add_executable(main_linked_imp1 main_linked.cpp)
target_include_directories(main_linked_imp1 PRIVATE include)
target_link_libraries(main_linked_imp1 PRIVATE implementation1 m)

add_executable(main_linked_imp2 main_linked.cpp)
target_include_directories(main_linked_imp2 PRIVATE include)
target_link_libraries(main_linked_imp2 PRIVATE implementation2 m)

add_executable(main_dynamic main_dynamic.cpp)
target_include_directories(main_dynamic PRIVATE include)
target_link_libraries(main_dynamic PRIVATE dl m)