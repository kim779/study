// DlgPB.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "DlgPB.h"
#include "../dll/ctrl/MemDC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPB dialog
#define	TM_STARTSLIDE	999
#define	TM_SLIDE	100
#define	TMI_SLIDE	100
#define	SLIDEGAP	10
#define	GAPX		4
#define SIZE_MARGIN	20

#define TITLE_HEIGHT 25

#define DATALINE_SIZE 3
#define RECT_HEIGHT 105
#define LINE_WIDTH 180
#define MINIMUM_WIDTH 220

#define TM_SHOWTIME 10000

CDlgPB::CDlgPB(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPB)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nRowHeight = 0;
	m_height = m_width = 0;
	m_pMain = (CMainFrame*)pParent;
	
	m_font.CreateFont(	12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,	           // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,           // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("돋움체"));     
	
	m_bmp = Axis::GetBitmap("MESSAGE02");
	m_stretch = FALSE;
}


void CDlgPB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPB)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPB, CDialog)
	//{{AFX_MSG_MAP(CDlgPB)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPB message handlers

void CDlgPB::ShowPBSlide(CRect baseRC)
{
	m_baseRc.CopyRect(&baseRC);
	const int x = m_baseRc.right - m_width;
	const int y = m_baseRc.top;

	SetWindowPos(&wndTop, x, y, m_width, m_height, SWP_NOACTIVATE|SWP_SHOWWINDOW);
	SetTimer(TM_STARTSLIDE, TM_SHOWTIME, NULL);
}

void CDlgPB::HideSlide()
{
	SetTimer(TM_SLIDE, TMI_SLIDE, NULL);
}

void CDlgPB::Slide()
{
	CRect wRc;
	int height{}, x{}, y{};
	const UINT uFlags = SWP_NOACTIVATE|SWP_SHOWWINDOW;

	GetWindowRect(wRc);
	height = wRc.Height() - SLIDEGAP;
	if (height <= 10)
	{
		KillTimer(TM_SLIDE);
		ShowWindow(SW_HIDE);
		this->PostMessage(WM_DESTROY, 0, 0); 
		return; 
	}

	x = m_baseRc.right - m_width;
	y = m_baseRc.top;

	if(m_bshow)
		SetWindowPos(&wndTop, x, y, m_width, height, uFlags);
}

void CDlgPB::StopSlide()
{
	KillTimer(TM_SLIDE);
}

void CDlgPB::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TM_STARTSLIDE:
		{
			KillTimer(TM_STARTSLIDE);
			HideSlide();
		}
		break;
	case TM_SLIDE:	
		Slide();
		break;
	default:		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CDlgPB::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC	mdc(&dc);
	OnDraw(&mdc);	
}

void CDlgPB::OnDraw(CDC* pDC)
{
	CFont*	old = pDC->SelectObject(&m_font);
	pDC->SetBkMode(TRANSPARENT);

	DrawTitle(pDC);
	DrawBack(pDC);
	DrawData(pDC);
	
	pDC->SelectObject(old);
}

void CDlgPB::DrawTitle(CDC* pDC)
{
	CRect rec;
	rec.CopyRect(m_rect);
	rec.bottom = rec.top + TITLE_HEIGHT;

	pDC->FillSolidRect(rec, RGB(0,0,64));
	
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->DrawText(" 촉n토크", rec, DT_VCENTER|DT_LEFT|DT_SINGLELINE);

	//닫기 버튼 
	CRect recbtn;
	recbtn.left = m_rect.right - 20;
	recbtn.right = m_rect.right - 5;
	recbtn.top = m_rect.top + 5;
	recbtn.bottom = m_rect.top + 20;
	recbtn.OffsetRect(0,1);
	pDC->FillSolidRect(recbtn, RGB(255,0,0));

	CPen	cPen(PS_SOLID, 2, RGB(255,255,255));
	const CPen *sPen = pDC->SelectObject(&cPen);

	pDC->MoveTo(recbtn.left + 2, recbtn.top + 2);
	pDC->LineTo(recbtn.right - 4, recbtn.bottom - 4);

	pDC->MoveTo(recbtn.right - 4, recbtn.top + 2);
	pDC->LineTo(recbtn.left + 2, recbtn.bottom - 4);
}

void CDlgPB::DrawBack(CDC* pDC)
{
	pDC->FillSolidRect(0, TITLE_HEIGHT + 2, m_width, m_height - TITLE_HEIGHT,  RGB(255, 255, 255));
	DrawFrame(pDC);
}

void CDlgPB::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_font.DeleteObject();
	m_pMain->HidePBArrItem(this);
}

void CDlgPB::DrawFrame(CDC* pDC)
{
	CBrush	brush1(RGB(55, 77, 116)), brush2(RGB(38, 109, 147)), brush3(RGB(97, 120, 164));	
	CRect	rect;
	rect = m_rect;
	rect.top += TITLE_HEIGHT + GAPX;
	pDC->FrameRect(rect, &brush1);
	rect.DeflateRect(1, 1);
}	

void CDlgPB::DrawData(CDC* pDC)
{
	const int	ncnt = m_arData.GetSize();
	CRect	cRc = m_rect;
	cRc.DeflateRect(3, 3);
	CRect	rcText;
	CString	sztmp;
	rcText = cRc;
	rcText.top += TITLE_HEIGHT + m_nRowHeight;
	rcText.bottom = rcText.top + m_nRowHeight;
	rcText.left += 20;
	const int	nMode = pDC->SetBkMode(TRANSPARENT);
	
	COLORREF color;
	color = pDC->SetTextColor(RGB(0, 0, 0));
	
	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{	
		if(ii < DATALINE_SIZE)
		{
			sztmp = m_arData.GetAt(ii);

			if( ii == DATALINE_SIZE - 1 && m_arData.GetSize() > DATALINE_SIZE)
				sztmp.Replace(sztmp.Right(3), "...");

			pDC->DrawText(sztmp, rcText, DT_VCENTER|DT_LEFT|DT_SINGLELINE);
			rcText.OffsetRect(0, m_nRowHeight);
		}
	}
	
	pDC->SetTextColor(color);
	pDC->SetBkMode(nMode);
}

void CDlgPB::SetData(CString dat, int kind)
{
	CString stemp, slinedata;
	CSize	size;
	const CRect	dispRc;
	const int	nMax = 0, nMaxY = TITLE_HEIGHT;
	CDC*	pDC = GetDC();
	CFont*	font = pDC->SelectObject(&m_font);
	
	int width_han = pDC->GetTextExtent("한").cx;
	int width_total = 0;
	bool bhan = false;
	m_bshow = true;
	
	const int cnt = 0;
	m_arData.RemoveAll();

	for(int ii = 0 ; ii < dat.GetLength() ; ii++)
	{
CString slog;
	
		if((dat.GetAt(ii) & 0x80) == 0x80)
			bhan = true;
		else
			bhan = false;

		if(!bhan)   //한글이 아닐경우
		{
			stemp = dat.GetAt(ii);
			slinedata += stemp;

			width_total += pDC->GetTextExtent(stemp).cx;
			
			size = pDC->GetTextExtent(slinedata);

			if(width_total > LINE_WIDTH)
			{
				m_arData.Add(slinedata);
				slinedata.Empty();
				width_total = 0;
			}
		}
		else		//한글인경우
		{
			stemp.Format("%c%c", dat.GetAt(ii), dat.GetAt(ii + 1));
			ii++;
			slinedata += stemp;	
		
			width_total += width_han;

			if(width_total > LINE_WIDTH)
			{
				m_arData.Add(slinedata);
				slinedata.Empty();
				width_total = 0;
			}
			
		}
	}

	if (slinedata.GetLength() > 0)
	{
		m_arData.Add(slinedata);
		size = pDC->GetTextExtent(slinedata);	
		slinedata.Empty();
	}

	m_nRowHeight = size.cy + GAPX;

	pDC->SelectObject(font);	
	ReleaseDC(pDC);

	m_height = RECT_HEIGHT;

	m_width = 230;
	SetWindowPos(NULL, m_baseRc.right - m_width, m_baseRc.top, m_width, m_height, SWP_NOACTIVATE|SWP_HIDEWINDOW);
	m_rect = CRect(0, 0, m_width, m_height);
}

void CDlgPB::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CRect rect;
	rect.left = m_rect.right - 25;
	rect.top = m_rect.top;
	rect.right = m_rect.right;
	rect.bottom = rect.top + 25;

	if(rect.PtInRect(point))
	{
		m_bshow = false;
		KillTimer(TM_SLIDE);
		ShowWindow(SW_HIDE);
		this->PostMessage(WM_DESTROY, 0, 0); 
	}
	else
	{
		m_pMain->OpenPBNews("");
	}

	CDialog::OnLButtonUp(nFlags, point);
}




void CDlgPB::DrawBitmap(CDC* pDC)
{
	ASSERT(m_bmp);
	
	CDC mdc;
	mdc.CreateCompatibleDC(pDC);
	
	CBitmap* oldBmp = mdc.SelectObject(m_bmp);
	
	if(m_stretch)
	{  
		pDC->StretchBlt(0, TITLE_HEIGHT + 2, m_width , m_height - TITLE_HEIGHT , &mdc, 2, 0, m_bmpSize.cx, m_bmpSize.cy, SRCCOPY);
	}
	else
	{
		pDC->BitBlt(0, 0, m_width, m_height, &mdc, 0, 0, SRCCOPY);
	}
	
	mdc.SelectObject(oldBmp);
}


void CDlgPB::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
/*
CString slog;
slog.Format("PB [%d] [%d] 한글사이즈=[%d] 영어사이즈=[%d]", dat.GetLength(), strlen((LPSTR)(LPCTSTR)dat), width_han, pDC->GetTextExtent("K").cx);
OutputDebugString(slog);
	
	for(int ii = 0 ; ii < dat.GetLength() ; ii++)
	{
		if((dat.GetAt(ii) & 0x80) == 0x80)
				bhan = true;

		if(!bhan)   //한글이 아닐경우
		{
			stemp.Format("%c", dat.GetAt(ii));
			slinedata += stemp;
			cnt++;

			width_total += pDC->GetTextExtent("stemp").cx;

			size = pDC->GetTextExtent(slinedata);

CString slog;
slog.Format("PB [ENG] %d [%s] stemp=%s", width_total, slinedata, stemp);
OutputDebugString(slog);

			if(width_total > 200)
			{
				m_arData.Add(slinedata);
				slinedata.Empty();
				
				if (width_total > nMax)
					nMax = width_total;	

				width_total = 0;
			}
		}
		else   //한글인 경우
		{
			if(ii + 1 < dat.GetLength())
			{
				stemp.Format("%c%c", dat.GetAt(ii), dat.GetAt(ii+1));
				slinedata += stemp;
				ii++;
				cnt++;

				width_total += width_han;
			}
			else
			{
				stemp.Format("%c", dat.GetAt(ii));
				slinedata += stemp;
				ii++;
				cnt++;	
				
				width_total += width_han;
			}
			
CString slog;
slog.Format("PB [ENG] [%d] [%s] stemp=%s", width_total, slinedata, stemp);
OutputDebugString(slog);

			if(width_total > 200)
			{
				m_arData.Add(slinedata);
				slinedata.Empty();

				if (width_total > nMax)
					nMax = width_total;	

				width_total = 0;
			}
		}
	}
*/

	/*
	for(int ii = 0 ; ii < dat.GetLength() ; ii++)
	{
		stemp.Empty();
		if(cnt % ONELINE_TEXT == 0  && (cnt != 0))
		{
			if((dat.GetAt(ii) & 0x80) == 0x80)
				bhan = true;
			
			if(!bhan)
			{
				stemp.Format("%c", dat.GetAt(ii));
				slinedata += stemp;
				cnt++;
			}
			else
			{
				if(ii + 1 < dat.GetLength())
				{
					stemp.Format("%c%c", dat.GetAt(ii), dat.GetAt(ii+1));
					slinedata += stemp;
					ii++;
					cnt++;
				}
				else
				{
					stemp.Format("%c", dat.GetAt(ii));
					slinedata += stemp;
					ii++;
					cnt++;		
				}
			}
			
			m_arData.Add(slinedata);
			size = pDC->GetTextExtent(slinedata);
			slinedata.Empty();
			
			if (size.cx > nMax)
				nMax = size.cx;	

			stemp.Empty();	
		}
		else
		{
			if((dat.GetAt(ii) & 0x80) == 0x80)
				bhan = true;
			
			if(!bhan)
			{
				stemp.Format("%c", dat.GetAt(ii));
				slinedata += stemp;
				cnt++;
			}
			else
			{
				if(ii + 1 < dat.GetLength())
				{
					stemp.Format("%c%c", dat.GetAt(ii), dat.GetAt(ii+1));
					slinedata += stemp;
					ii++;
					cnt++;
				}
				else
				{
					stemp.Format("%c", dat.GetAt(ii));
					slinedata += stemp;
					ii++;
					cnt++;
				}
			}
		}
	}
	
	if (slinedata.GetLength() > 0)
	{
		m_arData.Add(slinedata);
		size = pDC->GetTextExtent(slinedata);
		
		if (size.cx > nMax)
				nMax = size.cx;	

		slinedata.Empty();
	}
	*/

/*
void CDlgPB::SetData(CString dat, int kind)
{
	CString stemp, slinedata;
	CSize	size;
	CRect	dispRc;
	int	nMax = 0, nMaxY = TITLE_HEIGHT;
	CDC*	pDC = GetDC();
	CFont*	font = pDC->SelectObject(&m_font);
	
	int width_han = pDC->GetTextExtent("한").cx;
	int width_total = 0;
	bool bhan = false;
	m_bshow = true;
	
	int cnt = 0;
	m_arData.RemoveAll();

	int nLen = MultiByteToWideChar(CP_ACP, 0, (LPSTR)dat.operator LPCSTR(), lstrlen((LPSTR)(LPCTSTR)dat), NULL, NULL);
	WCHAR* pdwdata = new WCHAR[nLen + 2];
	memset(pdwdata, 0, nLen + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPSTR)dat.operator LPCSTR(), -1, pdwdata, nLen);


	for(int ii = 0 ; ii < nLen ; ii++)
	{
CString slog;
	
	//	if((dat.GetAt(ii) & 0x80) == 0x80)
		if((pdwdata[ii] & 0x80) == 0x80)
			bhan = true;
		else
			bhan = false;

		if(!bhan)   //한글이 아닐경우
		{
			stemp.Format("%c", pdwdata[ii]);
			slinedata += stemp;

			width_total += pDC->GetTextExtent(stemp).cx;
			
			size = pDC->GetTextExtent(slinedata);


slog.Format("PB [ENG] %d  stemp=%s", width_total, stemp);
OutputDebugString(slog);

			if(width_total > LINE_WIDTH)
			{
				m_arData.Add(slinedata);
				slinedata.Empty();
				
				if (width_total > nMax)
					nMax = width_total;	
				
				width_total = 0;
			}
		}
		else		//한글인경우
		{
			char* str2;
		//	int len = WideCharToMultiByte(CP_ACP, 0, pdwdata + ii, -1, str2, 0, NULL, NULL);
			int len = 2;
			str2 = new char[len + 2];
			memset(str2, 0x00, len + 2);
			WideCharToMultiByte(CP_ACP, 0, pdwdata + ii, -1, str2, len, NULL, NULL);

			stemp.Format("%s", str2);
			slinedata += stemp;
		
CString slog;
slog.Format("PB [KOR] [%d] stemp=%s [%d]", width_total, str2, len);
OutputDebugString(slog);
			
			width_total += width_han;

			if(width_total > LINE_WIDTH)
			{
				m_arData.Add(slinedata);
				slinedata.Empty();
				
				if (width_total > nMax)
					nMax = width_total;	
				
				width_total = 0;
			}
			
		}
	}

	if (slinedata.GetLength() > 0)
	{
		m_arData.Add(slinedata);
		size = pDC->GetTextExtent(slinedata);
		
		if (size.cx > nMax)
			nMax = size.cx;	
		
		slinedata.Empty();
	}

	m_nRowHeight = size.cy + GAPX;

	size.cx = nMax;
	pDC->SelectObject(font);	
	ReleaseDC(pDC);

	
	dispRc.SetRect(0, 0, size.cx, RECT_HEIGHT);
	dispRc.InflateRect(SIZE_MARGIN, 0);
	dispRc.InflateRect(4, 4, 2, 2);
	
	m_width = dispRc.Width();
	m_height = RECT_HEIGHT;

	if(m_width < MINIMUM_WIDTH)
		m_width = MINIMUM_WIDTH;

	m_width = 230;
	SetWindowPos(NULL, 0, 0, m_width, m_height, SWP_NOACTIVATE|SWP_HIDEWINDOW);
	m_rect = CRect(0, 0, m_width, m_height);
}
*/

BOOL CDlgPB::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
