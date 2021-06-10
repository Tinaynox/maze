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
function(maze_is_valid_source_path PATH IS_VALID)

    set(IS_VALID 1 PARENT_SCOPE)

    if(NOT MAZE_TARGET_PLATFORM_IS_WINDOWS)
        STRING(FIND ${PATH} "/win/" TEST)
        if(NOT ${TEST} EQUAL -1)
            set(IS_VALID 0 PARENT_SCOPE)
            return()
        endif()
    endif()

    if(NOT MAZE_TARGET_PLATFORM_IS_UNIX)
        STRING(FIND ${PATH} "/unix/" TEST)
        if(NOT ${TEST} EQUAL -1)
            set(IS_VALID 0 PARENT_SCOPE)
            return()
        endif()
    endif()

    if(NOT MAZE_TARGET_PLATFORM_IS_OSX)
        STRING(FIND ${PATH} "/osx/" TEST)
        if(NOT ${TEST} EQUAL -1)
            set(IS_VALID 0 PARENT_SCOPE)
            return()
        endif()
    endif()

    if(NOT MAZE_TARGET_PLATFORM_IS_IOS)
        STRING(FIND ${PATH} "/ios/" TEST)
        if(NOT ${TEST} EQUAL -1)
            set(IS_VALID 0 PARENT_SCOPE)
            return()
        endif()
    endif()

    if(NOT MAZE_TARGET_PLATFORM_IS_ANDROID)
        STRING(FIND ${PATH} "/android/" TEST)
        if(NOT ${TEST} EQUAL -1)
            set(IS_VALID 0 PARENT_SCOPE)
            return()
        endif()
    endif()

    if(NOT MAZE_TARGET_PLATFORM_IS_LINUX)
        STRING(FIND ${PATH} "/linux/" TEST)
        if(NOT ${TEST} EQUAL -1)
            set(IS_VALID 0 PARENT_SCOPE)
            return()
        endif()
    endif()
    
    if(NOT MAZE_TARGET_PLATFORM_IS_EMSCRIPTEN)
        STRING(FIND ${PATH} "/emscripten/" TEST)
        if(NOT ${TEST} EQUAL -1)
            set(IS_VALID 0 PARENT_SCOPE)
            return()
        endif()
    endif()


endfunction()


##########################################
macro(maze_add_sources PATH SRC_FILES)

    file(GLOB_RECURSE TMP_FILES ${PATH}/*.*)

    list(LENGTH TMP_FILES FILES_COUNT)
    MATH(EXPR INDEX_I 0)
    MATH(EXPR INDEX_IN "${FILES_COUNT}")
    while(      NOT INDEX_I EQUAL ${INDEX_IN}
            AND NOT INDEX_I GREATER ${INDEX_IN})
        list(GET TMP_FILES ${INDEX_I} ELEM_I)

        maze_is_valid_source_path(${ELEM_I} IS_VALID)

        if(NOT ${IS_VALID})
            list(REMOVE_AT TMP_FILES ${INDEX_I})
            MATH(EXPR INDEX_IN "${INDEX_IN}-1")
        else()
            MATH(EXPR INDEX_I "${INDEX_I}+1")
        endif()

    endwhile()


    get_filename_component(PATH_ABSOLUTE ${PATH} ABSOLUTE)

    foreach(FILENAME ${TMP_FILES})

        #file(RELATIVE_PATH FILENAME_RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${FILENAME})
        file(RELATIVE_PATH FILENAME_RELATIVE ${PATH_ABSOLUTE} ${FILENAME})
        set(FILENAME_RELATIVE "src/${FILENAME_RELATIVE}")
        string(REPLACE "../" "" FILENAME_RELATIVE ${FILENAME_RELATIVE})
        string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRC_GROUP ${FILENAME_RELATIVE})
        string(REPLACE / \\ SRC_GROUP ${SRC_GROUP})
        source_group("${SRC_GROUP}" FILES ${FILENAME})

    endforeach(FILENAME)


    set(${SRC_FILES} ${TMP_FILES})

endmacro()


##########################################
macro(maze_sort_sources SRC_FILES SORTED_FILES)

    set(TEMP_FILES "${SRC_FILES}")

    list(LENGTH TEMP_FILES FILES_COUNT)
    MATH(EXPR FILES_COUNT_MINUS_ONE "${FILES_COUNT}-1")

    MATH(EXPR INDEX_I 0)
    MATH(EXPR INDEX_IN "${FILES_COUNT}")
    while(NOT INDEX_I EQUAL ${INDEX_IN})

        MATH(EXPR INDEX_J 0)
        MATH(EXPR INDEX_JN "${FILES_COUNT}-${INDEX_I}-1")
        while(NOT INDEX_J EQUAL ${INDEX_JN})
            MATH(EXPR INDEX_J2 "${INDEX_J} + 1")

            list(GET TEMP_FILES ${INDEX_J} ELEM_J)
            list(GET TEMP_FILES ${INDEX_J2} ELEM_J2)
            get_filename_component(FILENAME_J ${ELEM_J} NAME)
            get_filename_component(FILENAME_J2 ${ELEM_J2} NAME)

            if(${FILENAME_J} STRGREATER ${FILENAME_J2})


                list(REMOVE_AT TEMP_FILES ${INDEX_J})

                if(${INDEX_J2} EQUAL ${FILES_COUNT_MINUS_ONE})
                    list(APPEND TEMP_FILES ${ELEM_J})
                else()
                    list(INSERT TEMP_FILES ${INDEX_J2} ${ELEM_J})
                endif()

            else()

            endif()


            MATH(EXPR INDEX_J "${INDEX_J}+1")
        endwhile()

        MATH(EXPR INDEX_I "${INDEX_I}+1")
    endwhile()




    set(${SORTED_FILES} "${TEMP_FILES}")



endmacro()



##########################################
# This little macro lets you set any Xcode specific property
#
##########################################
macro(maze_set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
    set_property(TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
endmacro ()


##########################################
# Set the appropriate standard library on each platform for the given target
# example: maze_setup_stdlib(gg-core)
#
##########################################
function(maze_setup_stdlib TARGET)
    # for gcc >= 4.0 on Windows, apply the MAZE_USE_STATIC_STD_LIBS option if it is enabled
    if(MAZE_TARGET_PLATFORM_IS_WINDOWS AND MAZE_COMPILER_IS_GCC AND NOT MAZE_GCC_VERSION VERSION_LESS "4")
        if(MAZE_USE_STATIC_STD_LIBS AND NOT MAZE_GCC_TDM)
            target_link_libraries(${TARGET} PRIVATE "-static-libgcc" "-static-libstdc++")
        elseif(NOT MAZE_USE_STATIC_STD_LIBS AND MAZE_COMPILER_GCC_TDM)
            target_link_libraries(${TARGET} PRIVATE "-shared-libgcc" "-shared-libstdc++")
        endif()
    endif()


    if(MAZE_TARGET_PLATFORM_IS_OSX)

        if(${CMAKE_GENERATOR} MATCHES "Xcode")
            maze_set_xcode_property(${TARGET} CLANG_CXX_LIBRARY "libc++")
            maze_set_xcode_property(${TARGET} CLANG_CXX_LANGUAGE_STANDARD "c++14")
        else()
            target_compile_options(${TARGET} PRIVATE "-stdlib=libc++ -std=c++11")
            #target_link_libraries(${TARGET} PRIVATE "-stdlib=libc++ -std=c++11")
        endif()

    endif()

    if(MAZE_TARGET_PLATFORM_IS_IOS)

        if(${CMAKE_GENERATOR} MATCHES "Xcode")
            maze_set_xcode_property(${TARGET} CLANG_CXX_LIBRARY "libc++")
            maze_set_xcode_property(${TARGET} CLANG_CXX_LANGUAGE_STANDARD "c++14")
        else()
            target_compile_options(${TARGET} PRIVATE "-stdlib=libc++ -std=c++11")
            #target_link_libraries(${TARGET} PRIVATE "-stdlib=libc++ -std=c++11")
        endif()

    endif()

    if(MAZE_TARGET_PLATFORM_IS_EMSCRIPTEN)

        # target_compile_definitions(${TARGET} PRIVATE "-stdlib=libc++ -std=c++11")
        target_compile_options(${TARGET} PRIVATE "-std=c++11")

    endif()



endfunction()


##########################################
# Add a new GG module library
# Example: maze_add_module(    gg-graphics
#                              INCLUDE_DIR include/gg/core
#                              SRC_DIR src/gg/core
#                              [STATIC]) # Always create a static library and ignore BUILD_SHARED_LIBS
#
##########################################
macro(maze_add_module MODULE_NAME)

    cmake_parse_arguments(MODULE "STATIC" "" "INCLUDE_DIR;SRC_DIR;FORWARD_HEADER" ${ARGN})
    if(NOT "${MODULE_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling target: ${MODULE_UNPARSED_ARGUMENTS}")
    endif()


    maze_add_sources(${MODULE_SRC_DIR} MODULE_SRC_FILES)
    maze_add_sources(${MODULE_INCLUDE_DIR} MODULE_INCLUDE_FILES)
    set(MODULE_FILES ${MODULE_SRC_FILES} ${MODULE_INCLUDE_FILES})
    maze_sort_sources("${MODULE_FILES}" MODULE_FILES)




    if(MAZE_COMPILER_IS_MSVC)

        foreach(MODULE_FILE ${MODULE_FILES})
            set_source_files_properties(
                ${MODULE_FILE}
                PROPERTIES
                COMPILE_FLAGS "/Yu${MODULE_FORWARD_HEADER}.hpp")
        endforeach()

        set_source_files_properties(${MODULE_SRC_DIR}/${MODULE_FORWARD_HEADER}.cpp
            PROPERTIES
            COMPILE_FLAGS "/Yc${MODULE_FORWARD_HEADER}.hpp")

    endif()
    

    if(MODULE_STATIC)
        add_library(${MODULE_NAME} STATIC ${MODULE_FILES})
    else()
        add_library(${MODULE_NAME} ${MODULE_FILES})
    endif()

    # Add <project>/third-party as public include directory
    target_include_directories(${MODULE_NAME}
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/third-party>)

    
    set(MAZE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}")
    set_target_properties(${MODULE_NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${MAZE_LIBRARY_OUTPUT_DIRECTORY}"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${MAZE_LIBRARY_OUTPUT_DIRECTORY}"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${MAZE_LIBRARY_OUTPUT_DIRECTORY}"
        LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MAZE_LIBRARY_OUTPUT_DIRECTORY}"
        RUNTIME_OUTPUT_DIRECTORY "${MAZE_LIBRARY_OUTPUT_DIRECTORY}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${MAZE_LIBRARY_OUTPUT_DIRECTORY}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MAZE_LIBRARY_OUTPUT_DIRECTORY}"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MAZE_LIBRARY_OUTPUT_DIRECTORY}"
        ARCHIVE_OUTPUT_DIRECTORY "${MAZE_LIBRARY_OUTPUT_DIRECTORY}"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${MAZE_LIBRARY_OUTPUT_DIRECTORY}"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${MAZE_LIBRARY_OUTPUT_DIRECTORY}"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MAZE_LIBRARY_OUTPUT_DIRECTORY}")


    if(MAZE_TARGET_PLATFORM_IS_APPLE)

        #set_target_properties(${MODULE_NAME} PROPERTIES
        #            XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES"
        #            XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${PROJECT_SOURCE_DIR}/${MODULE_SRC_DIR}/${MODULE_FORWARD_HEADER}.hpp")
    endif()



    # Define the export symbol of the module
    string(REPLACE "-" "_" MODULE_NAME_UPPER "${MODULE_NAME}")
    string(TOUPPER "${MODULE_NAME_UPPER}" MODULE_NAME_UPPER)
    set_target_properties(${MODULE_NAME} PROPERTIES DEFINE_SYMBOL ${MODULE_NAME_UPPER}_EXPORTS)

    if(BUILD_SHARED_LIBS AND NOT MODULE_STATIC)

        if(MAZE_TARGET_PLATFORM_IS_WINDOWS)
        
            set(MODULE_POSTFIX "-${MAZE_ARCH_SUFFIX}")
        
            set_target_properties(${MODULE_NAME} PROPERTIES DEBUG_POSTFIX "${MODULE_POSTFIX}-d")
            set_target_properties(${MODULE_NAME} PROPERTIES RELEASE_POSTFIX "${MODULE_POSTFIX}")
            set_target_properties(${MODULE_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "${MODULE_POSTFIX}")
            
            
            
            if(VERSION_MAJOR)
                # Include the major version number in Windows shared library names (but not import library names)
                set(MODULE_SUFFIX "${MODULE_SUFFIX}-${VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX}")
            endif()
            
            if(MODULE_SUFFIX)
                set_target_properties(${MODULE_NAME} PROPERTIES SUFFIX "${MODULE_SUFFIX}")
            endif()
            
        else()
            set_target_properties(${MODULE_NAME} PROPERTIES DEBUG_POSTFIX -d)
        endif()

        if(MAZE_TARGET_PLATFORM_IS_WINDOWS AND MAZE_COMPILER_IS_GCC)

            # on Windows/gcc get rid of "lib" prefix for shared libraries,
            # and transform the ".dll.a" suffix into ".a" for import libraries
            set_target_properties(${MODULE_NAME} PROPERTIES PREFIX "")
            set_target_properties(${MODULE_NAME} PROPERTIES IMPORT_SUFFIX ".a")

        endif()

    else()

        set_target_properties(${MODULE_NAME} PROPERTIES DEBUG_POSTFIX -s-d)
        set_target_properties(${MODULE_NAME} PROPERTIES RELEASE_POSTFIX -s)
        set_target_properties(${MODULE_NAME} PROPERTIES MINSIZEREL_POSTFIX -s)
        set_target_properties(${MODULE_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX -s)
    endif()


    # Set the version and soversion of the target (for compatible systems -- mostly Linuxes)
    # except for Android which strips soversion suffixes
    if(NOT MAZE_TARGET_PLATFORM_IS_ANDROID)

        if(VERSION_MAJOR AND VERSION_MINOR)
            set_target_properties(${MODULE_NAME} PROPERTIES SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR})
        endif()

        if(VERSION_MAJOR AND VERSION_MINOR AND VERSION_PATCH)
            set_target_properties(${MODULE_NAME} PROPERTIES VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
        endif()

    endif()

    set_property(TARGET ${MODULE_NAME} PROPERTY FOLDER "MazeEngine")

    # Setup stdlib
    maze_setup_stdlib(${MODULE_NAME})

    # For Visual Studio on Windows, export debug symbols (PDB files) to lib directory
    if(MAZE_GENERATE_PDB)

        # PDB files are only generated in Debug and RelWithDebInfo configurations, find out which one
        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            set(MAZE_PDB_POSTFIX "-d")
        else()
            set(MAZE_PDB_POSTFIX "")
        endif()

        if(BUILD_SHARED_LIBS AND NOT MODULE_STATIC)
            # DLLs export debug symbols in the linker PDB (the compiler PDB is an intermediate file)
            set_target_properties(${MODULE_NAME} PROPERTIES
                                   PDB_NAME "${MODULE_NAME}${MAZE_PDB_POSTFIX}"
                                   PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/${MAZE_ARCH_SUFFIX}")
        else()
            # Static libraries have no linker PDBs, thus the compiler PDBs are relevant
            set_target_properties(${MODULE_NAME} PROPERTIES
                                  COMPILE_PDB_NAME "${MODULE_NAME}-s${MAZE_PDB_POSTFIX}"
                                  COMPILE_PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/${MAZE_ARCH_SUFFIX}")
        endif()

    endif()


    # If using gcc >= 4.0 or clang >= 3.0 on a non-Windows platform, we must hide public symbols by default
    # (exported ones are explicitly marked)
    if(NOT MAZE_TARGET_PLATFORM_IS_WINDOWS AND ((MAZE_COMPILER_IS_GCC AND NOT MAZE_GCC_VERSION VERSION_LESS "4") OR (MAZE_COMPILER_IS_CLANG AND NOT MAZE_CLANG_VERSION VERSION_LESS "3")))
        set_target_properties(${MODULE_NAME} PROPERTIES COMPILE_FLAGS -fvisibility=hidden)
    endif()



    # Build frameworks or dylibs
    if(MAZE_OS_MACOSX AND BUILD_SHARED_LIBS AND NOT MODULE_STATIC)
        if(MAZE_BUILD_FRAMEWORKS)

            # Adapt target to build frameworks instead of dylibs
            set_target_properties(${MODULE_NAME} PROPERTIES
                                   FRAMEWORK TRUE
                                   FRAMEWORK_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                                   MACOSX_FRAMEWORK_IDENTIFIER com.groovy-gears.${MODULE_NAME}
                                   MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                                   MACOSX_FRAMEWORK_BUNDLE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
        endif()

        # Adapt install directory to allow distributing dylibs/frameworks in user's frameworks/application bundle
        # but only if cmake rpath options aren't set
        if(NOT CMAKE_SKIP_RPATH AND NOT CMAKE_SKIP_INSTALL_RPATH AND NOT CMAKE_INSTALL_RPATH AND NOT CMAKE_INSTALL_RPATH_USE_LINK_PATH AND NOT CMAKE_INSTALL_NAME_DIR)
            set_target_properties(${MODULE_NAME} PROPERTIES INSTALL_NAME_DIR "@rpath")
            if(NOT CMAKE_SKIP_BUILD_RPATH)
                if(CMAKE_VERSION VERSION_LESS 3.9)
                    set_target_properties(${MODULE_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
                else()
                    set_target_properties(${MODULE_NAME} PROPERTIES BUILD_WITH_INSTALL_NAME_DIR TRUE)
                endif()
            endif()
        endif()
    endif()


    # Enable automatic reference counting on iOS
    if(MAZE_TARGET_PLATFORM_IS_IOS)
        set_target_properties(${MODULE_NAME} PROPERTIES XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES)
    endif()



    # Add <project>/include as public include directory
    target_include_directories(${MODULE_NAME}
                               PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                               PRIVATE ${PROJECT_SOURCE_DIR}/src)

    target_include_directories(${MODULE_NAME}
                               PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${MODULE_INCLUDE_DIR}>
                               PRIVATE ${PROJECT_SOURCE_DIR}/${MODULE_SRC_DIR})


    if(MAZE_BUILD_FRAMEWORKS)
        target_include_directories(${MODULE_NAME} INTERFACE $<INSTALL_INTERFACE:GG.framework>)
    else()
        target_include_directories(${MODULE_NAME} INTERFACE $<INSTALL_INTERFACE:include>)
    endif()

    # Define MAZE_STATIC if the build type is not set to 'shared'
    if(NOT BUILD_SHARED_LIBS)
        target_compile_definitions(${MODULE_NAME} PUBLIC "MAZE_STATIC")
    endif()

endmacro()


##########################################
# Add a new GG example
# Example: maze_add_example(    gg-example-hello-world
#                               DIR "examples/gg-example-hello-world"
#
##########################################
macro(maze_add_example EXAMPLE_NAME)

    cmake_parse_arguments(EXAMPLE "STATIC" "" "DIR" ${ARGN})
    if(NOT "${EXAMPLE_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling target: ${EXAMPLE_UNPARSED_ARGUMENTS}")
    endif()


    maze_add_sources(${EXAMPLE_DIR} EXAMPLE_FILES)
    maze_sort_sources("${EXAMPLE_FILES}" EXAMPLE_FILES)


    if(MAZE_COMPILER_IS_MSVC)
        set(USER_FILE ${EXAMPLE_NAME}.vcxproj.user)
        set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})
        configure_file(${PROJECT_SOURCE_DIR}/examples/Example.options ${USER_FILE} @ONLY)
    endif()

    if(MAZE_COMPILER_IS_CLANG)
        add_compile_options(-Wno-undefined-var-template)
      endif()

    if(MAZE_TARGET_PLATFORM_IS_ANDROID)
        add_library(${EXAMPLE_NAME} ${EXAMPLE_FILES})
    else()
        add_executable(${EXAMPLE_NAME} ${EXAMPLE_FILES})
    endif()


    set_property(TARGET ${EXAMPLE_NAME} PROPERTY FOLDER "GGExamples")

    # Setup stdlib
    maze_setup_stdlib(${EXAMPLE_NAME})




    # Add <project>/include as public include directory
    target_include_directories(${EXAMPLE_NAME}
                               PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)


    target_link_libraries(${EXAMPLE_NAME} gg-core)

endmacro()
