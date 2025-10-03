cmake_minimum_required(VERSION 3.10)
project(Bootcamp
        DESCRIPTION "C++ Bootcamp for 15-445/645"
        LANGUAGES CXX)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_VERSION MATCHES "^14.")
    message(STATUS "You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
  else()
    message(WARNING "!! We recommend that you use clang-14 for this bootcamp. You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}, a different version.")
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  message(STATUS "You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
else()
  message(WARNING "!! We recommend that you use clang-14 for this bootcamp. You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}, which is not clang.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiling move semantics/references executables
add_executable(references src/references.cpp)
add_executable(move_semantics src/move_semantics.cpp)
add_executable(move_constructors src/move_constructors.cpp)
add_executable(pointers_references src/pointers_references.cpp)

# Compiling templates executables
add_executable(templated_functions src/templated_functions.cpp)
add_executable(templated_classes src/templated_classes.cpp)

# Compiling C++ STL executables
add_executable(vectors src/vectors.cpp)
add_executable(sets src/sets.cpp)
add_executable(unordered_maps src/unordered_maps.cpp)
add_executable(unique_ptr src/unique_ptr.cpp)
add_executable(shared_ptr src/shared_ptr.cpp)
add_executable(mutex src/mutex.cpp)
add_executable(scoped_lock src/scoped_lock.cpp)
add_executable(condition_variable src/condition_variable.cpp)
add_executable(rwlock src/rwlock.cpp)
add_executable(strings src/strings.cpp)
add_executable(heaps src/heaps.cpp)

# Compiling misc executables
add_executable(data_types src/data_types.cpp)
add_executable(memory_management src/memory_management.cpp)
add_executable(memory_addressing src/memory_addressing.cpp)
add_executable(disk_io src/disk_io.cpp)
add_executable(processes_threads src/processes_threads.cpp)
add_executable(cpu_architecture src/cpu_architecture.cpp)
add_executable(networking src/networking.cpp)
add_executable(udp_test src/udp_test.cpp)
add_executable(udp_server src/udp_server.cpp)
add_executable(udp_client src/udp_client.cpp)
add_executable(telnet_server src/telnet_server.cpp)
add_executable(telnet_client src/telnet_client.cpp)
add_executable(telnet_demo src/telnet_demo.cpp)
add_executable(wrapper_class src/wrapper_class.cpp)
add_executable(iterator src/iterator.cpp)
add_executable(auto src/auto.cpp)
add_executable(namespaces src/namespaces.cpp)

# Compiling bootcamp demo code
add_executable(s24_my_ptr src/spring2024/s24_my_ptr.cpp)