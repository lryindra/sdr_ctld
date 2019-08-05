cmake_minimum_required(VERSION 2.8.0)

# Project
project (sdr_ctld CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME sdr_ctld)
##### Main global variables #####

# We have some custom .cmake scripts not in the official distribution.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

# the following line builds the symbols for gdb
set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    add_definitions(-Wall)
    add_definitions(-Wextra)
    add_definitions(-Wno-unused-parameter)
    add_definitions(-Wsign-compare)
endif()

# function to collect all the sources from sub-directories
# into a single list
function(add_source_files list)
    get_property(is_defined GLOBAL PROPERTY SRCS_LIST DEFINED)
    if(NOT is_defined)
        define_property(GLOBAL PROPERTY ${list}
            BRIEF_DOCS "List of source files"
            FULL_DOCS "List of source files to be compiled in one library")
    endif()
    # make absolute paths
    set(SRCS)
    foreach(s IN LISTS ARGN)
        if(NOT IS_ABSOLUTE "${s}")
            get_filename_component(s "${s}" ABSOLUTE)
        endif()
        list(APPEND SRCS "${s}")
    endforeach()
    # append to global list
    set_property(GLOBAL APPEND PROPERTY ${list} "${SRCS}")
endfunction(add_source_files)

# Boost Libraries
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system program_options thread REQUIRED)
set(GR_REQUIRED_COMPONENTS RUNTIME PMT ANALOG FILTER BLOCKS )
find_package(Gnuradio REQUIRED)
if("${Gnuradio_VERSION}" VERSION_LESS MIN_GR_VERSION)
    MESSAGE(FATAL_ERROR "GnuRadio version required: >=\"" ${MIN_GR_VERSION} "\" found: \"" ${Gnuradio_VERSION} "\"")
endif()

find_package(LimeSuite)
find_package(Gnuradio-limesdr)
find_package(Log4cpp)
find_package(ALSA REQUIRED)

if(NOT GNURADIO_RUNTIME_FOUND)
    message(FATAL_ERROR "GnuRadio Runtime required to compile.")
endif()

if(NOT GNURADIO_LIMESDR_FOUND)
    message(FATAL_ERROR "GnuRadio gr-limesdr required to compile.")
endif()

if(NOT LIMESUITE_FOUND)
    message(FATAL_ERROR "LimeSuite required to compile.")
endif()

#Tell CMake to run moc when necissary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake to always to look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Finish configuring compiler /linker settings & flags
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
    ${GNURADIO_ALL_INCLUDE_DIRS}
    ${GNURADIO_LIMESDR_INCLUDE_DIRS}
    ${LIMESUITE_INCLUDE_DIRS}
    ${ALSA_INCLUDE_DIR}
)

link_directories(
    ${Boost_LIBRARY_DIRS}
    ${LOG4CPP_INCLUDE_DIRS}
    ${GNURADIO_ALL_LIBRARY_DIRS}
)

message("included cmake directory: ${CMAKE_SOURCE_DIR}/include")
message("included boost directory: ${Boost_INCLUDE_DIRS}")
message("included gnuradio runtime directory: ${GNURADIO_ALL_INCLUDE_DIRS}")
message("included limesdr directory: ${GNURADIO_LIMESDR_INCLUDE_DIRS}")
message("included LimeSuite directory: ${LIMESUITE_INCLUDE_DIRS}")
message("included ALSA directory: ${ALSA_INCLUDE_DIR}")
message("linked boost directory: ${Boost_LIBRARY_DIRS}")
message("linked log4cpp directory: ${LOG4CPP_INCLUDE_DIRS}")
message("linked gnuradio runtime directory: ${GNURADIO_ALL_LIBRARY_DIRS}")

# Add subdirectories
add_subdirectory(src)

