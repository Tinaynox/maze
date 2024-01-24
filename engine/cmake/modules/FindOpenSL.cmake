find_path(OPENSL_INCLUDE_DIR NAMES SLES/OpenSLES.h
    DOC "The OpenSL include directory")
find_path(OPENSL_ANDROID_INCLUDE_DIR NAMES SLES/OpenSLES_Android.h
    DOC "The OpenSL Android include directory")

find_library(OPENSL_LIBRARY NAMES OpenSLES
    DOC "The OpenSL library")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenSL REQUIRED_VARS OPENSL_LIBRARY OPENSL_INCLUDE_DIR
    OPENSL_ANDROID_INCLUDE_DIR)

if(OPENSL_FOUND)
    set(OPENSL_LIBRARIES ${OPENSL_LIBRARY})
    set(OPENSL_INCLUDE_DIRS ${OPENSL_INCLUDE_DIR} ${OPENSL_ANDROID_INCLUDE_DIR})
endif()

mark_as_advanced(OPENSL_INCLUDE_DIR OPENSL_ANDROID_INCLUDE_DIR OPENSL_LIBRARY)
