---
bookFlatSection: false
weight: 3
title: "Hardware Architecture"
math: true
bookCollapseSection: false
type: docs
---

{{< figure src="/docs/architecture/images/tag-simple.svg# center" title="Generic Tag Architecture" >}} 

A high level view of the major components of a generic tag is illustrated above
with optional sub-components indicated by dashed boxes.  Every tag has an interface, a battery, a real-time clock (RTC), and a processor.  In our tags we use the STM32L43x family of processors, which have extremely low-power "sleep" states, and one of several RTC chips -- the RV-3028, RV-8803, and RV-3032.  These RTC chips have exceptionally 
low energy requirements (40-240nA) and high accuracy (3-5ppm), and, for RV-8803 and RV-3032, temperature compensation.  

The "always on" domain includes the core processor, the RTC, and any sensors or flash with sufficiently low quiescent current ($I_q$) to remain powered throughout the tag life. For example, the accelerometer used in BitTags and the LIS2DW12 accelerometer discussed in [custom tags] are both low $I_q$ with quiescent currents of 10nA(50nA), respectively.
Some tags will have external flash and/or sensors with high quiescent current requiring
load switches to disconnect them when idle. Both the LPS27 pressure sensor  and the OPT3002 light sensors used
in the custom tags discussed in the section on customization are high $I_q$ sensors with quiescent currents of 0.9uA (0.5uA), respectively.  Similarly, while most external flash memory has high $I_q$, in our custom tags we use   
a unique low $I_q$ flash -- the AT25XE321, which can store 4mB data and operates 
over a wide voltage range.



{{< columns >}}

As an example, consider the two sides of a BitTag  stores
activity bits/counts for up to 6675 hours (9 months) with a 5.5mAh battery.   The energy performance of this tag is directly due to the ADXL362 accelerometer which has a hardware  state machine for activity detection.  When an "active" accelerometer observes acceleration below a programmed  threshold for a programmed period, it enters a low energy "inactive" state  in which it samples acceleration at 6Hz. When an "inactive" accelerometer detects acceleration above a programmed threshold, it wakes and samples at  a higher rate.   The BitTag firmware wakes the processor whenever the ADXL362 transitions between sleep and awake  states in order to track the active periods.

The major components of a BitTag are the processor (STM32l432), RTC (RV-3028), and accelerometer (ADXL362).  

<--->

{{< figure src="/docs/architecture/images/bittag-annotated.png#center" title="Two sides of BitTag" >}}
{{< /columns>}}


{{< columns >}}

The other major component of our hardware architecture is a base board (right) that provides software access to the tags
through an ARM standard ''serial wire debug'' (SWD) interface.  The tags are connected to the bases by an array of spring loaded ''pogo-pins'' (these contact the  six test points illustrated in the tag photo above) and
are supported by tag-specific 3D printed holders. In addition, our bases provide an external current measurement/voltage source interface to enable accurate dynamic current measurement during firmware development.  The base
hardware utilizes low-leakage analog switches to isolate the tag interface during current measurement.
Finally, our tag bases include a battery charger that supports multiple batteries.

We program and communicate with our tags through the bases using existing open-source tools and libraries such as openocd and st-util. To accommodate these tools, our base firmware emulates the st-link protocol used by ST Microelectronics in their proprietary programmers. 

<--->

{{< figure src="/docs/architecture/images/tagbase2.png#center" title="Tag Base">}}
{{< /columns >}}