// SoundSetWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SoundSetWnd.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;
/////////////////////////////////////////////////////////////////////////////
// CSoundSetWnd

CSoundSetWnd::CSoundSetWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	
	m_brushStatic.CreateSolidBrush(COLOR_SETWND_FACE);
}

CSoundSetWnd::~CSoundSetWnd()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	
	m_brushStatic.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSoundSetWnd, CWnd)
//{{AFX_MSG_MAP(CSoundSetWnd)
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_CTLCOLOR()
ON_WM_PAINT()
//}}AFX_MSG_MAP

ON_BN_CLICKED(IDC_BTN_NEWSCHK, OnBtnNewsChk)
ON_BN_CLICKED(IDC_BTN_CONDITIONCHK, OnBtnConditionChk)
ON_BN_CLICKED(IDC_BTN_FINDCHK, OnBtnFindChk)


END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSoundSetWnd message handlers

int CSoundSetWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	m_btnNewsChk.Create(_T("뉴스신호"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_NEWSCHK);
	m_btnNewsChk.SetFont(&m_fontDefault);
	
	m_btnConditionChk.Create(_T("주요지표신호"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_CONDITIONCHK);
	m_btnConditionChk.SetFont(&m_fontDefault);
	
	m_btnFindChk.Create(_T("조건검색신호"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_FINDCHK);
	m_btnFindChk.SetFont(&m_fontDefault);
	
	
	CString strTitle;
	int nCol, nRow;
	int nIDC_Index = 0;
	for (nCol = 0; nCol < IDC_COL_CNT; nCol++)
	{
		if (nCol == 0)
			strTitle = "매수신호";
		else if (nCol == 1)
			strTitle = "매도신호";
		else if (nCol == 2)
			strTitle = "중립신호";
		
		m_ctrlTitle[nCol].Create(strTitle, WS_CHILD | WS_VISIBLE | SS_CENTER, 
			CRect(0, 0, 0, 0), this);
		m_ctrlTitle[nCol].SetFont(&m_fontDefault);
		
		for (nRow = 0; nRow < IDC_ROW_CNT; nRow++)
		{
			m_comboSound[nCol][nRow].Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
				CRect(0, 0, 0, 0), this, IDC_COMBO_SOUND + nIDC_Index);
			m_comboSound[nCol][nRow].SetFont(&m_fontDefault);
			
			m_btnPlay[nCol][nRow].Create(_T(">"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
				CRect(0, 0, 0, 0), this, IDC_BTN_PLAY + nIDC_Index);
			m_btnPlay[nCol][nRow].SetFont(&m_fontDefault);
			
			m_btnFind[nCol][nRow].Create(_T("F"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
				CRect(0, 0, 0, 0), this, IDC_BTN_FIND + nIDC_Index);
			m_btnFind[nCol][nRow].SetFont(&m_fontDefault);
			
			nIDC_Index++;
		}
	}

	AfxSetResourceHandle(hInstSave);
	
	return 0;
}

void CSoundSetWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	int nWidthCol = 130;
	int nHeightCol = 20;
	int nHeight_Offset = 10;
	
	
	CRect reClient, reLeft, reTemp, reTemp1, reTemp2;
	GetClientRect(reClient);
	
	reLeft = reClient;
	
	reLeft.left += nHeight_Offset;
	reLeft.right = reLeft.left + 100;
	
	reTemp = reLeft;
	reTemp.top += nHeightCol + nHeight_Offset * 2;
	reTemp.bottom = reTemp.top + nHeightCol;
	m_btnNewsChk.MoveWindow(&reTemp);
	
	reTemp.top = reTemp.bottom + nHeight_Offset;
	reTemp.bottom = reTemp.top + nHeightCol;
	m_btnConditionChk.MoveWindow(&reTemp);
	
	reTemp.top = reTemp.bottom + nHeight_Offset;
	reTemp.bottom = reTemp.top + nHeightCol;
	m_btnFindChk.MoveWindow(&reTemp);
	
	
	int nCol, nRow;
	int nIDC_Index = 0;
	for (nCol = 0; nCol < IDC_COL_CNT; nCol++)
	{
		reLeft.left = reLeft.right + nHeight_Offset;
		reLeft.right = reLeft.left + nWidthCol;
		reTemp = reLeft;
		
		reTemp.top += nHeight_Offset;
		reTemp.bottom = reTemp.top + nHeightCol;
		m_ctrlTitle[nCol].MoveWindow(&reTemp);
		
		reTemp1 = reTemp;
		for (nRow = 0; nRow < IDC_ROW_CNT; nRow++)
		{
			reTemp1.top = reTemp1.bottom + nHeight_Offset;
			reTemp1.bottom = reTemp1.top + nHeightCol;
			
			reTemp1.right = reTemp.right;
			reTemp1.left = reTemp1.right - 20;
			m_btnFind[nCol][nRow].MoveWindow(&reTemp1);
			
			reTemp1.right = reTemp1.left - 1;
			reTemp1.left = reTemp1.right - 20;
			m_btnPlay[nCol][nRow].MoveWindow(&reTemp1);
			
			reTemp1.right = reTemp1.left;
			reTemp1.left = reTemp.left;
			reTemp1.bottom += 100;
			m_comboSound[nCol][nRow].MoveWindow(&reTemp1);
			
			reTemp1.bottom -= 100;
		}
	}
}


void CSoundSetWnd::OnBtnNewsChk()
{
	SetEnable_Row(0, m_btnNewsChk.GetCheck());
}

void CSoundSetWnd::OnBtnConditionChk()
{
	SetEnable_Row(1, m_btnConditionChk.GetCheck());
}

void CSoundSetWnd::OnBtnFindChk()
{
	SetEnable_Row(2, m_btnFindChk.GetCheck());
}

void CSoundSetWnd::SetEnable_Row(int nRow, BOOL bState)
{
	for (int nCol = 0; nCol < IDC_COL_CNT; nCol++)
	{
		m_comboSound[nCol][nRow].EnableWindow(bState);
		m_btnPlay[nCol][nRow].EnableWindow(bState);
		m_btnFind[nCol][nRow].EnableWindow(bState);
	}
}

HBRUSH CSoundSetWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushStatic;
	}
	
	return hbr;
}

void CSoundSetWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect reClient;
	GetClientRect(reClient);
	dc.FillSolidRect(reClient, COLOR_SETWND_FACE);
}
