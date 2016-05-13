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
use File::Copy::Recursive qw(dircopy);
use File::Path;
use File::Spec::Functions qw(rel2abs);
use Pithub;

my $scriptdir = dirname(rel2abs($0));

if(!defined($ENV{DEBFULLNAME}) || !defined($ENV{DEBEMAIL}))
{
    die("DEBFULLNAME and DEBEMAIL should be set before running this script!\n");
}

sub logentry
{
    my $handle = shift;
    my $ver = shift;
    my $commits = shift;
    my $ref = shift;
    my @entries = @_;

    print $handle "beebview ($ver~noth$commits) UNRELEASED; urgency=low\n\n";

    foreach my $entry (@entries)
    {
        print $handle "  * $entry\n";
    }

    chomp(my $reldate = `git log -1 --format=%aD $ref`);
    print $handle "\n -- $ENV{DEBFULLNAME} <$ENV{DEBEMAIL}>  $reldate\n\n";
}

my $ph = Pithub->new(
    user => 'ribbons',
    repo => 'BeebView',
    auto_pagination => 1
);

my $issues = $ph->issues->list(
    params => {
        state => 'closed'
    }
);

my %ver_issues;

while(my $issue = $issues->next)
{
    my %labels = map { $_->{name} => 1 } @{$issue->{labels}};

    if(!exists($labels{'fixed'}))
    {
        next;
    }

    push(@{$ver_issues{$issue->{milestone}->{title}}}, $issue->{title});
}

# Set up the temporary folder to build the package in
chomp(my $builddir = `mktemp -d`);

mkdir "$builddir/debian";
copy "$scriptdir/debian-control", "$builddir/debian/control";

# Generate a changelog in the required debian format
open(my $changelog, '>', "$builddir/debian/changelog");

my @tags = split /\n/, `git for-each-ref --sort=-taggerdate --format '%(tag)' refs/tags`;
chomp(my $current = `git describe`);

if(!grep /^$current$/, @tags)
{
    `git describe --abbrev=20` =~ m/-([0-9]+)-g([a-z0-9]+)$/;
    logentry($changelog, $tags[0], $1, 'HEAD', ("Development snapshot of git revision $2"));
}

foreach my $ver (@tags)
{
    logentry($changelog, $ver, '', $ver, @{$ver_issues{$ver}});
}

close($changelog);

# Set up correctly named package dir
chomp(my $ver = `dpkg-parsechangelog --file "$builddir/debian/changelog" -S version`);
chomp(my $arch = `dpkg-architecture -q DEB_BUILD_ARCH`);
my $package = "$builddir/beebview_${ver}_$arch";
mkpath "$package/DEBIAN";

# Build the binary
mkpath "$builddir/build";
chdir "$builddir/build";
system("cmake -DCMAKE_BUILD_TYPE=Release ".
             "-DCMAKE_INSTALL_PREFIX=$package/usr \"$scriptdir/..\"");
system("make");

# Install the binary into the package
umask 0022;
system("make install");
system("strip $package/usr/bin/beebview");

# Convert and add man page
mkpath "$package/usr/share/man/man1";
system("$scriptdir/manpage.pl $scriptdir/../help.md | gzip -n9 > $package/usr/share/man/man1/beebview.1.gz");

# Add mime-type, icon and .desktop file for launcher entry
mkpath "$package/usr/share/mime/packages";
copy "$scriptdir/beebview.xml", "$package/usr/share/mime/packages";
mkdir "$package/usr/share/pixmaps";
copy "$scriptdir/../Graphics/BeebView.png", "$package/usr/share/pixmaps/beebview.png";
mkdir "$package/usr/share/applications";
copy "$scriptdir/beebview.desktop", "$package/usr/share/applications";

# Add example files
mkdir "$package/usr/share/beebview";
dircopy "$scriptdir/../Examples", "$package/usr/share/beebview/examples";
chmod 0755, "$package/usr/share/beebview/examples";
chmod 0644, <"$package/usr/share/beebview/examples/*.bbg">;

# Generate machine readable copyright file
mkpath "$package/usr/share/doc/beebview";

my $copyright = '';

open(my $authors, "<", "$scriptdir/../AUTHORS.md");

while (<$authors>)
{
    if(m/# (.+ [0-9]{4}(?:-[0-9]{4})?)/)
    {
        $copyright .= "           $1\n"
    }
}

close $authors;

$copyright =~ s/^ +|\n+$//g;

open(my $cprfile, '>', "$package/usr/share/doc/beebview/copyright");

print $cprfile <<EOF;
Format: http://www.debian.org/doc/packaging-manuals/copyright-format/1.0/
Upstream-Name: BBC Graphics Viewer
Upstream-Contact: $ENV{DEBFULLNAME} <$ENV{DEBEMAIL}>
Source: https://nerdoftheherd.com/tools/beebview/

Files: *
Copyright: $copyright
License: GPL-3+
EOF

close($cprfile);

system("gzip -n9 < $builddir/debian/changelog > $package/usr/share/doc/beebview/changelog.gz");

# Maintainer details for control file
open(my $substvars, '>', "$builddir/debian/substvars");
print $substvars "maintainer=$ENV{DEBFULLNAME} <$ENV{DEBEMAIL}>";
close($substvars);

# Build the package
chdir "$builddir";
system("dpkg-shlibdeps -e$package/usr/bin/beebview");
system("dpkg-gencontrol -P$package -f$builddir/files");
system("fakeroot dpkg-deb --build $package");

# Lint the built package
system("lintian $package.deb");

copy "$package.deb", $scriptdir;

# Clean up
system("rm -r $builddir");
