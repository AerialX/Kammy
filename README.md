Kammy
=====

Kammy is a system for loading patches to lv2 (PlayStation 3's Game OS) from
a user application, using PSGroove or any other implementation of the
exploit that adds the peek/poke syscalls. Kammy was inspired by Treeki's
Nintendo Wii game patching system, Kamek.

Building
--------
Building Kammy requires different gcc compiler toolchains.

* gcc: A normal host gcc is required to build the raw2h application.
* ppu-gcc, ppu-binutils: A version of gcc that will compile 64bit PowerPC
  instructions is required to build the patches. Linux packages can be found
  on [BSC.es](http://www.bsc.es/plantillaH.php?cat_id=461).
  (note: ppu-lv2-gcc may suffice for this, untested)
* xxd: Creating patch bin files requires the xxd tool to be installed.
* dd: Also required for building the patch bin files.

With these dependencies installed, you can build Kammy by simply cd'ing to
the *loader* directory and running:

    make clean
    make


Usage
-----
Kammy must be used with a payload that supports poke/peek. This includes
PSGroove and most of its forks - including my own - among others. To apply a
Kammy patch, a loader application must be started on the PS3. This is
usually done from XMB from an installed package, or from USB using my
PSGroove fork's apploader payload.


Customizing
-----------
Kammy is made up of two main components:

* lv2: This folder contains the lv2 patches to be built. See the main kammy
  patch for an example. It is up to the patch to apply any hooks needed to
  lv2.
* libkammy: This is the basic library that handles the loading of Kammy
  patches.

The loader/ folder contains an example of using libkammy to load a patch
from the lv2 folder.


Notes
-----
Internally, Kammy obliderates syscall 11, so try not to run it with
payloads that provide that syscall.
