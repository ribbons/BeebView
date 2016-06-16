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
    # Copy the Qt libraries to the build folders and add them to the install
    foreach(THISLIB Core;Gui;Widgets)
        get_target_property(LIBLOC Qt5::${THISLIB} "IMPORTED_LOCATION_DEBUG")
        file(COPY "${LIBLOC}" DESTINATION Debug)
        install(FILES "${LIBLOC}" DESTINATION bin CONFIGURATIONS Debug)

        get_target_property(LIBLOC Qt5::${THISLIB} "IMPORTED_LOCATION_RELEASE")
        file(COPY "${LIBLOC}" DESTINATION Release)
        install(FILES "${LIBLOC}" DESTINATION bin
                CONFIGURATIONS RelWithDebInfo Release MinSizeRel)
    endforeach()

    # Add the runtime libraries to the list of files to be installed
    include(InstallRequiredSystemLibraries)

    set(CPACK_GENERATOR "WIX;ZIP")

    # Set general CPack properties
    set(CPACK_PACKAGE_NAME "BBC Graphics Viewer")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/package/eula.txt")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}")
    set(CPACK_PACKAGE_VENDOR "NerdoftheHerd.com")
    set(CPACK_PACKAGE_EXECUTABLES "beebview;BBC Graphics Viewer")

    # CPACK_SYSTEM_NAME isn't defined yet so work it out ourselves
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CPACK_SYSTEM_NAME "win64")
    else()
        set(CPACK_SYSTEM_NAME "win32")
    endif()

    set(CPACK_PACKAGE_FILE_NAME "BeebView-${VERSION}-${CPACK_SYSTEM_NAME}")

    # Check the version contains a number at the start
    if(NOT "${VERSION}" MATCHES "^[0-9]+")
        message(FATAL_ERROR "Unexpected version format \"${VERSION}\"")
    endif()

    # Use as much of the version that is just numbers with dots in-between
    string(REGEX REPLACE "^([0-9]+(\\.[0-9]+)*).*\$" "\\1"
           CPACK_PACKAGE_VERSION "${VERSION}")

    # WIX generator specific values
    set(CPACK_WIX_UI_DIALOG "${CMAKE_CURRENT_SOURCE_DIR}/package/windows/wix_ui_dialog.jpg")
    set(CPACK_WIX_UI_BANNER "${CMAKE_CURRENT_SOURCE_DIR}/package/windows/wix_ui_banner.jpg")
    set(CPACK_WIX_UPGRADE_GUID "edbb5c12-9179-46ad-bab2-3eac5be545e6")
    set(CPACK_WIX_PRODUCT_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Graphics/BeebView.ico")
    set(CPACK_WIX_PROPERTY_ARPHELPLINK "https://nerdoftheherd.com/tools/beebview/")
    set(CPACK_WIX_UI_REF "WixUI_Minimal")

    if(CPACK_SYSTEM_NAME STREQUAL "win32")
        # Prevent the 32-bit .msi being installed on 64-bit Windows
        set(WIX_PROD_FRAG_CONDITIONS "<Condition Message=\"This is the 32-bit [ProductName] installer, but you are running 64-bit Windows.  Please download the 64-bit installer from [ARPHELPLINK]\">NOT Msix64</Condition>")
    endif()

    configure_file("package/windows/wix_patch.xml" "wix_patch.xml")
    set(CPACK_WIX_PATCH_FILE "${CMAKE_BINARY_DIR}/wix_patch.xml")

    # Convert LICENSE & AUTHORS files line endings and install in program folder
    configure_file(LICENSE.txt LICENSE.txt)
    configure_file(AUTHORS.md AUTHORS.txt)
    install(FILES "${CMAKE_BINARY_DIR}/LICENSE.txt"
                  "${CMAKE_BINARY_DIR}/AUTHORS.txt" DESTINATION .)

    install(FILES "Examples/Aworks (LdPic).bbg"
                  "Examples/Balls (Dump - Mode 0).bbg"
                  "Examples/Flower (Dump - Mode 1).bbg"
            DESTINATION examples)

    # Code signing targets
    set(TS_SERVER http://time.certum.pl)

    # Add SHA1 sig for Vista & below, amend (/as) with SHA256 sig for Win 7+
    add_custom_target(beebview_signed
                      COMMAND signtool sign /t ${TS_SERVER} $<TARGET_FILE:beebview>
                      COMMAND signtool sign /as /fd sha256 /tr ${TS_SERVER} /td sha256 $<TARGET_FILE:beebview>
                      VERBATIM)

    # Sign .msi with just SHA256 for Win 7+ as they don't support multiple sigs
    add_custom_target(package_signed
                      signtool sign /fd sha256 /tr ${TS_SERVER} /td sha256 ${CPACK_PACKAGE_FILE_NAME}.msi
                      VERBATIM)

    add_dependencies(beebview_signed beebview)
    add_dependencies(package_signed PACKAGE)

    include(CPack)
endif()
