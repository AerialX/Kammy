Kammy
=====

Kammy is a system for loading patches to lv2 (PlayStation 3's Game OS) from
a user application, using PSGroove or any other implementation of the
exploit that adds the peek/poke syscalls. Kammy was inspired by Treeki's
Nintendo Wii game patching system, Kamek.

Building
--------
Building Kammy requires [PSL1GHT](http://github.com/AerialX/PSL1GHT)
installed to build. This includes needing one of the supported PS3 GCC
toolchains. Besides PSL1GHT, you will need the following utilities:

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
PSGroove fork's apploader payload (or PL3's payload_dev)._

### Ethernet Debug Example
The example loader included in Kammy installs a hook into lv2 that sends
debug messages over the ethernet cable of the PS3. This is useful for
getting data from the PS3 and lv2 (you get crash reports, and some info from
different apps). It's also a useful way of seeing printfs from any
applications you write and test, including ones built with PSL1GHT.

This setup has three requirements to work properly:

1. Your PS3 must be connected to a router by ethernet cable.
   Wireless must be disabled in the PS3 network settings.
2. The PC that you're retrieving the info with must be connected to the
   same router as the PS3. Alternatively you can connect the PS3's cable
   directly into your computer to get the output.
3. Your PC must have the socat program installed, so you can run the
   following command:

    socat udp-recv:18194 stdout


Customizing
-----------
Kammy is made up of three main components:

* lv2: This folder contains the lv2 patches to be built. See the ethdebug
  patch for an example. It is up to the patch to apply any hooks it needs to
  lv2.
* libkammy: This is the basic library that handles the loading of Kammy
  patches. Altering it should not be necessary.
* loader: This folder contains an example that shows how to use libkammy to
  load kammy and another patch bin from the lv2 folder.


Notes
-----
Internally, Kammy obliderates syscall 11, so try not to run it with
payloads that provide that syscall.
