cmake_minimum_required(VERSION 3.10.0)
set (CMAKE_CXX_STANDARD 17)
project(trolleybus_plus_plus VERSION 0.1.0 LANGUAGES CXX)

set(OSBitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OSBitness 64)
endif()
set (FullOutputDir ${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}${OSBitness}/${CMAKE_BUILD_TYPE})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FullOutputDir}")


file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/source/src/*.cpp)
add_executable(tpp ${SOURCES})
target_include_directories(tpp PUBLIC ${CMAKE_SOURCE_DIR}/source/include)
