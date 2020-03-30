set(SOURCES
    commonTypes.h
    fileDataChunk.cc
    fileDataChunk.h
    fileReader.cc
    fileReader.h
    patternSearchManager.cc
    patternSearchManager.h
    patternMatchProcessor.cc
    patternMatchProcessor.h
    mtfind.cc
)

add_library(core STATIC ${SOURCES})
target_include_directories(core PRIVATE "${CMAKE_SOURCE_DIR}/include")