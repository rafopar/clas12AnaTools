cmake_minimum_required (VERSION VERSION 3.16)

# CMakeLists for tools
project(CLAS12_AnaTools VERSION 1.0.1 DESCRIPTION "Tools for CLAS12 related analysis")

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "-std=c++17")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
set(CMAKE_PREFIX_PATH $ENV{ROOTSYS}/cmake)

find_package(ROOT REQUIRED)
find_package(LZ4 REQUIRED)
add_definitions(-D__LZ4__)
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   # shared libs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   # static libs

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/hipo/hipo4 ${ROOT_INCLUDE_DIRS})
set(CLAS12ANATOOLS_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/src/clas12AnaTools.cc" "${PROJECT_SOURCE_DIR}/src/RecParticle.cc" )
add_library(CLAS12AnaTools SHARED ${CLAS12ANATOOLS_SOURCE_FILES})
#target_link_libraries(CLAS12AnaTools PUBLIC Hipo4 ${ROOT_LIBRARIES} ROOT::Spectrum ${LZ4_LIBRARY} )
#target_link_libraries(CLAS12AnaTools PRIVATE Hipo4 ${ROOT_LIBRARIES} ROOT::Spectrum ${LZ4_LIBRARY} )
set_target_properties(CLAS12AnaTools PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)
set (CMAKE_INSTALL_LIBDIR "lib")

target_include_directories(CLAS12AnaTools
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

include(GNUInstallDirs)

install(TARGETS CLAS12AnaTools
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(CLAS12AnaTools.pc.in CLAS12AnaTools.pc @ONLY)
  
# Add directories
add_subdirectory(hipo/hipo4)
#add_subdirectory(CompareHistos)
add_subdirectory(uRWell)
