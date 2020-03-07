# SetCmd

SetCmd is a utility for the AmigaOS 4.x shell that lets you quickly and easily switch between different versions of installed software.

![SetCmd usage screen](misc/images/usage.png)

# Background

# Quickstart

After you have run the installer, create a `SETCMD:` assign pointing to the installation directory, and run `setcmd init` from there:

```
assign SETCMD: Software:Programs/SetCmd
SETCMD:setcmd init
```

This installs SetCmd so it's directory of managed commands comes first in your shell search path.

Then you can add a command, and add versions for that command. Here's a quick example using the UAE emulator as the command to be managed:

```
setcmd add-cmd uae
setcmd add-version uae system System:Emulation/E-UAE-amigfx/uae
setcmd add-version uae dev Work:Downloads/E-UAE-git/uae
```

And switch between them:

```
setcmd set-version uae system

; uae command now points to the system-provided UAE emulator

setcmd set-version uae dev

; uae command now points to the git checkout in my Downloads directory
```

## Automatic initialisation
If you'd like SetCmd to always be available, you can add the following lines to your `S:Shell-Startup`:

```
; BEGIN SetCmd
assign SETCMD: Software:Programs/SetCmd
SETCMD:setcmd init verbose
; END SetCmd
```

Just make sure you point the `SETCMD:` assign at the directory where SetCmd is installed.

This will make sure SetCmd is intialised on each new shell session, and will also display some verbose output showing what commands are available. You can change this to just `setcmd init` if you want to reduce the output, or `setcmd init quiet` for no output at all.

Alternatively, if you'd like it to be available system-wide and not just when you start a shell session, you could add the same lines to `S:User-Startup`. You'll probably want to use `setcmd init quiet` there, though.


# Documentation and Links
Running `setcmd` with no arguments displays the the usage screen shown at the top of this document, this provides quick examples for each function.


# Requirements

 * AmigaOS 4.1 (4.0 _may_ work, but I haven't yet checked all the DOS calls to ensure they are compatible with earlier versions)
 * I will attempt to port this to classic 68k AmigaOS 3.x, AROS, MorphOS and other Amiga-like systems in the future.

# Development

# Misc

