cmake_minimum_required(VERSION 2.8.6)

project(mtfind)

add_subdirectory(core)

add_executable(mtfind main.cc)

target_include_directories(mtfind PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(mtfind core)

add_executable(tests tests.cc)

target_include_directories(tests PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(tests core)
