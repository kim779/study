// RealPacketPage.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "RealPacketPage.h"

#include <afxpriv.h>
#include "../Include_Chart/xUtil_Chart.h"		// for CHART_CONVERT_BEGIN
#include "../Include_Chart/IPropertyMap.h"		// for IPropertyMap
#include "ChartPropertySheet.h"
#include "PropertiesData.h"
#include "Conversion.h"

#include "Realinfo.h"
#include "../include/SharedBase.h"		// for DE_SHARDGET

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealPacketPage property page

IMPLEMENT_DYNCREATE(CRealPacketPage, CPropertyPage)

CRealPacketPage::CRealPacketPage() : CPropertyPage(CRealPacketPage::IDD)
{
	//{{AFX_DATA_INIT(CRealPacketPage)
	m_sLocalSharedList = _T("");
	m_szRealList = _T("");
	m_strPacket = _T("");
	m_szKeyName = _T("");
	m_bUseRealData = FALSE;
	m_nPacketSize = 2048;
	m_bFireRealEvent = FALSE;
	//}}AFX_DATA_INIT
	m_bRedrawInCaseOfRealApplyToAllPacket = TRUE;

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = NULL;

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;
}

CRealPacketPage::~CRealPacketPage()
{
}

void CRealPacketPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealPacketPage)
	DDX_Control(pDX, IDC_COMBO_PACKETLIST, m_comboPacketList);
	DDX_Control(pDX, IDC_COMBO_BASEPACKET, m_comboBasePacket);
	DDX_Control(pDX, IDC_REAL_LIST, m_RealDataEdit);
	DDX_Control(pDX, IDC_CMB_SHAREDNAME, m_cmbSharedName);
	DDX_Control(pDX, IDC_ADVISE_LIST, m_lsAdviseList);
	DDX_Text(pDX, IDC_SHARE_LIST, m_sLocalSharedList);
	DDX_Text(pDX, IDC_REAL_LIST, m_szRealList);
	DDX_CBString(pDX, IDC_PACKET_TREE, m_strPacket);
	DDX_CBString(pDX, IDC_CMB_SHAREDNAME, m_szKeyName);
	DDX_Check(pDX, IDC_USE_REALDATA, m_bUseRealData);
	DDX_Text(pDX, IDC_PACKETSIZE, m_nPacketSize);
	DDX_Check(pDX, IDC_REDRAWINCASEOFAPPLYINGALLPACKET, m_bRedrawInCaseOfRealApplyToAllPacket);
	DDX_Check(pDX, IDC_CHECK_FIREEVENT, m_bFireRealEvent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRealPacketPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRealPacketPage)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_CBN_SELCHANGE(IDC_CMB_SHAREDNAME, OnSelchangeCmbSharedname)
	ON_CBN_SELCHANGE(IDC_PACKET_TREE, OnSelchangePacketTree)
	ON_CBN_SELENDOK(IDC_PACKET_TREE, OnSelendokPacketTree)
	ON_CBN_SELCHANGE(IDC_COMBO_BASEPACKET, OnSelchangeComboBasepacket)
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBtnEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealPacketPage message handlers
BOOL CRealPacketPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CString szDesignPath;
	char tmp1[128],tmp2[128],tmp3[128];
	::GetPrivateProfileString("CURRENT","WORK","",tmp1,sizeof(tmp1),"Hanaro.ini");
	::GetPrivateProfileString(tmp1,"PATH","",tmp2,sizeof(tmp2),"Hanaro.ini");
	::GetPrivateProfileString(tmp1,"CFGNAME","",tmp3,sizeof(tmp3),"Hanaro.ini");
 	szDesignPath.Format("%s\\bin\\%s",tmp2,tmp3);

	// (2006/10/15 - Seung-Won, Bae) Enable UI in Only DRDS CFG site.
	if( szDesignPath != "\\bin\\")
	{
		m_TreeCombo.SubclassDlgItem(IDC_PACKET_TREE, this);
		m_TreeCombo.SubclassDlgItem(NULL,this);
		m_TreeCombo.LoadTableInfo(szDesignPath);

		InitList();
	}
	else
	{
		m_comboPacketList.EnableWindow( FALSE);
		m_lsAdviseList.EnableWindow( FALSE);
		m_cmbSharedName.EnableWindow( FALSE);
		GetDlgItem( IDC_PACKET_TREE)->EnableWindow( FALSE);
		GetDlgItem( IDC_BTN_EDIT)->EnableWindow( FALSE);
		GetDlgItem( IDC_BTN_ADD)->EnableWindow( FALSE);
		GetDlgItem( IDC_BTN_DEL)->EnableWindow( FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRealPacketPage::InitList()
{
	DWORD dwNewStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_SORTDESCENDING | LVS_EX_HEADERDRAGDROP ;
	m_lsAdviseList.SetExtendedStyle(dwNewStyle);

	CRect rc;
	m_lsAdviseList.GetWindowRect(&rc);

	m_lsAdviseList.InsertColumn(0,"Chart Packet",LVCFMT_LEFT, 100);							
	m_lsAdviseList.InsertColumn(1,"Packet Name ",LVCFMT_LEFT, rc.Width()/4);    // Packet Name
	m_lsAdviseList.InsertColumn(2,"Field Name",LVCFMT_LEFT, 100);				// Field Name
	m_lsAdviseList.InsertColumn(3,"Key Name",LVCFMT_LEFT, 100 );		// Set
}

//"Packet1.S1.code.aa;Packet2.S1.time.bb;" 라고 들어왔을때 
//list의 첫번째 Item에 1, SubItem1에 "Packet1", SubItem2에 "S1", SubItem3에 "code", SubItem3에 "aa",
//두번째 Item에 2, SubItem1에 "Packet2", SubItem2에 "S1", SubItem3에 "time", SubItem3에 "bb"을 Insert.
void CRealPacketPage::Init_AdviseList()
{
	m_lsAdviseList.DeleteAllItems();

	CString szChartPacket, szPacket, szField, szKey;
	CString szAdviseList, szOneItem, szItem;

	szAdviseList = m_szRealList;
	szOneItem = CDataConversion::GetStringData(szAdviseList, ";");
	int i = 0;
	while(szOneItem != "") {
		szItem = CDataConversion::GetStringData(szOneItem, ".");
		m_lsAdviseList.InsertItem(i, szItem);
		szItem = CDataConversion::GetStringData(szOneItem, ".");
		m_lsAdviseList.SetItemText(i, 1, szItem);
		szItem = CDataConversion::GetStringData(szOneItem, ".");
		m_lsAdviseList.SetItemText(i, 2, szItem);
		m_lsAdviseList.SetItemText(i++, 3, szOneItem);
		szOneItem = CDataConversion::GetStringData(szAdviseList, ";");
	}
}

//list에 Item, subItem1, SubItem2, SubItem3, SubItem4를 뿌리기.
int CRealPacketPage::AddData(CString ChartPacket, CString packet, CString field, CString key)
{
	int nIndex = m_lsAdviseList.GetItemCount();

	m_lsAdviseList.InsertItem(nIndex, ChartPacket); 
	m_lsAdviseList.SetItemText(nIndex, 1, packet);
	m_lsAdviseList.SetItemText(nIndex, 2, field);
	m_lsAdviseList.SetItemText(nIndex, 3, key);

	return 1;
}

////////////////////////////////////////////////////////////////////
// Data Tree...

//RealpacketPage에서 두번째 콤보박스 채우기.
//SharedDataPage에서 선택했던 Item들을 콤보박스에 넣기.
int CRealPacketPage::InitSharedListCombo(CComboBox* pCombo,CString szSharedList)
{
	pCombo->ResetContent();
	szSharedList.TrimRight();
	if(szSharedList.GetLength()==0)		return 0;

	szSharedList += ";";
	int nIndex = szSharedList.Find(";");
	CString szShareName;
	BOOL   bGet, bSet, bStart;
	char   aTmp[2]={0,};
	int		nLen, nCnt=0, nTmp;

	while(nIndex>0)
	{
		bGet = bSet = bStart = FALSE;
		szShareName = szSharedList.Left(nIndex);
		szSharedList = szSharedList.Mid(nIndex+1);

		nLen = szShareName.GetLength();
		aTmp[0] = szShareName[nLen-1];
		szShareName = szShareName.Left(nLen-1);

		nTmp = atoi(aTmp);
		if(nTmp & 1) bStart = TRUE;
		if(nTmp & 4) bGet=TRUE;

		if(bGet || bStart)
		{
			int nIndex=0;
			if((nIndex=pCombo->FindString(nIndex, szShareName)) == LB_ERR)
			{
				pCombo->AddString(szShareName);
			}
		}
		nCnt++;

		nIndex = szSharedList.Find(";");
	}

	pCombo->SetCurSel(0);
	return nCnt;
}

//list에 입력되어 있는 Item들을 하나씩 얻어서 하나의 Edit박스에 뿌리기.
void CRealPacketPage::MakeRealList()
{
	CString szList;
	CString szChartPacket, szPacket, szField, szKey;

	int nCount = m_lsAdviseList.GetItemCount();
	for(int i=0; i < nCount; i++) {
		szChartPacket = m_lsAdviseList.GetItemText(i, 0);
		szPacket = m_lsAdviseList.GetItemText(i, 1);
		szField = m_lsAdviseList.GetItemText(i, 2);
		szKey = m_lsAdviseList.GetItemText(i, 3);

		CString szTmp;
		szTmp.Format("%s.%s.%s.%s;", szChartPacket, szPacket, szField, szKey);
		szList += szTmp;
	}

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_REAL_LIST);
	if(!pEdit) return;

	CString szText;
	pEdit->GetWindowText(szText);

	if(szText.Compare(szList)) {
		m_szRealList = szList;
		pEdit->SetWindowText(m_szRealList);
	}
}

void CRealPacketPage::OnBtnAdd() 
{
	CString ChartPacket, Packet, Field, KeyName;
	CString szOneAdvise = m_szOneAdvise;

	m_comboPacketList.GetLBText(m_comboPacketList.GetCurSel(), ChartPacket);
	m_cmbSharedName.GetLBText(m_cmbSharedName.GetCurSel(), KeyName);

	if(szOneAdvise == "" || ChartPacket == "" || KeyName == "")
		return;

	Packet = CDataConversion::GetStringData(szOneAdvise, ".");

	AddData(ChartPacket, Packet, szOneAdvise, KeyName);
	MakeRealList();
}

void CRealPacketPage::OnBtnEdit() 
{
	int nIndex = GetSelectItem();
	
	if(nIndex < 0) return;

	CString ChartPacket, Packet, Field, KeyName;
	CString szOneAdvise = m_szOneAdvise;

	m_comboPacketList.GetLBText(m_comboPacketList.GetCurSel(), ChartPacket);
	m_cmbSharedName.GetLBText(m_cmbSharedName.GetCurSel(), KeyName);

	if(szOneAdvise == "" || ChartPacket == "" || KeyName == "")
		return;

	Packet = CDataConversion::GetStringData(szOneAdvise, ".");
	
	m_lsAdviseList.SetItemText(nIndex, 0, ChartPacket);
	m_lsAdviseList.SetItemText(nIndex, 1, Packet);
	m_lsAdviseList.SetItemText(nIndex, 2, szOneAdvise);
	m_lsAdviseList.SetItemText(nIndex, 3, KeyName);

	MakeRealList();
}

void CRealPacketPage::OnBtnDel() 
{
	int nIndex = GetSelectItem();

	if(nIndex < 0) return;

	m_lsAdviseList.DeleteItem(nIndex);

	MakeRealList();
}

int CRealPacketPage::GetSelectItem()
{
	POSITION pos = m_lsAdviseList.GetFirstSelectedItemPosition();
	if(pos) 
	{
		int nItem = m_lsAdviseList.GetNextSelectedItem(pos);
		return nItem;
	}
	return -1;
}

void CRealPacketPage::OnSelchangeCmbSharedname() 
{
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_CMB_SHAREDNAME);
	int nIndex = pCombo->GetCurSel();

	m_cmbSharedName.GetLBText(nIndex,m_szKeyName);
}

void CRealPacketPage::OnSelchangePacketTree() 
{
	m_szOneAdvise = m_TreeCombo.GetCurrentTreePath();
}


void CRealPacketPage::OnSelendokPacketTree() 
{
	m_szOneAdvise = m_TreeCombo.GetCurrentTreePath();
}

BOOL CRealPacketPage::OnSetActive() 
{
	Init_AdviseList();

	InitSharedListCombo(&m_cmbSharedName, m_sLocalSharedList);
	InitBasePacketCombo();
	InitPacketListCombo();

	return CPropertyPage::OnSetActive();
}
//SharedDataPage에서 설정한 data를 받아서 멤버변수에 Set.
void CRealPacketPage::SetKeyName(CString keyname)
{
	m_sLocalSharedList = keyname;
}

CString CRealPacketPage::GetSavedRealPacketInfo()
{
	return m_szRealList;
}

void CRealPacketPage::SetRealPacketInfo(CString szRealPacket)
{
	m_szRealPackets = szRealPacket;
}

BOOL CRealPacketPage::OnKillActive() 
{
	MakeRealList();	
	return CPropertyPage::OnKillActive();
}

void CRealPacketPage::InitBasePacketCombo()
{
	m_comboBasePacket.ResetContent();

	if(m_szRealPackets == "")
		return;

	CString szPacketName, szRealPackets = m_szRealPackets;
	szPacketName = CDataConversion::GetStringData(szRealPackets, ";");
	while(szPacketName != "") {
		m_comboBasePacket.AddString( _MTOL0( szPacketName));
		szPacketName = CDataConversion::GetStringData(szRealPackets, ";");
	}

	int nIndex = -1;
	if(m_szBasePacketName.IsEmpty())
		nIndex = m_comboBasePacket.FindString(-1, _LTEXT0( C0_DATE_TIME));
	else
		nIndex = m_comboBasePacket.FindString(-1, _MTOL0( m_szBasePacketName));

	//sy 2004.04.26. - 잘못된 기준 packet일 경우는 잘못된 값으로 보여줘야 한다.
	if(nIndex < 0)
	{
		ML_SET_LANGUAGE_RES();
		CString strMsg;
		strMsg.LoadString( IDS_INVALID_BASE_PACKET);
		AfxMessageBox( strMsg, MB_ICONWARNING|MB_OK);
		m_comboBasePacket.SetCurSel(0);
		m_comboBasePacket.GetLBText(0, m_szBasePacketName);
	}
	else
		m_comboBasePacket.SetCurSel(nIndex);

	if(CB_ERR != m_comboBasePacket.GetCurSel())
		m_comboBasePacket.GetLBText(m_comboBasePacket.GetCurSel(), m_szBasePacketName);

	m_szBasePacketName = _LTOM0( m_szBasePacketName);
}

void CRealPacketPage::InitPacketListCombo()
{
	m_comboPacketList.ResetContent();

	if(m_szRealPackets == "")
		return;

	CString szPacketName, szRealPackets = m_szRealPackets;
	szPacketName = CDataConversion::GetStringData(szRealPackets, ";");
	while(szPacketName != "") {
		m_comboPacketList.AddString( _MTOL0( szPacketName));
		szPacketName = CDataConversion::GetStringData(szRealPackets, ";");
	}

	m_comboPacketList.SetCurSel(0);
}

void CRealPacketPage::OnSelchangeComboBasepacket() 
{
	m_comboBasePacket.GetLBText(m_comboBasePacket.GetCurSel(), m_szBasePacketName);
	m_szBasePacketName = _LTOM0( m_szBasePacketName);
}


//ocx에서 저장되어 있던 data를 받아서 멤버변수에 Set.
void CRealPacketPage::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_CONVERT_BEGIN()
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRLOCALSHAREDLIST		], m_sLocalSharedList,						_DPV_STRLOCALSHAREDLIST		);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	BUSEREALDATA			], m_bUseRealData,							_DPV_BUSEREALDATA			);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRREALLIST				], m_szRealList,							_DPV_STRREALLIST			);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	nHoldingPacketSize		], m_nPacketSize,							_DPV_nHoldingPacketSize		);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bYesOrNoRedrawbyReal	], m_bRedrawInCaseOfRealApplyToAllPacket,	_DPV_bYesOrNoRedrawbyReal	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRBASEPACKETNAME		], m_szBasePacketName,						_DPV_STRBASEPACKETNAME		);
		CHART_CONVERT_BOOL(		p_pIPropertyMap, g_anPropertyShortKey[	bFIREREALEVENT			], m_bFireRealEvent,						_DPV_bFIREREALEVENT			);
	CHART_CONVERT_END()

	// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
	if( m_szBasePacketName == _DPV_STRBASEPACKETNAME) m_szBasePacketName = _MTEXT( C0_DATE_TIME);

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = p_pIPropertyMap;
}

BOOL CRealPacketPage::OnApply() 
{
	UpdateData(FALSE);

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_ADDTOSTRLIST_START( TRUE)
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	BUSEREALDATA			], m_bUseRealData,							_DPV_BUSEREALDATA			);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	STRREALLIST				], m_szRealList,							_DPV_STRREALLIST			);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	nHoldingPacketSize		], m_nPacketSize,							_DPV_nHoldingPacketSize		);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	bYesOrNoRedrawbyReal	], m_bRedrawInCaseOfRealApplyToAllPacket,	_DPV_bYesOrNoRedrawbyReal	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	STRBASEPACKETNAME		], m_szBasePacketName,						_DPV_STRBASEPACKETNAME		);
		CHART_ADDTOSTRLIST_BOOL(	m_pIPropertyMap, g_anPropertyShortKey[	bFIREREALEVENT			], m_bFireRealEvent,						_DPV_bFIREREALEVENT			);
	CHART_ADDTOSTRLIST_END

	return CPropertyPage::OnApply();
}

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
void CRealPacketPage::SetOcxHwnd( HWND p_hOcxWnd)
{
	m_hOcxWnd = p_hOcxWnd;
	m_TreeCombo.SetOcxHwnd( p_hOcxWnd);
}
