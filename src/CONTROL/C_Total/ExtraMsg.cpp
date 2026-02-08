// ExtraMsg.cpp: implementation of the CExtraMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_Total.h"
#include "ExtraMsg.h"
#include "MainWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExtraMsg::CExtraMsg(class CMainWnd *pMain)
{
	m_pMainWnd = pMain;
	m_pApp = (CC_TotalApp *)AfxGetApp();
}

CExtraMsg::~CExtraMsg()
{

}

LRESULT CExtraMsg::InputEvent(WPARAM wParam, LPARAM lParam)
{
	CString	tmpS;
	switch (LOWORD(wParam))
	{
	case INPUT_GET_POS_COMMENT:
	case INPUT_CHANGE:
		break;
	case INPUT_DATA:
		switch (HIWORD(wParam))
		{
		case GET_TR_INFO:
			return GetInputTRInfo((char *)lParam);
		case getTRInfo2:
			return GetInputTRInfo2((char *)lParam);
		case GET_TOTAL_DAY:
			{
				struct _envInfo	*envinfo = NULL;
				if (m_pMainWnd->m_pwndChart)
					envinfo = (_envInfo*)m_pMainWnd->m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);

				int	uIndex = 0;
				switch (m_pMainWnd->m_iDtUnit)
				{
				case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
					uIndex = GU_FUTURE - GU_CODE;
					break;
				case GU_CODE:	case GU_INDEX:	
					uIndex = m_pMainWnd->m_iDtUnit - GU_CODE;
					break;
				case GU_FOREIGN:
					uIndex = 3;
					break;
				default:
					break;
				}
				return envinfo->datainfo.aaDayInfo[uIndex][m_pMainWnd->m_iDtIndex - GI_DAY].dwTotCnt;
			}
			break;
		case getDispDay:
			{
				struct _envInfo	*envinfo = NULL;
				if (m_pMainWnd->m_pwndChart)
					envinfo = (_envInfo*)m_pMainWnd->m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);

				int	uIndex = 0;
				switch (m_pMainWnd->m_iDtUnit)
				{
				case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
					uIndex = GU_FUTURE - GU_CODE;
					break;
				case GU_CODE:	case GU_INDEX:	
					uIndex = m_pMainWnd->m_iDtUnit - GU_CODE;
					break;
				case GU_FOREIGN:
					uIndex = 3;
					break;
				default:
					break;
				}
				return envinfo->datainfo.aaDayInfo[uIndex][m_pMainWnd->m_iDtIndex-GI_DAY].dwDisCnt;
			}
			break;
		case getCalendar:
			sprintf((char*)lParam, "%s", m_pMainWnd->m_strDay);
			break;
		case getCurGapList:
			break;
		case setCodeInfo:	break;
		case domminoCode:	break;
		case triggerCode:	break;
		case isDollarData:	return FALSE;
		case isTodayData:	return FALSE;
		case GET_VALID_CODE:	return (long)m_pMainWnd->m_strCode.operator LPCTSTR();
		case GET_TODAY:
			{
				_envInfo	*envinfo = (_envInfo*)m_pMainWnd->m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);
				return envinfo->datainfo.cToday;
			}
			break;		
		}
		break;
	case inputEnv:		break;
	case inputViewFocus:	break;
	case inputChgPalette:	break;
	default:		break;
	}

	return FALSE;
}

int CExtraMsg::GetInputTRInfo(char *pHeader)
{
	CString	codeS = _T("");
	switch (m_pMainWnd->m_iDtUnit)
	{
	case GU_CODE:	
		codeS = IJ_CODE;	break;
	case GU_INDEX:	
		codeS = II_CODE;	break;
	case GU_FUTURE:	
		codeS = IF_CODE;	break;
	case GU_OPTION:
		codeS = IO_CODE;	break;
	case GU_FOREIGN:
		codeS = IS_CODE;	break;
	case GU_COFUT:
		codeS = ICF_CODE;	break;
	case GU_COOPT:
		codeS = ICO_CODE;	break;
	default:	return 0;
	}

	CString	outSymbol = _T("");
	switch (m_pMainWnd->m_iDtUnit)
	{
	case GU_CODE:
		outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OJ_TIME, OJ_NAME, OJ_JGGA, OJ_DIFF, OJ_RATE, OJ_GVOL, OJ_MDHG, OJ_MSHG, 
			OJ_SIGA, OJ_KOGA, OJ_JEGA,
			OJ_CODE, OJ_CDAY, OJ_SAHG, OJ_HAHG, OJ_JJGA, OJ_STCK, OJ_START);
		break;
	case GU_INDEX:
		outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OI_TIME, OI_NAME, OI_JGGA, OI_DIFF, OI_RATE, OI_GVOL, OI_MDHG, OI_MSHG, 
			OI_SIGA, OI_KOGA, OI_JEGA,
			OI_CODE, OI_CDAY, OI_SAHG, OI_HAHG, OI_JJGA, OI_STCK, OI_START);
		break;
	case GU_FUTURE:
		outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OF_TIME, OF_NAME, OF_JGGA, OF_DIFF, OF_RATE, OF_GVOL, OF_MDHG, OF_MSHG, 
			OF_SIGA, OF_KOGA, OF_JEGA, 
			OF_CODE, OF_CDAY, OF_SAHG, OF_HAHG, OF_JJGA, OF_STCK, OF_START);
		break;
	case GU_OPTION:
		outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OO_TIME, OO_NAME, OO_JGGA, OO_DIFF, OO_RATE, OO_GVOL, OO_MDHG, OO_MSHG, 
			OO_SIGA, OO_KOGA, OO_JEGA, 
			OO_CODE, OO_CDAY, OO_SAHG, OO_HAHG, OO_JJGA, OO_STCK, OO_START);
		break;
	case GU_FOREIGN:
		outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OS_TIME, OS_NAME, OS_JGGA, OS_DIFF, OS_RATE, OS_GVOL, OS_MDHG, OS_MSHG,
			OS_SIGA, OS_KOGA, OS_JEGA, 
			OS_CODE, OS_CDAY, OS_SAHG, OS_HAHG, OS_JJGA, OS_STCK, OS_START);
		break;
	case GU_COFUT:
		outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OCF_TIME, OCF_NAME, OCF_JGGA, OCF_DIFF, OCF_RATE, OCF_GVOL, OCF_MDHG, OCF_MSHG, 
			OCF_SIGA, OCF_KOGA, OCF_JEGA,
			OCF_CODE, OCF_CDAY, OCF_SAHG, OCF_HAHG, OCF_JJGA, OCF_STCK, OCF_START);
		break;
	case GU_COOPT:
		outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OCO_TIME, OCO_NAME, OCO_JGGA, OCO_DIFF, OCO_RATE, OCO_GVOL, OCO_MDHG, OCO_MSHG, 
			OCO_SIGA, OCO_KOGA, OCO_JEGA,
			OCO_CODE, OCO_CDAY, OCO_SAHG, OCO_HAHG, OCO_JJGA, OCO_STCK, OCO_START);
		break;
	}

	sprintf(pHeader, "%s%c%s\t%s", codeS, 0x7F, m_pMainWnd->m_strCode, outSymbol);

	return strlen(pHeader);
}

int CExtraMsg::GetInputTRInfo2(char *pHeader)
{
	CString	codeS = _T("");
	switch (m_pMainWnd->m_iDtUnit)
	{
	case GU_CODE:	
		codeS = IJ_CODE;	break;
	case GU_INDEX:	
		codeS = II_CODE;	break;
	case GU_FUTURE:	
		codeS = IF_CODE;	break;
	case GU_OPTION:
		codeS = IO_CODE;	break;
	case GU_FOREIGN:
		codeS = IS_CODE;	break;
	case GU_COFUT:
		codeS = ICF_CODE;	break;
	case GU_COOPT:
		codeS = ICO_CODE;	break;
	default:	return 0;
	}

	sprintf(pHeader, "%s%c%s\t", codeS, 0x7F, m_pMainWnd->m_strCode);

	return strlen(pHeader);
}

//////////////////////////////////////////////////////////

LRESULT CExtraMsg::SiseEvent(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case siseChgPalette:
		break;
	case SISE_TR_DATA:
		{
/*
			int	dataL = HIWORD(wParam);
			if (dataL == 0)
				return TRUE;

//			if (m_pMainWnd->m_bForeign)
//				return receiveFORData((char*)lParam, HIWORD(wParam));
//			else
				return ReceiveData((char*)lParam, HIWORD(wParam));
*/
			struct _trData* pTD = (struct _trData*)lParam;
			if (pTD->iLen[0] == 0)
			{
				return 0;
			}
			//return ReceiveData((char*)lParam, HIWORD(wParam));
			pTD->iSiseLen[0] =  ReceiveData(pTD->pcData[0], pTD->iLen[0]);
		}
		break;
	case siseRTMData:
		realtime((char*)lParam);
		break;
	case siseUnit:	break;
	case siseData:
		switch (HIWORD(wParam))
		{
		case siseCurr:
			sprintf((char*)lParam, "%s\t%s\t%s\t%s\t%s", m_curr, m_udyl, m_jjga, m_sahg, m_hahg);
			break;
		case siseCode:
			return (long)m_pMainWnd->m_strCode.operator LPCTSTR();
		case siseDate:
			sprintf((char*)lParam, "%s%s", m_date, m_start);
			break;
		case sisePivot:
			sprintf((char*)lParam, "%s\t%s\t%s", m_sahg, m_hahg, m_jjga);
			break;
		case siseStock:
			return (long)m_stock.operator LPCTSTR();
		case siseDollar:	break;
		}
		break;
	case siseFlash:
	case siseFont:	break;
	}

	return FALSE;
}

// 시간,종목명,현재가,대비,등락율,거래량,매도호가,매수호가,시가,고가,저가,
// 코드,최근영업일자,상한가,하한가,전일종가,주식수
int CExtraMsg::ReceiveData(char* pcData, int iLen)
{
	m_code = _T("");
	m_sahg = _T("");
	m_hahg = _T("");
	m_jjga = _T("");
	m_date = _T("");
	m_stock = _T("");
	m_start = _T("");

	int	index = 0;
	char	token[64];
	CString	hName = _T("");

	for (int ii = 0; ii < 11; ii++)
	{
		int tLen = m_pMainWnd->GetToken(&pcData[index], token);
		index += tLen;

		switch (ii)
		{
		case 1:	// 종목명
			hName.Format("%s", token);
			break;
		case 2:	// 현재가
			m_curr.Format("%s", token);
			break;
		case 4:	// 등락
			m_udyl.Format("%s", token);
			m_udyl.Remove('+');
			break;
		default:break;
		}		
	}

	for (int ii = 0; ii < 7; ii++)
	{
		int tLen = m_pMainWnd->GetToken(&pcData[index], token);
		index += tLen;

		switch (ii)
		{
		case 0:	m_code.Format("%s", token);	break;
		case 1:	m_date.Format("%s", token);	break;
		case 2:	m_sahg.Format("%s", token);	break;
		case 3:	m_hahg.Format("%s", token);	break;
		case 4:	m_jjga.Format("%s", token);	break;
		case 5:	m_stock.Format("%s", token);	break;
		case 6:	m_start.Format("%s", token);	break;
		}
	}

	m_pMainWnd->m_strJName.Format("%s", hName);
	if (hName.IsEmpty())
		return 0;

	return index;
}

// 시간,종목명,현재가,대비,등락율,거래량,시가,고가,저가,
// 코드,최근영업일자,전일종가,주식수
int CExtraMsg::receiveFORData(char *pData, int len)
{
	m_code = _T("");
	m_sahg = _T("");
	m_hahg = _T("");
	m_jjga = _T("");

	int	index = 0;
	char	token[64];
	CString	hName = _T("");

	for (int ii = 0; ii < 11; ii++)
	{
		int tLen = m_pMainWnd->GetToken(&pData[index], token);
		index += tLen;

		switch (ii)
		{
		case 1:	// 종목명
			hName.Format("%s", token);
			break;
		case 2:	// 현재가
			m_curr.Format("%s", token);
			break;
		case 4:	// 등락
			m_udyl.Format("%s", token);
			m_udyl.Remove('+');
			break;
		default:
			break;
		}		
	}

	m_sahg = m_hahg = _T("");
	for (int ii = 0; ii < 7; ii++)
	{
		int tLen = m_pMainWnd->GetToken(&pData[index], token);
		index += tLen;

		switch (ii)
		{
		case 0:	m_code.Format("%s", token);	break;
		case 1:	m_date.Format("%s", token);	break;
		case 4:	m_jjga.Format("%s", token);	break;
		case 5:	m_stock.Format("%s", token);	break;
		case 6:	m_start.Format("%s", token);	break;
		}
	}

	if (hName.IsEmpty())
		return 0;

	return index;
}

// 시간,현재가,대비,등락율,거래량,시가,고가,저가,미결제
void CExtraMsg::realtime(char *pData)
{
	int	index = 0;
	char	token[64];

	for (int ii = 0; ii < 9; ii++)
	{
		int tLen = m_pMainWnd->GetToken(&pData[index], token);
		index += tLen;

		if (token[0] == 0x00)	continue;

		switch (ii)
		{
		case 1:	// 현재가
			m_curr.Format("%s", token);
			break;
		case 3:	// 등락
			m_udyl.Format("%s", token);
			m_udyl.Remove('+');
			break;
		default:
			break;
		}
	}
}

