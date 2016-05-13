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

my $header;

my @sec_names;
my %sections;

foreach (@ARGV)
{
    open(my $markdown, "<", $_);
    my $current;

    while (<$markdown>)
    {
        if(m/^# (.+)$/)
        {
            $header = $1;
        }
        elsif(m/^## (.+)$/)
        {
            $current = uc($1);

            if($current eq 'COMMAND LINE OPTIONS')
            {
                $current = 'OPTIONS';
            }
            elsif($current eq 'EXAMPLE USAGE')
            {
                $current = 'EXAMPLES';
            }

            push @sec_names, $current;
        }
        else
        {
            if ($current)
            {
                if (defined $sections{$current})
                {
                    $sections{$current} .= $_;
                }
                else
                {
                    $sections{$current} = $_;
                }
            }
        }
    }

    close $markdown;
};

print <<EOF;
.TH BEEBVIEW 1 2016-05-11 beebview "$header"
.SH NAME
beebview \- view and convert BBC graphics files
EOF

foreach (@sec_names)
{
    print ".SH $_\n";

    my $section = $sections{$_};

    # Escape dots
    $section =~ s/\./\\./g;

    # Second level headings
    $section =~ s/\n### (.+)\n\n/\n.TP\n.B $1\n/g;

    # **xyz** to .B xyz
    $section =~ s/\s+\*\*([^*]+)\*\*\s/\n.B $1\n/g;

    # **xyz**xyz to .BR "xyz" "xyz"
    $section =~ s/\s+\*\*([^*]+)\*\*(\S+)\s+/\n.BR "$1" "$2"\n/g;

    # *xyz* to .I xyz
    $section =~ s/\s+\*([^*]+)\*\s/\n.I $1\n/g;

    # *xyz*xyz to .IR "xyz" "xyz"
    $section =~ s/\s+\*([^*]+)\*(\S+)\s+/\n.IR "$1" "$2"\n/g;

    # `xyz` to .B xyz
    $section =~ s/\s+`([^`]+)`\s/\n.B $1\n/g;

    # `xyz`*xyz* to .BI "xyz" "xyz"
    $section =~ s/\s+`([^`]+)`\*(([^*]+))\*\s+/\n.BI "$1" "$2"\n/g;

    # `xyz`xyz to .BR "xyz" "xyz"
    $section =~ s/\s+`([^`]+)`(\S+)\s+/\n.BR "$1" "$2"\n/g;

    # Dots at the start of lines
    $section =~ s/\n\\./\n\\[char46]/g;

    print $section;
}
