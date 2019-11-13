# This file is part of the libconnbee library distribution (https://gitcloud.federationhq.de/byterazor/libconnbee)
# Copyright (c) 2019 Dominik Meyer <dmeyer@federationhq.de>.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required (VERSION 3.1 FATAL_ERROR)
project (libconnbee VERSION 0.1.0 LANGUAGES  C)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

find_package(Threads)

include(doxygen)
build_docs(PROCESS_DOXYFILE DOXYFILE_PATH "docs/Doxyfile.in" )

include(GNUInstallDirs)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(libs/yaap)


# provide all source files for the library
set(LIBCONNBEE_SOURCES
  src/slip.c
  src/slip.h
  src/crc16.h
  src/crc16.c
  include/connbee-queue.h
  src/connbee-queue.c
  src/connbee-send-receive.h
  src/connbee-send-receive.c
  src/connbee-functions.c
  include/connbee.h
  src/connbee.c
)

add_library(objlib OBJECT ${LIBCONNBEE_SOURCES})
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(objlib
  PUBLIC
          include
  PRIVATE
          src
)


add_library(connbee SHARED $<TARGET_OBJECTS:objlib>)
add_library(connbee-static STATIC $<TARGET_OBJECTS:objlib>)

target_include_directories(connbee
  PUBLIC
          include
  PRIVATE
          src
)

target_link_libraries(connbee ${CMAKE_THREAD_LIBS_INIT} yaap)

target_include_directories(connbee-static
  PUBLIC
          include
  PRIVATE
          src
)

target_link_libraries(connbee-static ${CMAKE_THREAD_LIBS_INIT} yaap)


add_executable(connbeectrl
                  src/connbeectrl/main.c
                  src/connbeectrl/version.h
                  src/connbeectrl/version.c
)
target_include_directories(connbeectrl
  PRIVATE
          src
)

target_link_libraries(connbeectrl connbee-static)
