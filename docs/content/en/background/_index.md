---
title: Background
linkTitle: "Background"
type: docs
weight: 5
bibFile: "resources/Nanotag-hugo.json"
menu:
  main:
    weight: 20
---

## Use of Tags in Studying Bird Behavior

{{< figure
src="/images/pinesiskin.png"
title="Pine Siskin with tag"
attr="Ben Vernasco"
width=300px
class="float-right ml-2"
>}}

Tags have long been used for studies of animal behavior; however, the past decade has seen a
"golden age" for tagging of smaller birds with the emergence of tags with a variety of sensors.
Sensors include light used for geolocation {{< cite bridge2013jfo >}}, {{< cite fudickar2012mee >}},
accelerometers and barometers {{< cite liechti2018me >}}, {{< cite shipley2018mee >}}, temperature 
{{< cite  mccafferty2015ab >}}, and even GPS {{< cite loteka >}}.

For small birds, such as the pine siskin illustrated to the right, 
the tags are attached to subject animals with a
lightweight harness made from elastic thread.  The tag rides under the bird's
feathers on its back and the harness loops under the bird's legs. {{< cite  naef-daenzer2005jeb >}}
{{< cite rappole1991jfo >}}

Many of the early studies utilizing sensing tags focused upon data from accelerometers which 
can be used to determine when animals are active and, to a degree, the type of their activity (e.g.
flight). {{< cite backman2017jab  >}}, {{< cite brown2013ab >}}, {{< cite collins2015ee  >}}.  Accelerometers
have led to some notable discoveries the extended aerial life of swifts. 
{{< cite hedenstrom2016cb >}}, {{< cite liechti2013nc >}}

Pressure sensors have been shown to have great utility in understanding the behavior of birds during
migration. For example, {{< cite dhanjal-adams2018cb >}} demonstrated that by comparing pressure measurements over time it is feasible
to determine which animals from a given site migrate together, {{< cite liechti2018me >}} demonstrated that one can reliably use
pressure measurements to determine when animals are migrating, and {{< cite sjoberg2021s >}} determined that small animals
may fly above 5000 meters during migration.





## The Problem

{{< figure
src="/images/BirdMass.png"
width=300px
class="float-right ml-3 text-right"
attr="(kays2015s)"
>}}

Tag mass is limited by the species being studied.  The adjacent figure illustrates 
the distribution of mass for bird species. {{< cite kays2015s >}}  The data for this 
figure were drawn from sources such as {{< cite blackburn1994o >}} and {{< cite dunning2007choabmse >}}.
Among North American birds, 12% of species are $20-30g$ and 27% of species are $10-20g$.

While there are no fixed limits on allowable tag mass, previous studies have limited them 
to 3-5% of body mass (e.g. {{< cite kenward2001 >}}) with 3-4% becoming a common restriction.
With these tighter limits, animals in the range $10-30g$ can only be studied with tags in the
ranges of $0.3-0.9g$ to  $0.4-1.2g$, respectively.  There have been a number of studies that attempt to assess the
impact of tag mass on animal survival and breeding (for example, {{< cite atema2016be >}}, {{< cite bell2017i >}})


A large fraction of tag weight is dedicated to energy storage and a large portion of the design 
effort for a tag is dedicated to energy efficiency.  The tags described in this website utilize
rechargeable lithium manganese batteries.  Three such batteries are presented in Table 1. Notice
that the capacity of all three (at $2.5V$) is roughly $225J/g$.  This is similar to other 
rechargeable battery chemistries.

<div class="table-caption">
  <span class="table-number">Table 1:</span>
  Seiko MS Series Batteries {{<cite siimed >}}
</div>

|  Type   | Size (DxH) mm | Mass ($g$) | Capacity ($mAh$) | Capacity ($J$) |
| :-----: | :-----------: | :--------: | :--------------: | :------------: |
| MS518SE |    5.8x1.8    |    0.13    |       3.4        | 30.6           |
| MS621FE |    6.8x2.1    |    0.23    |       5.5        | 49.5           |
| MS920SE |    9.5x2.1    |    0.47    |        11        | 99             |

The BitTags described in this site range from $0.45-0.85g$ with the three different batteries 
shown in Table 1.  

At the scale of the tags we describe, energy harvesting is not currently practical.  The additional
weight of the energy harvesting components and energy conversion electronics
would be significant. 


## This Project

We present an activity monitor, BitTag, that can continuously collect data for 4-12 months at $0.5-0.8g$, depending upon battery choice, and which has been used to collect more than 200,000 hours of data in a variety of experiments.

The BitTag architecture provides a general *platform* to support the development and deployment of
custom sub-$g$ tags.  This platform consists of a flexible tag architecture, software for both
tags and host computers, and hardware to provide the host/tag interface necessary for preparing tags
for "flight" and for accessing data "post-flight".

We present designs for custom tags with a variety of sensors and a process for developing them that 
utilizes a purpose-built development platform with off-the-shelf sensor evaluation boards to enable both
accurate energy and power measurements as well as supporting software development.   The host/tag software
architecture, built using Google protocol buffers, makes it straightforward to extend host and tag software to support new tags with backward compatibility.

The host hardware -- to program, configure, and charge tags -- is designed to support a variety of batteries and to enable new tags, with different physical outlines, to be supported simply by creating a new 3d-printed adapter. 

## Acknowledgement
 

This material is based upon work supported by the National Science Foundation Grant Number 1644717.  Any opinions, findings, and conclusions or recommendations expressed in this material are those of the author(s) and do not necessarily reflect the views of the National Science Foundation.


## References 

{{< bibliography cited >}}
