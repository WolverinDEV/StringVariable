cmake_minimum_required(VERSION 3.9)
project(StringVariable)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

set(HEADERS include/StringVariable.h)
set(SOURCES src/Stringvariable.cpp)

add_library(StringVariables SHARED ${SOURCES})
add_library(StringVariablesStatic STATIC ${SOURCES})

install(
        TARGETS StringVariables StringVariablesStatic
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
)
INSTALL (
        DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h*"
)

add_executable(StringVariableTest main.cpp)
target_link_libraries(StringVariableTest StringVariables)