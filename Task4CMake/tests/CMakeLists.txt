cmake_minimum_required(VERSION 3.15)

enable_testing()

add_executable(tests 
    test.cpp
    Functions.cpp 
    Functions.h  
)

target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set_property(TARGET tests PROPERTY CXX_STANDARD 17)
set_property(TARGET tests PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(tests PRIVATE 
    GTest::gtest_main
    $<$<BOOL:${WIN32}>:kernel32.lib>
)

include(GoogleTest)
gtest_discover_tests(tests)