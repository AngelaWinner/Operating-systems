cmake_minimum_required(VERSION 3.14)
project(Server-Client)
set(CMAKE_CXX_STANDARD 17)

# Устанавливаем пути к заголовочным файлам
include_directories(Headers)

# Сервер
add_executable(Server 
    Server.cpp 
    Headers/Functions.h
    Headers/Employee.h
)

# Клиент
add_executable(Client 
    Client.cpp
    Headers/Functions.h
    Headers/Employee.h
)

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0  # Стабильная версия
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Тесты
add_executable(Tests
    test.cpp
    Headers/Functions.h
    Headers/Employee.h
)

# Связываем тесты с gtest
target_link_libraries(Tests 
    PRIVATE 
    GTest::gtest_main
)

# Включаем тестирование
enable_testing()
include(GoogleTest)
gtest_discover_tests(Tests)