cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Task3CMake")

# Создаем первую цель с C++98
add_executable(Task3_Cpp98 "Task3.cpp")
set_target_properties(Task3_Cpp98 PROPERTIES CXX_STANDARD 98 CXX_STANDARD_REQUIRED YES)

# Создаем вторую цель с C++20
add_executable(Task3_Cpp20 "Task3CMake.cpp")
set_target_properties(Task3_Cpp20 PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  
endif()

# TODO: добавьте тесты и дополнительные настройки, если необходимо.