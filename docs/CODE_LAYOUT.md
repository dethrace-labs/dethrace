# Code layout

### DETHRACE
Game logic. According to the symbol dump, these files were originally stored in `C:\DETHRACE\src`.

- `DETHRACE/common` - all common logic
- `DETHRACE/pc-dos` - all platform-specific functions (DOS, in this case)
- `DETHRACE/pd` - platform-dependent generic headers.

_All code here is kept as similar to how we think the original code might have been. Any changes required are implemented as hooks into `harness`._

### BRSRC13

Graphics rendering library. [BRender](https://en.wikipedia.org/wiki/Argonaut_Games#BRender), originally stored in `C:\BRSRC13`. 

- Stainless Software used their own build of BRender with unknown modifications.

_All code here is kept as similar to how we think the original code might have been. Any changes required are implemented as hooks into `harness`._

### S3

Audio library. No other information. 

_All code here is kept as similar to how we think the original code might have been. Any changes required are implemented as hooks into `harness`._

### harness

- Provides functions that the original game logic calls to implement modern cross-platform support. 
- SDL2 for windowing + input + networking, OpenGL for rendering, OpenAL for audio
