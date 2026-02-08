#include "stdafx.h"
#include "C_UserGrpDlg.h"
#include "SetupDlg.h"

__declspec(dllexport) char* WINAPI axPropDlg(char* pStr, char* pMtblPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CSetupDlg dlg(pStr);
	static CString retVal;

	if (dlg.DoModal() == IDOK)
		retVal = dlg.m_sConfig;
	else
		retVal.Format("%s", pStr);

	return (char *)retVal.GetString(); 
}

__declspec(dllexport) char* WINAPI axGetSample(int pInfo1, int pInfo2, int *pLen)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	int	Pos = 0;
	struct _DnHead		*pDnHead   = nullptr;	// 내리는 헤더
	struct _DataInfo	*pDataInfo = nullptr;	// 
	struct _Data		*pData     = nullptr;	// 

	const	struct _UpHead		*pUpHead = (_UpHead *)pInfo1;
	const	struct _TestInfo	*pTestInfo = (_TestInfo *)pInfo2;

	const	int	nHead = pTestInfo->gcount;				// 그래프 개수
	const	bool	bVerticStr = pTestInfo->bVerticStr;			// 수직틱 여부 
	const	bool	bItemRts = pTestInfo->bItemRts;				// 항목별 실시간
	int	nData = atoi(CString(pUpHead->dcount, sizeof(pUpHead->dcount)));// 데이터 개수
	const	int	iSize = sz_VAL;
	int	rSize = sz_VAL;

	// check IGK_2D3NBAR
	////////////////////////////////////////////////////////////////
	bool	b2D3NBAR = false;
	if (pTestInfo->gKind[0] == IGK_2D3NBAR)	
		b2D3NBAR = true;
	if (b2D3NBAR)
		rSize = sz_VAL*3;
	////////////////////////////////////////////////////////////////
	// check IGK_2DBAR
	////////////////////////////////////////////////////////////////
	bool	b2D1NBAR = false;
	if (pTestInfo->gKind[0] == IGK_2D1NBAR)
		b2D1NBAR = true;
	////////////////////////////////////////////////////////////////

	int	bufSize = sz_DNHEAD + (nData * iSize) + nHead * (sz_DATAINFO + nData * rSize);
	std::unique_ptr<char[]>	pBuf = std::make_unique<char[]>(bufSize + 1);

	memset(pBuf.get(), ' ', bufSize + 1);
	
	// Header Set
	Pos = 0;
	pDnHead = (_DnHead *)&pBuf[Pos];
	memset(pDnHead, ' ', sz_DNHEAD);
	sprintf(pDnHead->nHead, "%02d", nHead);
	sprintf(pDnHead->nData, "%06d", nData);
	sprintf(pDnHead->irSize, "%02d", iSize);	// index size
	sprintf(pDnHead->drSize, "%02d", rSize);	// data size
	sprintf(pDnHead->xUnit[1], "[좌단위]");
	sprintf(pDnHead->xUnit[2], "[우단위]");
	sprintf(pDnHead->tipName, "가격\t이름1\t이름2\t");

	if (b2D3NBAR)	// ***********
		sprintf(pDnHead->tipName, "가격\t매수\t매도\t보합\t");

	// 색인 set
	Pos += sz_DNHEAD;
	const	CTime curTime = CTime::GetCurrentTime();
	double	total = 0;

	if (b2D3NBAR || b2D1NBAR)
	{
		pDnHead->xtype = '1';	// string index
		for (int ii = 0; ii < nData; ii++)
		{
			pData = (_Data *)&pBuf[Pos];
			sprintf(pData->val, "%d\t%d", 2000+ii, 2000+ii+1);
			total += atof(pData->val);

			Pos += iSize;
		}
	}
	else if (bVerticStr)
	{
		if (bItemRts)
		{
			for (int ii = 0; ii < nData; ii++)
			{
				pData = (_Data *)&pBuf[Pos];
				sprintf(pData->val, "%d\t%03d", 2000+ii, RS_BASE+ii);
				total += atof(pData->val);

				Pos += iSize;
			}
		}
		else
		{
			for (int ii = 0; ii < nData; ii++)
			{
				pData = (_Data *)&pBuf[Pos];
				sprintf(pData->val, "%d", 2000+ii);
				total += atof(pData->val);

				Pos += iSize;
			}
		}
	}
	else
	{
		for (int ii = 0; ii < nData; ii++)
		{
			pData = (_Data *)&pBuf[Pos];
			sprintf(pData->val, "%04d%02d%02d%02d%02d%02d", 
				curTime.GetYear(), 1 + ii/30, (ii+1)%30, 9, ii+1, 0);

			Pos += iSize;
		}
	}

	// Data set
	const	int	digit = rand()%3;
	for (int ii = 0, val = 0, average = 0; ii < nHead; ii++)
	{
		// 헤더부
		pDataInfo = (_DataInfo *)&pBuf[Pos];
		sprintf(pDataInfo->digit, "%02d", digit);
		sprintf(pDataInfo->name, "이름%d", ii+1);
		sprintf(pDataInfo->rtscode, "%s", RS_CODE);
		sprintf(pDataInfo->symbol, "%s", RS_SYM);

		// 데이터부
		Pos += sz_DATAINFO;
		average = 0;
		if (b2D3NBAR)
		{
			for (int jj = 0; jj < nData; jj++)
			{
				pData = (_Data *)&pBuf[Pos];
				val = rand()*10 + rand();
				if (jj%5 == 0)	val *= -1;

				sprintf(pData->val, "%d\t%d\t%d", abs(val), abs(val)/2, abs(val)/3+100);
				average += val;

				Pos += rSize;
			}
		}
		else
		{
			for (int jj = 0; jj < nData; jj++)
			{
				pData = (_Data *)&pBuf[Pos];
				val = rand()*10 + rand();
				if (jj%5 == 0)	val *= -1;

				sprintf(pData->val, "%d", val);
				average += val;

				Pos += rSize;
			}
		}

		if (nData > 0)
		{
			average /= nData;
			if (pTestInfo->gKind[ii] == IGK_XBASEVBAR)
				sprintf(pDataInfo->pivot, "%d", (int)total/nData);
			else if (pTestInfo->gKind[ii] == IGK_YBASELINE)
				sprintf(pDataInfo->pivot, "%07d", average);
		}
	}

	*pLen = bufSize;
	return pBuf.release(); 
}