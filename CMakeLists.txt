# Copyright © 2016-2024 Matt Robinson
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.2)
project("Beebview")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME "common")

find_package(Qt6Widgets REQUIRED)

set(BEEBIMAGE_NO_INSTALL ON)
add_subdirectory(libbeebimage)
include_directories(BEFORE libbeebimage)

if(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Werror")
endif()

# Fetch the year of the current git commit
execute_process(COMMAND git log -1 --format=%cd --date=format:%Y
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                OUTPUT_VARIABLE COMMIT_YEAR OUTPUT_STRIP_TRAILING_WHITESPACE)

# Fetch the current version string from git describe
execute_process(COMMAND git describe
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                OUTPUT_VARIABLE VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)

# Make the commit year and version strings available as macros
add_definitions(-DBEEBVIEW_COMMIT_YEAR="${COMMIT_YEAR}")
add_definitions(-DBEEBVIEW_VERSION="${VERSION}")

add_executable(beebview WIN32
               main.cpp About.h About.cpp Beebview.h Beebview.cpp
               BbcScreenWidget.h BbcScreenWidget.cpp Beebview.qrc Beebview.rc)

target_link_libraries(beebview Qt6::Widgets beebimage)

install(TARGETS beebview DESTINATION bin)

if(WIN32)
    # Copy the Qt libraries to the build folders and add them to the install
    foreach(THISLIB Core;Gui;Widgets)
        get_target_property(LIBLOC Qt6::${THISLIB} "IMPORTED_LOCATION_DEBUG")
        file(COPY "${LIBLOC}" DESTINATION Debug)
        install(FILES "${LIBLOC}" DESTINATION bin CONFIGURATIONS Debug)

        get_target_property(LIBLOC Qt6::${THISLIB} "IMPORTED_LOCATION_RELEASE")
        file(COPY "${LIBLOC}" DESTINATION Release)
        install(FILES "${LIBLOC}" DESTINATION bin
                CONFIGURATIONS RelWithDebInfo Release MinSizeRel)
    endforeach()

    # Add the runtime libraries to the list of files to be installed
    include(InstallRequiredSystemLibraries)
endif()

# Set general CPack properties
if(WIN32)
    set(CPACK_GENERATOR "WIX;ZIP")
    set(CPACK_SYSTEM_NAME "win64")
else()
    set(CPACK_GENERATOR "DEB;TGZ")
    set(CPACK_SYSTEM_NAME "amd64")
endif()

set(CPACK_PACKAGE_NAME "BBC Graphics Viewer")
set(CPACK_PACKAGE_CONTACT "Matt Robinson <packaging@nerdoftheherd.com>")
set(CPACK_PROJECT_CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/package/CPackOptions.cmake")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/package/eula.txt")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}")
set(CPACK_PACKAGE_VENDOR "NerdoftheHerd.com")
set(CPACK_PACKAGE_EXECUTABLES "beebview;BBC Graphics Viewer")
set(CPACK_PACKAGE_FILE_NAME "beebview-${VERSION}-${CPACK_SYSTEM_NAME}")
set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)
set(CPACK_STRIP_FILES ON)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
"View and convert graphics files created on BBC Micro or Master computers.
 BBC Graphics Viewer is able to display BBC graphics files saved in LdPic or
 memory dump format in Modes 0, 1, 2, 4 and 5.  It can also save any of the
 images it displays in standard image formats.")

if(WIN32)
    # Check the version contains a number at the start
    if(NOT "${VERSION}" MATCHES "^[0-9]+")
        message(FATAL_ERROR "Unexpected version format \"${VERSION}\"")
    endif()

    # Use as much of the version that is just numbers with dots in-between
    # as Windows Installer can only handle numeric versions
    string(REGEX REPLACE "^([0-9]+(\\.[0-9]+)*).*\$" "\\1"
           CPACK_PACKAGE_VERSION "${VERSION}")
else()
    set(CPACK_PACKAGE_VERSION "${VERSION}")
endif()

# Archive generator specific config
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

# WIX generator specific config
set(CPACK_WIX_UI_DIALOG "${CMAKE_CURRENT_SOURCE_DIR}/package/windows/wix_ui_dialog.jpg")
set(CPACK_WIX_UI_BANNER "${CMAKE_CURRENT_SOURCE_DIR}/package/windows/wix_ui_banner.jpg")
set(CPACK_WIX_UPGRADE_GUID "edbb5c12-9179-46ad-bab2-3eac5be545e6")
set(CPACK_WIX_PATCH_FILE "${CMAKE_CURRENT_SOURCE_DIR}/package/windows/wix_patch.xml")
set(CPACK_WIX_PRODUCT_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Graphics/BeebView.ico")
set(CPACK_WIX_PROPERTY_ARPHELPLINK "https://nerdoftheherd.com/tools/beebview/")
set(CPACK_WIX_UI_REF "WixUI_Minimal")

# DEB generator specific config
set(CPACK_DEBIAN_PACKAGE_NAME "beebview")
set(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "lsb-base")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://nerdoftheherd.com/tools/beebview/")
set(CPACK_DEB_COMPONENT_INSTALL ON)

# Generate debian format copyright file
configure_file("package/linux/debian-copyright" "copyright")
install(FILES "${CMAKE_BINARY_DIR}/copyright" DESTINATION share/doc/beebview
        COMPONENT debian)

if(NOT WIN32)
    execute_process(COMMAND date -R OUTPUT_VARIABLE PACKAGE_TIMESTAMP
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Generate debian format changelog file
    configure_file("package/linux/debian-changelog" "changelog.Debian")

    execute_process(COMMAND gzip -n9
                    INPUT_FILE "${CMAKE_BINARY_DIR}/changelog.Debian"
                    OUTPUT_FILE "${CMAKE_BINARY_DIR}/changelog.Debian.gz")

    install(FILES "${CMAKE_BINARY_DIR}/changelog.Debian.gz"
            DESTINATION share/doc/beebview COMPONENT debian)

    # Convert man page
    execute_process(COMMAND package/linux/generate-manpage help.md
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    OUTPUT_FILE "${CMAKE_BINARY_DIR}/beebview.1")

    execute_process(COMMAND gzip -n9
                    INPUT_FILE "${CMAKE_BINARY_DIR}/beebview.1"
                    OUTPUT_FILE "${CMAKE_BINARY_DIR}/beebview.1.gz")

    install(FILES "${CMAKE_BINARY_DIR}/beebview.1.gz"
            DESTINATION share/man/man1 COMPONENT linux)
endif()

# Convert LICENSE file line endings and install in program folder
configure_file(LICENSE.txt LICENSE.txt)
install(FILES "${CMAKE_BINARY_DIR}/LICENSE.txt" DESTINATION .
              COMPONENT rootfiles)

install(DIRECTORY Examples/ DESTINATION examples COMPONENT windows
        FILES_MATCHING PATTERN "*.bbg")

install(DIRECTORY Examples/ DESTINATION share/beebview/examples COMPONENT linux
        FILES_MATCHING PATTERN "*.bbg")

install(FILES package/linux/beebview.xml DESTINATION share/mime/packages
        COMPONENT linux)

install(FILES Graphics/BeebView.png DESTINATION share/pixmaps
        COMPONENT linux RENAME beebview.png)

install(FILES package/linux/beebview.desktop DESTINATION share/applications
        COMPONENT linux)

# Set permissions correctly for directories when building a DEB package
foreach(DIR IN ITEMS . bin share share/applications share/doc share/beebview
                     share/beebview/examples share/doc/beebview share/mime
                     share/mime/packages share/pixmaps share/man share/man/man1)
    install(DIRECTORY DESTINATION ${DIR}
            DIRECTORY_PERMISSIONS OWNER_READ OWNER_EXECUTE OWNER_WRITE
                                  GROUP_EXECUTE GROUP_READ
                                  WORLD_READ WORLD_EXECUTE
            COMPONENT debian)
endforeach()

include(CPack)
