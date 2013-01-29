cmake_minimum_required(VERSION 2.6)
project(HDRimage)
# The version number.
set(HDRimage_VERSION_MAJOR 1)
set(HDRimage_VERSION_MINOR 0)

# Initialize CXXFLAGS.
SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_FLAGS_HELP
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel.")
SET(COMPILER_INFORMATION
    "Configure for ${CMAKE_CXX_COMPILER} (C++) and ${CMAKE_C_COMPILER} (C) in ${CMAKE_BUILD_TYPE}.")

# add the binary tree to the search path for include files
# so that we will find HDRimageConfig.h
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")

# add the executable
file(GLOB_RECURSE source_files src/*.*pp)
file(GLOB_RECURSE public_header_files include/*.hpp)
add_executable(HDRimage ${source_files} ${public_header_files})

# Check endianess
include(TestBigEndian)
TEST_BIG_ENDIAN(BIG_ENDIAN)

# Check for 64bits x86 extension
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHx86_64 TRUE )
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHx86 TRUE)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

# Check for C++11
include("${CMAKE_SOURCE_DIR}/CMake/cpp11/CheckCXX11Features.cmake")
foreach(flag ${CXX11_FEATURE_LIST})
    set_property(TARGET HDRimage
      APPEND PROPERTY COMPILE_DEFINITIONS ${flag})
endforeach(flag ${CXX11_FEATURE_LIST})

# OpenMP
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -lnuma")

include("${CMAKE_SOURCE_DIR}/CMake/Compiler/Compiler.cmake")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/HDRimageConfig.hpp.in"
  "${PROJECT_BINARY_DIR}/HDRimageConfig.hpp"
  )

# add the install targets
install(TARGETS HDRimage DESTINATION bin)

# does the application run
add_test(HDRimageRuns HDRimage)

# build a CPack driven installer package
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${HDRimage_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${HDRimage_VERSION_MINOR}")
include(CPack)
