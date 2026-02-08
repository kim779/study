/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cttkit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


short WINAPI l_CandleConstruct(ctBars *b1, struct ctCandleInfo *info)
{
	short    pos;
	float    len, realbodytop, realbodybot;
	double   acr = 0; /* AverageCandleRange */
	
	if((info->candle = (struct ctCandles *)l_Malloc(b1->size * sizeof(struct ctCandles))) == NULL)
		return(ctErrMalloc);
	
	for (pos = 0; pos < b1->size; pos++)
		acr += fabs(ctArrayItem(b1->op, pos) - ctArrayItem(b1->cl, pos));
	acr /= b1->size;
	for (pos = 0; pos < b1->size; pos++)
	{
		if (ctArrayItem(b1->op, pos) > ctArrayItem(b1->cl, pos))
			info->candle[pos].color = ctBLACKCNDL;
		else if (ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos))
			info->candle[pos].color = ctWHITECNDL;
		else
			info->candle[pos].color = ctNONE;
		len = (float)fabs(ctArrayItem(b1->op, pos) - ctArrayItem(b1->cl, pos));
		realbodybot = min(ctArrayItem(b1->op, pos), ctArrayItem(b1->cl, pos));
		realbodytop = max(ctArrayItem(b1->op, pos), ctArrayItem(b1->cl, pos));
		if (len > acr)
			info->candle[pos].shape = ctLARGE;
		else if (len > realbodybot * info->equalpct)
			info->candle[pos].shape = ctSMALL;
		else
			info->candle[pos].shape = ctDOJI;
		if (info->candle[pos].color == ctNONE)
			len = info->equalpct * realbodytop;
		if (ctArrayItem(b1->hi, pos) - realbodytop > len * info->largeshadow)
			info->candle[pos].topshadow = ctLARGE;
		else if (ctArrayItem(b1->hi, pos) - realbodytop < len * info->smallshadow)
			info->candle[pos].topshadow = ctSMALL;
		else
			info->candle[pos].topshadow = ctMEDIUM;
		if (realbodybot - ctArrayItem(b1->lo, pos) > len * info->largeshadow)
			info->candle[pos].botshadow = ctLARGE;
		else if (realbodybot - ctArrayItem(b1->lo, pos) < len * info->smallshadow)
			info->candle[pos].botshadow = ctSMALL;
		else
			info->candle[pos].botshadow = ctMEDIUM;
	}
	return (0);
}

void WINAPI l_CandleFree(struct ctCandleInfo *info)
{
	l_Free(info->candle);
	return;
}

short WINAPI l_CandleFill(ctBars *b1, struct ctCandleInfo *info, ctArray *a2,
						  char *candlefunct, short start, short stop)
{
	short    i1, pos, cnt = 0;
	char    *formations[] = 
	{
		"l_AbandonedBabyBottom" ,
			"l_AbandonedBabyTop"    ,
			"l_AdvanceBlock"        ,
			"l_BeltHoldLineBear"    ,
			"l_BeltHoldLineBull"    ,
			"l_CounterAttackBear"   ,
			"l_CounterAttackBull"   ,
			"l_DarkCloudCover"      ,
			"l_EngulfingLineBear"   ,
			"l_EngulfingLineBull"   ,
			"l_EveningDojiStar"     ,
			"l_EveningStar"         ,
			"l_Falling3Method"      ,
			"l_GravestoneDoji"      ,
			"l_Hammer"              ,
			"l_Hangingman"          ,
			"l_HaramiBlack"         ,
			"l_HaramiCrossBlack"    ,
			"l_HaramiCrossWhite"    ,
			"l_HaramiWhite"         ,
			"l_HighWave"            ,
			"l_InNeckLine"          ,
			"l_InvertedHammer"      ,
			"l_LongLeggedDoji"      ,
			"l_MatHoldPattern"      ,
			"l_MorningDojiStar"     ,
			"l_MorningStar"         ,
			"l_PiercingLine"        ,
			"l_Rising3Method"       ,
			"l_SeperatingLineBear"  ,
			"l_SeperatingLineBull"  ,
			"l_ShootingStar"        ,
			"l_SideBySideWhiteGapDn",
			"l_SideBySideWhiteGapUp",
			"l_StalledPattern"      ,
			"l_TasukiDownsideGap"   ,
			"l_TasukiUpsideGap"     ,
			"l_ThreeGapsUp"         ,
			"l_ThreeGapsDown"       ,
			"l_ThreeWhiteSoldiers"  ,
			"l_ThrustingLine"       ,
			"l_UpsideGap2Crows"     ,
			""};
		
		stop = min(stop, min(b1->size - 1, a2->size - 1));
		for (i1 = 0; formations[i1][0]; i1++)
			if (!_stricmp(candlefunct, formations[i1]))
				break; 
			switch (i1)
			{
			case 0: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_AbandonedBabyBottom(b1, info, pos)) != ctFALSE);
				break;
			case 1: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_AbandonedBabyTop(b1, info, pos)) != ctFALSE);
				break;
			case 2: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_AdvanceBlock(b1, info, pos)) != ctFALSE);
				break;
			case 3: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_BeltHoldLineBear(b1, info, pos)) != ctFALSE);
				break;
			case 4: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_BeltHoldLineBull(b1, info, pos)) != ctFALSE);
				break;
			case 5: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_CounterAttackBear(b1, info, pos)) != ctFALSE);
				break;
			case 6: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_CounterAttackBull(b1, info, pos)) != ctFALSE);
				break;
			case 7: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_DarkCloudCover(b1, info, pos)) != ctFALSE);
				break;
			case 8: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_EngulfingLineBear(b1, info, pos)) != ctFALSE);
				break;
			case 9: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_EngulfingLineBull(b1, info, pos)) != ctFALSE);
				break;
			case 10: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_EveningDojiStar(b1, info, pos)) != ctFALSE);
				break;
			case 11: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_EveningStar(b1, info, pos)) != ctFALSE);
				break;
			case 12: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_Falling3Method(b1, info, pos)) != ctFALSE);
				break;
			case 13: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_GravestoneDoji(b1, info, pos)) != ctFALSE);
				break;
			case 14: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_Hammer(b1, info, pos)) != ctFALSE);
				break;
			case 15: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_Hangingman(b1, info, pos)) != ctFALSE);
				break;
			case 16: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_HaramiBlack(b1, info, pos)) != ctFALSE);
				break;
			case 17: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_HaramiCrossBlack(b1, info, pos)) != ctFALSE);
				break;
			case 18: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_HaramiCrossWhite(b1, info, pos)) != ctFALSE);
				break;
			case 19: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_HaramiWhite(b1, info, pos)) != ctFALSE);
				break;
			case 20: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_HighWave(b1, info, pos)) != ctFALSE);
				break;
			case 21: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_InNeckLine(b1, info, pos)) != ctFALSE);
				break;
			case 22: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_InvertedHammer(b1, info, pos)) != ctFALSE);
				break;
			case 23: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_LongLeggedDoji(b1, info, pos)) != ctFALSE);
				break;
			case 24: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_MatHoldPattern(b1, info, pos)) != ctFALSE);
				break;
			case 25: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_MorningDojiStar(b1, info, pos)) != ctFALSE);
				break;
			case 26: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_MorningStar(b1, info, pos)) != ctFALSE);
				break;
			case 27: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_PiercingLine(b1, info, pos)) != ctFALSE);
				break;
			case 28: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_Rising3Method(b1, info, pos)) != ctFALSE);
				break;
			case 29: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_SeperatingLineBear(b1, info, pos)) != ctFALSE);
				break;
			case 30: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_SeperatingLineBull(b1, info, pos)) != ctFALSE);
				break;
			case 31: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_ShootingStar(b1, info, pos)) != ctFALSE);
				break;
			case 32: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_SideBySideWhiteGapDn(b1, info, pos)) != ctFALSE);
				break;
			case 33: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_SideBySideWhiteGapUp(b1, info, pos)) != ctFALSE);
				break;
			case 34: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_StalledPattern(b1, info, pos)) != ctFALSE);
				break;
			case 35: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_TasukiDownsideGap(b1, info, pos)) != ctFALSE);
				break;
			case 36: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_TasukiUpsideGap(b1, info, pos)) != ctFALSE);
				break;
			case 37: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_ThreeGapsUp(b1, info, pos)) != ctFALSE);
				break;
			case 38: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_ThreeGapsDown(b1, info, pos)) != ctFALSE);
				break;
			case 39: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_ThreeWhiteSoldiers(b1, info, pos)) != ctFALSE);
				break;
			case 40: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_ThrustingLine(b1, info, pos)) != ctFALSE);
				break;
			case 41: 
				for(pos = start; pos <= stop; pos++)
					cnt += ((ctArrayItemP(a2, pos) = l_UpsideGap2Crows(b1, info, pos)) != ctFALSE);
				break;
			default:
				return (ctErrInvalidParm);
 } 
 return (cnt);
}    

short WINAPI l_ApproxEqual(float f1, float f2, float pct)
{
	return (fabs(f1 - f2) < f1 * pct);
}

float WINAPI l_BodyMidpoint(ctBars *b1, short pos)
{
	return ((ctArrayItem(b1->op, pos) + ctArrayItem(b1->cl, pos)) / 2);
}

short WINAPI l_BodyGapUp(ctBars *b1, short pos)
{
	return (min(ctArrayItem(b1->op, pos), ctArrayItem(b1->cl, pos)) >
		max(ctArrayItem(b1->op, (pos) - 1), ctArrayItem(b1->cl, (pos - 1))));
}

short WINAPI l_BodyGapDn(ctBars *b1, short pos)
{
	return (max(ctArrayItem(b1->op, pos), ctArrayItem(b1->cl, pos)) <
		min(ctArrayItem(b1->op, (pos) - 1), ctArrayItem(b1->cl, (pos - 1))));
}

short WINAPI l_WesternGapUp(ctBars *b1, short pos)
{
	return (ctArrayItem(b1->lo, pos) > ctArrayItem(b1->hi, (pos - 1)));
}

short WINAPI l_WesternGapDn(ctBars *b1, short pos)
{
	return (ctArrayItem(b1->hi, pos) < ctArrayItem(b1->lo, (pos - 1)));
}



short WINAPI l_AbandonedBabyBottom(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 2)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 2].color == ctBLACKCNDL &&
		info->candle[pos - 1].shape == ctDOJI &&
		info->candle[pos - 2].shape == ctLARGE &&
		l_WesternGapDn(b1, pos - 1) &&
		l_WesternGapUp(b1, pos) &&
		ctArrayItem(b1->cl, pos) > l_BodyMidpoint(b1, pos - 2))
		return (ctTRUE);
	else
		return (ctFALSE);
}


short WINAPI l_AbandonedBabyTop(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 2)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos - 2].color == ctWHITECNDL &&
		info->candle[pos - 1].shape == ctDOJI &&
		info->candle[pos - 2].shape == ctLARGE &&
		l_WesternGapUp(b1, pos - 1) &&
		l_WesternGapDn(b1, pos) &&
		ctArrayItem(b1->cl, pos) < l_BodyMidpoint(b1, pos - 2))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_AdvanceBlock(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	float    len0, len1, len2;
	
	if (pos < 2)
		return (ctFALSE);
	len0 = ctArrayItem(b1->cl, pos) - ctArrayItem(b1->op, pos);
	len1 = ctArrayItem(b1->cl, pos - 1) - ctArrayItem(b1->op, pos - 1);
	len2 = ctArrayItem(b1->cl, pos - 2) - ctArrayItem(b1->op, pos - 2);
	
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos - 2].color == ctWHITECNDL &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->cl, pos - 1) > ctArrayItem(b1->cl, pos - 2) &&
		((len2 > len1 && len1 > len0) ||
		(info->candle[pos - 2].topshadow != ctLARGE &&
		info->candle[pos].topshadow == ctLARGE && info->candle[pos - 1].topshadow == ctLARGE)))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_BeltHoldLineBull(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape == ctLARGE &&
		ctArrayItem(b1->op, pos) == ctArrayItem(b1->lo, pos))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_BeltHoldLineBear(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos].shape == ctLARGE &&
		ctArrayItem(b1->op, pos) == ctArrayItem(b1->hi, pos))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_CounterAttackBear(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos - 1].shape != ctDOJI &&
		l_ApproxEqual(ctArrayItem(b1->cl, pos), ctArrayItem(b1->cl, pos - 1), info->equalpct) &&
		ctArrayItem(b1->op, pos) - ctArrayItem(b1->cl, pos - 1) >=
        ctArrayItem(b1->cl, pos - 1) - ctArrayItem(b1->op, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_CounterAttackBull(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctBLACKCNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos - 1].shape != ctDOJI &&
		l_ApproxEqual(ctArrayItem(b1->cl, pos), ctArrayItem(b1->cl, pos - 1), info->equalpct) &&
		ctArrayItem(b1->cl, pos - 1) - ctArrayItem(b1->op, pos) >=
        ctArrayItem(b1->op, pos - 1) - ctArrayItem(b1->cl, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_DarkCloudCover(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos - 1].shape == ctLARGE &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->hi, pos - 1) &&
		ctArrayItem(b1->cl, pos) < l_BodyMidpoint(b1, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_EngulfingLineBear(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos].shape == ctLARGE &&
		info->candle[pos - 1].shape == ctSMALL &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->op, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_EngulfingLineBull(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctBLACKCNDL &&
		info->candle[pos].shape == ctLARGE &&
		info->candle[pos - 1].shape == ctSMALL &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->op, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_EveningDojiStar(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 2)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos - 2].color == ctWHITECNDL &&
		info->candle[pos - 1].shape == ctDOJI &&
		info->candle[pos - 2].shape == ctLARGE &&
		l_BodyGapUp(b1, pos - 1) &&
		ctArrayItem(b1->cl, pos) < l_BodyMidpoint(b1, pos - 2))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_EveningStar(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 2)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos - 2].color == ctWHITECNDL &&
		info->candle[pos - 1].shape == ctSMALL &&
		info->candle[pos - 2].shape == ctLARGE &&
		l_BodyGapUp(b1, pos - 1) &&
		ctArrayItem(b1->cl, pos) < l_BodyMidpoint(b1, pos - 2))
		return (ctTRUE);
	else
		return (ctFALSE);
}


short WINAPI l_Falling3Method(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos - 2].color == ctWHITECNDL &&
		info->candle[pos - 3].color == ctWHITECNDL &&
		info->candle[pos - 4].color == ctBLACKCNDL &&
		info->candle[pos].shape == ctLARGE &&
		info->candle[pos - 1].shape == ctSMALL &&
		info->candle[pos - 2].shape == ctSMALL &&
		info->candle[pos - 3].shape == ctSMALL &&
		info->candle[pos - 4].shape == ctLARGE &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->cl, pos - 4) &&
		ctArrayItem(b1->cl, pos - 4) < ctArrayItem(b1->op, pos - 3) &&
		ctArrayItem(b1->cl, pos - 3) < ctArrayItem(b1->cl, pos - 2) &&
		ctArrayItem(b1->cl, pos - 2) < ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->cl, pos - 1) <= max(ctArrayItem(b1->op, pos), ctArrayItem(b1->op, pos - 4)))
		return (ctTRUE);
	else
		return (ctFALSE);
}


short WINAPI l_GravestoneDoji(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (info->candle[pos].shape == ctDOJI &&
		ctArrayItem(b1->lo, pos) == min(ctArrayItem(b1->op, pos), ctArrayItem(b1->cl, pos)))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_Hammer(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos].shape != ctLARGE &&
		info->candle[pos].topshadow == ctSMALL &&
		info->candle[pos].botshadow == ctLARGE)
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_Hangingman(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape != ctLARGE &&
		info->candle[pos].topshadow == ctSMALL &&
		info->candle[pos].botshadow == ctLARGE)
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_HaramiBlack(ctBars *b1, struct ctCandleInfo *info, short pos)
{                               /* Bottom Signal */
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].shape == ctSMALL &&
		info->candle[pos - 1].color == ctBLACKCNDL &&
		info->candle[pos - 1].shape == ctLARGE &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->op, pos - 1) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->op, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_HaramiCrossBlack(ctBars *b1, struct ctCandleInfo *info, short pos)
{                               /* Bottom Signal */
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].shape == ctDOJI &&
		info->candle[pos - 1].color == ctBLACKCNDL &&
		info->candle[pos - 1].shape == ctLARGE &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->op, pos - 1) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->op, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_HaramiCrossWhite(ctBars *b1, struct ctCandleInfo *info, short pos)
{                               /* Top Signal */
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].shape == ctDOJI &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos - 1].shape == ctLARGE &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->op, pos - 1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->op, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}


short WINAPI l_HaramiWhite(ctBars *b1, struct ctCandleInfo *info, short pos)
{                               /* Top Signal */
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].shape == ctSMALL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos - 1].shape == ctLARGE &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->op, pos - 1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->op, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_HighWave(ctBars *b1, struct ctCandleInfo *info, short pos)     /* Grp of these
* indicates reversal */
{
	if (info->candle[pos].shape == ctSMALL &&
		(info->candle[pos].topshadow == ctLARGE || info->candle[pos].botshadow == ctLARGE))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_InNeckLine(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctBLACKCNDL &&
		info->candle[pos].shape == ctSMALL &&
		info->candle[pos - 1].shape != ctDOJI &&
		l_ApproxEqual(ctArrayItem(b1->cl, pos), ctArrayItem(b1->lo, pos - 1), info->equalpct) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->lo, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_InvertedHammer(ctBars *b1, struct ctCandleInfo *info, short pos)       /* Bullish */
{
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape != ctLARGE &&
		info->candle[pos].topshadow == ctLARGE &&
		info->candle[pos].botshadow == ctSMALL)
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_LongLeggedDoji(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (info->candle[pos].shape == ctDOJI &&
		info->candle[pos].topshadow == ctLARGE &&
		info->candle[pos].botshadow == ctLARGE)
		return (ctTRUE);
	else
		return (ctFALSE);
}


short WINAPI l_MatHoldPattern(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctBLACKCNDL &&
		info->candle[pos - 2].color == ctBLACKCNDL &&
		info->candle[pos - 3].color == ctBLACKCNDL &&
		info->candle[pos - 4].color == ctWHITECNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos - 1].shape == ctSMALL &&
		info->candle[pos - 2].shape == ctSMALL &&
		info->candle[pos - 3].shape == ctSMALL &&
		info->candle[pos - 4].shape == ctLARGE &&
		l_BodyGapUp(b1, pos - 3) &&
		l_BodyGapUp(b1, pos) &&
		ctArrayItem(b1->op, pos - 3) > ctArrayItem(b1->op, pos - 2) &&
		ctArrayItem(b1->op, pos - 2) > ctArrayItem(b1->op, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}


short WINAPI l_MorningStar(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 2)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 2].color == ctBLACKCNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos - 1].shape == ctSMALL &&
		info->candle[pos - 2].shape == ctLARGE &&
		l_BodyGapDn(b1, pos - 1) &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->op, pos - 1) &&
		ctArrayItem(b1->cl, pos) > l_BodyMidpoint(b1, pos - 2))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_MorningDojiStar(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 2)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 2].color == ctBLACKCNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos - 1].shape == ctDOJI &&
		info->candle[pos - 2].shape == ctLARGE &&
		l_BodyGapDn(b1, pos - 1) &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->op, pos - 1) &&
		ctArrayItem(b1->cl, pos) > l_BodyMidpoint(b1, pos - 2))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_PiercingLine(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctBLACKCNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos - 1].shape == ctLARGE &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->cl, pos) > l_BodyMidpoint(b1, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}


short WINAPI l_Rising3Method(ctBars *b1, struct ctCandleInfo *info, short pos)
{                               /* bullish pattern */
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctBLACKCNDL &&
		info->candle[pos - 2].color == ctBLACKCNDL &&
		info->candle[pos - 3].color == ctBLACKCNDL &&
		info->candle[pos - 4].color == ctWHITECNDL &&
		info->candle[pos].shape == ctLARGE &&
		info->candle[pos - 1].shape == ctSMALL &&
		info->candle[pos - 2].shape == ctSMALL &&
		info->candle[pos - 3].shape == ctSMALL &&
		info->candle[pos - 4].shape == ctLARGE &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->cl, pos - 4) &&
		ctArrayItem(b1->cl, pos - 4) > ctArrayItem(b1->op, pos - 3) &&
		ctArrayItem(b1->cl, pos - 3) > ctArrayItem(b1->cl, pos - 2) &&
		ctArrayItem(b1->cl, pos - 2) > ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->cl, pos - 1) >= min(ctArrayItem(b1->op, pos), ctArrayItem(b1->op, pos - 4)))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_SeperatingLineBear(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos - 1].shape != ctDOJI &&
		ctArrayItem(b1->op, pos) == ctArrayItem(b1->op, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_SeperatingLineBull(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctBLACKCNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos - 1].shape != ctDOJI &&
		ctArrayItem(b1->op, pos) == ctArrayItem(b1->op, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_ShootingStar(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape == ctSMALL &&
		info->candle[pos].topshadow == ctLARGE &&
		info->candle[pos].botshadow == ctSMALL)
		return (ctTRUE);
	else
		return (ctFALSE);
}


short WINAPI l_SideBySideWhiteGapDn(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos - 1].shape != ctDOJI &&
		info->candle[pos].shape == info->candle[pos - 1].shape &&
		l_BodyGapDn(b1, pos - 1) &&
		l_ApproxEqual(ctArrayItem(b1->cl, pos), ctArrayItem(b1->cl, pos - 1), info->equalpct))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_SideBySideWhiteGapUp(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos - 1].shape != ctDOJI &&
		info->candle[pos].shape == info->candle[pos - 1].shape &&
		l_BodyGapUp(b1, pos - 1) &&
		l_ApproxEqual(ctArrayItem(b1->op, pos), ctArrayItem(b1->op, pos - 1), info->equalpct))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_StalledPattern(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos].shape == ctSMALL &&
		info->candle[pos - 1].shape == ctLARGE &&
		l_ApproxEqual(ctArrayItem(b1->op, pos), ctArrayItem(b1->cl, pos - 1), info->equalpct))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_TasukiDownsideGap(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 2)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctBLACKCNDL &&
		info->candle[pos - 2].color == ctBLACKCNDL &&
		info->candle[pos].shape == info->candle[pos - 1].shape &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->op, pos - 1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->cl, pos - 2) &&
		ctArrayItem(b1->op, pos - 1) < ctArrayItem(b1->cl, pos - 2))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_TasukiUpsideGap(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 2)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos - 2].color == ctWHITECNDL &&
		info->candle[pos].shape == info->candle[pos - 1].shape &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->op, pos - 1) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->cl, pos - 2) &&
		ctArrayItem(b1->op, pos - 1) > ctArrayItem(b1->cl, pos - 2))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_ThreeGapsDown(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 2)
		return (ctFALSE);
	if (l_BodyGapDn(b1, pos) &&
		l_BodyGapDn(b1, pos - 1) &&
		l_BodyGapDn(b1, pos - 2))
		return (ctTRUE);
	else
		return (ctFALSE);
}
short WINAPI l_ThreeGapsUp(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 2)
		return (ctFALSE);
	if (l_BodyGapUp(b1, pos) &&
		l_BodyGapUp(b1, pos - 1) &&
		l_BodyGapUp(b1, pos - 2))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_ThreeWhiteSoldiers(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 2)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos - 2].color == ctWHITECNDL &&
		info->candle[pos].topshadow == ctSMALL &&
		info->candle[pos - 1].topshadow == ctSMALL &&
		info->candle[pos - 2].topshadow == ctSMALL &&
		ctArrayItem(b1->hi, pos) > ctArrayItem(b1->hi, pos - 1) &&
		ctArrayItem(b1->hi, pos - 1) > ctArrayItem(b1->hi, pos - 2) &&
		l_ApproxEqual(ctArrayItem(b1->hi, pos), ctArrayItem(b1->cl, pos), info->equalpct) &&
		l_ApproxEqual(ctArrayItem(b1->hi, pos - 1), ctArrayItem(b1->cl, pos - 1), info->equalpct) &&
		l_ApproxEqual(ctArrayItem(b1->hi, pos - 2), ctArrayItem(b1->cl, pos - 2), info->equalpct))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_ThrustingLine(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 1)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctBLACKCNDL &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->cl, pos) < l_BodyMidpoint(b1, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}

short WINAPI l_UpsideGap2Crows(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 2)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos - 1].color == ctBLACKCNDL &&
		info->candle[pos - 2].color == ctWHITECNDL &&
		info->candle[pos - 2].shape == ctLARGE &&
		l_BodyGapUp(b1, pos - 1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->cl, pos - 1) &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->op, pos - 1))
		return (ctTRUE);
	else
		return (ctFALSE);
}


short WINAPI l_CandleColor(struct ctCandleInfo *info, short pos)
{
	return (info->candle[pos].color);
}

short WINAPI l_CandleShape(struct ctCandleInfo *info, short pos)
{
	return (info->candle[pos].shape);
}

short WINAPI l_CandleTopShadow(struct ctCandleInfo *info, short pos)
{
	return (info->candle[pos].topshadow);
}

short WINAPI l_CandleBotShadow(struct ctCandleInfo *info, short pos)
{
	return (info->candle[pos].botshadow);
}




/**
__start()
{
}
**/