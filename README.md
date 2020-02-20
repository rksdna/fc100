# fc100

Two channel reciprocal frequency counter / interval timer with analog interpolators.

Structure:

![Image](/docs/structure.png)

Modes of operation:

- frequency / period measurement, up to 100MHz without prescaler
- time interval measurement (1nS resolution, with interpolators) with independent selection of start and end events (any edge, any input) 
- event counting
- gated frequency / period measurement

Hardware features:

- USB connection to PC
- galvanic isolation
- external connected or built-in 10 MHz reference (TCXO)

Hardware image:

![Image](/docs/hardware.png)

Software features:

- basic statistic and post-processing
- debian / windows portable

Screenshot:

![Image](/docs/software.png)

How to build:

- debian `PATH_TO_REPO/software/deploy/debian/deploy.sh PATH_TO_REPO/software OUTPUT_DEB_DIRECTORY`
- windows `PATH_TO_REPO/software/deploy/windows/deploy.sh PATH_TO_REPO/software OUTPUT_MSI_DIRECTORY` (reqired cross buld of Qt, see `PATH_TO_REPO/software/deploy/windows/i686-w64-mingw32-qt-5.9.5.sh`)







