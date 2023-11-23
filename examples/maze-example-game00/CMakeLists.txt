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
cmake_minimum_required(VERSION 3.6)


##########################################
project(maze-example-game00)


##########################################
set(EXAMPLE_NAME "${PROJECT_NAME}")
set(EXAMPLE_MAZE_LIBS
    maze-core
    maze-graphics
    maze-ui
    maze-gamepad
    maze-engine
    maze-editor-tools
    maze-plugin-loader-png)


##########################################
include("${CMAKE_CURRENT_SOURCE_DIR}/../../engine/cmake/Utils.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../../engine/cmake/Config.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../../engine/cmake/Macros.cmake")


##########################################
if(MAZE_RENDER_SYSTEM_OPENGL_ENABLED)
    list(APPEND EXAMPLE_MAZE_LIBS maze-render-system-opengl-core maze-render-system-opengl3)
endif()


##########################################
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../engine" "${CMAKE_CURRENT_BINARY_DIR}/MazeEngine")


##########################################
add_subdirectory("${MAZE_DIR}/plugins/loader-png" "${CMAKE_CURRENT_BINARY_DIR}/plugins/loader-png")


##########################################
maze_add_sources(${CMAKE_CURRENT_SOURCE_DIR}/src EXAMPLE_FILES)
maze_sort_sources("${EXAMPLE_FILES}" EXAMPLE_FILES)


##########################################
if(MAZE_COMPILER_IS_MSVC)
    set(USER_FILE ${EXAMPLE_NAME}.vcxproj.user)
    set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../templates/Example.options ${USER_FILE} @ONLY)
endif()

if(MAZE_COMPILER_IS_CLANG)
    add_compile_options(-Wno-undefined-var-template)
endif()






if(MAZE_TARGET_PLATFORM_IS_ANDROID)

    add_library(${EXAMPLE_NAME} ${EXAMPLE_FILES})
    
elseif(MAZE_TARGET_PLATFORM_IS_OSX)

    #set(CMAKE_MACOSX_RPATH ON) 
    #set(CMAKE_SKIP_BUILD_RPATH TRUE)
    #set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    #set(CMAKE_INSTALL_RPATH "")
    #set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


    set(APPLE_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../sys-res/osx/Resources)
    file(GLOB APPLE_RESOURCES_GLOB "${APPLE_RESOURCES}/*.*")
    list(REMOVE_ITEM APPLE_RESOURCES_GLOB ${APPLE_RESOURCES}/.DS_Store)

    set(APP_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../_otp/prj/resources/resources-x1.0-nc/data)

    add_executable(${EXAMPLE_NAME} MACOSX_BUNDLE ${EXAMPLE_FILES} ${APPLE_RESOURCES_GLOB} ${APP_RESOURCES} ${EXAMPLE_FRAMEWORKS})
    set_target_properties(${EXAMPLE_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/../sys-res/osx/InfoOSX.plist)
    set_source_files_properties(${APPLE_RESOURCES_GLOB} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${APP_RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${EXAMPLE_FRAMEWORKS} PROPERTIES MACOSX_PACKAGE_LOCATION Frameworks)
    

    set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../engine/lib/${MAZE_ARCH_SUFFIX}")
    set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../_otp/prj/${EXAMPLE_NAME}/build.${MAZE_ARCH_SUFFIX}")

    set_target_properties(${EXAMPLE_NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${BUILD_DIR}"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${BUILD_DIR}"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${BUILD_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BUILD_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BUILD_DIR}")


    if(BUILD_SHARED_LIBS AND NOT MODULE_STATIC)
    

        add_custom_command(TARGET ${EXAMPLE_NAME}
            POST_BUILD 
            COMMAND    ${CMAKE_COMMAND} -E make_directory ${BUILD_DIR}/${EXAMPLE_NAME}.app/Contents/Frameworks/
            COMMAND ${CMAKE_COMMAND} -E copy ${LIB_DIR}/*.dylib ${BUILD_DIR}/${EXAMPLE_NAME}.app/Contents/Frameworks/ )


    endif()

    set_target_properties(${EXAMPLE_NAME} PROPERTIES INSTALL_RPATH "@loader_path/../lib;@loader_path/../Frameworks;@executable_path/../lib;@executable_path/../Frameworks") 

    

else()


    add_executable(${EXAMPLE_NAME} ${EXAMPLE_FILES})



    if(MAZE_TARGET_PLATFORM_IS_LINUX)

        set (BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../_otp/prj/${EXAMPLE_NAME}/build.${MAZE_ARCH_SUFFIX}")

        set_target_properties(${EXAMPLE_NAME}
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
              COMMAND ${EXAMPLE_NAME}
              WORKING_DIRECTORY ${BUILD_DIR})


    endif()
    
endif()


if(MAZE_TARGET_PLATFORM_IS_EMSCRIPTEN)

    append_linker_flags(${EXAMPLE_NAME} "-s DEMANGLE_SUPPORT=1 --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/../../_otp/prj/${EXAMPLE_NAME}/resources/resources-x1.0-nc@/ --bind --emrun -v")

endif()


set_property(TARGET ${EXAMPLE_NAME} PROPERTY FOLDER "MazeExamples")

maze_setup_stdlib(${EXAMPLE_NAME})

# Add <project>/include as public include directory
target_include_directories(
    ${EXAMPLE_NAME}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


target_link_libraries(${EXAMPLE_NAME} ${EXAMPLE_MAZE_LIBS})


if(MAZE_TARGET_PLATFORM_IS_WINDOWS)
    if(BUILD_SHARED_LIBS AND NOT MODULE_STATIC)
    
        STRING(REGEX REPLACE "/" "\\\\" COPY_SRC \"${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>/*.dll\")
        STRING(REGEX REPLACE "/" "\\\\" COPY_DST \"${MAZE_OUTPUT_DIR}/prj/${EXAMPLE_NAME}/build.${MAZE_ARCH_SUFFIX}/\")
    
        add_custom_command(TARGET ${EXAMPLE_NAME} POST_BUILD
            COMMAND COMMAND copy ${COPY_SRC} ${COPY_DST}
            COMMENT "Copying Maze dll's...\n")
            
        #foreach(EXAMPLE_MAZE_LIB ${EXAMPLE_MAZE_LIBS})
        #
        #    get_target_property(EXAMPLE_MAZE_LIB_DEBUG_POSTFIX ${EXAMPLE_MAZE_LIB} DEBUG_POSTFIX)
        #    get_target_property(EXAMPLE_MAZE_LIB_RELEASE_POSTFIX ${EXAMPLE_MAZE_LIB} RELEASE_POSTFIX)
        #    get_target_property(EXAMPLE_MAZE_LIB_RELWITHDEBINFO_POSTFIX ${EXAMPLE_MAZE_LIB} RELWITHDEBINFO_POSTFIX)
        #
        #    set(EXAMPLE_MAZE_DLL_DEBUG "${EXAMPLE_MAZE_LIB}${EXAMPLE_MAZE_LIB_DEBUG_POSTFIX}.dll")
        #    set(EXAMPLE_MAZE_DLL_RELEASE "${EXAMPLE_MAZE_LIB}${EXAMPLE_MAZE_LIB_RELEASE_POSTFIX}.dll")
        #    set(EXAMPLE_MAZE_DLL_RELWITHDEBINFO "${EXAMPLE_MAZE_LIB}${EXAMPLE_MAZE_LIB_RELWITHDEBINFO_POSTFIX}.dll")
        #    
        #    append_linker_flags(${EXAMPLE_NAME} DEBUG "/DELAYLOAD:${EXAMPLE_MAZE_DLL_DEBUG}")
        #    append_linker_flags(${EXAMPLE_NAME} RELEASE "/DELAYLOAD:${EXAMPLE_MAZE_DLL_RELEASE}")
        #    append_linker_flags(${EXAMPLE_NAME} RELWITHDEBINFO "/DELAYLOAD:${EXAMPLE_MAZE_DLL_RELWITHDEBINFO}")
        #        
        #endforeach(EXAMPLE_MAZE_LIB)
            
    endif()
endif()