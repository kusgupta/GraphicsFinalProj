CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)
cmake_policy(SET CMP0054 NEW)
IF (WIN32)
	# VCPKG
	if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
		set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
	endif()
ENDIF ()

if (APPLE)
	set(GLEW_INCLUDE_DIRS "/usr/local/Cellar/glew/2.1.0_1/include")
	set(GLEW_LIBRARIES "/usr/local/Cellar/glew/2.1.0_1/lib/libGlEW.a")
endif ()

project(GLSL)

FILE(GLOB cmakes ${CMAKE_SOURCE_DIR}/cmake/*.cmake)
FOREACH(cmake ${cmakes})
	INCLUDE(${cmake})
ENDFOREACH(cmake)

MESSAGE(STATUS "stdgl: ${stdgl_libraries}")

ADD_SUBDIRECTORY(src)

IF (EXISTS ${CMAKE_SOURCE_DIR}/sln/CMakeLists.txt)
	ADD_SUBDIRECTORY(sln)
ENDIF()
