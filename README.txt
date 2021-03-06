SetCmd
======
SetCmd is a utility for the AmigaOS 4.x shell that lets you quickly and easily
switch between different versions of installed software.

Requirements
============
 * AmigaOS 4.1 (4.0 _may_ work, but I haven't yet checked all the DOS calls 
   to ensure they are compatible with earlier versions).
 * I will attempt to port this to classic 68k AmigaOS 3.x, AROS, MorphOS and 
   other Amiga-like systems in the future.

Quickstart
==========
After you have run the installer, create a SETCMD: assign pointing to the
installation directory, and run "setcmd init" from there:


assign SETCMD: Software:Programs/SetCmd
SETCMD:setcmd init


This installs SetCmd so it's directory of managed commands comes first in your
shell search path.

Then you can add a command, and add versions for that command. Here's a quick
example using the UAE emulator as the command to be managed:


setcmd add-cmd uae
setcmd add-version uae system System:Emulation/E-UAE-amigfx/uae
setcmd add-version uae dev Work:Downloads/E-UAE-git/uae


And switch between them:


setcmd set-version uae system

; uae command now points to the system-provided UAE emulator

setcmd set-version uae dev

; uae command now points to the git checkout in my Downloads directory


Automatic initialisation
------------------------
If you'd like SetCmd to always be available, you can add the following lines to
your S:Shell-Startup:


; BEGIN SetCmd
assign SETCMD: Software:Programs/SetCmd
SETCMD:setcmd init verbose
; END SetCmd


Just make sure you point the SETCMD: assign at the directory where SetCmd is
installed.

This will make sure SetCmd is intialised on each new shell session, and will
also display some verbose output showing what commands are available.

You can change this to just "setcmd init" if you want to reduce the output, or
"setcmd init quiet" for no output at all.

Alternatively, if you'd like it to be available system-wide and not just when
you start a shell session, you could add the same lines to S:User-Startup.
You'll probably want to use "setcmd init quiet" there, though.

Formatting
----------
If you would prefer SetCmd output displayed without formatting (no bold text,
highlighting etc.) then you can set the local environment variable 
SETCMD_NOFORMAT. For example:

set SETCMD_NOFORMAT true
setcmd list verbose 


Documentation and Links
=======================
Running setcmd with no arguments displays the the usage screen shown at the top
of this document, this provides quick examples for each function.

There is also a full manual provided in AmigaGuide format that is installed
along with the tool.

To read about the background of the development of this tool (and for my
adventures with the Amiga in general), browse my Amiga blog:
http://www.markround.com/amiga


Background
==========
I developed SetCmd primarily to solve a problem I had - switching between
different versions of the UAE Emulator on my X5000 while using classic Amiga
software. It was also written to accompany my series of blog posts
(http://markround.com/amiga) exploring the next-gen AmigaOS, written from the
perspective of an old ex-Amigan returning to the scene. I used it as an
experiment to learn how to develop, package and distribute software for AmigaOS,
as well as picking up the C language along the way. Excluding variations on
"Hello, World!" I studied years ago at University, this tool is my first full C
program ever, and my first release for the Amiga in a very long time! 

I wanted to make the tool as Amiga-native as possible. This meant things like:

 * Providing an Installer script to install the software
 * Documentation in AmigaGuide format 
 * Using AmigaOS 4.x SDK functions and standards instead of "portable" C
   (options parsing via IDOS->ReadArgs instead of just pulling things out of
   argc, shell output through IDOS->Printf instead of printf in the C standard
   library, etc.)

So if there are any hardcore Amiga developers out there reading this, bear in
mind this is my first attempt at all of this! That said, I'd love any feedback,
pull requests, comments etc. that will help my learning and understanding of all
these different technologies.

Changes
=======
1.1.0 : Added SETCMD_NOFORMAT env var option to control formatting
1.0.0 : Initial public release
