// ChartStockOpinionAddInImp.cpp: implementation of the CChartCommentAddInImp class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ChartCommentAddInImp.h"
#include "Include_AddIn.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartCommentAddInImp::CChartCommentAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase(p_pIChartOCX, p_pIAddInDllBase)
{
	m_bRun = FALSE;
	m_ToolTip.m_hWnd = NULL;
}

CChartCommentAddInImp::~CChartCommentAddInImp()
{
	ResetData();
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartCommentAddInImp::m_strAddInItemName = "USER_COMMENT";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP(CChartCommentAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnMouseMove)
END_ADDIN_EVENT_MAP()
//////////////////////////////////////////////////////////////////////

void CChartCommentAddInImp::OnDrawAfterGraph(HDC p_hDC)
{
	if( m_bRun == FALSE )		return;

	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	int	nR, nC, nStart, nEnd;
	CRect rcBlock;
	if (!GetChartBasicData(nStart, nEnd, nR, nC, rcBlock))
		return;

	CDC * pDC = CDC::FromHandle(p_hDC);

	CRgn rgn;
//	rgn.CreateRectRgn((int)rcGraph.left, (int)rcGraph.top, (int)rcGraph.right, (int)rcGraph.bottom);
	rgn.CreateRectRgn((int)rcBlock.left, (int)rcBlock.top, (int)rcBlock.right, (int)rcBlock.bottom);
	pDC->SelectClipRgn(&rgn);

	CFont *pOldFont = NULL;
	if (m_boldFont.GetSafeHandle() == NULL)
	{
		pOldFont = pDC->GetCurrentFont();
		LOGFONT lf;
		pOldFont->GetLogFont(&lf);
		lf.lfWeight = FW_BOLD;
		m_boldFont.CreateFontIndirect(&lf);
	}

	pOldFont = pDC->SelectObject(&m_boldFont);

 	IPacket *pIPacketForLow = m_pIDefaultPacketManager->GetPacket(_MTEXT( C0_LOW));
	IPacket *pIPacketForHigh = m_pIDefaultPacketManager->GetPacket(_MTEXT( C0_HIGH));
	if (m_arrCommentList.GetSize() <= 0)
		return;

	//	[sashia]	임시
	if ((pIPacketForLow == NULL) || (pIPacketForHigh == NULL))
		return;
	//	[sashia]	임시

	try
	{
		CPoint point;
		CString strDate;
		double dPrice;
		int nItemWidth = rcBlock.Width() / (nEnd - nStart);

//		AddupDrawData(TRUE, nStart, nEnd);		//	매수 시간별 합계
//		AddupDrawData(FALSE, nStart, nEnd);		//	매도 시간별 합계

		CCommentItem* pObj = NULL;
		int nIndex=0, nBeforeIdx=0, nBuySum=0, nSellSum=0;
		int nSize = m_arrCommentList.GetSize();
		for (int i=0; i<nSize; i++)
		{
			pObj = m_arrCommentList.GetAt(i);
			if (pObj->m_nIndex < 0)
				continue;

//			if (pObj->m_lSumOrdCnt <= 0)
//				continue;

			point.x = (long)((pObj->m_nIndex-nStart) * (rcBlock.Width()+1) + (rcBlock.Width()+1) / 2.) /
						(nEnd - nStart + 1) + rcBlock.left+2;

			if (!pObj->m_bBuy)
			{
				if (!pIPacketForHigh->GetData(pObj->m_nIndex, dPrice))
					throw this;
			}
			else
			{
				if (!pIPacketForLow->GetData(pObj->m_nIndex, dPrice))
					throw this;
			}

			point.y = m_pIChartOCX->ConvertDataToYPosition(dPrice, m_dViewMax, m_dViewMin,
						m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);

			pObj->SetPosition(pDC, &point);
			pObj->Draw(pDC);
		}
	}
	catch (CException*)
	{
		pDC->SelectClipRgn(&rgn, NULLREGION);
		rgn.DeleteObject();

		pIPacketForLow->Release();
		pIPacketForHigh->Release();

		return;
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectClipRgn(&rgn, NULLREGION);
	rgn.DeleteObject();
	pIPacketForLow->Release();
	pIPacketForHigh->Release();
}

/*
void CChartCommentAddInImp::AddupDrawData(BOOL bBuy, int nStart, int nEnd)
{
	long lDate;
	CCommentItem *pObj = NULL, *pBeforeObj=NULL;
	int nBeforeIdx=0, nSum=0;
	int nSize = m_arrCommentList.GetSize();

	for (int i=0; i<nSize; i++)
	{
		pObj = m_arrCommentList.GetAt(i);

		if (bBuy != pObj->m_bBuy)
			continue;

		pObj->m_nIndex = GetIndexFromValue(pObj->m_strDateTime, nStart, nEnd, lDate);
		if (pObj->m_nIndex < 0)
			continue;

		if (nBeforeIdx == pObj->m_nIndex)
		{
			nSum += pObj->m_lOrdCnt;

			if (pBeforeObj)
				pBeforeObj->m_lSumOrdCnt = 0;

			pObj->m_lSumOrdCnt = nSum;

			pBeforeObj = pObj;

			continue;
		}

		nSum = pObj->m_lOrdCnt;
		pObj->m_lSumOrdCnt = nSum;
		nBeforeIdx = pObj->m_nIndex;
		pBeforeObj = pObj;
	}
}
*/
BOOL CChartCommentAddInImp::GetChartBasicData(int &p_nStartIndex, int &p_nEndIndex, int &p_nR, int &p_nC, CRect &p_rctBlockRegion)
{
	// 현 Data View의 Start/End Index를 구한다.
	if (!m_pIChartOCX->GetViewStartEndIndex( p_nStartIndex, p_nEndIndex))
		return FALSE;

	int nV, nG;

	// 가격차트가 포함된 Block의 Index를 구한다.
//	if (!m_pIChartManager->GetIndexOfGraph( "가격차트", p_nR, p_nC, nV, nG))
	if (!m_pIChartManager->GetIndexOfGraph(_MTEXT( C2_PRICE_CHART), p_nR, p_nC, nV, nG))
		return FALSE;

	// Block의 Log/뒤집기 설정을 확인하여 정확하게 Data를 변환한다. (가격<->Y Pos)
	IBlock *pIBlock = m_pIChartManager->GetBlock(p_nR, p_nC);
	if (pIBlock)
	{
		pIBlock->GetLogAndReverseScaleFlag(m_bLog, m_bReverse);
		pIBlock->GetVScaleGroupMinMax(nV, m_dViewMin, m_dViewMax);
	   	pIBlock->GetGraphRegions(m_rctGraphRegion, p_rctBlockRegion);

		// 6. Block에서의 Graph Resion을 구한다.
		m_rctGraphDrawingRegion = pIBlock->GetGraphDrwingRegion();

		pIBlock->Release();
		pIBlock = NULL;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 데이터로부터 인넥스 얻기
int CChartCommentAddInImp::GetIndexFromValue(CString strTime, int nStartIndex, int nEndIndex, long& lDate)
{
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	if( !pIPacket) return -1;

	strTime.TrimRight();
	int nIndex = pIPacket->GetIndexFromData( atof(strTime));
	pIPacket->Release();

	return nIndex;
	

/*
//	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( "자료일자");
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket(_MTEXT( C0_DATE_TIME));

	int lx = 0;
	try
	{
		ILPCSTR is = pIPacket->GetType();
		CString s = is;
		if (s == "YYYYMMDD")
			lx = atol(strTime.Left(8));
		else if (s == "YYYYMM")
			lx = atol(strTime.Left(6));
		else if (s == "MMDDHHMM")
		{
			if (strTime.GetLength() >= 12)
				lx = atol(strTime.Mid(4,8));
		}

		lDate = lx;

		double dTime;
		for (int i=0; i <= nEndIndex ;i++)
		{
			if (!pIPacket->GetData(i, dTime))
				throw this;

			if (s == "YYYYMM" && dTime >= 1000000)
				dTime = dTime/100;

			if (lx == (long)dTime)	// 있으면...
			{
				pIPacket->Release();
				return (i);
			}
			else if (lx < (long)dTime)
			{
				pIPacket->Release();
				return (i-1);
			}
		}
	}
	catch (CChartCommentAddInImp *)
	{
		pIPacket->Release();
		return -1;
	}

	pIPacket->Release();
*/
	return -1; 
}

void CChartCommentAddInImp::OnResetChart()
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	ResetData();
}

void CChartCommentAddInImp::ResetData()
{
	CCommentItem *pItem = NULL;
	int nSize = m_arrCommentList.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pItem = m_arrCommentList.GetAt(i);
		if (pItem != NULL)
		{
			delete pItem;
			pItem = NULL;
		}
	}
	m_arrCommentList.RemoveAll();

	if (m_boldFont.GetSafeHandle() == NULL)
		m_boldFont.DeleteObject();
}

BOOL CChartCommentAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	int nCommandType = atoi(p_szCommandName);

	switch (nCommandType)
	{
	case 0:			//	주문데이터 입력.
		SetCommentData(p_szData);
		break;
	case 1:			//	
		if( stricmp(p_szData, "ON") == 0 )
		{
			m_bRun = TRUE;
		}
		else
		{
			m_bRun = FALSE;
		}
		break;
	case 2:			//	ResetData
		ResetData();
		break;
	}

	return TRUE;
}

BOOL CChartCommentAddInImp::InvokeAddIn( int p_nCommandType, long p_lData)
{
	if(p_nCommandType == 5)
	{
	}
	else if(!p_lData) return FALSE;	

	if(p_nCommandType == 1 || p_nCommandType == 2)			// ADD OPEN_POSITION / PENDING_ORDER
	{
	 	CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*>* arrOrderList = (CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*>*)p_lData;
		for(int nIndex = 0; nIndex < arrOrderList->GetSize(); nIndex++)
		{
			FX_CHE_STRUCT* pDataPosition = (FX_CHE_STRUCT*)arrOrderList->GetAt(nIndex);
			SetCommentData(pDataPosition);
		}
	}
	else if(p_nCommandType == 3 || p_nCommandType == 4)	// ADD OPEN_POSITION / PENDING_ORDER
	{
		FX_CHE_STRUCT* pDataPosition = (FX_CHE_STRUCT*)p_lData;
		if(pDataPosition)
			SetCommentData(pDataPosition);
	}
	else if(p_nCommandType == 5)	// reset
	{
		ResetData();
	}
	return TRUE;
}


void CChartCommentAddInImp::SetCommentData(CString strData)
{
	CString strBuff = strData;
	CStringArray arr;
	TokenizeEx(strBuff, &arr, ":");

	if (arr.GetSize() < 3)
		return ;

	int nStartIndex, nEndIndex;
	long lDate;

	if (!m_pIChartOCX->GetViewStartEndIndex(nStartIndex, nEndIndex))
		return;

	CString strDateTime	= arr.GetAt(0);
	CString strComment	= arr.GetAt(1);
	CString strIsBuy	= arr.GetAt(2);

	CCommentItem* pItem	= new CCommentItem;

	pItem->m_nIndex			= GetIndexFromValue(strDateTime, nStartIndex, nEndIndex, lDate);
	pItem->m_strDate.Format("%d", (int)lDate);
	pItem->m_bBuy			= atoi(strIsBuy);
	pItem->m_strDateTime	= strDateTime;
	pItem->m_strComment		= strComment;

	m_arrCommentList.Add(pItem);

	m_pIChartOCX->InvalidateControl();
}

void CChartCommentAddInImp::TokenizeEx(CString strSrc, CStringArray* pArr, const char* pszTokens)
{
	CString strRes = _T("");
	int nStart, nCur = 0;
	int nTokenLen = strlen(pszTokens);
	nStart = strSrc.Find(pszTokens, nCur);

	while (nStart >= 0)
	{
		strRes = strSrc.Mid(nCur, nStart-nCur);
		pArr->Add(strRes);
		nCur = nStart + nTokenLen;
		nStart = strSrc.Find(pszTokens, nCur);
		if (strSrc.GetLength() <= nStart)
			break;
	}
}

void CChartCommentAddInImp::SetCommentData(FX_CHE_STRUCT* pDataPosition)
{
	if( memcmp(pDataPosition->szKeyType, "CH01", 4) != 0 )
		return;

	int nStartIndex, nEndIndex;
	long lDate;

	if (!m_pIChartOCX->GetViewStartEndIndex(nStartIndex, nEndIndex))
		return;

	CCommentItem* pItem	= new CCommentItem;

	CString strDateTime(pDataPosition->szExeTime, sizeof(pDataPosition->szExeTime));
	CString strComment;
	CString strIsBuy;
	if( memcmp(pDataPosition->szTradeType, "11", 2) == 0 )
		strComment = "진입";
	else if( memcmp(pDataPosition->szTradeType, "12", 2) == 0 )
		strComment = "청산";
	else if( memcmp(pDataPosition->szTradeType, "13", 2) == 0 )
		strComment = "마진콜";
	else
		return;
	
	if( memcmp(pDataPosition->szSide, "082", 3) == 0 || memcmp(pDataPosition->szSide, "079", 3) == 0 )
		strIsBuy = "1";
	else
		strIsBuy = "0";
/*
//aaaaaaaaaaaaaaaa임시
	strDateTime.TrimLeft();		strDateTime.TrimRight();
	if( strDateTime.GetLength() < 14 )
	{
		SYSTEMTIME st;
		GetLocalTime( &st);
//		__int64 nLocalTime = st.wYear * 10000000000 + st.wMonth * 100000000 + st.wDay * 1000000 + st.wHour * 10000 + st.wMinute * 100 + st.wSecond;

		strDateTime.Format("%04d%02d%02d%s", st.wYear, st.wMonth, st.wDay, strDateTime);
	}
*/
//aaaaaaaaaaaaaaaa임시끝
	pItem->m_nIndex			= GetIndexFromValue(strDateTime, nStartIndex, nEndIndex, lDate);
	pItem->m_strDate.Format("%d", (int)lDate);
	pItem->m_bBuy			= atoi(strIsBuy);
	pItem->m_strDateTime	= strDateTime;
	pItem->m_strComment		= strComment;

	m_arrCommentList.Add(pItem);

	m_pIChartOCX->InvalidateControl();
}

BOOL CChartCommentAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	if(!m_bRun)		return FALSE;

	BOOL	 bLineOver = FALSE;
//1. 마우스 오버시
//2. 영역 밖일 경우 

	//가격차트가 포함된 Block의 Index를 구한다.
		int nR, nC, nV, nG;
		ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, "DEFAULT");
		if( szGraphName.IsEmpty()) return TRUE;

		IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC );
		if( !pIBlock) return TRUE;

		CRect rctNoFrameAndMargin;
		pIBlock->GetGraphRegions(m_rctGraphRegion, rctNoFrameAndMargin);
		if( m_rctGraphRegion.IsRectEmpty() )
			return FALSE;
		
		m_rctGraphDrawingRegion	= pIBlock->GetGraphDrwingRegion();
		if( m_rctGraphDrawingRegion.IsRectEmpty() )
			return FALSE;

		// 4. GraphRegion이 넘어서면 영역을 GraphRegion으로 제한한다.
		if( m_rctGraphRegion.top >= point.y )
			point.y = m_rctGraphRegion.top;

		if( m_rctGraphRegion.bottom <= point.y )
			point.y = m_rctGraphRegion.bottom;

		m_bLog = pIBlock->IsLogScale();
		m_bReverse = pIBlock->IsVScaleInvert( nV);
		


	if(SetItemStatusOnMouseMove(point))
	{				
		SetToolTipText(m_strToolTipText, TRUE);
	}
	else
		SetToolTipText(" ", FALSE);


	
	m_pIChartOCX->InvalidateControl();


	if(m_strToolTipText.IsEmpty())
		return TRUE;
	
	if(m_ToolTip.m_hWnd == NULL)
		SetToolTipText(m_strToolTipText, true);
	
	MSG msg;
	msg.hwnd = m_pChartCWnd->GetSafeHwnd();
	msg.message = WM_MOUSEMOVE;
	msg.wParam = 0;
	msg.lParam = 0;
	m_ToolTip.RelayEvent(&msg);

	return TRUE;
}

BOOL CChartCommentAddInImp::SetItemStatusOnMouseMove(CPoint point)
{
	BOOL bLineOver = FALSE;

	CCommentItem* pItem;
	int nSize = m_arrCommentList.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pItem = m_arrCommentList.GetAt(i);
		if(pItem) 
		{
			if(pItem->IsInRegion(point))
			{
				bLineOver = TRUE;
				//pItem->m_bShow = TRUE;
				//pItem->m_bMouseOver = TRUE;
				m_strToolTipText = pItem->GetToolTipText();
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CChartCommentAddInImp::InitToolTip()
{
	if(m_ToolTip.m_hWnd != NULL)
		return;
	
	m_ToolTip.Create(m_pChartCWnd);
	m_ToolTip.Activate(FALSE);
	m_ToolTip.SetMaxTipWidth(200);
}

void CChartCommentAddInImp::AddToolInTooltip(const CString& strText, const bool bActivate)
{
	// tooltip의 text가 없을 경우에 추가 한다.
	if(m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(m_pChartCWnd->GetSafeHwnd(), rectBtn);
		m_ToolTip.AddTool(m_pChartCWnd, (LPCTSTR)strText, rectBtn, 1);
	}
	
	// ooltip text 를 set
	m_ToolTip.UpdateTipText((LPCTSTR)strText, m_pChartCWnd, 1);
	m_ToolTip.Activate(bActivate);
}

void CChartCommentAddInImp::SetToolTipText(const CString& strText)
{
	m_strToolTipText = strText;
}

void CChartCommentAddInImp::SetToolTipText(const CString& strText, const bool bActivate)
{
	if(strText.IsEmpty())
		return;
	
	InitToolTip();
	AddToolInTooltip(strText, bActivate);
}
