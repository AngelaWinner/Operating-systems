cmake_minimum_required(VERSION 3.15)

project(Task4CMake)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включение горячей перезагрузки для MSVC
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_executable(Task4CMake 
    Task4CMake.cpp
    Task4CMake.h
    Receiver.cpp
    Sender.cpp
)

if(WIN32)
    target_link_libraries(Task4CMake kernel32.lib)
endif()

enable_testing()
add_subdirectory(tests)