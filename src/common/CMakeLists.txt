cmake_minimum_required(VERSION 3.1)

## start setting
SET (this_target common_new)
PROJECT(${this_target})


## section: source files
# Add your source files here (one file per line)
# listed with $(ls -1 *.cpp)
SET (${this_target}_SOURCE_FILES
c_Module.cpp
c_timer.cpp
c_rAString.cpp
    )

## section: header files
# Add your header files here(one file per line)
# listed with $(ls -1 *.{h,hpp})
SET(${this_target}_HEADER_FILES
c_Module.h
c_timer.h
c_rAString.h
    )


## section: precompiled header	
#ADD_MSVC_PRECOMPILED_HEADER("stdafx.h" "stdafx.cpp" ${this_target}_SOURCE_FILES)

SET_SOURCE_FILES_PROPERTIES(${this_target}_HEADER_FILES
                            PROPERTIES HEADER_FILE_ONLY TRUE)
LIST(APPEND ${this_target}_SOURCE_FILES ${${this_target}_HEADER_FILES})

## section: add definitions
#   add prefix -D. example> -DSHP
#  - DO NOT add  the following definitions(already defined in ${OSP_DEFINITIONS}:
#   -DSHP, -DWIN32, -D_WINDOWS, -D_DEBUG, -D_USRDLL, -D_CRT_SECURE_NO_DEPRECATE
#ADD_DEFINITIONS()

## section: add target

ADD_LIBRARY(${this_target} ${${this_target}_SOURCE_FILES} )

