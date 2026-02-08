// TOP10Dialog.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "TOP10Dialog.h"
#include "../dll/sm/EnBitmap.h"
#include "../dll/sm/resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTOP10Dialog dialog


CTOP10Dialog::CTOP10Dialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTOP10Dialog::IDD, NULL)
{   
	m_dateStatic = _T("");
	m_pParent = pParent;
	m_bShown = false;
	m_bTracking = false;
	m_bAlways = false;
	m_bMouseIn = FALSE;
	m_rateColor.RemoveAll();
	m_arCodes.RemoveAll();
	m_arData.RemoveAll();
	m_mapData.RemoveAll();
	m_arNew.RemoveAll();
	m_nPrevious = 0;
	m_nCurrent = 0;
}


void CTOP10Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTOP10Dialog)
	DDX_Control(pDX, IDC_TOP10BG, m_static);
	DDX_Text(pDX, IDC_STATIC_DATE, m_dateStatic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTOP10Dialog, CDialog)
	//{{AFX_MSG_MAP(CTOP10Dialog)
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_L1, OnButtonL1)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_L2, OnButtonL2)
	ON_BN_CLICKED(IDC_BUTTON_L3, OnButtonL3)
	ON_BN_CLICKED(IDC_BUTTON_L4, OnButtonL4)
	ON_BN_CLICKED(IDC_BUTTON_L5, OnButtonL5)
	ON_BN_CLICKED(IDC_BUTTON_L6, OnButtonL6)
	ON_BN_CLICKED(IDC_BUTTON_L7, OnButtonL7)
	ON_BN_CLICKED(IDC_BUTTON_L8, OnButtonL8)
	ON_BN_CLICKED(IDC_BUTTON_L9, OnButtonL9)
	ON_BN_CLICKED(IDC_BUTTON_L10, OnButtonL10)
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_L11, OnButtonL11)
	ON_BN_CLICKED(IDC_BUTTON_L12, OnButtonL12)
	ON_BN_CLICKED(IDC_BUTTON_L13, OnButtonL13)
	ON_BN_CLICKED(IDC_BUTTON_L14, OnButtonL14)
	ON_BN_CLICKED(IDC_BUTTON_L15, OnButtonL15)
	ON_BN_CLICKED(IDC_BUTTON_L16, OnButtonL16)
	ON_BN_CLICKED(IDC_BUTTON_L17, OnButtonL17)
	ON_BN_CLICKED(IDC_BUTTON_L18, OnButtonL18)
	ON_BN_CLICKED(IDC_BUTTON_L19, OnButtonL19)
	ON_BN_CLICKED(IDC_BUTTON_L20, OnButtonL20)
	ON_BN_CLICKED(IDC_BUTTON_L21, OnButtonL21)
	ON_BN_CLICKED(IDC_BUTTON_L22, OnButtonL22)
	ON_BN_CLICKED(IDC_BUTTON_L23, OnButtonL23)
	ON_BN_CLICKED(IDC_BUTTON_L24, OnButtonL24)
	ON_BN_CLICKED(IDC_BUTTON_L25, OnButtonL25)
	ON_BN_CLICKED(IDC_BUTTON_L26, OnButtonL26)
	ON_BN_CLICKED(IDC_BUTTON_L27, OnButtonL27)
	ON_BN_CLICKED(IDC_BUTTON_L28, OnButtonL28)
	ON_BN_CLICKED(IDC_BUTTON_L29, OnButtonL29)
	ON_BN_CLICKED(IDC_BUTTON_L30, OnButtonL30)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTOP10Dialog message handlers

BOOL CTOP10Dialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString imgN;

	imgN.Format("%s\\%s\\TOP10_BG.bmp", Axis::home, IMAGEDIR);

	m_bitmap = CEnBitmap::LoadImageFile(imgN, RGB(255, 255, 255));

	m_font.CreateFont(	   12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,	              // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"±¼¸²");                 // lpszFacename

	CreateBoldFont(m_selfont,90,TRUE,"±¼¸²");
	CreateBoldFont(m_ufont,90,FALSE,"±¼¸²");
	
	if(m_bitmap)
	{
		BITMAP bm;
		
		GetObject(m_bitmap, sizeof(bm), &bm);

		m_static.SetBitmap(m_bitmap);

		m_static.SetWindowPos(NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOZORDER);
	}

	m_brush.CreateSolidBrush(RGB(255, 255, 255));

	CreateBmpButton(IDC_BUTTON_CLOSE,"buy_btn_close",FALSE);

	for(int j=0;j<20;j++)
	{
		imgN.Format("buy_no%02d",j+1);
		CreateBmpButton(IDC_BUTTON_L11+j,imgN,FALSE);
	}
	
	imgN.Format("%s\\%s\\TOP10_BTN.bmp", Axis::home, IMAGEDIR);

	m_checkBtn = CreateBmpButton(IDC_BUTTON_HELP,"buy_check_off",FALSE);

	int nBtnID = 0;

	for(int i = 0; i < 20 ; i++)
	{
		if(i < 10)
			nBtnID = IDC_BUTTON_L1 + i;
		else
			nBtnID = IDC_BUTTON_L31 + i - 10;

		m_updownButtons.Add(CreateBmpButton(nBtnID,"buy_icon_up",FALSE));  

		CreateBmpButton(IDC_BUTTON_L41 + i,"buy_icon_new",FALSE);
		CRect rec;
		((CWnd*)GetDlgItem(IDC_BUTTON_L41 + i))->GetWindowRect(&rec);
		rec.OffsetRect(0, -3);
		ScreenToClient(rec);
		((CWnd*)GetDlgItem(IDC_BUTTON_L41 + i))->MoveWindow(rec);

		GetDlgItem(IDC_BUTTON_L41 + i)->ShowWindow(SW_HIDE);
	}

	GetDlgItem(IDC_STATIC_DATE)->SetWindowText(" Ç×»ó À§ ");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTOP10Dialog::SetBaseRect( CRect rect )
{
	CRect rc;
	GetClientRect(&rc);

	BITMAP bm;
	
	//get the info of the bitmap
	GetObject(m_bitmap, sizeof(bm), &bm);

	m_BaseRect.CopyRect(&rect);
	const int x = m_BaseRect.right - bm.bmWidth + 3;
	const int y = m_BaseRect.top + 30;

	SetWindowPos(&wndTop, x, y, bm.bmWidth, bm.bmHeight, SWP_NOZORDER);
}

void CTOP10Dialog::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
}

void CTOP10Dialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void CTOP10Dialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0;i<20;i++)
	{
		int nID = 0;

		if(i < 10)
		{
			nID = IDC_STATIC_L1 + i;
		}
		else
		{
			nID = IDC_STATIC_L11 + i - 10;
		}

		CRect rc;

		((CStatic*)GetDlgItem(nID))->GetWindowRect(rc);

		ScreenToClient(rc);

		if(rc.PtInRect(point))
		{
			if(i<m_arCodes.GetSize())
			{
				CString code = m_arCodes.GetAt(i);
				
				m_pParent->SendMessage(WD_TOP10,0,(LPARAM)code.operator LPCTSTR());
			}
			break;
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CTOP10Dialog::CreateBoldFont(CFont& font, int point, BOOL bUnderline,const char* fontName)
{
	LOGFONT	lf;
	
	font.CreatePointFont(point, fontName);
	font.GetLogFont(&lf);
	font.DeleteObject();
	
	font.CreateFont(	lf.lfHeight,			// nHeight
		lf.lfWidth,			// nWidth
		lf.lfEscapement,		// nEscapement
		lf.lfOrientation,		// nOrientation
		FW_SEMIBOLD,			// nWeight
		lf.lfItalic,			// bItalic
		bUnderline,			// bUnderline
		lf.lfStrikeOut,			// cStrikeOut
		lf.lfCharSet,			// nCharSet
		lf.lfOutPrecision,		// nOutPrecision
		lf.lfClipPrecision,		// nClipPrecision
		lf.lfQuality,			// nQuality
		lf.lfPitchAndFamily,		// nPitchAndFamily
		(char*)(const char*)fontName);        // lpszFacename//*/
	
}

void CTOP10Dialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	const bool bflag = false;

	CRect rc;
	GetWindowRect(rc);
	ScreenToClient(rc);

	const int nID = 0;
	
	for(int i=0;i<m_arData.GetSize();i++)
	{
		const int nBtnID = 0, nUpDownID = 0, nRchgID = 0;
		int nID{};
		if(i < 10)
		{
			nID = IDC_STATIC_L1 + i;
		}
		else
		{
			nID = IDC_STATIC_L11 + i - 10;
		}

		((CStatic*)GetDlgItem(nID))->GetWindowRect(rc);
		
		ScreenToClient(rc);

		if(rc.PtInRect(point))
		{
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
			
			GetDlgItem(nID)->SetFont(&m_selfont,TRUE);
		}
		else
		{
			CFont* font = GetDlgItem(nID)->GetFont();

			LOGFONT lfont;
			font->GetLogFont(&lfont);
			if(lfont.lfUnderline == TRUE)
				GetDlgItem(nID)->SetFont(&m_font,TRUE);
		}
	}

	m_bOverBtn = bflag;

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CTOP10Dialog::IsData() 
{
	if (m_arData.GetSize() > 0)
		return TRUE;
	else
		return FALSE;
}

void CTOP10Dialog::SetData(const  CStringArray& arr,const CStringArray& arcodes )
{
	if(arr.GetSize() == 0)
		ShowWindow(SW_HIDE);

	m_arData.RemoveAll();
	m_rateColor.RemoveAll();
	m_arNew.RemoveAll();
	for(int i=0;i<20;i++)
	{
		if(i<arr.GetSize())
		{
			CString data = arr.GetAt(i);

			CString name,rchg,isnew;

			const int nFirst = data.Find("\t");
			name = data.Mid(0,nFirst + 1);

			const int nSecond = data.Find("\t",nFirst+1);
			rchg = data.Mid(nFirst,nSecond - nFirst);

			isnew = data.Mid(nSecond+1);

			m_arData.Add(data);
			const int isize = m_arData.GetSize();
			
			if(data.GetLength() > 30)
				data = data.Mid(0,30);

			if(isnew == "Y")
			{
				//m_arNew.Add("NEW");
				((CBmpButton*)GetDlgItem(IDC_BUTTON_L41 + i))->SetImgBitmap("buy_icon_new", 1);
				GetDlgItem(IDC_BUTTON_L41 + i)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BUTTON_L41 + i)->EnableWindow(TRUE);
			}
			else if(isnew == "U")
			{
				((CBmpButton*)GetDlgItem(IDC_BUTTON_L41 + i))->SetImgBitmap("buy_icon_upup", 1);
				GetDlgItem(IDC_BUTTON_L41 + i)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BUTTON_L41 + i)->EnableWindow(TRUE);
			}
			else
			{
				//m_arNew.Add("NOT");
				((CBmpButton*)GetDlgItem(IDC_BUTTON_L41 + i))->SetImgBitmap("buy_icon_new", 1);
				GetDlgItem(IDC_BUTTON_L41 + i)->ShowWindow(SW_HIDE);
			}
			

			const int  nUpDownID = 0 , nRchgID = 0;
			int nID{}, nBtnID{};
			if(i < 10)
			{
				nID = IDC_STATIC_L1 + i;
				nBtnID = IDC_BUTTON_L1 + i;
			}
			else
			{
				nID = IDC_STATIC_L11 + i - 10;
				nBtnID = IDC_BUTTON_L31 + i - 10;
			}
			rchg.TrimLeft();
			rchg.TrimRight();

			CBmpButton* updownBtn = m_updownButtons.GetAt(i);

			if(rchg.GetAt(0) == '-')
			{
				updownBtn->SetImgBitmap("buy_icon_down",1);
				((CButton*)GetDlgItem(nBtnID))->ShowWindow(SW_SHOW);
				((CButton*)GetDlgItem(nBtnID))->EnableWindow(TRUE);
				((CStatic*)GetDlgItem(IDC_STATIC_RCHG + i))->ShowWindow(SW_SHOW);

				rchg.Remove('-');

				m_rateColor.Add("M");
			}
			else if(rchg.Find("new") > -1)
			{
				updownBtn->SetImgBitmap("buy_icon_new",1);
				((CButton*)GetDlgItem(nBtnID))->ShowWindow(SW_SHOW);

				const CRect rc;
				
				((CStatic*)GetDlgItem(IDC_STATIC_RCHG + i))->ShowWindow(SW_HIDE);
				m_rateColor.Add("0");
			}
			else if(rchg.GetAt(0) == '0')
			{
				((CButton*)GetDlgItem(nBtnID))->ShowWindow(SW_HIDE);
				((CButton*)GetDlgItem(nBtnID))->EnableWindow(FALSE);
				((CStatic*)GetDlgItem(IDC_STATIC_RCHG + i))->ShowWindow(SW_HIDE);
				m_rateColor.Add("0");
			}
			else
			{
				updownBtn->SetImgBitmap("buy_icon_up",1);
				((CButton*)GetDlgItem(nBtnID))->ShowWindow(SW_SHOW);
				((CButton*)GetDlgItem(nBtnID))->EnableWindow(TRUE);
				((CStatic*)GetDlgItem(IDC_STATIC_RCHG + i))->ShowWindow(SW_SHOW);
				m_rateColor.Add("P");
			}

			((CStatic*)GetDlgItem(IDC_STATIC_RCHG + i))->SetWindowText(rchg);

			name.TrimRight();

			((CStatic*)GetDlgItem(nID))->SetWindowText(name);
			((CStatic*)GetDlgItem(nID))->EnableWindow(TRUE);
			((CStatic*)GetDlgItem(nID))->ShowWindow(SW_SHOW);
			((CButton*)GetDlgItem(IDC_BUTTON_L11 + i))->ShowWindow(SW_SHOW);
			((CButton*)GetDlgItem(IDC_BUTTON_L11 + i))->EnableWindow(TRUE);
	
		}
		else
		{
			int nID{}, nBtnID = 0;
			
			if(i < 10)
			{
				nID = IDC_STATIC_L1 + i;
				nBtnID = IDC_BUTTON_L1 + i;
			}
			else
			{
				nID = IDC_STATIC_L11 + i - 10;
				nBtnID = IDC_BUTTON_L31 + i - 10;
			}
			m_rateColor.Add("0");
			m_arNew.Add("NOT");

			((CStatic*)GetDlgItem(nID))->SetWindowText("");
			((CStatic*)GetDlgItem(nID))->EnableWindow(FALSE);
			((CStatic*)GetDlgItem(nID))->ShowWindow(SW_HIDE);
			((CButton*)GetDlgItem(IDC_BUTTON_L11 + i))->ShowWindow(SW_HIDE);
			((CButton*)GetDlgItem(IDC_BUTTON_L11 + i))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(nBtnID))->ShowWindow(SW_HIDE);
			((CButton*)GetDlgItem(nBtnID))->EnableWindow(FALSE);
			((CStatic*)GetDlgItem(IDC_STATIC_RCHG + i))->ShowWindow(SW_HIDE);
			((CStatic*)GetDlgItem(IDC_STATIC_RCHG + i))->SetWindowText("");
			GetDlgItem(IDC_BUTTON_L41+i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_L41+i)->EnableWindow(FALSE);
		}
	}

	Invalidate();

	m_arCodes.RemoveAll();
	for(int j=0;j<arcodes.GetSize();j++)
	{
		m_arCodes.Add(arcodes.GetAt(j));
	}
}

HBRUSH CTOP10Dialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	const int nCtrlID = pWnd->GetDlgCtrlID();
	if ((nCtrlID >= IDC_STATIC_L1 && nCtrlID <= IDC_STATIC_DATE) || (nCtrlID >= IDC_STATIC_L11 && nCtrlID <= IDC_STATIC_L20) || (nCtrlID >= IDC_STATIC_RCHG && nCtrlID <= IDC_STATIC_RCHG20))
	{
		HGDIOBJ hBrush = m_brush.GetSafeHandle();
		SetBkMode(pDC->m_hDC, TRANSPARENT);
		if(nCtrlID >= IDC_STATIC_RCHG && nCtrlID <= IDC_STATIC_RCHG20)
		{
			const int nIndex = nCtrlID - IDC_STATIC_RCHG;

			if(nIndex > -1 && nIndex < m_rateColor.GetSize())
			{
				CString sValue = m_rateColor.GetAt(nIndex);
				
				if(sValue == "P")
					pDC->SetTextColor(RGB(255,0,0));
				else if(sValue == "M")
					pDC->SetTextColor(RGB(0,0,255));
				else
					pDC->SetTextColor(RGB(0,0,0));
			}
		}

		return (HBRUSH)hBrush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CTOP10Dialog::OnDestroy() 
{
	CDialog::OnDestroy();
	

	m_brush.DeleteObject();
}

void CTOP10Dialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
}

void CTOP10Dialog::OnButtonL1() 
{
	
}

#pragma warning (disable : 26400)
#pragma warning (disable : 26409)
CBmpButton* CTOP10Dialog::CreateBmpButton(UINT buttonID, const char* bmpFileName, BOOL check)
{
	CBmpButton* btn = new CBmpButton;
	btn->SetImgBitmap(bmpFileName, 1);
	btn->SubclassWindow(GetDlgItem(buttonID)->m_hWnd);
	if (check)
	{
		btn->ModifyStyle(0, BS_AUTORADIOBUTTON);
		btn->ModifyStyle(0, BS_PUSHLIKE);
	}

	GetDlgItem(buttonID)->Invalidate();
	btn->Invalidate(TRUE);
	return btn;
}
#pragma warning (default : 26400)
#pragma warning (default : 26409)
void CTOP10Dialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	m_bShown = bShow;
	
	if(bShow)
	{
		SetFocus();
	}
	// TODO: Add your message handler code here
}

void CTOP10Dialog::OnButtonL2() 
{
	// TODO: Add your control notification handler code here
	//CString code = m_arCodes.GetAt(1);
	
	//m_pParent->SendMessage(WD_TOP10,0,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL3() 
{
	// TODO: Add your control notification handler code here
	//CString code = m_arCodes.GetAt(2);
	
	//m_pParent->SendMessage(WD_TOP10,0,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL4() 
{
	// TODO: Add your control notification handler code here
	//CString code = m_arCodes.GetAt(3);
	
	//m_pParent->SendMessage(WD_TOP10,0,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL5() 
{
	// TODO: Add your control notification handler code here
	//CString code = m_arCodes.GetAt(4);
	
	//m_pParent->SendMessage(WD_TOP10,0,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL6() 
{
	// TODO: Add your control notification handler code here
	//CString code = m_arCodes.GetAt(5);
	
	//m_pParent->SendMessage(WD_TOP10,0,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL7() 
{
	// TODO: Add your control notification handler code here
	//CString code = m_arCodes.GetAt(6);
	
	//m_pParent->SendMessage(WD_TOP10,0,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL8() 
{
	// TODO: Add your control notification handler code here
	//CString code = m_arCodes.GetAt(7);
	
	//m_pParent->SendMessage(WD_TOP10,0,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL9() 
{
	// TODO: Add your control notification handler code here
	//CString code = m_arCodes.GetAt(8);
	
	//m_pParent->SendMessage(WD_TOP10,0,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL10() 
{
	// TODO: Add your control notification handler code here
	//CString code = m_arCodes.GetAt(9);
	
	//m_pParent->SendMessage(WD_TOP10,0,(LPARAM)code.operator LPCTSTR());
}

LRESULT CTOP10Dialog::OnMouseLeave( WPARAM wparam, LPARAM lparam )
{
	m_bTracking = FALSE;
	m_bMouseIn = FALSE;

	//ShowWindow(SW_HIDE);
	return 1;
}

void CTOP10Dialog::OnButtonHelp() 
{
	// TODO: Add your control notification handler code here
	//m_pParent->SendMessage(WD_TOP10,2,0);
	if(!m_bAlways)
	{
		m_bAlways = true;
		
		m_checkBtn->SetImgBitmap("buy_check_on",1);
	}
	else
	{
		m_bAlways = false;
		
		m_checkBtn->SetImgBitmap("buy_check_off",1);
	}
}

HBITMAP CTOP10Dialog::SetBitMap(CString name)
{
	HBITMAP hBitmap;
	CString fileName;

	fileName.Format("%s\\image\\%s.bmp", Axis::home, name);
	
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), fileName, 
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	return hBitmap;
}

void CTOP10Dialog::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
}

void CTOP10Dialog::SetCurrent( int index )
{
	m_nPrevious = m_nCurrent;
	
	m_nCurrent = index;
	
	int nID{}, nPreID = 0;
	
	if(index < 10)
	{
		nID = IDC_STATIC_L1 + index;
	}
	else
	{
		nID = IDC_STATIC_L11 + index - 10;
	}
	
	if(m_nPrevious < 10)
	{
		nPreID = IDC_STATIC_L1 + m_nPrevious;
	}
	else
	{
		nPreID = IDC_STATIC_L11 + m_nPrevious - 10;
	}
	
	GetDlgItem(nID)->SetFont(&m_ufont,TRUE);
	GetDlgItem(nPreID)->SetFont(&m_font,TRUE);
}

void CTOP10Dialog::OnButtonL11() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(0);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL12() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(1);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL13() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(2);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL14() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(3);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL15() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(4);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL16() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(5);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL17() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(6);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL18() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(7);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL19() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(8);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL20() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(9);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL21() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(10);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL22() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(11);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL23() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(12);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL24() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(13);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL25() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(14);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL26() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(15);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL27() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(16);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL28() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(17);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL29() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(18);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}

void CTOP10Dialog::OnButtonL30() 
{
	// TODO: Add your control notification handler code here
	CString code = m_arCodes.GetAt(19);
	
	m_pParent->SendMessage(WD_TOP10,1,(LPARAM)code.operator LPCTSTR());
}
