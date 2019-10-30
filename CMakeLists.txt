cmake_minimum_required (VERSION 3.7)
project ("Orion's Furnace")
include_directories("src")
include_directories(${CMAKE_BINARY_DIR})

set(TARGET "orions-furnace")

# Options
option(HINT_SUGGESTIONS "Use hinting suggestions" OFF)
option(SANITIZE "Enable sanitizers (requires various lib*san libraries)" ON)
option(I_SHOULD_UPDATE_MINGW "Disable certain things so it builds on GCC 7.2.0" OFF)

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

# Winsock
if (WIN32)
  target_link_libraries(${TARGET} "-lws2_32")
endif()

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

# Aggressive warnings
set(COMMON_FLAGS "-Wall -Wextra -Wconversion -Wpedantic -pedantic -Wformat=2 -Wnull-dereference -Wuninitialized -Wstrict-aliasing=2 -Wfloat-equal -Wshadow -Wpointer-arith -Wwrite-strings -Wcast-qual -Wdate-time")
# Hardening and fail-fast behavior
if (SANITIZE)
  set(COMMON_FLAGS "${COMMON_FLAGS} -fsanitize=address -fsanitize=undefined -fstack-protector-all -fsanitize-address-use-after-scope")
endif()
# Verbose debugging information and tracking
set(COMMON_FLAGS "${COMMON_FLAGS} -gdwarf-4 -ggdb3 -fno-omit-frame-pointer")

# Check for compiler specific options
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Aggressive warnings
  set(COMMON_FLAGS "${COMMON_FLAGS} -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-gnu-zero-variadic-macro-arguments")
  # Hardening and fail-fast behavior
  set(COMMON_FLAGS "${COMMON_FLAGS} -fstack-check")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # Aggressive warnings
  set(COMMON_FLAGS "${COMMON_FLAGS} -Wlogical-op -Wformat-overflow=2 -Wformat-signedness -Wformat-truncation=2 -Wduplicated-cond -Walloca -Warray-bounds=2 -Wtrampolines")
  if (!I_SHOULD_UPDATE_MINGW)
    set(COMMON_FLAGS "${COMMON_FLAGS} -Wstringop-truncation -Wcast-align=strict")
  endif()
  # Hardening and fail-fast behavior
  if (SANITIZE)
    set(COMMON_FLAGS "${COMMON_FLAGS} -static-libubsan")
    set(COMMON_FLAGS "${COMMON_FLAGS} -fstack-clash-protection -fcf-protection=full")
  endif()
  # Verbose debugging information and tracking
  set(COMMON_FLAGS "${COMMON_FLAGS} -fvar-tracking -fvar-tracking-assignments")
  # Hinting suggestions
  if (HINT_SUGGESTIONS)
    set(COMMON_FLAGS "${COMMON_FLAGS} -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=malloc -Wsuggest-attribute=format -Wmissing-format-attribute -Wsuggest-attribute=cold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override")
    set(COMMON_FLAGS "${COMMON_FLAGS} -Wvector-operation-performance -Winline")
  endif()
  if (!I_SHOULD_UPDATE_MINGW)
    # Display template errors/warnings in a slightly more legible way
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-show-template-tree")
  endif()
endif()

# Apply options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS} -std=c11 -Wc++-compat -Wbad-function-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} -std=c++11")
