cmake_minimum_required (VERSION 3.1 FATAL_ERROR)
project (yaap VERSION 0.1.0 LANGUAGES  C)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

include(doxygen)
build_docs(PROCESS_DOXYFILE DOXYFILE_PATH "Doxyfile.in" )

include(GNUInstallDirs)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_library(yaap STATIC
        src/argparse.c
        src/argparse.h
)
set_property(TARGET yaap PROPERTY POSITION_INDEPENDENT_CODE 1)

target_include_directories(yaap
  PUBLIC
          src
)
