/* 하락반전형 캔들 패턴	*/

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

short WINAPI n_isUpGoing(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (ctArrayItem(b1->cl, pos-3) < ctArrayItem(b1->cl, pos - 2) &&
	    ctArrayItem(b1->cl, pos-2) < ctArrayItem(b1->cl, pos - 1) &&
	    ctArrayItem(b1->cl, pos-1) < ctArrayItem(b1->cl, pos))
		return(ctTRUE);
	return (ctFALSE);
}

short WINAPI n_HangingMan(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 3)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape != ctLARGE &&
		info->candle[pos].topshadow == ctSMALL &&
		info->candle[pos].botshadow == ctLARGE &&	/* hammer */
		n_isUpGoing(b1, info, pos))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_BearishBeltHold(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 3)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos].shape == ctLARGE &&
		info->candle[pos].botshadow == ctSMALL &&
		ctArrayItem(b1->op, pos) == ctArrayItem(b1->hi, pos) &&
		n_isUpGoing(b1, info, pos))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_BearishEngulfing(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos-1].color == ctWHITECNDL &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->cl, pos-1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->op, pos-1) &&
		n_isUpGoing(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_HaramiBlack(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos-1].color == ctWHITECNDL &&
		info->candle[pos-1].shape == ctLARGE &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos-1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->op, pos-1) &&
		n_isUpGoing(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_HaramiCrossBlack(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos-1].color == ctBLACKCNDL &&
		info->candle[pos-1].shape == ctLARGE &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos-1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->op, pos-1) &&
		ctArrayItem(b1->cl, pos) == ctArrayItem(b1->op, pos) && /* Doji */
		n_isUpGoing(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_BearishCounterAttckLine(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos-1].color == ctWHITECNDL &&
		info->candle[pos-1].shape == ctLARGE &&
		ctArrayItem(b1->cl, pos) >= ctArrayItem(b1->cl, pos-1) &&
		n_isUpGoing(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_DarkCrowsCover(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos].shape == ctLARGE &&
		info->candle[pos-1].color == ctWHITECNDL &&
		info->candle[pos-1].shape == ctLARGE &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->hi, pos-1) &&
		l_BodyMidpoint(b1, pos-1) > ctArrayItem(b1->cl, pos) &&
		n_isUpGoing(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_EveningStar(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos-1].color == ctWHITECNDL &&
		info->candle[pos-1].shape == ctSMALL &&
		info->candle[pos-2].color == ctWHITECNDL &&
		info->candle[pos-2].shape == ctLARGE &&
		l_BodyGapUp(b1, pos-1) &&
		l_BodyGapDn(b1, pos) &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos-2) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->op, pos-2) &&
		n_isUpGoing(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_EveningDojiStar(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos-1].color == ctWHITECNDL &&
		info->candle[pos-1].shape == ctDOJI &&
		info->candle[pos-2].color == ctWHITECNDL &&
		info->candle[pos-2].shape == ctLARGE &&
		l_BodyGapUp(b1, pos-1) &&
		l_BodyGapDn(b1, pos) &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos-2) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->op, pos-2) &&
		n_isUpGoing(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_AbandonedBabyBlack(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos-1].color == ctWHITECNDL &&
		info->candle[pos-1].shape == ctDOJI &&
		info->candle[pos-2].color == ctBLACKCNDL &&
		info->candle[pos-2].shape == ctLARGE &&
		l_BodyGapDn(b1, pos - 1) &&
		ctArrayItem(b1->hi, pos-2) < ctArrayItem(b1->lo, pos-1) &&
		ctArrayItem(b1->hi, pos)   > ctArrayItem(b1->lo, pos-1) &&
		n_isUpGoing(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_ThreeInsideDown(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (n_HaramiBlack(b1, info, pos-1) &&
		info->candle[pos].color == ctBLACKCNDL &&
		ctArrayItem(b1->cl, pos-1) > ctArrayItem(b1->cl, pos))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_ThreeOutsideDown(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (n_BearishEngulfing(b1, info, pos-1) &&
		info->candle[pos].color == ctBLACKCNDL &&
		ctArrayItem(b1->cl, pos-1) > ctArrayItem(b1->cl, pos) &&
		ctArrayItem(b1->op, pos-2) > ctArrayItem(b1->cl, pos))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_UpsideGap2Crows(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos].shape == ctLARGE &&
		info->candle[pos-1].color == ctBLACKCNDL &&
		info->candle[pos-2].color == ctWHITECNDL &&
		l_BodyGapUp(b1, pos-1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->cl, pos-1) &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->op, pos-1) &&
		n_isUpGoing(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_BearishDojiStar(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos].shape == ctDOJI &&
		info->candle[pos-1].color == ctWHITECNDL &&
		l_BodyGapUp(b1, pos) &&
		n_isUpGoing(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_TweezersTop(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos-1].color == ctWHITECNDL &&
/*
		ctArrayItem(b1->hi, pos) == ctArrayItem(b1->hi, pos-1) &&
*/
		l_ApproxEqual(ctArrayItem(b1->hi, pos), ctArrayItem(b1->hi, pos-1), info->equalpct) &&
		n_isUpGoing(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}
