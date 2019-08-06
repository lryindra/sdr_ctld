# sdr_ctld
control the LimeSDR by setting the SDR interface to Hamlib NET rigctl
-------------------------------------------------------------------------
This project has the following dependencies:
C++ Dependencies
- C++11
- pthread
- boost (same version as GNURadio)
- Log4cpp (same version as GNURadio)
- ALSA
    - you probably have this already "apt-get install alsa-base"
    - optionally "apt-get install alsa-utils"
- GNURadio default (if this was built from pybombs, setup your prefix)
    - I build from pybombs, but I have had success building with the package from "apt-get install gnuradio-dev".
    - GNU Radio recently added a cmake package. If your version of GNU Radio is older you might need to fix CMakeLists.txt.
- LimeSuite
    - instructions: https://wiki.myriadrf.org/Lime_Suite
    - repository: https://github.com/myriadrf/LimeSuite.git
- gr-limesdr
    - instructions: https://wiki.myriadrf.org/Gr-limesdr_Plugin_for_GNURadio
    - repository: https://github.com/myriadrf/gr-limesdr

This is a CMake project
to build:
- mkdir build && cd build
- cmake ..
- make

copy over the .asoundrc in this folder to ~ (/home/user)
- See audio_notes.txt for more info

Create a audio Loopback device. (I add this to the system boot scripts)
- sudo modprobe snd-aloop pcm_substreams=2

to execute:
- cd build
- ./src/sdr_ctld

After the server is started it should print out "Type q [return] to exit." this means that you can now start WSJT-X.  In WSJT-X, go to File -> Settings... -> Radio and change the following:
- Rig: Hamlib NET rigctl
- Network Server: 127.0.0.1:4532
- PTT Method: CAT
- Transmit Audio Source: Front/Mic
- Mode: None
- Split Operation: None

The transmit/receive data is sent through ALSA by default. There are command line options to select something other than default.

Use these Soundcard settings in WSJT-S and JS8Call:
- Input: alsa_output.platform-snd_aloop.0.analog-mono.monitor
- Output: alsa_output.platform-snd_aloop.0.analog-mono

This software appears as "ALSA plug-in [sdr_ctld]" on Playback and Recording tabs of pavucontrol.
