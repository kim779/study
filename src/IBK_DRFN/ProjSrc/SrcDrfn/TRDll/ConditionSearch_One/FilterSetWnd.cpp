// FilterSetWnd.cpp : implementation file
//

#include "stdafx.h"
#include "FilterSetWnd.h"

#include "MainFrmWnd.h"
#include "SubFilterDlg.h"
#include "FilterSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterSetWnd

CFilterSetWnd::CFilterSetWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	m_brushStatic.CreateSolidBrush(COLOR_DLG_CANVAS);

	m_stFilterInfo.plistDealTimeVol = new CList <SubFilterInfo, SubFilterInfo&>;
	m_stFilterInfo.plistDealTimePrice = new CList <SubFilterInfo, SubFilterInfo&>;

	m_listDealTimeVol.RemoveAll();
	m_listDealTimePrice.RemoveAll();
}

CFilterSetWnd::~CFilterSetWnd()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();

	m_brushStatic.DeleteObject();
}

BEGIN_MESSAGE_MAP(CFilterSetWnd, CWnd)
	//{{AFX_MSG_MAP(CFilterSetWnd)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP

	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_MARKETRADIO, IDC_BTN_MARKETRADIO + _countof(TITLE_MARKET), OnBtnMarketRadio)

	ON_BN_CLICKED(IDC_BTN_NOWPRICECHK, OnBtnNowPriceChk)
	ON_BN_CLICKED(IDC_BTN_CHANGERATECHK, OnBtnChangeRateChk)

	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_CHANGEVALCHK, IDC_BTN_CHANGEVALCHK + _countof(TITLE_CHANGEVAL), OnBtnChangeValChk)

	ON_BN_CLICKED(IDC_BTN_DEALVOLRADIO, OnBtnDealVolRadio)
	ON_BN_CLICKED(IDC_BTN_DEALVOLTIMERADIO, OnBtnDealVolTimeRadio)
	ON_BN_CLICKED(IDC_BTN_DEALPRICERADIO, OnBtnDealPriceRadio)
	ON_BN_CLICKED(IDC_BTN_DEALPRICETIMERADIO, OnBtnDealPriceTimeRadio)

	ON_BN_CLICKED(IDC_BTN_SETDEALVOLTIME, OnBtnSetDealVolTime)
	ON_BN_CLICKED(IDC_BTN_SETDEALPRICETIME, OnBtnSetDealPriceTime)

	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_DIRECTIONCHK, IDC_BTN_DIRECTIONCHK + _countof(TITLE_DIRECTION), OnBtnDirectionChk)

	ON_BN_CLICKED(IDC_BTN_CODEADD, OnBtnCodeAdd)
	ON_BN_CLICKED(IDC_BTN_CODEDELETE, OnBtnCodeDelete)
	ON_BN_CLICKED(IDC_BTN_CODEDELETEALL, OnBtnCodeDeleteAll)


END_MESSAGE_MAP()

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CFilterSetWnd message handlers

BOOL CFilterSetWnd::Create(CWnd* pParentWnd, UINT nID, CWnd* pMainWnd)
{
	m_bCreateState = FALSE;

	m_pMainWnd = pMainWnd;
	
	BOOL bReault;
	bReault = CWnd::Create(_T("STATIC"), _T("Filter Set Wnd"), WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), pParentWnd, nID);

	m_bCreateState = TRUE;

	CRect reClient;
	GetClientRect(reClient);
	OnSize(reClient);

	return bReault;
}

int CFilterSetWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	int nIndex;

	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pMainWnd;
	//m_ctrlCodeInput.InitCodeInput((CDialog*)this, CRect(0, 0, 100, 0), IDC_CODEINPUT, CODEINPUT_PROP_STOCK);
	m_ctrlCodeInput.Create(_T("Code Input"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeInput.SetFont(&m_fontDefault);

	m_ctrlTitle_In.Create(_T("대상종목"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTitle_In.SetFont(&m_fontBold, 4);

	m_btnBaseGroup.Create(_T("시장구분"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPIN);
	m_btnBaseGroup.SetFont(&m_fontDefault, 4);

	for (nIndex = 0; nIndex < _countof(TITLE_MARKET); nIndex++)
	{
		m_btnMarketRao[nIndex].Create(TITLE_MARKET[nIndex], WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
			CRect(0, 0, 0, 0), this, IDC_BTN_MARKETRADIO + nIndex);
		m_btnMarketRao[nIndex].SetFont(&m_fontDefault);
	}

	m_btnPriceGroup.Create(_T("가격범위"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPPRICE);
	m_btnPriceGroup.SetFont(&m_fontDefault);

	m_btnNowPriceChk.Create(_T("현재가(원)"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_NOWPRICECHK);
	m_btnNowPriceChk.SetFont(&m_fontDefault);

	m_editNowPriceUp.Create(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_EDIT_NOWPRICEUP);
	m_editNowPriceUp.SetFont(&m_fontDefault);
	m_editNowPriceUp.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	//m_editNowPriceUp.UseNumericType(TRUE);

	m_ctrlNowPrice1.Create(_T("이상 ~"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlNowPrice1.SetFont(&m_fontDefault);

	m_editNowPriceDown.Create(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_EDIT_NOWPRICEDOWN);
	m_editNowPriceDown.SetFont(&m_fontDefault);
	m_editNowPriceDown.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	//m_editNowPriceDown.UseNumericType(TRUE);

	m_ctrlNowPrice2.Create(_T("이하"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlNowPrice2.SetFont(&m_fontDefault);

	m_btnChangRateChk.Create(_T("등락률(%)"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_CHANGERATECHK);
	m_btnChangRateChk.SetFont(&m_fontDefault);

	m_editChangRateUp.Create(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_EDIT_CHANGERATEUP);
	m_editChangRateUp.SetFont(&m_fontDefault);
	m_editChangRateUp.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	//m_editChangRateUp.UseNumericType(TRUE);

	m_ctrlChangRate1.Create(_T("이상 ~"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlChangRate1.SetFont(&m_fontDefault);

	m_editChangRateDown.Create(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_EDIT_CHANGERATEDOWN);
	m_editChangRateDown.SetFont(&m_fontDefault);
	m_editChangRateDown.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	//m_editChangRateDown.UseNumericType(TRUE);

	m_ctrlChangRate2.Create(_T("이하"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlChangRate2.SetFont(&m_fontDefault);

	m_btnChangGroup.Create(_T("등락 구분"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPCHANGE);
	m_btnChangGroup.SetFont(&m_fontDefault);

	for (nIndex = 0; nIndex < _countof(TITLE_CHANGEVAL); nIndex++)
	{
		m_btnChangValChk[nIndex].Create(TITLE_CHANGEVAL[nIndex], WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
			CRect(0, 0, 0, 0), this, IDC_BTN_CHANGEVALCHK + nIndex);
		m_btnChangValChk[nIndex].SetFont(&m_fontDefault);
	}

	m_btnDealVolGroup.Create(_T("거래량 범위"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPDEALVOL);
	m_btnDealVolGroup.SetFont(&m_fontDefault);

	m_btnDealVolRao.Create(_T("거래량(고정)"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DEALVOLRADIO);
	m_btnDealVolRao.SetFont(&m_fontDefault);

	m_editDealVol.Create(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_EDIT_DEALVOL);
	m_editDealVol.SetFont(&m_fontDefault);
	m_editDealVol.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	//m_editDealVol.UseNumericType(TRUE);

	m_ctrlDealVol.Create(_T("천주 이상"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlDealVol.SetFont(&m_fontDefault);

	m_btnDealVolTimeRao.Create(_T("시간대별 거래량"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DEALVOLTIMERADIO);
	m_btnDealVolTimeRao.SetFont(&m_fontDefault);

	m_btnSetDealVolTime.Create(_T("설정/보기"), CRect(0, 0, 0, 0), this, IDC_BTN_SETDEALVOLTIME);
	pMainWnd->SetBtnImg(&m_btnSetDealVolTime, 6);

	m_btnDealPriceGroup.Create(_T("거래대금 범위"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPDEALPRICE);
	m_btnDealPriceGroup.SetFont(&m_fontDefault);

	m_btnDealPriceRao.Create(_T("거래대금"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DEALPRICERADIO);
	m_btnDealPriceRao.SetFont(&m_fontDefault);

	m_editDealPrice.Create(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_EDIT_DEALPRICE);
	m_editDealPrice.SetFont(&m_fontDefault);
	m_editDealPrice.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	//m_editDealPrice.UseNumericType(TRUE);

	m_ctrlDealPrice.Create(_T("백만원 이상"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlDealPrice.SetFont(&m_fontDefault);

	m_btnDealPriceTimeRao.Create(_T("시간대별 거래대금"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DEALPRICETIMERADIO);
	m_btnDealPriceTimeRao.SetFont(&m_fontDefault);

	m_btnSetDealPriceTime.Create(_T("설정/보기"), CRect(0, 0, 0, 0), this, IDC_BTN_SETDEALPRICETIME);
	pMainWnd->SetBtnImg(&m_btnSetDealPriceTime, 6);

	m_btnConcludeGroup.Create(_T("순간 체결량 범위"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_CONCLUDE_GROUP);
	m_btnConcludeGroup.SetFont(&m_fontDefault);

	m_btnDealVolChk_Conclude.Create(_T("거래량"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_CONCLUDE_DEALVOLCHK);
	m_btnDealVolChk_Conclude.SetFont(&m_fontDefault);

	m_editDealVol_Conclude.Create(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_EDIT_CONCLUDE_DEALVOL);
	m_editDealVol_Conclude.SetFont(&m_fontDefault);
	m_editDealVol_Conclude.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	//m_editDealVol_Conclude.UseNumericType(TRUE);

	m_ctrlDealVol_Conclude.Create(_T("백주 이상"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlDealVol_Conclude.SetFont(&m_fontDefault);

	m_btnDealPriceChk_Conclude.Create(_T("거래대금"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_CONCLUDE_DEALPRICECHK);
	m_btnDealPriceChk_Conclude.SetFont(&m_fontDefault);

	m_editDealPrice_Conclude.Create(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_EDIT_CONCLUDE_DEALPRICE);
	m_editDealPrice_Conclude.SetFont(&m_fontDefault);
	m_editDealPrice_Conclude.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	//m_editDealPrice_Conclude.UseNumericType(TRUE);

	m_ctrlDealPrice_Conclude.Create(_T("만원 이상"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlDealPrice_Conclude.SetFont(&m_fontDefault);


	m_ctrlTitle_Out.Create(_T("제외종목"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTitle_Out.SetFont(&m_fontBold);

	m_btntDirectionGroup.Create(_T("시장조치/우선주"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPDIRECTION);
	m_btntDirectionGroup.SetFont(&m_fontDefault);

	for (nIndex = 0; nIndex < _countof(TITLE_DIRECTION); nIndex++)
	{
		m_btntDirectionChk[nIndex].Create(TITLE_DIRECTION[nIndex], WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
			CRect(0, 0, 0, 0), this, IDC_BTN_DIRECTIONCHK + nIndex);
		m_btntDirectionChk[nIndex].SetFont(&m_fontDefault);
	}

	m_btnOutCodeGroup.Create(_T("제외종목"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPOUTCODE);
	m_btnOutCodeGroup.SetFont(&m_fontDefault);

	m_ctrlCodeName.Create(_T("종목명"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeName.SetFont(&m_fontDefault);

	m_btnCodeAdd.Create(_T("추가"), CRect(0, 0, 0, 0), this, IDC_BTN_CODEADD);
	pMainWnd->SetBtnImg(&m_btnCodeAdd, 2);

	m_btnCodeDelete.Create(_T("삭제"), CRect(0, 0, 0, 0), this, IDC_BTN_CODEDELETE);
	pMainWnd->SetBtnImg(&m_btnCodeAdd, 2);

	m_btnCodeDeleteAll.Create(_T("전체삭제"), CRect(0, 0, 0, 0), this, IDC_BTN_CODEDELETEALL);
	pMainWnd->SetBtnImg(&m_btnCodeDeleteAll, 4);

	m_gridOutCode.CreateGrid(WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_GRID_OUTCODE);

	AfxSetResourceHandle(hInstSave);

	return 0;
}

void CFilterSetWnd::SetInit()
{
	m_bLoad_Filter = SetInfo_Filter(TRUE);
	if (m_bLoad_Filter == TRUE)
		SetInfo();
}

void CFilterSetWnd::SetInit_Base()
{
	m_bLoad_Filter = SetInfo_Filter(TRUE, TRUE);
	if (m_bLoad_Filter == TRUE)
		SetInfo();
}

void CFilterSetWnd::SetInfo()
{
	int nIndex;
	CString strData;

	//# Set Data!
	m_btnMarketRao[m_stFilterInfo.nMarketType].SetCheck(TRUE);

	m_btnNowPriceChk.SetCheck(m_stFilterInfo.nNowPriceChk);

	strData.Format("%d", m_stFilterInfo.nNowPriceDown);
	m_editNowPriceDown.SetWindowText(strData);

	strData.Format("%d", m_stFilterInfo.nNowPriceUp);
	m_editNowPriceUp.SetWindowText(strData);

	OnBtnNowPriceChk();


	m_btnChangRateChk.SetCheck(m_stFilterInfo.nChangRateChk);

	strData.Format("%.2f", m_stFilterInfo.nChangRateDown / 100.0);
	m_editChangRateDown.SetWindowText(strData);

	strData.Format("%.2f", m_stFilterInfo.nChangRateUp / 100.0);
	m_editChangRateUp.SetWindowText(strData);

	OnBtnChangeRateChk();


	for (nIndex = 0; nIndex < _countof(TITLE_CHANGEVAL); nIndex++)
		m_btnChangValChk[nIndex].SetCheck(m_stFilterInfo.nChangValChk[nIndex]);

	POSITION pos;
	SubFilterInfo		stSubFilterInfo;
	if (m_stFilterInfo.nDealVolType == 0)
	{
		m_btnDealVolRao.SetCheck(TRUE);

		strData.Format("%d", m_stFilterInfo.nDealVol);
		m_editDealVol.SetWindowText(strData);

		OnBtnDealVolRadio();
	}
	else if (m_stFilterInfo.nDealVolType == 1)
	{
		m_editDealVol.SetWindowText("");

		m_btnDealVolTimeRao.SetCheck(TRUE);
		OnBtnDealVolTimeRadio();

		m_listDealTimeVol.RemoveAll();
		if (m_stFilterInfo.plistDealTimeVol->GetCount() > 0)
		{
			for (nIndex = 0; nIndex < m_stFilterInfo.plistDealTimeVol->GetCount(); nIndex++)
			{
				pos = m_stFilterInfo.plistDealTimeVol->FindIndex(nIndex);
				stSubFilterInfo = m_stFilterInfo.plistDealTimeVol->GetAt(pos);

				m_listDealTimeVol.AddTail(stSubFilterInfo);
			}
		}
	}

	if (m_stFilterInfo.nDealPriceType == 0)
	{
		m_btnDealPriceRao.SetCheck(TRUE);

		strData.Format("%d", m_stFilterInfo.nDealPrice);
		m_editDealPrice.SetWindowText(strData);
		OnBtnDealPriceRadio();
	}
	else if (m_stFilterInfo.nDealPriceType == 1)
	{
		m_editDealPrice.SetWindowText("");

		m_btnDealPriceTimeRao.SetCheck(TRUE);
		OnBtnDealPriceTimeRadio();

		m_listDealTimePrice.RemoveAll();
		if (m_stFilterInfo.plistDealTimePrice->GetCount() > 0)
		{
			for (nIndex = 0; nIndex < m_stFilterInfo.plistDealTimePrice->GetCount(); nIndex++)
			{
				pos = m_stFilterInfo.plistDealTimePrice->FindIndex(nIndex);
				stSubFilterInfo = m_stFilterInfo.plistDealTimePrice->GetAt(pos);

				m_listDealTimePrice.AddTail(stSubFilterInfo);
			}
		}
	}

	for (nIndex = 0; nIndex < _countof(TITLE_DIRECTION); nIndex++)
		m_btntDirectionChk[nIndex].SetCheck(m_stFilterInfo.nDirectionChk[nIndex]);

	m_saOutCode.RemoveAll();
	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pMainWnd;
	m_gridOutCode.HeaderSetup("CODE", pMainWnd->GetAxColor(74));
	for (nIndex = 0; nIndex < m_stFilterInfo.saOutCode.GetSize(); nIndex++)
	{
		m_gridOutCode.SetAddRow(m_stFilterInfo.saOutCode.GetAt(nIndex), 
			GetCodeToName(m_stFilterInfo.saOutCode.GetAt(nIndex)));
		m_saOutCode.Add(m_stFilterInfo.saOutCode.GetAt(nIndex));
	}
	m_gridOutCode.RedrawAll();
}

BOOL CFilterSetWnd::GetInfo()
{
	BOOL bResult = FALSE; 

	int nIndex;
	CString strData, strData2;

	CButton *pBtn;
	for (nIndex = 0; nIndex < _countof(TITLE_MARKET); nIndex++)
	{
		pBtn = (CButton*)GetDlgItem(IDC_BTN_MARKETRADIO + nIndex);
		if (pBtn->GetCheck() == TRUE)
			m_stFilterInfo.nMarketType = nIndex;
	}

	m_editNowPriceDown.GetWindowText(strData);
	m_editNowPriceUp.GetWindowText(strData2);
	if (m_btnNowPriceChk.GetCheck() == TRUE)
	{
		if (atoi(strData) <= atoi(strData2))
		{
			SetMessageBox("현재가 입력값 오류! \n 상한은 하한 보다 큰 값이어야 합니다.");
			m_editNowPriceDown.SetWindowText(strData2);
			m_editNowPriceDown.SetSel(strData2.GetLength());
			m_editNowPriceDown.SetFocus();

			return bResult;
		}

		m_stFilterInfo.nNowPriceChk = 1;
	}
	else
		m_stFilterInfo.nNowPriceChk = 0;
	m_stFilterInfo.nNowPriceDown = atoi(strData);
	m_stFilterInfo.nNowPriceUp = atoi(strData2);


	m_editChangRateDown.GetWindowText(strData);
	m_editChangRateUp.GetWindowText(strData2);
	if (m_btnChangRateChk.GetCheck() == TRUE)
	{
		if (atof(strData) > 100)
		{
			SetMessageBox("상한 등락율이 입력값 오류! \n 100보다 작은 값이어야 합니다.");
			m_editChangRateDown.SetSel(strData.GetLength());
			m_editChangRateDown.SetFocus();

			return bResult;
		}
		else if (atof(strData2) > 100)
		{
			SetMessageBox("하한 등락율이 입력값 오류! \n 100보다 작은 값이어야 합니다.");
			m_editChangRateUp.SetSel(strData2.GetLength());
			m_editChangRateUp.SetFocus();

			return bResult;
		}
		else if (atof(strData) <= atof(strData2))
		{
			SetMessageBox("등락률이 입력값 오류! \n 상한은 하한 보다 큰 값이어야 합니다.");
			m_editChangRateDown.SetWindowText(strData2);
			m_editChangRateDown.SetSel(strData2.GetLength());
			m_editChangRateDown.SetFocus();

			return bResult;
		}

		m_stFilterInfo.nChangRateChk = 1;
	}
	else
		m_stFilterInfo.nChangRateChk = 0;

	m_stFilterInfo.nChangRateDown = (int)(atof(strData) * 100);
	m_stFilterInfo.nChangRateUp = (int)(atof(strData2) * 100);

	for (nIndex = 0; nIndex < (int)CNT_CHANGEVAL; nIndex++)
	{
		pBtn = (CButton*)GetDlgItem(IDC_BTN_CHANGEVALCHK + nIndex);
		m_stFilterInfo.nChangValChk[nIndex] = pBtn->GetCheck();
	}

	POSITION pos;
	SubFilterInfo		stSubFilterInfo;
	if (m_btnDealVolRao.GetCheck() == TRUE)
	{
		m_stFilterInfo.nDealVolType = 0;

		m_editDealVol.GetWindowText(strData);
		m_stFilterInfo.nDealVol = atoi(strData);
	}
	else if (m_btnDealVolTimeRao.GetCheck() == TRUE)
	{
		m_stFilterInfo.nDealVolType = 1;
		m_stFilterInfo.nDealVol = 0;

		if (m_listDealTimeVol.GetCount() > 0)
		{
			m_stFilterInfo.plistDealTimeVol->RemoveAll();

			for (nIndex = 0; nIndex < m_listDealTimeVol.GetCount(); nIndex++)
			{
				pos = m_listDealTimeVol.FindIndex(nIndex);
				stSubFilterInfo = m_listDealTimeVol.GetAt(pos);

				m_stFilterInfo.plistDealTimeVol->AddTail(stSubFilterInfo);
			}
		}
	}

	if (m_btnDealPriceRao.GetCheck() == TRUE)
	{
		m_stFilterInfo.nDealPriceType = 0;

		m_editDealPrice.GetWindowText(strData);
		m_stFilterInfo.nDealPrice = atoi(strData);
	}
	else if (m_btnDealPriceTimeRao.GetCheck() == TRUE)
	{
		m_stFilterInfo.nDealPriceType = 1;
		m_stFilterInfo.nDealPrice = 0;

		if (m_listDealTimePrice.GetCount() > 0)
		{
			m_stFilterInfo.plistDealTimePrice->RemoveAll();

			for (nIndex = 0; nIndex < m_listDealTimePrice.GetCount(); nIndex++)
			{
				pos = m_listDealTimePrice.FindIndex(nIndex);
				stSubFilterInfo = m_listDealTimePrice.GetAt(pos);

				m_stFilterInfo.plistDealTimePrice->AddTail(stSubFilterInfo);
			}
		}
	}

	/*
	for (nIndex = 0; nIndex < _countof(TITLE_NEWS); nIndex++)
	{
	pBtn = (CButton*)GetDlgItem(IDC_BTN_NEWSRADIO + nIndex);
	if (pBtn->GetCheck() == TRUE)
	m_stFilterInfo.nNewsType = nIndex;
	}
	*/

	for (nIndex = 0; nIndex < _countof(TITLE_DIRECTION); nIndex++)
	{
		pBtn = (CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + nIndex);

		m_stFilterInfo.nDirectionChk[nIndex] = pBtn->GetCheck();
	}

	m_stFilterInfo.saOutCode.RemoveAll();
	CString strCode;
	for (nIndex = 0; nIndex < m_gridOutCode.GetNumberRows(); nIndex++)
	{
		m_gridOutCode.GetRowData(nIndex, strCode, strData);
		m_stFilterInfo.saOutCode.Add(strCode);
	}

	return TRUE;
}

void CFilterSetWnd::OnDestroy() 
{
	//	CWnd::OnDestroy();

	m_stFilterInfo.plistDealTimeVol->RemoveAll();
	delete m_stFilterInfo.plistDealTimeVol;
	m_stFilterInfo.plistDealTimeVol = NULL;

	m_stFilterInfo.plistDealTimePrice->RemoveAll();
	delete m_stFilterInfo.plistDealTimePrice;
	m_stFilterInfo.plistDealTimePrice = NULL;

	m_listDealTimeVol.RemoveAll();
	m_listDealTimePrice.RemoveAll();

	CWnd::OnDestroy();
}

void CFilterSetWnd::OnBtnMarketRadio(UINT nID)
{
	for (int nIndex = 0; nIndex < _countof(TITLE_MARKET); nIndex++)
	{
		CButton *pBtn = (CButton*)GetDlgItem(IDC_BTN_MARKETRADIO + nIndex);
		if ((int)nID == IDC_BTN_MARKETRADIO + nIndex)
			pBtn->SetCheck(TRUE);
		else
			pBtn->SetCheck(FALSE);
	}
}

void CFilterSetWnd::OnBtnNowPriceChk()
{
	BOOL bState = m_btnNowPriceChk.GetCheck();

	m_editNowPriceUp.EnableWindow(bState);
	m_editNowPriceDown.EnableWindow(bState);
	m_ctrlNowPrice1.EnableWindow(bState);
	m_ctrlNowPrice2.EnableWindow(bState);
}

void CFilterSetWnd::OnBtnChangeRateChk()
{
	BOOL bState = m_btnChangRateChk.GetCheck();

	m_editChangRateUp.EnableWindow(bState);
	m_editChangRateDown.EnableWindow(bState);
	m_ctrlChangRate1.EnableWindow(bState);
	m_ctrlChangRate2.EnableWindow(bState);
}

void CFilterSetWnd::OnBtnChangeValChk(UINT nID)
{
	/*
	for (int nIndex = 0; nIndex < _countof(TITLE_CHANGEVAL); nIndex++)
	{
	if ((int)nID == (int)IDC_BTN_CHANGEVALCHK + nIndex)
	{
	CButton *pBtn = (CButton*)GetDlgItem(nID);

	BOOL bState = pBtn->GetCheck();
	m_stFilterInfo.nChangValChk[nIndex] = bState;

	break;
	}
	}
	*/
}

void CFilterSetWnd::OnBtnDealVolRadio()
{
	BOOL bState = TRUE;
	m_btnDealVolRao.SetCheck(bState);

	m_editDealVol.EnableWindow(bState);
	m_ctrlDealVol.EnableWindow(bState);

	bState = FALSE;
	m_btnDealVolTimeRao.SetCheck(bState);
	m_btnSetDealVolTime.EnableWindow(bState);
}

void CFilterSetWnd::OnBtnDealVolTimeRadio()
{
	BOOL bState = FALSE;
	m_btnDealVolRao.SetCheck(bState);

	m_editDealVol.EnableWindow(bState);
	m_ctrlDealVol.EnableWindow(bState);

	bState = TRUE;
	m_btnDealVolTimeRao.SetCheck(bState);
	m_btnSetDealVolTime.EnableWindow(bState);
}

void CFilterSetWnd::OnBtnDealPriceRadio()
{
	BOOL bState = TRUE;
	m_btnDealPriceRao.SetCheck(bState);

	m_editDealPrice.EnableWindow(bState);
	m_ctrlDealPrice.EnableWindow(bState);

	bState = FALSE;
	m_btnDealPriceTimeRao.SetCheck(bState);
	m_btnSetDealPriceTime.EnableWindow(bState);
}

void CFilterSetWnd::OnBtnDealPriceTimeRadio()
{
	BOOL bState = FALSE;
	m_btnDealPriceRao.SetCheck(bState);

	m_editDealPrice.EnableWindow(bState);
	m_ctrlDealPrice.EnableWindow(bState);

	bState = TRUE;
	m_btnDealPriceTimeRao.SetCheck(bState);
	m_btnSetDealPriceTime.EnableWindow(bState);
}

void CFilterSetWnd::OnBtnSetDealVolTime()
{
	CSubFilterDlg dlgSubFilter(this, "VOL", &m_listDealTimeVol, m_pMainWnd);
	if (dlgSubFilter.DoModal() == IDOK)
	{

	}
}

void CFilterSetWnd::OnBtnSetDealPriceTime()
{
	CSubFilterDlg dlgSubFilter(this, "PRICE", &m_listDealTimePrice, m_pMainWnd);
	if (dlgSubFilter.DoModal() == IDOK)
	{

	}
}

void CFilterSetWnd::OnBtnDirectionChk(UINT nID)
{

}

void CFilterSetWnd::OnBtnCodeAdd()
{
	/*
	CString strCode, strName;
	strCode = m_ctrlCodeInput.GetDataText();
	m_ctrlCodeName.GetWindowText(strName);

	BOOL bAdd = TRUE;
	for (int nIndex = 0; nIndex < m_saOutCode.GetSize(); nIndex++)
	{
	if (strCode == m_saOutCode.GetAt(nIndex))
	{
	bAdd = FALSE;
	break;
	}
	}

	if (bAdd == FALSE)
	SetMessageBox("이미 추가된 종목입니다.");
	else
	{
	if (strCode.IsEmpty() == FALSE)
	{
	m_gridOutCode.SetAddRow(strCode, strName, TRUE);
	m_saOutCode.Add(strCode);
	}
	}
	*/
}

void CFilterSetWnd::OnBtnCodeDelete()
{
	CString strCode, strName;
	m_gridOutCode.GetRowData(m_gridOutCode.m_nLastSelRow, strCode, strName);

	for (int nIndex = 0; nIndex < m_saOutCode.GetSize(); nIndex++)
	{
		if (strCode == m_saOutCode.GetAt(nIndex))
		{
			m_saOutCode.RemoveAt(nIndex);
			break;
		}
	}

	m_gridOutCode.SetDeleteRow(m_gridOutCode.m_nLastSelRow);
}

void CFilterSetWnd::OnBtnCodeDeleteAll()
{
	m_gridOutCode.ClearAll();
	m_saOutCode.RemoveAll();
}

void CFilterSetWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect reClient;
	GetClientRect(reClient);

	if (m_bCreateState == TRUE)
		OnSize(reClient);
}

void CFilterSetWnd::OnSize(CRect reClient)
{
	CRect reLeft;
	reLeft = reClient;

	reLeft.top += 5;
	reLeft.bottom -= 5;

	reLeft.left += 5;
	reLeft.right = reLeft.left + 280;
	OnSize_Left(reLeft);

	reLeft.left = reLeft.right + 5;
	reLeft.right = reClient.right - 5;
	//	OnSize_Right(reLeft);
}

void CFilterSetWnd::OnSize_Left(CRect reClient)
{
	int nGroup_Offset = 5;

	CRect reTop, reTemp, reLine, reLeft;
	reTop = reClient;

	reTop.bottom = reTop.top + 16;
	m_ctrlTitle_In.MoveWindow(&reTop);

	reTop.top = reTop.bottom + nGroup_Offset;
	reTop.bottom = reTop.top + 67;
	m_btnPriceGroup.MoveWindow(&reTop);

	reTemp = reTop;
	reTemp.top += 15;
	reTemp.bottom -= 5;
	reTemp.left += 5;

	reLine = reTemp;
	reLine.bottom = reLine.top + 20;

	reLeft = reLine;
	reLeft.right = reLeft.left + 80;
	m_btnNowPriceChk.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 3;
	reLeft.right = reLeft.left + 50;
	m_editNowPriceUp.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 40;
	reTemp = reLeft;
	reTemp.top += 2;
	m_ctrlNowPrice1.MoveWindow(&reTemp);

	reLeft.left = reLeft.right + 3;
	reLeft.right = reLeft.left + 50;
	m_editNowPriceDown.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 30;
	reTemp = reLeft;
	reTemp.top += 2;
	m_ctrlNowPrice2.MoveWindow(&reTemp);


	reLine.top = reLine.bottom + 5;
	reLine.bottom = reLine.top + 20;

	reLeft = reLine;
	reLeft.right = reLeft.left + 80;
	m_btnChangRateChk.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 3;
	reLeft.right = reLeft.left + 50;
	m_editChangRateUp.MoveWindow(&reLeft);


	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 40;
	reTemp = reLeft;
	reTemp.top += 2;
	m_ctrlChangRate1.MoveWindow(&reTemp);

	reLeft.left = reLeft.right + 3;
	reLeft.right = reLeft.left + 50;
	m_editChangRateDown.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 30;
	reTemp = reLeft;
	reTemp.top += 2;
	m_ctrlChangRate2.MoveWindow(&reTemp);


	reTop.top = reTop.bottom + 5;
	reTop.bottom = reTop.top + 38;
	m_btnChangGroup.MoveWindow(&reTop);

	reTemp = reTop;
	reTemp.top += 15;
	reTemp.bottom -= 5;
	reTemp.left += 5;

	int nWidth_Btn = 45;
	for (int nIndex = 0; nIndex < _countof(TITLE_CHANGEVAL); nIndex++)
	{
		if (nIndex == 0)
			reTemp.left += 10;
		else
			reTemp.left += nWidth_Btn + 5;
		reTemp.right = reTemp.left + nWidth_Btn;
		m_btnChangValChk[nIndex].MoveWindow(&reTemp);
	}


	reTop.top = reTop.bottom + nGroup_Offset;
	reTop.bottom = reTop.top + 65;
	m_btnDealVolGroup.MoveWindow(&reTop);

	reTemp = reTop;
	reTemp.top += 15;
	reTemp.bottom -= 5;
	reTemp.left += 5;

	reLine = reTemp;
	reLine.bottom = reLine.top + 20;

	reLeft = reLine;
	reLeft.right = reLeft.left + 100;
	m_btnDealVolRao.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 3;
	reLeft.right = reLeft.left + 50;
	m_editDealVol.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 80;
	reTemp = reLeft;
	reTemp.top += 2;
	m_ctrlDealVol.MoveWindow(&reTemp);

	reLine.top = reLine.bottom + 5;
	reLine.bottom = reLine.top + 20;

	reLeft = reLine;
	reLeft.right = reLeft.left + 110;
	m_btnDealVolTimeRao.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 20;
	reLeft.right = reLeft.left + 84;
	reTemp = reLeft;
	reTemp.top -= 1;
	reTemp.bottom = reTemp.top + 20;
	m_btnSetDealVolTime.MoveWindow(&reTemp);


	/*
	reTop.top = reTop.bottom + nGroup_Offset;
	reTop.bottom = reTop.top + 65;
	m_btnDealPriceGroup.MoveWindow(&reTop);

	reTemp = reTop;
	reTemp.top += 15;
	reTemp.bottom -= 5;
	reTemp.left += 5;

	reLine = reTemp;
	reLine.bottom = reLine.top + 20;

	reLeft = reLine;
	reLeft.right = reLeft.left + 100;
	m_btnDealPriceRao.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 3;
	reLeft.right = reLeft.left + 50;
	m_editDealPrice.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 80;
	reTemp = reLeft;
	reTemp.top += 2;
	m_ctrlDealPrice.MoveWindow(&reTemp);

	reLine.top = reLine.bottom + 3;
	reLine.bottom = reLine.top + 18;

	reLeft = reLine;
	reLeft.right = reLeft.left + 130;
	m_btnDealPriceTimeRao.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 20;
	reLeft.right = reLeft.left + 70;
	reTemp = reLeft;
	reTemp.top -= 1;
	reTemp.bottom = reTemp.top + 20;
	m_btnSetDealPriceTime.MoveWindow(&reTemp);

	reTop.top = reTop.bottom + nGroup_Offset;
	reTop.bottom = reTop.top + 65;
	m_btnConcludeGroup.MoveWindow(&reTop);

	reTemp = reTop;
	reTemp.top += 15;
	reTemp.bottom -= 5;
	reTemp.left += 5;

	reLine = reTemp;
	reLine.bottom = reLine.top + 20;

	reLeft = reLine;
	reLeft.right = reLeft.left + 70;
	m_btnDealVolChk_Conclude.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 3;
	reLeft.right = reLeft.left + 50;
	m_editDealVol_Conclude.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 80;
	reTemp = reLeft;
	reTemp.top += 2;
	m_ctrlDealVol_Conclude.MoveWindow(&reTemp);

	reLine.top = reLine.bottom + 5;
	reLine.bottom = reLine.top + 20;

	reLeft = reLine;
	reLeft.right = reLeft.left + 70;
	m_btnDealPriceChk_Conclude.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 3;
	reLeft.right = reLeft.left + 50;
	m_editDealPrice_Conclude.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 80;
	reTemp = reLeft;
	reTemp.top += 2;
	m_ctrlDealPrice_Conclude.MoveWindow(&reTemp);
	*/
}

void CFilterSetWnd::OnSize_Right(CRect reClient)
{
	CRect reTop, reLeft, reTemp, reLine;
	reTop = reClient;

	/*
	reTop.bottom = reTop.top + 65;
	m_btnConcludeGroup.MoveWindow(&reTop);

	reTemp = reTop;
	reTemp.top += 15;
	reTemp.bottom -= 5;
	reTemp.left += 5;

	reLine = reTemp;
	reLine.bottom = reLine.top + 20;

	reLeft = reLine;
	reLeft.right = reLeft.left + 70;
	m_btnDealVolChk_Conclude.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 3;
	reLeft.right = reLeft.left + 50;
	m_editDealVol_Conclude.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 80;
	reTemp = reLeft;
	reTemp.top += 2;
	m_ctrlDealVol_Conclude.MoveWindow(&reTemp);

	reLine.top = reLine.bottom + 5;
	reLine.bottom = reLine.top + 20;

	reLeft = reLine;
	reLeft.right = reLeft.left + 70;
	m_btnDealPriceChk_Conclude.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 3;
	reLeft.right = reLeft.left + 50;
	m_editDealPrice_Conclude.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 80;
	reTemp = reLeft;
	reTemp.top += 2;
	m_ctrlDealPrice_Conclude.MoveWindow(&reTemp);
	*/

	//reTop.top = reTop.bottom + 8;
	reTop.bottom = reTop.top + 16;
	m_ctrlTitle_Out.MoveWindow(&reTop);

	reTop.top = reTop.bottom + 5;
	reTop.bottom = reClient.bottom;

	reLeft = reTop;
	reLeft.right = reLeft.left + 110;
	m_btntDirectionGroup.MoveWindow(&reLeft);

	reTemp = reLeft;
	reTemp.top += 18;
	reTemp.left += 10;
	reTemp.right -= 3;
	for (int nItem = 0; nItem < _countof(TITLE_DIRECTION); nItem++)
	{
		if (nItem > 0)
			reTemp.top = reTemp.bottom + 3;
		reTemp.bottom = reTemp.top + 22;
		m_btntDirectionChk[nItem].MoveWindow(&reTemp);
	}

	reLeft.left = reLeft.right + 2;
	reLeft.right = reClient.right;
	m_btnOutCodeGroup.MoveWindow(&reLeft);

	reTemp = reLeft;
	reTemp.top += 18;
	reTemp.left += 8;
	reTemp.right -= 8;

	reTemp.bottom = reTemp.top + 20;

	reLine = reTemp;
	reLine.right = reLine.left + 100;
	m_ctrlCodeInput.MoveWindow(&reLine);

	reLine.left = reLine.right + 2;
	reLine.right = reTemp.right;
	reLine.top += 4;
	reLine.bottom -= 2;
	m_ctrlCodeName.MoveWindow(&reLine);
	reLine.top -= 4;
	reLine.bottom += 2;

	reTemp.top = reTemp.bottom + 2;
	reTemp.bottom = reTemp.top + 20;

	reLine = reTemp;
	int nBtnWidth = (reTemp.right - reTemp.left - 4) / 3;
	reLine.right = reLine.left + nBtnWidth;
	m_btnCodeAdd.MoveWindow(&reLine);

	reLine.left = reLine.right + 2;
	reLine.right = reLine.left + nBtnWidth;	
	m_btnCodeDelete.MoveWindow(&reLine);

	reLine.left = reLine.right + 2;
	reLine.right = reLine.left + nBtnWidth;	
	m_btnCodeDeleteAll.MoveWindow(&reLine);

	reTemp.top = reTemp.bottom + 2;
	reTemp.bottom = reClient.bottom - 10;
	m_gridOutCode.MoveWindow(&reTemp);
}

HBRUSH CFilterSetWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pMainWnd;

		pDC->SetBkMode(COLOR_DLG_CANVAS);
		hbr = pMainWnd->GetAxBrush(pMainWnd->GetAxColor(66));
	}

	return hbr;
}


void CFilterSetWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pMainWnd;

	CRect reClient;
	GetClientRect(reClient);
	//dc.FillSolidRect(reClient, COLOR_DLG_CANVAS);
	dc.FillSolidRect(reClient, pMainWnd->GetAxColor(66));
}

LRESULT CFilterSetWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	/*
	if (message == RMSG_EVENT_CODEINPUT)
	{
	LPST_CTRLEVENT stEvent = (LPST_CTRLEVENT)lParam;
	if(strcmp("LengthFull", stEvent->lpszEventName) == 0 || strcmp("SelChanged", stEvent->lpszEventName) == 0)
	{
	CString strCode;
	strCode = m_ctrlCodeInput.GetDataText();
	m_ctrlCodeName.SetWindowText(GetCodeToName(strCode));
	}
	}
	*/

	return CWnd::WindowProc(message, wParam, lParam);
}

CString CFilterSetWnd::GetCodeToName(CString strCode)
{
	CString strName = "";

	/*
	int nID = 5;
	IMasterDataManager* pManager = (IMasterDataManager*)AfxGetPctrInterface(nID);
	if (pManager != NULL)
	{
	strName = pManager->GetCodeInfo("JMASTER", 0, strCode);

	if (strName.IsEmpty() == TRUE)
	strName = pManager->GetCodeInfo("KMASTER", 0, strCode);
	}
	*/

	return strName;
}

void CFilterSetWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
}

BOOL CFilterSetWnd::SetInfo_Filter(BOOL bLoad, BOOL bDefultVal)
{
	BOOL bResult = FALSE;
	if ((bLoad == FALSE) && (m_bLoad_Filter == FALSE))
		return bResult;

	CFilterSetDlg* pFilterSetDlg = (CFilterSetDlg*)GetParent();
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pMainWnd;
	
	CString strUserPath;
	strUserPath.Format("%s%s%s", pMainFrmWnd->Variant(homeCC), USRDIR, pMainFrmWnd->Variant(nameCC));
	
	CString strFullPath;
	if(pFilterSetDlg->GetIndustry())
		strFullPath = strUserPath + FILTTER_FILE_REAL;
	else
		strFullPath = strUserPath + FILTTER_FILE;
	
	HINSTANCE		hConditionMng;
	typedef BOOL (*_DLL_GetFilter) (CString strFullPath, long* plFilterInfo);
	typedef void (*_DLL_GetFilter_Base) (long* plFilterInfo);
	typedef BOOL (*_DLL_GetFilterToPacket) (CString strFullPath, LPSTR &lpPacket);
	typedef BOOL (*_DLL_SetFilter) (CString strFullPath, long* plFilterInfo);

	_DLL_GetFilter					Lib_GetFilter;
	_DLL_GetFilter_Base     Lib_GetFilter_Base;
	_DLL_GetFilterToPacket	Lib_GetFilterToPacket;
	_DLL_SetFilter					Lib_SetFilter;

	hConditionMng = LoadLibrary("Lib_ConditionMng.dll");
	if (hConditionMng)
	{
		Lib_GetFilter = (_DLL_GetFilter)::GetProcAddress(hConditionMng, "_DLL_GetFilter");
		Lib_GetFilter_Base = (_DLL_GetFilter_Base)::GetProcAddress(hConditionMng, "_DLL_GetFilter_Base");
		Lib_GetFilterToPacket = (_DLL_GetFilterToPacket)::GetProcAddress(hConditionMng, "_DLL_GetFilterToPacket");
		Lib_SetFilter = (_DLL_SetFilter)::GetProcAddress(hConditionMng, "_DLL_SetFilter");

		if (bLoad == TRUE)
		{
			if (bDefultVal == TRUE)
			{
				Lib_GetFilter_Base((long*)&m_stFilterInfo);
				bResult = TRUE;
			}
			else
				bResult = Lib_GetFilter(strFullPath, (long*)&m_stFilterInfo);
		}
		else
		{
			if (GetInfo() == TRUE)
				bResult = Lib_SetFilter(strFullPath, (long*)&m_stFilterInfo);
			else 
				bResult = FALSE;
		}
	}

	FreeLibrary(hConditionMng); 
	hConditionMng = NULL;

	return bResult;
}

int CFilterSetWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	if (lpszCaption == NULL)
	{
		CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pMainWnd;

		if(pMainFrmWnd->m_nIndustry == 0)
			lpszCaption = (CString)PROGRAM_TITLE, MB_OK;
		else if(pMainFrmWnd->m_nIndustry == 1)
			lpszCaption = (CString)PROGRAM_TITLE_INDUSTRY, MB_OK;
		else
			lpszCaption = (CString)PROGRAM_TITLE_REAL, MB_OK;
	}

	return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
}
