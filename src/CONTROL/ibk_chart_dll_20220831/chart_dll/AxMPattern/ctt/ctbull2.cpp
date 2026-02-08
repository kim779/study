/* »ó½ÂÁö¼ÓÇü Äµµé ÆÐÅÏ	*/

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

short WINAPI n_ThreeWhiteSoldier(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos-1].color == ctWHITECNDL &&
		info->candle[pos-2].color == ctWHITECNDL &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->op, pos-1) &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos-1) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->cl, pos-1) &&
		ctArrayItem(b1->op, pos-1) > ctArrayItem(b1->op, pos-2) &&
		ctArrayItem(b1->op, pos-1) < ctArrayItem(b1->cl, pos-2) &&
		ctArrayItem(b1->cl, pos-1) > ctArrayItem(b1->cl, pos-2) &&
		n_isDowning(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_AdvancedBlock(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos].shape == ctSMALL &&
		info->candle[pos].topshadow == ctLARGE &&
		info->candle[pos-1].color == ctWHITECNDL &&
		info->candle[pos-1].shape == ctSMALL &&
		info->candle[pos-1].topshadow == ctLARGE &&
		info->candle[pos-2].color == ctWHITECNDL &&
		info->candle[pos-2].topshadow != ctLARGE &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->cl, pos-1) &&
		ctArrayItem(b1->cl, pos-1) > ctArrayItem(b1->cl, pos-2) &&
		n_isUpGoing(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_StalledPattern(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos - 2].color == ctWHITECNDL &&
		info->candle[pos].shape == ctSMALL &&
		info->candle[pos - 1].shape == ctLARGE &&
		l_ApproxEqual(ctArrayItem(b1->op, pos), ctArrayItem(b1->cl, pos - 1), info->equalpct) &&
		n_isUpGoing(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_UpsideGapTasuki(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos-1].color == ctWHITECNDL &&
		info->candle[pos-2].color == ctWHITECNDL &&
		info->candle[pos].shape == info->candle[pos - 1].shape &&
		l_BodyGapUp(b1, pos-1) &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos-1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->op, pos-1) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->cl, pos-2) &&
		n_isUpGoing(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_UpsideGap3Method(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos - 1].color == ctWHITECNDL &&
		info->candle[pos - 2].color == ctWHITECNDL &&
		l_BodyGapUp(b1, pos-1) &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos-1) &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->op, pos-1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->op, pos-1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->cl, pos-2) &&
		n_isUpGoing(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}
