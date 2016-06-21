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

if("${CPACK_GENERATOR}" STREQUAL "DEB")
    set(CPACK_COMPONENTS_ALL COMMON LINUX DEBIAN)
elseif("${CPACK_GENERATOR}" STREQUAL "TGZ")
    set(CPACK_COMPONENTS_ALL COMMON LINUX ROOTFILES)
elseif("${CPACK_GENERATOR}" MATCHES "WIX|ZIP")
    set(CPACK_COMPONENTS_ALL COMMON WINDOWS ROOTFILES)
else()
    set(CPACK_COMPONENTS_ALL COMMON)
endif()
