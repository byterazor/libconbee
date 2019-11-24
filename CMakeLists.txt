# This file is part of the libconbee library distribution (https://gitcloud.federationhq.de/byterazor/libconbee)
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
project (libconbee VERSION 0.1.0 LANGUAGES  C)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

find_package(Threads)

include(doxygen)
build_docs(PROCESS_DOXYFILE DOXYFILE_PATH "docs/Doxyfile.in" )

include(compdb)

include(GNUInstallDirs)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(libs/yaap)


# provide all source files for the library
set(LIBCONNBEE_SOURCES
  src/slip.c
  src/slip.h
  src/crc16.h
  src/crc16.c
  include/conbee-queue.h
  src/conbee-queue.c
  src/conbee-send-receive.h
  src/conbee-send-receive.c
  src/conbee-functions.c
  include/conbee.h
  src/conbee.c
)

add_library(objlib OBJECT ${LIBCONNBEE_SOURCES})
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(objlib
  PUBLIC
          include
  PRIVATE
          src
)


add_library(conbee SHARED $<TARGET_OBJECTS:objlib>)
add_library(conbee-static STATIC $<TARGET_OBJECTS:objlib>)

target_include_directories(conbee
  PUBLIC
          include
  PRIVATE
          src
)

target_link_libraries(conbee ${CMAKE_THREAD_LIBS_INIT} yaap)

target_include_directories(conbee-static
  PUBLIC
          include
  PRIVATE
          src
)

target_link_libraries(conbee-static ${CMAKE_THREAD_LIBS_INIT} yaap)


add_executable(conbeectrl
                  src/conbeectrl/main.c
                  src/conbeectrl/version.h
                  src/conbeectrl/version.c
                  src/conbeectrl/mac.h
                  src/conbeectrl/mac.c
                  src/conbeectrl/panid.h
                  src/conbeectrl/panid.c
                  src/conbeectrl/nwk-address.h
                  src/conbeectrl/nwk-address.c
                  src/conbeectrl/nwk_ext_panid.h
                  src/conbeectrl/nwk_ext_panid.c
                  src/conbeectrl/network-mode.h
                  src/conbeectrl/network-mode.c
                  src/conbeectrl/set-network-mode.h
                  src/conbeectrl/set-network-mode.c
                  src/conbeectrl/channel-mask.h
                  src/conbeectrl/channel-mask.c
                  src/conbeectrl/set-channel-mask.h
                  src/conbeectrl/set-channel-mask.c
                  src/conbeectrl/aps_ext_panid.h
                  src/conbeectrl/aps_ext_panid.c
                  src/conbeectrl/set-aps-ext-panid.h
                  src/conbeectrl/set-aps-ext-panid.c
                  src/conbeectrl/tc-address.h
                  src/conbeectrl/tc-address.c
                  src/conbeectrl/set-trust-center.h
                  src/conbeectrl/set-trust-center.c
                  src/conbeectrl/security-mode.h
                  src/conbeectrl/security-mode.c
                  src/conbeectrl/set-security-mode.h
                  src/conbeectrl/set-security-mode.c
)
target_include_directories(conbeectrl
  PRIVATE
          src
)

target_link_libraries(conbeectrl conbee-static)
