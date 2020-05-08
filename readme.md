# NautiLum

## The Project

NautiLum brings any [NOAA Certified Nautical Chart](https://nauticalcharts.noaa.gov/) to life. 

Nautical charts are one of many tools used by boaters for nautical navigation. By providing a visual reference for both geographical features and navigational markers, they can be used for things like maintaining one's position ploting a course, etc. Essentially, they're nautical roadmaps.

One key feature of nautical chars are a vareity markers that represent their real-life counterparts. These can be buoys, spindles, or lighthouses, which often have lights that flash at specific intervals. These intervals are specified on the chart itself and used by boaters to identify each marker while on the water. See the table below for a list of these different flashing characteristics:

| Characteristic           | Abbr.   | Definition                                                      | Example       | Representation                               |
|--------------------------|---------|-----------------------------------------------------------------|---------------|----------------------------------------------|
| Fixed                    | F       | A continuous, steady, light.                                    | F R           | ![image](images/Light_Code_F_R.gif)          |
| Flashing                 | Fl      | A flash is repeated regularly at a rate below 50 per minute.    | Fl G 5s       | ![image](images/Light_Code_Fl_G_5s.gif)      |
| Occulting                | Oc/Occ  | A dark period is repeated regularly.                            | Oc R 6s       | ![image](images/Light_Code_Oc_R_6s.gif)      |
| Quick                    | Q       | Quick flashes are repeated regularly.                           | Q W           | ![image](images/Light_Code_Q_W.gif)          |
| Group Flashing           | Fl(x)   | A group of a specific number of flashes are repeated regularly. | Fl(3) R 15s   | ![image](images/Light_Code_Fl(3)_R_15s.gif)  |
| Composite Group Flashing | Fl(x+y) | Similar to group-flashing, but with several groups of flashes.  | Fl(2+1) W 15s | ![image](images/Light_Code_Fl(2+1)_W_5s.gif) |

[Source](https://en.wikipedia.org/wiki/Light_characteristic)

## The Code

The sketch (buoys.ino) uses a single Buoy class, in which each "buoy" with an individual flashing characteristic is instantiated with the parameters (pin, buoyType, period, group1=0, group2=0), where:

`pin` = Digital Output In on the Arduino

`buoyType` = which flashing scheme should be used (see list in code)

`_period` = the period of the flash sequence

`_group1` = for group flash, how many flashes in the first group (defaults to 0)

`_group2` = for group flash, how many flashes in the second group (defaults to 0)

In the main loop, the `millis` function is used to measure time intervals, which get passed to the `flash` method of each buoy, in order to set the output of the specified pin `HIGH` or `LOW`.