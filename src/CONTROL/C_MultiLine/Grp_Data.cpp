#include "stdafx.h"
#include "C_MultiLine.h"
#include "Grp_Data.h"
#include "GrpWnd.h"
#include "../../h/axiscgrp.h"
#include <math.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// ECN realtime Symbol
#define	ER_HNAM	"722"	// 종목명
#define	ER_CURR	"723"	// 현재가
#define	ER_DIFF	"724"	// 전일대비
#define	ER_MDGA	"725"	// 매도호가
#define	ER_MSGA	"726"	// 매수호가
#define	ER_GVOL	"727"	// 거래량
#define	ER_GAMT	"728"	// 거래대금(억)
#define	ER_SIGA	"729"	// 시가
#define	ER_KOGA	"730"	// 고가
#define	ER_JEGA	"731"	// 저가
#define	ER_CVOL	"732"	// 체결거래량(체결수량)
#define	ER_UDYL	"733"	// 등락율
#define	ER_CTIM	"734"	// 체결시간
#define	ER_CCOL	"735"	// 체결구분
#define	ER_HTIM	"740"	// 호가시간

CGrp_Data::CGrp_Data(class CGrpWnd *pGrpWnd, int dKind, int dUnit, int dIndex)
{
	m_pGrpWnd = pGrpWnd;
	m_dKind = dKind;
	m_dUnit = dUnit;
	m_dIndex = dIndex;
	m_DataQue.RemoveAll();
	m_szFrame = 0;
}

CGrp_Data::~CGrp_Data()
{
	char *pData = nullptr;
	for (int ii = 0; ii < m_DataQue.GetUpperBound(); ii++)
	{
		pData = m_DataQue.GetAt(ii);
		if (pData)
		{
			delete[] pData;
			pData = nullptr;
		}
	}	
	m_DataQue.RemoveAll();
}

int CGrp_Data::AttachGraphData(char* data, int count, int mgap)
{
	m_dGap = mgap;
	int	lcnt = m_pGrpWnd->m_lcnt;
	
	m_szFrame = (sz_cgIndex + lcnt * sizeof(double));

	if (!CreateDataQue(count))
		return -1;

	struct _multi_lineD	*multi_lineD = NULL;
	struct _multi_line	*multi_line = NULL;
	
	int	dPos = 0;
	char	*pQue;
	int	nSize = 14 + lcnt * 10;

	for (int ii = 0; ii < count; ii++)
	{
		pQue = m_DataQue.GetAt(ii);

		multi_line = (struct _multi_line*)pQue;
		multi_lineD = (struct _multi_lineD*)&data[dPos];
		
		multi_line->index.date.yy = short(CharToInt(&multi_lineD->index[0], 4));
		multi_line->index.date.mm = unsigned char(CharToInt(&multi_lineD->index[4], 2));
		multi_line->index.date.dd = unsigned char(CharToInt(&multi_lineD->index[6], 2));
		multi_line->index.time.hh = unsigned char(CharToInt(&multi_lineD->index[8], 2));
		multi_line->index.time.mm = unsigned char(CharToInt(&multi_lineD->index[10], 2));
		multi_line->index.time.ss = unsigned char(CharToInt(&multi_lineD->index[12], 2));

		for ( int jj = 0 ; jj < lcnt ; jj++ )
		{
			if (multi_lineD->data[jj][0] == 'n' ||
				multi_lineD->data[jj][0] == 'N')
				multi_line->data[jj] = UNUSED_VALUE;
			else
				multi_line->data[jj] = CharToDouble(multi_lineD->data[jj], sizeof(multi_lineD->data[jj]));
		}

		dPos += nSize;
	}

	return nSize;
}

bool CGrp_Data::UpdateRTM(char* RTM)
{	
	CString			rtm = RTM;
	CMapStringToString	rtmStore;
	CString			val, symbol;
	int			index = 0;
	int			ii = 0, jj = 0;
	int			nDataSize = m_DataQue.GetSize();
	bool			bCreate = false;


	CString			timecode = _T("");
	CString			sTime = _T("");
	CString			tmpstr;

	if (m_dUnit == CDU_ECN)
		timecode = ER_CTIM;
	else
		timecode = R_CTIM;

	for (; !rtm.IsEmpty(); )
	{
		index = rtm.Find('\t');
		if (index < 0)
			break;
		symbol = rtm.Left(index++);

		if (symbol.Compare("327") == 0)
			TRACE("good\n");
		rtm = rtm.Mid(index);

		index = rtm.Find('\t');
		if (index < 0)
			index = rtm.GetLength();
		val = rtm.Left(index++);

		rtmStore.SetAt(symbol, val);

		if (rtm.GetLength() <= index)
			break;
		rtm = rtm.Mid(index);
	}

	if (!rtmStore.Lookup(timecode, sTime))
		return false;

	tmpstr.Format("%.8s%.6s", m_pGrpWnd->m_szDate, sTime);
	sTime = tmpstr;

	if (sTime.IsEmpty())
		return false;

	char*	pData = NULL;
	char*	prevData = NULL;
	struct _multi_line*	mline;
	struct _multi_line*	pline;
	struct	_cgIndex	gIndex;
	int	nTime = 0;
	int	hh = 0, mm = 0, ss = 0;
	int	mok = 0, mod = 0;
	CStringArray	arData;
	
	gIndex.date.yy = short(atoi(sTime.Left(4)));
	gIndex.date.mm = unsigned char(atoi(sTime.Mid(4, 2)));
	gIndex.date.dd = unsigned char(atoi(sTime.Mid(6, 2)));
	gIndex.time.hh = unsigned char(atoi(sTime.Mid(8, 2)));
	nTime = gIndex.time.hh * 3600;
	gIndex.time.mm = unsigned char(atoi(sTime.Mid(10, 2)));
	nTime += gIndex.time.mm * 60;
	gIndex.time.ss = unsigned char(atoi(sTime.Mid(12, 2)));
	nTime += gIndex.time.ss;

	if (nDataSize == 0)
	{
		bCreate = true;	
		pData = new char[m_szFrame];
		memset(pData, 0x00, m_szFrame);		
		mline = (struct _multi_line*)pData;

		memcpy(&mline->index, &gIndex, sz_cgIndex);

		if (m_dIndex == CDI_MIN && m_dGap < 60)
		{
			ss = mline->index.time.ss;
			mok = ss / m_dGap;
			mline->index.time.ss = mok * m_dGap;
		}
		
		bCreate = true;
		m_DataQue.Add(pData);
	}
	else
	{
		prevData = m_DataQue.GetAt(nDataSize - 1);
		pline = (struct _multi_line*)prevData;
		int	nGab = 0;
		int	nPrev = 0;

		switch (m_dIndex)
		{
		case CDI_MIN:
			{
				nPrev = pline->index.time.hh * 3600;
				nPrev += pline->index.time.mm * 60;
				nPrev += pline->index.time.ss;

				if ((nTime - nPrev) >= m_dGap)
				{
					pData = new char[m_szFrame];
					memset(pData, 0x00, m_szFrame);		
					mline = (struct _multi_line*)pData;
					memcpy(mline, pline, m_szFrame);
					memcpy(&mline->index, &gIndex, sz_cgIndex);
				
					if (m_dGap < 60)
					{
						ss = mline->index.time.ss;
						mok = ss / m_dGap;
						mline->index.time.ss = mok * m_dGap;
					}
					else
						mline->index.time.ss = 0;
					
					m_DataQue.Add(pData);
					bCreate = true;
				}
				else
				{
					pData = prevData;
				}
			}
			break;
		case CDI_TICK:
			{
				pData = new char[m_szFrame];
				memset(pData, 0x00, m_szFrame);					
				mline = (struct _multi_line*)pData;
				memcpy(mline, pline, m_szFrame);
				memcpy(&mline->index, &gIndex, sz_cgIndex);
				m_DataQue.Add(pData);
				bCreate = true;
			}
			break;
		case CDI_DAY:
			{
				pData = prevData;
			}
			break;
		}		
	}

	mline = (struct _multi_line*)pData;
	char	rsym[7];
	double	div = 0;
	double	value = 0;
	bool	bUpdate = false;

	for ( ii = 0, index = 0; ii < m_pGrpWnd->m_RgnCnt ; ii++ )
	{
		div = pow(10.0, m_pGrpWnd->m_pRgnInfo[ii]->digit);
		for ( jj = 0 ; jj < m_pGrpWnd->m_pRgnInfo[ii]->lcnt ; jj++ )
		{
			memset(rsym, 0x00, sizeof(rsym));
			memcpy(rsym, m_pGrpWnd->m_pRgnInfo[ii]->rcod[jj], sizeof(m_pGrpWnd->m_pRgnInfo[ii]->rcod[jj]));
			symbol.Format("%03d", atoi(rsym));
			
			if (rtmStore.Lookup(symbol, val))
			{
				if (!m_pGrpWnd->m_bSign)
				{
					if (val.Find("--") >= 0)
					{
						val.Replace("--", "-");
						value = atof(val) * div;
					}
					else if (val.Find("+-") >= 0)
					{
						val.Replace("+-", "-");
						value = atof(val) * div;
					}
					else
						value = fabs(atof(val)) * div;
				}
				else
					value = atof(val);

				mline->data[index] = value;
				bUpdate = true;
			}

			index++;
		}
	}

	if (!bUpdate)
	{
		if (bCreate)
		{
			int	nCount = m_DataQue.GetSize();
			pData = m_DataQue.GetAt(nCount - 1);
			delete [] pData;
			pData = nullptr;
			m_DataQue.RemoveAt(nCount - 1);
		}
	}
	else
	{
		if (bCreate)
		{
			if (m_pGrpWnd->m_bCtrl)
			{
				pData = m_DataQue.GetAt(0);
				delete [] pData;
				pData = nullptr;
				m_DataQue.RemoveAt(0);
			}
			else
			{
				if (m_pGrpWnd->m_dCount < m_DataQue.GetSize())
				{
					pData = m_DataQue.GetAt(0);
					delete [] pData;
					pData = nullptr;
					m_DataQue.RemoveAt(0);
				}

				m_pGrpWnd->m_totalDay = m_pGrpWnd->m_dispEnd = m_pGrpWnd->m_dispDay = m_DataQue.GetSize();
				m_pGrpWnd->m_dispPos = 0;			
			}
		}
	}

	return bCreate;
}

bool CGrp_Data::CreateDataQue(int count)
{
	if (m_szFrame <= 0)
		return false;

	char	*pData;
	for (int ii = 0; ii < count; ii++)
	{
		pData = new char[m_szFrame];
		ZeroMemory(pData, m_szFrame);
		m_DataQue.Add(pData);
	}

	return true;
}

char* CGrp_Data::GetGraphData(int index)
{
	if (index < 0 || index > m_DataQue.GetUpperBound())
		return (char *) 0;

	return m_DataQue.GetAt(index);
}

int CGrp_Data::CharToInt(const char* cnvB, int cnvL)
{
	char	rWb[64];
	CopyMemory(rWb, cnvB, cnvL);
	rWb[cnvL] = '\0';

	return atoi(rWb);
}

double CGrp_Data::CharToDouble(const char* cnvB, int cnvL)
{
	char	rWb[64];
	CopyMemory(rWb, cnvB, cnvL);
	rWb[cnvL] = '\0';

	return atof(rWb);
}
