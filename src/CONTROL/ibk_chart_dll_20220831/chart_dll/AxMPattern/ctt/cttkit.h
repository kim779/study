/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

/**
#ifdef _WINDOWS
#include <windows.h>
#else
#define WINAPI
#define min(x,y) (x > y) ? y : x
#define max(x,y) (x < y) ? y : x
#define floor(x) ((float)x)
#endif
**/

#ifndef __CTTKIT_H__
#define __CTTKIT_H__



/* -------------------------------------------------------------------- */ 
/* ctreturn.h                                                           */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

#define ctSuccess                0
#define ctErrSuccess             0
#define ctErrInvalidParm        -1
#define ctErrFileOpen           -2
#define ctErrMalloc             -3
#define ctErrFileRead           -4
#define ctErrFileWrite          -5
#define ctErrFileSeek           -6
#define ctErrFileExists         -7
#define ctErrFileNotFound       -8
#define ctErrRecordNotFound     -9
#define ctChartErrNoGraphics   -10
#define ctChartErrNoFonts      -11
#define ctChartErrUndefined    -12
#define ctChartErrMaxlines     -13
#define ctChartErrMaxgrids     -14

/* -------------------------------------------------------------------- */ 
/* ctarray.h                                                            */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */


typedef struct
{
 float   *data;
 short    size;
 short    width;
 short    element;
 short    type; /* 0=float, 1=short */
} ctArray;

typedef struct
{
 unsigned short *data;
 short    size;
 short    width;
 short    element;
 short    type; /* 0=float, 1=short */
} ctArrayI;

typedef struct
{
 ctArrayI dt, ti;                     
 ctArray  op, hi, lo, cl, vol, oi;
 short    size;
 short    datasize;
 long     reccnt;
 char     path[80];
 char     name[32];
 char     description[32];
 char     symbol[16];
 char     cusip[12];
 unsigned short begindate;
 unsigned short enddate;
 short    frequency;         /* I D W M Q Y */
 short    datatype;          /* C S M O B X */
 short    optiontype;        /* C P */
 unsigned short deliverydate;
 float    strikeprice;
 short    type;              /* file type constant */
 char     filler[2];
} ctBars;

extern ctArray  ctNOARRAY;
extern ctArray *ctNULLARRAY;
extern ctBars   ctNOBARS;
extern ctBars  *ctNULLBARS;

#define ctIsNullBars(b1) (b1->dt.data==NULL)
#define ctIsNullArray(a1) (a1->data==NULL)
#define ctIsNoBars(b1) (b1.dt.data==NULL)
#define ctIsNoArray(a1) (a1.data==NULL)

#define ctArrayItem(a1,pos) a1.data[(pos)*a1.width+a1.element]
#define ctArrayItemP(a1,pos) a1->data[(pos)*a1->width+a1->element]
#define ctArrayItemM(a1,el,pos) a1.data[(pos)*a1.width+(el)]
#define ctArrayItemPM(a1,el,pos) a1->data[(pos)*a1->width+(el)]
#define ctBarSize(b1) (b1.size)
#define ctBarSizeP(b1) (b1->size)

#define SIZEOFBARDATA (2*sizeof(short)+6*sizeof(float))

/* file type */
#define ctCSI 1
#define ctCT  2
#define ctDJ  3
#define ctFS  4
#define ctTC  5
#define ctTT  6
#define ctTXT 7

/* frequency */
#define ctUNKNOWN    0 
#define ctINTRADAY   1
#define ctIDAILY      2
#define ctWEEKLY     3
#define ctMONTHLY    4
#define ctQUARTERLY  5
#define ctYEARLY     6

/* datatype */
#define ctBOND       1
#define ctCOMMODITY  2
#define ctMUTUALFUND 3
#define ctOPTION     4
#define ctSTOCK      5
#define ctINDEX      6

/* optiontype */
#define ctCALL       1
#define ctPUT        2

short WINAPI l_AllocArray(ctArray *a1, short size);
short WINAPI l_AllocArrayI(ctArrayI *a1, short size);
short WINAPI l_AllocArrayM(ctArray *a1, short width, short size);
short WINAPI l_AllocArrayIM(ctArrayI *a1, short width, short size);
short WINAPI l_AllocBars(ctBars *b1, short size);
long WINAPI l_Decimals(double value, short places);
void WINAPI l_FreeArray(ctArray *a1);
void WINAPI l_FreeArrayI(ctArrayI *a1);
void WINAPI l_FreeBars(ctBars *b1);
float WINAPI l_GetArrayItem(ctArray *a1, short pos);
float WINAPI l_GetArrayItemM(ctArray *a1, short element, short pos);
short WINAPI l_IEEEtoMS(float *ieee, float *ms);
short WINAPI l_MStoIEEE(float *ms, float *ieee);
short WINAPI l_ReAllocBars(ctBars *b1, short size);
short WINAPI l_ReSizeBars(ctBars *b1, short size);
void WINAPI l_SetArrayItem(ctArray *a1, short pos, float value);
void WINAPI l_SetArrayItemM(ctArray *a1, short element, short pos, float value);
short WINAPI l_ErrDesc(short err, char *buffer);

void l_Free(void *buffer);
void *l_Malloc(unsigned short size);

/**
#ifdef _WINDOWS
int   sprintf(char *text, const char *format, ...);
int   fprintf(FILE *stream, const char *format, ...);
#endif
**/
/* -------------------------------------------------------------------- */ 
/* ctdate.h                                                             */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

#define DTFORMAT_YY_MM_DD    1
#define DTFORMAT_CCYY_MM_DD  2
#define DTFORMAT_MM_DD_YY    3
#define DTFORMAT_MM_DD_CCYY  4
#define DTFORMAT_DD_MM_YY    5
#define DTFORMAT_DD_MM_CCYY  6

#define DTFORMAT_YYMMDD      7
#define DTFORMAT_CCYYMMDD    8
#define DTFORMAT_MMDDYY      9
#define DTFORMAT_MMDDCCYY   10
#define DTFORMAT_DDMMYY     11
#define DTFORMAT_DDMMCCYY   12

#define TIFORMAT_HH_MM_SS    1
#define TIFORMAT_HH_MM       2
#define TIFORMAT_HHMMSS      3
#define TIFORMAT_HHMM        4

unsigned short WINAPI l_DateTxtDif(char *date1, char *date2, short format1, short format2);
unsigned short WINAPI l_DateTxtToJulian(char *date, short format);
short          WINAPI l_DateTxtVal(char *date, short format);
unsigned short WINAPI l_HMSDif(short hr1, short min1, short sec1, short hr2, short min2, short sec2);
unsigned short WINAPI l_HMSToJulian(short hr, short min, short sec);
unsigned short WINAPI l_JulianDif(unsigned short dti1, unsigned short dti2);
short          WINAPI l_JulianDOW(unsigned short date);
void           WINAPI l_JulianToDateTxt(unsigned short date, short format, char *sep, char *buffer);
void           WINAPI l_JulianToHMS(unsigned short time, short *hr, short *min, short *sec);
void           WINAPI l_JulianToTimeTxt(unsigned short time, short format, char *sep, char *buffer);
void           WINAPI l_JulianToYMD(unsigned short date, short *year, short *month, short *day);
unsigned short WINAPI l_TimeTxtDif(char *time1, char *time2, short format1, short format2);
unsigned short WINAPI l_TimeTxtToJulian(char *time, short format);
unsigned short WINAPI l_YMDDif(short year1, short month1, short day1, short year2, short month2, short day2);
unsigned short WINAPI l_YMDToJulian(short year, short month, short day);
/* -------------------------------------------------------------------- */ 
/* ctstr.h                                                              */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

short    l_ChrCnt(char *string, char chr);
short    l_ChrsCnt(char *string, char *chrs);
void     l_Ltrim(char *string);
void     l_Rtrim(char *string);
void     l_Trim(char *string);
short    l_StrCpyChr(char *to, char *from, char chr);
short    l_StrNthChr(char *str, char chr, short n);

/* -------------------------------------------------------------------- */ 
/* ctio.h                                                               */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

short WINAPI ctPNFchart(ctBars *pnfbars, float box, char *outfile, short append);
short WINAPI ctTXT2Array(char *infile, ctArray *a1, char *delimit, short skip, short startcol, short stopcol);

#if !defined(_WINDOWS) || defined(_WIN32)
short WINAPI ctArrayPrintf(char *outfile, short append, short start, short stop, char *heading, char *format, ...);
#endif

#ifndef _WINDOWS
short        GetPrivateProfileString(char *section, char *entry, char *def, char *returnbuffer, short returnbufferlen, char *filename);
#endif
/* -------------------------------------------------------------------- */ 
/* ctcalc.h                                                             */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

#define ctZERO   0
#define ctUP     1
#define ctDOWN  -1
#define ctTRUE   1
#define ctFALSE  0

#define BY_DAY   1
#define BY_WEEK  2
#define BY_MONTH 3

#define ExpMA      0
#define LstSqrMA   1
#define SimpleMA   2
#define WeightedMA 3
#define WellesMA   4

short WINAPI l_Accumulate(ctArray *a1, ctArray *a2, short start);
short WINAPI l_ADXR(ctBars *b1, ctArray *a2, short term1, short term2, short term3, short start);
short WINAPI l_ADX(ctBars *b1, ctArray *a2, short term1, short term2, short start);
short WINAPI l_ArmsEMV(ctBars *b1, ctArray *a2, long volinc, short start);
short WINAPI l_ArmsIdx(ctArray *a1, ctArray *a2, ctArray *a3, ctArray *a4, ctArray *a5, short start, short stop);
short WINAPI l_ArrayStats(ctArray *a1, float *mini, float *maxi, float *avg, float *sum, short start, short stop, short nonzero);
short WINAPI l_ArrayStatsPos(ctArray *a1, short *mini, short *maxi, short start, short stop, short nonzero);
short WINAPI l_ATR(ctBars *b1, ctArray *a2, short term, short start);
float WINAPI l_AvgPrice(ctBars *b1, short pos);
short WINAPI l_Bars2PNF(ctBars *b1, ctBars *b2, ctArray *index, float box, float rev, short start);
short WINAPI l_Bars2PNFhilo(ctBars *b1, ctBars *b2, ctArray *index, float box, float rev, short start);
short WINAPI l_BarStats(ctBars *b1, float *min1, float *max1, short start, short stop);
short WINAPI l_Bollinger(ctBars *b1, ctArray *a2, ctArray *a3, ctArray *a4, float factor, short term, short start);
short WINAPI l_BoltonTremblay(ctArray *a1, ctArray *a2, ctArray *a3, ctArray *a4, short start);
short WINAPI l_CCI(ctBars *b1, ctArray *a2, short term, short start);
short WINAPI l_CFTPP(ctBars *b1, ctArray *a2, ctArray *a3, ctArray *a4, ctArray *a5, short start);
short WINAPI l_ChaikinMoneyFlow(ctBars *b1, ctArray *a2, short term, short start);
float WINAPI l_CorCoef(ctArray *a1, ctArray *a2, short samp, short start, short stop);
short WINAPI l_CorCoefMv(ctArray *a1, ctArray *a2, ctArray *a3, short samp, short term, short start);
float WINAPI l_Covariance(ctArray *a1, ctArray *a2, short start, short stop);
short WINAPI l_CovarianceMv(ctArray *a1, ctArray *a2, ctArray *a3, short term, short start);
short WINAPI l_DemandIdx(ctBars *b1, ctArray *a2, short term, short stock, short start);
short WINAPI l_DeTrend(ctArray *a1, ctArray *a2, short start, short stop);
short WINAPI l_DI(ctBars *b1, ctArray *a2, ctArray *a3, short term, short start);
float WINAPI l_Dispersion(ctArray *a1, short start, short stop);
short WINAPI l_DispersionMv(ctArray *a1, ctArray *a2, short term, short start);
short WINAPI l_DMI(ctBars *b1, ctArray *a2, ctArray *a3, short start);
short WINAPI l_DownAverage(ctArray *a1, ctArray *a2, short term, short start);
short WINAPI l_DX(ctBars *b1, ctArray *a2, short term, short start);
short WINAPI l_Envelope(ctArray *a1, ctArray *a2, ctArray *a3, ctArray *a4, float pct, short start);
short WINAPI l_EnvelopePct(ctArray *a1, ctArray *a2, ctArray *a3, float pct, short start);
short WINAPI l_ExpMA(ctArray *a1, ctArray *a2, short term, short start);
float WINAPI l_ExpToPeriod(float e1);
short WINAPI l_FastD(ctBars *b1, ctArray *a2, short term1, short term2, short start);
short WINAPI l_FibonacciProject(double f1, double f2, double *project1, double *project2, double *project3);
short WINAPI l_FibonacciRetrace(double f1, double f2, double *retrace1, double *retrace2, double *retrace3);
short WINAPI l_FillInterval(ctArray *a1, ctArray *a2, ctArray *a3, short start);
short WINAPI l_GroupBars(ctBars *b1, float *op, float *hi, float *lo, float *cl, float *vol, float *oi, short term, short start);
short WINAPI l_GroupBarsArray(ctBars *b1, ctBars *b2, short term, short start);
short WINAPI l_GroupBarsDate(ctBars *b1, ctBars *b2, short period, unsigned short startdt, unsigned short startti, unsigned short enddt, unsigned short endti);
short WINAPI l_GroupBarsTime(ctBars *b1, ctBars *b2, unsigned period, unsigned short startdt, unsigned short startti, unsigned short enddt, unsigned short endti);
short WINAPI l_HerrickPayoffIndex(ctBars *b1, ctArray *a2, float mult, float factor, short start);
short WINAPI l_LastHigh(ctArray *a1, short start, short stop);
short WINAPI l_LastLow(ctArray *a1, short start, short stop);
short WINAPI l_LineOsc(ctArray *a1, ctArray *a2, ctArray *a3, short term, short start);
short WINAPI l_LstSqrMA(ctArray *a1, ctArray *a2, short term, short start);
short WINAPI l_MACD(ctArray *a1, ctArray *a2, ctArray *a3, ctArray *a4, ctArray *a5, ctArray *a6, short term1, short term2, short term3, short start);
short WINAPI l_MAEnvelope(ctArray *a1, ctArray *a2, ctArray *a3, float pct, short type, short term, short start);
short WINAPI l_MAHighLow(ctBars *b1, ctArray *a2, ctArray *a3, short type, short term, short start);
short WINAPI l_McClellanOscSum(ctArray *a1, ctArray *a2, ctArray *a3, ctArray *a4, short term1, short term2, short start);
short WINAPI l_MFI(ctBars *b1, ctArray *a2, short start);
short WINAPI l_Momentum(ctArray *a1, ctArray *a2, short term, short start);
short WINAPI l_MovingSum(ctArray *a1, ctArray *a2, short term, short start);
float WINAPI l_OBOi(ctBars *b1, short start, short stop);
short WINAPI l_OBOiMv(ctBars *b1, ctArray *a2, short term, short start);
float WINAPI l_OBVol(ctBars *b1, short start, short stop);
short WINAPI l_OBVolExp(ctBars *b1, ctArray *obv, ctArray *breakout, ctArray *fieldtrend, long seedvol, short start);
short WINAPI l_OBVolMv(ctBars *b1, ctArray *a2, short term, short start);
short WINAPI l_Oscillator(ctArray *a1, ctArray *a2, ctArray *a3, short start);
float WINAPI l_PeriodToExp(float n1);
short WINAPI l_RateOfChange(ctArray *a1, ctArray *a2, short factor, short term, short start);
short WINAPI l_Ratio(ctArray *a1, ctArray *a2, ctArray *a3, short start);
short WINAPI l_RawK(ctBars *b1, ctArray *a2, short term, short start);
short WINAPI l_RegressionLine(ctArray *a1, float *slope, float *constant, short start, short stop);
short WINAPI l_RegressionLineMv(ctArray *a1, ctArray *a2, ctArray *a3, short term, short start, short stop);
short WINAPI l_RSI(ctArray *a1, ctArray *a2, short term, short start);
short WINAPI l_RSIC(ctBars *b1, ctBars *b2, ctArray *a1, short term, unsigned short startdate);
short WINAPI l_RWI(ctBars *b1, ctArray *a2, ctArray *a3, short term, short start);
short WINAPI l_ScaleArray(ctArray *a1, ctArray *a2, float amin, float amax, float mini, float maxi, short nonzero, short start);
short WINAPI l_SimpleMA(ctArray *a1, ctArray *a2, short term, short start);
float WINAPI l_SlopeArrayPts(ctArray *a1, short x1, short x2);
float WINAPI l_SlopePts(float x1, float y1, float x2, float y2);
short WINAPI l_SlowD(ctBars *b1, ctArray *a2, short term1, short term2, short term3, short start);
float WINAPI l_StdDev(ctArray *a1, short samp, short start, short stop);
short WINAPI l_StdDevMv(ctArray *a1, ctArray *a2, short samp, short term, short start);
short WINAPI l_Stochastic(ctArray *a1, ctArray *a2, short term, short start);
float WINAPI l_SumPrev(ctArray *a1, short term, short start);
short WINAPI l_TR(ctBars *b1, ctArray *a2, short start);
short WINAPI l_TrendScore(ctArray *a1, ctArray *a2, short term, short start);
float WINAPI l_TrueHigh(ctBars *b1, short pos);
float WINAPI l_TrueLow(ctBars *b1, short pos);
float WINAPI l_TrueRange(ctBars *b1, short pos);
short WINAPI l_TSI(ctArray *a1, ctArray *a2, short term1, short term2, short start);
short WINAPI l_TXT2Array(char *infile, ctArray *a1, char *delimit, short skip, short startcol, short stopcol);
short WINAPI l_UpAverage(ctArray *a1, ctArray *a2, short term, short start);
short WINAPI l_UltimateOsc(ctBars *b1, ctArray *a2, short term1, short term2, short term3, short start);
short WINAPI l_VHF(ctArray *a1, ctArray *a2, short term, short start);
short WINAPI l_WeightedMA(ctArray *a1, ctArray *a2, short term, short start);
short WINAPI l_WellesMA(ctArray *a1, ctArray *a2, short term, short start);
short WINAPI l_WellesSum(ctArray *a1, ctArray *a2, short term, short start);
short WINAPI l_WellesVolatility(ctBars *b1, ctArray *a2, ctArray *a3, ctArray *a4, ctArray *a5, float constant, short term, short start);
short WINAPI l_WilliamsAD(ctBars *b1, ctArray *a2, short start);
short WINAPI l_WilliamsR(ctBars *b1, ctArray *a2, short term, short start);
short WINAPI l_FormationFill(ctBars *b1, ctArray *a2, char *formfunc, short start, short stop);
short WINAPI l_CPR(ctBars *b1, short pos);
short WINAPI l_Gap(ctBars *b1, short pos);
short WINAPI l_IslandReversal(ctBars *b1, short pos);
short WINAPI l_InsideBar(ctBars *b1, short pos);
short WINAPI l_KeyReversal(ctBars *b1, short pos);
short WINAPI l_OutsideBar(ctBars *b1, short pos);
short WINAPI l_CrossOver(ctArray *a1, ctArray *a2, short pos);
short WINAPI l_CrossOverFill(ctArray *a1, ctArray *a2, ctArray *a3, short start, short stop);
short WINAPI l_Turn(ctArray *a1, short pos);
short WINAPI l_TurnFill(ctArray *a1, ctArray *a2, short start, short stop);
/* -------------------------------------------------------------------- */ 
/* ctcandle.h                                                           */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright(C) 1997 FM Labs */

//#pragma pack(1)

#define ctNONE      0
#define ctBLACKCNDL 1
#define ctWHITECNDL 2
#define ctDOJI      0
#define ctSMALL     1
#define ctMEDIUM    2
#define ctLARGE     3

struct ctCandles
{
 unsigned color:2;           /*ctNONE ctBLACKCNDL ctWHITECNDL */
 unsigned shape:2;           /*ctLARGE ctSMALL  ctDOJI  */
 unsigned topshadow:2;       /*ctLARGE ctMEDIUM ctSMALL */
 unsigned botshadow:2;       /*ctLARGE ctMEDIUM ctSMALL */
};

struct ctCandleInfo
{
 struct ctCandles *candle; 
 float  largeshadow;         /* Length pct to determine if shadow is ctLARGE */
 float  smallshadow;         /* Length pct to determine if shadow is ctSMALL */
 float  equalpct;            /* Tolerance level to determine equality */
};

short WINAPI l_CandleConstruct(ctBars *b1, struct ctCandleInfo *info);
void  WINAPI l_CandleFree(struct ctCandleInfo *info);
short WINAPI l_CandleFill(ctBars *b1, struct ctCandleInfo *info, ctArray *a2,
                          char *candlefunct, short start, short stop);
short WINAPI l_ApproxEqual(float f1, float f2, float pct);
float WINAPI l_BodyMidpoint(ctBars *b1, short pos);
short WINAPI l_BodyGapUp(ctBars *b1, short pos);
short WINAPI l_BodyGapDn(ctBars *b1, short pos);
short WINAPI l_WesternGapUp(ctBars *b1, short pos);
short WINAPI l_WesternGapDn(ctBars *b1, short pos);
short WINAPI l_AbandonedBabyBottom(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_AbandonedBabyTop(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_AdvanceBlock(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_BeltHoldLineBear(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_BeltHoldLineBull(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_CounterAttackBear(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_CounterAttackBull(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_DarkCloudCover(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_EngulfingLineBear(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_EngulfingLineBull(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_EveningDojiStar(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_EveningStar(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_Falling3Method(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_GravestoneDoji(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_Hammer(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_Hangingman(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_HaramiBlack(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_HaramiCrossBlack(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_HaramiCrossWhite(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_HaramiWhite(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_HighWave(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_InNeckLine(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_InvertedHammer(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_LongLeggedDoji(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_MatHoldPattern(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_MorningDojiStar(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_MorningStar(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_PiercingLine(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_Rising3Method(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_SeperatingLineBear(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_SeperatingLineBull(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_ShootingStar(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_SideBySideWhiteGapDn(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_SideBySideWhiteGapUp(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_StalledPattern(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_TasukiDownsideGap(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_TasukiUpsideGap(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_ThreeGapsUp(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_ThreeGapsDown(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_ThreeWhiteSoldiers(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_ThrustingLine(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_UpsideGap2Crows(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI l_CandleColor(struct ctCandleInfo *info, short pos);
short WINAPI l_CandleShape(struct ctCandleInfo *info, short pos);
short WINAPI l_CandleTopShadow(struct ctCandleInfo *info, short pos);
short WINAPI l_CandleBotShadow(struct ctCandleInfo *info, short pos);


short WINAPI n_InvertedHammer(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_Hammer(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_BullishBeltHold(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_BullishEngulfing(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_Harami(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_HaramiCross(ctBars *b1, struct ctCandleInfo *info, short pos); 
short WINAPI n_BullishCounterAttckLine(ctBars *b1, struct ctCandleInfo *info, short pos); 
short WINAPI n_PiercingLine(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_MorningStar(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_MorningDojiStar(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_AbandonedBaby(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_ThreeInsideUp(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_ThreeOutsideUp(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_UpsideGap2CrowsDown(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_BullishDojiStar(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_TweezersBottom(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_ThreeWhiteSoldier(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_AdvancedBlock(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_StalledPattern(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_UpsideGapTasuki(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_UpsideGap3Method(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_HangingMan(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_BearishBeltHold(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_BearishEngulfing(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_HaramiBlack(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_HaramiCrossBlack(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_BearishCounterAttckLine(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_DarkCrowsCover(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_EveningStar(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_EveningDojiStar(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_AbandonedBabyBlack(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_ThreeInsideDown(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_ThreeOutsideDown(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_UpsideGap2Crows(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_BearishDojiStar(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_TweezersTop(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_ThreeBlackCrows(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_IdenticalThreeCrows(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_DownsideGapTasuki(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_DownsideGap3Method(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_ShootingStar(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_isDowning(ctBars *b1, struct ctCandleInfo *info, short pos);
short WINAPI n_isUpGoing(ctBars *b1, struct ctCandleInfo *info, short pos);

/* -------------------------------------------------------------------- */ 
/* ctfin.h                                                              */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

#define ctANNUAL_PAYMENTS       1
#define ctSEMIANNUAL_PAYMENTS   2
#define ctQUARTERLY_PAYMENTS    4
#define ctMONTHLY_PAYMENTS     12
#define ctIDAILY_PAYMENTS      360

struct ctOption
{
 double price;
 double strike;
 short  type;
 unsigned short expiredate;
};

struct ctOptionSpreads
{
 double stockprice;
 struct ctOption buy[2];
 struct ctOption sell[2];
 double maxprofit;
 double riskreward;
 double riskamt;
 double breakevenup;
 double breakevendn;
 double pctbreakevenup;
 double pctbreakevendn;
};

struct ctTimeValue
{
 double principal;
 double rate;
 double periods;
 short  compound;
 char   filler[2];
};

struct ctTimeValueA
{
 double principal;
 double discrate;
 double numpayments;
};

struct ctBondInfo
{
 double facevalue;
 double price;
 double coupon;
 short  pymtsperyr;
 unsigned short maturitydate;
};

double WINAPI l_ApprxCumProbDist(double d1);
short  WINAPI l_BearCall(struct ctOptionSpreads *ops); /* Buy Hi strike Sell Lo strike */
short  WINAPI l_BearPut(struct ctOptionSpreads *ops); /* Buy Hi strike Sell Lo strike */
double WINAPI l_BlackScholes(struct ctOption *opt, double stockprice, double intrate, double variance, unsigned short asofdate);
double WINAPI l_BondAccrInt(double coupon, unsigned short lastpaydate, unsigned short asofdate, double daysinyr);
double WINAPI l_BondDuration(struct ctBondInfo *bond, double discrate, unsigned short asofdate);
double WINAPI l_BondDurationPriceChg(double price, double olddiscrate, double duration, double discrate);
double WINAPI l_BondPricing (struct ctBondInfo *bond, double discrate, unsigned short asofdate);
short  WINAPI l_BullCall(struct ctOptionSpreads *ops); /* Buy Lo strike Sell Hi strike */
short  WINAPI l_BullPut(struct ctOptionSpreads *ops); /* Buy Lo strike Sell Hi strike */
double WINAPI l_FutureValue (struct ctTimeValue *tvalue);
double WINAPI l_FutureValuePrinc (struct ctTimeValue *tvalue, double futurevalue);
double WINAPI l_FutureValueInt (struct ctTimeValue *tvalue, double futurevalue);
double WINAPI l_FutureValueAnnuity(struct ctTimeValue *tvalue);
short  WINAPI l_LongStraddle(struct ctOptionSpreads *ops);
short  WINAPI l_LongStrangle(struct ctOptionSpreads *ops);
double WINAPI l_PresentValue(struct ctTimeValue *tvalue);
double WINAPI l_PresentValueAnnuity(struct ctTimeValueA *tvaluea);
short  WINAPI l_OptionInit(struct ctOptionSpreads *ops);
short  WINAPI l_ShortStraddle(struct ctOptionSpreads *ops);
short  WINAPI l_ShortStrangle(struct ctOptionSpreads *ops);
double WINAPI l_TBillDiscYield (struct ctBondInfo *bond, unsigned short asofdate);
double WINAPI l_TBillBondEqYield(struct ctBondInfo *bond, unsigned short asofdate);
double WINAPI l_YieldtoMaturity(struct ctBondInfo *bond, unsigned short asofdate);
/* -------------------------------------------------------------------- */ 
/* cttrade.h                                                            */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

#define ctMARKET 1        /* at ind */
#define ctSTOP   2        /* at ind */
#define ctLIMIT  3        /* at ind */
#define ctDAY    1        /* time in force */
#define ctGTC    2        /* time in force */
#define ctCLOSE  1
#define ctOPEN   2
#define ctBUY    1
#define ctSELL   2
#define ctLONG   1
#define ctSHORT  2

/* Trade Graph Constants */
#define ctBO     1         /* buy to open   */
#define ctSO     2         /* sell to open  */
#define ctBC     4         /* buy to close  */
#define ctSC     8         /* sell to close */

struct ctOrderParms
{
 char     systemname[80];
 double   ticksize;        /* Tick increment for this issue */
 double   valueoftick;
 double   slippage;
 double   commission;
 double   margin;
 long     ordertimedelay;  /* number of seconds before At Market order can occur */
 short    reversal;        /* allow auto exit and re-entry */
 short    bestprice;       /* enter at open if better price */
 char     ttreport[80];    /* Name of Trade By Trade Report file or "\0" */
};

struct ctCurrent
{
 short    barnum;
 short    position;        /* Long Short Nothing                  */
 long     longvol;         /* Number of Shares or Contracts Long  */
 long     shortvol;        /* Number of Shares or Contracts Short */
 long     totalvolume;     /* Contract                            */
 double   avgentryprice;   /* Weigthed Avg of all open executions */
 unsigned short entrydate; /* Of Latest Trade */
 unsigned short entrytime; /* Of Latest Trade */
 short    longduration;
 short    shortduration;
 short    duration;        /* Number of bars in current position  */
 char     filler[2];   
};

struct ctOrder
{
 char     name[20];     /* Order Description     */
 char     fromname[20]; /* Used by Close Orders  */
 long     volume;       /* Volume                */
 short    type;         /* (O)pen (C)lose Cancel(X)           */
 short    buysell;      /* (B)uy (S)ell                       */
 double   reqprice;     /* Requested Price                    */
 short    atind;        /* At (M)arket, (S)top or (L)imit     */
                        /* (S)top = Next Tick higher for Buy  */
                        /* (S)top = Next Tick lower  for Sell */
                        /* (L)imit = or Better       for Buy  */
                        /* (L)imit = or Better       for Sell */
 short    timeinforce;  /* Day / GTC             */
 unsigned short date;   /* Order Date            */
 unsigned short time;   /* Order Time            */
 short    deletesw;     /* For Close Orders only */
 char     filler[2];   
};

struct ctExecution
{
 char     name[20];     /* Order Description */
 short    barnum;       /* Bar Number        */
 short    buysell;      /* Buy / Sell        */
 unsigned short date;   /* Order Date        */
 unsigned short time;   /* Order Time        */
 double   price;        /* Actual Price      */
 long     volume;       /* Volume            */
 long     remvolume;    /* Remaining Volume  */
};

struct ctPerformance
{
 double   netprofit;       /* NP */
 double   grossprofit;     /* GP */
 double   grossloss;       /* GL */
 long     trades;          /* NT */
 double   pctprofitable;   /* PP */
 short    win;             /* NW */
 short    loss;            /* NL */
 double   largestwin;      /* LW */
 double   largestloss;     /* LL */
 double   avgwin;          /* AW */
 double   avgloss;         /* AL */
 double   avgwintoavgloss; /* WL */
 double   avgtrade;        /* AT */
 short    maxconwin;       /* MW */
 short    maxconloss;      /* ML */
 double   avgwinstreak;    /* WS */
 double   avglosstreak;    /* LS */
 double   maxdrawdown;     /* MD */
 double   profitfactor;    /* PF */
 double   maxcontractsheld;/* MC */
 double   acctsizereqd;    /* AS */
 double   rtnonacct;       /* RA */
 double   prr;             /* PR */
 double   openpospl;
 short    winbars;
 short    lossbars;
 short    winstreak;
 short    lossstreak;
};

struct ctOrderSystem
{
 struct ctOrderParms  OrderParms;
 struct ctCurrent     Current;
 struct ctPerformance Long;
 struct ctPerformance Short;
 struct ctPerformance Net;
 struct ctOrder      *Order;
 struct ctExecution  *InExec;
 ctBars bars;
 ctArray tradegraph;
 short  OrderSize;
 short  OrderCnt;
 short  InExecSize;
 short  InExecCnt;
};

struct ctOptVar
{
 double   from;
 double   to;
 double   incr;
 long     count;
 double   value;
};

short WINAPI l_OrderSystemInit(struct ctOrderSystem *ctOS, char *systemname, ctBars *bars, short maxorders);
void  WINAPI l_OrderSystemReset(struct ctOrderSystem *ctOS);
void  WINAPI l_OrderSystemClose(struct ctOrderSystem *ctOS);
short WINAPI l_ProcessOrders(struct ctOrderSystem *ctOS);
short WINAPI l_BuyToOpen(struct ctOrderSystem *ctOS, char *name, long volume, double price, short atind, short timeinforce);
short WINAPI l_SellToOpen(struct ctOrderSystem *ctOS, char *name, long volume, double price, short atind, short timeinforce);
short WINAPI l_SellToClose(struct ctOrderSystem *ctOS, char *name, char *fromname, long volume, double price, short atind, short timeinforce);
short WINAPI l_BuyToClose(struct ctOrderSystem *ctOS, char *name, char *fromname, long volume, double price, short atind, short timeinforce);
short WINAPI l_CancelOrder(struct ctOrderSystem *ctOS, char *name);
short WINAPI l_PrintOrders(struct ctOrderSystem *ctOS, char *outfile, short append);
short WINAPI l_ProfitRpt(struct ctOrderSystem *ctOS, char *outfile, short append);
short WINAPI l_ProfitRpt1(struct ctOrderSystem *ctOS, char *outfile, short append, char *heading, char *fields);
long  WINAPI l_OptVarCount(struct ctOptVar *vars, short numvars);
short WINAPI l_OptVarValue(struct ctOptVar *vars, short numvars, long iteration);
/* -------------------------------------------------------------------- */ 
/* ctfile.h                                                             */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

short WINAPI l_FILEtype(char *type);
long  WINAPI l_FILEfindrec(char *type, char *path, char *name, unsigned short date, unsigned short time,
		         unsigned short *actualdate, unsigned short *actualtime);
short WINAPI l_FILElist(char *type, char *path, char *outfile, short append);
short WINAPI l_FILEprintinfo(ctBars *b1, char *outfile, short append);
short WINAPI l_FILEread(char *type, ctBars *b1, char *path, char *name, long start, short cnt, short allocate);
/* -------------------------------------------------------------------- */ 
/* ctcsi.h                                                              */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

//#pragma pack(1)

struct ctCSImrec                /* record of master file            */
{
 char     id[4];                /* CSI ID number                    */
 char     name[20];             /* Item name                        */
 char     type;                 /* File type (D,W,M,S)              */
 char     dm[2];                /* Delivery month                   */
 char     dy[2];                /* Delivery year                    */
 char     cf[2];                /* Conversion Factor                */
 char     pu[5];                /* Pricing Unit                     */
 char     csym[2];              /* Commodity Symbol if a commodity  */
 char     csflag;               /* C if commodity, S if stock       */
 char     oflag;                /* N=normal, P=put, C=call          */
 char     sprice[5];            /* Striking price if an option      */
 char     ssym[6];              /* Stock symbol if a stock          */
 char     dflag;                /* 0=record active, 1=record avail  */
 char     sfed[2];              /* Short file end date (not used)   */
 char     expan[10];            /* reserved for future expansion    */
};

struct ctCSIrec                 /* record of csi data file          */
{
 float    date;                 /* YYMMDD format                    */
 char     dow;                  /* day of week (1-5 or 9)           */
 unsigned open;
 unsigned high;
 unsigned low;
 unsigned close;
 unsigned noon;
 unsigned cash;
 unsigned tvol;                 /* total volume                     */
 unsigned char tvol1;           /* tvol multiplier * 65535          */
 unsigned toi;                  /* total open interest              */
 unsigned char toi1;            /* toi multiplier * 65535           */
 unsigned cvol;                 /* contract level volume            */
 unsigned char cvol1;           /* cvol multiplier * 65535          */
 unsigned coi;                  /* contract level open interest     */
 unsigned char coi1;            /* coi multiplier * 65535           */
 unsigned exop:2;               /* extension bits for open          */
 unsigned exhi:2;               /* extension bits for high          */
 unsigned exlo:2;               /* extension bits for low           */
 unsigned excl:2;               /* extension bits for close         */
 unsigned exnoon:2;             /* extension bits for noon          */
 unsigned excash:2;             /* extension bits for cash          */
 unsigned exunused1:2;          /* unused extension bits            */
 unsigned exunused2:2;          /* unused extension bits            */
 char     expan;                /* reserved for future expansion    */
};

struct ctCSIhdr                 /* header of csi data file          */
{
 float    ferp;                 /* last physical record             */
 float    mdp;                  /* last record updated              */
 float    hh;                   /* highest high                     */
 float    ll;                   /* lowest low                       */
 float    fpdof;                /* first physical date on file      */
 float    lpdof;                /* last physical date on file       */
 char     filler[8];
};

long  WINAPI l_CSIfindrec(char *path, char *name, unsigned short date, unsigned short *actualdate);
short WINAPI l_CSIlist(char *path, char *outfile, short append);
short WINAPI l_CSIread(ctBars *b1, char *path, char *name, long start, short cnt, short allocate);
/* -------------------------------------------------------------------- */ 
/* ctct.h                                                               */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

//#pragma pack(1)

struct ctCTdathdr
{
 short    maxrecs;
 short    lastrec;
};

struct ctCTmasthdr
{
 short    numentries;
 short    nextfilenum;
 char     filler[49];
};

struct ctCTdata
{
 unsigned char filenbr;   /* File number          */
 short    filetype;       /* File type CT resrvd  */
 char     reclen;         /* DAT file rec length  */
 char     numfields;      /* DAT file num fields  */
 char     filler1[2];
 char     name[16];       /* Name of issue        */
 char     filler2;
 char     ctv28flag;      /* CT version 2.8 flag  */
 float    begindate;      /* First date on file   */
 float    enddate;        /* Last date on file    */
 char     dataformat;     /* I D W M Q Y          */
 short    idatime;        /* Base time            */
 char     datasvcuse[16]; /* For data service use */
 char     filler3;
};

struct ctCTdir
{
 unsigned char filenbr;   /* File number not used for dir */
 char     filler1[6];
 char     name[8];        /* DOS directory name           */
 char     filler2[3];
 char     directory[5];   /* <DIR>                        */
 char     filler3[13];
 char     datasvcuse[16];
 char     filler4;
};


long  WINAPI l_CTfindrec(char *path, char *name, unsigned short date, unsigned short time,
              unsigned short *actualdate, unsigned short *actualtime);
short WINAPI l_CTlist(char *path, char *outfile, short append);
short WINAPI l_CTread(ctBars *b1, char *path, char *name, long start, short cnt, short allocate);
/* -------------------------------------------------------------------- */ 
/* cttxt.h                                                              */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

struct ctTXTinfo
{
 char  delimit[16];    /* Character(s) that delimit the columns, space = _ */
 short skip;           /* Number of lines to skip at begining of file      */
 short dt, ti;         /* Column numbers (or zero if field is not in file) */
 short op, hi, lo, cl; /* Column numbers (or zero if field is not in file) */
 short vol, oi;        /* Column numbers (or zero if field is not in file) */
 short dtformat;       /* Format of date */
 short tiformat;       /* Format of time */
 short filler;
};

long WINAPI l_TXTfindrec(char *path, char *name, struct ctTXTinfo *info,
		        unsigned short date, unsigned short time,
		        unsigned short *actualdate, unsigned short *actualtime);
short WINAPI l_TXTread(ctBars *b1, char *path, char *name, struct ctTXTinfo *info,
		        long start, short cnt, short allocate);
short WINAPI l_TXTreadinfo(char *path, char *infile, struct ctTXTinfo *info);
short WINAPI l_TXTwriteinfo(char *path, char *outfile, struct ctTXTinfo *info);
/* -------------------------------------------------------------------- */ 
/* ctdj.h                                                               */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

//#pragma pack(1)

#define ctDAYSFROM1920 21549

struct ctDJinfohdr
{
 char  filler1[45];
 short reccnt;
 char  filler2[24];
};

struct ctDJinfo
{
 char     symbol[8];  /* symbol used by the Market Analyzer PLUS                   */
 char     equitytype; /* equity type: S stock, M mutual fund, O option, B bond,    */
                      /*              T treasury, W Tradeline, X DJ index,         */
                      /*              V commodity, Y user index, Z other           */
 char     decimals;   /* number of decimal places displayed                        */
                      /*        ASCII 46=0 47=1 40=2 49=3 50=4 places              */
 char     content;    /* data content '1' close                                    */
                      /*              '2' close,volume                             */
                      /*              '3' high,low,close                           */
                      /*       '0' or '4' high,low,close,volume                    */
                      /*              '5' high,low,close,open                      */
                      /*              '6' high,low,close,open,open interest        */
                      /*              '7' high,low,close,open,volume               */
                      /*              '8' high,low,close,open,open interest,volume */
                      /*              '9' all contracts volume and open interest   */
 char     filler1;    /* reserved                                                  */
 unsigned begindt;    /* Julian beginning date (0=Jan 1, 1920)                     */
 unsigned enddt;      /* Julian ending date for the data in the historical file    */
 float    lastclose;  /* last closing price as real number                         */
 char     company[15];/* company name ASCII                                        */
 char     filler2[10];/* reserved                                                  */
 short    reccnt;     /* integer number of units of data in the historical file    */
 char     datatype;   /* data type: D daily, W weekly, M monthly                   */
 char     tag[3];     /* 3 byte tag ASCII                                          */
 char     filler3[8]; /* reserved                                                  */
 char     avail[2];   /* available for use                                         */
 char     cusip[9];   /* Cusip   ASCII                                             */
 char     filler4;    /* reserved                                                  */
};

struct ctDJqwik
{
 char     symbol[8];  /* program ticker symbol                                     */
 char     datatype;   /* data type: D daily, W weekly, M monthly                   */
 char     equitytype; /* equity type: S stock, M mutual fund, O option, B bond,    */
                      /*              T treasury, W Tradeline, X DJ index,         */
                      /*              V commodity, Y user index, Z other           */
 short    recno;      /* record in MAPINFO.DAT that belongs to this ticker         */
};

struct ctDJdc
{
 short dt;
 float cl;
};
struct ctDJdvc
{
 short dt;
 long vol;
 float cl;
};
struct ctDJdhlc
{
 short dt;
 float hi, lo, cl;
};
struct ctDJdvhlc
{
 short dt;
 long vol;
 float hi, lo, cl;
};
struct ctDJdhlco
{
 short dt;
 float hi, lo, cl, op;
};
struct ctDJdihlco
{
 short dt;
 long oi;
 float hi, lo, cl, op;
};
struct ctDJdvhlco
{
 short dt;
 long vol;
 float hi, lo, cl, op;
};
struct ctDJdvihlco
{
 short dt;
 long oi;
 long vol;
 float hi, lo, cl, op;
};
struct ctDJdiv
{
 short dt;
 long oi;
 long vol;
};

short WINAPI l_DJlist(char *path, char *outfile, short append);
short WINAPI l_DJread(ctBars *b1, char *path, char *name, long start, short cnt, short allocate);
/* -------------------------------------------------------------------- */ 
/* ctfs.h                                                               */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

//#pragma pack(1)

struct ctFScontractspec
{
 char     filler[8];
};

struct ctFScontractinfo
{
 char     exchange;
 char     fraction;
 char     multiplier;
 unsigned fluctuation;
 short    factor;
};

struct ctFSinfo  /* header */
{
 short    version;
 unsigned dataoffset;
 unsigned contractoffset;
 unsigned char numcontracts;
 unsigned studyoffset;
 unsigned char numstudies;
 unsigned char studytype;
 unsigned char recordlen;
 char     filename[13];
 char     filetype;
 unsigned char minutes;
 unsigned char fraction;
 char     power;
 char     filler1;
 unsigned voifileexists:1;
 unsigned bit32values:1;
 unsigned signeddata:1;
 unsigned autoupdatenorm:1;
 unsigned autoupdatevoi:1;
 unsigned archived:1;
 short    timezone;
 unsigned expirationdate;
 unsigned char daymap1;
 unsigned start1;
 unsigned end1;
 unsigned char daymap2;
 unsigned start2;
 unsigned end2;
 char     filler2[10];
 char     symbolname[4];
 char     symboltype;
 struct ctFScontractspec contractspec;
 struct ctFScontractinfo contractinfo1;
 struct ctFScontractinfo contractinfo2;
 struct ctFScontractinfo contractinfo3;
 char     desc[28];
};

struct ctFSdaily16
{
 unsigned short date, open, high, low, close;
};

struct ctFSvar16
{
 unsigned short date, time, open, high, low, close;
};

struct ctFStick16
{
 unsigned short date, time, close;
};

struct ctFSvoi16
{
 unsigned short date, vol, oi;
 unsigned filler1:1;
 unsigned volexp:2;
 unsigned oiexp:2;
 unsigned filler2:3;
};

struct ctFSdaily32
{
 unsigned short date;
 unsigned long open, high, low, close;
};

struct ctFSvar32
{
 unsigned short date, time;
 unsigned long open, high, low, close;
};

struct ctFStick32
{
 unsigned short date, time;
 unsigned long close;
};

struct ctFSvoi32
{
 unsigned short date;
 unsigned long vol, oi;
};

struct ctFStablehdr
{
 unsigned version;
 unsigned recordlen;
};

struct ctFSfracs
{
 char     shortname[8];
 char     longname[32];
 unsigned xmitdenom;
 unsigned dispdenom;
 unsigned char nondec;
 unsigned char bitmap;
 unsigned divide;
 unsigned multiply;
 unsigned subtract;
 unsigned add;
 unsigned xmitpower;
 unsigned disppower;
};

short WINAPI l_FSread(ctBars *b1, char *path, char *name, long start, short cnt, short allocate);
/* -------------------------------------------------------------------- */ 
/* cttc.h                                                               */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

struct ctTCrec
{
 float    dt;
 float    hi;
 float    lo;
 float    cl;
 float    vol;
 float    op;
};

short WINAPI l_TClist(char *path, char *outfile, short append);
short WINAPI l_TCread(ctBars * b1, char *path, char *name, long start, short cnt, short allocate);
/* -------------------------------------------------------------------- */ 
/* cttt.h                                                               */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

//#pragma pack(1)

struct ctTTrec
{
 unsigned short dt;
 unsigned short op;
 unsigned short hi;
 unsigned short lo;
 unsigned short cl;
 unsigned char  vol[3];
 unsigned char  oi[3];
};

long  WINAPI l_TTfindrec(char *path, char *name, unsigned short date, unsigned short *actualdate);
short WINAPI l_TTread(ctBars *b1, char *path, char *name, long start, short cnt, short allocate);
/* -------------------------------------------------------------------- */ 
/* ctchart.h                                                            */ 
/* -------------------------------------------------------------------- */ 
/* Technical Analysis Programmer's Toolkit - Copyright(C) 1997 FM Labs */

#define ctCHART_HILOCL    1
#define ctCHART_OPHILOCL  2
#define ctCHART_CANDLE    3
#define ctCHART_HIST      4
#define ctCHART_LINE      5
#define ctCHART_POINT     6
#define ctCHART_FILL      7
#define ctCHART_TRADES    8

#define ctCHART_DEFINED   12345
#define ctCHART_SUBCNTMAX 8 /* MUST be multiple of 4 to preserve 32bit structure alignment */

struct ctChartLine
{
 ctArray  a1;
 ctBars   b1;
 short    chartnum;
 short    type;
 float    ystart;
 float    ystop;
 long     color1;
 long     color2;
 long     style;
};

struct ctChartGrids
{
 short    chartnum;
 short    filler;
 float    value;
 float    ystart;
 float    ystop;
 long     color;
 long     style;
};

 struct ctChartScreen
{
 short    defined;           /* == ctCHART_DEFINED */
 short    scalepos;          /* 0=off 1=left 2=right */
 short    datestyle;         /* 0=off 1=auto 2=J 3=Jan 4=January 5=31Jan */
 short    vgrid;             /* linestyle, ctNULLSTYLE=off */
 short    hgrid;             /* linestyle, ctNULLSTYLE=off */
 short    xwidth;
 short    xstart;
 short    xstop;
 short    linecnt;
 short    gridcnt;
 short    linemax;
 short    gridmax;
 short    x1;
 short    y1;
 short    x2;
 short    y2;
 char     heading[160];
 char     footing[160];
 long     fgdcolor;
 long     bgdcolor;
 long     numcolor;
 long     linecolor;
 long     gridcolor;
 struct ctChartLine *chartline;
 struct ctChartGrids *chartgrid;
 short    subchartsize[ctCHART_SUBCNTMAX];
};

struct ctChartInfo
{
 short    xmax;
 short    xscale;
 short    top;
 short    bottom;
 short    left;
 short    right;
 short    scaleleft;
 short    scaleright;
 short    dateleft;
 short    subtop[ctCHART_SUBCNTMAX + 1];
 float    subystart[ctCHART_SUBCNTMAX];
 float    subystop[ctCHART_SUBCNTMAX];
 short    fontwidth;
 short    fontheight;
 ctBars   masterbars;
};

#ifdef _WINDOWS

#define ctSOLID          0
#define ctDASH           1
#define ctDOT            2
#define ctDASHDOT        3
#define ctDASHDOTDOT     4
#define ctNULLSTYLE      5
#define ctBLACK          0x0
#define ctBLUE           0x800000
#define ctGREEN          0x008000
#define ctCYAN           0x808000
#define ctRED            0x000080
#define ctMAGENTA        0x800080
#define ctBROWN          0x008080
#define ctWHITE          0xC0C0C0
#define ctGRAY           0xC0C0C0
#define ctGREY           0x808080
#define ctBRIGHTBLUE     0xFF0000
#define ctBRIGHTGREEN    0x00FF00
#define ctBRIGHTCYAN     0xFFFF00
#define ctBRIGHTRED      0x0000FF
#define ctBRIGHTMAGENTA  0xFF00FF
#define ctBRIGHTBROWN    0x00FFFF
#define ctYELLOW         0x00FFFF
#define ctBRIGHTWHITE    0xFFFFFF

short WINAPI l_Chart(HDC hdc, ctArray *pa1, ctBars *pb1, short xstart, short xstop, short xwidth,
       		   float ystart, float ystop, short x1, short y1, short x2, short y2,
		         short type, long color1, long color2, long style);
short WINAPI l_ChartGrid(HDC hdc, float level, float ystart, float ystop,
       		   short x1, short y1, short x2, short y2, long color, long style);
short WINAPI l_ChartScreenInit(struct ctChartScreen *screen, short linemax, short gridmax);
void  WINAPI l_ChartScreenClose(struct ctChartScreen *screen);
short WINAPI l_ChartLineDef(struct ctChartScreen *screen, short chartnum,
		         ctArray *a1, ctBars *b1, float ystart, float ystop,
		         short type, long color1, long color2, long style);
short WINAPI l_ChartGridDef(struct ctChartScreen *screen, short chartnum, float value,
		         float ystart, float ystop, long color, long style);
short WINAPI l_ChartSetup(HDC hdc, struct ctChartScreen *screen, struct ctChartInfo *info);
short WINAPI l_ChartPaint(HDC hdc, struct ctChartScreen *screen, struct ctChartInfo *info);
short WINAPI l_ChartDraw(struct ctChartScreen *screen);
short WINAPI l_ChartPosition(struct ctChartScreen *screen, struct ctChartInfo *info,
			     short x1, short y1, short *barnum, short *barpos, short *subchart, float *yvalue);

#else /* _WINDOWS not defined */

#define ctSOLID          0xFFFF
#define ctDASH           0xF0F0
#define ctDOT            0x5555
#define ctDASHDOT        0xF6F6
#define ctDASHDOTDOT     0xDADA
#define ctNULLSTYLE       0
#define ctBLACK           0
#define ctBLUE            1
#define ctGREEN           2
#define ctCYAN            3
#define ctRED             4
#define ctMAGENTA         5
#define ctBROWN           6
#define ctWHITE           7
#define ctGRAY            8
#define ctGREY            8
#define ctBRIGHTBLUE      9
#define ctBRIGHTGREEN    10
#define ctBRIGHTCYAN     11
#define ctBRIGHTRED      12
#define ctBRIGHTMAGENTA  13
#define ctBRIGHTBROWN    14
#define ctYELLOW         14
#define ctBRIGHTWHITE    15

extern char *ctChartPath;

short l_Chart(ctArray *pa1, ctBars *pb1, short xstart, short xstop, short xwidth,
        float ystart, float ystop, short x1, short y1, short x2, short y2,
        short type, long color1, long color2, long style);
short l_ChartGrid(float level, float ystart, float ystop,
        short x1, short y1, short x2, short y2, long color, long style);
short l_ChartScreenInit(struct ctChartScreen *screen, short linemax, short gridmax);
void  l_ChartScreenClose(struct ctChartScreen *screen);
short l_ChartLineDef(struct ctChartScreen *screen, short chartnum,
        ctArray *a1, ctBars *b1, float ystart, float ystop,
        short type, long color1, long color2, long style);
short l_ChartGridDef(struct ctChartScreen *screen, short chartnum, float value,
        float ystart, float ystop, long color, long style);
short l_ChartSetup(struct ctChartScreen *screen, struct ctChartInfo *info);
short l_ChartPaint(struct ctChartScreen *screen, struct ctChartInfo *info);
short l_ChartDraw(struct ctChartScreen *screen);
short l_ChartPosition(struct ctChartScreen *screen, struct ctChartInfo *info,
		      short x1, short y1, short *barnum, short *barpos, short *subchart, float *yvalue);
#endif


/* ctChart parameter quick reference                 */
/*                                                   */
/* type      array   bars    color1  color2  style   */
/* ----      -----   ----    ------  ------  -----   */
/* HILOCL    n/a     req'd   lines   n/a     n/a     */
/* OPHILOCL  n/a     req'd   lines   n/a     n/a     */
/* CANDLE    n/a     req'd   outline upcolor dncolor */
/* LINE      req'd   n/a     line    width   style   */
/* HIST      req'd   n/a     fgd     bgd     pattern */
/* POINT     req'd   n/a     point   n/a     n/a     */
/* FILL      req'd   n/a     fgd     bgd     pattern */
/* TRADES    n/a     req'd   buy     sell    close   */

#endif // __CTTKIT_H__