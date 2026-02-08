/* ÇÏ¶ôÁö¼ÓÇü Äµµé ÆÐÅÏ	*/
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

short WINAPI n_ThreeBlackCrows(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos-1].color == ctBLACKCNDL &&
		info->candle[pos-2].color == ctBLACKCNDL &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->op, pos-1) &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->cl, pos-1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->cl, pos-1) &&
		ctArrayItem(b1->op, pos-1) < ctArrayItem(b1->op, pos-2) &&
		ctArrayItem(b1->op, pos-1) > ctArrayItem(b1->cl, pos-2) &&
		ctArrayItem(b1->cl, pos-1) < ctArrayItem(b1->cl, pos-2) &&
		n_isUpGoing(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_IdenticalThreeCrows(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos].shape == ctLARGE &&
		info->candle[pos-1].color == ctBLACKCNDL &&
		info->candle[pos-1].shape == ctLARGE &&
		info->candle[pos-2].color == ctBLACKCNDL &&
		info->candle[pos-2].shape == ctLARGE &&
		l_ApproxEqual(ctArrayItem(b1->op, pos), ctArrayItem(b1->cl, pos-1), info->equalpct) &&
		l_ApproxEqual(ctArrayItem(b1->op, pos-1), ctArrayItem(b1->cl, pos-2), info->equalpct) &&
		n_isUpGoing(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_DownsideGapTasuki(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos-1].color == ctBLACKCNDL &&
		info->candle[pos-2].color == ctBLACKCNDL &&
		info->candle[pos].shape == info->candle[pos-1].shape &&
		l_BodyGapDn(b1, pos-1) &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->cl, pos-1) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->op, pos-1) &&
		ctArrayItem(b1->cl, pos) < ctArrayItem(b1->cl, pos-2) &&
		n_isDowning(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_DownsideGap3Method(ctBars *b1, struct ctCandleInfo *info, short pos)
{
	if (pos < 5)
		return (ctFALSE);
	if (info->candle[pos].color == ctWHITECNDL &&
		info->candle[pos-1].color == ctBLACKCNDL &&
		info->candle[pos-2].color == ctBLACKCNDL &&
		l_BodyGapDn(b1, pos-1) &&
		ctArrayItem(b1->op, pos) < ctArrayItem(b1->op, pos-1) &&
		ctArrayItem(b1->op, pos) > ctArrayItem(b1->cl, pos-1) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->op, pos-1) &&
		ctArrayItem(b1->cl, pos) > ctArrayItem(b1->cl, pos-2) &&
		n_isDowning(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}

short WINAPI n_ShootingStar(ctBars *b1, struct ctCandleInfo *info, short pos)
{	/* À½ºÀ ¿ª¸ÁÄ¡Çü	*/
	if (pos < 4)
		return (ctFALSE);
	if (info->candle[pos].color == ctBLACKCNDL &&
		info->candle[pos].shape != ctLARGE &&
		info->candle[pos].topshadow == ctLARGE &&
		info->candle[pos].botshadow == ctSMALL &&
		n_isUpGoing(b1, info, pos-2))
		return (ctTRUE);
	return (ctFALSE);
}
