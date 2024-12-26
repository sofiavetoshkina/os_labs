set(CMAKE_CXX_STANDARD 17)

# Включить предупреждения
add_compile_options(-Wno-pedantic 
  -Wno-tautological-constant-compare 
  -Wno-implicit-fallthrough 
  -Wno-stringop-overflow 
  -Wno-switch
  -Wno-unused-result
  -Wno-deprecated-declarations
  -Wno-unused-variable
  -Wno-unused-but-set-variable
  )

# Найти необходимые библиотеки
# Установка Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
  TLS_VERIFY false
)
FetchContent_MakeAvailable(googletest)


# Установка ZeroMQ
FetchContent_Declare(
  libzmq
  GIT_REPOSITORY https://github.com/zeromq/libzmq.git
  GIT_TAG v4.3.5
  TLS_VERIFY false
)

FetchContent_Declare(
  cppzmq
  GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
  GIT_TAG v4.10.0
  TLS_VERIFY false
)

FetchContent_MakeAvailable(libzmq cppzmq)


# Создание исполняемых файлов
add_executable(client
    include/CalculationNode.h
    include/ZMQFunctions.h 
    include/BalancedTree.h
    src/ClientProgram.cpp
)

add_executable(server 
    include/CalculationNode.h
    include/ZMQFunctions.h 
    include/BalancedTree.h
    src/ServerProgram.cpp
)

# Включить заголовочные файлы
target_include_directories(client PRIVATE 
    include
    ${libzmq_SOURCE_DIR}/include
    ${cppzmq_SOURCE_DIR}
)

target_include_directories(server PRIVATE 
    include
    ${libzmq_SOURCE_DIR}/include
    ${cppzmq_SOURCE_DIR}
)

# Линковка библиотек
target_link_libraries(client 
    libzmq-static
    cppzmq
    pthread
)

target_link_libraries(server 
    libzmq-static
    cppzmq
    pthread
)
