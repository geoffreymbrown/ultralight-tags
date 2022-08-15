---
bookFlatSection: false
weight: 10
title: "Preparing for Flight"
description: >
    Preparing tags for flight
---

Preparing BitTags for flight requires the following steps:

1.  Charging -- be prepared to charge each tag for at least 2 days
2.  Configuring -- the data collection protocol including start/end times, hibernation periods, and sensor parameters.
3.  Insulating -- after configuration the test points should be immediately covered with insulating tape
4.  Harness attachment

In this guide we discuss 1-3.  **Note** some of the photos were made with earlier versions of the tags and tag bases; however, the principles remain the same.

## Visual Glossary

### Bit Tags

{{< columns >}}

Ultra-low power reusable accelerometer loggers, capable of logging  activity up to a year

<---> 
![](./images/image42.jpg) 

<---> 

![](./images/image37.jpg)

{{< /columns >}}


### Charger Bases

{{< columns >}}
Charges Bit Tag batteries and displays battery charge status.

<--->

![](./images/image30.jpg)

{{< /columns >}}

### Programmer Base

{{< class "cf" >}}
{{< class "fr w-50" >}}
![](./images/image17.jpg)
{{< /class >}}


Allows data transfer between Bit Tags and computer, for configuration
and data downloading
{{< /class >}}


###  Mini-USB Cable


{{< columns >}}
Delivers power and data to Bit Tag chargers and programming base.

<--->

![](./images/image44.jpg)
{{< /columns >}}

### Insulating Tape (3mm Kapton)

{{< columns >}}
Protects Bit Tag contacts from short-circuits.
<--->
![](./images/image28.jpg)
{{< /columns >}}

###  Tape Application Sticks (4mm width)

{{< columns >}}
Flat wooden sticks, commonly used as coffee stirrers. Useful for
applying tape to Bit Tags, and for measuring tape length.

<--->

![](./images/image23.jpg)

{{< /columns >}}

###  Harness Material

{{< columns >}}
Elastic sewing thread.  May also be available through dental supply houses.
<--->
![](./images/image36.jpg)

{{< /columns >}}

###  Scissors

{{< columns >}}
For cutting tape and harness material. The smaller the better.

<--->
![](./images/image27.jpg)
{{< /columns >}}

### Monitor Program

{{< columns >}}
Configures and downloads data from BitTag

<--->


![](./images/image40.png)

{{< /columns >}}

###  btviz Program

{{< columns >}}
Displays data and generates actograms.

<--->

![](./images/image7.png)

{{< /columns >}}
 

## BitTags
--------

{{< columns >}}

![](./images/image20.jpg)

<--->

![](./images/image21.jpg)

{{< /columns >}}

### Specifications

-   Weight (Without Harness) **0.64g**

-   Dimensions **22Lx9Wx6H mm**

-   Run Time (Depends on mode, limited by battery size)

    -   Bits Per Second **239 hours (\~10 days)**

    -   Counts per Minute **2395 hours (\~100 days)**

    -   Counts per 4 Minutes **9900 hours (\~414 days)**

    -   Counts per 5 Minutes **8700 hours (\~362 days)**

-   Battery Capacity **5.5 mAh**

-   Average Current Consumption **\< 1uA**

-   CPU **STM32L432KC**

-   Accelerometer **ADXL362**

-   Clock Accuracy **±3ppm**

## Pre-Flight

### 1. Charge Bit Tags

{{< columns >}}

Plug in Charger Base and ensure it is receiving power. A green light will illuminate on the Charger Base that is plugged in.  All other Charger Bases can then share power from this Charger Base.

<--->

![](./images/image18.jpg)

{{< /columns >}}

{{< columns >}}

Remove nuts and lid from Charger Base

<--->

![](./images/image41.jpg)

{{< /columns >}}

{{< columns >}}

Place Bit Tag in the plastic holder, ensuring it seats fully.

<--->
![](./images/image38.jpg)

{{< /columns >}}

{{< columns >}}

Place lid on base, aligning dots.

<--->

![](./images/image24.jpg)

{{< /columns >}}

{{< columns >}}
Gently tighten nuts with one finger, to avoid damaging delicate electronics.

<--->

![](./images/image33.jpg)

{{< /columns >}}

{{< columns >}}
* Check to make sure BitTag makes contact with charger.
* Bit Tags typically take 48-27 hours to charge.
* Charging is **Not** complete when the battery indicator turns from red to green.  To fully charge, batteries must be held at their charge voltage for 24-48 hours.
<--->
 ![](./images/image13.jpg)
 ![](./images/image12.jpg)
 {{< /columns >}}


* Remove Bit Tags when done charging. Unused Bit Tags can store  for 1-2 months before needing to be recharged.

* To remove, reverse the installation process.

### 2. Configure Bit Tags

**Note: Images in this section need updating for newer software**

{{< columns >}}
Place Bit Tag in Programmer Base, following the same procedure as installing Bit Tags into chargers.
**Note:** Programmer Base must be plugged into computer before receiving a Bit Tag.
<--->
![](./images/image38.jpg)
{{< /columns >}}

{{< columns >}}
* Open the monitor program.
* Click "Attach"
* Check that battery voltage is **3.0 volts** or greater. If not, detach and recharge. While still functional, battery voltage below 3.0 volts will result in suboptimal runtimes.

<--->
![](./images/image11.png)

![](./images/image1.png)
{{< /columns >}}

{{< columns >}}


* If Bit Tag is in a state other than "IDLE", you may need to "Stop" and then "Erase" Bit Tag.

<--->
![](./images/image3.png)
{{< /columns >}}

{{< columns >}}
* Once the tag is in the "idle" state, run the internal tests ("Test") and then synchronize the clock "Sync".
* If any tests fail, you should not use the tag.
<--->
![](./images/image25.png)
![](./images/image5.png)
{{< /columns >}}

{{< columns >}}

* Click into the "Configure" tab.
* Schedule BitTag Data Collection

<--->
![](./images/image35.png)
![](./images/image6.png)
{{< /columns >}}
{{< columns >}}
* Select Data Type Log Format based upon experiment requirements
    1.  **Activity Bit Per Second**: Gives second-by-second log of whether or not the animal is active. Highest resolution data. Run Time of around **10 days**.
    2.  **Activity Bit Count Per Minute**: Records the percentage of each minute the animal was active. Run Time of around **100 days**.
    3.  **Activity Bit Count Per Four/Five Minutes**: Same as Activity Bit Count Per Minute, but over four or five minute intervals. Lowest resolution, but allows Run
    Time of around **365 days**.
<--->
![](./images/image34.png)
{{< /columns >}}

{{< columns >}}
Click "Run" to save settings and start Bit Tag.
<--->
![](./images/image19.png)
{{< /columns >}}

{{< columns >}}
Return to the "Tag State" tab.
Ensure that the Bit Tag is in the "CONFIGURED" or "RUNNING" state. If not, check your configuration and click "Run" again.
<--->
![](./images/image4.png)
{{< /columns >}}

{{< columns >}}
Click "Detach" and  remove BitTag from base.

<--->
![](./images/image2.png)
{{< /columns >}}

### 3. Insulate Bit Tags

{{< columns >}}
Cut a short strip of Insulating Tape just long enough to fully cover contacts (about 4mm - or the width of tape application sticks) on the back of Bit Tag. 
<--->

![](./images/image22.jpg)
![](./images/image14.jpg)

{{< /columns >}}

{{< columns >}}
Place tape partially on Tape Application Stick (leaving \~2mm overhang lengthwise) 
and gently stick one edge onto Bit Tag

<--->
![](./images/image43.jpg)
{{< /columns >}}

{{< columns >}}

Press stick firmly onto tape and "squeegee" out any air bubbles or pockets. Tape should sit flat on surface of Bit Tag.
<--->
![](./images/image31.jpg)
{{< /columns >}}

### 4 Build Harnesses

See section ???

{{< columns >}}
Bit Tags can be attached via harness using the four mounting holes (1mm diameter).
For songbird, the size of the leg-loop harness can be estimated using the function described by Naef-Daenzer (2007, J. Avian Biology, 38: 404-407)

<--->
![](./images/image29.jpg)

{{< /columns >}}

## Post-Processing

### 1 Recovery

{{< columns >}}
Remove harnesses and insulating tape, being careful not to contact the Bit Tag with sharp or metallic tools.
The Tape Application Stick can also be used to gently peel up the tape.

<--->

![](./images/image45.png)
{{< /columns >}}

{{< columns >}}
* Place Bit Tag into Programmer Base.
  -   Note: Ensure Programmer Base is plugged into the computer first.
* Open the monitor program and attach.
<--->
![](./images/image11.png)
{{< /columns >}}

{{< columns >}}
Stop the BitTag.
<--->

![](./images/image26.png)
{{< /columns >}}

{{< columns >}}
* Save data. **Important:** Save your data as a ".txt" file.
    - **Note: The data length may sometimes show up as zero, even if there is saved data onboard.**
* Detach the BitTag and remove from base.
<--->
![](./images/image15.png)
{{< /columns >}}

### 2 Data Visualization

See Section ???


{{< columns >}}
* Open btviz

* To import data click 'load' and select data file (data file should have a .txt extension)
<--->
![](./images/image8.png)
{{< /columns >}}

{{< columns >}}
If you need to trim your data, enter Start and End times then click "Zoom"
<--->
![](./images/image39.png)
{{< /columns >}}

{{< columns >}}
You may also use your mouse to visually select these times:

1.  Double-click on the chart with the Left Mouse Button to set the Start time (One-finger double-click on Mac).

2.  Double-click on the chart with the Right Mouse Button to set the End time (Double-click with two fingers, or while holding Control (⌃) on Mac).
3.  Click "Zoom"

<--->
![](./images/image9.png)
{{< /columns >}}


{{< columns >}}
You may export your data in a variety of formats:
* **PDF/PNG**: Saves the currently displayed figure as an image
* **CSV**: Saves all data currently visible in the image as a .csv file of data points.
<--->
![](./images/image32.png)
{{< /columns >}}


{{< columns >}}
You may also view and save actograms by clicking on the 'actogram' tab at the top.
<--->
![](./images/image10.png)
{{< /columns >}}


