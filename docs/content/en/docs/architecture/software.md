---
bookFlatSection: false
weight: 20
title: "Software Architecture"
bookCollapseSection: false
type: docs
---

The system software for NanoTags consists of firmware (the software running on the tags), a host library supporting communication with tags that are connected through a base, and various host applications utilizing this library including a configuration GUI, and command-line tools for testing and
commissioning tags.  Additionally, we have developed a data analysis tool for BitTags which we will not discuss in this section.

Our goal in developing this system software was to create an architecture that can "grow" with the addition of new types of tags and the creation of specialized firmware to support specific experiments.   At the heart of our architecture is communication and data
transfer (definition) through Google Protocol Buffers.   The fundamental idea of Protocol Buffers is to provide a language for describing data (messages) and a standard way to encode these data in a binary serial form.   The message (data) definitions then form the interface between communicating systems -- in this case tags and system software.  In practice, protocol buffers provides much more in the form of tools that enable their use with a wide variety of languages including compilation of message definitions into language specific libraries.   Protocol Buffers have efficient support for use in embedded code through the nanopb tools.

The use of protocol buffers greatly simplifies the problem of providing a coherent and extensible interface between tags and hosts.  By compiling from common source -- the message definitions -- all aspects of shared data are kept coordinated.  Further, Protocol Buffers provides a standard way to extend the message definitions without breaking backwards compatability.  Thus, we can add support for new tags by extending the message definitions (for example to support new sensor types), and extend the host tools to utilize these new definitions without "breaking" support for existing tags.  This is particularly important where tags "in flight" may be in use for many months.


# Architecture

We  begin by presenting an abstract model of the
tag "life cycle" which describes the execution of an arbitrary tag.  This tag lifecycle
will serve to motivate the interface to the host library used by our host
applications; we will demonstrate this library with a simple application
that sets the real-time clock on an arbitrary tag.  We then discuss how the
methods of this host interface library are realized with Protocol Buffer based RPC and finally discuss
the tag firmware.

## Tag

{{< figure src="/docs/architecture/images/lifecycle.svg#center" title="Tag Lifecycle" >}}

Consider the life cycle of a tag.
which corresponds directly to the runtime state machine.
Notice that there are three *categories* of states - *Erased*, *Active*, and *Completed*.  In the *Erased* states, no data or configuration is stored in non-volatile memory.  The *Active* states correspond to a tag that is configured or is actively collecting data.  In these states, configuration, and event and data logs are stored in non-volatile memory.   Finally, the *Completed* states are post-experiment in the sense that data collection has ceased by design, by external command, or through an unrecoverable error (e.g. power brown-out).  In the  *Completed* states, the event and data logs from the last *Active* period are preserved in non-volatile memory.

In addition to organization by groups of states, our figure distinguishes between three types of state transitions.   Internal transitions
are shown as *dashed* arrows, transitions in response to an external command are shown as *solid* arrows, and the transition after commissioning is shown as a *double* arrow.

The initial state after commissioning and erasure is **Idle**.  From **Idle** various tag-specific tests can be executed under host control.  When a test is initiated, that tag enters the **Test** state; once the test is completed, the tag returns to the **Idle** state.  Test
results are stored in volatile memory.

An experiment (biological or development) is initiated by the host from **Idle**, by configuring the tag with a **Start(config)** command.  The tag configuration includes stop and end times, hibernation periods (when no data are collected), and, tag-specific parameters for sensors and data collection.

From the **Configured** state the tag enters the **Running** state once the configured start time (Config.start) has been reached.  In **Running** the tag actively collects data;  a tag may *hibernate* (2) for a configured hibernation period during which all data collection ceases.  The fundamental difference between **Hibernation** and **Running** is that in the former, all sensors are powered down and the tag enters the lowest possible energy (dormant) state while in the
later the tag is either quiescent (waiting for an event) or the
processor is actively running.

Under ordinary conditions, a tag remains active until an end condition is met.  This can be a configured end time, exhaustion of available data storage, or exhaustion of available energy.  In each of these cases, the tag will enter **Finish**.  In
the occurrence of an unplanned (1) event, for example a power brown-out, all of the active states will transition to **Aborted**.

The tag is returned from a completed state to **Idle** via an external **Erase()** command; the intermediate **Reset** manages the erasure of the non-volatile storage.


## Host

The host software consists of a code library that provides a procedural interface to a tag, and applications that use this library.  These applications
include a graphical user interface intended to be used by researchers who need to configure tags and download collected data, and command-line applications that can be used to test tags and configure them from stored configuration files.  The host library is written in C++ an provides a 
tag object model that allows software to use a procedural interface for tag access.

{{< columns >}}
Consider again the state diagram describing the tag "lifecycle" and notice the state transitions labeled Test(), Start(config), Stop(), and Erase().
These correspond directly to methods of the tag class.

The **Start()** command takes a configuration object as a parameter.  In execution, this
configuration object (a Protocol Buffer message) is transferred to the tag; when the tag evaluates the Start command, it records the configuration and enters the **Configurated** state.  It is possible to abort execution of a tag by issuing a **Stop()** command.  
A tag may have its data erased with the **Erase()** command.   Finally, all tags have some built-in test functions.  The test sequence
is initiated with the **Test()** command and test status (including failures) returned as a string with the **GetStatus()** command.
 
<--->

```c++

bool Start(Config &config);
bool Stop();
bool Test(TestReq test);
bool Erase();
bool GetStatus(Status &status);
```



```c++
bool GetConfig(Config &cfg);
bool SetRtc();
```

{{< /columns >}}

{{< columns >}}

Because the tag software architecture can support a variety of tag types, it is important that the application support a degree of introspection.  This allows the software to determine the type of the
tag, significant aspects of the tag software that admit tracing
back to the github repository from which the software was built, and
the tag processor's unique identifier (UUID).

<--->

```c++
struct TagInfo {
  TagType tag_type;       // tag type
  std::string board_desc; // board description
  std::string uuid;       // processor unique id
  int64 intflashsz;       // internal flash size (bytes)
  int64 extflashsz;       // external flash size (bytes)
  // software
  std::string firmware;    // firmware description
  std::string gitrepo;     // git repo
  std::string githash;     // git version hash
  std::string source_path; // path to source in repo
  std::string build_time;  // build time
  };

bool GetTagInfo(TagInfo &info);
```

{{< /columns >}}

Finally, the tag library provides methods for downloading data and state logs.  During execution, the tag records each of the system states that it executes (**Configured**, **Running**, etc.) along with key state information such as the time the state was entered, the amount of data recorded, etc.  This system log is accessed through a method that returns this sequence of recorded states.

```c++
bool GetSystemLog(std::vector<State> &system_log);
```

The type(s) of data recorded by various tags depends upon the sensors on the tag and the tag configuration. This variation 
is handled by returning the protocol buffer acknowledgement message to be decoded by a separate library which converts data messages to human readable form

```c++
 bool GetDataLog(Ack &data_log, int index);
```

The amount of data returned by a single execution of this method is implementation specific; the *index* parameter indicates the starting point for the next block of data to be returned.

Finally, the tag library provides methods to connect and disconnect from tags.

```c++
bool Attach(UsbDev usbdev = UsbDev());
bool Detach();
bool IsAttached(); 
```

From the perspective of host software, a tag appears as a USB device (a base) to which the host software attaches and detaches.  The optional parameters of the **Attach** method allow selecting a specific device.

## An Example

{{< columns >}}
We exploit this interface both for a GUI based
tool that we provide to biologists and for
command-line tools to test and commission tags.
The following example illustrates the simplicity
of this interface for creating custom command-line
tools -- in this case to set and check the RTC
for short term drift.
This is one of the steps performed in the commissioning process
to ensure that the RTC is correctly configured.

The first step is to *Attach* to the tag (through a base connected by USB).  Once the
tag is attached, its real time clock can be set (based upon the host clock).  In this example, the host sleeps for 2 seconds and then reads the tag status which includes its current RTC value in milliseconds.  Finally, the test program computes the clock drift.   While this may appear a useless exercise, it can detect whether the tag RTC is misconfigured or not running.

<--->

```c++
int main(int argc, char **argv)
{
  using MS = chrono::milliseconds;
  Tag tag;
  Status status;
  int64_t millis;

  if (tag.Attach())
  {
    // set the RTC

    tag.SetRtc();

    // sleep 2 seconds

    this_thread::sleep_for(MS(2000));

    // Read the RTC

    tag.GetStatus(status);

    // Compute drift

    auto now =  chrono::system_clock::now();
    auto ts =  chrono::time_point_cast<MS>(now);
    float error = (status.millis()
          - ts.time_since_epoch().count())/1000.0;
    cout << "Clock Error: " << error << endl;
  }
}
```
{{< /columns>}}

# Communication Model

Recall our basic system model consisting of a tag and an associated base..

{{< figure src="/docs/architecture/images/genericsystem.svg#center" width=600px >}}

For communication between the base and a tag, we utilize a standard serial interface present in all ARM based processors --
the SWD (serial wire debug) protocol for communication.  By utilizing this interface, our architecture
can, in principle, support tags built with any ARM Cortex embedded processor.  The 
physical host/base communication utilizes the ST Microelectronics stlink protocol over USB.  By leveraging
these existing interfaces, we can exploit off-the-shelf tools to program and debug tags.  For example,
we use openocd, an open source package that supports the stlink protocol, for programming and debugging the
tag firmware.   The only host device driver required is libusb, which is supported on large number of operating
systems including linux, windows, and OS X.

Our system architecture leverages these physical and link-layer protocols (swd/stlink/usb) to support host-tag communication.   Host-tag communication is implemented  with a simple remote procedure call (RPC) protocol that commnicates through the SWD debugger interface with the tag processor and utilizes a feature of Cortex-M3 and Cortex-M4 based processors that supports "debug monitor" interrupts. The RPC messages are encoded with the Google Protocol Buffers message format.

We support the tag host library described previously with a low-level interface that communicates over USB to the base and through the base over SWD to the tags.  The definition for this interface is:

```c++
bool Attach(UsbDev dev = UsbDev());
void Detach();
bool Voltage(float &voltage);
bool Rpc(size_t *size);
size_t MaxPacket();
```

There are methods for attaching/detaching that mirror those in the tag library, a few methods for status information
(**Voltage()**, **MaxPacket()**, and a remote procedure call (**Rpc**) method that uses a private buffer for the exchange.  As we shall see, our Protocol Buffer definitions include two major messages -- **Req** and **Ack** supported by a number of other message types.   Hosts send requests (**Req**) to tags and receive acknowlegements (**Ack**) in return.

## Protocol Definitions

{{< columns >}}
As discussed above, communication with the tags is implemented through a remote procedure call mechanism encoded with protocol buffers.  The Req(uest) message is defined as illustrated to the right.  A request message  contains on of the various choices.  For example, the current tag state is requested with a request containing an (empty) **get_status** message and the current configuration is requested with an (empty) **get_config** message.  Some request messages are non-empty.  For example, the **start** request is encoded as a **Config** message, which, as we shall see, carries all of the information necessary to configure a tag such as start/stop times, hibernation periods, sensor and data logging configuration.

The current system and data logs can be requested with **log** message.   The **LogReq** message type will be discussed subsequently.

In the protocol buffer languages message *fields* have unique *tags* (here,the integers 1-9).  These tags play an important role in encoding and decoding messages.   Furthermore, (sub)-messages can be defined as new types and their contents embedded in other messages.  An important characteristic of the encoding/decoding process for protocol messages is that unknown fields are ignored; the consequence of this is that messages in our application can be extended with new fields to support new types of tags without breaking backwards compatibility with exisitng tags.



<--->

```protobuf
message Req {
  oneof payload {

    // Information

    Empty get_info = 1;   // static tag information
    Empty get_status = 2; // current tag status
    Empty get_config = 3; // current tag configuration

    // control

    Empty erase = 4;   // erase tag (return to Idle)
    Config start = 5;  // configure & start tag
    Empty stop = 6;    // stop tag
    TestReq test = 7;  // start test
    int64 set_rtc = 8; // set clock (milliseconds)

    // logs -- see log message for types

    LogReq log = 9;
  }
}
```

{{< /columns >}}

{{< columns >}}

Tag info is returned as a
message with fields matching the
C++ interface described above.

<--->

```protobuf
enum Info {
    Info_NOT_DEFINED = 0;
    MONITOR = 1message TagInfo {
  TagType tag_type = 1; // tag type (should match config)
  // hardware
  string board_desc = 2; // board description
  string uuid = 3;       // processor unique id
  int64 intflashsz = 4;  // internal flash size (bytes)
  int64 extflashsz = 5;  // external flash size (bytes)
  // software
  string firmware = 6;    // firmware description
  string gitrepo = 7;     // git repo
  string githash = 8;      // git version hash
  string source_path = 9; // path to source in repo
  string build_time = 10;   // build time
}
```
{{< /columns >}}
Because of the properties of protocol buffers, the definitions for the **Req** message and **Info** enumerated type can
be extended as needed for additional tag types.   The only requirement is that the additional fields are given new and unique numbers.
These additional fields will be ignored by tags not supporting them.

{{< columns >}}
Every **Req** sent to a tag must be acknowledged.  The **Ack**(nowlegemnt) message contains two fields -- an error code and return data of various types.    The data returned in response a **Req** depends both upon the type of the request and the type of the tag.   This is
particular the case for log data.  This example provides a single log data type.  New tags or tag firmware may necessitate adding
additional return types.

<--->

```protobuf
message Ack {
  enum Err {
    UNSPECIFIED = 0;
    OK = 1;
    // Host Error
    EMPTY_RETURN = 2; // return type unexpected
    MONITOR = 3;      // reserved for monitor
    // Tag Error
    NODATA = 4; // no data
    INVAL = 5;  // invalid operand
    PERM = 6;   // operation not permitted
    NXIO = 7;   // operation failed
    NANOPB = 8; // nanopb error
  }

  Err err = 1;
  oneof payload {

    TagInfo info = 2;
    Config config = 3;        // configuration return
    Status status = 4;        // current status
    string error_message = 5; // debugging support

    // Log types

    StateLog system_log = 6;
    BitTagLog bittag_data_log = 7;
     ... other data log types added as needed ...
  }
}
```

{{< /columns >}}

The protocol definitions include a number of specialized messages for carrying data including
tag status, configuration, and data/system log messages.   For example, the tag status message includes the current *state* (from the tag lifecycle diagram), current RTC value, and various other internal information:

```protobuf
enum TagState {
  STATE_UNSPECIFIED = 0;
  TEST = 1;        // self test in progress
  IDLE = 2;        // ready
  CONFIGURED = 3;  // configured
  RUNNING = 4;     // actively collecting data
  HIBERNATING = 5; // running, but not active
  ABORTED = 6;     // error, or stop received
  FINISHED = 7;    // normal termination
  sRESET = 8;      // reset in progress
  EXCEPTION = 9;   // unhandled exception
}

message Status {
  int64 millis = 1;              // current time in ms
  TagState state = 2;            // current state
  TestResult test_status = 3;    // test result
  int32 internal_data_count = 4; // number of internal data entries
  int32 external_data_count = 5; // number of external data entries
  float voltage = 6;             // system voltage
  float temperature = 7;         // processor Temperature
}
```
With the creation of new tags, some message types are extended (configuration) and new message types added (data logs).  In general, the protocols for our tags are designed to be extensible in the sense that these changes should be backwards compatible with existing tags.

# Tag Firmware

The tag firmware, written using the ChibiOS RTOS and using the Nanopb implementation of protocol buffers is organized as two threads.  One that executes the state machine in the lifecycle and the other, which is only created when an external device connects, handles protocol buffer communications with the host.

The state machine organization allows us to isolate the changes necessary to create new tags.  In particular, only the portions of the code related to configuration, data collection, and data logging need to change.  Creating a tag with a new type of sensor requires providing a device driver for the sensor and routines to initialize and query the sensor.  Furthermore, it requires extending the configuration message, mechanisms for storing and retrieving the configuration, and the mechanisms for storing and retrieving data.