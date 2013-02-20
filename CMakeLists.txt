cmake_minimum_required(VERSION 2.6)
project(AdvancedGraphics)
# The version number.
set(AdvancedGraphics_VERSION_MAJOR 1)
set(AdvancedGraphics_VERSION_MINOR 0)

find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
include(${QT_USE_FILE})
set(CMAKE_AUTOMOC ON)

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
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_BINARY_DIR}/part1.1")
include_directories("${PROJECT_BINARY_DIR}/part1.2")
include_directories("${PROJECT_BINARY_DIR}/part2.2")
include_directories("${PROJECT_BINARY_DIR}/part2.3")


# add the executable
file(GLOB_RECURSE source_header_files src/*.h* part*/*.h*)
file(GLOB_RECURSE public_header_files include/*.h*)
file(GLOB_RECURSE source_source_files src/*.cpp)
file(GLOB_RECURSE source_part1.1 part1.1/*.cpp)
file(GLOB_RECURSE source_part1.2 part1.2/*.cpp)
file(GLOB_RECURSE source_part2.2 part2.2/*.cpp)
file(GLOB_RECURSE source_part2.3 part2.3/*.cpp)

file(
  GLOB_RECURSE
  source_UIS
  src/*.ui
  part1.1/*.ui
  part1.2/*.ui
  part2.2/*.ui
  part2.3/*.ui
)
file(
  GLOB_RECURSE
  source_RESOURCES
  src/*.qrc
  part1.1/*.qrc
  part1.2/*.qrc
  part2.2/*.qrc
  part2.3/*.qrc
)

qt4_wrap_ui(source_UIS_H ${source_UIS})
qt4_add_resources(source_RESOURCES_CPP ${source_RESOURCES})

add_executable(
    HDRimage
    ${source_part1.1}
    ${source_source_files}
    ${source_MOC}
    ${source_UIS_H}
    ${source_RESOURCES_CPP}
)
add_executable(
    relighting
    ${source_part1.2}
    ${source_source_files}
    ${source_MOC}
    ${source_UIS_H}
    ${source_RESOURCES_CPP}
)

add_executable(
    envMap
    ${source_part2.2}
    ${source_source_files}
    ${source_MOC}
    ${source_UIS_H}
    ${source_RESOURCES_CPP}
)

add_executable(
    phongSampling
    ${source_part2.3}
    ${source_source_files}
    ${source_MOC}
    ${source_UIS_H}
    ${source_RESOURCES_CPP}
)

target_link_libraries(HDRimage ${QT_LIBRARIES})
target_link_libraries(relighting ${QT_LIBRARIES})
target_link_libraries(envMap ${QT_LIBRARIES})
target_link_libraries(phongSampling ${QT_LIBRARIES})

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
    set_property(TARGET relighting
      APPEND PROPERTY COMPILE_DEFINITIONS ${flag})
    set_property(TARGET envMap
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

include("${CMAKE_SOURCE_DIR}/CMake/Compiler/Compiler.cmake")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/AdvancedGraphicsConfig.hpp.in"
  "${PROJECT_BINARY_DIR}/AdvancedGraphicsConfig.hpp"
  )

# add the install targets
install(TARGETS HDRimage    DESTINATION bin)
install(TARGETS relighting DESTINATION bin)
install(TARGETS envMap DESTINATION bin)
install(TARGETS phongSampling DESTINATION bin)


# does the application run
add_test(HDRimageRuns    HDRimage)
add_test(relightingRuns relighting)
add_test(envMapRuns envMap)
add_test(phongSamplingRuns phongSampling)

# build a CPack driven installer package
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${HDRimage_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${HDRimage_VERSION_MINOR}")
include(CPack)
