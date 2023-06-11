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
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../engine" "${CMAKE_CURRENT_BINARY_DIR}/MazeEngine")


##########################################
include("${CMAKE_CURRENT_SOURCE_DIR}/../../engine/cmake/Config.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../../engine/cmake/Macros.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../../engine/cmake/Utils.cmake")


##########################################
maze_add_sources(${CMAKE_CURRENT_SOURCE_DIR}/src TOOL_FILES)
maze_sort_sources("${TOOL_FILES}" TOOL_FILES)


##########################################
if(MAZE_COMPILER_IS_MSVC)
    set(USER_FILE ${TOOL_NAME}.vcxproj.user)
    set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../templates/Example.options ${USER_FILE} @ONLY)
endif()

if(MAZE_COMPILER_IS_CLANG)
    add_compile_options(-Wno-undefined-var-template)
endif()






if(MAZE_TARGET_PLATFORM_IS_ANDROID)

    add_library(${TOOL_NAME} ${TOOL_FILES} ${ADDITIONAL_TOOL_FILES})
    
elseif(MAZE_TARGET_PLATFORM_IS_OSX)

    set(APPLE_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../sys-res/osx/Resources)
    file(GLOB APPLE_RESOURCES_GLOB "${APPLE_RESOURCES}/*.*")
    list(REMOVE_ITEM APPLE_RESOURCES_GLOB ${APPLE_RESOURCES}/.DS_Store)

    set(APP_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../prj/resources/resources-x1.0-nc/data)
    if(NOT EXISTS ${APP_RESOURCES})
        set(APP_RESOURCES "")
    endif()

    add_executable(${TOOL_NAME} MACOSX_BUNDLE ${TOOL_FILES} ${ADDITIONAL_TOOL_FILES} ${APPLE_RESOURCES_GLOB} ${APP_RESOURCES} ${TOOL_FRAMEWORKS})
    set_target_properties(${TOOL_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/../sys-res/osx/InfoOSX.plist)
    set_source_files_properties(${APPLE_RESOURCES_GLOB} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${APP_RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${TOOL_FRAMEWORKS} PROPERTIES MACOSX_PACKAGE_LOCATION Frameworks)
    

    set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../engine/lib/${MAZE_ARCH_SUFFIX}")
    set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../prj/${TOOL_NAME}/build.${MAZE_ARCH_SUFFIX}")

    set_target_properties(${TOOL_NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${BUILD_DIR}"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${BUILD_DIR}"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${BUILD_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BUILD_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BUILD_DIR}")


    if(BUILD_SHARED_LIBS AND NOT MODULE_STATIC)
    

        add_custom_command(TARGET ${TOOL_NAME}
            POST_BUILD 
            COMMAND ${CMAKE_COMMAND} -E make_directory ${BUILD_DIR}/${TOOL_NAME}.app/Contents/Frameworks/
            COMMAND ${CMAKE_COMMAND} -E copy ${LIB_DIR}/*.dylib ${BUILD_DIR}/${TOOL_NAME}.app/Contents/Frameworks/ )


    endif()

    set_target_properties(${TOOL_NAME} PROPERTIES INSTALL_RPATH "@loader_path/../lib;@loader_path/../Frameworks;@executable_path/../lib;@executable_path/../Frameworks") 

else()

    add_executable(${TOOL_NAME} ${TOOL_FILES} ${ADDITIONAL_TOOL_FILES})

    if(MAZE_TARGET_PLATFORM_IS_LINUX)

        set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../prj/${TOOL_NAME}/build.${MAZE_ARCH_SUFFIX}")

        set_target_properties(${TOOL_NAME}
            PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${BUILD_DIR}"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG "${BUILD_DIR}"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${BUILD_DIR}"
            RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BUILD_DIR}"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BUILD_DIR}"
            ARCHIVE_OUTPUT_DIRECTORY "${BUILD_DIR}"
            ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${BUILD_DIR}"
            ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${BUILD_DIR}")

        ADD_CUSTOM_TARGET(run
              COMMAND ${TOOL_NAME}
              WORKING_DIRECTORY ${BUILD_DIR})


    endif()
    
endif()


if(MAZE_TARGET_PLATFORM_IS_EMSCRIPTEN)
    append_linker_flags(${TOOL_NAME} "-s DEMANGLE_SUPPORT=1 --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/../../prj/${TOOL_NAME}/resources/resources-x1.0-nc@/ --bind --emrun -v")
endif()


set_property(TARGET ${TOOL_NAME} PROPERTY FOLDER "MazeTools")

maze_setup_stdlib(${TOOL_NAME})

# Add <project>/include as public include directory
target_include_directories(
    ${TOOL_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${TOOL_NAME} ${TOOL_MAZE_LIBS})


if(MAZE_TARGET_PLATFORM_IS_WINDOWS)
    if(BUILD_SHARED_LIBS AND NOT MODULE_STATIC)
    
        STRING(REGEX REPLACE "/" "\\\\" COPY_SRC \"${CMAKE_CURRENT_SOURCE_DIR}/../../engine/lib/${MAZE_ARCH_SUFFIX}/*.dll\")
        STRING(REGEX REPLACE "/" "\\\\" COPY_DST \"${CMAKE_CURRENT_SOURCE_DIR}/../../prj/${TOOL_NAME}/build.${MAZE_ARCH_SUFFIX}/\")
            
        add_custom_command(TARGET ${TOOL_NAME} POST_BUILD
            COMMAND COMMAND copy ${COPY_SRC} ${COPY_DST}
            COMMENT "Copying Maze dll's...\n")
            
    endif()
endif()