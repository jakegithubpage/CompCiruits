cmake_minimum_required(VERSION 3.20)

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
    set(CMAKE_BUILD_TYPE debug cache string "build type" FORCE)
endif()

#--Library(Engine)--
add_library(compcircuits STATIC
    SRC/cc_solver.c
    SRC/main.c
    SRC/BaseGUI.c
    SRC/CktEngine.c
)

target_include_directories(compcircuits 
    PUBLIC
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
    apps/compcircuits_cli.c
)

target_link_libraries(compcircuits_cli PRIVATE compcircuits)
