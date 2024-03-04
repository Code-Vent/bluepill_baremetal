cmake_minimum_required(VERSION 3.20)

##include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain.cmake)

set(PROJECT_NAME bluepill_image_processing)

set(CPU_PARAMETERS
    -mcpu=cortex-m3
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=soft)

set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/../bluepill.ld)




set(MCU_LINKER_OPTIONS 
    #-Wl,
    #$<$<VERSION_GREATER:$<C_COMPILER_VERSION>,10.3.1>:-Wl,--no-warn-rwx-segments>
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)

message(STATUS ${MCU_LINKER_OPTIONS})

set(SOURCE_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wdouble-promotion
    -Wformat=2 -Wformat-truncation
    -Wundef
    -fno-common
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wconversion
        -Wno-volatile
        -Wold-style-cast
        -Wuseless-cast
        -Wsuggest-override>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>
)

#enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
set(CMAKE_CXX_EXTENSIONS ON)

project(${PROJECT_NAME})




add_subdirectory(thirdparty)
add_subdirectory(include)
add_subdirectory(src)

option(BUILD_EXAMPLES "" ON)

if(BUILD_EXAMPLES)
	message(STATUS "Building examples......")
	add_subdirectory(examples)
endif()

#target_compile_features(${EXECUTABLE} PUBLIC cxx_std_20)

# Set microcontroller information

message(STATUS ${MCU_LINKER_SCRIPT})
