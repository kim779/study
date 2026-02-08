// ConfirmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB300600.h"
#include "ConfirmDlg.h"
#include "ControlWnd.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfirmDlg dialog


CConfirmDlg::CConfirmDlg(CWnd* pParent, CWnd *pWizard)
	: CDialog(CConfirmDlg::IDD, pParent), CAxisExt(pWizard)
{
	//{{AFX_DATA_INIT(CConfirmDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	TCHAR buff[1024];
	CString path, img_path, home;

	home = Variant(homeCC, "");
	path.Format(_T("%s\\tab\\PALETTE.INI"), (LPCTSTR)home);
	int readL = GetPrivateProfileString(_T("General"), _T("Palette"), _T("Blue"), buff, sizeof(buff), path);
	
	img_path.Format(_T("%s\\image\\axpanel%s1_"), (LPCSTR)home, buff);
	m_hRndBmp[0] = GetAxBitmap((LPCSTR)( img_path + _T("lt.bmp")));
	m_hRndBmp[1] = GetAxBitmap((LPCSTR)( img_path + _T("rt.bmp")));
	m_hRndBmp[2] = GetAxBitmap((LPCSTR)( img_path + _T("lb.bmp")));
	m_hRndBmp[3] = GetAxBitmap((LPCSTR)( img_path + _T("rb.bmp")));

	m_hBtn2[0] = GetAxBitmap((LPCSTR)( home + _T("\\image\\2btn.bmp")));
	m_hBtn2[1] = GetAxBitmap((LPCSTR)( home + _T("\\image\\2btn_dn.bmp")));
	m_hBtn2[2] = GetAxBitmap((LPCSTR)( home + _T("\\image\\2btn_en.bmp")));

	m_odgb = 0;
	m_pBtnYes = m_pBtnNo = NULL;
}


void CConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfirmDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfirmDlg, CDialog)
	//{{AFX_MSG_MAP(CConfirmDlg)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_YES, OnYesClick)
	ON_BN_CLICKED(IDC_BTN_NO, OnNoClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfirmDlg message handlers

BOOL CConfirmDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetFont(m_pFont);

	m_pBtnYes = CreateImgButton("확인", IDC_BTN_YES, m_hBtn2);
	//m_pBtnYes->SetDrawFocus(TRUE);

	m_pBtnNo  = CreateImgButton("취소", IDC_BTN_NO, m_hBtn2);
	//m_pBtnYes->SetDrawFocus(TRUE);

	CalcSize();

	//m_pBtnYes->SetFocus();

	SetTimer(1000,300,NULL);

	return TRUE;
}

void CConfirmDlg::SetBtnFocus()
{
	m_pBtnYes->SetDrawFocus(TRUE);
	m_pBtnNo->SetDrawFocus(TRUE);

	m_pBtnYes->SetFocus();
}

void CConfirmDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pBtnYes) { m_pBtnYes->DestroyWindow(); delete m_pBtnYes; }
	if (m_pBtnNo)  { m_pBtnNo->DestroyWindow(); delete m_pBtnNo; }
}

void CConfirmDlg::DrawRoundRectangle(CDC *pDC, CRect rc)
{
	CSize size(2,2);
	pDC->FillSolidRect((LPRECT)rc, m_crBk);
	rc.DeflateRect(size);
	pDC->FillSolidRect((LPRECT)rc, m_crBodyBk);
	DrawBitmap(pDC, rc.left,    rc.top,      5, 5, m_hRndBmp[0]->operator HBITMAP());
	DrawBitmap(pDC, rc.right-5, rc.top,      5, 5, m_hRndBmp[1]->operator HBITMAP());
	DrawBitmap(pDC, rc.left,    rc.bottom-5, 5, 5, m_hRndBmp[2]->operator HBITMAP());
	DrawBitmap(pDC, rc.right-5, rc.bottom-5, 5, 5, m_hRndBmp[3]->operator HBITMAP());
}

void CConfirmDlg::DrawBitmap(CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol)
{
	HDC hDC = ::CreateCompatibleDC(pDC->m_hDC);
	HBITMAP tBmp = (HBITMAP)::SelectObject(hDC, hBitmap);
	::TransparentBlt(pDC->m_hDC, x, y, w, h, hDC, 0, 0, w, h, tCol);
	::SelectObject(hDC, tBmp);
	::DeleteObject(hDC);
}

void CConfirmDlg::DrawBitmap(CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol)
{
	DrawBitmap(pDC, pRC->left, pRC->top, pRC->Width(), pRC->Height(), hBitmap, tCol);
}

void CConfirmDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	GetClientRect(m_rcClient);
	CalcSize();
}

void CConfirmDlg::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc = m_rcClient;
	
	CFont *pOldFont = dc.SelectObject(m_pFont);

	rc.DeflateRect(CSize(2,2));
	dc.FillSolidRect(m_rcClient, m_crBk);
	DrawRoundRectangle(&dc, rc);
	rc.DeflateRect(CSize(PADDING_VALUE,PADDING_VALUE));

//  for testing----------------
// 	
// 	m_odgb = 1;
// 	m_oprc = 21770;
// 	m_oqty = 1000;
// 	m_acno_qty = 4;
// 	m_cdgb = 10;
// 	

	switch(m_odgb)
	{
	case 1: DrawSellOrder(&dc, &rc); break;
	case 2: DrawBuyOrder(&dc, &rc); break;
	case 3: DrawModifyOrder(&dc, &rc); break;
	case 4: DrawCancelOrder(&dc, &rc); break;
	}

	dc.SelectObject(pOldFont);
}

void CConfirmDlg::DrawLabel(CDC *pDC, CRect *pRC, CString msg, UINT format, COLORREF crText, COLORREF crBk, COLORREF crLine)
{
	CPen pen(PS_SOLID, 1, crLine), *pOldPen;
	CBrush brush(crBk), *pOldBrush;

	pOldPen = pDC->SelectObject(&pen);
	pOldBrush = pDC->SelectObject(&brush);
	pDC->SetBkMode(TRANSPARENT);

	pDC->Rectangle(pRC);
	pDC->SetTextColor(crText);
	pRC->DeflateRect(CSize(PADDING_VALUE));
	pDC->DrawText(msg, pRC, format);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pen.DeleteObject();
}

void CConfirmDlg::DrawSellOrder( CDC *pDC, CRect *pRC)
{
	int line_h = 20, head_w = 50, pos_x = 0, pos_y = 0;

	CString qty_txt;
	COLORREF crHeadBk = 0xE6CDBB, crTextBk = 0xF2D3A6, crLine = 0x9C8F87;

	CRect rc = *pRC;
	rc.DeflateRect(CSize(PADDING_VALUE));

	pDC->FillSolidRect(pRC->left, pRC->top, pRC->Width(), pRC->Height()-30, m_crMadoBk);

	rc.bottom = rc.top + line_h;
	pDC->SelectObject(m_pFontB);
	pDC->SetTextColor(m_crMinus);
	pDC->DrawText(CString("※ 매도주문 확인창"), rc, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
	
	pDC->SelectObject(m_pFont);
	rc.OffsetRect(CPoint(0, line_h)); 
	DrawLine(pDC, &rc, "종  목", m_cdnm, 80, m_crNormal, crHeadBk, crTextBk, crLine);

	rc.OffsetRect(CPoint(0, line_h-1)); 
	DrawLine(pDC, &rc, "구  분", GetCdgbName(), 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	qty_txt.Format("%s 계좌", Int2CommaStr(m_acno_qty));
	rc.OffsetRect(CPoint(0, line_h+PADDING_VALUE)); 
	DrawLine(pDC, &rc, "계  좌", qty_txt, 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	qty_txt.Format("%s 계약", Int2CommaStr(m_oqty));
	rc.OffsetRect(CPoint(0, line_h-1)); 
	DrawLine(pDC, &rc, "수  량", qty_txt, 80, m_crNormal, crHeadBk, crTextBk, crLine);

	rc.OffsetRect(CPoint(0, line_h-1)); 
	if (m_oprc!=MARKET_JPRC)
		DrawLine(pDC, &rc, "단  가", Int2Str(m_oprc, 2), 80, m_crNormal, crHeadBk, crTextBk, crLine);

	rc.OffsetRect(CPoint(0, line_h+PADDING_VALUE)); 
	pDC->SelectObject(m_pFontB);
	pDC->DrawText(CString("주문처리 하시겠습니까?"), rc, DT_VCENTER|DT_SINGLELINE|DT_LEFT);

}

void CConfirmDlg::DrawLine( CDC *pDC, CRect *pRC, CString header, CString text, int iSplit, COLORREF crText, COLORREF crHeadBk, COLORREF crTextBk, COLORREF crLine )
{
	CRect rc;
	rc.SetRect(pRC->left, pRC->top, pRC->left+iSplit, pRC->bottom);
	DrawLabel(pDC, &rc, header, DT_SINGLELINE|DT_CENTER|DT_VCENTER, crText, crHeadBk, crLine);
	rc.SetRect(pRC->left+iSplit-1, pRC->top, pRC->right, pRC->bottom);
	DrawLabel(pDC, &rc, text, DT_SINGLELINE|DT_RIGHT|DT_VCENTER, crText, crTextBk, crLine);
}

void CConfirmDlg::DrawBuyOrder( CDC *pDC, CRect *pRC )
{
	int line_h = 20, head_w = 50, pos_x = 0, pos_y = 0;
	
	CString qty_txt;
	COLORREF crHeadBk = 0xC3BAF4, crTextBk = 0xC9C5F8, crLine = 0x8580F1;
	
	CRect rc = *pRC;
	rc.DeflateRect(CSize(PADDING_VALUE));
	
	pDC->FillSolidRect(pRC->left, pRC->top, pRC->Width(), pRC->Height()-30, m_crMasuBk);
	
	rc.bottom = rc.top + line_h;
	pDC->SelectObject(m_pFontB);
	pDC->SetTextColor(m_crPlus);
	pDC->DrawText(CString("※ 매수주문 확인창"), rc, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
	
	pDC->SelectObject(m_pFont);
	rc.OffsetRect(CPoint(0, line_h)); 
	DrawLine(pDC, &rc, "종  목", m_cdnm, 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	rc.OffsetRect(CPoint(0, line_h-1)); 
	DrawLine(pDC, &rc, "구  분", GetCdgbName(), 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	qty_txt.Format("%s 계좌", Int2CommaStr(m_acno_qty));
	rc.OffsetRect(CPoint(0, line_h+PADDING_VALUE)); 
	DrawLine(pDC, &rc, "계  좌", qty_txt, 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	qty_txt.Format("%s 계약", Int2CommaStr(m_oqty));
	rc.OffsetRect(CPoint(0, line_h-1)); 
	DrawLine(pDC, &rc, "수  량", qty_txt, 80, m_crNormal, crHeadBk, crTextBk, crLine);

	rc.OffsetRect(CPoint(0, line_h-1)); 
	if (m_oprc!=MARKET_JPRC)
		DrawLine(pDC, &rc, "단  가", Int2Str(m_oprc, 2), 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	rc.OffsetRect(CPoint(0, line_h+PADDING_VALUE)); 
	pDC->SelectObject(m_pFontB);
	pDC->DrawText(CString("주문처리 하시겠습니까?"), rc, DT_VCENTER|DT_SINGLELINE|DT_LEFT);

}

void CConfirmDlg::DrawModifyOrder( CDC *pDC, CRect *pRC )
{
	int line_h = 20, head_w = 50, pos_x = 0, pos_y = 0;
	
	CString qty_txt;
	COLORREF crHeadBk = 0xA2DEC3, crTextBk = 0xA5EAC0, crLine = 0x73C392, crBox = 0xD4F5E6;
	
	CRect rc = *pRC;
	rc.DeflateRect(CSize(PADDING_VALUE));
	
	pDC->FillSolidRect(pRC->left, pRC->top, pRC->Width(), pRC->Height()-30, crBox);
	
	rc.bottom = rc.top + line_h;
	pDC->SelectObject(m_pFontB);
	pDC->SetTextColor(m_crNormal);
	pDC->DrawText(CString("※ 정정주문 확인창"), rc, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
	
	pDC->SelectObject(m_pFont);
	rc.OffsetRect(CPoint(0, line_h)); 
	DrawLine(pDC, &rc, "종  목", m_cdnm, 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	if (m_jqty>1) qty_txt.Format("%d 외 %d", m_ojno, m_jqty-1);
	else              qty_txt.Format("%d", m_ojno);
	rc.OffsetRect(CPoint(0, line_h+PADDING_VALUE)); 
	DrawLine(pDC, &rc, "원주문", qty_txt, 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	qty_txt.Format("%s 계좌", Int2CommaStr(m_acno_qty));
	rc.OffsetRect(CPoint(0, line_h-1)); 
	DrawLine(pDC, &rc, "계  좌", qty_txt, 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	qty_txt.Format("%s 계약", Int2CommaStr(m_oqty));
	rc.OffsetRect(CPoint(0, line_h-1)); 
	DrawLine(pDC, &rc, "수  량", qty_txt, 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	qty_txt.Format("%s -> %s", Int2Str(m_old_oprc), Int2Str(m_oprc));
	rc.OffsetRect(CPoint(0, line_h-1)); 
	DrawLine(pDC, &rc, "단  가", qty_txt, 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	rc.OffsetRect(CPoint(0, line_h+PADDING_VALUE)); 
	pDC->SelectObject(m_pFontB);
	pDC->DrawText(CString("정정처리 하시겠습니까?"), rc, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
}

void CConfirmDlg::DrawCancelOrder( CDC *pDC, CRect *pRC )
{
	int line_h = 20, head_w = 50, pos_x = 0, pos_y = 0;
	
	CString qty_txt;
	COLORREF crHeadBk = 0xBCDAE7, crTextBk = 0xEBF9FF, crLine = 0x98BCCD, crBox = 0xD8EEF8;
	
	CRect rc = *pRC;
	rc.DeflateRect(CSize(PADDING_VALUE));
	
	pDC->FillSolidRect(pRC->left, pRC->top, pRC->Width(), pRC->Height()-30, crBox);
	
	rc.bottom = rc.top + line_h;
	pDC->SelectObject(m_pFontB);
	pDC->SetTextColor(m_crNormal);
	pDC->DrawText(CString("※ 취소주문 확인창"), rc, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
	
	pDC->SelectObject(m_pFont);
	rc.OffsetRect(CPoint(0, line_h)); 
	DrawLine(pDC, &rc, "종  목", m_cdnm, 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	if (m_jqty>1) qty_txt.Format("%d 외 %d", m_ojno, m_jqty-1);
	else              qty_txt.Format("%d", m_ojno);
	rc.OffsetRect(CPoint(0, line_h+PADDING_VALUE)); 
	DrawLine(pDC, &rc, "원주문", qty_txt, 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	qty_txt.Format("%s 계좌", Int2CommaStr(m_acno_qty));
	rc.OffsetRect(CPoint(0, line_h-1)); 
	DrawLine(pDC, &rc, "계  좌", qty_txt, 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	qty_txt.Format("%s 계약", Int2CommaStr(m_oqty));
	rc.OffsetRect(CPoint(0, line_h-1)); 
	DrawLine(pDC, &rc, "수  량", qty_txt, 80, m_crNormal, crHeadBk, crTextBk, crLine);
	
	/*
	qty_txt.Format("%s -> %s", Int2Str(m_old_oprc), Int2Str(m_oprc));
	rc.OffsetRect(CPoint(0, line_h-1)); 
	DrawLine(pDC, &rc, "단  가", qty_txt, 80, m_crNormal, crHeadBk, crTextBk, crLine);
	*/
	
	rc.OffsetRect(CPoint(0, line_h+PADDING_VALUE)); 
	pDC->SelectObject(m_pFontB);
	pDC->DrawText(CString("취소처리 하시겠습니까?"), rc, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
}


CString CConfirmDlg::GetCdgbName()
{
	if(m_oprc == MARKET_JPRC)
	{
		return "시장가";
	}
	else
	{
		switch(m_cdgb)
		{
		case 0: return "지정가";
		case 1: return "지정가(IOC)";
		case 2: return "지정가(FOK)";
		case 3: return "시장가";
		default: return "";
		}
	}
}

CfxImgButton* CConfirmDlg::CreateImgButton(LPCSTR caption, int nID, CBitmap **bmp)
{
	CfxImgButton *pBtn = new CfxImgButton(m_pFont);
	if (pBtn)
	{
		if (pBtn->Create(caption, CRect(0, 0, 0, 0), this, nID, FALSE, TRUE))
		{
			pBtn->SetImgBitmap(	bmp[0]->operator HBITMAP(), bmp[1]->operator HBITMAP(), bmp[2]->operator HBITMAP() );
			return pBtn;
		}
		else 
		{
			delete pBtn;
			return NULL;
		}
	}
	else return NULL;
}

void CConfirmDlg::CalcSize()
{
	CRect rc = m_rcClient;
	rc.DeflateRect(CSize(PADDING_VALUE*2));
	rc.top = rc.bottom - 20;
	rc.OffsetRect(CPoint(0, -10));
	
	rc.left = rc.right - 40;
	if (m_pBtnNo) m_pBtnNo->MoveWindow(rc, TRUE);
	
	rc.OffsetRect(CPoint(-40-PADDING_VALUE, 0));
	if (m_pBtnYes) m_pBtnYes->MoveWindow(rc, TRUE);
}

void CConfirmDlg::OnNoClick()
{
	OnCancel();
}

void CConfirmDlg::OnYesClick()
{
// 	OutputDebugString("CONFIRM OK\n");
	OnOK();
}

BOOL CConfirmDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 13)  
	{
		pMsg->wParam = 1;  

// 		if(m_pBtnYes->m_bFocus)
// 			OnOK();
// 		else
// 			OnCancel();
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CConfirmDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CConfirmDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void CConfirmDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1000)
	{
		KillTimer(1000);

		SetBtnFocus();
	}
	
	CDialog::OnTimer(nIDEvent);
}
