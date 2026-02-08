// ConfirmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ib100600.h"
#include "ConfirmDlg.h"
#include "MapWnd.h"
#include "Define.h"
#include "../../control/fx_misc/misctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfirmDlg dialog


CConfirmDlg::CConfirmDlg(CWnd* pParent /*=NULL*/, CMapWnd* pMapWnd, int nType)
	: CDialog(CConfirmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfirmDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMapWnd = pMapWnd;
	m_pOk = nullptr;
	m_pCancel = nullptr;
	m_pConfirmGrid = nullptr;
	m_pArrOrder = nullptr;
	m_nOrderType = nType;
}

CConfirmDlg::~CConfirmDlg()
{
	//_SAFEDESTROY(m_pOk);
	//_SAFEDESTROY(m_pCancel);
	//_SAFEDESTROY(m_pConfirmGrid);
}


void CConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfirmDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfirmDlg, CDialog)
	//{{AFX_MSG_MAP(CConfirmDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfirmDlg message handlers

BOOL CConfirmDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_pOk = std::make_unique<CfxImgButton>();
	m_pCancel = std::make_unique<CfxImgButton>();

	CFont* pFont = m_pMapWnd->GetNormalFont();
	CRect rcClient;
	CRect rcCtrl;
	GetClientRect(rcClient);

	const int igap = 6;
	m_pConfirmGrid = std::make_unique<CConfirmGrid>(m_pMapWnd);
	rcCtrl.SetRect(SZ_FORM_MARGIN, SZ_FORM_MARGIN, rcClient.right - SZ_FORM_MARGIN, rcClient.bottom - SZ_FORM_MARGIN - SZ_CTRL_HEIGHT * 2 - SZ_CTRL_VGAP * 2);
	rcCtrl.SetRect(igap, igap, rcClient.right - igap, rcClient.bottom - igap - SZ_CTRL_HEIGHT * 2 - SZ_CTRL_VGAP * 2);

	m_pConfirmGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_CONFIRMGRID);

	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;
	m_pMsg = std::make_unique<CfxStatic>();
	m_pMsg->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_MSG);

	m_pMsg->SetBkColor(m_pMapWnd->GetIndexColor(CLR_PANNEL_BACK));
	m_pMsg->SetFgColor(m_pMapWnd->GetIndexColor(CLR_STATIC_FORE));
	m_pMsg->SetFont(pFont);

	CString strPath = "";
	strPath.Format("%s\\Image\\", m_pMapWnd->GetAxRoot());

	HBITMAP hbmp	= m_pMapWnd->GetAxBitmap(strPath + "4btn.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn	= m_pMapWnd->GetAxBitmap(strPath + "4btn_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv = m_pMapWnd->GetAxBitmap(strPath + "4btn_en.bmp")->operator HBITMAP();

	rcCtrl.SetRect(rcClient.CenterPoint().x - SZ_CTRL_4BUTTON - 1, rcClient.bottom - igap - SZ_CTRL_HEIGHT,
					rcClient.CenterPoint().x - 1, rcClient.bottom - igap);
	m_pOk->Create(_T("주문"), rcCtrl, this, IDOK);
	m_pOk->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pOk->SetFont(pFont);

	rcCtrl.SetRect(rcClient.CenterPoint().x + 1, rcClient.bottom - igap - SZ_CTRL_HEIGHT,
					rcClient.CenterPoint().x + 1 + SZ_CTRL_4BUTTON, rcClient.bottom - igap);

	m_pCancel->Create(_T("취소"), rcCtrl, this, IDCANCEL);
	m_pCancel->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pCancel->SetFont(pFont);

	FillOrderData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfirmDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (!m_pMapWnd)
		return;
	
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, m_pMapWnd->GetIndexColor(CLR_FORM_BACK));
	rcClient.DeflateRect(3,3,3,3); 

	dc.FillSolidRect(&rcClient, m_pMapWnd->GetIndexColor(CLR_CONTENTS_BACK));
	m_pMapWnd->DrawCornerRoundBitmap(&dc, rcClient, 0);
 

//	dc.FillSolidRect(&rcClient, m_pMapWnd->GetIndexColor(CLR_PANNEL_BACK));
//	m_pMapWnd->dra
}

void CConfirmDlg::SetOrderData(CStringArray *pArr)
{
	m_pArrOrder = pArr;
}

void CConfirmDlg::FillOrderData()
{
	if (!m_pArrOrder)
		return;

	CString strData(_T(""));
	CString strVal(_T(""));
	const int nCount = m_pArrOrder->GetSize();

	for (int i = 0; i < nCount; i++)
	{
		strData = m_pArrOrder->GetAt(i);

		if (strData.IsEmpty())
			continue;

		strVal = Parser(strData, _T("\t"));
		strVal.Insert(3, _T("-"));
		strVal.Insert(6, _T("-"));

		m_pConfirmGrid->QuickSetText(0, i, (LPCTSTR)strVal);
		strVal = Parser(strData, _T("\t"));
		m_pConfirmGrid->QuickSetText(1, i, (LPCTSTR)strVal);
		
		Parser(strData, _T("\t"));				// 종목코드

		if (m_nOrderType == 3 || m_nOrderType == 4)
			Parser(strData, _T("\t"));				// 원주문번호

		strVal = Parser(strData, _T("\t"));
		m_pConfirmGrid->QuickSetText(2, i, (LPCTSTR)strVal);
		
		strVal = Parser(strData, _T("\t"));
		m_pConfirmGrid->QuickSetText(3, i, (LPCTSTR)strVal);

		if (m_nOrderType == 1 || m_nOrderType == 2)
		{
			Parser(strData, _T("\t"));
			Parser(strData, _T("\t"));
		}

		strVal = Parser(strData, _T("\t"));

		if (m_nOrderType == 1 || m_nOrderType == 2)
			m_pConfirmGrid->QuickSetText(4, i, (LPCTSTR)strVal);
//		else
			

		strVal = Parser(strData, _T("\t"));
		
//		if (m_nOrderType == 3 || m_nOrderType == 4)
//			m_pConfirmGrid->QuickSetText(6, i, (LPCTSTR)strVal);

		strVal = Parser(strData, _T("\t"));

//		if (m_nOrderType == 1 || m_nOrderType == 2)
//		{
			m_pConfirmGrid->QuickSetText(5, i, (LPCTSTR)strVal);
			strVal = Parser(strData, _T("\t"));
			m_pConfirmGrid->QuickSetText(6, i, (LPCTSTR)strVal);
//		}
//		else
//			m_pConfirmGrid->QuickSetText(5, i, (LPCTSTR)strVal);
	}

	m_pConfirmGrid->RedrawAll();

	CString strMsg(_T(""));
	CString strType(_T(""));

	switch (m_nOrderType)
	{
	case 1:
	case 2:
		strType = _T("주문");
		break;
	case 3:
		strType = _T("정정");
		break;
	case 4:
		strType = _T("취소");
		break;
	}

	strMsg.Format("주식복수종목 %s 총 %d건을 처리하시겠습니까?", (LPCTSTR)strType, nCount);
	m_pMsg->SetAlignment(alLEFT);
	m_pMsg->SetText(strMsg);

	strMsg.Format("[1006] 주문::주식복수종목 %s 확인창", (LPCTSTR)strType);
	SetWindowText(strMsg);
}

CString CConfirmDlg::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}
