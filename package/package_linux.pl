#!/usr/bin/perl -w

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

use strict;
use warnings;

use autodie qw(:all);
use File::Basename;
use File::Copy;
use File::Spec::Functions qw(rel2abs);

my $scriptdir = dirname(rel2abs($0));

if(!defined($ENV{DEBFULLNAME}) || !defined($ENV{DEBEMAIL}))
{
    die("DEBFULLNAME and DEBEMAIL should be set before running this script!\n");
}

# Set up the temporary folder to build the packages in
chomp(my $builddir = `mktemp -d`);

# Build the packages
chdir "$builddir";
system("cmake -DCPACK_PACKAGE_CONTACT=\"$ENV{DEBFULLNAME} <$ENV{DEBEMAIL}>\" ".
             "-DCMAKE_BUILD_TYPE=Release \"$scriptdir/..\"");
system("make package");

# Lint the built package
system("lintian *.deb");

# Fetch the built packages
copy <*.deb>, "$scriptdir";
copy <*.tar.gz>, "$scriptdir";

# Clean up
system("rm -r $builddir");
