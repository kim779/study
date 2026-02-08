// SharedDataPage.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "SharedDataPage.h"

#include "../Include_Chart/xUtil_Chart.h"		// for CHART_CONVERT_BEGIN
#include "../Include_Chart/IPropertyMap.h"		// for IPropertyMap
#include "DlgShareSet.h"
#include "ChartPropertySheet.h"
#include "PropertiesData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSharedDataPage property page

IMPLEMENT_DYNCREATE(CSharedDataPage, CPropertyPage)

CSharedDataPage::CSharedDataPage() : CPropertyPage(CSharedDataPage::IDD)
{
	//{{AFX_DATA_INIT(CSharedDataPage)
	m_sLocalSharedList = _T("");
	m_sGlobalSharedList = _T("");
	//}}AFX_DATA_INIT

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = NULL;

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;
}

CSharedDataPage::~CSharedDataPage()
{
}

void CSharedDataPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSharedDataPage)
	DDX_Control(pDX, IDC_GSHARELIST, m_ctrlGShareList);
	DDX_Control(pDX, IDC_LSHARELIST, m_ctrlLShareList);
	DDX_Text(pDX, IDC_LSHAREDEDIT, m_sLocalSharedList);
	DDX_Text(pDX, IDC_GSHAREDEDIT, m_sGlobalSharedList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSharedDataPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSharedDataPage)
	ON_BN_CLICKED(IDC_BTNADD, OnBtnadd)
	ON_BN_CLICKED(IDC_BTNDEL, OnBtndel)
	ON_BN_CLICKED(IDC_BTNMODIFY, OnBtnmodify)
	ON_BN_CLICKED(IDC_GBTNADD, OnGBtnAdd)
	ON_BN_CLICKED(IDC_GBTNDEL, OnGBtnDel)
	ON_BN_CLICKED(IDC_GBTNMODIFY, OnGBtnModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSharedDataPage message handlers

BOOL CSharedDataPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
//m_ctrlLShareList에 Data넣기..
	InitList(&m_ctrlLShareList, m_sLocalSharedList);	//Global 공유 List초기화..

	// (2004.06.08, 배승원) Global 공유 Property를 추가한다.
	InitList( &m_ctrlGShareList, m_sGlobalSharedList);	//Global 공유 List초기화..

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 리스트컨트롤을 초기화한다.
// (2004.06.08, 배승원) Global 공유 Property를 추가한다.
//		Local/Global의 겸용 초기화 Routine으로 개선한다.
void CSharedDataPage::InitList(CListCtrl* pList, LPCSTR _szData)
{
	ASSERT(pList!=NULL);
	DWORD dwNewStyle = LVS_EX_FULLROWSELECT|LVS_SORTDESCENDING  |LVS_EX_HEADERDRAGDROP ;
	pList->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwNewStyle));

	CRect rc;
	pList->GetWindowRect(&rc);

	pList->InsertColumn(0,"#",LVCFMT_LEFT, 20);   // INDEX
	pList->InsertColumn(1,"ShareName",LVCFMT_LEFT, rc.Width()-180);   // 아이템명
	pList->InsertColumn(2,"Start",LVCFMT_LEFT, 50);		// Start
	pList->InsertColumn(3,"Set",LVCFMT_LEFT, 50 );		// Set
	pList->InsertColumn(4,"Get",LVCFMT_LEFT, 50 );		// Get

	AddData(pList, _szData);
}

int CSharedDataPage::AddData(CListCtrl* pList, CString _szData)
{
	_szData.TrimRight();
	if(_szData.GetLength()==0) return 0;

	_szData += ";";
	int nIndex = _szData.Find(";");
	CString szShareName;
	BOOL   bGet, bSet, bStart;
	char   aTmp[2]={0,};
	int		nLen, nCnt=0, nTmp;

	while(nIndex>0)
	{
		bGet = bSet = bStart = FALSE;
		szShareName = _szData.Left(nIndex);
		_szData = _szData.Mid(nIndex+1);

		nLen = szShareName.GetLength();
		aTmp[0] = szShareName[nLen-1];
		szShareName = szShareName.Left(nLen-1);

		nTmp = atoi(aTmp);
		if(nTmp & 1) bStart = TRUE;
		if(nTmp & 2) bSet=TRUE;
		if(nTmp & 4) bGet=TRUE;

		AddData(pList, szShareName, bStart, bSet, bGet);
		nCnt++;

		nIndex = _szData.Find(";");
	}

	return nCnt;
}

int CSharedDataPage::AddData(CListCtrl* pList, CString _szShareName, BOOL _bStart, BOOL _bSet, BOOL _bGet)
{
	ASSERT(pList!=NULL);
	int nIndex = pList->GetItemCount();

	char tmp[10];
	sprintf(tmp, "%d", nIndex+1);
	int nInsert = pList->InsertItem(nIndex, tmp);	//⊙
	pList->SetItemText(nInsert, 1, _szShareName);

	sprintf(tmp, "%d", _bStart);
	pList->SetItemText(nInsert, 2, tmp);

	sprintf(tmp, "%d", _bSet);
	pList->SetItemText(nInsert, 3, tmp);

	sprintf(tmp, "%d", _bGet);
	pList->SetItemText(nInsert, 4, tmp);

	return 1;
}

void CSharedDataPage::Refresh(CListCtrl* pList)
{
	ASSERT(pList!=NULL);

	int nCount = pList->GetItemCount();
	char tmp[10];
	for(int i=0; i<nCount; i++)
	{
		sprintf(tmp, "%d", i+1); 
		pList->SetItemText(i, 0, tmp);
	}
}

int CSharedDataPage::GetSelectItem(CListCtrl* pList)
{
	ASSERT(pList!=NULL);

	POSITION pos = pList->GetFirstSelectedItemPosition();
	if(pos) 
	{
		int nItem = pList->GetNextSelectedItem(pos);
		return nItem;
	}
	return -1;
}

// (2004.06.08, 배승원) Global 공유 Property를 추가한다.
//		Local/Global의 겸용 초기화 Routine으로 개선한다.
void CSharedDataPage::MakeShareList(CListCtrl* pList, UINT nID)
{
	ASSERT(pList!=NULL);

	CString szList, szShare, szTmp;
	CString sGet, sSet, sStart;
	int  nGet, nSet, nStart;

	int nCount = pList->GetItemCount();
	for(int i=0; i<nCount; i++) 
	{
		szShare = pList->GetItemText(i, 1);
		sStart = pList->GetItemText(i, 2);
		sSet = pList->GetItemText(i, 3);
		sGet = pList->GetItemText(i, 4);

		nStart = atoi(sStart);
		nGet = atoi(sGet);
		nSet = atoi(sSet);
		

		szTmp.Format("%s%d;", szShare, nStart+(nSet*2)+(nGet*4));
		szList += szTmp;
	}

	CEdit* pEdit = (CEdit*)GetDlgItem(nID);
	if(!pEdit) return;

	CString szText;
	pEdit->GetWindowText(szText);

	if(szText.Compare(szList))
		pEdit->SetWindowText(szList);

}

BOOL CSharedDataPage::OnKillActive() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_LSHAREDEDIT);
	pEdit->GetWindowText(m_sLocalSharedList);

//@solomon[sashia]	20090902 --- 기능 축소 --------->>
//	(((CChartPropertySheet *)GetParent())->m_RealPacketPage).SetKeyName(m_sLocalSharedList);
//@solomon[sashia]	20090902 --- 기능 축소 ---------<<

	return CPropertyPage::OnKillActive();
}

// (2004.06.08, 배승원) Global 공유 Property를 추가한다.
//		추가/삭제/수정 기능을 제공한다.
void CSharedDataPage::OnGBtnAdd() 
{
	// TODO: Add your control notification handler code here
	LFnBtnAdd(&m_ctrlGShareList,IDC_GSHAREDEDIT);
}
void CSharedDataPage::OnGBtnDel() 
{
	// TODO: Add your control notification handler code here
	LFnBtnDel(&m_ctrlGShareList,IDC_GSHAREDEDIT);
}
void CSharedDataPage::OnGBtnModify() 
{
	// TODO: Add your control notification handler code here
	LFnBtnModify(&m_ctrlGShareList,IDC_GSHAREDEDIT);
}

// (2004.06.08, 배승원) Global 공유 Property를 추가한다.
//		Local/Global의 겸용 초기화 Routine으로 개선한다.
void CSharedDataPage::OnBtnadd() 
{
	LFnBtnAdd(&m_ctrlLShareList,IDC_LSHAREDEDIT);
}
void CSharedDataPage::OnBtndel() 
{
	LFnBtnDel(&m_ctrlLShareList,IDC_LSHAREDEDIT);
}
void CSharedDataPage::OnBtnmodify() 
{
	LFnBtnModify(&m_ctrlLShareList,IDC_LSHAREDEDIT);
}
void CSharedDataPage::LFnBtnAdd( CListCtrl* pList, UINT nID)
{
	if(!pList) return;

	ML_SET_LANGUAGE_RES();
	CDlgShareSet dlg;
	if(IDOK==dlg.DoModal())
	{
		if(dlg.m_szSharedName.GetLength()>0)
		{
			AddData(pList, dlg.m_szSharedName, dlg.m_bStart, dlg.m_bSet, dlg.m_bGet);
			MakeShareList(pList, nID);
		}
	}
}
void CSharedDataPage::LFnBtnDel(CListCtrl* pList, UINT nID)
{
	if(!pList) return;

	int nItem = GetSelectItem(pList);
	if(nItem>=0)
	{
		pList->DeleteItem(nItem);
		Refresh(pList);
		MakeShareList(pList, nID);
	}	
}
void CSharedDataPage::LFnBtnModify(CListCtrl* pList, UINT nID)
{
	if(!pList) return;

	int nItem = GetSelectItem(pList);
	if(nItem<0)		return;

	ML_SET_LANGUAGE_RES();
	CDlgShareSet dlg;
	CString sStart = pList->GetItemText(nItem, 2);
	CString sSet = pList->GetItemText(nItem, 3);
	CString sGet = pList->GetItemText(nItem, 4);

	dlg.m_szSharedName = pList->GetItemText(nItem, 1);
	
	dlg.m_bStart = atoi(sStart);
	dlg.m_bSet = atoi(sSet);
	dlg.m_bGet = atoi(sGet);
	
	dlg.m_bSName = TRUE;

	if(IDOK==dlg.DoModal())
	{
		UpdateItemData(pList, dlg.m_szSharedName, dlg.m_bStart, dlg.m_bSet, dlg.m_bGet, nItem);
		MakeShareList(pList, nID);
	}	
}
int CSharedDataPage::UpdateItemData(CListCtrl* pList, CString _szShareName, BOOL _bStart, BOOL _bSet, BOOL _bGet, int _nIndex)
{
	ASSERT(pList!=NULL);
	if(_nIndex<0)	return 0;

	char tmp[10];
	pList->SetItemText(_nIndex, 1, _szShareName);

	sprintf(tmp, "%d", _bStart);
	pList->SetItemText(_nIndex, 2, tmp);

	sprintf(tmp, "%d", _bSet);
	pList->SetItemText(_nIndex, 3, tmp);

	sprintf(tmp, "%d", _bGet);
	pList->SetItemText(_nIndex, 4, tmp);

	return 1;
}

void CSharedDataPage::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_CONVERT_BEGIN()
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRLOCALSHAREDLIST	], m_sLocalSharedList,	_DPV_STRLOCALSHAREDLIST		);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRGLOBALSHAREDLIST	], m_sGlobalSharedList,	_DPV_STRGLOBALSHAREDLIST	);	// (2004.06.08, 배승원) Global 공유를 추가한다.
	CHART_CONVERT_END()

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = p_pIPropertyMap;
}

BOOL CSharedDataPage::OnApply() 
{
	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_ADDTOSTRLIST_START( TRUE)
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	STRLOCALSHAREDLIST	], m_sLocalSharedList,	_DPV_STRLOCALSHAREDLIST		);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	STRGLOBALSHAREDLIST	], m_sGlobalSharedList,	_DPV_STRGLOBALSHAREDLIST	);
	CHART_ADDTOSTRLIST_END

	return CPropertyPage::OnApply();
}
