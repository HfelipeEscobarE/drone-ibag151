cmake_minimum_required(VERSION 3.14)
project(DroneIBag151 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Evita el aviso de FetchContent sobre la fecha de los archivos descargados
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# Nucleo: modelo + controlador + vista + adaptador (sin main), asi lo reutilizan la app
# y los tests
add_library(drone_core
    src/model/Drone.cpp
    src/controller/ControladorDeComandos.cpp
    src/view/VistaConsola.cpp
    src/other/CanalConsola.cpp)
target_include_directories(drone_core PUBLIC include)
target_compile_options(drone_core PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>)

add_executable(drone_app src/main.cpp)
target_link_libraries(drone_app PRIVATE drone_core)

# ---- Pruebas ----------------------------------------------------------------
include(CTest)
if(BUILD_TESTING)
    find_package(GTest QUIET)
    if(NOT GTest_FOUND)
        include(FetchContent)
        FetchContent_Declare(googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

    add_executable(drone_tests
        tests/DroneTests.cpp
        tests/ControladorTests.cpp
        tests/VistaTests.cpp)
    target_link_libraries(drone_tests PRIVATE drone_core GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(drone_tests)
endif()
