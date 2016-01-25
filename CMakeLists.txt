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

if(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")
endif()

add_executable(beebview
               main.cpp About.h About.cpp Beebview.h Beebview.cpp
               BbcScreenWidget.h BbcScreenWidget.cpp
               BbcScreen.h BbcScreen.cpp Beebview.qrc)

TARGET_LINK_LIBRARIES(beebview Qt5::Widgets)