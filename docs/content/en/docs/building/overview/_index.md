---
bookFlatSection: false
weight: 1
title: "Software Tools/Libraries"
bookCollapseSection: false
type: docs
---

In addition to CMake and Git, which were discussed in this [Page](),
the host, tag, and base software requires a number of tools and libraries. To compile
the host tools, we require:

* A C++ compiler
* Python (version 3.7 or later)
* `vcpkg` --    package management system (Windows only)
* `homebrew` -- package management system (OS X only)

We also require the following libraries

* Google Protocol Buffers -- libraries and compiler for google protocol buffers
* libusb -- a library for interacting with USB devices
* Qt 5 -- GUI interface libraries

Because we support compilation on three different types of systems, the installation process
can be involved.   The goal is to both install the tools and libraries, and to insure that CMake 
can find them during the build process.  Furthermore, where possible, we would like to target 
static libraries in order to make distribution of the compiled software straightforward.

## 1.1 C++ Compiler 

### Linux

Although many Linux distributions include C++, we also require some standard development tools 
in order to build the various libraries that we install.  On Ubuntu systems the compiler and these  tools can be installed from the command line:

```shell
$ sudo apt update
$ sudo apt-get install build-essential
```

### OS X

The compilation tools we need are part of [Xcode](https://developer.apple.com/xcode/) which is
a free download.  Once Xcode is installed, you need to enable the command line tools.  In a 
terminal window (/Applications/Utilities/...):

```shell
xcode-select -inst-all
```

### Windows


Install the [Visual Studio 19 Community Edition](https://visualstudio.microsoft.com/downloads/)

## 1.2 Python 

Install at least version 3.7.


* [Linux Python3](https://docs.python-guide.org/starting/install3/linux/)
* [OS X Python3](https://www.python.org/downloads/macos/) 
* [Windows Python3](https://www.python.org/downloads/windows/)

## 1.3 vcpkg (Windows Only)

Because Windows has a (very) broken method for managing paths to libraries and 
applications, we use the [vcpkg](https://vcpkg.io/en/getting-started.html) manager
for installing the various libraries that we require.

Choose an appropriate location in which to install `vcpkg`, then, in a powershell window:

```shell
$ git clone https://github.com/Microsoft/vcpkg.git
$ ./vcpkg/bootstrap-vcpkg.sh
```
**Note** you may need to add the path to git to your powershell environment, for example by adding

```shell
$env:Path += ";PathToGit"           
```
to the file `$Home\Documents\PowerShell\Profile.ps1`.  

## 1.4 Homebrew (OS X only)

Directions for installing homebrew are [here](https://brew.sh/)

The basic approach is to execute the following in a terminal window:

```shell
$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
``` 

## 1.5 Protocol Buffers

Google Protocol Buffers is a fundamental tool for both host and tag software as we use 
protocol files in the protocol buffer language to define host/tag communications.

### Linux

```shell
$ apt install -y protobuf-compiler
$ protoc --version  # Ensure compiler version is 3+
```

### OS X

Install using home brew 

```shell
$ brew install protobuf
$ protoc --version  # Ensure compiler version is 3+
```

### Windows

On windows we use the vcpkg manager to handle the installation.  In a powershell 
in the directory where you installed vcpkg, execute:

```shell
$ .\vcpkg install protobuf:x64-windows-static
```



## 3. Host Software Libraries

Both host and embedded software require Google Protocol buffers.  In addition, the 
host software requires Qt5 for graphical user interfaces and libusb to communicate with tag bases.  


### OS X
* qt5 -- install with homebrew. 
* libusb -- install with homebrew.

### Linux

* qt5 -- [install](https://wiki.qt.io/Install_Qt_5_on_Ubuntu_)
* libusb -- `sudo apt-get install libusb-dev`

### Windows

Building large applications on Windows is challenging because of the lack of 
reasonable "path" mechanism allowing compilation tools to find the necessary libraries and because building portable applications requires "collecting" all of the necessary link libraries from the host OS. To solve the "path" problem, we utilize the vcpkg package manager and to solve the link library problem we utilize static builds of the necessary software libraries.

Instructions for installing vcpkg manager can be found [here](https://vcpkg.io/en/getting-started.html). Once vckpg is installed you may install the necessary packages using the following command:

```shell
vcpkg install [packages to install]
```

You will need to install the following packages:

* protobuf:x64-windows-static   
* qt5:x64-windows-static       
* libusb:x64-windows-static
* angle:x64-windows-static  


Notice that in each case we have selected the "static" version of the libraries.  This ensures that the binaries that you build are portable across windows systems.  The last of these packages (angle) provides openGL graphics compatibility.


## 4. Embedded Tools

* fmpp
* java
* gnuarm
* st programmer




