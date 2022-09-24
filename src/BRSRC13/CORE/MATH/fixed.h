#ifndef _FIXED_H_
#define _FIXED_H_

#include "brender/br_types.h"
#include "brender/br_inline_funcs.h"

#define BR_ONE_LU ((br_fixed_lu)0x00010000)
#define BR_ONE_LSF ((br_fixed_ls)0x00008000)
#define BR_ONE_LUF ((br_fixed_lu)0x00010000)
#define BR_ONE_SS ((br_fixed_ss)0x0100)
#define BR_ONE_SU ((br_fixed_ss)0x0100)
#define BR_ONE_SSF ((br_fixed_ss)0x0080)
#define BR_ONE_SUF ((br_fixed_ss)0x0100)
#define BrIntToFixed(i) ((br_fixed_ls)(((unsigned)(i))<<16))
#define BrFloatToFixed(f) ((br_fixed_ls)((f)*65536.f))
#define BrFixedToInt(i) ((i)>>16)
//#define BrFixedToFloat(i) ((float)((i)*(1.f/65536.f)))

#define BrFloatToFixedFraction(f) ((br_fixed_lsf)((f)*32768.f))
#define BrFixedFractionToFloat(f) ((float)(((br_fixed_lsf)(f))*(1.f/32768.f)))

#define BrFloatToFixedUFraction(f) ((br_fixed_luf)((f)*65536.f))
#define BrFixedUFractionToFloat(f) ((float)(((br_fixed_luf)(f))*(1.f/65536.f)))

#if 1
#define BrFloatToScalar(f) f
#define BrScalarToFlat(f) f
#define BrFixedToScalar BrFixedToFloat
#define BrScalarToFixed BrFloatToFixed

#define BrScalarToFraction(f) f
#define BrFractionToScalar(f) f
#endif

br_fixed_ls BrFixedAbs(br_fixed_ls a);

br_fixed_ls BrFixedMul(br_fixed_ls a, br_fixed_ls b);

br_fixed_ls BrFixedMac2(br_fixed_ls a, br_fixed_ls b, br_fixed_ls c, br_fixed_ls d);

br_fixed_ls BrFixedMac3(br_fixed_ls a, br_fixed_ls b, br_fixed_ls c, br_fixed_ls d, br_fixed_ls e, br_fixed_ls f);

br_fixed_ls BrFixedMac4(br_fixed_ls a, br_fixed_ls b, br_fixed_ls c, br_fixed_ls d, br_fixed_ls e, br_fixed_ls f, br_fixed_ls g, br_fixed_ls h);

br_fixed_ls BrFixedLength2(br_fixed_ls a, br_fixed_ls b);

br_fixed_ls BrFixedLength3(br_fixed_ls a, br_fixed_ls b, br_fixed_ls c);

br_fixed_ls BrFixedLength4(br_fixed_ls a, br_fixed_ls b, br_fixed_ls c, br_fixed_ls d);

br_fixed_ls BrFixedRLength2(br_fixed_ls a, br_fixed_ls b);

br_fixed_ls BrFixedRLength3(br_fixed_ls a, br_fixed_ls b, br_fixed_ls c);

br_fixed_ls BrFixedRLength4(br_fixed_ls a, br_fixed_ls b, br_fixed_ls c, br_fixed_ls d);

br_fixed_ls BrFixedDiv(br_fixed_ls a, br_fixed_ls b);

br_fixed_ls BrFixedDivR(br_fixed_ls a, br_fixed_ls b);

br_fixed_ls BrFixedDivF(br_fixed_ls a, br_fixed_ls b);

br_fixed_ls BrFixedMulDiv(br_fixed_ls a, br_fixed_ls b, br_fixed_ls c);

br_fixed_ls BrFixedMac2Div(br_fixed_ls a, br_fixed_ls b, br_fixed_ls c, br_fixed_ls d, br_fixed_ls e);

br_fixed_ls BrFixedMac3Div(br_fixed_ls a, br_fixed_ls b, br_fixed_ls c, br_fixed_ls d, br_fixed_ls e, br_fixed_ls f, br_fixed_ls g);

br_fixed_ls BrFixedMac4Div(br_fixed_ls a, br_fixed_ls b, br_fixed_ls c, br_fixed_ls d, br_fixed_ls e, br_fixed_ls f, br_fixed_ls g, br_fixed_ls h, br_fixed_ls i);

br_fixed_ls BrFixedFMac2(br_fixed_lsf a, br_fixed_ls b, br_fixed_lsf c, br_fixed_ls d);

br_fixed_ls BrFixedFMac3(br_fixed_lsf a, br_fixed_ls b, br_fixed_lsf c, br_fixed_ls d, br_fixed_lsf e, br_fixed_ls f);

br_fixed_ls BrFixedFMac4(br_fixed_lsf a, br_fixed_ls b, br_fixed_lsf c, br_fixed_ls d, br_fixed_lsf e, br_fixed_ls f, br_fixed_lsf g, br_fixed_ls h);

br_fixed_ls BrFixedRcp(br_fixed_ls a);

br_fixed_ls BrFixedSqr(br_fixed_ls a);

br_fixed_ls BrFixedSqr2(br_fixed_ls a, br_fixed_ls b);

br_fixed_ls BrFixedSqr3(br_fixed_ls a, br_fixed_ls b, br_fixed_ls c);

br_fixed_ls BrFixedSqr4(br_fixed_ls a, br_fixed_ls b, br_fixed_ls c, br_fixed_ls d);

br_fixed_ls BrFixedSin(br_angle a);

br_fixed_ls BrFixedCos(br_angle a);

br_angle BrFixedASin(br_fixed_ls a);

br_angle BrFixedACos(br_fixed_ls a);

br_angle BrFixedATan2(br_fixed_ls x, br_fixed_ls y);

br_angle BrFixedATan2Fast(br_fixed_ls x, br_fixed_ls y);

br_uint_16 _BrISqrt32(br_uint_32 a);

br_uint_32 _BrISqrt64(uint64_t a);

br_uint_16 _BrFastSqrt32(br_uint_32 v);

br_uint_32 _BrFastRSqrt32(br_uint_32 v);

br_uint_32 _BrFastRSqrt64(uint64_t v);

#endif
