cmake_minimum_required(VERSION 3.20)

#if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
#    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain")
#endif()

project(compcircuits
    VERSION 0.1.0
    DESCRIPTION "Gamified/Competitive circuit analysis for EE students"
    LANGUAGES C
)


#--Options--
option(CC_BUILD_TESTS "build tests" ON)

#-----Language / build settings -----
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED on)
set(CMAKE_C_EXTENSIONS off)

# Default build config 
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug CACHE STRING "build type" FORCE)
endif()


find_package(SDL2 CONFIG REQUIRED)

set(CC_CAIRO_TARGET "")
set(_VCPKG_TRIPLET_DIR "${CMAKE_SOURCE_DIR}/vcpkg/installed/${VCPKG_TARGET_TRIPLET}")

find_path(CC_CAIRO_INCLUDE_DIR
    NAMES cairo/cairo.h
    PATHS "${_VCPKG_TRIPLET_DIR}/include"
    NO_DEFAULT_PATH
)

find_library(CC_CAIRO_LIB_RELEASE
    NAMES cairo
    PATHS "${_VCPKG_TRIPLET_DIR}/lib"
    NO_DEFAULT_PATH
)

find_library(CC_CAIRO_LIB_DEBUG
    NAMES cairo
    PATHS "${_VCPKG_TRIPLET_DIR}/debug/lib"
    NO_DEFAULT_PATH
)

if(NOT CC_CAIRO_INCLUDE_DIR OR NOT CC_CAIRO_LIB_RELEASE)
    message(FATAL_ERROR "Cairo headers/libs not found under ${_VCPKG_TRIPLET_DIR}")
endif()

add_library(Cairo::Cairo UNKNOWN IMPORTED)
set_target_properties(Cairo::Cairo PROPERTIES
    IMPORTED_LOCATION_RELEASE "${CC_CAIRO_LIB_RELEASE}"
    IMPORTED_LOCATION_DEBUG "${CC_CAIRO_LIB_DEBUG}"
    INTERFACE_INCLUDE_DIRECTORIES "${CC_CAIRO_INCLUDE_DIR}"
)
set(CC_CAIRO_TARGET Cairo::Cairo)
message(STATUS "Using Cairo target: ${CC_CAIRO_TARGET}")

#--Library(Engine)--
add_library(compcircuits STATIC
    SRC/cc_DcMatrix.c
    SRC/cc_solver.c
    SRC/CktEngine.c
)

target_include_directories(compcircuits PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)



#--Warning set--
if(MSVC)
    target_compile_options(compcircuits PRIVATE /W4 /permissive-)
else()
    target_compile_options(compcircuits PRIVATE -Wall -Wextra -Wpedantic)
endif()

#EXTRAS-"pretty stuff"(compile definitions)
target_compile_definitions(compcircuits PRIVATE CC_VERSION=\"${PROJECT_VERSION}\")

#--CLI application--
add_executable(compcircuits_cli
    SRC/main.c
)
add_executable(compcircuits_gui
    SRC/BaseGUI.c
)
target_link_libraries(compcircuits_cli PRIVATE compcircuits)
target_link_libraries(compcircuits_gui PRIVATE
    compcircuits
    SDL2::SDL2
    SDL2::SDL2main
    ${CC_CAIRO_TARGET}
)
