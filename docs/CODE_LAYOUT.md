# Code layout

### DETHRACE
Game logic. According to the symbol dump, these files were originally stored in `C:\DETHRACE\src`.

- `DETHRACE/common` - common game logic
- `DETHRACE/pc-dos` - DOS-specific functions
- `DETHRACE/win95sys.c` - Windows-specific functions
- `DETHRACE/pd` - platform-dependent generic headers.

_All code here is kept as similar to how we think the original code might have looked. Any changes required are implemented as hooks into `harness`._

### BRSRC13

Graphics rendering library. [BRender](https://en.wikipedia.org/wiki/Argonaut_Games#BRender), originally stored in `C:\BRSRC13`. 

- Stainless Software used their own build of BRender with unknown modifications.

_All code here is kept as similar to how we think the original code might have looked. Any changes required are implemented as hooks into `harness`._

### S3

Audio library. Possibly short for "Stainless Sound System"?! Supports at least two audio backends - [SOS](http://web.archive.org/web/19990221132448/http://www.humanmachine.com/sos.html) (DOS) and DirectSound.

_All code here is kept as similar to how we think the original code might have looked, with the addition of a small amount of code integrating [miniaudio](https://miniaud.io)

### smackw32

Implements the [RAD Smacker lib](https://wiki.multimedia.cx/index.php/RAD_Game_Tools_Smacker_API) interface. The implementation is backed by [libsmacker](https://libsmacker.sourceforge.net/).

### harness

- Provides functions that the original game logic calls to implement modern cross-platform support. 
- SDL2, OpenGL, miniaudio
