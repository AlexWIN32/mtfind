cmake_minimum_required(VERSION 2.8.6)

project(mtfind)

set(SOURCES
    main.cc
    fileDataChunk.cc
    fileDataChunk.h
    fileReader.cc
    fileReader.h
    patternSearchManager.cc
    patternSearchManager.h
    patternMatchProcessor.cc
    patternMatchProcessor.h
    )

add_executable(mtfind ${SOURCES})
target_include_directories(mtfind PRIVATE "${CMAKE_SOURCE_DIR}/include" )