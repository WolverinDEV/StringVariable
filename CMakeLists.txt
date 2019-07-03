cmake_minimum_required(VERSION 3.1)
project(StringVariable)

set(CMAKE_CXX_STANDARD 11)

set(HEADERS include/StringVariable.h)
set(SOURCES src/Stringvariable.cpp)

macro(configure_msvc_runtime)
	if(MSVC)
		# Default to statically-linked runtime.
		if("${MSVC_RUNTIME}" STREQUAL "")
			set(MSVC_RUNTIME "static")
		endif()

		# Set compiler options.
		set(variables
				CMAKE_C_FLAGS_DEBUG
				CMAKE_C_FLAGS_MINSIZEREL
				CMAKE_C_FLAGS_RELEASE
				CMAKE_C_FLAGS_RELWITHDEBINFO
				CMAKE_CXX_FLAGS_DEBUG
				CMAKE_CXX_FLAGS_MINSIZEREL
				CMAKE_CXX_FLAGS_RELEASE
				CMAKE_CXX_FLAGS_RELWITHDEBINFO
				)
		if(${MSVC_RUNTIME} STREQUAL "static")
			foreach(variable ${variables})
				if(${variable} MATCHES "/MD")
					string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
				endif()
			endforeach()
		else()
			foreach(variable ${variables})
				if(${variable} MATCHES "/MT")
					string(REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
				endif()
			endforeach()
		endif()
	endif()
endmacro()
configure_msvc_runtime()

#Using ${HEADERS} so its not greyed in CLion
add_library(StringVariablesShared SHARED ${SOURCES} ${HEADERS})
add_library(StringVariablesStatic STATIC ${SOURCES})

install(
        TARGETS StringVariablesShared StringVariablesStatic
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
)
INSTALL (
        DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION include
		COMPONENT ${HEADERS}
)

add_executable(StringVariableTest main.cpp)
target_link_libraries(StringVariableTest StringVariablesStatic)
