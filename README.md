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

- USB connection as CDC device class
- frontend galvanic isolation
- standard 1 MΩ / 10 pF inputs for 10:1 oscilliscope probes (±5 threshold range, 20 mV sensitivity)
- external connected or built-in 10 MHz reference (TCXO)

Hardware image:

![Image](/docs/hardware.png)

Software features:

- basic statistic and post-processing
- Debian / Windows portable

Screenshots:

![Image](/docs/software.png)

How to build:

- Debian `REPO_PATH/software/deploy/debian/deploy.sh PATH_TO_REPO/software OUTPUT_DEB_DIRECTORY`
- Windows `REPO_PATH/software/deploy/windows/deploy.sh PATH_TO_REPO/software OUTPUT_MSI_DIRECTORY` 

Cross-compile Qt for Windows: `REPO_PATH/software/deploy/windows/i686-w64-mingw32-qt-5.9.5.sh`
