---
bookFlatSection: false
weight: 100
title: "Command-Line Tools"
description: >
    Tag configuration, testing, and data downloading using command line tools
---

The software library upon which the configuration tool is built makes it easy to create command-line tools.  For example, we make extensive use of a `tag-test` tool during the tag build process.  This tool sets and checks the internal clock and runs the tag self-test routines.

```text
~/Research/NanoTag/build$ ./bin/tag-test
Board name: BitTag V5
Build time and date: Nov 22 2020 : 10:29:27
Internal Flash size: 256kb
External Flash size: 0kb
Repo: git@github.iu.edu:geobrown/NanoTag.git
Repo hash: 3bde753
UUID: 20383055324850090051006E
Voltage: 3.04
Initial Clock drift: -0.07
#  Checking RTC (2 second delay)
Clock drift after setting: -0.02
State: IDLE
#  Running test
Test Result: ALL_PASSED
```

We have planned, but not yet built, a tool to configure tags from stored configuration files.  This tool will:

* run the self-test
* synchronize the clock
* print tag information
* configure the tag from a file

We have developed a simple command line tool -- `tag-dwnld` that downloads data from stopped tags.   In addition, we have developed a primitive command line interfaces `tag-cli` that admits reading the status from, stopping, and erasing tags.

