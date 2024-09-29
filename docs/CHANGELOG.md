# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

### July 2024
- Support for CD audio added [PR](https://github.com/dethrace-labs/dethrace/pull/393)

### June 2024
- Reverse engineered BRender code replaced with now-open-sourced [BRender-v1.3.2](https://github.com/dethrace-labs/BRender-v1.3.2)
- Replaced home-made OpenGL renderer with BRender software renderer
- [PR](https://github.com/dethrace-labs/dethrace/pull/363)

### July 2023
- First cut of network code
  
### May 2023
- Fixed multi-channel audio which worked in the original DOS code, but was broken in the original win95 code [PR](https://github.com/dethrace-labs/dethrace/pull/329)

### November 2022
- Action Replay mode added. [PR](https://github.com/dethrace-labs/dethrace/pull/230)
  
### October 2022
- "hires" SVGA 640x480 mode added. [PR](https://github.com/dethrace-labs/dethrace/pull/217)
### June 2022
- Audio added, thanks to [miniaudio](https://miniaud.io/). [PR](https://github.com/dethrace-labs/dethrace/pull/130)

### May 2022
- Pedestrians added! [PR](https://github.com/dethrace-labs/dethrace/pull/118)
  
### April 2022
- Vehicle damage added. [PR](https://github.com/dethrace-labs/dethrace/pull/116)

### March 2022
- Fog depth effects added. This really gives a boost to looking like the original game now. [PR](https://github.com/dethrace-labs/dethrace/pull/95)
  
### January 2022
- Texturing implemented, no more ghostly shades of grey for everything! [PR](https://github.com/dethrace-labs/dethrace/pull/69)

### September 2021
- Vehicle physics are implemented enough to drive around parts of the tracks [PR](https://github.com/dethrace-labs/dethrace/pull/57)

### July 17 2021
- 3d rendering somewhat implemented. Flat shaded, no textures, but camera, cars and track are visible in the right place!

### April 21 2021
- Actual game main loop running! Rendering, phyiscs, sound etc commented out, so all we can see is a black screen with the 2d rendering.

### March 24 2021
- Menu screens up to and including race and car selection implemented
- Fixed OpenGL crashes by moving back to single thread

### September 3 2020
- Cutscenes displayed on startup.
- Main menu renders and responds to key up and down events. Hitting "enter" on any option causes a crash

### July 6 2020
- OpenGL renderer added. Shows nothing but a black screen then exits.

### November 25, 2019
- First unit tests added and hooked up to the CI build pipeline

### November 18, 2019
- c skeleton compiles for the first time on OSX and Linux!

### November 14, 2019
- First [commit](https://github.com/jeff-1amstudios/dethrace/pull/9) of the correct generated c skeleton. It does not compile.

### November 12, 2019
- A [bug fix](https://github.com/jeff-1amstudios/open-watcom-v2/commit/1a00368a6c5d8dddb1d27f972ef21e399dd48b60) in Watcom `exedump` finally allowed fully correct function args to be identified

### October 27, 2018
- crayzkirk has been working with IDA to match up the symbols by hand from an older build to the released binary

### March 18, 2017
- crayzkirk finds richer debug information by using Watcom `exedump` tool 

### December 14, 2014
- Carmageddon Watcom debug symbols [discovered and dumped](http://1amstudios.com/2014/12/02/carma1-symbols-dumped/)
