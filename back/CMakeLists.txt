cmake_minimum_required(VERSION 3.10)
project(LocadoraServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(LocadoraServer ${SOURCES})

target_include_directories(LocadoraServer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models
    ${CMAKE_CURRENT_SOURCE_DIR}/src/handlers
)

find_package(cpprestsdk CONFIG REQUIRED)

target_link_libraries(LocadoraServer PRIVATE cpprestsdk::cpprest)
