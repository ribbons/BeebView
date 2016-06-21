#!/bin/sh -e

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

SCRIPTDIR=$(dirname "$(readlink -f "$0")")

if [ -z "$DEBFULLNAME" ] || [ -z "$DEBEMAIL" ]; then
    echo "DEBFULLNAME and DEBEMAIL should be set before running this script!" >&2
    exit 1
fi

# Set up the temporary folder to build the packages in
BUILDDIR=$(mktemp -d)

# Build the packages
cd "$BUILDDIR"
cmake -DCPACK_PACKAGE_CONTACT="$DEBFULLNAME <$DEBEMAIL>" \
      -DCMAKE_BUILD_TYPE=Release "$SCRIPTDIR/.."
make package

# Lint the built package
lintian *.deb

# Fetch the built packages
cp *.deb "$SCRIPTDIR"
cp *.tar.gz "$SCRIPTDIR"

# Clean up
rm -r "$BUILDDIR"
