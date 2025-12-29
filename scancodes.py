#!/usr/bin/env python3 -u

import sys
import os

codes = {
    'SCANCODE_ESCAPE' : 0x01,
'SCANCODE_1' : 0x02,
'SCANCODE_2' : 0x03,
'SCANCODE_3' : 0x04,
'SCANCODE_4' : 0x05,
'SCANCODE_5' : 0x06,
'SCANCODE_6' : 0x07,
'SCANCODE_7' : 0x08,
'SCANCODE_8' : 0x09,
'SCANCODE_9' : 0x0A,
'SCANCODE_0' : 0x0B,
'SCANCODE_MINUS' : 0x0C,
'SCANCODE_EQUALS' : 0x0D,
'SCANCODE_BACK' : 0x0E,
'SCANCODE_TAB' : 0x0F,
'SCANCODE_Q' : 0x10,
'SCANCODE_W' : 0x11,
'SCANCODE_E' : 0x12,
'SCANCODE_R' : 0x13,
'SCANCODE_T' : 0x14,
'SCANCODE_Y' : 0x15,
'SCANCODE_U' : 0x16,
'SCANCODE_I' : 0x17,
'SCANCODE_O' : 0x18,
'SCANCODE_P' : 0x19,
'SCANCODE_LBRACKET' : 0x1A,
'SCANCODE_RBRACKET' : 0x1B,
'SCANCODE_RETURN' : 0x1C,
'SCANCODE_LCONTROL' : 0x1D,
'SCANCODE_A' : 0x1E,
'SCANCODE_S' : 0x1F,
'SCANCODE_D' : 0x20,
'SCANCODE_F' : 0x21,
'SCANCODE_G' : 0x22,
'SCANCODE_H' : 0x23,
'SCANCODE_J' : 0x24,
'SCANCODE_K' : 0x25,
'SCANCODE_L' : 0x26,
'SCANCODE_SEMICOLON' : 0x27,
'SCANCODE_APOSTROPHE' : 0x28,
'SCANCODE_GRAVE' : 0x29,
'SCANCODE_LSHIFT' : 0x2A,
'SCANCODE_BACKSLASH' : 0x2B,
'SCANCODE_Z' : 0x2C,
'SCANCODE_X' : 0x2D,
'SCANCODE_C' : 0x2E,
'SCANCODE_V' : 0x2F,
'SCANCODE_B' : 0x30,
'SCANCODE_N' : 0x31,
'SCANCODE_M' : 0x32,
'SCANCODE_COMMA' : 0x33,
'SCANCODE_PERIOD' : 0x34,
'SCANCODE_SLASH' : 0x35,
'SCANCODE_RSHIFT' : 0x36,
'SCANCODE_MULTIPLY' : 0x37,
'SCANCODE_LMENU' : 0x38,
'SCANCODE_SPACE' : 0x39,
'SCANCODE_CAPITAL' : 0x3A,
'SCANCODE_F1' : 0x3B,
'SCANCODE_F2' : 0x3C,
'SCANCODE_F3' : 0x3D,
'SCANCODE_F4' : 0x3E,
'SCANCODE_F5' : 0x3F,
'SCANCODE_F6' : 0x40,
'SCANCODE_F7' : 0x41,
'SCANCODE_F8' : 0x42,
'SCANCODE_F9' : 0x43,
'SCANCODE_F10' : 0x44,
'SCANCODE_NUMLOCK' : 0x45,
'SCANCODE_SCROLL' : 0x46,
'SCANCODE_NUMPAD7' : 0x47,
'SCANCODE_NUMPAD8' : 0x48,
'SCANCODE_NUMPAD9' : 0x49,
'SCANCODE_SUBTRACT' : 0x4A,
'SCANCODE_NUMPAD4' : 0x4B,
'SCANCODE_NUMPAD5' : 0x4C,
'SCANCODE_NUMPAD6' : 0x4D,
'SCANCODE_ADD' : 0x4E,
'SCANCODE_NUMPAD1' : 0x4F,
'SCANCODE_NUMPAD2' : 0x50,
'SCANCODE_NUMPAD3' : 0x51,
'SCANCODE_NUMPAD0' : 0x52,
'SCANCODE_DECIMAL' : 0x53,
'SCANCODE_OEM_102' : 0x56,
'SCANCODE_F11' : 0x57,
'SCANCODE_F12' : 0x58,
'SCANCODE_F13' : 0x64 ,
'SCANCODE_F14' : 0x65  ,
'SCANCODE_F15' : 0x66   ,
'SCANCODE_KANA' : 0x70   ,
'SCANCODE_ABNT_C1' : 0x73 ,
'SCANCODE_CONVERT' : 0x79  ,
'SCANCODE_NOCONVERT' : 0x7B ,
'SCANCODE_YEN' : 0x7D       ,
'SCANCODE_ABNT_C2' : 0x7E   ,
'SCANCODE_NUMPADEQUALS' : 0x8D,
'SCANCODE_PREVTRACK' : 0x90   ,
'SCANCODE_AT' : 0x91          ,
'SCANCODE_COLON' : 0x92       ,
'SCANCODE_UNDERLINE' : 0x93   ,
'SCANCODE_KANJI' : 0x94       ,
'SCANCODE_STOP' : 0x95        ,
'SCANCODE_AX' : 0x96          ,
'SCANCODE_UNLABELED' : 0x97   ,
'SCANCODE_NEXTTRACK' : 0x99   ,
'SCANCODE_NUMPADENTER' : 0x9C ,
'SCANCODE_RCONTROL': 0x9D,
'SCANCODE_MUTE': 0xA0     ,
'SCANCODE_CALCULATOR': 0xA1 ,
'SCANCODE_PLAYPAUSE': 0xA2   ,
'SCANCODE_MEDIASTOP': 0xA4   ,
'SCANCODE_VOLUMEDOWN': 0xAE  ,
'SCANCODE_VOLUMEUP': 0xB0    ,
'SCANCODE_WEBHOME': 0xB2     ,
'SCANCODE_NUMPADCOMMA': 0xB3 ,
'SCANCODE_DIVIDE': 0xB5      ,
'SCANCODE_SYSRQ': 0xB7,
'SCANCODE_RCONTROL' : 0x9D,
'SCANCODE_RMENU': 0xB8 ,
'SCANCODE_PAUSE': 0xC5 ,
'SCANCODE_HOME': 0xC7 ,
'SCANCODE_UP': 0xC8    ,
'SCANCODE_PRIOR': 0xC9  ,
'SCANCODE_LEFT': 0xCB   ,
'SCANCODE_RIGHT': 0xCD  ,
'SCANCODE_END': 0xCF    ,
'SCANCODE_DOWN': 0xD0   ,
'SCANCODE_NEXT': 0xD1   ,
'SCANCODE_INSERT': 0xD2 ,
'SCANCODE_DELETE': 0xD3
}

keycodes = {
    'KEY_SHIFT_ANY' : 0,
    'KEY_ALT_ANY' : 1,
    'KEY_CTRL_ANY' : 2,
    'KEY_CTRL_ANY_2' : 3,
    'KEY_CAPSLOCK' : 4,
    'KEY_RSHIFT' : 5,
    'KEY_RALT' : 6,
    'KEY_RCTRL' : 7,
    'KEY_LSHIFT' : 8,
    'KEY_LALT' : 9,
    'KEY_LCTRL' : 10,
    'KEY_0' : 11,
    'KEY_1' : 12,
    'KEY_2' : 13,
    'KEY_3' : 14,
    'KEY_4' : 15,
    'KEY_5' : 16,
    'KEY_6' : 17,
    'KEY_7' : 18,
    'KEY_8' : 19,
    'KEY_9' : 20,
    'KEY_A' : 21,
    'KEY_B' : 22,
    'KEY_C' : 23,
    'KEY_D' : 24,
    'KEY_E' : 25,
    'KEY_F' : 26,
    'KEY_G' : 27,
    'KEY_H' : 28,
    'KEY_I' : 29,
    'KEY_J' : 30,
    'KEY_K' : 31,
    'KEY_L' : 32,
    'KEY_M' : 33,
    'KEY_N' : 34,
    'KEY_O' : 35,
    'KEY_P' : 36,
    'KEY_Q' : 37,
    'KEY_R' : 38,
    'KEY_S' : 39,
    'KEY_T' : 40,
    'KEY_U' : 41,
    'KEY_V' : 42,
    'KEY_W' : 43,
    'KEY_X' : 44,
    'KEY_Y' : 45,
    'KEY_Z' : 46,
    'KEY_GRAVE' : 47,
    'KEY_MINUS' : 48,
    'KEY_EQUALS' : 49,
    'KEY_BACKSPACE' : 50,
    'KEY_RETURN' : 51,
    'KEY_KP_ENTER' : 52,
    'KEY_TAB' : 53,
    'KEY_SLASH' : 54,
    'KEY_UNKNOWN_55' : 55,
    'KEY_SEMICOLON' : 56,
    'KEY_APOSTROPHE' : 57,
    'KEY_PERIOD' : 58,
    'KEY_COMMA' : 59,
    'KEY_LBRACKET' : 60,
    'KEY_RBRACKET' : 61,
    'KEY_BACKSLASH' : 62,
    'KEY_ESCAPE' : 63,
    'KEY_INSERT' : 64,
    'KEY_DELETE' : 65,
    'KEY_HOME' : 66,
    'KEY_END' : 67,
    'KEY_PAGEUP' : 68,
    'KEY_PAGEDOWN' : 69,
    'KEY_LEFT' : 70,
    'KEY_RIGHT' : 71,
    'KEY_UP' : 72,
    'KEY_DOWN' : 73,
    'KEY_KP_NUMLOCK' : 74,
    'KEY_KP_DIVIDE' : 75,
    'KEY_KP_MULTIPLY' : 76,
    'KEY_KP_MINUS' : 77,
    'KEY_KP_PLUS' : 78,
    'KEY_KP_PERIOD' : 79,
    'KEY_KP_EQUALS' : 80,
    'KEY_KP_0' : 81,
    'KEY_KP_1' : 82,
    'KEY_KP_2' : 83,
    'KEY_KP_3' : 84,
    'KEY_KP_4' : 85,
    'KEY_KP_5' : 86,
    'KEY_KP_6' : 87,
    'KEY_KP_7' : 88,
    'KEY_KP_8' : 89,
    'KEY_KP_9' : 90,
    'KEY_F1' : 91,
    'KEY_F2' : 92,
    'KEY_F3' : 93,
    'KEY_F4' : 94,
    'KEY_F5' : 95,
    'KEY_F6' : 96,
    'KEY_F7' : 97,
    'KEY_F8' : 98,
    'KEY_F9' : 99,
    'KEY_F10' : 100,
    'KEY_F11' : 101,
    'KEY_F12' : 102,
    'KEY_PRTSCN' : 103,
    'KEY_SCRLK' : 104,
    'KEY_PAUSE' : 105,
    'KEY_SPACE' : 106,
}

stri = '''gScan_code[11] = 0xB;
  gScan_code[13] = 3;
  gScan_code[14] = 4;
  gScan_code[12] = 2;
  gScan_code[15] = 5;
  gScan_code[17] = 7;
  gScan_code[18] = 8;
  gScan_code[16] = 6;
  gScan_code[19] = 9;
  gScan_code[21] = 0x1E;
  gScan_code[22] = 0x30;
  gScan_code[20] = 0xA;
  gScan_code[23] = 0x2E;
  gScan_code[25] = 0x12;
  gScan_code[26] = 0x21;
  gScan_code[24] = 0x20;
  gScan_code[27] = 0x22;
  gScan_code[29] = 0x17;
  gScan_code[30] = 0x24;
  gScan_code[28] = 0x23;
  gScan_code[31] = 0x25;
  gScan_code[33] = 0x32;
  gScan_code[34] = 0x31;
  gScan_code[32] = 0x26;
  gScan_code[35] = 0x18;
  gScan_code[37] = 0x10;
  gScan_code[38] = 0x13;
  gScan_code[36] = 0x19;
  gScan_code[39] = 0x1F;
  gScan_code[41] = 0x16;
  gScan_code[42] = 0x2F;
  gScan_code[40] = 0x14;
  gScan_code[43] = 0x11;
  gScan_code[44] = 0x2D;
  gScan_code[45] = 0x15;
  gScan_code[46] = 0x2C;
  gScan_code[47] = 0x29;
  gScan_code[48] = 0xC;
  gScan_code[49] = 0xD;
  gScan_code[50] = 0xE;
  gScan_code[51] = 0x1C;
  gScan_code[52] = 0x9C;
  gScan_code[0] = 0x362A;
  gScan_code[1] = 0xB838;
  gScan_code[2] = 0x9D1D;
  gScan_code[3] = 0x9D1D;
  gScan_code[4] = 0x3A;
  gScan_code[55] = 0x56;
  gScan_code[54] = 0x35;
  gScan_code[56] = 0x27;
  gScan_code[59] = 0x33;
  gScan_code[53] = 0xF;
  gScan_code[58] = 0x34;
  gScan_code[60] = 0x1A;
  gScan_code[63] = 1;
  gScan_code[57] = 0x28;
  gScan_code[62] = 0x2B;
  gScan_code[64] = 0xD2;
  gScan_code[67] = 0xCF;
  gScan_code[61] = 0x1B;
  gScan_code[66] = 0xC7;
  gScan_code[68] = 0xC9;
  gScan_code[71] = 0xCD;
  gScan_code[65] = 0xD3;
  gScan_code[70] = 0xCB;
  gScan_code[72] = 0xC8;
  gScan_code[69] = 0xD1;
  gScan_code[74] = 0x45;
  gScan_code[73] = 0xD0;
  gScan_code[75] = 0xB5;
  gScan_code[76] = 0x37;
  gScan_code[78] = 0x4E;
  gScan_code[77] = 0x4A;
  gScan_code[80] = 0;
  gScan_code[79] = 0x53;
  gScan_code[82] = 0x4F;
  gScan_code[84] = 0x51;
  gScan_code[81] = 0x52;
  gScan_code[83] = 0x50;
  gScan_code[86] = 0x4C;
  gScan_code[88] = 0x47;
  gScan_code[85] = 0x4B;
  gScan_code[87] = 0x4D;
  gScan_code[90] = 0x49;
  gScan_code[92] = 0x3C;
  gScan_code[89] = 0x48;
  gScan_code[91] = 0x3B;
  gScan_code[94] = 0x3E;
  gScan_code[96] = 0x40;
  gScan_code[93] = 0x3D;
  gScan_code[95] = 0x3F;
  gScan_code[98] = 0x42;
  gScan_code[100] = 0x44;
  gScan_code[97] = 0x41;
  gScan_code[99] = 0x43;
  gScan_code[102] = 0x58;
  gScan_code[104] = 0x46;
  gScan_code[101] = 0x57;
  gScan_code[103] = 0;
  gScan_code[105] = 0;
  gScan_code[106] = 0x39;
  gScan_code[5] = 0x36;
  gScan_code[6] = 0xB8;
  gScan_code[7] = 0x9D;
  gScan_code[8] = 0x2A;
  gScan_code[9] = 0x38;
  gScan_code[10] = 0x1D;'''

lines = stri.splitlines()
for l in lines:
    parts = l.split('=')
    # print(parts)
    val = parts[1].strip().replace(';', '')
    val_int = int(val, 16)

    parts2 = l.split(']')
    # print(parts)
    parts2[0] = parts2[0].strip()
    # i = parts[0].index(']')
    val_int2 = int(parts2[0][11:])


    code = -1
    keycode = -1
    printed = False
    for c in codes:
        if codes[c] == val_int:
            code = c

    if code == -1:
        code = val_int

    for c in keycodes:
        if keycodes[c] == val_int2:

            # printed = True
            keycode = c
        # print(parts[0] + '= ' +  str(val_int) + ';')

    if keycode == -1:
        keycode = val_int2

    print('gScan_code[' + str(keycode) + '][0] = ' +  str(code) + ';')


stri = '''gScan_code[11][0] = SCANCODE_0;
    gScan_code[13][0] = SCANCODE_2;
    gScan_code[14][0] = SCANCODE_3;
    gScan_code[12][0] = SCANCODE_1;
    gScan_code[15][0] = SCANCODE_4;
    gScan_code[17][0] = SCANCODE_6;
    gScan_code[18][0] = SCANCODE_7;
    gScan_code[16][0] = SCANCODE_5;
    gScan_code[19][0] = SCANCODE_8;
    gScan_code[21][0] = SCANCODE_A;
    gScan_code[22][0] = SCANCODE_B;
    gScan_code[20][0] = SCANCODE_9;
    gScan_code[23][0] = SCANCODE_C;
    gScan_code[25][0] = SCANCODE_E;
    gScan_code[26][0] = SCANCODE_F;
    gScan_code[24][0] = SCANCODE_D;
    gScan_code[27][0] = SCANCODE_G;
    gScan_code[29][0] = SCANCODE_I;
    gScan_code[30][0] = SCANCODE_J;
    gScan_code[28][0] = SCANCODE_H;
    gScan_code[31][0] = SCANCODE_K;
    gScan_code[33][0] = SCANCODE_M;
    gScan_code[34][0] = SCANCODE_N;
    gScan_code[32][0] = SCANCODE_L;
    gScan_code[35][0] = SCANCODE_O;
    gScan_code[37][0] = SCANCODE_Q;
    gScan_code[38][0] = SCANCODE_R;
    gScan_code[36][0] = SCANCODE_P;
    gScan_code[39][0] = SCANCODE_S;
    gScan_code[41][0] = SCANCODE_U;
    gScan_code[42][0] = SCANCODE_V;
    gScan_code[40][0] = SCANCODE_T;
    gScan_code[43][0] = SCANCODE_W;
    gScan_code[44][0] = SCANCODE_X;
    gScan_code[45][0] = SCANCODE_Y;
    gScan_code[46][0] = SCANCODE_Z;
    gScan_code[47][0] = SCANCODE_GRAVE;
    gScan_code[48][0] = SCANCODE_MINUS;
    gScan_code[49][0] = SCANCODE_EQUALS;
    gScan_code[50][0] = SCANCODE_BACK;
    gScan_code[51][0] = SCANCODE_RETURN;
    gScan_code[52][0] = SCANCODE_NUMPADENTER;
    gScan_code[0][0] = SCANCODE_LSHIFT;
    gScan_code[0][1] = SCANCODE_RSHIFT;
    gScan_code[1][0] = SCANCODE_LMENU;
    gScan_code[1][1] = SCANCODE_RMENU;
    gScan_code[2][0] = SCANCODE_LCONTROL;
    gScan_code[2][1] = SCANCODE_RCONTROL;
    gScan_code[3][0] = SCANCODE_LCONTROL;
    gScan_code[3][1] = SCANCODE_RCONTROL;
    gScan_code[4][0] = SCANCODE_CAPITAL;
    gScan_code[55][0] = SCANCODE_OEM_102;
    gScan_code[54][0] = SCANCODE_SLASH;
    gScan_code[56][0] = SCANCODE_SEMICOLON;
    gScan_code[59][0] = SCANCODE_COMMA;
    gScan_code[53][0] = SCANCODE_TAB;
    gScan_code[58][0] = SCANCODE_PERIOD;
    gScan_code[60][0] = SCANCODE_LBRACKET;
    gScan_code[63][0] = SCANCODE_ESCAPE;
    gScan_code[57][0] = SCANCODE_APOSTROPHE;
    gScan_code[62][0] = SCANCODE_BACKSLASH;
    gScan_code[64][0] = SCANCODE_INSERT;
    gScan_code[67][0] = SCANCODE_END;
    gScan_code[61][0] = SCANCODE_RBRACKET;
    gScan_code[66][0] = SCANCODE_HOME;
    gScan_code[68][0] = SCANCODE_PRIOR;
    gScan_code[71][0] = SCANCODE_RIGHT;
    gScan_code[65][0] = SCANCODE_DELETE;
    gScan_code[70][0] = SCANCODE_LEFT;
    gScan_code[72][0] = SCANCODE_UP;
    gScan_code[69][0] = SCANCODE_NEXT;
    gScan_code[74][0] = SCANCODE_NUMLOCK;
    gScan_code[73][0] = SCANCODE_DOWN;
    gScan_code[75][0] = SCANCODE_DIVIDE;
    gScan_code[76][0] = SCANCODE_MULTIPLY;
    gScan_code[78][0] = SCANCODE_ADD;
    gScan_code[77][0] = SCANCODE_SUBTRACT;
    gScan_code[80][0] = 0;'''

# lines = stri.splitlines()
# for l in lines:
#     parts = l.split(']')
#     # print(parts)
#     parts[0] = parts[0].strip()
#     # i = parts[0].index(']')
#     val_int = int(parts[0][11:])
#     # print(val_int)

#     printed = False
#     for c in keycodes:
#         if keycodes[c] == val_int:
#             print('gScan_code[' + c + ']' + parts[1] + ']' + parts[2])
#             printed = True
#     if not printed:
#         print('error!')
#     # break
