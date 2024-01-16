##########################################
#
# Maze Engine
# Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
#
# This software is provided 'as-is', without any express or implied warranty.
# In no event will the authors be held liable for any damages arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it freely,
# subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented;
#    you must not claim that you wrote the original software.
#    If you use this software in a product, an acknowledgment
#    in the product documentation would be appreciated but is not required.
#
# 2. Altered source versions must be plainly marked as such,
#    and must not be misrepresented as being the original software.
#
# 3. This notice may not be removed or altered from any source distribution.
#
##########################################


##########################################
include("${CMAKE_CURRENT_LIST_DIR}/zlib_info.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/pkgconfig_info.cmake")


##########################################
set(ALSOFT_UTILS OFF CACHE BOOL "" FORCE)
set(ALSOFT_NO_CONFIG_UTIL ON CACHE BOOL "" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_CONFIG OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_HRTF_DATA OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_UTILS OFF CACHE BOOL "" FORCE)
set(ALSOFT_UPDATE_BUILD_VERSION OFF CACHE BOOL "" FORCE)
set(ALSOFT_BACKEND_PULSEAUDIO OFF CACHE BOOL "" FORCE)
set(ALSOFT_BACKEND_ALSA OFF CACHE BOOL "" FORCE)
set(ALSOFT_BACKEND_OSS OFF CACHE BOOL "" FORCE)
set(ALSOFT_BACKEND_SOLARIS OFF CACHE BOOL "" FORCE)
set(ALSOFT_BACKEND_SNDIO OFF CACHE BOOL "" FORCE)
set(ALSOFT_BACKEND_JACK OFF CACHE BOOL "" FORCE)
set(ALSOFT_BACKEND_OBOE OFF CACHE BOOL "" FORCE)
set(ALSOFT_BACKEND_PORTAUDIO OFF CACHE BOOL "" FORCE)
set(ALSOFT_BACKEND_PIPEWIRE OFF CACHE BOOL "" FORCE)
set(ALSOFT_RTKIT OFF CACHE BOOL "" FORCE)

add_subdirectory("${MAZE_DIR}/third-party/openal-soft" "${CMAKE_CURRENT_BINARY_DIR}/third-party/openal-soft")
set_property(TARGET OpenAL PROPERTY FOLDER "MazeThirdParty")
target_include_directories(
    OpenAL
    PUBLIC $<BUILD_INTERFACE:${MAZE_DIR}/third-party/openal-soft/include>)
set_target_properties(OpenAL
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>")