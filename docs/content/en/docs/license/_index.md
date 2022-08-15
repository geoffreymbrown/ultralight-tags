---
bookFlatSection: false
weight: 150
title: "Licenses"
bookCollapseSection: false
type: docs
---

# Applicable Licenses

The code in this repository builds upon the contributions of a number of other projects.   Unless otherwise stated, or unless other license terms dominate, the software developed for this project is distributed under 
a [BSD 3-clause license](/software_license).

For example, the Qt based applications were developed using the Qt GPL terms, therefore those applications honor the terms of that licence.  In contrast, the command-line applications rely on libraries whose license terms that do not take precedence over the project license.

**A Request**

We request that any use of this work or derivatives of this work in scientific research appropriately cite our contributions in any publications.

how to cite:


## 1. Host Software

We consider the host software in three categories -- the tag and monitor libraries,  and the various applications.   The tag/monitor libraries build upon libraries that are compatible with our project license.

### 1.1 Tag/Monitor Libraries

The monitor library provides connectivity, via USB, to the baseboard which emulates an stlink device.  The monitor library utilizes libusb which is
licensed under the [lesser gpl 2 licence](/lgpl2_1.txt).  In addition, the monitor library code utilizes constants and ideas from stlink-org which is licensed under a [BSD 3-clause licence](/stlink-org-license.md).   

The tag library uses code built with the Google ProtoBuf compiler and linked to the Google ProtoBuf libraries that code is licensed under a [Google License](/protobuf-license.txt).

### 1.2  Command-line Applications

The command-line applications use the monitor and tag libraries; any other code is licensed under the default terms described above.

### 1.3 Qt Based Applications

The two Qt based applications were developed under the Qt open source license terms and hence the use of Qt libraries is covered by the [LGPLv3](/lgplv3.txt) license.  ** how to get QT source code **

The BitTag data visualization application (btviz) uses the QCustomPlot library and hence is licensed under [GPLv3](https://www.gnu.org/licenses/gpl-3.0.html).  The creator of [QCustomPlot](https://www.qcustomplot.com/) offers commercial licenses.  In this case, the Qt open source license terms apply to the use of Qt libraries, and our project license applies to the remaining source code.

## 2. Embedded Software

### 2.1 Base Boards


### 2.2 Tags

## 3. Hardware
