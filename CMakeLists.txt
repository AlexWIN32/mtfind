cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mtfind)

add_subdirectory(core)

add_executable(mtfind main.cc)

set(LIBS core)
if(UNIX)
    set(LIBS ${LIBS} "pthread")
endif()

target_include_directories(mtfind PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(mtfind ${LIBS})

add_executable(tests tests.cc)

target_include_directories(tests PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(tests ${LIBS})

