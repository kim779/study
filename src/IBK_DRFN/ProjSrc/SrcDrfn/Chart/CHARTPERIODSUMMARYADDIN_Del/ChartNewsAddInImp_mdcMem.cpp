// ChartStockOpinionAddInImp.cpp: implementation of the CChartNewsAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartPeriodSummaryAddIn.h"
#include "resource.h"
#include "ChartNewsAddInImp.h"

#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h

#include "../../inc/RealUpdate.h"
#include "../../inc/IMasterDataMng.h"

//#include <winsock2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define IDC_HAND MAKEINTRESOURCE(32649)	// 손 모양 마우스 커서 정의 


//{{ 통신(소켓) 처리 모듈
#define DR_MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartNewsAddInImp::CChartNewsAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_bCentered = TRUE;
	m_nCommandType = 0;
	m_pBmp = NULL;
	m_TRComm = NULL;
	m_pNews = NULL;
	m_pCurNewsObj = NULL;
	//////////////////////////////////////////////////////////////////////////
	// 통신(소켓) 처리 모듈
	IGateManager* pGateMng;
	AfxGetIGateManager(pGateMng);
	if(pGateMng)
	{
		int nID = 2;
		m_pITrCommManager = (IAUTrCommManager*)pGateMng->GetInterface(nID);
		m_TRComm = m_pITrCommManager->AdviseTrComm();
		m_TRComm->ChangeCallback(&m_xTrCommSite);
	}
	//////////////////////////////////////////////////////////////////////////
}

CChartNewsAddInImp::~CChartNewsAddInImp()
{
	// font.DeleteObject(); 
	if(m_pITrCommManager && m_TRComm)
		m_pITrCommManager->UnAdviseTrComm(m_TRComm);
	
	//AfxMessageBox("~CChartNewsAddInImp");
	POSITION pos = m_NewsObjList.GetHeadPosition();
	while (pos)
	{
		CNewsObj* pNews = m_NewsObjList.GetNextNewsObj(pos);
		delete pNews;
		pNews = NULL;

	}
	m_NewsObjList.RemoveAll();
	if(m_pBmp)
	{
		delete m_pBmp;
		m_pBmp = NULL;
	}
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartNewsAddInImp::m_strAddInItemName = "NEWS";	// <-- 각 기능 이름

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartNewsAddInImp)
	ONADDINEVENT(OnDrawAfterGraph)	
	ONADDINEVENT(OnDrawBeforeGraph)
	ONADDINEVENT(OnUpdatePacketData)
	ONADDINEVENT(OnMouseMove)
	ONADDINEVENT(OnLButtonDown)
	ONADDINEVENT(OnLButtonUp)
	ONADDINEVENT(OnRButtonDown)
	ONADDINEVENT(OnRButtonUp)
	ONADDINEVENT(OnCmdMsg)
	ONADDINEVENT(OnPacketData)
	ONADDINEVENT(OnResetChart)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////
void CChartNewsAddInImp::OnResetChart()
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	//////////////////////////////////////////////////////////////////////////
	// 기존 데이터 클리어
	POSITION pos = m_NewsObjList.GetHeadPosition();
	while (pos)
	{
		CNewsObj* pNews = m_NewsObjList.GetNextNewsObj(pos);
		delete pNews;
		pNews = NULL;
	}
	m_NewsObjList.RemoveAll();
	m_nCommandType = 0;
	//////////////////////////////////////////////////////////////////////////

}

void CChartNewsAddInImp::OnPacketData(const char *p_szPacketNames, BOOL p_bWithReset)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	//////////////////////////////////////////////////////////////////////////
	// 기존 데이터 클리어
	POSITION pos = m_NewsObjList.GetHeadPosition();
	while (pos)
	{
		CNewsObj* pNews = m_NewsObjList.GetNextNewsObj(pos);
		delete pNews;
		pNews = NULL;
	}
	m_NewsObjList.RemoveAll();
	m_nCommandType = 0;
	//////////////////////////////////////////////////////////////////////////

/*
	//////////////////////////////////////////////////////////////////////////
	// 기존 데이터 클리어
	POSITION pos = m_NewsObjList.GetHeadPosition();
	while (pos)
	{
		CNewsObj* pNews = m_NewsObjList.GetNextNewsObj(pos);
		delete pNews;
		pNews = NULL;
	}
	m_NewsObjList.RemoveAll();
	//////////////////////////////////////////////////////////////////////////

	int		nR, nC ;
	if( !GetChartBasicData(nStartIndex, nEndIndex, nR, nC, rctBlockRegion, rctGraphRegion))
		throw this;

	CClientDC dc(NULL);
	m_dcMem.DeleteDC();
	m_dcMem.CreateCompatibleDC(&dc);
	if(m_pBmp)
	{
		delete m_pBmp;
		m_pBmp = NULL;
	}
	m_pBmp = new CBitmap;
	m_pBmp->CreateCompatibleBitmap(&dc, rctBlockRegion.Width(), rctBlockRegion.Height());
	m_dcMem.SelectObject(m_pBmp);

	if(m_nCommandType)
		RequestSiseData();
		*/
}

BOOL CChartNewsAddInImp::OnCmdMsg(UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	//AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		switch( nID)
		{
		case ID_KSA_NEWS_NEWSINFO:
			if( p_pICmdUI)
			{
				p_pICmdUI->Enable(TRUE);
				//////////////////////////////////////////////////////////////////////////
				// 메뉴의 텍스트를 일자:카운트 형태로 변경
				CString strMenu;
				strMenu.Format("%s, 건수:%d", m_pCurNewsObj->m_strDate, m_pCurNewsObj->m_lCntNews);
				p_pICmdUI->SetText(strMenu);
				//////////////////////////////////////////////////////////////////////////
			}
			break;
		default:
			return FALSE;
		}
	}

	if( CN_COMMAND == nCode)
	{
		switch(nID)
		{
		case ID_KSA_NEWS_NEWSINFO:
			OnNewsInfoRequest();
			break;
		default:
			return FALSE;
		}
	}
	return TRUE;
}

void CChartNewsAddInImp::OnUpdatePacketData(BOOL p_bTrUpdate)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	
}

BOOL CChartNewsAddInImp::OnMouseMove(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	m_pIChartOCX->SetCursorHandle(::LoadCursor(NULL, IDC_ARROW));
	POINT ptTemp = point;
	point.x -= m_ptOffSet.x;
	point.y -= m_ptOffSet.y;

	POSITION pos = m_NewsObjList.GetHeadPosition();
	while(pos)
	{
		CNewsObj* pNews = m_NewsObjList.GetNextNewsObj(pos);
		if(!pNews) continue;
		if( pNews->IsInRegion(point) )
		{
			SetCursor(::LoadCursor(NULL, IDC_HAND));
			return TRUE;
		//	break;
		}
	}
	return FALSE;
}

BOOL CChartNewsAddInImp::OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;
	m_dwMouseActionID = p_dwMouseActionID;

	POINT ptTemp = point; 
	point.x -= m_ptOffSet.x;
	point.y -= m_ptOffSet.y;

	CDC * pDC = CDC::FromHandle(p_hDC);

	POSITION pos = m_NewsObjList.GetHeadPosition();
	while(pos)
	{
		CNewsObj* pNews = m_NewsObjList.GetNextNewsObj(pos);
		if(!pNews) continue;
		if( pNews->IsInRegion(point) )
		{

			m_pCurNewsObj = pNews;
			m_pCurNewsObj->ButtonDownAt(point);//SetPos(point, m_pCurSelectedLine->m_CurClickedPosition);
			m_pCurNewsObj->SetPos(point);
			//m_pCurNewsObj->Erase(&m_dcMem);
			POSITION pos2 = m_NewsObjList.Find(m_pCurNewsObj);
			m_NewsObjList.RemoveAt(pos2);
			m_NewsObjList.AddHeadNewsObj(m_pCurNewsObj);
			m_pCurNewsObj->m_bSelected = TRUE;
			m_pCurNewsObj->Draw(&m_dcMem);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CChartNewsAddInImp::OnLButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	m_pIChartOCX->SetCursorHandle(::LoadCursor(NULL, IDC_ARROW));

	if(!m_nCommandType) return FALSE;
	POINT ptTemp = point;  
	point.x -= m_ptOffSet.x;
	point.y -= m_ptOffSet.y;

	if(m_dwMouseActionID == p_dwMouseActionID)
	{
		if(!m_pCurNewsObj) return FALSE;
		if(m_pCurNewsObj->IsInRegion(point))
		{
			m_pIChartOCX->SetCursorHandle(::LoadCursor(NULL, IDC_HAND));

			//////////////////////////////////////////////////////////////////////////
			// popup menu 생성
			CPoint tmpPt = m_pCurNewsObj->m_ptAt;
			tmpPt.x += m_pCurNewsObj->m_size.cx;
			::ClientToScreen(m_pIChartOCX->GetOcxHwnd(),&tmpPt);

			CMenu menu;
			VERIFY(menu.LoadMenu(IDR_MENU1));


			m_hPopupMenu = GetSubMenu(menu.GetSafeHmenu(),2);
			TrackPopupMenu(m_hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, tmpPt.x, tmpPt.y,	0, m_pIChartOCX->GetOcxHwnd(), NULL);
			DestroyMenu(m_hPopupMenu);
			//////////////////////////////////////////////////////////////////////////
		return TRUE;
		}
	}
	return FALSE;
}

BOOL CChartNewsAddInImp::OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!m_nCommandType) return FALSE;

	return FALSE;
}

BOOL CChartNewsAddInImp::OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	return FALSE;
}


BOOL CChartNewsAddInImp::OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 차트 뒤쪽에 그리기
void CChartNewsAddInImp::OnDrawBeforeGraph(HDC p_hDC)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
}

//////////////////////////////////////////////////////////////////////////
// 차트 앞쪽에 그리기
void CChartNewsAddInImp::OnDrawAfterGraph(HDC p_hDC)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

 	IPacket *pIPacketForLow = m_pIPacketManager->GetPacket( "저가");
	try
	{
		CDC * pDC = CDC::FromHandle(p_hDC);
		int	nR, nC ;

		CRect rcPrev = rctGraphRegion;
		if( !GetChartBasicData(nStartIndex, nEndIndex, nR, nC, rctBlockRegion, rctGraphRegion))
			return;
		if(m_dcMem.GetSafeHdc())
		{
			// 화면 크기 변경에 따른 그리기 영역 변경
			CClientDC dc(NULL);
			if(!m_dcMem)
			{
				m_dcMem.CreateCompatibleDC(&dc);
			}
			if(rcPrev != rctGraphRegion)
			{
				if(m_pBmp)
				{
					delete m_pBmp;
					m_pBmp = NULL;
				}
				m_pBmp = new CBitmap;
				m_pBmp->CreateCompatibleBitmap(&dc, rctGraphRegion.Width(), rctGraphRegion.Height());
				m_dcMem.SelectObject(m_pBmp);
			}
//			TRACE1("\nHeight:%d",rctGraphRegion.Height());
			m_dcMem.BitBlt(0, 0, rctGraphRegion.Width(), rctGraphRegion.Height(), pDC, rctGraphRegion.left,rctGraphRegion.top,SRCCOPY);
			//pDC->SetClipRgn();
		}

		double dLow;

		POSITION pos = m_NewsObjList.GetHeadPosition();
		while (pos)
		{
			CNewsObj* pNews = m_NewsObjList.GetNextNewsObj(pos);
			pNews->Initialize();
			if( !pIPacketForLow->GetData( pNews->m_nIndex, dLow)) throw this;
			GetPointFromIndex(pNews->m_nIndex,0,pNews->m_ptAt);
			pNews->m_ptAt.y = m_pIChartOCX->ConvertDataToYPosition(dLow, m_dViewMax, m_dViewMin,rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			pNews->Draw(&m_dcMem);
		}

		pDC->BitBlt(rctGraphRegion.left, rctGraphRegion.top, rctGraphRegion.Width(), rctGraphRegion.Height(), &m_dcMem, 0,0,SRCCOPY);

	}
	catch (CException*)
	{
		pIPacketForLow->Release();
		return;		
	}

	pIPacketForLow->Release();
}
//////////////////////////////////////////////////////////////////////////
// Chart ocx와 통신 시작
BOOL CChartNewsAddInImp::InvokeAddIn(int p_nCommandType, long p_lData)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	if(!p_lData) return TRUE;
	m_strCode.Format("%06d",p_lData);


	// p_nCommandType = 3이면 기존 설정 계속
	if(p_nCommandType == 3)
		p_nCommandType = 0;

	switch(p_nCommandType)
	{
	case 0:	// Stop, & hide
		{
			m_nCommandType = p_nCommandType;

			//////////////////////////////////////////////////////////////////////////
			// 기존 데이터 클리어
			POSITION pos = m_NewsObjList.GetHeadPosition();
			while (pos)
			{

				CNewsObj* pNews = m_NewsObjList.GetNextNewsObj(pos);
				delete pNews;
				pNews = NULL;
			}
			m_NewsObjList.RemoveAll();
			//////////////////////////////////////////////////////////////////////////
			
			if(m_pBmp)
			{
				delete m_pBmp;
				m_pBmp = NULL;
			}
			m_pIChartOCX->InvalidateControl();
		}
		break;
	case 1:	// Run
		{
			m_nCommandType = p_nCommandType;

			//////////////////////////////////////////////////////////////////////////
			// 기존 데이터 클리어
			POSITION pos = m_NewsObjList.GetHeadPosition();
			while (pos)
			{
				CNewsObj* pNews = m_NewsObjList.GetNextNewsObj(pos);
				delete pNews;
			}
			m_NewsObjList.RemoveAll();
			//////////////////////////////////////////////////////////////////////////

			int		nR, nC ;
			if( !GetChartBasicData(nStartIndex, nEndIndex, nR, nC, rctBlockRegion, rctGraphRegion))
				return FALSE;

			CClientDC dc(NULL);
			m_dcMem.DeleteDC();
			m_dcMem.CreateCompatibleDC(&dc);
			if(m_pBmp)
			{
				delete m_pBmp;
				m_pBmp = NULL;
			}
			m_pBmp = new CBitmap;
			m_pBmp->CreateCompatibleBitmap(&dc, rctGraphRegion.Width(), rctGraphRegion.Height());
			m_dcMem.SelectObject(m_pBmp);

			RequestSiseData();
		}
		break;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 차트의 기본 데이터 얻기
BOOL CChartNewsAddInImp::GetChartBasicData(	int &p_nStartIndex, int &p_nEndIndex, int &p_nR, int &p_nC, CRect &p_rctBlockRegion, CRect &p_rctGraphRegion)
{
	// 현 Data View의 Start/End Index를 구한다.
	if( !m_pIChartOCX->GetViewStartEndIndex( p_nStartIndex, p_nEndIndex)) return FALSE;
	int nV, nG;

	// 가격차트가 포함된 Block의 Index를 구한다.
	if( !m_pIChartManager->GetIndexOfGraph( "가격차트", p_nR, p_nC, nV, nG)) return FALSE;
	
	// Block의 Log/뒤집기 설정을 확인하여 정확하게 Data를 변환한다. (가격<->Y Pos)
	IBlock *pIBlock = m_pIChartManager->GetBlock( p_nR, p_nC);
	if( pIBlock)
		{
			pIBlock->GetLogAndReverseScaleFlag( m_bLog, m_bReverse);
			pIBlock->GetVScaleGroupMinMax(nV, m_dViewMin, m_dViewMax);
	   		pIBlock->GetGraphRegions(p_rctGraphRegion, p_rctBlockRegion );
			//p_rctGraphRegion = pIBlock->GetGraphDrwingRegion();		
			// Scale에 따른 offset 얻기//////////////////////////////////
			RECT rctTopScale, rctBottomScale,rctLeftScale,rctRightScale;
			
			pIBlock->GetHorzScaleRegion(rctTopScale, rctBottomScale);
			pIBlock->GetVertScaleRegion(rctLeftScale, rctRightScale);
			m_ptOffSet.x = rctLeftScale.right;
			m_ptOffSet.y = rctTopScale.bottom;
			//////////////////////////////////////////////////////
			pIBlock->Release();
			pIBlock = NULL;
		}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 좌표로부터 데이터 값 얻기
BOOL CChartNewsAddInImp::GetValueFromPoint(CPoint& ptAt/*IN, OUT*/, long &x/*OUT*/, double &y/*OUT*/)
{
	int ix, iy;
	if(GetIndexFromPoint(ptAt, ix, iy) == -1) return FALSE;
	if(GetValueFromIndex(ix, iy, x, y) == -1) return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 좌표로부터 인덱스 얻기
int CChartNewsAddInImp::GetIndexFromPoint(CPoint& pt/*IN, OUT*/, int &x/*IN*/, int &y/*IN*/)
{
	if(!m_pIChartManager->GetBlockIndex(pt,y, x)) return R_ERROR;
	IBlock * pIBlock = m_pIChartManager->GetBlock(y, x);
	if( !pIBlock) return FALSE;
	CRect rctBlockRegion, rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( rctBlockRegion, rctNoFrameAndMargin);
	CRect rctGraphRegion = pIBlock->GetGraphDrwingRegion();
	pIBlock->Release();
	pIBlock = NULL;
	rctBlockRegion.left = rctGraphRegion.left;
	rctBlockRegion.right = rctGraphRegion.right;
	if( !rctBlockRegion.PtInRect( pt)) return R_ERROR;
	int nStartIndex, nEndIndex;
	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) return R_ERROR;
	// 1.3 지정된 Mouse Point의 Data Index를 구한다.
	x = y = nStartIndex + ( ( pt.x - rctBlockRegion.left) * ( nEndIndex - nStartIndex + 1)) / rctBlockRegion.Width();

	if(m_bCentered)	// 중심선에 정렬 
		pt.x = (x*rctBlockRegion.Width() -nStartIndex*rctBlockRegion.Width() + rctBlockRegion.Width()/2.)/( nEndIndex - nStartIndex + 1) + rctBlockRegion.left;
	else
		pt.x = (x*rctBlockRegion.Width() - nStartIndex*rctBlockRegion.Width())/( nEndIndex - nStartIndex + 1) + rctBlockRegion.left;

	POINT ptAt;
	ptAt.x = pt.x;
	ptAt.y = pt.y;
	::ClientToScreen(m_pIChartOCX->GetOcxHwnd(),&ptAt);
	::SetCursorPos(ptAt.x, ptAt.y);
	return R_OK;
}

//////////////////////////////////////////////////////////////////////////
// 인덱스로부터 데이터 값 얻기
int CChartNewsAddInImp::GetValueFromIndex(int ix, int iy, long &lx, double &dy)
{
	// 2.1 자료일자 Packet을 구한다.
	IPacket *pIPacket = m_pIPacketManager->GetPacket( "자료일자");
	if( !pIPacket) return FALSE;

	try
	{
		double dTime;
		if( !pIPacket->GetData( ix, dTime)) throw this;
		lx = ( long)dTime;
	}
	catch( CChartNewsAddInImp *)
	{
		pIPacket->Release();
		return R_ERROR;
	}

	// 종가를 구한다
	pIPacket = m_pIPacketManager->GetPacket( "종가");
	if( !pIPacket) return FALSE;

	try
	{
		if( !pIPacket->GetData( iy, dy)) throw this;
	}
	catch( CChartNewsAddInImp *)
	{
		pIPacket->Release();
		return R_ERROR;
	}

	pIPacket->Release();


	return R_OK;
}

//////////////////////////////////////////////////////////////////////////
// 인덱스로부터 좌표 구하기
int CChartNewsAddInImp::GetPointFromIndex(int x, int y, CPoint &ptIndex)
{
//	ptIndex.x = (x*rctBlockRegion.Width() - nStartIndex*rctBlockRegion.Width())/( nEndIndex - nStartIndex + 1) + rctBlockRegion.left;
	ptIndex.x = (x*rctBlockRegion.Width() -nStartIndex*rctBlockRegion.Width() + rctBlockRegion.Width()/2.)/( nEndIndex - nStartIndex + 1) + rctBlockRegion.left;

//	ptIndex.y = m_pIChartOCX->ConvertDataToYPosition(dLocation,m_dViewMax, m_dViewMin,rctBlockRegion.top, rctBlockRegion.bottom, m_bLog, m_bReverse);
	ptIndex.x -= m_ptOffSet.x;
	ptIndex.y -= m_ptOffSet.y;

	return R_OK;
}

//////////////////////////////////////////////////////////////////////////
// 데이터로부터 인넥스 얻기
int CChartNewsAddInImp::GetIndexFromValue(long lx, double dy, int &ix, int &iy)
{
	IPacket *pIPacket = m_pIPacketManager->GetPacket( "자료일자");

	try
	{
		double dTime;
		for(;iy< nEndIndex/* - nStartIndex*/+1;iy++)
		{
			if( !pIPacket->GetData( iy, dTime)) throw this;
			if(lx == ( long)dTime)	// 있으면...
			{
				ix = iy;
				pIPacket->Release();
				iy++;
				return R_OK;
			}
			else if(lx < ( long)dTime)
			{
				//iy++;
				pIPacket->Release();
				return R_ERROR;
			}
		}
		
	}
	catch( CChartNewsAddInImp *)
	{
		pIPacket->Release();
		return R_ERROR;
	}

	pIPacket->Release();
	return R_ERROR; 
}

typedef struct _OPEN_MAP
{
	CStringArray strTags;
	CStringArray strValues;
	CString strMap;
}structOpenMap;
//////////////////////////////////////////////////////////////////////////
// 선택된 news의 날짜 외부 전달
#define UM_OPEN_MAP (WM_USER + 603)	

void CChartNewsAddInImp::OnNewsInfoRequest()
{
	structOpenMap om;

	m_pCurNewsObj->m_lxData;
	GetValueFromIndex(m_pCurNewsObj->m_nIndex, 0, m_pCurNewsObj->m_lxData, m_pCurNewsObj->m_dyData);
	m_pIChartOCX->FireAddInEvent("ChartPeriodSummaryAddIn.dll:PERIOD_SUMMARY", 1, 0,0,m_pCurNewsObj->m_lxData);

	// 메인에 전달
	CWnd* pMainWnd = AfxGetMainWnd();
	CString s;
	s.Format("%o",pMainWnd);
//	AfxMessageBox(s);
//	m_pCurNewsObj->m_lxData, m_pCurNewsObj->m_lCntNews
 	CString strCount, strDate, strDateSeq;
 	strCount.Format("%d",m_pCurNewsObj->m_lCntNews);
 	strDate.Format("%d",m_pCurNewsObj->m_lxData);
 	strDateSeq = strDate + "99999999999999";
 
//	om.nCount = m_pCurNewsObj->m_lCntNews;
//	om.lDate = m_pCurNewsObj->m_lxData;
//	om.strCode = m_strCode;
//	om.strDateSeq = strDateSeq;

	om.strTags.Add("Count");
	om.strTags.Add("hfis120aq_일자");
	om.strTags.Add("DateSeq");
	om.strTags.Add("hfis120aq_종목코드");

	om.strValues.Add(strCount);
	om.strValues.Add(strDate);
	om.strValues.Add(strDateSeq);
	om.strValues.Add(m_strCode);
	
	om.strMap = "hfis120aq";
//	#define WMU_SETVAR WM_USER+122
//	#define WMU_OPEN_USRMODALLESS WM_USER + 800

// 	pMainWnd->SendMessage(WMU_SETVAR, (WPARAM)"_chart_count", (LPARAM)(LPCTSTR)strCount);
// 	pMainWnd->SendMessage(WMU_SETVAR, (WPARAM)"_chart_date", (LPARAM)(LPCTSTR)strDate);
// 	pMainWnd->SendMessage(WMU_SETVAR, (WPARAM)"_chart_code", (LPARAM)(LPCTSTR)m_strCode);
// 	pMainWnd->SendMessage(WMU_SETVAR, (WPARAM)"_chart_seq", (LPARAM)(LPCTSTR)ni.strDateSeq);
// 	pMainWnd->SendMessage(WMU_OPEN_USRMODALLESS, (WPARAM)(LPCTSTR)"chartnotice", (LPARAM)"");
	pMainWnd->SendMessage(UM_OPEN_MAP, 1, (LPARAM)&om);
}


HRESULT CChartNewsAddInImp::CTrCommSite::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	if(dwTRDateLen==0)
		return 0;
	DR_MYMETHOD_PROLOGUE(CChartNewsAddInImp, TrCommSite)
	CString strTR = szTR;

	if(strTR == "hfit_o061111")
	{
		pThis->ReceiveSiseData(aTRData, dwTRDateLen);
	}
	return S_OK;
}

HRESULT CChartNewsAddInImp::CTrCommSite::StatusChange(int nState)
{
	return  S_OK;
}

HRESULT CChartNewsAddInImp::CTrCommSite::ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData)
{
	return 1;
}

//////////////////////////////////////////////////////////////////////////
// TR 요청
void CChartNewsAddInImp::RequestSiseData()
{
	//TR예제 - TR번호 -01001인경우
	if(!m_TRComm) return;
	m_TRComm->SetTR("hfit_o061111");
	m_TRComm->ChangeCallback(&m_xTrCommSite);
	CString strStartDate;
	CString strCode;
	int nDummy = 0;
	double dDummy = 0;
	long lDate;
	GetValueFromIndex(0/*nStartIndex*/, nDummy, lDate, dDummy);
	strStartDate.Format("%6d", lDate);
	CString strCount;
	strCount.Format("%04d", nEndIndex /*- nStartIndex*/ + 1);
	CString  strBuf = strStartDate + m_strCode + strCount;
	int len = strBuf.GetLength();
	m_TRComm->SetDestination('U');
	m_TRComm->Send2Server(strBuf.GetBuffer(len), len, FALSE);
}	

//////////////////////////////////////////////////////////////////////////
// TR 데이터 받기
void CChartNewsAddInImp::ReceiveSiseData(LPVOID	aTRData, long dwTRDateLen)
{
	//데이터 
	char* pData = (char*)aTRData;

	long lCount;
	int nOffset=0;
	memcpy(&lCount, pData+nOffset, 4);
	ConvertBigEndian(lCount);
	nOffset +=(BYTE)4;
	char szDate[9] ={0};
	long lCntNews = 0, lDate = 0;
	int lDummy = 0;
	CNewsObj* pNews;
	int j=0;//nStartIndex;

	for(int i=0; i<lCount; i++)
	{
		pNews = new CNewsObj;
		memcpy(szDate, pData+nOffset, 8);		nOffset += 8;
		memcpy(&lCntNews, pData+nOffset, 4);		nOffset += 4;
		ConvertBigEndian(lCntNews);
		lDate = atol(szDate);
		pNews->m_lCntNews = lCntNews;
		if(GetIndexFromValue( lDate, lCount, pNews->m_nIndex, j) == R_ERROR)
		{
			delete pNews;
			continue;
		}
		pNews->SetDate(lDate);
		m_NewsObjList.AddTail(pNews);
	}
 	// 화면 업데이트
	m_pIChartOCX->InvalidateControl();
}

//////////////////////////////////////////////////////////////////////////
// 전송 받은 long 데이터의 상위/하위 위치를 뒤집는다.
void CChartNewsAddInImp::ConvertBigEndian(long &d)
{
	long s=d;
	char *ps = (char*)&s;
	char *pd = (char*)&d;

	pd[0] = ps[3];
	pd[1] = ps[2];
	pd[2] = ps[1];
	pd[3] = ps[0];
}
