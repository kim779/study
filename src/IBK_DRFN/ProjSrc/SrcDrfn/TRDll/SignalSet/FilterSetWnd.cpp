// FilterSetWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrmWnd.h"
#include "FilterSetWnd.h"
#include "SubFilterDlg.h"

#include "../../../SrcSite/include/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;
/////////////////////////////////////////////////////////////////////////////
// CFilterSetWnd
CFilterSetWnd::CFilterSetWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	
	m_brushStatic.CreateSolidBrush(COLOR_SETWND_FACE);
	m_brushSubTitlePoint.CreateSolidBrush(COLOR_SUBTITLE_POINT);
	
	m_stFilterInfo.psaInCode = NULL;
	m_stFilterInfo.plistDealTimeVol = NULL;
	m_stFilterInfo.plistDealTimePrice = NULL;
	
	m_pctrlCodeInput = NULL;
	m_bShow_PF_CTL = PF_CONTROL_SHOW;
}

CFilterSetWnd::~CFilterSetWnd()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	
	m_brushStatic.DeleteObject();
	m_brushSubTitlePoint.DeleteObject();
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

ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_INCODERADIO, IDC_BTN_INCODERADIO + _countof(TITLE_INCODE), OnBtnInCodeRadio)

ON_BN_CLICKED(IDC_BTN_NOWPRICECHK, OnBtnNowPriceChk)
ON_BN_CLICKED(IDC_BTN_CHANGERATECHK, OnBtnChangeRateChk)

ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_CHANGEVALCHK, IDC_BTN_CHANGEVALCHK + _countof(TITLE_CHANGEVAL), OnBtnChangeValChk)

ON_BN_CLICKED(IDC_BTN_DEALVOLRADIO, OnBtnDealVolRadio)
ON_BN_CLICKED(IDC_BTN_DEALVOLTIMERADIO, OnBtnDealVolTimeRadio)
ON_BN_CLICKED(IDC_BTN_DEALPRICERADIO, OnBtnDealPriceRadio)
ON_BN_CLICKED(IDC_BTN_DEALPRICETIMERADIO, OnBtnDealPriceTimeRadio)

ON_BN_CLICKED(IDC_BTN_SETDEALVOLTIME, OnBtnSetDealVolTime)
ON_BN_CLICKED(IDC_BTN_SETDEALPRICETIME, OnBtnSetDealPriceTime)

ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_NEWSRADIO, IDC_BTN_NEWSRADIO + _countof(TITLE_NEWS), OnBtnNewsRadio)
ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_DIRECTIONCHK, IDC_BTN_DIRECTIONCHK + _countof(TITLE_DIRECTION), OnBtnDirectionChk)

ON_BN_CLICKED(IDC_BTN_CODEADD, OnBtnCodeAdd)
ON_BN_CLICKED(IDC_BTN_CODEDELETE, OnBtnCodeDelete)
ON_BN_CLICKED(IDC_BTN_CODEDELETEALL, OnBtnCodeDeleteAll)

ON_BN_CLICKED(IDC_CHK_SOUND, OnChkSound)
ON_BN_CLICKED(IDC_BTN_SEARCHSOUND, OnBtnSearchsound)
ON_BN_CLICKED(IDC_BTN_SOUND, OnBtnSound)

ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFilterSetWnd message handlers

int CFilterSetWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	
	int nIndex;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
		
	m_ctrlLeftTitlePoint.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this, IDC_SIATIC_SUBTITLEPOINT);
	
	m_ctrlTitle_In.Create(_T("대상종목"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTitle_In.SetFont(&m_fontBold);
	
	
	m_btnInCodeGroup.Create(_T("대상그룹"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_INCODEGROUP);
	m_btnInCodeGroup.SetFont(&m_fontDefault);

	
	for (nIndex = 0; nIndex < _countof(TITLE_INCODE); nIndex++)
	{
		m_btnInCodeRao[nIndex].Create(TITLE_INCODE[nIndex], WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
			CRect(0, 0, 0, 0), this, IDC_BTN_INCODERADIO + nIndex);
		m_btnInCodeRao[nIndex].SetFont(&m_fontDefault);
	}
	
	m_comboMarket.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_MARKET);
	m_comboMarket.SetFont(&m_fontDefault);
	
	m_comboPortGroup.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_PORTGROUP);
	m_comboPortGroup.SetFont(&m_fontDefault);
	
	
	m_btnPriceGroup.Create(_T("가격범위"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPPRICE);
	m_btnPriceGroup.SetFont(&m_fontDefault);
	
	m_btnNowPriceChk.Create(_T("현재가(원)"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_NOWPRICECHK);
	m_btnNowPriceChk.SetFont(&m_fontDefault);
	
	m_editNowPriceUp.Create(WS_CHILD | WS_VISIBLE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_EDIT_NOWPRICEUP);
	m_editNowPriceUp.SetFont(&m_fontDefault);
	m_editNowPriceUp.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	m_ctrlNowPrice1.Create(_T("이상 ~"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlNowPrice1.SetFont(&m_fontDefault);
	
	m_editNowPriceDown.Create(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_EDIT_NOWPRICEDOWN);
	m_editNowPriceDown.SetFont(&m_fontDefault);
	m_editNowPriceDown.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
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
	
	m_ctrlChangRate1.Create(_T("이상 ~"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlChangRate1.SetFont(&m_fontDefault);
	
	m_editChangRateDown.Create(WS_CHILD | WS_VISIBLE, //| WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_EDIT_CHANGERATEDOWN);
	m_editChangRateDown.SetFont(&m_fontDefault);
	m_editChangRateDown.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	
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
	
	m_editDealVol.Create(WS_CHILD | WS_VISIBLE,//| WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_EDIT_DEALVOL);
	m_editDealVol.SetFont(&m_fontDefault);
	m_editDealVol.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	
	m_ctrlDealVol.Create(_T("천주 이상"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlDealVol.SetFont(&m_fontDefault);
	
	m_btnDealVolTimeRao.Create(_T("시간대별 거래량"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DEALVOLTIMERADIO);
	m_btnDealVolTimeRao.SetFont(&m_fontDefault);
	
	m_btnSetDealVolTime.Create(_T("설정/보기"), CRect(0, 0, 0, 0), this, IDC_BTN_SETDEALVOLTIME);
	pwndMainFrm->SetBtnImg(&m_btnSetDealVolTime, 6);
	
	m_btnDealPriceGroup.Create(_T("거래대금 범위"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPDEALPRICE);
	m_btnDealPriceGroup.SetFont(&m_fontDefault);
	
	m_btnDealPriceRao.Create(_T("거래대금"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DEALPRICERADIO);
	m_btnDealPriceRao.SetFont(&m_fontDefault);
	
	m_editDealPrice.Create(WS_CHILD | WS_VISIBLE,//| WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_EDIT_DEALPRICE);
	m_editDealPrice.SetFont(&m_fontDefault);
	m_editDealPrice.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	m_ctrlDealPrice.Create(_T("백만원 이상"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlDealPrice.SetFont(&m_fontDefault);
	
	m_btnDealPriceTimeRao.Create(_T("시간대별 거래대금"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DEALPRICETIMERADIO);
	m_btnDealPriceTimeRao.SetFont(&m_fontDefault);
	
	m_btnSetDealPriceTime.Create(_T("설정/보기"), CRect(0, 0, 0, 0), this, IDC_BTN_SETDEALPRICETIME);
	pwndMainFrm->SetBtnImg(&m_btnSetDealPriceTime, 6);
	
	m_btnNewsGroup.Create(_T("뉴스구분"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPNEWS);
	m_btnNewsGroup.SetFont(&m_fontDefault);
	m_btnNewsGroup.ShowWindow(SW_HIDE);
	
	for (nIndex = 0; nIndex < _countof(TITLE_NEWS); nIndex++)
	{
		m_btnNewsRao[nIndex].Create(TITLE_NEWS[nIndex], WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
			CRect(0, 0, 0, 0), this, IDC_BTN_NEWSRADIO + nIndex);
		m_btnNewsRao[nIndex].SetFont(&m_fontDefault);
		m_btnNewsRao[nIndex].EnableWindow(FALSE);
		m_btnNewsRao[nIndex].ShowWindow(SW_HIDE);
	}
	
	m_ctrlRightTitlePoint.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_SIATIC_SUBTITLEPOINT + 1);
	
	m_ctrlTitle_Out.Create(_T("제외종목"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTitle_Out.SetFont(&m_fontBold);
	
	m_btnDirectionGroup.Create(_T("시장조치/우선주"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPDIRECTION);
	m_btnDirectionGroup.SetFont(&m_fontDefault);

	m_btnSoundGroup.Create(_T("소리설정"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
	CRect(0, 0, 0, 0), this, IDC_BTN_GROUPSOUNDSET);
	m_btnSoundGroup.SetFont(&m_fontDefault);

	m_chkSound.Create(_T("소리사용"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_CHK_SOUND);
	m_chkSound.SetFont(&m_fontDefault);

	m_editSound.Create(WS_CHILD | WS_VISIBLE,//| WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_EDIT_SOUND);
	m_editSound.SetFont(&m_fontDefault);
	m_editSound.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	m_btnSound.Create(_T("소리"), CRect(0, 0, 0, 0), this, IDC_BTN_SOUND);
	pwndMainFrm->SetBtnImg(&m_btnSound, 2);
	
	m_btnSearchSound.Create(_T("..."), CRect(0, 0, 0, 0), this, IDC_BTN_SEARCHSOUND);
	pwndMainFrm->SetBtnImg(&m_btnSearchSound, 2);
	
	for (nIndex = 0; nIndex < _countof(TITLE_DIRECTION); nIndex++)
	{
		m_btnDirectionChk[nIndex].Create(TITLE_DIRECTION[nIndex], WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
			CRect(0, 0, 0, 0), this, IDC_BTN_DIRECTIONCHK + nIndex);
		m_btnDirectionChk[nIndex].SetFont(&m_fontDefault);
	}
	
	m_ctrlCodeInput.Create(_T("Code Input"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeInput.SetFont(&m_fontDefault);
	
	m_btnOutCodeGroup.Create(_T("제외종목"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPOUTCODE);
	m_btnOutCodeGroup.SetFont(&m_fontDefault);
	
	m_ctrlCodeName.Create(_T("종목명"), WS_CHILD | /*WS_VISIBLE |*/ SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeName.SetFont(&m_fontDefault);
	
	m_btnCodeAdd.Create(_T("추가"), CRect(0, 0, 0, 0), this, IDC_BTN_CODEADD);
	pwndMainFrm->SetBtnImg(&m_btnCodeAdd, 2);
	
	m_btnCodeDelete.Create(_T("삭제"), CRect(0, 0, 0, 0), this, IDC_BTN_CODEDELETE);
	pwndMainFrm->SetBtnImg(&m_btnCodeDelete, 2);
	
	m_btnCodeDeleteAll.Create(_T("전체삭제"), CRect(0, 0, 0, 0), this, IDC_BTN_CODEDELETEALL);
	pwndMainFrm->SetBtnImg(&m_btnCodeDeleteAll, 4);
	
	m_gridOutCode.Create(NULL, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, NULL);
	
	AfxSetResourceHandle(hInstSave);

	return 0;
}

void CFilterSetWnd::SetInit(CMapStringToOb	*pMapControlName)
{
	m_gridOutCode.SetInit("CODE");
	
	m_comboMarket.ResetContent();
	m_comboMarket.AddString("전체");
	m_comboMarket.AddString("거래소");
	m_comboMarket.AddString("코스닥");
	m_comboMarket.SetCurSel(0);

	OnChkSound();
	
	if (m_stFilterInfo.psaInCode == NULL)
		m_stFilterInfo.psaInCode = new CStringArray;
	
	if (m_stFilterInfo.plistDealTimeVol == NULL)
		m_stFilterInfo.plistDealTimeVol = new CList <SubFilterInfo, SubFilterInfo&>;
	
	if (m_stFilterInfo.plistDealTimePrice == NULL)
		m_stFilterInfo.plistDealTimePrice = new CList <SubFilterInfo, SubFilterInfo&>;
	
	m_listDealTimeVol.RemoveAll();
	m_listDealTimePrice.RemoveAll();
	
	GetPortData();
}

void CFilterSetWnd::SetCodeComboInfo(CCodeCtrl* pCodeCombo, HWND hCodeCombo)
{
	m_pctrlJMCombo = pCodeCombo;
	m_pctrlCodeInput = CWnd::FromHandle(hCodeCombo);
}

extern void Dll_GetRegKwansimMasterGroupName(CStringArray &arrName);
void CFilterSetWnd::GetPortData()
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	m_comboPortGroup.ResetContent();
	
	//CStringArray	saGroupKey, saGroupName;

	pwndMainFrm->GetData_PortGroup(m_GwanSimGroup.strKey, m_GwanSimGroup.strGroupName);
	
	for(int nIndex = 0; nIndex < m_GwanSimGroup.strGroupName.GetSize(); nIndex++)
		m_comboPortGroup.AddString(m_GwanSimGroup.strGroupName.GetAt(nIndex));
	
	if(m_GwanSimGroup.strGroupName.GetSize() > 0)
		m_comboPortGroup.SetCurSel(0);
}

void CFilterSetWnd::SetInit_SetVal()
{
	int nIndex;
	m_stFilterInfo.nInCodeType = 0;        //# 0 : Market, 1 : PortPolio 
	m_stFilterInfo.strInCodeData = "0";  //# [0 : 전체, 1 : 코스피, 2 : 코스닥] or [PortPolio Name]
	
	if (m_stFilterInfo.psaInCode != NULL)
		m_stFilterInfo.psaInCode->RemoveAll();  //# User After Load!
	
	m_stFilterInfo.nNowPriceChk = 0;
	m_stFilterInfo.nNowPriceUp = 5000; 
	m_stFilterInfo.nNowPriceDown = 10000; 
	
	m_stFilterInfo.nChangRateChk = 0; 
	m_stFilterInfo.nChangRateUp = 0; 
	m_stFilterInfo.nChangRateDown = 500; 
	
	for (nIndex = 0; nIndex < _countof(TITLE_CHANGEVAL); nIndex++)
		m_stFilterInfo.nChangValChk[nIndex] = 1;
	
	m_stFilterInfo.nDealVolType = 0;		//# 0 : 고정, 1 : Time
	m_stFilterInfo.nDealVol = 5;
	
	if (m_stFilterInfo.plistDealTimeVol != NULL)
		m_stFilterInfo.plistDealTimeVol->RemoveAll();
	
	m_stFilterInfo.nDealPriceType = 0;		//# 0 : 고정, 1 : Time
	m_stFilterInfo.nDealPrice = 50;
	if (m_stFilterInfo.plistDealTimePrice != NULL)
		m_stFilterInfo.plistDealTimePrice->RemoveAll();
	
	for (nIndex = 0; nIndex < _countof(TITLE_DIRECTION); nIndex++)
		m_stFilterInfo.nDirectionChk[nIndex] = 0;
	
	m_stFilterInfo.nNewsType = 0;   //# 0 : 전체, 1 : 뉴스, 2 : 공시
	
	m_stFilterInfo.saOutCode.RemoveAll();
	
	SetInfo();
}

void CFilterSetWnd::SetInfo()
{
	int nIndex;
	CString strData;
	
	//# Set Data!
	for (nIndex = 0; nIndex < _countof(TITLE_INCODE); nIndex++)
	{
		CButton *pBtn = (CButton*)GetDlgItem(IDC_BTN_INCODERADIO + nIndex);
		if (m_stFilterInfo.nInCodeType == nIndex)
		{
			OnBtnInCodeRadio(IDC_BTN_INCODERADIO + nIndex);
			break;
		}
	}
	
	if (m_stFilterInfo.nInCodeType == 0)  //# 0 : Market
	{
		nIndex = atoi(m_stFilterInfo.strInCodeData);
		m_comboMarket.SetCurSel(nIndex);    //# [0 : 전체, 1 : 코스피, 2 : 코스닥] or [PortPolio Name]
	}
	else if (m_stFilterInfo.nInCodeType == 1)  //# 1 : PortPolio
	{
		//# [0 : 전체, 1 : 코스피, 2 : 코스닥] or [PortPolio Name]
		nIndex = m_comboPortGroup.FindString(0, m_stFilterInfo.strInCodeData);
		
		if (nIndex == LB_ERR)
		{
			if (m_comboPortGroup.GetCount() > 0)
				m_comboPortGroup.SetCurSel(0);
		}
		else
			m_comboPortGroup.SetCurSel(nIndex);
	}
	
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
	strData.Format("%d", m_stFilterInfo.nDealVol);
	m_editDealVol.SetWindowText(strData);
	
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
	
	if (m_stFilterInfo.nDealVolType == 0)
	{
		m_btnDealVolRao.SetCheck(TRUE);
		OnBtnDealVolRadio();
	}
	else if (m_stFilterInfo.nDealVolType == 1)
	{
		m_btnDealVolTimeRao.SetCheck(TRUE);
		OnBtnDealVolTimeRadio();
	}
	
	strData.Format("%d", m_stFilterInfo.nDealPrice);
	m_editDealPrice.SetWindowText(strData);
	
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
	
	if (m_stFilterInfo.nDealPriceType == 0)
	{
		m_btnDealPriceRao.SetCheck(TRUE);
		OnBtnDealPriceRadio();
	}
	else if (m_stFilterInfo.nDealPriceType == 1)
	{
		m_btnDealPriceTimeRao.SetCheck(TRUE);
		OnBtnDealPriceTimeRadio();
	}
	
	/*m_btnNewsRao[m_stFilterInfo.nNewsType].SetCheck(TRUE);*/
	for (nIndex = 0; nIndex < _countof(TITLE_NEWS); nIndex++)
	{
		CButton *pBtn = (CButton*)GetDlgItem(IDC_BTN_NEWSRADIO + nIndex);
		if (m_stFilterInfo.nNewsType == nIndex)
			pBtn->SetCheck(TRUE);
		else
			pBtn->SetCheck(FALSE);
	}
	
	for (nIndex = 0; nIndex < _countof(TITLE_DIRECTION); nIndex++)
		m_btnDirectionChk[nIndex].SetCheck(m_stFilterInfo.nDirectionChk[nIndex]);
	
	m_saOutCode.RemoveAll();
	int nPos;
	CString strCode, strName;
	for (nIndex = 0; nIndex < m_stFilterInfo.saOutCode.GetSize(); nIndex++)
	{
		strData = m_stFilterInfo.saOutCode.GetAt(nIndex);
		nPos = strData.Find("/");
		strCode = strData.Left(nPos);
		strName = strData.Right(strData.GetLength() - nPos -1);
		
		m_gridOutCode.SetAdd_Row(strCode, strName);
		m_saOutCode.Add(strData);
	}
	m_gridOutCode.RedrawAll();

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	
	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = pwndMainFrm->GetAxColor(74);
	stGridColor.clrGridHilight = pwndMainFrm->GetAxColor(78);
	
	CString strFilePath, strAppPath, strFileName;
	GetDir_DrfnRoot(strAppPath);
	strFileName = strAppPath + "\\user\\" + "soundSet.ini";
	char sztemp[128];
	GetPrivateProfileString(_T("CHKSOUND"), _T("CHECKSTATE"), "0", sztemp, sizeof(sztemp), strFileName);
	if (atoi(sztemp) == 0)
		m_chkSound.SetCheck(FALSE);
	else
	{
		m_chkSound.SetCheck(TRUE);
		m_editSound.EnableWindow(TRUE);	
		m_btnSound.EnableWindow(TRUE);
		m_btnSearchSound.EnableWindow(TRUE);
		GetPrivateProfileString(_T("ALARMSET"), _T("alarm"), "1", sztemp, sizeof(sztemp), strFileName);
		strFilePath = sztemp;
		m_editSound.SetWindowText(strFilePath);
	}
	ChangeGridColor(stGridColor);
}

BOOL CFilterSetWnd::GetInfo()
{
	BOOL bResult = FALSE; 
	
	int nIndex;
	CString strData, strData2;
	
	CButton *pBtn;
	for (nIndex = 0; nIndex < _countof(TITLE_INCODE); nIndex++)
	{
		pBtn = (CButton*)GetDlgItem(IDC_BTN_INCODERADIO + nIndex);  //# 0 : Market, 1 : PortPolio 
		if (pBtn->GetCheck() == TRUE)
		{
			m_stFilterInfo.nInCodeType = nIndex;
			break;
		}
	}
	
	if (m_stFilterInfo.nInCodeType == 0)  //# 0 : Market
	{
		strData.Format("%d", m_comboMarket.GetCurSel());
		m_stFilterInfo.strInCodeData = strData;  //# [0 : 전체, 1 : 코스피, 2 : 코스닥] or [PortPolio Name]
	}
	else if (m_stFilterInfo.nInCodeType == 1)  //# 1 : PortPolio
	{
		m_comboPortGroup.GetWindowText(strData);
		for(int i=0; i < m_GwanSimGroup.strGroupName.GetSize(); i++)
		{
			if(strData == m_GwanSimGroup.strGroupName.GetAt(i))
				strData = m_GwanSimGroup.strKey.GetAt(i);
		}
		m_stFilterInfo.strInCodeData = strData;  //# [0 : 전체, 1 : 코스피, 2 : 코스닥] or [PortPolio Name]
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
			SetMessageBox("상한 등락률이 입력값 오류! \n 100보다 작은 값이어야 합니다.");
			m_editChangRateDown.SetSel(strData.GetLength());
			m_editChangRateDown.SetFocus();
			
			return bResult;
		}
		else if (atof(strData2) > 100)
		{
			SetMessageBox("하한 등락률이 입력값 오류! \n 100보다 작은 값이어야 합니다.");
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
	
	for (nIndex = 0; nIndex < _countof(TITLE_CHANGEVAL); nIndex++)
	{
		pBtn = (CButton*)GetDlgItem(IDC_BTN_CHANGEVALCHK + nIndex);
		m_stFilterInfo.nChangValChk[nIndex] = pBtn->GetCheck();
	}
	
	POSITION pos;
	SubFilterInfo		stSubFilterInfo;
	m_editDealVol.GetWindowText(strData);
	m_stFilterInfo.nDealVol = atoi(strData);
	
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
	
	if (m_btnDealVolRao.GetCheck() == TRUE)
		m_stFilterInfo.nDealVolType = 0;
	else if (m_btnDealVolTimeRao.GetCheck() == TRUE)
		m_stFilterInfo.nDealVolType = 1;
  		
	m_editDealPrice.GetWindowText(strData);
	m_stFilterInfo.nDealPrice = atoi(strData);
	
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
	
	if (m_btnDealPriceRao.GetCheck() == TRUE)
		m_stFilterInfo.nDealPriceType = 0;
	else if (m_btnDealPriceTimeRao.GetCheck() == TRUE)
		m_stFilterInfo.nDealPriceType = 1;
	
	for (nIndex = 0; nIndex < _countof(TITLE_NEWS); nIndex++)
	{
		pBtn = (CButton*)GetDlgItem(IDC_BTN_NEWSRADIO + nIndex);
		if (pBtn->GetCheck() == TRUE)
		{
			m_stFilterInfo.nNewsType = nIndex;
			break;
		}
	}
	
	for (nIndex = 0; nIndex < _countof(TITLE_DIRECTION); nIndex++)
	{
		pBtn = (CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + nIndex);
		
		m_stFilterInfo.nDirectionChk[nIndex] = pBtn->GetCheck();
	}
	
	m_stFilterInfo.saOutCode.RemoveAll();
	CString strCode, strName;
	for (nIndex = 0; nIndex < m_gridOutCode.GetNumberRows(); nIndex++)
	{
		m_gridOutCode.GetData_Row(nIndex, strCode, strName);
		m_stFilterInfo.saOutCode.Add(strCode + "/" + strName);
	}

	CString strAppPath;
	GetDir_DrfnRoot(strAppPath);
	CString strFileName = strAppPath + "\\user\\" + "soundSet.ini";
	m_bchksound=((CButton*)GetDlgItem(IDC_CHK_SOUND))->GetCheck();
	WritePrivateProfileString("CHKSOUND","CHECKSTATE",m_bchksound ? "1" : "0", strFileName);
	return TRUE;
}

void CFilterSetWnd::OnDestroy() 
{
	if (m_stFilterInfo.psaInCode != NULL)
	{
		m_stFilterInfo.psaInCode->RemoveAll();
		delete m_stFilterInfo.psaInCode;
		m_stFilterInfo.psaInCode = NULL;
	}
	
	if (m_stFilterInfo.plistDealTimeVol != NULL)
	{
		m_stFilterInfo.plistDealTimeVol->RemoveAll();
		delete m_stFilterInfo.plistDealTimeVol;
		m_stFilterInfo.plistDealTimeVol = NULL;
	}
	
	if (m_stFilterInfo.plistDealTimePrice != NULL)
	{
		m_stFilterInfo.plistDealTimePrice->RemoveAll();
		delete m_stFilterInfo.plistDealTimePrice;
		m_stFilterInfo.plistDealTimePrice = NULL;
	}
	
	m_listDealTimeVol.RemoveAll();
	m_listDealTimePrice.RemoveAll();
	
	CWnd::OnDestroy();
}

void CFilterSetWnd::OnBtnInCodeRadio(UINT nID)
{
	for (int nIndex = 0; nIndex < _countof(TITLE_INCODE); nIndex++)
	{
		CButton *pBtn = (CButton*)GetDlgItem(IDC_BTN_INCODERADIO + nIndex);
		if ((int)nID == IDC_BTN_INCODERADIO + nIndex)
		{
			pBtn->SetCheck(TRUE);
			
			if (nIndex == 0)
				m_comboMarket.EnableWindow(TRUE);
			else if (nIndex == 1)
			{
				if (m_comboPortGroup.GetCount() > 0)
					m_comboPortGroup.EnableWindow(TRUE);
				else
				{
					m_comboPortGroup.EnableWindow(FALSE);
					m_comboPortGroup.SetWindowText("없음");
				}
			}
		}
		else
		{
			pBtn->SetCheck(FALSE);
			
			if (nIndex == 0)
				m_comboMarket.EnableWindow(FALSE);
			else if (nIndex == 1)
				m_comboPortGroup.EnableWindow(FALSE);
		}
	}
}

void CFilterSetWnd::OnBtnNowPriceChk()
{
	BOOL bState = m_btnNowPriceChk.GetCheck();
	
	m_editNowPriceUp.EnableWindow(bState);
	m_editNowPriceDown.EnableWindow(bState);
	m_ctrlNowPrice1.EnableWindow(bState);
	m_ctrlNowPrice2.EnableWindow(bState);
	
	if (bState == TRUE)
	{
		CString strData;
		m_editNowPriceUp.GetWindowText(strData);
		m_editNowPriceUp.SetSel(0, strData.GetLength());
		
		m_editNowPriceUp.SetFocus();
	}
}

void CFilterSetWnd::OnBtnChangeRateChk()
{
	BOOL bState = m_btnChangRateChk.GetCheck();
	
	m_editChangRateUp.EnableWindow(bState);
	m_editChangRateDown.EnableWindow(bState);
	m_ctrlChangRate1.EnableWindow(bState);
	m_ctrlChangRate2.EnableWindow(bState);
	
	if (bState == TRUE)
	{
		CString strData;
		m_editChangRateUp.GetWindowText(strData);
		m_editChangRateUp.SetSel(0, strData.GetLength());
		
		m_editChangRateUp.SetFocus();
	}
}

void CFilterSetWnd::OnBtnChangeValChk(UINT nID)
{
	
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

	OnPF_Contorl_Move();
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

	OnPF_Contorl_Move();
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

	OnPF_Contorl_Move();
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

	OnPF_Contorl_Move();
}

void CFilterSetWnd::OnBtnSetDealVolTime()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CRect rePos;
	GetWindowRect(rePos);
	CSubFilterDlg dlgSubFilter(this, rePos, "VOL", &m_listDealTimeVol, GetParent());
	if (dlgSubFilter.DoModal() == IDOK)
	{
		
	}

	AfxSetResourceHandle(hInstSave);
}

void CFilterSetWnd::OnBtnSetDealPriceTime()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CRect rePos;
	GetWindowRect(rePos);
	CSubFilterDlg dlgSubFilter(this, rePos, "PRICE", &m_listDealTimePrice, GetParent());
	if (dlgSubFilter.DoModal() == IDOK)
	{
		
	}

	AfxSetResourceHandle(hInstSave);
}

void CFilterSetWnd::OnBtnNewsRadio(UINT nID)
{
	for (int nIndex = 0; nIndex < _countof(TITLE_NEWS); nIndex++)
	{
		CButton *pBtn = (CButton*)GetDlgItem(IDC_BTN_NEWSRADIO + nIndex);
		if ((int)nID == IDC_BTN_NEWSRADIO + nIndex)
		{
			pBtn->SetCheck(TRUE);
			m_stFilterInfo.nNewsType = nIndex;
		}
		else
			pBtn->SetCheck(FALSE);
	}
}

void CFilterSetWnd::OnBtnDirectionChk(UINT nID)
{
	
}

void CFilterSetWnd::OnBtnCodeAdd()
{
	SetAdd_OutCode();
}

void CFilterSetWnd::SetAdd_OutCode()
{
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)GetParent();

	CString strData, strCode, strName;
	if (m_pctrlJMCombo == NULL)
		return;
	strCode = m_pctrlJMCombo->GetCode();
	
	strCode.TrimRight(" ");
	if (strCode.IsEmpty() == TRUE)
		return;

	char szBuf[16];
	sprintf(szBuf, "%s ", strCode);
	strName = (char*)pMainFrmWnd->GetModuleMain()->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)szBuf);
	strName.TrimLeft();
	strName.TrimRight();
	
	BOOL bAdd = TRUE;
	for (int nIndex = 0; nIndex < m_saOutCode.GetSize(); nIndex++)
	{
		strData = m_saOutCode.GetAt(nIndex);
		if (strCode == strData.Left((int)CODE_LEN))
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
			m_gridOutCode.SetAdd_Row(strCode, strName, TRUE);
			m_saOutCode.Add(strCode + "/" + strName);
		}
	}
}

void CFilterSetWnd::OnBtnCodeDelete()
{
	CString strCode, strName, strData;
	m_gridOutCode.GetData_Row(m_gridOutCode.m_nLastSelRow, strCode, strName);
	
	for (int nIndex = 0; nIndex < m_saOutCode.GetSize(); nIndex++)
	{
		strData = m_saOutCode.GetAt(nIndex);
		if (strCode == strData.Left((int)CODE_LEN))
		{
			m_saOutCode.RemoveAt(nIndex);
			break;
		}
	}
	
	m_gridOutCode.SetDelete_Row(m_gridOutCode.m_nLastSelRow);
}

void CFilterSetWnd::OnBtnCodeDeleteAll()
{
	m_gridOutCode.ClearAll();
	m_saOutCode.RemoveAll();
}

void CFilterSetWnd::OnChkSound()
{
	m_bchksound = m_chkSound.GetCheck();
	m_editSound.EnableWindow(m_bchksound);	
	m_btnSound.EnableWindow(m_bchksound);
	m_btnSearchSound.EnableWindow(m_bchksound);

	CString file, strFileName; 
	file = m_strPathSound;
	GETPATH_APP(strAppPath);
	int nPos = strAppPath.ReverseFind('\\');
	file = strAppPath.Left(nPos);
	file = file +  "\\image\\alarm.wav";
	m_editSound.SetWindowText(file);
	m_strPathSound = file;

	GetDir_DrfnRoot(strAppPath);
	strFileName = strAppPath + "\\user\\" + "soundSet.ini";
	WritePrivateProfileString(_T("ALARMSET"), "alarm", m_strPathSound, strFileName);
}

void CFilterSetWnd::OnBtnSearchsound() 
{
	CString file, fileFullName, fileName, fileExt;
	file = m_strPathSound;
	
	if(file == "")
	{
		GETPATH_APP(strAppPath);
		int nPos = strAppPath.ReverseFind('\\');
		file = strAppPath.Left(nPos);
		file = file +  "\\image\\alarm.wav";
	}
	
	if(file != "")
	{		
		int nTot = file.GetLength();
		int nPos = file.ReverseFind('\\');
		fileFullName = file.Right( nTot - (nPos + 1) );
		nPos = fileFullName.ReverseFind('.');
		fileExt = fileFullName.Right(fileFullName.GetLength() - (nPos + 1));
		fileName = fileFullName.Left(nPos);
	}
	
	if(fileFullName.GetLength() <= 3 || fileFullName == "" )
		fileFullName = "alarm.wav";	
	
	char szFilter[] = "Sound Files(*.wav)|*.wav|All Files(*.*)|*.*||";
	CFileDialog fileDlg(TRUE, "wav", file, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);	
	
	if(IDOK == fileDlg.DoModal())
	{
		file = fileDlg.GetPathName();
		m_editSound.SetWindowText(file);		
		m_strPathSound = file;
		CString strAppPath, strFileName;
		GetDir_DrfnRoot(strAppPath);
		strFileName = strAppPath + "\\user\\" + "soundSet.ini";
		WritePrivateProfileString(_T("ALARMSET"), fileName, m_strPathSound, strFileName);
	}	
}

void CFilterSetWnd::OnBtnSound() 
{	
	BOOL bOK = FALSE;
	CString strFilePath;
	m_editSound.GetWindowText(strFilePath);
	
	
	if(strFilePath.GetLength() > 0)
		bOK = sndPlaySound(strFilePath, SND_ASYNC );
}

void CFilterSetWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect reClient, reLeft;
	GetClientRect(reClient);
	
	reClient.top += 7;
	reClient.bottom -= 11;
	
	reLeft = reClient;
	
	reLeft.right = reLeft.left + 280;
	OnSize_Left(reLeft);
	
	reLeft.left = reLeft.right + 5;
	reLeft.right = reClient.right;
	OnSize_Right(reLeft);

	OnPF_Contorl_Move();
}

void CFilterSetWnd::OnSize_Left(CRect reClient)
{
	int nIndex;
	CRect reTop, reTemp, reLine, reLeft;
	reTop = reClient;
	
	reTop.bottom = reTop.top + 11;
	reTemp = reTop;
	
	reTemp.right = reTemp.left + 3;
	m_ctrlLeftTitlePoint.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 6;
	reTemp.right = reTop.right;
	m_ctrlTitle_In.MoveWindow(&reTemp);
	
	int nWidth_Btn = 70;
	
	reTop.top = reTop.bottom + 5;
	reTop.bottom = reTop.top + 71;
	m_btnInCodeGroup.MoveWindow(&reTop);
	
	reLine = reTop;
	reLine.top += 15;
	reLine.bottom -= 5;
	reLine.left += 5;
	
	reLine.bottom = reLine.top + 20;
	reTemp = reLine;
	
	reTemp.right = reTemp.left + nWidth_Btn;
	m_btnInCodeRao[0].MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 100;
	reTemp.bottom = reTemp.top + 100;
	m_comboMarket.MoveWindow(&reTemp);
	
	reLine.top = reLine.bottom + 5;
	reLine.bottom = reLine.top + 20;
	reTemp = reLine;
	
	reTemp.right = reTemp.left + nWidth_Btn;
	m_btnInCodeRao[1].MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 150;
	reTemp.bottom = reTemp.top + 100;
	m_comboPortGroup.MoveWindow(&reTemp);
	
	reTop.top = reTop.bottom + 5;
	reTop.bottom = reTop.top + 71;
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
	reTop.bottom = reTop.top + 45;
	m_btnChangGroup.MoveWindow(&reTop);
	
	reTemp = reTop;
	reTemp.top += 15;
	reTemp.bottom -= 5;
	reTemp.left += 5;
	
	nWidth_Btn = 45;
	for (nIndex = 0; nIndex < _countof(TITLE_CHANGEVAL); nIndex++)
	{
		if (nIndex == 0)
			reTemp.left += 10;
		else
			reTemp.left += nWidth_Btn + 5;
		reTemp.right = reTemp.left + nWidth_Btn;
		m_btnChangValChk[nIndex].MoveWindow(&reTemp);
	}
	
	reTop.top = reTop.bottom + 5;
	reTop.bottom = reTop.top + 69;
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
	
	reTop.top = reTop.bottom + 5;
	reTop.bottom = reTop.top + 69;
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
	reLeft.right = reLeft.left + 84;
	reTemp = reLeft;
	reTemp.top -= 1;
	reTemp.bottom = reTemp.top + 20;
	m_btnSetDealPriceTime.MoveWindow(&reTemp);
}

void CFilterSetWnd::OnSize_Right(CRect reClient)
{
	int nIndex;
	CRect reTop, reLeft, reTemp, reLine;
	reTop = reClient;
	
	reTop.bottom = reTop.top + 38;
	m_btnNewsGroup.MoveWindow(&reTop);
	
	reTemp = reTop;
// 	reTemp.top += 15;
// 	reTemp.bottom -= 5;
	
	int nWidth_Btn = 70;
	for (nIndex = 0; nIndex < _countof(TITLE_NEWS); nIndex++)
	{
		if (nIndex == 0)
			reTemp.left += 10;
		else
			reTemp.left += nWidth_Btn + 5;
		reTemp.right = reTemp.left + nWidth_Btn;
		m_btnNewsRao[nIndex].MoveWindow(&reTemp);
	}
	
// 	reTop.top = reTop.bottom + 8;
 	reTop.bottom = reTop.top + 11;
	reTemp = reTop;
	
	reTemp.right = reTemp.left + 3;
	m_ctrlRightTitlePoint.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 6;
	reTemp.right = reTop.right;
	m_ctrlTitle_Out.MoveWindow(&reTemp);
	
	reTop.top = reTop.bottom + 5;
	reTop.bottom = reClient.bottom - 30;
	
	reLeft = reTop;
	reLeft.right = reLeft.left + 110;
	m_btnDirectionGroup.MoveWindow(&reLeft);
	
	reTemp = reLeft;
	reTemp.top += 18;
	reTemp.left += 10;
	reTemp.right -= 3;
	for (int nItem = 0; nItem < _countof(TITLE_DIRECTION); nItem++)
	{
		if (nItem > 0)
			reTemp.top = reTemp.bottom + 3;
		//reTemp.bottom = reTemp.top + 22;
		reTemp.bottom = reTemp.top + 18;
		m_btnDirectionChk[nItem].MoveWindow(&reTemp);
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
	reLine.right = reLine.left + 120;
	
	if (m_pctrlCodeInput != NULL)
	{
		CRect reCodeInput;
		reCodeInput = reLine;
		reCodeInput.top += 25;
		reCodeInput.bottom = reCodeInput.top + 20;
		
		reCodeInput.left += 10;
		reCodeInput.right = reCodeInput.left + 84;
		
		m_pctrlCodeInput->MoveWindow(&reCodeInput);
	}
	
	
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
	reLine.right = reLine.left + 36;
	m_btnCodeAdd.MoveWindow(&reLine);
	
	reLine.left = reLine.right + 2;
	reLine.right = reLine.left + 36;
	m_btnCodeDelete.MoveWindow(&reLine);
	
	reLine.left = reLine.right + 2;
	reLine.right = reLine.left + 60;
	m_btnCodeDeleteAll.MoveWindow(&reLine);
	
	reTemp.top = reTemp.bottom + 2;
	reTemp.bottom = reClient.bottom - 36;
	m_gridOutCode.MoveWindow(&reTemp);

 	reTemp.left = reTemp.right - 337;
 	reTemp.right = reTemp.left + 345;
	reTemp.top = reTemp.bottom + 10;
	reTemp.bottom = reClient.bottom + 10;
	m_btnSoundGroup.MoveWindow(&reTemp);

	reTemp = reTop;
	reTemp.top += 315;
	reTemp.left += 10;
	
	reLine = reTemp;
	reLine.bottom = reLine.top + 20;
	reLeft = reLine;
	reLeft.right = reLeft.left + 66;
	m_chkSound.MoveWindow(&reLeft);

	reTemp.left += 70;
	reLine = reTemp;

	reLine.bottom = reLine.top + 18;
	reLeft = reLine;
	reTemp.left = reTemp.right - 20;
	reLeft.right = reLeft.left + 180;
	m_editSound.MoveWindow(&reLeft);

	
	reTemp.top = reTemp.bottom + 15;
	reTemp.bottom = reTemp.top + 20;
	reLine.left = reLine.right - 80;
	reLine.right = reLine.left + 40;
	m_btnSound.MoveWindow(&reLine);

	reLine.left = reLine.right + 0;
	reLine.right = reLine.left + 30;
	m_btnSearchSound.MoveWindow(&reLine);

}

int CFilterSetWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	return pwndMainFrm->SetMessageBox(lpszText, lpszCaption, nType);
}

HBRUSH CFilterSetWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if ((pWnd->GetDlgCtrlID() == IDC_SIATIC_SUBTITLEPOINT) ||
			(pWnd->GetDlgCtrlID() == IDC_SIATIC_SUBTITLEPOINT + 1))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(COLOR_SUBTITLE_POINT);
			hbr = m_brushSubTitlePoint;
		}
		else
		{
			CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(COLOR_SETWND_FACE);
			//hbr = m_brushStatic;
			hbr = pwndMainFrm->GetAxBrush(pwndMainFrm->GetAxColor(66));
		}
	}
	else if (nCtlColor == CTLCOLOR_BTN)
	{
		CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();

		pDC->SetBkMode(TRANSPARENT);
		//hbr = m_brushStatic;
		hbr = pwndMainFrm->GetAxBrush(pwndMainFrm->GetAxColor(66));
	}
	
	return hbr;
}

void CFilterSetWnd::OnPaint() 
{
	CPaintDC dc(this);
}

void CFilterSetWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
}

BOOL CFilterSetWnd::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN) 
	{
		if (pMsg->wParam == VK_TAB)
		{
			if (pMsg->hwnd == GetDlgItem(IDC_BTN_INCODERADIO)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_INCODERADIO + 1)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_INCODERADIO + 1)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_INCODERADIO + 2)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_INCODERADIO + 2)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_INCODERADIO)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_EDIT_NOWPRICEUP)->GetSafeHwnd())
				GetDlgItem(IDC_EDIT_NOWPRICEDOWN)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_EDIT_NOWPRICEDOWN)->GetSafeHwnd())
				GetDlgItem(IDC_EDIT_NOWPRICEUP)->SetFocus();
			
			else if (pMsg->hwnd == GetDlgItem(IDC_EDIT_CHANGERATEUP)->GetSafeHwnd())
				GetDlgItem(IDC_EDIT_CHANGERATEDOWN)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_EDIT_CHANGERATEDOWN)->GetSafeHwnd())
				GetDlgItem(IDC_EDIT_CHANGERATEUP)->SetFocus();
			
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_CHANGEVALCHK)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_CHANGEVALCHK + 1)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_CHANGEVALCHK + 1)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_CHANGEVALCHK + 2)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_CHANGEVALCHK + 2)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_CHANGEVALCHK + 3)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_CHANGEVALCHK + 3)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_CHANGEVALCHK + 4)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_CHANGEVALCHK + 4)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_CHANGEVALCHK)->SetFocus();
			
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DEALVOLRADIO)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_DEALVOLTIMERADIO)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DEALVOLTIMERADIO)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_DEALVOLRADIO)->SetFocus();
			
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DEALPRICERADIO)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_DEALPRICETIMERADIO)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DEALPRICETIMERADIO)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_DEALPRICERADIO)->SetFocus();
			
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_NEWSRADIO)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_NEWSRADIO + 1)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_NEWSRADIO + 1)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_NEWSRADIO + 2)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_NEWSRADIO + 2)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_NEWSRADIO)->SetFocus();
			
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_DIRECTIONCHK + 1)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 1)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_DIRECTIONCHK + 2)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 2)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_DIRECTIONCHK + 3)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 3)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_DIRECTIONCHK + 4)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 4)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_DIRECTIONCHK + 5)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 5)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_DIRECTIONCHK + 6)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 6)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_DIRECTIONCHK + 7)->SetFocus();
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 7)->GetSafeHwnd())
				GetDlgItem(IDC_BTN_DIRECTIONCHK)->SetFocus();
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			if (pMsg->hwnd == GetDlgItem(IDC_BTN_CHANGEVALCHK)->GetSafeHwnd())
				((CButton*)GetDlgItem(IDC_BTN_CHANGEVALCHK))->SetCheck(!((CButton*)GetDlgItem(IDC_BTN_CHANGEVALCHK))->GetCheck());
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_CHANGEVALCHK + 1)->GetSafeHwnd())
				((CButton*)GetDlgItem(IDC_BTN_CHANGEVALCHK + 1))->SetCheck(!((CButton*)GetDlgItem(IDC_BTN_CHANGEVALCHK + 1))->GetCheck());
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_CHANGEVALCHK + 2)->GetSafeHwnd())
				((CButton*)GetDlgItem(IDC_BTN_CHANGEVALCHK + 2))->SetCheck(!((CButton*)GetDlgItem(IDC_BTN_CHANGEVALCHK + 2))->GetCheck());
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_CHANGEVALCHK + 3)->GetSafeHwnd())
				((CButton*)GetDlgItem(IDC_BTN_CHANGEVALCHK + 3))->SetCheck(!((CButton*)GetDlgItem(IDC_BTN_CHANGEVALCHK + 3))->GetCheck());
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_CHANGEVALCHK + 4)->GetSafeHwnd())
				((CButton*)GetDlgItem(IDC_BTN_CHANGEVALCHK + 4))->SetCheck(!((CButton*)GetDlgItem(IDC_BTN_CHANGEVALCHK + 4))->GetCheck());
			
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK)->GetSafeHwnd())
				((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK))->SetCheck(!((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK))->GetCheck());
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 1)->GetSafeHwnd())
				((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 1))->SetCheck(!((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 1))->GetCheck());
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 2)->GetSafeHwnd())
				((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 2))->SetCheck(!((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 2))->GetCheck());
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 3)->GetSafeHwnd())
				((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 3))->SetCheck(!((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 3))->GetCheck());
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 4)->GetSafeHwnd())
				((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 4))->SetCheck(!((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 4))->GetCheck());
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 5)->GetSafeHwnd())
				((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 5))->SetCheck(!((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 5))->GetCheck());
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 6)->GetSafeHwnd())
				((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 6))->SetCheck(!((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 6))->GetCheck());
			else if (pMsg->hwnd == GetDlgItem(IDC_BTN_DIRECTIONCHK + 7)->GetSafeHwnd())
				((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 7))->SetCheck(!((CButton*)GetDlgItem(IDC_BTN_DIRECTIONCHK + 7))->GetCheck());
		}
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CFilterSetWnd::SetPosCodeInput()
{
	CRect rcTemp;
	m_btnOutCodeGroup.GetWindowRect(rcTemp);
	
	ScreenToClient(rcTemp);
	rcTemp.top += 43;
	rcTemp.bottom = rcTemp.top + 20;
	
	rcTemp.left += 134;
	rcTemp.right = rcTemp.left + 90;
	//m_pctrlCodeInput->MoveWindow(&rcTemp);
}

void CFilterSetWnd::OnPF_Contorl_Move()
{
	if(!GetParent()) return;
	
	ST_MOVEITEM stItem;	
	stItem.nType = PF_ITEM_BTN;	
	
	SET_ST_MOVEITEM(stItem, "btnAddEx", &m_btnCodeAdd);	
	GetParent()->SendMessage(MF_PFCONTORL_MOVE, m_bShow_PF_CTL, (LPARAM)&stItem);
	
	SET_ST_MOVEITEM(stItem, "btnRemoveAllEx", &m_btnCodeDeleteAll);
	GetParent()->SendMessage(MF_PFCONTORL_MOVE, m_bShow_PF_CTL, (LPARAM)&stItem);
	
	SET_ST_MOVEITEM(stItem, "btnRemoveEx", &m_btnCodeDelete);
	GetParent()->SendMessage(MF_PFCONTORL_MOVE, m_bShow_PF_CTL, (LPARAM)&stItem);

	SET_ST_MOVEITEM(stItem, "btnVolTime", &m_btnSetDealVolTime);
	GetParent()->SendMessage(MF_PFCONTORL_MOVE, m_bShow_PF_CTL, (LPARAM)&stItem);

	SET_ST_MOVEITEM(stItem, "btnPriceTime", &m_btnSetDealPriceTime);
	GetParent()->SendMessage(MF_PFCONTORL_MOVE, m_bShow_PF_CTL, (LPARAM)&stItem);
}

void CFilterSetWnd::PlatFormBtnAddEx()
{
	OnBtnCodeAdd();
}
void CFilterSetWnd::PlatFormBtnRemoveEx()
{
	OnBtnCodeDelete();
}
void CFilterSetWnd::PlatFormBtnRemoveAllEx()
{
	OnBtnCodeDeleteAll();
}
void CFilterSetWnd::PlatFormBtnVolTime()
{
	OnBtnSetDealVolTime();
}
void CFilterSetWnd::PlatFormBtnPriceTime()
{
	OnBtnSetDealPriceTime();
}

BOOL CFilterSetWnd::OnEraseBkgnd(CDC* pDC)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();

	CRect reClient;
	GetClientRect(reClient);
	//pDC->FillSolidRect(reClient, COLOR_SETWND_FACE);
	pDC->FillSolidRect(reClient, pwndMainFrm->GetAxColor(66));
	m_pctrlJMCombo->Invalidate();
	return FALSE;
}

void CFilterSetWnd::ChangeSkinColor(COLORREF clrBkgnd)
{
	if(!clrBkgnd)
		return;
	
	m_clrMainSkin = clrBkgnd;
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();

	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = pwndMainFrm->GetAxColor(74);
	stGridColor.clrGridHilight = pwndMainFrm->GetAxColor(78);
	
	ChangeGridColor(stGridColor);

	Invalidate();
}

void CFilterSetWnd::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	m_gridOutCode.ChangeGridColor(stGridColor);
	m_gridOutCode.Invalidate();
}