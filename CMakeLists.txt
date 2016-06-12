# This file is part of BBC Graphics Viewer.
# Copyright © 2016 by the authors - see the AUTHORS file for details.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.2)
project("Beebview")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

set(BEEBIMAGE_NO_INSTALL ON)
add_subdirectory(libbeebimage)
include_directories(BEFORE libbeebimage)

if(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")
endif()

# Extract the author names and years for the copyright string
file(READ "AUTHORS.md" AUTHORS)
string(REGEX MATCHALL "\n## [^\n]+ [0-9]+(-[0-9]+)?\n" AUTHORS "${AUTHORS}")
string(REGEX REPLACE "\n## ([^\n]+)\n" "\\1" AUTHORS "${AUTHORS}")
string(REPLACE ";" ", " AUTHORS "${AUTHORS}")

# Fetch the current version string from git describe
execute_process(COMMAND git describe
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                OUTPUT_VARIABLE VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)

# Make the author and versions strings available as macros
add_definitions(-DBEEBVIEW_AUTHORS="${AUTHORS}")
add_definitions(-DBEEBVIEW_VERSION="${VERSION}")

add_executable(beebview WIN32
               main.cpp About.h About.cpp Beebview.h Beebview.cpp
               BbcScreenWidget.h BbcScreenWidget.cpp Beebview.qrc Beebview.rc)

target_link_libraries(beebview Qt5::Widgets beebimage)

install(TARGETS beebview DESTINATION bin)

if(WIN32)
    foreach(THISLIB Core;Gui;Widgets)
        get_target_property(LIBLOC Qt5::${THISLIB} "IMPORTED_LOCATION_DEBUG")
        file(COPY "${LIBLOC}" DESTINATION Debug)

        get_target_property(LIBLOC Qt5::${THISLIB} "IMPORTED_LOCATION_RELEASE")
        file(COPY "${LIBLOC}" DESTINATION Release)
    endforeach()
endif()
