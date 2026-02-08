// GrpGrid.cpp : implementation file
//

#include "stdafx.h"
#include "axisGMain.h"
#include "GrpGrid.h"
#include "ObjMgr.h"
#include "CrossLine.h"
#include "memDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TOTAL_GRID_CLASS	"Total Grid"
#define	DISPARITY	10

#define	F_HEIGHT	14
#define	F_WIDTH		7
#define	TOTAL_WIDTH	(F_WIDTH * 31 + 3)	//(F_WIDTH * 34 + 3)



/////////////////////////////////////////////////////////////////////////////
// CGrpGrid

CGrpGrid::CGrpGrid()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, TOTAL_GRID_CLASS, &wndcls)))
	{
		wndcls.style		= CS_SAVEBITS;
		wndcls.lpfnWndProc	= ::DefWindowProc;
		wndcls.cbClsExtra	= wndcls.cbWndExtra = 0;
		wndcls.hInstance	= hInst;
		wndcls.hIcon		= NULL;
		wndcls.hCursor		= LoadCursor(hInst, IDC_ARROW);
		wndcls.hbrBackground	= (HBRUSH)(COLOR_INFOBK + 1); 
		wndcls.lpszMenuName	= NULL;
		wndcls.lpszClassName	= TOTAL_GRID_CLASS;

		if (!AfxRegisterClass(&wndcls))	AfxThrowResourceException();
	}

	m_pwndPnChart = NULL;
	m_pObjMgr = NULL;
	
	m_strData = _T("");

	m_textRGB = PALETTERGB( 0, 0, 0);
	m_backRGB = PALETTERGB( 255, 255, 255);
	m_frameRGB = PALETTERGB( 112, 112, 112);
	m_lineRGB = PALETTERGB( 192, 192, 192);
	m_scrRGB = PALETTERGB( 112, 112, 112);
	m_timeRGB = PALETTERGB( 245, 215, 142); 
	m_specRGB = PALETTERGB( 222, 228, 237);

//	F_HEIGHT = 14;
//	F_WIDTH = 7;
	m_iLineCnt = 0;
	m_rectDraw.SetRectEmpty();

	m_ppenLine = NULL;
	m_pbrushBG = NULL;
	m_pbrushFG = NULL;
	m_pbrushScr = NULL;
	m_pbrushTime = NULL;
	m_pbrushSpec = NULL;

//	m_iMaxHeadSize = 0;
//	m_iMaxDataSize = 0;
	m_ePos = eLeft;
	m_bFirst = true;
}

CGrpGrid::~CGrpGrid()
{
}


BEGIN_MESSAGE_MAP(CGrpGrid, CWnd)
	//{{AFX_MSG_MAP(CGrpGrid)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGrpGrid message handlers

void CGrpGrid::OnPaint() 
{
	if (m_pwndPnChart == (CWnd*)NULL)
		return;

	CPaintDC dc(this);
	CMemDC	mdc(&dc);
	DrawGrid(&mdc);
}

// Modified by schbang 2006.05.04
void CGrpGrid::DrawGrid(CDC *pDC)
{
	
	CFont* sFont = pDC->SelectObject(m_pfont);
	CPen* sPen = pDC->SelectObject(m_ppenLine);
	COLORREF sColor = pDC->SetTextColor(m_textRGB);
	int sMode = pDC->SetBkMode(OPAQUE);

	pDC->FillRect(&m_rectDraw, m_pbrushBG);

	CRect rectC;
	GetClientRect(&rectC);
	CRect rectC2 = rectC;
	rectC2.DeflateRect(0, 1);

	CPoint pt;
	char cKind;
	CString strHead;
	CString strData;
//	CString strTemp;
	CRect rectRow;
	int iCount = m_arData.GetSize();
	for (int ii = 0; ii < iCount; ii++)
	{
		rectRow = rectC2;
		rectRow.top = rectC2.top + (F_HEIGHT + 1) * ii;
		rectRow.bottom = rectRow.top + F_HEIGHT;

		if (ii != iCount - 1)	// not last
		{
			pt.x = rectRow.left;
			pt.y = rectRow.bottom;
			pDC->MoveTo(pt);

			pt.x = rectRow.right;
			pDC->LineTo(pt);
		}

		pDC->SetTextColor(m_textRGB);
		pDC->SelectObject(m_pfont);

		rectRow.InflateRect(-3, 0);


		cKind = m_arKind.GetAt(ii);
		strHead = m_arHead.GetAt(ii);
		strData = m_arData.GetAt(ii);

		//if (strTemp.Find("@@") != -1)	// Y Pos Value
		if (cKind == '@')	// Y Pos Value
		{
			COLORREF sbgColor = pDC->SetBkColor(m_scrRGB);
			pDC->SetTextColor(RGB(255,255,255));
			CRect	DrawRc = CRect(m_rectDraw.left, rectRow.top, m_rectDraw.right, rectRow.bottom);
			pDC->FillRect(&DrawRc, m_pbrushScr);
			pDC->DrawText(strHead, strHead.GetLength(), &rectRow, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		
//			strTemp = _T("                    ");
//			pDC->DrawText(strTemp, strTemp.GetLength(), &rectRow, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
			pDC->DrawText(strData, strData.GetLength(), &rectRow, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

			pDC->SetBkColor(sbgColor);
			continue;
		}
		//else if (strTemp.Find("~~") != -1)	// Header
		else if (cKind == '~')	// Header
		{
			COLORREF sbgColor = pDC->SetBkColor(m_timeRGB);
			CRect	DrawRc = CRect(m_rectDraw.left, rectRow.top, m_rectDraw.right, rectRow.bottom);
			pDC->FillRect(&DrawRc, m_pbrushTime);
			pDC->DrawText(strHead, strHead.GetLength(), &rectRow, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		
//			strTemp = _T("                    ");
//			pDC->DrawText(strTemp, strTemp.GetLength(), &rectRow, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
			pDC->DrawText(strData, strData.GetLength(), &rectRow, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

			pDC->SetBkColor(sbgColor);
			continue;
		}
		//else if (strTemp.Find("$$") != -1)	// Volume, Trade Money
		else if (cKind == '$')	// Volume, Trade Money
		{
			COLORREF sbgColor = pDC->SetBkColor(m_specRGB);
			CRect	DrawRc = CRect(m_rectDraw.left, rectRow.top, m_rectDraw.right, rectRow.bottom);
			pDC->FillRect(&DrawRc, m_pbrushSpec);
			pDC->DrawText(strHead, strHead.GetLength(), &rectRow, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		
//			strTemp = _T("                    ");
//			pDC->DrawText(strTemp, strTemp.GetLength(), &rectRow, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
			pDC->DrawText(strData, strData.GetLength(), &rectRow, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

			pDC->SetBkColor(sbgColor);
			continue;
		}
		//else if (strTemp.Find("&&") != -1)	// Bold
		else if (cKind == '&')	// Bold
		{
			pDC->SelectObject(m_pfontB);
			pDC->SetTextColor(RGB(255,0,255));
		}
		//else if (strTemp.Find("##") != -1)	// Sign
		else if (cKind == '#')	// Sign
		{
			pDC->DrawText(strHead, strHead.GetLength(), &rectRow, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
//			strTemp = _T("                    ");
//			pDC->DrawText(strTemp, strTemp.GetLength(), &rectRow, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

			if (strData.GetAt(0) == '+')
				pDC->SetTextColor(RGB(255,0,0));
			else if (strData.GetAt(0) == '-')
				pDC->SetTextColor(RGB(0,0,255));

			pDC->DrawText(strData, strData.GetLength(), &rectRow, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
			continue;
		}

		pDC->DrawText(strHead, strHead.GetLength(), &rectRow, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
//		strTemp = _T("                    ");
//		pDC->DrawText(strTemp, strTemp.GetLength(), &rectRow, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

		pDC->DrawText(strData, strData.GetLength(), &rectRow, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
	}

	pDC->FrameRect(&rectC, m_pbrushFG);

	pt.x = F_WIDTH * 12 + 1;
	//pt.x = m_iMaxHeadSize + 5;
	pt.y = rectC2.top;
	pDC->MoveTo(pt);

	pt.y = rectC2.bottom;
	pDC->LineTo(pt);

	pDC->SelectObject(sFont);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
	pDC->SelectObject(sPen);
}

void CGrpGrid::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect	rc;
	CRect	GrpRc;
	CPoint	pt;

	GetClientRect(&rc);
	ClientToScreen(&rc);
		
	GrpRc = m_pObjMgr->m_rectObj;
	m_pwndPnChart->ClientToScreen(&GrpRc);

	pt.y = GrpRc.top;
	if (rc.left == GrpRc.left)
		pt.x = GrpRc.right - rc.Width();
	else
		pt.x = GrpRc.left;

	// 2009.10.30 : DELETE : 수치 조회창이 ReDraw가 안되는 현상 수정
	//SetWindowPos(&wndTop, pt.x, pt.y, m_rectDraw.Width(), m_rectDraw.Height(), SWP_NOACTIVATE);
	ShowWindow(SW_SHOWNOACTIVATE);
	
	Invalidate(FALSE);
	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CGrpGrid::Create(CWnd* pwndView, CWnd* pwndPnChart, class CObjMgr* pObjMgr)
{
	m_pApp = (CAxisGMainApp *)AfxGetApp();
	m_pwndView = pwndView;
	m_pwndPnChart = pwndPnChart;
	m_pObjMgr = pObjMgr;

	//m_pfont = m_pApp->GetFont(m_pwndView, 9, "굴림");
	m_pfont = m_pApp->GetFont(m_pwndView, 9, "굴림체");
	m_pfontB = m_pApp->GetFont(m_pwndView, 9, "굴림체", FW_BOLD);

	DWORD dwStyle = WS_POPUP; 
	DWORD dwExStyle = WS_EX_TOOLWINDOW;

	CWnd* pWnd = pwndPnChart->GetParent()->GetParent()->GetParent()->GetParent();
	if (!pWnd)
		pWnd = pwndPnChart->GetParent()->GetParent()->GetParent();

	return CreateEx(dwExStyle, TOTAL_GRID_CLASS, NULL, dwStyle, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		pWnd->m_hWnd, NULL, NULL );
		//pwndPnChart->m_hWnd, NULL, NULL );

	
}

void CGrpGrid::ShowData(CString strData, CPoint* pPoint)
{
	if (m_pwndPnChart == NULL)
		return;

	int iLineCnt = ParseData(strData);

	CRect rectGrid;
	GetWindowRect(&rectGrid);


	CRect rectGrp = m_pObjMgr->m_rectObj;
	m_pwndPnChart->ClientToScreen(&rectGrp);

	if (iLineCnt == 0)
	{
		ShowWindow(SW_HIDE);
	}
	else 
	{
		CPoint pt = *pPoint;
		m_pwndPnChart->ClientToScreen(&pt);


		//if (m_bFirst)
		// 처음이거나 메인차트 영역을 벗어난 경우
		if (m_bFirst || rectGrid.left < rectGrp.left || rectGrid.right > rectGrp.right || rectGrid.top < rectGrp.top || rectGrid.bottom > rectGrp.bottom)
		{
			m_bFirst = false;
			CRect rectFirst = m_pObjMgr->m_rectObj;
			m_pwndPnChart->ClientToScreen(rectFirst);
			//SetWindowPos(&wndTop, rectFirst.left, rectFirst.top, NULL, NULL, SWP_NOACTIVATE | SWP_NOSIZE);
			SetWindowPos(&wndTop, rectFirst.left, rectFirst.top, m_rectDraw.Width(), m_rectDraw.Height(), SWP_NOACTIVATE);
		}
		//else if (m_ePos == eLeft && pt.x >= rectGrid.left && pt.x < rectGrid.right)	// 차트 오른쪽 위로 이동
		// 2009.10.30 : CHANGE : 수치 조회창이 ReDraw가 안되는 현상 수정
		//else if (rectGrid.left == rectGrp.left && pt.x >= rectGrid.left && pt.x < rectGrid.right)	// 차트 오른쪽 위로 이동
		else if (rectGrid.left == rectGrp.left && pt.x >= rectGrid.left && pt.x < rectGrid.right &&
			     (rectGrp.right - pt.x) > rectGrid.Width())	// 차트 오른쪽 위로 이동
		{
			pt.y = rectGrp.top;
			pt.x = rectGrp.right - rectGrid.Width();
			SetWindowPos(&wndTop, pt.x, pt.y, m_rectDraw.Width(), m_rectDraw.Height(), SWP_NOACTIVATE);
			m_ePos = eRight;
		}
		//else if (m_ePos == eRight && pt.x >= rectGrid.left && pt.x < rectGrid.right)	// 차트 왼쪽 위로 이동
		// 2009.10.30 : CHANGE : 수치 조회창이 ReDraw가 안되는 현상 수정
		//else if (rectGrid.right == rectGrp.right && pt.x >= rectGrid.left && pt.x < rectGrid.right)	// 차트 왼쪽 위로 이동
		else if (rectGrid.right == rectGrp.right && pt.x >= rectGrid.left && pt.x < rectGrid.right &&
			     (pt.x - rectGrp.left) > rectGrid.Width() )	// 차트 왼쪽 위로 이동
		{
			pt.y = rectGrp.top;
			pt.x = rectGrp.left;
			SetWindowPos(&wndTop, pt.x, pt.y, m_rectDraw.Width(), m_rectDraw.Height(), SWP_NOACTIVATE);
			m_ePos = eLeft;
		}
		else if (iLineCnt != m_iLineCnt)
		{
			pt.y = rectGrp.top;
			pt.x = rectGrid.left;

			SetWindowPos(&wndTop, pt.x, pt.y, m_rectDraw.Width(), m_rectDraw.Height(), SWP_NOACTIVATE);
		}
		else
		{
			ShowWindow(SW_SHOWNOACTIVATE);
			Invalidate(FALSE);
		}
	}

	m_iLineCnt = iLineCnt;
}

void CGrpGrid::MoveGrid(CPoint pt, CString data)
{
	if (m_pwndPnChart == (CWnd*)NULL)
		return;

	if (!data.IsEmpty())
		m_iLineCnt = ParseData(data);
	else
	{
		m_rectDraw.right = m_rectDraw.left + 1;
		//m_rectDraw.right = TOTAL_WIDTH;
		m_rectDraw.bottom = m_rectDraw.top + 1;
	}

	m_pwndPnChart->ClientToScreen(&pt);
	SetWindowPos(&wndTop, pt.x, pt.y, m_rectDraw.Width(), m_rectDraw.Height(), SWP_NOACTIVATE);

}

/*
void CGrpGrid::SetData(CString strData)
{
	m_iLineCnt = ParseData(strData);
}
*/
/*
int CGrpGrid::ParseData(CString data)
{
	int	DataCnt = 0;

	if (data.IsEmpty())
	{
		m_arData.RemoveAll();
		m_rectDraw.right = F_WIDTH* 24 + 3;
		//m_rectDraw.right = m_rectDraw.left +1;
		m_rectDraw.bottom = m_rectDraw.top +1;
		m_strData = _T("");
		return 0;
	}
	m_arData.RemoveAll();
	int	Index = 0;
	CString	tStr = data;
	CString	dummy = _T("");

	while(TRUE)
	{
		Index = tStr.Find("\t");

		if (Index == -1)
		{
			if (!tStr.IsEmpty())
			{
				m_arData.Add(tStr);
				DataCnt++;
			}
			break;
		}

		CString	str = tStr.Mid(0, Index);

		if (str.Find(":") < 0)
		{
			if (str.Find("/") < 0)
			{
				dummy = tStr.Mid(0, Index+1);
				tStr = tStr.Mid(++Index);
				continue;
			}
		}

		m_arData.Add(dummy + str);

		Index++;
		tStr = tStr.Mid(Index);
		DataCnt++;
		dummy = _T(""); 
	}

	if (DataCnt)
	{
		m_rectDraw.right =  F_WIDTH* 24 + 3;
		m_rectDraw.bottom = (F_HEIGHT + 1) * DataCnt + 1;
	}
	else
	{
		m_rectDraw.right = F_WIDTH* 24 + 3;
		m_rectDraw.bottom = m_rectDraw.top+1;
	}

	m_strData = data;
	return DataCnt;
}
*/

/*
int CGrpGrid::ParseData(CString strData)
{
	if (strData.IsEmpty())
	{
		m_arData.RemoveAll();
		m_rectDraw.right = TOTAL_WIDTH;
		m_rectDraw.bottom = m_rectDraw.top + 1;
		m_strData = _T("");
		return 0;
	}

	int iDataCnt = 0;
	m_arData.RemoveAll();
	m_strData = strData;
	CString	strDummy = _T("");

	while(TRUE)
	{
		int iPos = strData.Find("\t");
		if (iPos == -1)
		{
			if (!strData.IsEmpty())
			{
				m_arData.Add(strData);
				iDataCnt++;
			}
			break;
		}

		CString	strOne = strData.Left(iPos);
		if (strOne.Find(":") < 0)
		{
			if (strOne.Find("/") < 0)
			{
				strDummy = strData.Mid(0, iPos+1);
				strData = strData.Mid(++iPos);
				continue;
			}
		}

		m_arData.Add(strDummy + strOne);

		iPos++;
		strData = strData.Mid(iPos);
		iDataCnt++;
		strDummy = _T(""); 
	}

	if (iDataCnt)
	{
		m_rectDraw.right =  TOTAL_WIDTH;
		m_rectDraw.bottom = (F_HEIGHT + 1) * iDataCnt + 1;
	}
	else
	{
		m_rectDraw.right = TOTAL_WIDTH;
		m_rectDraw.bottom = m_rectDraw.top+1;
	}

	return iDataCnt;
}
*/
int CGrpGrid::ParseData(CString strData)
{
	if (strData.IsEmpty())
	{
		m_arData.RemoveAll();
		m_rectDraw.right = m_rectDraw.left + 1;;
		m_rectDraw.bottom = m_rectDraw.top + 1;
		m_strData = _T("");
		return 0;
	}

	int iDataCnt = 0;
	m_arKind.RemoveAll();
	m_arHead.RemoveAll();
	m_arData.RemoveAll();
	m_strData = strData;
//	CString	strDummy = _T("");

	CString	strOne;
	CString strHead;
//	int iMaxHeadSize = 0;
//	int iMaxDataSize = 0;
	CSize size;
	while(!strData.IsEmpty())
	{
		int iPos = strData.Find("\t");
		if (iPos == -1)
		{
			strOne = strData;
			strData.Empty();
		}
		else
		{
			strOne = strData.Left(iPos++);
			strData = strData.Mid(iPos);
		}

//		if (strOne.Find(":") < 0)
//		{
//			if (strOne.Find("/") < 0)
//			{
//				strDummy = strData.Mid(0, iPos+1);
//				strData = strData.Mid(++iPos);
//				continue;
//			}
//		}

		iPos = strOne.Find(":");
		strHead = strOne.Left(iPos++);
		strOne = strOne.Mid(iPos);

		char cKind = 0;
		if ((iPos = strHead.Find("@@")) > -1)
		{
			strHead = strHead.Mid(2);
			cKind = '@';
		}
		else if ((iPos = strHead.Find("~~")) > -1)
		{
			strHead = strHead.Mid(2);
			cKind = '~';
		}
		else if ((iPos = strHead.Find("$$")) > -1)
		{
			strHead = strHead.Mid(2);
			cKind = '$';
		}
		else if ((iPos = strHead.Find("&&")) > -1)
		{
			strHead = strHead.Mid(2);
			cKind = '&';
		}
		else if ((iPos = strHead.Find("##")) > -1)
		{
			strHead = strHead.Mid(2);
			cKind = '#';
		}

//		size = CalStrSize(strHead);
//		if (size.cx > iMaxHeadSize)
//			iMaxHeadSize = size.cx;

//		size = CalStrSize(strOne);
//		if (size.cx > iMaxDataSize)
//			iMaxDataSize = size.cx;


		m_arKind.Add(cKind);
		m_arHead.Add(strHead);
		m_arData.Add(strOne);
		iDataCnt++;
//		strDummy.Empty(); 
	}

//	m_iMaxHeadSize = iMaxHeadSize;
//	m_iMaxDataSize = iMaxDataSize;

	if (iDataCnt)
	{
		//m_rectDraw.right =  m_iMaxHeadSize + m_iMaxDataSize + 15;	//TOTAL_WIDTH;
		m_rectDraw.right =  TOTAL_WIDTH;
		m_rectDraw.bottom = (F_HEIGHT + 1) * iDataCnt + 1;
/*
		if (m_ePos == eLeft)
			SetWindowPos(&wndTop, 0, 0, m_rectDraw.right, m_rectDraw.bottom, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREDRAW);
		else
		{
			CRect rectGrp = m_pObjMgr->m_rectObj;
			m_pwndPnChart->ClientToScreen(rectGrp);
			SetWindowPos(&wndTop, rectGrp.right - m_rectDraw.Width(), rectGrp.top, m_rectDraw.right, m_rectDraw.bottom, SWP_NOACTIVATE | SWP_NOREDRAW);
		}
*/
	}
	else
	{
		m_rectDraw.right = 0;
		m_rectDraw.bottom = 0;
	}

	return iDataCnt;
}
/*
CSize CGrpGrid::CalStrSize(CString strExam)
{
	CDC* pDC = GetDC();
	CFont	*sFont = pDC->SelectObject(m_pfont);
	CSize size = pDC->GetTextExtent(strExam);
	pDC->SelectObject(sFont);
	ReleaseDC(pDC);		
	return size;
}
*/
int CGrpGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	Init();	
	return 0;
}

void CGrpGrid::Init() 
{
	m_ppenLine = m_pApp->GetPen(m_pwndView, PS_SOLID, 0, m_lineRGB);
	m_pbrushBG = m_pApp->GetBrush(m_pwndView, m_backRGB);
	m_pbrushFG = m_pApp->GetBrush(m_pwndView, m_frameRGB);
	m_pbrushScr = m_pApp->GetBrush(m_pwndView, m_scrRGB);
	m_pbrushTime = m_pApp->GetBrush(m_pwndView, m_timeRGB);
	m_pbrushSpec = m_pApp->GetBrush(m_pwndView, m_specRGB);
}
