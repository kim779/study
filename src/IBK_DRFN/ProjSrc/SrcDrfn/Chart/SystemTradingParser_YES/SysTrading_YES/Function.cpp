#include <stdio.h>
#include <stdlib.h>

#include "Function.h"

int CnvtBufInit()
{
	memset(&gtCnvtBufTbl, 0x00, sizeof(CNVT_BUF_TABLE));

	gtCnvtBufTbl.iWPos = -1;
	gtCnvtBufTbl.iRPos = 0;
	gtCnvtBufTbl.iDataCnt = 0;
	gtCnvtBufTbl.iMaxSize = MAX_CNVT_BUF;

	return 1;
}

int CnvtBufClose()
{

	return 1;
}

int CnvtBufInsert(FILE *ptpFp, char *psFormat, ...)
{
	int iIdx;
	int liLen;
	va_list ltVaList;

	if (gtCnvtBufTbl.iDataCnt >= gtCnvtBufTbl.iMaxSize)
	{
		CnvtBufWriteFile(1);
	}

	gtCnvtBufTbl.iWPos = (gtCnvtBufTbl.iWPos + 1) % gtCnvtBufTbl.iMaxSize;
	iIdx = gtCnvtBufTbl.iWPos;

	gtCnvtBufTbl.tpItem[iIdx].tpFp = ptpFp;
	gtCnvtBufTbl.tpItem[iIdx].iLineNo = giLineNo;

	va_start(ltVaList, psFormat);
	gtCnvtBufTbl.tpItem[iIdx].iLen =
		vsprintf(gtCnvtBufTbl.tpItem[iIdx].sBuf, psFormat, ltVaList);	
	va_end(ltVaList);
	
	gtCnvtBufTbl.iDataCnt++;

	return gtCnvtBufTbl.tpItem[iIdx].iLen;
}

int CnvtBufWriteFile(int piMethod)
{
	int i;
	int liIdx;
	int liCnt;

	if (piMethod <= 0 || piMethod >= gtCnvtBufTbl.iDataCnt)
	{
		liCnt = gtCnvtBufTbl.iDataCnt;
	}
	else
	{
		liCnt = piMethod;	
	}

	for (i = 0; i < liCnt; i++)
	{
		liIdx = gtCnvtBufTbl.iRPos;
		fprintf(gtCnvtBufTbl.tpItem[liIdx].tpFp, "%.*s", 
			gtCnvtBufTbl.tpItem[liIdx].iLen,
			gtCnvtBufTbl.tpItem[liIdx].sBuf);
			
		gtCnvtBufTbl.iRPos = (gtCnvtBufTbl.iRPos + 1) % gtCnvtBufTbl.iMaxSize;
	}

	gtCnvtBufTbl.iDataCnt -= liCnt;

	return liCnt;
}
