/* »ó½Â¹ÝÀüÇü Äµµé ÆÐÅÏ	*/

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

short WINAPI n_isDowning(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (ctArrayItem(b1->cl, pos-3) > ctArrayItem(b1->cl, pos - 2) &&
	    ctArrayItem(b1->cl, pos-2) > ctArrayItem(b1->cl, pos - 1) &&
	    ctArrayItem(b1->cl, pos-1) > ctArrayItem(b1->cl, pos))
		return(ctTRUE);
	return (ctFALSE);
}

short WINAPI n_InvertedHammer(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 3)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape != ctLARGE &&
		info->candle[pos].topshadow == ctLARGE &&
		info->candle[pos].botshadow == ctSMALL &&	/* inverted hammer */
		n_isDowning(b1, info, pos))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_Hammer(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 3)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos].shape != ctLARGE &&
		info->candle[pos].topshadow == ctSMALL &&
		info->candle[pos].botshadow == ctLARGE &&	/* hammer */
		n_isDowning(b1, info, pos))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_BullishBeltHold(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 3)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape == ctLARGE &&
		info->candle[pos].topshadow == ctSMALL &&
		ctArrayItem(b1->op, pos) == ctArrayItem(b1->lo, pos) &&
		n_isDowning(b1, info, pos))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_BullishEngulfing(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos-1].color == ctBLACKCNDL &&
		ctArrayItem(b1->op, pos-1) < ctArrayItem(b1->cl, pos) &&
		ctArrayItem(b1->cl, pos-1) > ctArrayItem(b1->op, pos) &&
		n_isDowning(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_Harami(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos-1].color == ctBLACKCNDL &&
		info->candle[pos-1].shape == ctLARGE &&
		ctArrayItem(b1->op, pos-1) > ctArrayItem(b1->cl, pos) &&
		ctArrayItem(b1->cl, pos-1) < ctArrayItem(b1->op, pos) &&
		n_isDowning(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_HaramiCross(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos-1].color == ctBLACKCNDL &&
		info->candle[pos-1].shape == ctLARGE &&
		ctArrayItem(b1->op, pos-1) > ctArrayItem(b1->cl, pos) &&
		ctArrayItem(b1->cl, pos-1) < ctArrayItem(b1->op, pos) &&
		ctArrayItem(b1->cl, pos) == ctArrayItem(b1->op, pos) && /* Doji */
		n_isDowning(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_BullishCounterAttckLine(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos-1].color == ctBLACKCNDL &&
		info->candle[pos-1].shape == ctLARGE &&
		ctArrayItem(b1->cl, pos-1) >= ctArrayItem(b1->cl, pos) &&
		n_isDowning(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_PiercingLine(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape == ctLARGE &&
		info->candle[pos-1].color == ctBLACKCNDL &&
		info->candle[pos-1].shape == ctLARGE &&
		ctArrayItem(b1->lo, pos-1) > ctArrayItem(b1->op, pos) &&
		l_BodyMidpoint(b1, pos-1) < ctArrayItem(b1->cl, pos) &&
		n_isDowning(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_MorningStar(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos-1].color == ctWHITECNDL &&
		info->candle[pos-1].shape == ctSMALL &&
		info->candle[pos-2].color == ctBLACKCNDL &&
		info->candle[pos-2].shape == ctLARGE &&
		l_BodyGapDn(b1, pos-1) &&
		ctArrayItem(b1->cl, pos-2) < ctArrayItem(b1->op, pos) &&
		ctArrayItem(b1->op, pos-2) > ctArrayItem(b1->cl, pos) &&
		n_isDowning(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_MorningDojiStar(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos-1].color == ctWHITECNDL &&
		info->candle[pos-1].shape == ctDOJI &&
		info->candle[pos-2].color == ctBLACKCNDL &&
		info->candle[pos-2].shape == ctLARGE &&
		l_BodyGapDn(b1, pos - 1) &&
		ctArrayItem(b1->cl, pos-2) < ctArrayItem(b1->op, pos) &&
		ctArrayItem(b1->op, pos-2) > ctArrayItem(b1->cl, pos) &&
		n_isDowning(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_AbandonedBaby(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape != ctDOJI &&
		info->candle[pos-1].color == ctWHITECNDL &&
		info->candle[pos-1].shape == ctDOJI &&
		info->candle[pos-2].color == ctBLACKCNDL &&
		info->candle[pos-2].shape == ctLARGE &&
		l_BodyGapDn(b1, pos - 1) &&
		ctArrayItem(b1->lo, pos-2) > ctArrayItem(b1->hi, pos-1) &&
		ctArrayItem(b1->lo, pos)   > ctArrayItem(b1->hi, pos-1) &&
		n_isDowning(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_ThreeInsideUp(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (n_Harami(b1, info, pos-1) &&
		info->candle[pos].color == ctWHITECNDL &&
		ctArrayItem(b1->cl, pos-1) < ctArrayItem(b1->cl, pos))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_ThreeOutsideUp(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (n_BullishEngulfing(b1, info, pos-1) &&
		info->candle[pos].color == ctWHITECNDL &&
		ctArrayItem(b1->cl, pos-1) < ctArrayItem(b1->cl, pos) &&
		ctArrayItem(b1->op, pos-2) < ctArrayItem(b1->cl, pos))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_UpsideGap2CrowsDown(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape == ctLARGE &&
		info->candle[pos-1].color == ctWHITECNDL &&
		info->candle[pos-2].color == ctBLACKCNDL &&
		l_BodyGapDn(b1, pos-1) &&
		ctArrayItem(b1->cl, pos-1) < ctArrayItem(b1->cl, pos) &&
		ctArrayItem(b1->op, pos-1) < ctArrayItem(b1->op, pos) &&
		n_isDowning(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_BullishDojiStar(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape == ctDOJI &&
		info->candle[pos-1].color == ctBLACKCNDL &&
		l_BodyGapDn(b1, pos) &&
		n_isDowning(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_TweezersBottom(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos-1].color == ctBLACKCNDL &&
/*
		ctArrayItem(b1->lo, pos) == ctArrayItem(b1->lo, pos-1) &&
*/
		l_ApproxEqual(ctArrayItem(b1->lo, pos), ctArrayItem(b1->lo, pos-1), info->equalpct) &&
		n_isDowning(b1, info, pos-1))
		return (ctTRUE);
	return (ctFALSE);
}
