# Copyright © 2016-2024 Matt Robinson
#
# SPDX-License-Identifier: GPL-3.0-or-later

if("${CPACK_GENERATOR}" STREQUAL "DEB")
    set(CPACK_COMPONENTS_ALL common linux debian)
elseif("${CPACK_GENERATOR}" STREQUAL "TGZ")
    set(CPACK_COMPONENTS_ALL common linux rootfiles)
elseif("${CPACK_GENERATOR}" MATCHES "WIX|ZIP")
    set(CPACK_COMPONENTS_ALL common windows rootfiles)
else()
    set(CPACK_COMPONENTS_ALL common)
endif()
