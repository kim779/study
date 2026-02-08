// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Depth.h"
#include "ConfigDlg.h"
#include "depth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(struct _config *pConfig, CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	//}}AFX_DATA_INIT

	m_pConfig = pConfig;
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, IDC_CHKBOLD3, m_chkBOLD3);
	DDX_Control(pDX, IDC_CHKBOLD2, m_chkBOLD2);
	DDX_Control(pDX, IDC_CHKSHL, m_chkSHL);
	DDX_Control(pDX, IDC_CHKDBONG, m_chkDBONG);
	DDX_Control(pDX, IDC_CHKSGRP, m_chkSGRP);
	DDX_Control(pDX, IDC_CHKDYUL, m_chkDYUL);
	DDX_Control(pDX, IDC_CHKCURR, m_chkCURR);
	DDX_Control(pDX, IDC_CHKBOLD, m_chkBOLD);
	DDX_Control(pDX, IDC_CHKMIDPRC, m_chkMIDPRC);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_BN_CLICKED(IDC_CHKDYUL, OnChkdyul)
	ON_BN_CLICKED(IDC_CHKCURR, OnChkcurr)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDDEFAULT, &CConfigDlg::OnBnClickedDefault)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_LR_NO, &CConfigDlg::OnBnClickedLrNo)
	ON_BN_CLICKED(IDC_LR_GRAPH, &CConfigDlg::OnBnClickedLrGraph)
	ON_BN_CLICKED(IDC_LR_CHE, &CConfigDlg::OnBnClickedLrChe)
	ON_BN_CLICKED(IDC_RR_NO, &CConfigDlg::OnBnClickedRrNo)
	ON_BN_CLICKED(IDC_RR_SGJJ, &CConfigDlg::OnBnClickedRrSgjj)
	ON_BN_CLICKED(IDC_RR_PIVOT, &CConfigDlg::OnBnClickedRrPivot)
	ON_BN_CLICKED(IDC_RR_CURR, &CConfigDlg::OnBnClickedRrCurr)
	ON_BN_CLICKED(IDC_RR_JVI, &CConfigDlg::OnBnClickedRrJvi)
	ON_BN_CLICKED(IDC_RR_MID, &CConfigDlg::OnBnClickedRrMid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

void CConfigDlg::OnChkdyul() 
{
//	if (m_chkDYUL.GetCheck())	m_chkBOLD.EnableWindow(FALSE);	
//	else				m_chkBOLD.EnableWindow(TRUE);	
}

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CPoint	pt;
	GetCursorPos(&pt);
	CRect	rc;
	GetWindowRect(&rc);
	MoveWindow(pt.x, pt.y, rc.Width(), rc.Height());

	m_pOldconfig = new struct _config;
	memset(m_pOldconfig, ' ', sizeof(struct _config));
	memcpy(m_pOldconfig, m_pConfig, sizeof(struct _config));
	
	if (m_pConfig->curr)	m_chkCURR.SetCheck(1);
	if (m_pConfig->sgrp)	m_chkSGRP.SetCheck(1);
	if (m_pConfig->hdyul)	m_chkDYUL.SetCheck(1);
	if (m_pConfig->hbold)	m_chkBOLD.SetCheck(1);
	if (m_pConfig->sbong)	m_chkDBONG.SetCheck(1);
	if (m_pConfig->shl)		m_chkSHL.SetCheck(1);
	if (m_pConfig->hrbold)	m_chkBOLD2.SetCheck(1);
	if (m_pConfig->hcbold)	m_chkBOLD3.SetCheck(1);
	if (m_pConfig->bshowMidPrc)	m_chkMIDPRC.SetCheck(1);

	if(m_iVersion < DF_DEPTHVS)
		m_chkMIDPRC.SetCheck(1);

	if (!m_bEnableMid)
	{
		m_chkMIDPRC.SetCheck(0);
		m_chkMIDPRC.EnableWindow(FALSE);
	}
//	if (m_chkDYUL.GetCheck())	m_chkBOLD.EnableWindow(FALSE);
	
	CString strBmpPath;
	strBmpPath.Format("%s\\image\\%s", m_sRoot, "호가설정판좌측하단.bmp");
	HBITMAP hBmp = (HBITMAP)::LoadImage(
		NULL,
		strBmpPath,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	if (hBmp)
		m_bitmapLeft.Attach(hBmp);        // CBitmap에 Attach

	strBmpPath.Format("%s\\image\\%s", m_sRoot, "호가설정판우측상단.bmp");
	HBITMAP hBmpR = (HBITMAP)::LoadImage(
		NULL,
		strBmpPath,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	if (hBmpR)
		m_bitmapRight.Attach(hBmpR);        // CBitmap에 Attach


	CFont* pFont = GetDlgItem(IDC_STATIC_HOGA)->GetFont();

	LOGFONT lf;
	pFont->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	m_boldFont.CreateFontIndirect(&lf);

	GetDlgItem(IDC_STATIC_HOGA)->SetFont(&m_boldFont);
	GetDlgItem(IDC_STATIC_ADD)->SetFont(&m_boldFont);
	GetDlgItem(IDC_STATIC_ADD2)->SetFont(&m_boldFont);


	m_radios[0].SubclassDlgItem(IDC_LR_NO, this);          //표시 안함
	m_radios[0].ModifyStyle(0, WS_GROUP);
	m_radios[1].SubclassDlgItem(IDC_LR_GRAPH, this);    //그래프
	m_radios[2].SubclassDlgItem(IDC_LR_CHE, this);         //체결

	m_radios[3].SubclassDlgItem(IDC_RR_NO, this);          //표시 안함
	m_radios[3].ModifyStyle(0, WS_GROUP);
	m_radios[4].SubclassDlgItem(IDC_RR_SGJJ, this);         //시고저종
	m_radios[5].SubclassDlgItem(IDC_RR_PIVOT, this);      //PIVOT
	m_radios[6].SubclassDlgItem(IDC_RR_CURR, this);       //현재가
	m_radios[7].SubclassDlgItem(IDC_RR_JVI, this);           //정적VI
	m_radios[8].SubclassDlgItem(IDC_RR_MID, this);         //중간가


	switch (m_pConfig->mbong)
	{
	case 0:
		m_radios[0].SetCheck(true);
		break;
	case 1:
		m_radios[1].SetCheck(true);
		break;
	case 2:
		m_radios[2].SetCheck(true);
		break;
	}

	switch (m_pConfig->info)
	{
	case 0:
		m_radios[3].SetCheck(true);
		break;
	case 1:
		m_radios[4].SetCheck(true);
		break;
	case 2:
		m_radios[5].SetCheck(true);
		break;
	case 3:
		m_radios[6].SetCheck(true);
		break;
	case 4:
		m_radios[7].SetCheck(true);
		break;
	case 5:
		m_radios[8].SetCheck(true);
		break;
	}

	m_radios[8].ShowWindow(SW_HIDE);
	return TRUE;  
}

void CConfigDlg::OnOK() 
{
	if (m_chkCURR.GetCheck())	m_pConfig->curr = 1;	//현재가
	else				m_pConfig->curr = 0;
	if (m_chkSGRP.GetCheck())	m_pConfig->sgrp = 1;	//호가잔량 그래프
	else				m_pConfig->sgrp = 0;
	if (m_chkDYUL.GetCheck())	m_pConfig->hdyul = 1;	//호가별 등락률
	else				m_pConfig->hdyul = 0;
	if (m_chkBOLD.GetCheck())	m_pConfig->hbold = 1;	//호가 굵게
	else				m_pConfig->hbold = 0;
	if (m_chkDBONG.GetCheck())	m_pConfig->sbong = 1;	//당일 봉차트
	else				m_pConfig->sbong = 0;
	if (m_chkSHL.GetCheck())	m_pConfig->shl = 1;		//시고저종 표시
	else				m_pConfig->shl = 0;
	if (m_chkBOLD2.GetCheck())	m_pConfig->hrbold = 1;	// 호가 잔량 굵게
	else				m_pConfig->hrbold = 0;
	if (m_chkBOLD3.GetCheck())	m_pConfig->hcbold = 1;	// 호가 건수 굵게
	else				m_pConfig->hcbold = 0;
	if (m_chkMIDPRC.GetCheck())	m_pConfig->bshowMidPrc = 1;	// 중간가 표시
	else				m_pConfig->bshowMidPrc = 0;
	CDialog::OnOK();
}

void CConfigDlg::OnChkcurr() 
{
	// TODO: Add your control notification handler code here
	
}

void CConfigDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	memcpy(m_pConfig, m_pOldconfig, sizeof(struct _config));
	CDialog::OnCancel();
}

void CConfigDlg::OnBnClickedDefault()
{
	m_chkCURR.SetCheck(TRUE);  //현재가 추적
	m_chkDYUL.SetCheck(TRUE);   //등락률
	m_chkBOLD.SetCheck(TRUE);   //호가 굵게
	m_chkBOLD2.SetCheck(TRUE);  // 호가 잔량 굵게
	m_chkBOLD3.SetCheck(TRUE);  // 호가 대비? 건수 굵게
	m_chkDBONG.SetCheck(TRUE);  //당일 봉차트
	m_chkSGRP.SetCheck(TRUE);  //호가잔량 그래프
	m_chkSHL.SetCheck(TRUE);    //시고저종 표시
	m_chkMIDPRC.SetCheck(TRUE);  //중간가

	m_radios[0].SetCheck(false);
	m_radios[1].SetCheck(true);
	m_pConfig->mbong = 1;
	m_radios[2].SetCheck(false);

	m_radios[3].SetCheck(false);
	m_radios[4].SetCheck(true);
	m_pConfig->info = 1;
	m_radios[5].SetCheck(false);
	m_radios[6].SetCheck(false);
	m_radios[7].SetCheck(false);
	m_radios[8].SetCheck(false);
}


void CConfigDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	  // 비트맵 DC 생성
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	// 비트맵 선택
	CBitmap* pOldBitmap = memDC.SelectObject(&m_bitmapLeft);
	BITMAP bmp;
	m_bitmapLeft.GetBitmap(&bmp);
	CRect rcStatic;
	GetDlgItem(IDC_STATIC_LEFT)->GetWindowRect(&rcStatic);
	ScreenToClient(&rcStatic);
	int x = rcStatic.left - bmp.bmWidth - 5; // Static 왼쪽 10px
	int y = rcStatic.top - 10;
	dc.BitBlt(x, y, bmp.bmWidth, bmp.bmHeight, &memDC, 0, 0, SRCCOPY);


	pOldBitmap = memDC.SelectObject(&m_bitmapRight);
	BITMAP bmpR;
	m_bitmapRight.GetBitmap(&bmpR);
	GetDlgItem(IDC_STATIC_RIGHT)->GetWindowRect(&rcStatic);
	ScreenToClient(&rcStatic);
	x = rcStatic.left - bmpR.bmWidth - 5; // Static 왼쪽 10px
	y = rcStatic.top - 10;
	dc.BitBlt(x, y, bmpR.bmWidth, bmpR.bmHeight, &memDC, 0, 0, SRCCOPY);


	memDC.SelectObject(pOldBitmap);

}

void CConfigDlg::OnBnClickedLrNo()
{
	// 좌측 표시안함
	m_pConfig->mbong = 0;
}
void CConfigDlg::OnBnClickedLrGraph()
{
	// 좌측 그래프
	m_pConfig->mbong = 1;
}
void CConfigDlg::OnBnClickedLrChe()
{
	// 좌측 체결
	m_pConfig->mbong = 2;
}




void CConfigDlg::OnBnClickedRrNo()
{
	// 우측 표시안함
	m_pConfig->info = 0;
}
void CConfigDlg::OnBnClickedRrSgjj()
{
	// 우측 시고저종
	m_pConfig->info = 1;
}
void CConfigDlg::OnBnClickedRrPivot()
{
	// 우측 PIVOT
	m_pConfig->info = 2;
}
void CConfigDlg::OnBnClickedRrCurr()
{
	// 우측 현재가
	m_pConfig->info = 3;
}
void CConfigDlg::OnBnClickedRrJvi()
{
	// 우측 정적VI
	m_pConfig->info = 4;
}
void CConfigDlg::OnBnClickedRrMid()
{
	// 우측 중간가
	m_pConfig->info = 5;
}