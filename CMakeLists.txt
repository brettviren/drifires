cmake_minimum_required(VERSION 3.16)

project(drifires VERSION 0.0.0 LANGUAGES CXX)

# This CMake build replicates the features of the waf build described in
# ./wscript.  One waf feature is intentionally omitted: discovery of the "moo"
# tool and the associated Jsonnet/Jinja2 code generation of
# inc/drifires/nljs.hpp and inc/drifires/structs.hpp.  Those generated headers
# are committed to the source tree and are used here as-is.

# ---------------------------------------------------------------------------
# Options, mirroring the waf --with-* options.
#
# Each points at an install area (containing include/ and lib/) and is used as
# a hint for the corresponding find_package / find_* below.  Users may instead
# rely on the standard CMAKE_PREFIX_PATH.
# ---------------------------------------------------------------------------
set(WITH_NLJS       "" CACHE PATH "Path to nlohmann::json install area")
set(WITH_ROOT       "" CACHE PATH "Path to ROOT install area")
set(WITH_GARFIELDPP "" CACHE PATH "Path to Garfield++ install area")

# Fold the --with-* hints into CMAKE_PREFIX_PATH so find_package can locate the
# packages' CMake config files there.
foreach(hint IN ITEMS WITH_NLJS WITH_ROOT WITH_GARFIELDPP)
  if(${hint})
    message(STATUS "using ${hint} at ${${hint}}")
    list(PREPEND CMAKE_PREFIX_PATH "${${hint}}")
  endif()
endforeach()

# ---------------------------------------------------------------------------
# Dependencies (all mandatory, as in the waf build).
# ---------------------------------------------------------------------------

# nlohmann::json (header only).  Prefer its CMake config; otherwise locate the
# header directly.
find_package(nlohmann_json QUIET)
if(nlohmann_json_FOUND)
  set(NLJS_TARGET nlohmann_json::nlohmann_json)
else()
  find_path(NLJS_INCLUDE_DIR nlohmann/json.hpp
    HINTS "${WITH_NLJS}" PATH_SUFFIXES include)
  if(NOT NLJS_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find nlohmann/json.hpp (set WITH_NLJS)")
  endif()
  add_library(nljs INTERFACE)
  target_include_directories(nljs INTERFACE "${NLJS_INCLUDE_DIR}")
  set(NLJS_TARGET nljs)
endif()

# ROOT.
find_package(ROOT REQUIRED
  COMPONENTS Core Graf Hist Graf3d Gpad Geom Matrix MathCore)

# Garfield++.  Prefer its CMake config; otherwise locate header + library.
find_package(Garfield QUIET)
if(Garfield_FOUND)
  set(GARFIELD_TARGET Garfield::Garfield)
else()
  find_path(GARFIELD_INCLUDE_DIR Garfield/Medium.hh
    HINTS "${WITH_GARFIELDPP}" PATH_SUFFIXES include)
  find_library(GARFIELD_LIBRARY Garfield
    HINTS "${WITH_GARFIELDPP}" PATH_SUFFIXES lib)
  if(NOT GARFIELD_INCLUDE_DIR OR NOT GARFIELD_LIBRARY)
    message(FATAL_ERROR "Could not find Garfield++ (set WITH_GARFIELDPP)")
  endif()
  add_library(garfieldpp INTERFACE)
  target_include_directories(garfieldpp INTERFACE "${GARFIELD_INCLUDE_DIR}")
  target_link_libraries(garfieldpp INTERFACE "${GARFIELD_LIBRARY}")
  set(GARFIELD_TARGET garfieldpp)
endif()

set(ROOT_TARGETS
  ROOT::Core ROOT::Graf ROOT::Hist ROOT::Graf3d
  ROOT::Gpad ROOT::Geom ROOT::Matrix ROOT::MathCore)

# ---------------------------------------------------------------------------
# Compiler settings, mirroring the waf configure() CXXFLAGS.
# ---------------------------------------------------------------------------
# ROOT requires that consumers compile with at least the same C++ standard ROOT
# itself was built with; otherwise its headers emit a #warning (fatal here under
# -Werror).  ROOTConfig.cmake exports ROOT_CXX_STANDARD, so track it when found
# and fall back to C++17 (the minimum this code was written against) otherwise.
if(DEFINED ROOT_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD "${ROOT_CXX_STANDARD}")
else()
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-ggdb3 -Wall -Wpedantic -Werror)

# ---------------------------------------------------------------------------
# RPATH: allow apps/tests to run from the build tree and after install without
# needing LD_LIBRARY_PATH (mirrors the waf rpath handling).
# ---------------------------------------------------------------------------
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)

# ---------------------------------------------------------------------------
# The drifires shared library.
# ---------------------------------------------------------------------------
file(GLOB DRIFIRES_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(drifires SHARED ${DRIFIRES_SOURCES})

target_include_directories(drifires PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(drifires PUBLIC
  ${NLJS_TARGET} ${GARFIELD_TARGET} ${ROOT_TARGETS})

# HAVE_* definitions, matching the define_name's from the waf checks.
target_compile_definitions(drifires PUBLIC HAVE_NLJS HAVE_ROOT HAVE_GARFIELDPP)

# ---------------------------------------------------------------------------
# Applications: one program per apps/*.cpp.
# ---------------------------------------------------------------------------
file(GLOB DRIFIRES_APPS CONFIGURE_DEPENDS apps/*.cpp)
foreach(app_src IN LISTS DRIFIRES_APPS)
  get_filename_component(app_name "${app_src}" NAME_WE)
  add_executable(${app_name} "${app_src}")
  target_link_libraries(${app_name} PRIVATE drifires)
  install(TARGETS ${app_name} RUNTIME DESTINATION bin)
endforeach()

# ---------------------------------------------------------------------------
# Test program built from test/test_*.cpp.
# ---------------------------------------------------------------------------
file(GLOB DRIFIRES_TEST_SOURCES CONFIGURE_DEPENDS test/test_*.cpp)
if(DRIFIRES_TEST_SOURCES)
  add_executable(drifires-test ${DRIFIRES_TEST_SOURCES})
  target_include_directories(drifires-test PRIVATE test)
  target_link_libraries(drifires-test PRIVATE drifires)

  enable_testing()
  add_test(NAME drifires-test COMMAND drifires-test)
endif()

# ---------------------------------------------------------------------------
# Installation.
# ---------------------------------------------------------------------------
install(TARGETS drifires LIBRARY DESTINATION lib)

install(DIRECTORY inc/drifires
  DESTINATION include
  FILES_MATCHING PATTERN "*.hpp")
