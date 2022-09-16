---
bookFlatSection: false
weight: 120
title: "Installation"
bookCollapseSection: true
type: docs
description: >
    Installing and building software and hardware design files
---

The tag system consists of software, both host and embedded, and hardware including printed circuit boards (PCBs) and 3D printed adapters.  Consequently, there are a relatively large number of software tools involved.   All of the software and 
hardware designs (source code) are available in the [github repository](https://git.iu.edu/geobrown/Nanotag-paper).  While you can explore that repository on-line, in order to compile the code and generate the processing files necessary to build boards, it is necessary to first
*clone* the repository as described in [Installing the Repository]({{< ref "#install-1" >}})

An
abbreviated view of this repository is shown in the following 
figure. There are two directories containing the designs for
this project -- software and hardware.  Two directories that
reference code from other projects as *submodules* -- nanopb, which
provides the compiler and libraries for the communication protocols
used to exchange data and control messages with the tags, and 
ChibiOS, which is the embedded operating system (RTOS) used in the 
tags.  Finally there is a directory (cmake) and files (CMakeLists.txt) used by the CMake build system.

**Note** it's better to install nanopb from the available binary as this is less likely to have version problems with the install protoc



```
.
├── ChibiOS
├── cmake
├── CMakeLists.txt
├── hardware
│   ├── AccelTag
│   ├── BitTagv5
│   ├── BitTagv7
│   ├── CMakeLists.txt
│   ... 
│   ├── Mechanical
│   ...
├── nanopb
├── README.md
└── software
    ├── CMakeLists.txt
    ├── embedded
    ├── host
    ├── include
    └── proto
```

The hardware directory contains a separate directory for each board,
libraries used by the Kicad, the computer aided design system (CAD) that we use to design hardware, and a directory of mechanical 
components (Mechanical) that includes the design files for the 3D 
printed bases.

The software directory includes separate sub-directories for embedded and host code, and a directory for the protocol definitions used to communicate between the host and the tags.

All of the software and hardware for tags can be compiled (built) on Windows, OS X, or Linux.  Enabling this process requires
installing a number of programs and libraries.  There are 
differences in the installation and build process for the three 
platforms that will be called out.  In this section we describe
the processes for *cloning* the tag repository and installing the CMake build system.  The tools and libraries required to perform a build are discussed in separate sections for
hardware and software.

##  1. Installing the Repository {#install-1}

In order to install the repository, you should use the program `git`, which is available on Windows, OS X, and Linux. Instructions
for installing `git` can be found [here](https://git-scm.com/downloads)

Once git is installed, copy the git repo link (for https) and execute the following in a terminal within the directory where you would like the repo installed

```shell
git clone https://github.iu.edu/geobrown/NanoTag-paper.git target_directory_name
cd target_directory_name
git submodule update --init --recursive 
```

The repository for this project includes one *submodules* -- ChibiOS; these are external repositories upon which our system builds. Nanopb used to be installed as a submodule but it's best to install the available binary distribution in the named directory. Nanopb is a code library and tool for creating the communication code used in our tags to communicate with the host applications.  ChibiOS is an excellent embedded operating systems that is used on our tags.

The line `git submodule...` initializes these submodules and clones their contents.

## 2. Installing the CMake Build Tool

In order to automate the process of *building* the various system components, we use
the CMake tool.  This tool checks for the presence of all other necessary tools and issues warnings for any tools that are missing. 

[Cmake on OS X](https://cmake.org/download/)

### Additional Installation For Windows

make -- gnu make is needed for building embedded software.  The easiest way to install gnu make is with the `chocolaty` package manager.  [choco](https://community.chocolatey.org/packages/make)

Setting up paths  https://david.gardiner.net.au/2020/04/powershell-visualstudio-integration.html
https://docs.microsoft.com/en-us/visualstudio/ide/reference/command-prompt-powershell?view=vs-2019#windows-10

### Building with CMake (additional softare required)

Assuming all the necessary tools are installed the build process consists of creating a build directory, running CMake from within that directory, and then *making* any desired components.  On a linux system, for example, the following sequence of commands will build the BitTag data visualization tool `btviz`.

```shell
mkdir build_directory
cd build_directory
cmake path_to_repository
make btviz
```

The first two lines create a directory in which to *build* the system components;
the third line populates this directory with the scripts necessary to perform the build (in this case Unix "make" files); finally, the fourth line builds the "target" btviz.  
Continuing the example, one can build and download the firmware to a BitTag (e.g. v6) that is connected to a base:

```shell
make BitTagv6-download
```
The CMake build tool can be installed from [here](https://cmake.org/download/).

The various software *targets* that can be built include

* Host Tools
  * qtmonitor -- the tag configuration tool
  * btviz -- the BitTag visualization tool
  * various command-line tools
* Tags
  * AccelTag -- Tag design with ST Accelerometer
  * BitTagv5 -- BitTag with rv-8803 temperature compensated RTC
  * BitTagv7 -- BitTag with rv-3028 45nA RTC
  * LuxTag   -- Tag design with light sensor
  * NucleoTag -- Tag "design" using STM Nucleo board for demonstration
  * PresTAg  -- Tag design with LPS27 Pressure sensor 
* Bases
  * bittag-base-jlcpcb-v3[-dfu] -- Tag base (-dfu for programming)
  * tag-breakout-base-jlcpcb32-v1[-dfu] -- Tag breakout board stlink interface

In addition, cmake files are provided for generating gerber and outline files for the following hardware:

* AccelTag-pcb -- AccelTag board
* tagbase-jlcpcb-v3 -- Tag base board
* MultiCharger-pcb -- Tag charger board

Finally, the 3d printed tag adapter for BitTags can be generated

* multicharger-acrylic

## 3. Building on OS X

The process for building on OS X is similar to that for Linux except that OS X applications are typically *bundled* into 
`.app` directories.  To build these bundles, we depend upon
a tool, `macdeployqt`, that combines the compiled (host) applications
with any necessary libraries in an `.app` directory.  The 
installation of Qt5 is discussed in the section on Software.  In order  for CMake to find `macdeployqt`, you must make sure that 
the directory `qt/5.15.1/bin` is included in your bash path -- note that version number 5.15.1 should be changed to fit the version of Qt5 installed.

All of the OS X applications created by make can be *packed* into 
a singled `.dmg` file for distribution to other users.

```shell
cd build_directory
cpack -G DragNDrop
```

## 3. Building on Windows

Unfortunately, the process for building on Windows is 
significantly more complicated than for other platforms.  The 
fundamental reason for this is the lack of a coherent *path* mechanism,
within a terminal window, for scripts to find applications.  This problem is particularly acute when using the compilation tools 
in Visual Studio from a command line.  To mitigate this problem you 
must:

* Use the powershell window configured for Visual Studio 
* Modify the powershell path environment variable
* Use vcpkg to manage the various software libaries

Powershell integration with visual studio is described [here]( https://david.gardiner.net.au/2020/04/powershell-visualstudio-integration.html) and [here](https://docs.microsoft.com/en-us/visualstudio/ide/reference/command-prompt-powershell?view=vs-2019#windows-10).  Briefly, 
when Visual Studio 2019 is installed, a "Developer PowerShell for VS 2019" is made available in the Start menu.  This PowerShell instance has many of the required path variables installed.

In addition, the path environment variables should be 
modified (described [here](https://docs.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_environment_variables?view=powershell-7.1)) to include the 
paths to the various tools used in the build process (CMake, STM32_Programmer_CLI,FMPP, etc.)

Finally, we use vcpkg to manage the various software libraries (described in the Software section).

With all that accomplished, the process of building becomes

```shell
mkdir build_directory
cd build_directory
cmake -G "Visual Studio 16 2019" -Ax64 -DVCPKG_TARGET_TRIPLET="x64-windows-static" -DCMAKE_TOOLCHAIN_FILE="path_to/vcpkg/scripts/buildsystems/vcpkg.cmake"  path_to_nanotag_repo
cmake --build . --config Release
```

The differences with Linux are, lines 3 and 4.  In line 3,
CMake is directed to configure the build for Visual Studio 2019 using the toolchain file from vcpkg and for a static x64 target.
Line 4 performs the actual build (this configuration does not use make files).

In addition to a Release configuration, one can build a Debug configuration:

```shell
cmake --build . --config Debug
```
One can build and download tags as in 

```shell
cmake --build . --target [BitTagv6|BitTagv5|NucleoTag]
cmake --build . --target BitTagv6-download
```

Furthermore, one can erase tags as in

```shell
cmake --build . --target BitTagv6-erase.
```

Finally, one can build and download baseboard firmware:

```shell
cmake --build . --target bittag-base-jlcpcb-v3
cmake --build . --target bittag-base-jlcpcb-v3-dfu
```


# building with OS X

Use the "brew" version of QT because it links statically.

Make sure to add /usr/local/Cellar/qt/5.15.1/bin (or current version) to your path so that the various find_program tools work (esp macdeployqt)

Building the dmg file for system toos

cpack -G DragNDrop

# 2. Embedded Software

Compiling and downloading embedded software requires several additional tools.

* [gnuarm](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
* [fmpp](http://fmpp.sourceforge.net/)
* gnu make

The primary toolchain for the stm32 processors used in all our devices is the free gunarm toolchain.  ChibiOS designs additionally require the freemarker-based file preprocessor (a java application).  On Windows it will also be necessary to install gnu make (for example from chocolaty).

We utilize the STMicroelectronics [stm32cube programmer](https://wiki.st.com/stm32mpu/wiki/STM32CubeProgrammer) to program and erase both tags and baseboards. 

## Programming with STM32Cube Programmer

The cmake configuration files include rules to download and erase tags and bases.  It is also possible to use the programmer from the command line.  The following illustrates this process on Windows.

```
~/Software/STM32CubeProgrammer/bin/STM32_Programmer_CLI.exe -c port=SWD mode=UR -d ch.elf -v -g 0x08000000 
```

* `port=SWD` driver
* `mode=UR` -- attach under reset
* `-g 0x08000000`  -- execute program after completion
* `-v` verify
* `-vb 3`  verbose logging (for debugging)


### DFU for base boards

The base boards do not have a arm debug interface and 
hence are programmed using DFU mode.  To put the device in DFU mode, plug in USB while holding down the dfu button on the device.

Make sure a device is in dfu mode

```
STM32_Programmer_CLI.exe -l usb
```

To program from command line

```
 ~/Software/STM32CubeProgrammer/bin/STM32_Programmer_CLI -c port=usb1 -d build/ch.elf 
 ```
