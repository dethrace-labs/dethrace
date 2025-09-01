#include "flicplay.h"
#include "brender.h"
#include "displays.h"
#include "drmem.h"
#include "errors.h"
#include "globvars.h"
#include "graphics.h"
#include "harness/trace.h"
#include "input.h"
#include "loading.h"
#include "main.h"
#include "pd/sys.h"
#include "sound.h"
#include "utility.h"
#include <stdlib.h>
#include <string.h>

// GLOBAL: CARM95 0x00514fa0
int gPalette_allocate_count;

// GLOBAL: CARM95 0x00514fa4
int gPalette_fuck_prevention;

// GLOBAL: CARM95 0x00514fa8
int gDark_mode;

// GLOBAL: CARM95 0x00514fac
int gTransparency_on;

// GLOBAL: CARM95 0x00514fb0
int gPanel_flic_disable;

// GLOBAL: CARM95 0x00514fb4
int gPending_flic = -1;

// GLOBAL: CARM95 0x00514fb8
int gPlay_from_disk;

// GLOBAL: CARM95 0x00514fbc
int gTrans_enabled = 1;

// GLOBAL: CARM95 0x00514fc0
br_pixelmap* gPanel_buffer[2];

// GLOBAL: CARM95 0x00514fc8
tU32 gSound_time;

// GLOBAL: CARM95 0x00514fd0
tFlic_spec gMain_flic_list[372] = {
    { "MAINSTIL.FLI", 1, 0, 0, 0, 0, 25, NULL, 0u }, // only used by the demo (set to "xxxxxxxx.FLI" by the full version)
    { "DEMSTRT2.FLI", 1, 0, 0, 0, 0, 0, NULL, 0u },
    { "DEMSTRT1.FLI", 1, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWFLIC.FLI", 1, 0, 0, 0, 0, 0, NULL, 0u },
    { "GOTOFLIC.FLI", 1, 0, 0, 0, 0, 0, NULL, 0u },
    { "ENDFLIC.FLI", 1, 0, 0, 0, 0, 0, NULL, 0u },
    { "OVERFLIC.FLI", 1, 0, 0, 0, 0, 0, NULL, 0u },
    { "DEMO8.FLI", 1, 0, 0, 0, 0, 0, NULL, 0u },
    { "COMPLETE.FLI", 1, 0, 0, 0, 0, 0, NULL, 0u },
    { "DEMOEND.FLI", 1, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINSTIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "MAINCNFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINCNGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINCNIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINABFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINABGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINQTFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINQTGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINSVFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINSVGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINLDFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINLDGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINRCFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINRCGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINARFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINARGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINOPFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINOPGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAI2STIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAI2COME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAI2AWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINRCGY.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAINARGY.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SVVYSTIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SVVYAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "BGBUTTFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "BGBUTTGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SVVYOKIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CANBUTIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SAVECOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SAVEAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "SMLBUTFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SMLBUTGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SMLBUTFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SMLBUTGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SAVECAIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NRACCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NRACAWAY.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCARCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NSUMSTIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NSUMAWAY.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPTCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPTAWAY.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPTDEIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT00GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT00FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "LOADSTIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "LOADCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "LOADAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "LOADHIFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "LOADHIGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT01GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT01FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT02GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT02FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT03GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWGCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWGAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "NEWGHLFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWGHLGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWGDNAV.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWGDNGY.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWGDNFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWGDNGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWGDNIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWGCAIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWNSTIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWNSTFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWNSTGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CHCKBXFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CHCKBXGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CHCKBXON.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CHCKBXOF.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "RADBUTFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "RADBUTGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "RADBUTON.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWNCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NEWNAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "NETTCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NETTAWAY.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NETOCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NETOAWAY.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NETNCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NETNAWAY.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "RADBUTOF.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT03FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SKILCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SKILAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "NOPT04GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT04FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT05GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SKILL1FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SKILL1GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SKILLIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SKILL2FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SKILL2GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SKILL3FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SKILL3GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHOCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHOAWAY.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHOOPIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "ERRRSTIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT05FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT06GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT06FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT07GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "QUT1STIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "QUT2STIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "QUT3STIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT07FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT08GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "QUITOKIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT08FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT09GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT09FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT10GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "OPTNCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "OPTNAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "NOPT10FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT11GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "OPTNSNIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "OPTNCNIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "OPTNGRIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "OPTNMSIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NOPT11FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SNDOCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SNDOAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "DNEBUTIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SNDOOLFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SNDOOLGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPHCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPHAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "NCHO00GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHO01GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHO02GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHO03GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHO04GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHO05GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHO06GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CNTLCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CNTLAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "CNTLDNIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CNTLCAIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CNTLDFIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CNTLMRIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CNTLDNFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CNTLDNGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CNTLSTIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "OTHRCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "OTHRAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "NCHO00FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHO01FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHO02FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHO03FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHO04FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHO05FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NCHO06FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "STRTSTIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "STRTCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "STRTAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "CNTL00FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CNTL00GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "STRTCRIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "STRTPSIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "STRTSRIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "STRTCCIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NTSHSTIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NTSCSTIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NTSHSTIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NTSHENIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NTSCLVIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NTSHSTEN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "NTSXSTIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "VWSC2IN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "VWIN2OPP.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "VWOPP2SC.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "2BUTONFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "2BUTONGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "VWOPUPIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "VWOPDWFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "VWOPDWGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "VWOPDWIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CHRCCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CHRCAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CHCRCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "CHCRAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "GRPH00GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH01GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH02GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH03GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH04GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH05GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH06GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH07GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH08GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH09GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH10GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH11GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PARTCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PARTAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PARTARGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PARTARIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PARTPFIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PARTOFIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PARTEXIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PARTSPIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PARTARGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PARTPFGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PARTOFGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH00FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH01FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH02FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH03FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH04FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH05FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH06FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH07FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH08FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH09FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH10FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRPH11FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PSRMCOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PSRMAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "PSRMDIIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "RADBUTFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "RADBUTGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "RADBUTOF.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "RADBUTON.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRIDSTIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRIDAWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "GRIDLFFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRIDLFGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRIDLFIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRIDRTFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRIDRTGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "GRIDRTIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "DARECOME.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "DAREACIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "DARECHIN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SUM1STIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SUM1AWAY.FLI", 0, 1, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "SUM2STIL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "BGBUT8GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "BGBUT8FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "DNBUT8IN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "BKBUT8IN.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "BKBUTOFF.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "BKBUTON.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAI2QTFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAI2QTGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAI2LDFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAI2LDGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAI2N1FL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAI2N1GL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAI2NNFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAI2NNGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAI2OPFL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u },
    { "MAI2OPGL.FLI", 0, 0, 0, 0, 0, 0, NULL, 0u }
};

// GLOBAL: CARM95 0x00518420
int gFlic_bunch0[29] = {
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    24,
    25,
    35,
    26,
    27,
    36,
    28,
    29,
    130,
    131,
    132,
    42,
    43,
    135,
    45
};

// GLOBAL: CARM95 0x00518498
int gFlic_bunch1[31] = {
    140,
    141,
    42,
    43,
    144,
    145,
    146,
    147,
    45,
    150,
    151,
    42,
    43,
    154,
    155,
    156,
    160,
    161,
    42,
    43,
    154,
    170,
    171,
    176,
    177,
    172,
    180,
    181,
    42,
    43,
    154
};

// GLOBAL: CARM95 0x00518518
int gFlic_bunch2[8] = { 70, 71, 72, 73, 74, 56, 57, 59 };

// GLOBAL: CARM95 0x00518538
int gFlic_bunch3[13] = { 40, 41, 42, 43, 44, 45, 50, 51, 73, 74, 56, 57, 59 };

// GLOBAL: CARM95 0x00518570
int gFlic_bunch4[22] = {
    80,
    81,
    82,
    83,
    84,
    85,
    42,
    43,
    88,
    45,
    110,
    111,
    42,
    43,
    45,
    115,
    116,
    117,
    118,
    119,
    120,
    121
};

// GLOBAL: CARM95 0x005185c8
int gFlic_bunch5[5] = { 100, 101, 42, 43, 45 };

// GLOBAL: CARM95 0x005185e0
int gFlic_bunch6[51] = {
    190,
    191,
    192,
    42,
    43,
    195,
    200,
    201,
    210,
    212,
    213,
    220,
    221,
    222,
    220,
    221,
    225,
    230,
    231,
    42,
    43,
    154,
    45,
    220,
    221,
    222,
    220,
    221,
    225,
    250,
    251,
    42,
    43,
    254,
    255,
    256,
    154,
    42,
    43,
    260,
    220,
    221,
    222,
    220,
    221,
    225,
    280,
    281,
    42,
    43,
    284
};

// GLOBAL: CARM95 0x005186b0
int gFlic_bunch7[7] = { 130, 131, 132, 42, 43, 135, 45 };

// GLOBAL: CARM95 0x005186d0
int gFlic_bunch8[16] = { 290, 291, 292, 293, 294, 295, 296, 297, 42, 43, 154, 301, 42, 43, 304, 305 };

// GLOBAL: CARM95 0x00518710
tFlic_bunch gFlic_bunch[9] = {
    { COUNT_OF(gFlic_bunch0), gFlic_bunch0 },
    { COUNT_OF(gFlic_bunch1), gFlic_bunch1 },
    { COUNT_OF(gFlic_bunch2), gFlic_bunch2 },
    { COUNT_OF(gFlic_bunch3), gFlic_bunch3 },
    { COUNT_OF(gFlic_bunch4), gFlic_bunch4 },
    { COUNT_OF(gFlic_bunch5), gFlic_bunch5 },
    { COUNT_OF(gFlic_bunch6), gFlic_bunch6 },
    { COUNT_OF(gFlic_bunch7), gFlic_bunch7 },
    { COUNT_OF(gFlic_bunch8), gFlic_bunch8 },
};

// GLOBAL: CARM95 0x00518758
char gLast_flic_name[14];

// GLOBAL: CARM95 0x0053d060
tU32 gPanel_flic_data_length[2];

// GLOBAL: CARM95 0x0053d1b8
tU32 gLast_panel_frame_time[2];

// GLOBAL: CARM95 0x0053d0b8
tU8* gPanel_flic_data[2];

// GLOBAL: CARM95 0x0053d0c0
int gPanel_flic_top[2];

// GLOBAL: CARM95 0x0053d0d8
tFlic_descriptor gPanel_flic[2];

// GLOBAL: CARM95 0x0053d0b0
int gPanel_flic_left[2];

// GLOBAL: CARM95 0x0053d0cc
int gPending_pending_flic = -1;

// GLOBAL: CARM95 0x0053d1c0
int gSound_ID;

// GLOBAL: CARM95 0x0053d06c
int gTranslation_count;

// GLOBAL: CARM95 0x0053d070
tDR_font* gTrans_fonts[15];

// GLOBAL: CARM95 0x0053d0d0
tTranslation_record* gTranslations;

// GLOBAL: CARM95 0x0053d0c8
br_pixelmap* gPalette;

// GLOBAL: CARM95 0x0053d068
void* gPalette_pixels;

// GLOBAL: CARM95 0x0053d0ac
tFlic_descriptor* gFirst_flic;

// Use this function to avoid unaligned memory access.
// Added by DethRace
tU16 mem_read_u16(void* memory) {
    tU16 u16;

    memcpy(&u16, memory, sizeof(tU16));
    return u16;
}

// Use this function to avoid unaligned memory access
// Added by DethRace
void mem_write_u16(void* memory, tU16 u16) {

    memcpy(memory, &u16, sizeof(tU16));
}

// IDA: void __cdecl EnableTranslationText()
// FUNCTION: CARM95 0x00495990
void EnableTranslationText(void) {

    gTrans_enabled = 1;
}

// IDA: void __cdecl DisableTranslationText()
// FUNCTION: CARM95 0x004959a5
void DisableTranslationText(void) {

    gTrans_enabled = 0;
}

// IDA: void __usercall SetFlicSound(int pSound_ID@<EAX>, tU32 pSound_time@<EDX>)
// FUNCTION: CARM95 0x004959ba
void SetFlicSound(int pSound_ID, tU32 pSound_time) {

    gSound_time = pSound_time;
    gSound_ID = pSound_ID;
}

// IDA: int __cdecl TranslationMode()
// FUNCTION: CARM95 0x004959d5
int TranslationMode(void) {

    return gTranslation_count;
}

// IDA: void __cdecl DontLetFlicFuckWithPalettes()
// FUNCTION: CARM95 0x004959ea
void DontLetFlicFuckWithPalettes(void) {

    gPalette_fuck_prevention = 1;
}

// IDA: void __cdecl LetFlicFuckWithPalettes()
// FUNCTION: CARM95 0x004959ff
void LetFlicFuckWithPalettes(void) {

    gPalette_fuck_prevention = 0;
}

// IDA: void __cdecl PlayFlicsInDarkness()
// FUNCTION: CARM95 0x00495a14
void PlayFlicsInDarkness(void) {

    gDark_mode = 1;
}

// IDA: void __cdecl ReilluminateFlics()
// FUNCTION: CARM95 0x00495a29
void ReilluminateFlics(void) {

    gDark_mode = 0;
    FadePaletteUp();
}

// IDA: void __cdecl TurnFlicTransparencyOn()
// FUNCTION: CARM95 0x00495a43
void TurnFlicTransparencyOn(void) {

    gTransparency_on = 1;
}

// IDA: void __cdecl TurnFlicTransparencyOff()
// FUNCTION: CARM95 0x00495a58
void TurnFlicTransparencyOff(void) {
    gTransparency_on = 0;
}

// IDA: void __cdecl PlayFlicsFromDisk()
// FUNCTION: CARM95 0x00495a6d
void PlayFlicsFromDisk(void) {
    gPlay_from_disk = 1;
}

// IDA: void __cdecl PlayFlicsFromMemory()
// FUNCTION: CARM95 0x00495a82
void PlayFlicsFromMemory(void) {
    gPlay_from_disk = 0;
}

// IDA: int __cdecl FlicsPlayedFromDisk()
// FUNCTION: CARM95 0x00495a97
int FlicsPlayedFromDisk(void) {
    return gPlay_from_disk;
}

// IDA: void __cdecl TurnOffPanelFlics()
// FUNCTION: CARM95 0x00495aac
void TurnOffPanelFlics(void) {
    gPanel_flic_disable = 1;
}

// IDA: void __cdecl TurnOnPanelFlics()
// FUNCTION: CARM95 0x00495ac1
void TurnOnPanelFlics(void) {
    gPanel_flic_disable = 0;
}

// IDA: int __usercall GetPanelFlicFrameIndex@<EAX>(int pIndex@<EAX>)
// FUNCTION: CARM95 0x00495ad6
int GetPanelFlicFrameIndex(int pIndex) {

    return gPanel_flic[pIndex].current_frame;
}

// IDA: void __cdecl FlicPaletteAllocate()
// FUNCTION: CARM95 0x00495af9
void FlicPaletteAllocate(void) {

    gPalette_pixels = BrMemAllocate(0x400u, kMem_flic_pal);
    gPalette = DRPixelmapAllocate(BR_PMT_RGBX_888, 1, 256, gPalette_pixels, 0);
}

// IDA: void __usercall AssertFlicPixelmap(tFlic_descriptor_ptr pFlic_info@<EAX>, br_pixelmap *pDest_pixelmap@<EDX>)
// FUNCTION: CARM95 0x00495b39
void AssertFlicPixelmap(tFlic_descriptor_ptr pFlic_info, br_pixelmap* pDest_pixelmap) {

    if (pDest_pixelmap != NULL) {
        pFlic_info->first_pixel = (tU8*)pDest_pixelmap->pixels
            + pFlic_info->x_offset
            + pFlic_info->y_offset * pDest_pixelmap->row_bytes;
    }
    pFlic_info->the_pixelmap = pDest_pixelmap;
}

// IDA: int __usercall StartFlic@<EAX>(char *pFile_name@<EAX>, int pIndex@<EDX>, tFlic_descriptor_ptr pFlic_info@<EBX>, tU32 pSize@<ECX>, tS8 *pData_ptr, br_pixelmap *pDest_pixelmap, int pX_offset, int pY_offset, int pFrame_rate)
// FUNCTION: CARM95 0x00495b77
int StartFlic(char* pFile_name, int pIndex, tFlic_descriptor_ptr pFlic_info, tU32 pSize, tS8* pData_ptr, br_pixelmap* pDest_pixelmap, int pX_offset, int pY_offset, int pFrame_rate) {
    tU16 claimed_speed;
    tU16 magic_number;
    tPath_name the_path;
    int total_size;

    if (gPlay_from_disk) {
        PathCat(the_path, gApplication_path, "ANIM");
        PathCat(the_path, the_path, pFile_name);
        pFlic_info->f = DRfopen(the_path, "rb");

        if (pFlic_info->f == NULL) {
            FatalError(kFatalError_LoadFlicFile_S, pFile_name);
        }
        total_size = GetFileLength(pFlic_info->f);
        if (total_size >= 75000) {
            pFlic_info->bytes_in_buffer = 75000;
        } else {
            pFlic_info->bytes_in_buffer = total_size;
        }
        if (pFlic_info->data_start == NULL) {
            pFlic_info->data_start = BrMemAllocate(pFlic_info->bytes_in_buffer, kMem_flic_data);
        }

        pFlic_info->data = pFlic_info->data_start;
        strcpy(gLast_flic_name, pFile_name);
        fread(pFlic_info->data_start, 1, pFlic_info->bytes_in_buffer, pFlic_info->f);
        pFlic_info->bytes_still_to_be_read = total_size - pFlic_info->bytes_in_buffer;
    } else {
        pFlic_info->f = NULL;
        pFlic_info->data = (char*)pData_ptr;
        // TODO: remove this - we added this line because of the padding hack in PlayNextFlicFrame2
        pFlic_info->data_start = (char*)pData_ptr;
    }
    pFlic_info->bytes_remaining = MemReadU32(&pFlic_info->data);
    magic_number = MemReadU16(&pFlic_info->data);
    if (magic_number == 0xaf11) {
        pFlic_info->new_format = 0;
    } else if (magic_number == 0xaf12) {
        pFlic_info->new_format = 1;
    } else {
        return -1;
    }

    pFlic_info->frames_left = MemReadU16(&pFlic_info->data);
    pFlic_info->current_frame = 0;
    pFlic_info->width = MemReadU16(&pFlic_info->data);
    pFlic_info->height = MemReadU16(&pFlic_info->data);
    if (MemReadU16(&pFlic_info->data) != 8) {
        FatalError(kFatalError_FlicFileNot8Bits_S, gLast_flic_name);
    }
    MemSkipBytes(&pFlic_info->data, 2);
    claimed_speed = MemReadU16(&pFlic_info->data);
    MemSkipBytes(&pFlic_info->data, 0x6e);
    pFlic_info->the_pixelmap = pDest_pixelmap;

    if (pX_offset != -1) {
        pFlic_info->x_offset = pX_offset;
    } else if (pDest_pixelmap != NULL) {
        pFlic_info->x_offset = (pDest_pixelmap->width - pFlic_info->width) / 2;
    } else {
        pFlic_info->x_offset = 0;
    }
    if (pY_offset != -1) {
        pFlic_info->y_offset = pY_offset;
    } else if (pDest_pixelmap != NULL) {
        pFlic_info->y_offset = (pDest_pixelmap->height - pFlic_info->height) / 2;
    } else {
        pFlic_info->y_offset = 0;
    }

    if (pFrame_rate != 0) {
        pFlic_info->frame_period = 1000 / pFrame_rate;
    } else {
        if (claimed_speed == 0) {
            FatalError(kFatalError_FlicFileNoFrameRate_S, gLast_flic_name);
        }
        if (pFlic_info->new_format) {
            pFlic_info->frame_period = claimed_speed;
        } else {
            pFlic_info->frame_period = 14 * claimed_speed;
        }
    }
    pFlic_info->the_index = pIndex;
    AssertFlicPixelmap(pFlic_info, pDest_pixelmap);
    return 0;
}

// IDA: void __cdecl FreeFlicPaletteAllocate()
// FUNCTION: CARM95 0x00495f27
void FreeFlicPaletteAllocate(void) {

    if (gPalette_allocate_count == 0) {
        FatalError(kFatalError_FlicPaletteDisposeBeforeAllocation);
    }
    gPalette_allocate_count--;
    if (gPalette_allocate_count == 0) {
        BrMemFree(gPalette_pixels);
        BrPixelmapFree(gPalette);
    }
}

// IDA: int __usercall EndFlic@<EAX>(tFlic_descriptor_ptr pFlic_info@<EAX>)
// FUNCTION: CARM95 0x00495f71
int EndFlic(tFlic_descriptor_ptr pFlic_info) {

    if (pFlic_info->f != NULL) {
        BrMemFree(pFlic_info->data_start);
        pFlic_info->data_start = NULL;
        fclose(pFlic_info->f);
        pFlic_info->f = NULL;
    }
    if (pFlic_info->data != NULL) {
        pFlic_info->data = NULL;
    }
    return 0;
}

// IDA: void __usercall DoColourMap(tFlic_descriptor_ptr pFlic_info@<EAX>, tU32 chunk_length@<EDX>)
// FUNCTION: CARM95 0x0049639a
void DoColourMap(tFlic_descriptor_ptr pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int packet_count;
    int skip_count;
    int change_count;
    int current_colour = 0;
    tU8* palette_pixels;
    tU8 red;
    tU8 green;
    tU8 blue;

    palette_pixels = gPalette_pixels;

    packet_count = MemReadU16(&pFlic_info->data);
    for (i = 0; i < packet_count; i++) {
        skip_count = MemReadU8(&pFlic_info->data);
        change_count = MemReadU8(&pFlic_info->data);
        if (!change_count) {
            change_count = 256;
        }
        palette_pixels += skip_count * sizeof(br_int_32);
        current_colour += skip_count;
        for (j = 0; j < change_count; j++) {
            red = MemReadU8(&pFlic_info->data);
            blue = MemReadU8(&pFlic_info->data);
            green = MemReadU8(&pFlic_info->data);
            // argb
#if BR_ENDIAN_BIG
            palette_pixels[3] = green * 4;
            palette_pixels[2] = blue * 4;
            palette_pixels[1] = red * 4;
            palette_pixels[0] = 0;
#else
            palette_pixels[0] = green * 4;
            palette_pixels[1] = blue * 4;
            palette_pixels[2] = red * 4;
            palette_pixels[3] = 0;
#endif
            palette_pixels += 4;
        }
        if (!gPalette_fuck_prevention) {
            DRSetPaletteEntries(gPalette, current_colour, change_count);
        }
    }
}

// IDA: void __usercall DoDifferenceX(tFlic_descriptor *pFlic_info@<EAX>, tU32 chunk_length@<EDX>)
// FUNCTION: CARM95 0x004964e3
void DoDifferenceX(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int k;
    int first_line;
    int line_count;
    int number_of_packets;
    int skip_count;
    int size_count;
    tU8* pixel_ptr;
    tU8* line_pixel_ptr;
    tU8 the_byte;
    tU32 the_row_bytes;

    first_line = MemReadU16(&pFlic_info->data);
    line_count = MemReadU16(&pFlic_info->data);
    the_row_bytes = pFlic_info->the_pixelmap->row_bytes;
    line_pixel_ptr = pFlic_info->first_pixel + first_line * the_row_bytes;
    for (i = 0; i < line_count; i++) {
        pixel_ptr = line_pixel_ptr;
        number_of_packets = MemReadU8(&pFlic_info->data);
        for (j = 0; j < number_of_packets; j++) {
            skip_count = MemReadU8(&pFlic_info->data);
            size_count = MemReadS8(&pFlic_info->data);
            pixel_ptr += skip_count;
            if (size_count >= 0) {
                for (k = 0; k < size_count; k++) {
                    *pixel_ptr = *pFlic_info->data;
                    pFlic_info->data++;
                    pixel_ptr++;
                }
            } else {
                the_byte = *pFlic_info->data;
                pFlic_info->data++;
                for (k = 0; k < -size_count; k++) {
                    *pixel_ptr = the_byte;
                    pixel_ptr++;
                }
            }
        }
        line_pixel_ptr += the_row_bytes;
    }
}

// IDA: void __usercall DoDifferenceTrans(tFlic_descriptor *pFlic_info@<EAX>, tU32 chunk_length@<EDX>)
// FUNCTION: CARM95 0x0049663f
void DoDifferenceTrans(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int k;
    int first_line;
    int line_count;
    int number_of_packets;
    int skip_count;
    int size_count;
    tU8* pixel_ptr;
    tU8* line_pixel_ptr;
    tU8 the_byte;
    tU32 the_row_bytes;

    first_line = MemReadU16(&pFlic_info->data);
    line_count = MemReadU16(&pFlic_info->data);
    the_row_bytes = pFlic_info->the_pixelmap->row_bytes;
    line_pixel_ptr = pFlic_info->first_pixel + first_line * the_row_bytes;
    for (i = 0; i < line_count; i++) {
        pixel_ptr = line_pixel_ptr;
        number_of_packets = MemReadU8(&pFlic_info->data);
        for (j = 0; j < number_of_packets; j++) {
            skip_count = MemReadU8(&pFlic_info->data);
            size_count = MemReadS8(&pFlic_info->data);
            pixel_ptr += skip_count;
            if (size_count >= 0) {
                for (k = 0; k < size_count; k++) {
                    the_byte = *pFlic_info->data;
                    pFlic_info->data++;
                    if (the_byte != '\0') {
                        *pixel_ptr = the_byte;
                    }
                    pixel_ptr++;
                }
            } else {
                the_byte = *pFlic_info->data;
                pFlic_info->data++;
                if (the_byte == '\0') {
                    pixel_ptr += size_count;
                } else {
                    for (k = 0; k < -size_count; k++) {
                        *pixel_ptr = the_byte;
                        pixel_ptr++;
                    }
                }
            }
        }
        line_pixel_ptr += the_row_bytes;
    }
}

// IDA: void __usercall DoColour256(tFlic_descriptor *pFlic_info@<EAX>, tU32 chunk_length@<EDX>)
// FUNCTION: CARM95 0x004967ce
void DoColour256(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int packet_count;
    int skip_count;
    int change_count;
    int current_colour;
    tU8* palette_pixels;
    tU8 red;
    tU8 green;
    tU8 blue;

    current_colour = 0;
    palette_pixels = gPalette_pixels;

    packet_count = MemReadU16(&pFlic_info->data);
    for (i = 0; i < packet_count; i++) {
        skip_count = MemReadU8(&pFlic_info->data);
        change_count = MemReadU8(&pFlic_info->data);
        if (!change_count) {
            change_count = 256;
        }
        palette_pixels += skip_count * sizeof(br_int_32);
        current_colour += skip_count;
        for (j = 0; j < change_count; j++) {
            red = MemReadU8(&pFlic_info->data);
            blue = MemReadU8(&pFlic_info->data);
            green = MemReadU8(&pFlic_info->data);
            // argb
#if BR_ENDIAN_BIG
            palette_pixels[3] = green;
            palette_pixels[2] = blue;
            palette_pixels[1] = red;
            palette_pixels[0] = 0;
#else
            palette_pixels[0] = green;
            palette_pixels[1] = blue;
            palette_pixels[2] = red;
            palette_pixels[3] = 0;
#endif
            palette_pixels += 4;
            // LOG_DEBUG("color %d", current_colour);
        }
        if (!gPalette_fuck_prevention) {
            DRSetPaletteEntries(gPalette, current_colour, change_count);
        }
    }
}

// IDA: void __usercall DoDeltaTrans(tFlic_descriptor *pFlic_info@<EAX>, tU32 chunk_length@<EDX>)
// FUNCTION: CARM95 0x00496902
void DoDeltaTrans(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int k;
    int line_count;
    int number_of_packets;
    int skip_count;
    int size_count;
    tU8* pixel_ptr;
    tU8 the_byte;
    tU8 the_byte2;
    tU32 the_row_bytes;
    tU16* line_pixel_ptr;
    tU16 the_word;

    line_count = MemReadU16(&pFlic_info->data);
    the_row_bytes = pFlic_info->the_pixelmap->row_bytes;
    pixel_ptr = pFlic_info->first_pixel;

    for (i = 0; i < line_count;) {
        number_of_packets = MemReadS16(&pFlic_info->data);
        line_pixel_ptr = (tU16*)pixel_ptr;

        if (number_of_packets < 0) {
            pixel_ptr = pixel_ptr + the_row_bytes * -number_of_packets;
        } else {
            for (j = 0; j < number_of_packets; j++) {
                skip_count = MemReadU8(&pFlic_info->data);
                size_count = MemReadS8(&pFlic_info->data);
                line_pixel_ptr += skip_count / 2;
                if (size_count < 0) {
                    the_byte = *pFlic_info->data++;
                    the_byte2 = *pFlic_info->data++;

                    if (the_byte && the_byte2) {
                        the_word = *((tU16*)pFlic_info->data - 1);
                        for (k = 0; k < -size_count; k++) {
                            mem_write_u16(line_pixel_ptr, the_word);
                            line_pixel_ptr++;
                        }
                    } else {
                        for (k = 0; k < -size_count; k++) {
                            if (the_byte) {
                                *(tU8*)line_pixel_ptr = the_byte;
                            }
                            line_pixel_ptr = (tU16*)((tU8*)line_pixel_ptr + 1);
                            if (the_byte2) {
                                *(tU8*)line_pixel_ptr = the_byte2;
                            }
                            line_pixel_ptr = (tU16*)((tU8*)line_pixel_ptr + 1);
                        }
                    }
                } else {
                    for (k = 0; k < size_count; k++) {
                        the_word = *(tU16*)pFlic_info->data;
                        pFlic_info->data += 2;
                        if (the_word) {
                            mem_write_u16(line_pixel_ptr, the_word);
                        }
                        line_pixel_ptr++;
                    }
                }
            }
            pixel_ptr = pixel_ptr + the_row_bytes;
            i++;
        }
    }
}

// IDA: void __usercall DoDeltaX(tFlic_descriptor *pFlic_info@<EAX>, tU32 chunk_length@<EDX>)
// FUNCTION: CARM95 0x00496b49
void DoDeltaX(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int k;
    int line_count;
    int number_of_packets;
    int skip_count;
    int size_count;
    tU8* pixel_ptr;
    tU32 the_row_bytes;
    tU16* line_pixel_ptr;
    tU16 the_word;

    line_count = MemReadU16(&pFlic_info->data);
    the_row_bytes = pFlic_info->the_pixelmap->row_bytes;
    pixel_ptr = pFlic_info->first_pixel;

    for (i = 0; i < line_count;) {
        number_of_packets = MemReadS16(&pFlic_info->data);
        line_pixel_ptr = (tU16*)pixel_ptr;

        if (number_of_packets < 0) {
            pixel_ptr = pixel_ptr + the_row_bytes * -number_of_packets;
        } else {
            for (j = 0; j < number_of_packets; j++) {
                skip_count = MemReadU8(&pFlic_info->data);
                size_count = MemReadS8(&pFlic_info->data);
                line_pixel_ptr += skip_count / 2;
                if (size_count < 0) {
                    the_word = *(tU16*)pFlic_info->data;
                    pFlic_info->data += 2;
                    for (k = 0; k < -size_count; k++) {
                        *line_pixel_ptr = the_word;
                        line_pixel_ptr++;
                    }
                } else {
                    for (k = 0; k < size_count; k++) {
                        the_word = *(tU16*)pFlic_info->data;
                        pFlic_info->data += 2;
                        *line_pixel_ptr = the_word;
                        line_pixel_ptr++;
                    }
                }
            }
            pixel_ptr = pixel_ptr + the_row_bytes;
            i++;
        }
    }
}

// IDA: void __usercall DoBlack(tFlic_descriptor *pFlic_info@<EAX>, tU32 chunk_length@<EDX>)
// FUNCTION: CARM95 0x00496cb0
void DoBlack(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int the_width;
    tU8* pixel_ptr;
    tU32 the_row_bytes;
    tU32* line_pixel_ptr;

    pixel_ptr = pFlic_info->first_pixel;
    the_row_bytes = pFlic_info->the_pixelmap->row_bytes;
    the_width = pFlic_info->width;
    for (i = 0; i < pFlic_info->height; i++) {
        line_pixel_ptr = (tU32*)pixel_ptr;
        for (j = 0; j < the_width / sizeof(tU32); j++) {
            *line_pixel_ptr = 0;
            line_pixel_ptr++;
        }
        pixel_ptr += the_row_bytes;
    }
}

// IDA: void __usercall DoRunLengthX(tFlic_descriptor *pFlic_info@<EAX>, tU32 chunk_length@<EDX>)
// FUNCTION: CARM95 0x00496d3c
void DoRunLengthX(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int k;
    int number_of_packets;
    int size_count;
    tU8* pixel_ptr;
    tU8* line_pixel_ptr;
    tU8 the_byte;
    tU32 the_row_bytes;

    the_row_bytes = pFlic_info->the_pixelmap->row_bytes;
    pixel_ptr = pFlic_info->first_pixel;

    for (i = 0; i < pFlic_info->height; i++) {
        line_pixel_ptr = pixel_ptr;
        number_of_packets = MemReadU8(&pFlic_info->data);
        for (j = 0; j < number_of_packets; j++) {
            size_count = MemReadS8(&pFlic_info->data);
            if (size_count >= 0) {
                the_byte = MemReadU8(&pFlic_info->data);
                for (k = 0; k < size_count; k++) {
                    *line_pixel_ptr = the_byte;
                    line_pixel_ptr++;
                }
            } else {
                for (k = 0; k < -size_count; k++) {
                    the_byte = MemReadU8(&pFlic_info->data);
                    *line_pixel_ptr = the_byte;
                    line_pixel_ptr++;
                }
            }
        }
        pixel_ptr += the_row_bytes;
    }
}

// IDA: void __usercall DoRunLengthTrans(tFlic_descriptor *pFlic_info@<EAX>, tU32 chunk_length@<EDX>)
// FUNCTION: CARM95 0x00496e53
void DoRunLengthTrans(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int k;
    int number_of_packets;
    int size_count;
    tU8* pixel_ptr;
    tU8* line_pixel_ptr;
    tU8 the_byte;
    tU32 the_row_bytes;

    the_row_bytes = pFlic_info->the_pixelmap->row_bytes;
    pixel_ptr = pFlic_info->first_pixel;

    for (i = 0; i < pFlic_info->height; i++) {
        line_pixel_ptr = pixel_ptr;
        number_of_packets = MemReadU8(&pFlic_info->data);
        for (j = 0; j < number_of_packets; j++) {
            size_count = MemReadS8(&pFlic_info->data);
            if (size_count >= 0) {
                the_byte = MemReadU8(&pFlic_info->data);

                for (k = 0; k < size_count; k++) {
                    if (the_byte) {
                        *line_pixel_ptr = the_byte;
                    }
                    line_pixel_ptr++;
                }
            } else {
                for (k = 0; k < -size_count; k++) {
                    the_byte = MemReadU8(&pFlic_info->data);
                    if (the_byte) {
                        *line_pixel_ptr = the_byte;
                    }
                    line_pixel_ptr++;
                }
            }
        }
        pixel_ptr += the_row_bytes;
    }
}

// IDA: void __usercall DoUncompressed(tFlic_descriptor *pFlic_info@<EAX>, tU32 chunk_length@<EDX>)
// FUNCTION: CARM95 0x00496f9d
void DoUncompressed(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int the_width;
    tU8* pixel_ptr;
    tU32 the_row_bytes;
    tU32* line_pixel_ptr;

    pixel_ptr = pFlic_info->first_pixel;
    the_row_bytes = pFlic_info->the_pixelmap->row_bytes;
    the_width = pFlic_info->width;
    for (i = 0; i < pFlic_info->height; i++) {
        line_pixel_ptr = (tU32*)pixel_ptr;
        for (j = 0; j < the_width / 4; j++) {
            *line_pixel_ptr = MemReadU32(&pFlic_info->data);
            line_pixel_ptr++;
        }
        pixel_ptr += the_row_bytes;
    }
}

// IDA: void __usercall DoUncompressedTrans(tFlic_descriptor *pFlic_info@<EAX>, tU32 chunk_length@<EDX>)
// FUNCTION: CARM95 0x00497031
void DoUncompressedTrans(tFlic_descriptor* pFlic_info, tU32 chunk_length) {
    int i;
    int j;
    int the_width;
    tU8* pixel_ptr;
    tU8* line_pixel_ptr;
    tU8 the_byte;
    tU32 the_row_bytes;

    pixel_ptr = pFlic_info->first_pixel;
    the_row_bytes = pFlic_info->the_pixelmap->row_bytes;
    the_width = pFlic_info->width;
    for (i = 0; i < pFlic_info->height; i++) {
        line_pixel_ptr = pixel_ptr;
        for (j = 0; j < the_width; j++) {
#if defined(DETHRACE_FIX_BUGS)
            the_byte = MemReadU8(&pFlic_info->data);
#else
            the_byte = MemReadU32(&pFlic_info->data);
#endif
            if (the_byte != '\0') {
                *line_pixel_ptr = the_byte;
            }
            line_pixel_ptr++;
        }
        pixel_ptr += the_row_bytes;
    }
}

// IDA: void __usercall DoMini(tFlic_descriptor *pFlic_info@<EAX>, tU32 chunk_length@<EDX>)
// FUNCTION: CARM95 0x004970dc
void DoMini(tFlic_descriptor* pFlic_info, tU32 chunk_length) {

    MemSkipBytes(&pFlic_info->data, chunk_length - 6);
}

// IDA: void __usercall DrawTranslations(tFlic_descriptor *pFlic_info@<EAX>, int pLast_frame@<EDX>)
// FUNCTION: CARM95 0x004970fa
void DrawTranslations(tFlic_descriptor* pFlic_info, int pLast_frame) {
    tTranslation_record* trans;
    int i;
    int x;
    int width;
    int right_edge;

    for (i = 0; i < gTranslation_count; i++) {
        trans = &gTranslations[i];
        if (trans->flic_index == pFlic_info->the_index && (trans->every_frame || pLast_frame)) {
            width = DRTextWidth(gTrans_fonts[trans->font_index], trans->text);
            switch (trans->justification) {
            case eJust_left:
                x = trans->x;
                right_edge = x + width;
                break;
            case eJust_right:
                x = trans->x - width;
                right_edge = x;
                break;
            case eJust_centre:
                x = trans->x - width / 2;
                right_edge = x + width / 2;
                break;
            default:
                TELL_ME_IF_WE_PASS_THIS_WAY();
            }
            TransDRPixelmapText(
                pFlic_info->the_pixelmap,
                x + (trans->global ? 0 : pFlic_info->x_offset),
                trans->y + (trans->global ? 0 : pFlic_info->y_offset),
                gTrans_fonts[trans->font_index],
                trans->text,
                right_edge);
        }
    }
}

// IDA: int __usercall PlayNextFlicFrame2@<EAX>(tFlic_descriptor *pFlic_info@<EAX>, int pPanel_flic@<EDX>)
// FUNCTION: CARM95 0x00495ff5
int PlayNextFlicFrame2(tFlic_descriptor* pFlic_info, int pPanel_flic) {
    tU32 frame_length;
    tU32 chunk_length;
    int chunk_count;
    int chunk_counter;
    int chunk_type;
    int magic_bytes;
    int last_frame;
    int data_knocked_off;
    int read_amount;

    // LOG_DEBUG("%d (%p), frames left: %d offset: %d", pFlic_info->the_index, pFlic_info, pFlic_info->frames_left, (pFlic_info->data - pFlic_info->data_start) + 4);
    PossibleService();
    frame_length = MemReadU32(&pFlic_info->data);
    magic_bytes = MemReadU16(&pFlic_info->data);
    chunk_count = MemReadU16(&pFlic_info->data);

    MemSkipBytes(&pFlic_info->data, 8);
    if (magic_bytes == 0xf1fa) {
        for (chunk_counter = 0; chunk_counter < chunk_count; chunk_counter++) {
            chunk_length = MemReadU32(&pFlic_info->data);
            chunk_type = MemReadU16(&pFlic_info->data);
            switch (chunk_type) {
            case 4:
                DoColour256(pFlic_info, chunk_length);
                break;
            case 7:
                if (gTransparency_on) {
                    DoDeltaTrans(pFlic_info, chunk_length);
                } else {
                    DoDeltaX(pFlic_info, chunk_length);
                }
                break;
            case 11:
                DoColourMap(pFlic_info, chunk_length);
                break;
            case 12:
                if (gTransparency_on) {
                    DoDifferenceTrans(pFlic_info, chunk_length);
                } else {
                    DoDifferenceX(pFlic_info, chunk_length);
                }
                break;
            case 13:
                DoBlack(pFlic_info, chunk_length);
                break;
            case 15:
                if (gTransparency_on) {
                    DoRunLengthTrans(pFlic_info, chunk_length);
                } else {
                    DoRunLengthX(pFlic_info, chunk_length);
                }
                break;
            case 16:
                if (gTransparency_on) {
                    DoUncompressedTrans(pFlic_info, chunk_length);
                } else {
                    DoUncompressed(pFlic_info, chunk_length);
                }
                break;
            case 18:
                DoMini(pFlic_info, chunk_length);
                break;
            default:
                LOG_WARN("unrecognized chunk type");
                MemSkipBytes(&pFlic_info->data, chunk_length - 6);
                break;
            }
            // Align on even byte
            pFlic_info->data = (char*)((uintptr_t)(pFlic_info->data + 1) & (~(uintptr_t)1));
        }
    } else {
        LOG_WARN("not frame header");
        MemSkipBytes(&pFlic_info->data, frame_length - 16);
        pFlic_info->frames_left++;
        pFlic_info->current_frame--;
    }
    pFlic_info->current_frame++;
    pFlic_info->frames_left--;
    if (gTrans_enabled && gTranslation_count != 0 && !pPanel_flic) {
        DrawTranslations(pFlic_info, pFlic_info->frames_left == 0);
    }
    if (pFlic_info->f != NULL && pFlic_info->bytes_still_to_be_read) {
        data_knocked_off = pFlic_info->data - pFlic_info->data_start;
        memmove(pFlic_info->data_start, pFlic_info->data, pFlic_info->bytes_in_buffer - data_knocked_off);
        pFlic_info->data = pFlic_info->data_start;
        pFlic_info->bytes_in_buffer -= data_knocked_off;

        if (pFlic_info->bytes_still_to_be_read > data_knocked_off) {
            read_amount = data_knocked_off;
        } else {
            read_amount = pFlic_info->bytes_still_to_be_read;
        }
        if (read_amount != 0) {
            fread(&pFlic_info->data_start[pFlic_info->bytes_in_buffer], 1, read_amount, pFlic_info->f);
        }
        pFlic_info->bytes_in_buffer += read_amount;
        pFlic_info->bytes_still_to_be_read -= read_amount;
    }
    return pFlic_info->frames_left == 0;
}

// IDA: int __usercall PlayNextFlicFrame@<EAX>(tFlic_descriptor *pFlic_info@<EAX>)
// FUNCTION: CARM95 0x00495fd7
int PlayNextFlicFrame(tFlic_descriptor* pFlic_info) {

    return PlayNextFlicFrame2(pFlic_info, 0);
}

// IDA: int __usercall PlayFlic@<EAX>(int pIndex@<EAX>, tU32 pSize@<EDX>, tS8 *pData_ptr@<EBX>, br_pixelmap *pDest_pixelmap@<ECX>, int pX_offset, int pY_offset, void (*DoPerFrame)(), int pInterruptable, int pFrame_rate)
// FUNCTION: CARM95 0x00497278
int PlayFlic(int pIndex, tU32 pSize, tS8* pData_ptr, br_pixelmap* pDest_pixelmap, int pX_offset, int pY_offset, void (*DoPerFrame)(void), int pInterruptable, int pFrame_rate) {
    int finished_playing;
    tFlic_descriptor the_flic;
    tU32 last_frame;
    tU32 new_time;
    tU32 frame_period;

    finished_playing = 0;
    the_flic.data_start = NULL;
    if (StartFlic(gMain_flic_list[pIndex].file_name, pIndex, &the_flic, pSize, pData_ptr, pDest_pixelmap, pX_offset, pY_offset, pFrame_rate)) {
        LOG_WARN("startflic returned error");
        return -1;
    }

    last_frame = 0;
    while ((!pInterruptable || !AnyKeyDown()) && !finished_playing) {
        new_time = PDGetTotalTime();
        frame_period = new_time - last_frame;

        if (gSound_time != 0 && new_time >= gSound_time) {
            DRS3StartSound(gEffects_outlet, gSound_ID);
            gSound_time = 0;
        }
        if (frame_period >= the_flic.frame_period) {
            last_frame = new_time;
            finished_playing = PlayNextFlicFrame(&the_flic);
            DoPerFrame();
            if (!gDark_mode) {
                EnsurePaletteUp();
            }
            ServiceGame();
        }
    }
    ServiceGame();
    EndFlic(&the_flic);
    return 0;
}

// IDA: void __cdecl SwapScreen()
// FUNCTION: CARM95 0x00497444
void SwapScreen(void) {
    PDScreenBufferSwap(0);
}

// IDA: void __usercall ShowFlic(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004973a3
void ShowFlic(int pIndex) {
    do {
        PlayFlic(
            pIndex,
            gMain_flic_list[pIndex].the_size,
            gMain_flic_list[pIndex].data_ptr,
            gBack_screen,
            gMain_flic_list[pIndex].x_offset,
            gMain_flic_list[pIndex].y_offset,
            SwapScreen,
            gMain_flic_list[pIndex].interruptable,
            gMain_flic_list[pIndex].frame_rate);
    } while (gMain_flic_list[pIndex].repeat && !AnyKeyDown());
    gLast_flic_name[0] = '\0'; // byte_10344C;
}

// IDA: void __cdecl InitFlics()
// FUNCTION: CARM95 0x00497459
void InitFlics(void) {
    int i;

    for (i = 0; i < COUNT_OF(gMain_flic_list); i++) {
        gMain_flic_list[i].data_ptr = NULL;
    }
}

// IDA: int __usercall LoadFlic@<EAX>(int pIndex@<EAX>)
// FUNCTION: CARM95 0x00497499
int LoadFlic(int pIndex) {
    tPath_name the_path;
    FILE* f;
    char* the_buffer;

    if (pIndex < 0) {
        return 0;
    }
    if (gMain_flic_list[pIndex].data_ptr != NULL) {
        MAMSLock((void**)&gMain_flic_list[pIndex].data_ptr);
        return 1;
    }
    if (gPlay_from_disk) {
        gMain_flic_list[pIndex].data_ptr = NULL;
        return 1;
    }
    PossibleService();
    PathCat(the_path, gApplication_path, "ANIM");
    PathCat(the_path, the_path, gMain_flic_list[pIndex].file_name);
    f = DRfopen(the_path, "rb");

    if (f == NULL) {
        FatalError(kFatalError_LoadFlicFile_S, gMain_flic_list[pIndex].file_name);
    }

    gMain_flic_list[pIndex].the_size = GetFileLength(f);
    gMain_flic_list[pIndex].data_ptr = BrMemAllocate(gMain_flic_list[pIndex].the_size, 0x90u);

    if (gMain_flic_list[pIndex].data_ptr == NULL) {
        if (AllocationErrorsAreFatal()) {
            FatalError(kFatalError_AllocateFlicFile_S, gMain_flic_list[pIndex].file_name);
        }
#ifdef DETHRACE_FIX_BUGS
        fclose(f);
#endif
        return 0;
    }

    fread(gMain_flic_list[pIndex].data_ptr, 1, gMain_flic_list[pIndex].the_size, f);
    strcpy(gLast_flic_name, gMain_flic_list[pIndex].file_name);
    fclose(f);
    return 1;
}

// IDA: void __usercall UnlockFlic(int pIndex@<EAX>)
// FUNCTION: CARM95 0x00497683
void UnlockFlic(int pIndex) {
    if (pIndex >= 0) {
        if (gMain_flic_list[pIndex].data_ptr != NULL) {
            MAMSUnlock((void**)&gMain_flic_list[pIndex].data_ptr);
        }
    }
}

// IDA: int __usercall LoadFlicData@<EAX>(char *pName@<EAX>, tU8 **pData@<EDX>, tU32 *pData_length@<EBX>)
// FUNCTION: CARM95 0x004976cb
int LoadFlicData(char* pName, tU8** pData, tU32* pData_length) {
    FILE* f;
    tPath_name the_path;

    if (*pData != NULL) {
        MAMSLock((void**)pData);
        return 1;
    }
    if (gPlay_from_disk) {
        return 1;
    }
    PossibleService();
    PathCat(the_path, gApplication_path, "ANIM");
    PathCat(the_path, the_path, pName);
    f = DRfopen(the_path, "rb");
    if (f == NULL) {
        return 0;
    }
    *pData_length = GetFileLength(f);
    *pData = BrMemAllocate(*pData_length, kMem_flic_data_2);
    if (*pData == NULL) {
        fclose(f);
        return 0;
    }
    fread(*pData, 1, *pData_length, f);
    fclose(f);
    return 1;
}

// IDA: void __usercall FreeFlic(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004977de
void FreeFlic(int pIndex) {

    PossibleService();
    if (gMain_flic_list[pIndex].data_ptr != NULL) {
        BrMemFree(gMain_flic_list[pIndex].data_ptr);
        gMain_flic_list[pIndex].data_ptr = NULL;
    }
}

// IDA: void __usercall ForceRunFlic(int pIndex@<EAX>)
// FUNCTION: CARM95 0x00497908
void ForceRunFlic(int pIndex) {

    LoadFlic(pIndex);
    ShowFlic(pIndex);
    UnlockFlic(pIndex);
}

// IDA: void __usercall RunFlicAt(int pIndex@<EAX>, int pX@<EDX>, int pY@<EBX>)
// FUNCTION: CARM95 0x00497829
void RunFlicAt(int pIndex, int pX, int pY) {

    LoadFlic(pIndex);
    PlayFlic(
        pIndex,
        gMain_flic_list[pIndex].the_size,
        gMain_flic_list[pIndex].data_ptr,
        gBack_screen,
        pX,
        pY,
        SwapScreen,
        0,
        0);
    UnlockFlic(pIndex);
}

// IDA: void __usercall RunFlic(int pIndex@<EAX>)
// FUNCTION: CARM95 0x0049788b
void RunFlic(int pIndex) {

    if (gPending_flic >= 0) {
        ForceRunFlic(gPending_flic);
        gPending_flic = -1;
    }
    if (LoadFlic(pIndex)) {
        if (gMain_flic_list[pIndex].queued) {
            gPending_flic = pIndex;
        } else {
            ShowFlic(pIndex);
            UnlockFlic(pIndex);
        }
    }
}

// IDA: void __usercall PreloadBunchOfFlics(int pBunch_index@<EAX>)
// FUNCTION: CARM95 0x00497937
void PreloadBunchOfFlics(int pBunch_index) {
    int i;

    for (i = 0; i < gFlic_bunch[pBunch_index].count; i++) {
        LoadFlic(gFlic_bunch[pBunch_index].indexes[i]);
    }
}

// IDA: void __usercall UnlockBunchOfFlics(int pBunch_index@<EAX>)
// FUNCTION: CARM95 0x00497986
void UnlockBunchOfFlics(int pBunch_index) {
    int i;

    for (i = 0; i < gFlic_bunch[pBunch_index].count; i++) {
        UnlockFlic(gFlic_bunch[pBunch_index].indexes[i]);
    }
}

// IDA: void __usercall FlushAllFlics(int pBunch_index@<EAX>)
// FUNCTION: CARM95 0x004979d5
void FlushAllFlics(int pBunch_index) {
    int i;

    for (i = 0; i < COUNT_OF(gMain_flic_list); i++) {
        FreeFlic(i);
    }
}

// IDA: void __cdecl InitFlicQueue()
// FUNCTION: CARM95 0x00497a10
void InitFlicQueue(void) {
    gFirst_flic = NULL;
}

// IDA: int __cdecl FlicQueueFinished()
// FUNCTION: CARM95 0x00497a25
int FlicQueueFinished(void) {
    tFlic_descriptor* the_flic;

    the_flic = gFirst_flic;
    while (the_flic != NULL) {
        if (the_flic->must_finish) {
            return 0;
        }
        the_flic = the_flic->next;
    }
    return 1;
}

// IDA: void __usercall ProcessFlicQueue(tU32 pInterval@<EAX>)
// FUNCTION: CARM95 0x00497a71
void ProcessFlicQueue(tU32 pInterval) {
    tFlic_descriptor* the_flic;
    tFlic_descriptor* last_flic;
    tFlic_descriptor* doomed_flic;
    tU32 new_time;
    int finished_playing;

    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    the_flic = gFirst_flic;
    last_flic = NULL;
    new_time = PDGetTotalTime();
    while (the_flic != NULL) {
        if (new_time - the_flic->last_frame < the_flic->frame_period) {
            finished_playing = 0;
        } else {
            the_flic->last_frame = new_time;
            finished_playing = PlayNextFlicFrame(the_flic);
        }
        if (finished_playing) {
            EndFlic(the_flic);
            if (last_flic != NULL) {
                last_flic->next = the_flic->next;
            } else {
                gFirst_flic = the_flic->next;
            }
            doomed_flic = the_flic;
            the_flic = the_flic->next;
            BrMemFree(doomed_flic);
        } else {
            last_flic = the_flic;
            the_flic = the_flic->next;
        }
    }
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
}

// IDA: void __cdecl FlushFlicQueue()
// FUNCTION: CARM95 0x00497b5d
void FlushFlicQueue(void) {
    tFlic_descriptor* the_flic;
    tFlic_descriptor* old_flic;

    // Jeff: loop through pending flics until we reach the end or we find one that is `must_finish`.
    // If `must_finish`, we process the queue then check again.
    // If there are no `must_finish` in the list, immediately end any remaining flics

    while (!FlicQueueFinished()) {
        RemoveTransientBitmaps(1);
        ProcessFlicQueue(gFrame_period);
        DoMouseCursor();
        PDScreenBufferSwap(0);
    }
    the_flic = gFirst_flic;
    while (the_flic != NULL) {
        EndFlic(the_flic);
        old_flic = the_flic;
        the_flic = the_flic->next;
        BrMemFree(old_flic);
    }
    gFirst_flic = NULL;
}

// IDA: void __usercall AddToFlicQueue(int pIndex@<EAX>, int pX@<EDX>, int pY@<EBX>, int pMust_finish@<ECX>)
// FUNCTION: CARM95 0x00497bec
void AddToFlicQueue(int pIndex, int pX, int pY, int pMust_finish) {
    tFlic_descriptor* the_flic = NULL;
    tFlic_descriptor* new_flic = NULL;
    tFlic_descriptor* last_flic = NULL;
    tFlic_descriptor* doomed_flic = NULL;

    the_flic = gFirst_flic;
    while (the_flic != NULL) {
        if (pX == the_flic->x_offset && pY == the_flic->y_offset) {
            doomed_flic = the_flic;
            break;
        }
        last_flic = the_flic;
        the_flic = the_flic->next;
    }

    if (doomed_flic != NULL) {
        EndFlic(doomed_flic);
        if (last_flic != NULL) {
            last_flic->next = doomed_flic->next;
        } else {
            gFirst_flic = doomed_flic->next;
        }
        BrMemFree(doomed_flic);
    }

    LoadFlic(pIndex);
    new_flic = BrMemAllocate(sizeof(tFlic_descriptor), kMem_queued_flic);
    new_flic->next = NULL;
    the_flic = gFirst_flic;
    if (gFirst_flic != NULL) {
        while (the_flic->next != NULL) {
            the_flic = the_flic->next;
        }
        the_flic->next = new_flic;
    } else {
        gFirst_flic = new_flic;
    }
    new_flic->last_frame = 0;
    new_flic->data_start = NULL;
    new_flic->the_index = pIndex;
    new_flic->must_finish = pMust_finish;

    StartFlic(
        gMain_flic_list[pIndex].file_name,
        pIndex,
        new_flic,
        gMain_flic_list[pIndex].the_size,
        gMain_flic_list[pIndex].data_ptr,
        gBack_screen,
        pX >= 0 ? pX : gMain_flic_list[pIndex].x_offset,
        pY >= 0 ? pY : gMain_flic_list[pIndex].y_offset,
        20);
}

// IDA: void __usercall InitialiseFlicPanel(int pIndex@<EAX>, int pLeft@<EDX>, int pTop@<EBX>, int pWidth@<ECX>, int pHeight)
// FUNCTION: CARM95 0x00497dcd
void InitialiseFlicPanel(int pIndex, int pLeft, int pTop, int pWidth, int pHeight) {
    void* the_pixels;

    gPanel_flic[pIndex].data = NULL;
    gPanel_flic_left[pIndex] = pLeft;
    gPanel_flic_top[pIndex] = pTop;
    the_pixels = BrMemAllocate(pHeight * ((pWidth + 3) & ~3), kFlic_panel_pixels);
    if (gScreen->row_bytes < 0) {
        BrFatal(
            "..\\..\\source\\common\\flicplay.c",
            2116,
            "Bruce bug at line %d, file ..\\..\\source\\common\\flicplay.c",
            68);
    }
    gPanel_buffer[pIndex] = DRPixelmapAllocate(
#ifdef DETHRACE_3DFX_PATCH
        gBack_screen->type,
#else
        gScreen->type,
#endif
        pWidth,
        pHeight,
        the_pixels,
        0);
}

// IDA: void __usercall DisposeFlicPanel(int pIndex@<EAX>)
// FUNCTION: CARM95 0x00497e80
void DisposeFlicPanel(int pIndex) {

    EndFlic(&gPanel_flic[pIndex]);
    BrMemFree(gPanel_buffer[pIndex]->pixels);
    BrPixelmapFree(gPanel_buffer[pIndex]);
    gPanel_buffer[pIndex] = NULL;
}

// IDA: void __usercall ServicePanelFlics(int pCopy_to_buffer@<EAX>)
// FUNCTION: CARM95 0x00497edd
void ServicePanelFlics(int pCopy_to_buffer) {
    tU32 time_diff;
    tU32 the_time;
    tU32 old_last_time[2];
    int i;
    int j;
    int iteration_count;
    int finished;

    if (gPanel_flic_disable) {
        return;
    }
    the_time = PDGetTotalTime();
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();

    for (i = 0; i < COUNT_OF(gPanel_flic); i++) {
        old_last_time[i] = gLast_panel_frame_time[i];
        if (gPanel_buffer[i] != NULL && gPanel_flic[i].data != NULL) {
            if (old_last_time[i] != 0) {
                time_diff = the_time - old_last_time[i];
                iteration_count = time_diff / gPanel_flic[i].frame_period;
            } else {
                iteration_count = 1;
            }
            for (j = 0; j < iteration_count; j++) {
                finished = PlayNextFlicFrame2(&gPanel_flic[i], 1);
                if (finished) {
                    EndFlic(&gPanel_flic[i]);
                    StartFlic(
                        gPanel_flic[i].file_name,
                        gPanel_flic[i].the_index,
                        &gPanel_flic[i],
                        gPanel_flic_data_length[i],
                        (tS8*)gPanel_flic_data[i],
                        gPanel_buffer[i],
                        0,
                        0,
                        0);
                }
                gLast_panel_frame_time[i] = the_time;
            }
            if (pCopy_to_buffer) {
                BrPixelmapRectangleCopy(
                    gBack_screen,
                    gPanel_flic_left[i],
                    gPanel_flic_top[i],
                    gPanel_buffer[i],
                    0,
                    0,
                    gPanel_buffer[i]->width,
                    gPanel_buffer[i]->height);
            }
        }
    }
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
}

// IDA: void __usercall ChangePanelFlic(int pIndex@<EAX>, tU8 *pData@<EDX>, tU32 pData_length@<EBX>)
// FUNCTION: CARM95 0x004980ec
void ChangePanelFlic(int pIndex, tU8* pData, tU32 pData_length) {

    EndFlic(&gPanel_flic[pIndex]);
    gPanel_flic_data[pIndex] = pData;
    gPanel_flic_data_length[pIndex] = pData_length;
    BrPixelmapFill(gPanel_buffer[pIndex], 0);
    StartFlic(
        gPanel_flic[pIndex].file_name,
        pIndex,
        &gPanel_flic[pIndex],
        gPanel_flic_data_length[pIndex],
        (tS8*)gPanel_flic_data[pIndex],
        gPanel_buffer[pIndex],
        0,
        0,
        0);
    gLast_panel_frame_time[pIndex] = 0;
    ServicePanelFlics(0);
}

// IDA: br_pixelmap* __usercall GetPanelPixelmap@<EAX>(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004981b5
br_pixelmap* GetPanelPixelmap(int pIndex) {

    return gPanel_buffer[pIndex];
}

// IDA: void __cdecl LoadInterfaceStrings()
// FUNCTION: CARM95 0x004981cf
void LoadInterfaceStrings(void) {
    FILE* f;
    char s[256];
    char s2[256];
    char* str;
    char* comment;
    char ch;
    tPath_name the_path;
    int i;
    int j;
    int len;

    gTranslation_count = 0;
    PathCat(the_path, gApplication_path, "TRNSLATE.TXT");
    f = fopen(the_path, "rt");
    if (f != NULL) {

        while (!feof(f)) {
            GetALineAndDontArgue(f, s);
            gTranslation_count++;
        }
        rewind(f);
        gTranslations = BrMemAllocate(gTranslation_count * sizeof(tTranslation_record), kMem_translations);
        for (i = 0; i < gTranslation_count; i++) {
            GetALineAndDontArgue(f, s);
            str = strtok(s, "\t ,/");
            strcpy(s2, str);
            strtok(s2, ".");
            strcat(s2, ".FLI");
            gTranslations[i].flic_index = -1;
            for (j = 0; j < COUNT_OF(gMain_flic_list); j++) {
                if (strcmp(gMain_flic_list[j].file_name, s2) == 0) {
                    gTranslations[i].flic_index = j;
                    break;
                }
            }
            if (gTranslations[i].flic_index < 0) {
                FatalError(kFatalError_FindFlicUsedInTranslationFile_S, s2);
            }
            str[strlen(str)] = ',';
            str = strtok(s, "\t ,/");
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%d", &gTranslations[i].x);
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%d", &gTranslations[i].y);
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%d", &gTranslations[i].font_index);
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%c", &ch);
            switch (ch) {
            case 'L':
            case 'l':
                gTranslations[i].justification = eJust_left;
                break;
            case 'R':
            case 'r':
                gTranslations[i].justification = eJust_right;
                break;
            case 'C':
            case 'c':
                gTranslations[i].justification = eJust_centre;
                break;
            }
            str = strtok(NULL, "\t ,/");
            sscanf(str, "%c", &ch);
            gTranslations[i].global = ch == 'g' || ch == 'G';
            gTranslations[i].every_frame = strlen(str) > 1 && (str[1] == 'e' || str[1] == 'E');
            str += strlen(str) + 1;
            comment = strstr(str, "//");
            if (comment != NULL) {
                *comment = '\0';
            }
            len = strlen(str);
            for (j = len - 1; j >= 0; j--) {
                if (str[j] != ' ' && str[j] != '\t') {
                    break;
                }
            }
            str[j + 1] = '\0';
            gTranslations[i].text = BrMemAllocate(strlen(str) + 1, kMem_translations_text);
            strcpy(gTranslations[i].text, str);
        }
        LoadFont(kFont_BLUEHEAD);
        LoadFont(kFont_ORANGHED);
        LoadFont(kFont_GREENHED);
        LoadFont(kFont_LITPLAQ);
        LoadFont(kFont_BUTTOUT);
        LoadFont(kFont_DRKPLAQ);
        LoadFont(kFont_BUTTIN);
        LoadFont(kFont_GRNLIT);
        LoadFont(kFont_GRYLIT);
        LoadFont(kFont_GRNDK);
        LoadFont(kFont_GRYDK);
        LoadFont(kFont_LITPLAQ1);
        LoadFont(kFont_BUTTOUT1);
        LoadFont(kFont_DRKPLAQ1);
        LoadFont(kFont_BUTTIN1);
        gTrans_fonts[0] = &gFonts[kFont_ORANGHED];
        gTrans_fonts[1] = &gFonts[kFont_LITPLAQ];
        gTrans_fonts[2] = &gFonts[kFont_BUTTOUT];
        gTrans_fonts[3] = &gFonts[kFont_DRKPLAQ];
        gTrans_fonts[4] = &gFonts[kFont_BUTTIN];
        gTrans_fonts[5] = &gFonts[kFont_GRNLIT];
        gTrans_fonts[6] = &gFonts[kFont_GRYLIT];
        gTrans_fonts[7] = &gFonts[kFont_GRNDK];
        gTrans_fonts[8] = &gFonts[kFont_GRYDK];
        gTrans_fonts[9] = &gFonts[kFont_LITPLAQ1];
        gTrans_fonts[10] = &gFonts[kFont_DRKPLAQ1];
        gTrans_fonts[11] = &gFonts[kFont_BUTTOUT1];
        gTrans_fonts[12] = &gFonts[kFont_BUTTIN1];
    }

#ifdef DETHRACE_FIX_BUGS
    fclose(f);
#endif
}

// IDA: void __cdecl FlushInterfaceFonts()
// FUNCTION: CARM95 0x00498961
void FlushInterfaceFonts(void) {

    DisposeFont(19);
    DisposeFont(18);
    DisposeFont(20);
    DisposeFont(17);
    DisposeFont(15);
    DisposeFont(14);
    DisposeFont(16);
    DisposeFont(13);
}

// IDA: void __cdecl SuspendPendingFlic()
// FUNCTION: CARM95 0x004989bc
void SuspendPendingFlic(void) {

    gPending_pending_flic = gPending_flic;
    gPending_flic = -1;
}

// IDA: void __cdecl ResumePendingFlic()
// FUNCTION: CARM95 0x004989db
void ResumePendingFlic(void) {

    gPending_flic = gPending_pending_flic;
}
