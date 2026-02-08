// InfoPopup.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "InfoPopup.h"
#include "axwnd.h"
#include "../../h/axisvar.h"
#include "../../h/memDC.h"

#define IDC_CLOSE	100

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const int headerH = 20, dataH = 20;
const int titleH = 18, tabW = 70, gap1 = 4;

/////////////////////////////////////////////////////////////////////////////
// CInfoPopup

CInfoPopup::CInfoPopup(CWnd *pWnd) : CAxWnd(pWnd)
{
	m_bDrag = false;
	m_nSelected = 0;
	m_pEdit = NULL;
	m_sKey = "";

	m_ujongsise = false;
}

CInfoPopup::~CInfoPopup()
{
//	saveMemo();
	if (m_pEdit)
	{
		if (IsWindow(m_pEdit->GetSafeHwnd()))
			m_pEdit->SendMessage(WM_CLOSE);
		m_pEdit->Detach();
		m_pEdit.reset();
	}
}

BEGIN_MESSAGE_MAP(CInfoPopup, CAxWnd)
	//{{AFX_MSG_MAP(CInfoPopup)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInfoPopup message handlers

BOOL CInfoPopup::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

int CInfoPopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rc;

	m_pFont = GetAxFont(LoadStringTb(IDS_BASEFONTNAME4), 9, 0);

	m_pEdit = std::make_unique<CEditX>();
	m_pEdit->m_brush.CreateSolidBrush(GetIndexColor(97));
	m_pEdit->Create(this, 100); m_pEdit->SetFont(m_pFont);


	GetClientRect(&rc);
	rc.left = rc.right - 20;
	rc.top = rc.top + 6;
	rc.bottom = rc.top + 15;
	rc.right = rc.left + 15;

	m_pCtlBtnClose = std::make_unique<CNButton>(IDC_CLOSE);
	if (m_pCtlBtnClose->Create(NULL, "", WS_VISIBLE|WS_CHILD, rc, this, IDC_CLOSE))
	{
		CString imgF;
		
		imgF.Format("%s\\%s\\%s.bmp", m_sRoot, "image", "종목닫기");
		m_pCtlBtnClose->set_Image(imgF);
		//m_pCtlBtnClose->SetCButtomMsg(TRUE);
	}
	return 0;
}

#include "symbolwnd.h"
void CInfoPopup::OnSetFocus(CWnd* pOldWnd) 
{	
	CWnd::OnSetFocus(pOldWnd);

	MSG	msg{};

	for (m_fDone = FALSE; !m_fDone; WaitMessage())
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if ((msg.message == WM_KILLFOCUS && msg.hwnd == this->m_hWnd) || 
				((msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN) && (msg.hwnd != this->m_hWnd && msg.hwnd != m_pEdit->m_hWnd)) ||
				(msg.message == WM_USER && msg.hwnd == this->m_hWnd))
			{
				if (msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN)
					::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				m_fDone = TRUE;
				break;
			}

			if (!IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (m_fDone)
			break;
	}
	//PostMessage(WM_CLOSE);
	ShowWindow(SW_HIDE);

	if (m_jHeader)
	{
		m_sKey = CString(m_jHeader->codx, 12);
		m_sKey.Trim();
		if (!m_sKey.IsEmpty()) saveMemo(m_sKey);
	}
}

void CInfoPopup::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE)
		PostMessage(WM_USER, 0, 0);
}

BOOL CInfoPopup::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CRect rc;
	CPoint pnt;
	GetCursorPos(&pnt);

	ScreenToClient(&pnt);

	GetClientRect(&rc);

	rc.left = rc.right - 20;
	rc.top = rc.bottom - 20;

	if (rc.PtInRect(pnt))
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
	else
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CInfoPopup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rc;

	GetClientRect(&rc);

	rc.left = rc.right - 14;
	rc.top = rc.bottom - 14;

	if (rc.PtInRect(point))
	{
		m_firstPnt = point;
		SetCapture();
		GetClientRect(&m_rc);
		m_bDrag = true;
	}
	else
	{
		m_firstPnt = CPoint(-1, -1);
		GetClientRect(&rc);
		rc.bottom = rc.top + 20;
		rc.right = rc.left + 80;
		if (rc.PtInRect(point))
		{
			m_nSelected = 0;
			RedrawWindow();
			m_pEdit->ShowWindow(SW_HIDE);
		}
		
		rc.OffsetRect(80, 0);
		if (rc.PtInRect(point))
		{
			m_nSelected = 1;
			RedrawWindow();
			m_pEdit->ShowWindow(SW_SHOW);
			m_pEdit->SetFocus();
		}

		GetClientRect(&rc);
		rc.left = rc.right - 18;
		rc.top = rc.top + 9;
		rc.bottom = rc.top + 10;
		rc.right = rc.left + 10;
		if (rc.PtInRect(point))
			ShowWindow(SW_HIDE);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CInfoPopup::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDrag)
	{
		CSize sz;
		sz.cx = m_rc.Width() - (m_firstPnt.x - point.x);
		sz.cy = m_rc.Height() - (m_firstPnt.y - point.y);

		if (sz.cx < 200)
			sz.cx = 200;
		if (sz.cy < 150)
			sz.cy = 150;

		SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOZORDER|SWP_NOMOVE);
		Invalidate();
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CInfoPopup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDrag)
	{
		ReleaseCapture();
		m_bDrag = false;
	}
	CWnd::OnLButtonUp(nFlags, point);
}


void CInfoPopup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	xxx::CMemDC	memdc(&dc);
	CRect	rc, rect;
	GetClientRect(&rc);
	rect.CopyRect(rc);
	
	CPen	*pOldPen = memdc.SelectObject(GetAxPen(GetIndexColor(67), 1, PS_SOLID));
	CBrush	*pOldBr	= memdc.SelectObject(GetAxBrush(GetIndexColor(97)));
	memdc.Rectangle(rc);	

	drawCaption(&memdc);
	drawData(&memdc);
	
	memdc.SelectObject(GetAxPen(GetSysColor(GetIndexColor(99)/*COLOR_HIGHLIGHTTEXT*/), 1, PS_SOLID));

	// exit button draw ======================================================================================
//	GetClientRect(&rc);
//	rc.left = rc.right - 18;
//	rc.top = rc.top + 9;
//	rc.bottom = rc.top + 10;
//	rc.right = rc.left + 10;
//	memdc.Rectangle(rc);
//	rc.DeflateRect(1, 1);
//	memdc.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
//	rc.InflateRect(1, 1);
//	memdc.MoveTo(rc.left, rc.top);
//	memdc.LineTo(rc.right - 1, rc.bottom - 1);
//	memdc.MoveTo(rc.right - 1, rc.top);
//	memdc.LineTo(rc.left, rc.bottom - 1);
	// ========================================================================================================
	
	rect.top  = rect.bottom - 20;
	rect.left = rect.right - 20;
	drawIndicator1(memdc, rect, GetIndexColor(98));

	memdc.SelectObject(pOldPen);
	memdc.SelectObject(pOldBr);
}

void CInfoPopup::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rc; GetClientRect(&rc);
	rc.DeflateRect(1, headerH+5, 1, 2);
	m_pEdit->MoveWindow(&rc);
	
	GetClientRect(&rc);
	rc.left = rc.right - 20;
	rc.top = rc.top + 6;
	rc.bottom = rc.top + 15;
	rc.right = rc.left + 15;

	m_pCtlBtnClose->MoveWindow(&rc);

}

void CInfoPopup::setJinfo(Chjinfo* hjinfo, Cejinfo* ejinfo, bool ujongsise)
{
	m_jHeader = m_hjinfo = hjinfo;
	m_ejinfo = ejinfo;

	if (m_jHeader == NULL)
		m_jHeader = m_ejinfo;
	if (m_jHeader == NULL)
		return;

	m_ujongsise = ujongsise;
	m_sKey = CString(m_jHeader->codx, 12);
	loadMemo(m_sKey);
}

void CInfoPopup::drawCaption(CDC *pDC)
{
	CRect	clientRc, rect;
	CFont*	pOldFont = nullptr;

	GetClientRect(&clientRc); rect = clientRc;
	rect.top += gap1; rect.bottom = rect.top + headerH;
	rect.DeflateRect(gap1, 0);
	pDC->FillSolidRect(rect, GetIndexColor(98)/*GetSysColor(COLOR_INFOBK)*/);
	pDC->SetBkMode(TRANSPARENT);
	rect.left += 4;
	rect.top  += 2;

	// display IDS_DETAILINFO ===============================================================================
	rect.right = rect.left + tabW;
	if (m_nSelected == 0)
	{
		pDC->SetTextColor(GetIndexColor(99));
		pDC->FillSolidRect(rect, GetIndexColor(97));
		pOldFont = (CFont*)pDC->SelectObject(GetAxFont(LoadStringTb(IDS_CHANGEFONTNAME4), 9, 3));
	}
	else
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pOldFont = (CFont*)pDC->SelectObject(GetAxFont(LoadStringTb(IDS_CHANGEFONTNAME4), 9, 0));
	}

	pDC->DrawText(LoadStringTb(IDS_DETAILINFO), -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);

	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	pOldFont = (CFont*)pDC->SelectObject(GetAxFont(LoadStringTb(IDS_CHANGEFONTNAME4), 9, 0));

	rect.left = rect.right; rect.right = rect.left + gap1;
	pDC->SelectObject(pOldFont);

	// display IDS_MEMOSTR ==================================================================================
	rect.left = rect.right; rect.right = rect.left + tabW;
	if (m_nSelected == 1)
	{
		pDC->SetTextColor(GetIndexColor(99));
		pDC->FillSolidRect(rect, GetIndexColor(97));
		pOldFont = (CFont*)pDC->SelectObject(GetAxFont(LoadStringTb(IDS_CHANGEFONTNAME4), 9, 3));
	}
	else
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pOldFont = (CFont*)pDC->SelectObject(GetAxFont(LoadStringTb(IDS_CHANGEFONTNAME4), 9, 0));
	}

	pDC->DrawText(LoadStringTb(IDS_MEMOSTR), -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
}

void CInfoPopup::drawData(CDC *pDC)
{
	if (m_jHeader == NULL)
		return;
	if (m_nSelected != 0)
		return;
	
	CRect	rect, irc, drc;
	CFont	font;
	int	ival = 0;
	CString	tmpx, temp, string;
	CStringArray	stringAry;
	const	char	sGubn[][20] = {"", "거래소", "코스닥", "제3시장", "ETF", "ELW"};

	GetClientRect(&rect);
	rect.top = rect.top + headerH; rect.DeflateRect(1, 1);
	//	pDC->FillSolidRect(rect, GetSysColor(COLOR_INFOBK));
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	CFont	*pOldFont = (CFont*)pDC->SelectObject(GetAxFont(LoadStringTb(IDS_CHANGEFONTNAME4), 9, 0));
	
	// line 	string = _T("");
	
	ival = atoi(m_jHeader->jgub);
	temp.Format("%s", (ival >= 0 && ival <= 4 ? sGubn[ival] : "" )); // 거래소 구분
	string += temp + " | ";
	
	if (ival == 5) // elw처리
	{
		if (m_ejinfo == NULL)
		{
			pDC->SelectObject(pOldFont);
			return;
		}

		string = "";

		if (!temp.IsEmpty())
		{
			string = temp;
		}

		// Line 1
		temp.Format("기초자산 %s", m_ejinfo->gcjs); temp.Trim();// 기초자산
		string += temp + " | ";
		temp.Format("%s", m_ejinfo->krye); temp.TrimRight();	// 권리유형
		string += temp + " | ";
		temp.Format("%s", m_ejinfo->khbe); temp.TrimRight();	// 권리행사방법
		string += temp;
		stringAry.Add(string);

		// Line 2
		temp.Format("%s", m_ejinfo->gjbe); temp.TrimRight();	// 결제방법
		string = temp + " | ";
		temp.Format("%s", m_ejinfo->hbge); temp.TrimRight();	// 한글발행기관명
		string += temp;
		stringAry.Add(string);

		// Line 3
		temp.Format("%s", m_ejinfo->sjjs); temp.Trim();		// 발행주식수 (천)
		string = "발행주식수 " + temp + " 천주 | ";
		temp.Format("%s", m_ejinfo->sgca); temp.Trim();		// 발행가 (시가총액:억)
		string += "시가총액 " + temp + "억";
		stringAry.Add(string);

		// Line 4
		temp.Format("%s", m_ejinfo->hsga); temp.TrimRight();	// 행사가격
		string = "행사가 " + temp + "  | ";
		temp.Format("%s", m_ejinfo->jhre); temp.TrimRight();	// 전환비율
		string += "전환비율 " + temp + " | ";
		temp.Format("%s", m_ejinfo->jjis); temp.Trim();		// 잔존일수
		string += "잔존일수 " + temp + " 일 ";
		
		stringAry.Add(string);

		// Line 5
		temp.Format("%s", m_ejinfo->mgil); temp.TrimRight();	// 상장일
		string = "만기일 " + temp + "  | ";
		temp.Format("%s", m_ejinfo->edte); temp.TrimRight();	// 최종거래일
		string += "최종거래일 " + temp;
		stringAry.Add(string);

		// Line 6
		temp.Format("%s", m_ejinfo->sjil); temp.TrimRight();	// 상장일
		string = "상장일 " + temp + "  | ";
		temp.Format("%s", m_ejinfo->jgil); temp.TrimRight();	// 최종거래일
		string += "지급일 " + temp;
		stringAry.Add(string);
		
		// Line 7
		temp.Format("%s", m_ejinfo->lpsa); temp.TrimRight();	// LP회원사
		string = "LP회원사 " + temp + "  | ";
		temp.Format("%s", m_ejinfo->lpjg); temp.TrimRight();	// LP주문가능여부
		string += "LP주문가능여부 " + temp;
		stringAry.Add(string);
		
		// Line 8
		temp.Format("%s", m_ejinfo->lpsu); temp.TrimRight();	// LP회원사
		string = "LP보유수량 " + temp + "  | ";
		temp.Format("%s", m_ejinfo->lpbr); temp.TrimRight();	// LP주문가능여부
		string += "LP보유비율 " + temp;
		stringAry.Add(string);
	}
	else
	{
		if (m_hjinfo == NULL)
		{
			pDC->SelectObject(pOldFont);
			return;
		}
		if (!temp.IsEmpty())
		{
			temp.Format("%s", m_hjinfo->size); temp.TrimRight();		// 기업규모
			string += temp + " | ";
			
			temp.Format("%s", m_hjinfo->ujgb); temp.TrimRight();		// 업종구분 
			string += temp + " | ";
			
			temp.Format("%s", m_hjinfo->k2gb); temp.TrimRight();		// 소구분
			
			if (temp.CompareNoCase("K200(미채용)") == 0)
				temp = "KOSPI";
			
			string += temp;
			
			stringAry.Add(string);
		}
		
		// line 2
		string = _T("");
		
		temp.Format("%s", m_hjinfo->usun); temp.TrimRight();		// 보통주,우선주...
		string += temp + " | ";
		
		
		temp.Format("%s", m_hjinfo->krgb); temp.TrimRight();		// 시장조치
		if (temp.CompareNoCase("정상") != 0)
			string += temp + " | ";
		
		ival = atoi(m_hjinfo->ksmm);
		temp.Format("%s %02d%s", "결산월", ival, "월");			// 결산월
		string += temp;
		
		stringAry.Add(string);
		
		
		// line 3
		string = _T("");
		
		m_hjinfo->amga.TrimRight();
		string += "액면가 " + m_hjinfo->amga;
		string += " | ";
		
		m_hjinfo->jbkm.TrimRight();
		string += "자본금 " + m_hjinfo->jbkm;
		
		
		stringAry.Add(string);
		
		// line 4
		string = _T("");
		
		tmpx.Format("%.f", atof(m_hjinfo->siga)); tmpx = SetComma(tmpx);
		temp.Format("%s %s%s", "전일시가총액", tmpx, "억");
		string += temp;
		
		
		stringAry.Add(string);
		
		// line 5
		string = _T("");
		
		tmpx.Format("%.f", atof(m_hjinfo->sjjs)); tmpx = SetComma(tmpx);
		temp.Format("%s %s%s", "상장주식수", tmpx, "천주");
		string += temp;
		
		stringAry.Add(string);
		
		// line 6
		string = _T("");
		
		m_hjinfo->jrat.TrimRight();
		temp.Format("증거금율 %s%%", m_hjinfo->jrat);
		string += temp + " | ";
		
		int rat;
		rat = atoi(m_hjinfo->nrat);
		
		if (rat == 0)
			rat = atoi(m_hjinfo->jrat);
		
		m_hjinfo->nrat.TrimRight();
		
		if (rat == 100)		temp = "신용불가";
		else			temp.Format("신용비율 %d%%", rat);
		string += temp;
	
		stringAry.Add(string);
			
		if (m_ujongsise)
		{
			// line 7
			string = _T("");
			
			tmpx.Format("%.2f", atof(m_hjinfo->jisu)); tmpx = SetComma(tmpx);
			temp.Format("%s %s", "업종지수", tmpx);		// 지수
			string += temp + " ";
			
			temp.Format("%.2f", atof(m_hjinfo->diff));	// 전일대비
			string += temp + " ";
			
			temp.Format("%.2f%%", atof(m_hjinfo->rate));	// 등략율
			string += temp;
			
			stringAry.Add(string);
			
			// line 8
			string = _T("");
			
			tmpx.Format("%.0f", atof(m_hjinfo->gvol)); tmpx = SetComma(tmpx);
			temp.Format("%s %s%s", "거래량", tmpx, "천주");
			string += temp;
			
			stringAry.Add(string);
			
		}
	}
	

	for (int ii = 0; ii < stringAry.GetSize(); ii++)
	{
		string = stringAry.GetAt(ii);
		if (string.IsEmpty())
			continue;

		drc = rect;
		drc.bottom = drc.top + dataH;

		irc = drc; irc.right = irc.left + dataH;
		irc.DeflateRect(1, 1);
		drawIndicator(pDC, irc);

		drc.left += dataH; drc.top += 2;
		pDC->DrawText(string, drc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		rect.OffsetRect(CPoint(0, dataH));
	}
	pDC->SelectObject(pOldFont);
}

void CInfoPopup::drawIndicator(CDC* dc, CRect rect)
{
	CPoint	pts[3];
	int	gap = 0;

	CPen* oldPen   = dc->SelectObject(GetAxPen(GetSysColor(COLOR_INFOBK), 1, PS_SOLID));
	CBrush* oldBrush = dc->SelectObject(GetAxBrush(GetSysColor(COLOR_GRAYTEXT)));

	dc->SetPolyFillMode(ALTERNATE);
	gap = rect.Height()*2/5;

	rect.DeflateRect(gap, 4, gap+1, 4);
	rect.OffsetRect(0, 1);
	rect.bottom = rect.top + (rect.Height()/2)*2-1;
	rect.right = rect.left + rect.Height()/2;

	pts[0].y = rect.top;
	pts[1].y = rect.top + (rect.Height()/2);
	pts[2].y = rect.bottom-1;
	pts[0].x = rect.left;
	pts[1].x = rect.right;
	pts[2].x = rect.left;
	dc->Polygon(pts, 3);

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);
}

void CInfoPopup::saveMemo(CString sKey)
{
	if (sKey.IsEmpty())
		return;

	if (m_pEdit)
	{
		CString str, rDir, wDir;

		rDir.Format("%s\\%s\\%s\\memo.mmo", m_sRoot, USER, m_sUserID);
		wDir.Format("%s\\%s\\%s\\memo.mm2", m_sRoot, USER, m_sUserID);

		m_pEdit->GetWindowText(str);
		
		char	key[12]{}, lBytes[4]{};
		CString dat;
		CFile wFile(wDir, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone);

		if (!str.IsEmpty())
		{
			FillMemory(key, sizeof(key), ' ');
			FillMemory(lBytes, sizeof(lBytes), ' ');

			strcpy(key, sKey.operator LPCTSTR());
			itoa(str.GetLength(), lBytes, 10);
			wFile.Write(key, 12);
			wFile.Write(lBytes, 4);
			wFile.Write(str, str.GetLength());
		}

		CFileFind	finder;
		if (!finder.FindFile(rDir))
		{
			wFile.Close();
			CopyFile(wDir, rDir, false);
			DeleteFile(wDir);
			return;
		}
		
		TRY
		{
			CFile rFile(rDir, CFile::modeRead | CFile::shareDenyNone);
			
			UINT nBytesRead;
			do
			{
				FillMemory(key, sizeof(key), ' ');
				FillMemory(lBytes, sizeof(lBytes), ' ');

				nBytesRead = rFile.Read(&key, sizeof(key));
				if (nBytesRead == sizeof(key))
				{
					nBytesRead = rFile.Read(&lBytes, sizeof(lBytes));
					if (nBytesRead == sizeof(lBytes))
					{
						int lSize = atoi(CString(lBytes, 4));
						nBytesRead = rFile.Read(dat.GetBuffer(lSize), lSize);
						
						if ((int)nBytesRead != lSize)
							break;
						
						if (!sKey.Compare(key))
						{
							dat.ReleaseBuffer();
							continue;
						}
						wFile.Write(key, 12);
						wFile.Write(lBytes, 4);
						wFile.Write(dat, nBytesRead);
						dat.ReleaseBuffer();
					}
					else
						break;
				}
				else
					break;
				
			} while ((int)nBytesRead);
			rFile.Close();
		} CATCH(CFileException, e )
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		}
		END_CATCH

		wFile.Close();
		CopyFile(wDir, rDir, false);
		DeleteFile(wDir);
	}
}

void CInfoPopup::OnDestroy() 
{
	CWnd::OnDestroy();
}

void CInfoPopup::loadMemo(CString sKey)
{
	if (sKey.IsEmpty())
		return;

	sKey.TrimRight();

	CString dir, dat;
	char	key[12]{}, lBytes[4]{};

	dir.Format("%s\\%s\\%s\\memo.mmo", m_sRoot, USER, m_sUserID);
	CFileFind	finder;
	if (!finder.FindFile(dir))
		return;

	TRY
	{ 
		CFile file(dir, CFile::modeRead | CFile::shareDenyNone);
		UINT nBytesRead;
		do
		{
			nBytesRead = file.Read(&key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = file.Read(&lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = file.Read(dat.GetBufferSetLength(lSize), lSize);

					if ((int)nBytesRead != lSize)
					{
						dat.ReleaseBuffer();
						break;
					}

					if (!sKey.Compare(key))
					{
						m_pEdit->SetWindowText(dat);
						m_pEdit->SetSel(0, -1);
						dat.ReleaseBuffer();
						break;
					}

					dat.ReleaseBuffer();
				}
				else
					break;
			}
			else
				break;
				
		} while ((int)nBytesRead);
		file.Close();
	}
	CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH
}

void CInfoPopup::setPathInfo(CString sRoot, CString sID)
{
	m_sUserID = sID;
	m_sRoot = sRoot;
	m_sKey.Trim(); 
}

CString CInfoPopup::SetComma(CString src)
{
	CString temp, floatStr;

	if (src.Find('.') >= 0)
	{
		floatStr = src.Mid(src.Find('.'));
		src = src.Left(src.Find('.'));
	}
	else
		floatStr.Empty();
	
	temp = src;
	src.Empty();
	
	for (int ii = temp.GetLength() - 1, jj = -1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (temp.GetAt(0) != '-' && temp.GetAt(0) != '+')
				src = ',' + src;
			else if (ii != 0)
				src = ',' + src;
			
			jj = 0;
		}
		src = temp.GetAt(ii) + src;
	}
	src += floatStr;
	
	return src;
}

CString CInfoPopup::getDiffSym(char chDiff)
{
	switch (chDiff)
	{
	case '1':	// 상한
		return "↑";
	case '+':
	case '2':	// 상승
		return "▲";
	case '3':	// 보합
		return "";
	case '4':	// 하락
		return "↓";
	case '-':
	case '5':	// 상한
		return "▼";
	}
	return "";
}

CString CInfoPopup::LoadStringTb(UINT nIdx)
{
	CString str;
	str.LoadString(nIdx);
	return str;
}

void CInfoPopup::drawIndicator1(CDC* dc, CRect rect, COLORREF clr)
{
	CPoint	pts[3];
	CPen*   oldPen   = dc->SelectObject(GetAxPen(clr, 1, PS_SOLID));
	CBrush* oldBrush = dc->SelectObject(GetAxBrush(clr));

	dc->SetPolyFillMode(ALTERNATE);

	rect.DeflateRect(1, 1, 2, 2);
	const	int	gap    = rect.Height() / 6;
	const	int	height = (rect.Height() - gap) / 2;

	rect.top  = rect.bottom - height;
	rect.left = rect.right - height;

	pts[0].x = rect.left;
	pts[0].y = rect.bottom;
	pts[1].x = rect.right;
	pts[1].y = rect.top;
	pts[2].x = rect.right;
	pts[2].y = rect.bottom;
	dc->Polygon(pts, 3);

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);
}
