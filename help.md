# BBC Graphics Viewer Help


## Introduction

BBC Graphics Viewer is a utility for viewing and converting BBC graphics files
on Windows PCs.


## Installation

Simply unzip the contents of the zip file into a convenient location.  If you
don't have a suitable program for this, you might like to use
[7zip](http://www.7-zip.org/).


## Usage

Although when the image files are originally transferred from the BBC they do
not have an extension, BBC Graphics Viewer is designed with the assumption in
mind that the files have been renamed to have an extension of **.bbg**.  If you
have not renamed your image files, you will still be able to open them, but you
may find it to be more convenient if you do.

To open a BBC graphics file, you can either use browse to it using the
**File** > **Open** dialog, or you can double click on the file, and choose
Beebview.exe as the program that you would like to use to open **.bbg** files.

Depending on the format of the file, you may need to change the display mode
and colour mappings.  LdPic format files contain information about the correct
mode and colours, but the memory dump files do not.

To set the correct mode and logical colour mappings for a memory dump image,
simply select a mode from the **Mode** menu, and then use the **Cycle Colour**
menu until the correct colours are displayed.


## Graphics Modes

The following BBC modes are supported (Mode 1 is the default):

### Mode 0

640 x 256, 2 colours.  Finest detail, used by Wapping Editor.

### Mode 1

320 x 256, 4 colours.  Most commonly used mode, also used by Quest Paint.

### Mode 2

160 x 256, 16 colours.  Most colours, but coarsest detail.

### Mode 4

320 x 256, 2 colours.  Less commonly used, required less memory.

### Mode 5

160 x 256, 4 colours.  Less commonly used, required less memory.


## Command Line Options

BBC Graphics viewer supports the following command line switches:

### --mode0

Force starting in graphics Mode 0.

### --mode1

Force starting in graphics Mode 1.

### --mode2

Force starting in graphics Mode 2.

### --mode4

Force starting in graphics Mode 4.

### --mode5

Force starting in graphics Mode 5.

### --save

Save the image specified on the command line as a bitmap and then quit.  The
bitmap is given the same name as the input file, but with the extension '.bmp'
instead.


### Examples

Open an image:
`Beebview "c:\users\example\bbc\image one.bbg"`

Open an image in Mode 0:
`Beebview --mode0 "c:\users\example\bbc\image one.bbg"`

Convert an image to a bitmap:
`Beebview --save "c:\users\example\bbc\image three.bbg"`

## Questions?

If you have any questions relating to Beebview, then feel free to [get in
touch](https://nerdoftheherd.com/contact.php?subject=BBC%20Graphics%20Viewer) -
I will be happy to hear from you.
