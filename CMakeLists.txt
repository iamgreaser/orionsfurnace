cmake_minimum_required (VERSION 3.7)
project ("Orion's Furnace")
include_directories("src")
include_directories(${CMAKE_BINARY_DIR})

set(TARGET "orions-furnace")

# Check for native cmake Libraries
find_package(PkgConfig REQUIRED)
find_package(SDL2 REQUIRED)

# Find all .cpp files
FILE(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

# Specify our target
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} ${SDL2_LIBRARIES})
include_directories(${SDL2_INCLUDE_DIRS})

#
# Find libraries without cmake config via .pc files
#

# SDL2_ttf doesn't provide a cmake config file, use .pc
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
target_link_libraries(${TARGET} ${SDL2_TTF_LIBRARIES})
target_include_directories(${TARGET} PUBLIC ${SDL2_TTF_INCLUDE_DIRS})
target_compile_options(${TARGET} PUBLIC ${SDL2_TTF_CFLAGS_OTHER})

pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
target_link_libraries(${TARGET} ${SDL2_IMAGE_LIBRARIES})
target_include_directories(${TARGET} PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS})
target_compile_options(${TARGET} PUBLIC ${SDL2_IMAGE_CFLAGS_OTHER})

# Generate version information
execute_process(COMMAND  git describe --always --dirty
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
		OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file (
  "${PROJECT_SOURCE_DIR}/src/engine_version.h.in"
  "${PROJECT_BINARY_DIR}/engine_version.h"
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize=undefined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined")
