# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).



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
