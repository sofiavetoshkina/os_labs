include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)


set(CMAKE_CXX_STANDARD 17)

add_executable(lab1_test lab1_test.cpp ../lab1/src/parent.cpp ../lab1/include/parent.hpp)

target_link_libraries(lab1_test PRIVATE GTest::GTest)
target_include_directories(lab1_test PRIVATE ../lab1/include)

add_executable(lab2_test lab2_test.cpp ../lab2/include/filter.h ../lab2/src/filter.cpp)

target_link_libraries(lab2_test PRIVATE GTest::GTest)
target_include_directories(lab2_test PRIVATE ../lab2/include)

add_executable(lab3_test lab3_test.cpp ../lab3/src/parent.cpp ../lab3/include/parent.hpp)

target_link_libraries(lab3_test PRIVATE GTest::GTest rt)
target_include_directories(lab3_test PRIVATE ../lab3/include)

add_executable(lab4_1_test lab4_1_test.cpp ../lab4/include/functions.h ../lab4/src/implementation1.cpp)

target_link_libraries(lab4_1_test PRIVATE GTest::GTest) 
target_include_directories(lab4_1_test PRIVATE ../lab4/include) 

add_executable(lab4_2_test lab4_2_test.cpp ../lab4/include/functions.h ../lab4/src/implementation2.cpp)

target_link_libraries(lab4_2_test PRIVATE GTest::GTest)
target_include_directories(lab4_2_test PRIVATE ../lab4/include)

add_test(lab1_test lab1_test)

add_test(lab2_test lab2_test)

add_test(lab3_test lab3_test)

add_test(lab4_1_test lab4_1_test)
add_test(lab4_2_test lab4_2_test)