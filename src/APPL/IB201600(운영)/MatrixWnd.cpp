// MatrixWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ib201600.h"
#include "MatrixWnd.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMatrixWnd

CMatrixWnd::CMatrixWnd(CMainWnd* parent, CWnd* mainWnd)
{
	m_pParent = (CMainWnd*)parent;
	m_pMainWnd = mainWnd;
	
	initMatrix();

	EnableAutomation();
}

CMatrixWnd::~CMatrixWnd()
{

}

void CMatrixWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMatrixWnd, CWnd)
	//{{AFX_MSG_MAP(CMatrixWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMatrixWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMatrixWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMatrixWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {A76C7CE4-07DF-4E22-B5ED-C0EA4ECB71FB}
static const IID IID_IMatrixWnd =
{ 0xa76c7ce4, 0x7df, 0x4e22, { 0xb5, 0xed, 0xc0, 0xea, 0x4e, 0xcb, 0x71, 0xfb } };

BEGIN_INTERFACE_MAP(CMatrixWnd, CWnd)
	INTERFACE_PART(CMatrixWnd, IID_IMatrixWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMatrixWnd message handlers

int CMatrixWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect	rect, clientRC; 
	GetClientRect(clientRC);
	
	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");
	m_font = getAxFont(_T("굴림체"), 9, true, false);
	m_strPal = GetPalette();
	
	CString	imgPath; 
	imgPath.Format("%s/%s/", m_root, IMAGEDIR);
	
	SetOwner(m_pMainWnd);
	
	m_labelClose = std::make_unique<CStatic>();
	rect.top = clientRC.top + 82;
	rect.bottom = rect.top + 16;
	rect.left = 0;
	rect.right = 160;
	m_labelClose->Create("닫    기", WS_CHILD | WS_VISIBLE | SS_CENTER, rect, this, 8040);
	m_labelClose->SetFont(m_font);

	m_labelClose->ShowWindow(SW_SHOW);
	return 0;
}

void CMatrixWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	xxx::CMemDC memdc(&dc);
	CRect rc, rcRtg;
	GetClientRect(&rc);
	CPen myPen, *pOldPen = nullptr;

	myPen.CreatePen(PS_SOLID, 1, GetColor(255));
	pOldPen = memdc.SelectObject(&myPen);

	memdc.FillSolidRect(&rc,GetColor(64));
	
	rcRtg.top = rc.top;
	rcRtg.bottom = rcRtg.top+20;
	rcRtg.left = rc.left;
	rcRtg.right = rcRtg.left+20;

	for(int ii=0 ; ii<MATRIX_MAXCOL	; ii++)
	{
		for(int jj=0 ; jj<MATRIX_MAXROW ; jj++)
		{
			memdc.FillSolidRect(&rcRtg, GetColor(255));
			memdc->Rectangle(rcRtg);

			if(!m_aryMatrix[ii][jj])
			{
				memdc.FillSolidRect(rcRtg.left+1, rcRtg.top+1, rcRtg.Width()-2, rcRtg.Height()-2, GetColor(181));
			}
			else
			{
				memdc.FillSolidRect(rcRtg.left+1, rcRtg.top+1, rcRtg.Width()-2, rcRtg.Height()-2, GetColor(64));
			}

			rcRtg.left = rcRtg.right;
			rcRtg.right = rcRtg.left+20;
		}

		rcRtg.left = 0;
		rcRtg.right = rcRtg.left+20;		
		rcRtg.top = rcRtg.bottom;
		rcRtg.bottom = rcRtg.top+20;
	}
	// Do not call CWnd::OnPaint() for painting messages
	myPen.DeleteObject();
}

void CMatrixWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	const int row = 1, col = 1;

	GetRowColFromPoint(point, row, col);
	Invalidate(FALSE);

	CWnd::OnMouseMove(nFlags, point);
}

//행열 개수 구하기
void CMatrixWnd::GetRowColFromPoint(CPoint point, int row, int col)
{
	CRect rect;
	int ii{}, jj{};
	GetClientRect(rect);
	
	for(ii=20 ; ii<=MATRIX_MAXROW*20 ; ii=ii+20)
	{
		if(point.x > ii)
		{
			row++;
		}
	}
	
	if(point.y < 80)
	{
		for(jj=20 ; jj<=MATRIX_MAXCOL*20 ; jj=jj+20)
		{
			if(point.y > jj)
			{
				col++; 
			}	
		}
	}
	else
	{
		row = 0;
		col = 0;
	}


	m_iCols = col;
	m_iRows = row;

	m_pParent->SetCols(m_iCols);
	m_pParent->SetRows(m_iRows);

	initMatrix();

	for(ii=1 ; ii<=col ; ii++)
	{
		for(jj=1 ; jj<=row ; jj++)
		{
			m_aryMatrix[ii-1][jj-1] = TRUE;
		}
	}
}


void CMatrixWnd::Draw(CDC *pDC, CRect rcClient)
{
	CRect rect;
	
	GetClientRect(&rect);
	
	//background
	pDC->FillSolidRect(&rect,GetColor(0));//64
	
	//사각 스퀘어 여기다 그린다
	
	
}

void CMatrixWnd::SendMatrixInfo(bool result)
{
	if(result)
	{
		m_pParent->SendMessage(WM_MANAGE, MK_SETROWCOL);
	}
}


void CMatrixWnd::initMatrix()
{
	for(int ii=0 ; ii<MATRIX_MAXCOL ; ii++)
	{
		for(int jj=0 ; jj<MATRIX_MAXROW ; jj++)
		{
			m_aryMatrix[ii][jj] = FALSE;
		}
	}
}

LONG CMatrixWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	const int ret = 0;
	switch (LOWORD(wParam))
	{
	case MK_SETPAL:
		m_strPal = GetPalette();
		m_clrPallete = (COLORREF)lParam;
		
		Invalidate(FALSE);
		break;
	}
	return ret;
}

CBrush* CMatrixWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pMainWnd->SendMessage(WM_USER, getBRUSH, (long)clr);
}


CFont* CMatrixWnd::getAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;
	
	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;

	return (CFont*)SendAxisMessage( getFONT, (long)&fontR);
}


COLORREF CMatrixWnd::GetColor(int color)
{
	if (color & 0x02000000)
		return color;
	
	return SendAxisMessage( MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CString	CMatrixWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	CString strPath = Variant(homeCC, "");
	
	file.Format("%s\\%s\\%s", strPath, "tab", "palette.ini");
	
	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;
	
	return strPal;
}

CString	CMatrixWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
	if((long)dta > 1)
		retvalue = CString(dta);
	
	return retvalue;
}

LRESULT CMatrixWnd::SendAxisMessage(WPARAM wParam, LPARAM lPAram)
{	
	return m_pMainWnd->SendMessage(WM_USER, wParam, lPAram);
}

BOOL CMatrixWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (LOWORD(wParam))
	{
	case 8030:
	
		break;
	}	
	return CWnd::OnCommand(wParam, lParam);
}

int	CMatrixWnd::GetRows()
{
	return m_iRows;
}


int	CMatrixWnd::GetCols()
{
	return m_iCols;
}

void CMatrixWnd::SetRows(int row)
{
	m_iRows = row;
}


void CMatrixWnd::SetCols(int col)
{
	m_iCols = col;
}

void CMatrixWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect matrixRc;
	
	GetClientRect(matrixRc);
	
	if(point.x >= matrixRc.top && point.x <= matrixRc.Width() && point.y >= matrixRc.top && point.y <= matrixRc.Height() -20)
	{
		m_pParent->SetIsOpen(true);
		SendMatrixInfo(true);
	}
	else
	{
		initMatrix();
		m_iCols = 0;
		m_iRows = 0;

		m_pParent->SendMessage(WM_MANAGE, MK_HIDEMATRIX);
	}
	
	ShowWindow(SW_HIDE);
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CMatrixWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
}

HBRUSH CMatrixWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		HDC hdc = pDC->m_hDC;
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, GetColor(181));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
		return hbr;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
