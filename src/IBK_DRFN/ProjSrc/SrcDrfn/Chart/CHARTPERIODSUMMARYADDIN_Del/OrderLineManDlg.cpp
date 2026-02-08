// OrderLineManDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chartperiodsummaryaddin.h"
#include "ChartOrderLineAddInImp.h"
#include "OrderLineManDlg.h"
#include "LineObj.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderLineManDlg dialog


COrderLineManDlg::COrderLineManDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrderLineManDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderLineManDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COrderLineManDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderLineManDlg)
	DDX_Control(pDX, IDC_LIST1, m_ctrlOLList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderLineManDlg, CDialog)
	//{{AFX_MSG_MAP(COrderLineManDlg)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_WM_KEYDOWN()
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, OnKeydownList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderLineManDlg message handlers

BOOL COrderLineManDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// List contol initialization
	DWORD dwNewStyle = m_ctrlOLList.GetExtendedStyle();
	dwNewStyle |= LVS_EX_FULLROWSELECT;
	m_ctrlOLList.SetExtendedStyle(dwNewStyle);
	
	m_ctrlOLList.InsertColumn(0, "NO");
	m_ctrlOLList.InsertColumn(1, "종목코드");
	m_ctrlOLList.InsertColumn(2, "종목명");
	m_ctrlOLList.InsertColumn(3, "기준봉");
	m_ctrlOLList.InsertColumn(4, "개수");
//	m_ctrlOLList.InsertColumn(5, "삭제");
	
	m_ctrlOLList.SetColumnWidth(0, 30);
	m_ctrlOLList.SetColumnWidth(1, 70);
	m_ctrlOLList.SetColumnWidth(2, 69);
	m_ctrlOLList.SetColumnWidth(3, 50);
	m_ctrlOLList.SetColumnWidth(4, 40);
//	m_ctrlOLList.SetColumnWidth(5, 40);


	//////////////////////////////////////////////////////////////////////////
	//	저장된 주문선 정보 얻기
	CString strLineInfo;

	int nCnt = 0;

//	CString strFileName = m_strUserPath + OL_FILE;

//	CFile f;
//	if( !f.Open( strFileName, CFile::modeRead ) ) {
//		  return FALSE;
//	}
//	CArchive ar( &f, CArchive::load);

//	m_pSavedAllOrderLines->Serialize(ar);
	// 리스트에 삽입
	POSITION pos = m_pSavedAllOrderLines->GetHeadPosition();
	while (pos)
	{
		int iStart = 0;
		CString s;
		strLineInfo = m_pSavedAllOrderLines->GetNext(pos);
		strLineInfo += ",";
		CString strSerial = TokenizeEx(strLineInfo, ",", iStart);
		int nIndex = FindListItem(strSerial);
		if(nIndex == -1)
		{
			nIndex = m_ctrlOLList.GetItemCount();
			s.Format("%d",(nIndex+1));
			m_ctrlOLList.InsertItem(nIndex,s);
			nCnt = 1;
		}
		else
		{
			s = m_ctrlOLList.GetItemText(nIndex, 4);
			nCnt = atoi(s) + 1;
		}
		m_ctrlOLList.SetItemText(nIndex, 1, TokenizeEx(strLineInfo, ",", iStart));	// 종목코드
		m_ctrlOLList.SetItemText(nIndex, 2, TokenizeEx(strLineInfo, ",", iStart));	// 종목명
		m_ctrlOLList.SetItemText(nIndex, 3, TokenizeEx(strLineInfo, ",", iStart));	// 기준봉
		s.Format("%d", nCnt);
		m_ctrlOLList.SetItemText(nIndex, 4, s);	// 개수
		m_ctrlOLList.SetItemText(nIndex, 6, TokenizeEx(strLineInfo, ",", iStart));	// 시리얼 (hidden)
	}
	//////////////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COrderLineManDlg::OnBtnDel() 
{
	POSITION pos = m_ctrlOLList.GetFirstSelectedItemPosition();

	while(pos)
	{
		int nItem = m_ctrlOLList.GetNextSelectedItem(pos);
		CString strCodeAndDateType = m_ctrlOLList.GetItemText(nItem, 1) + m_ctrlOLList.GetItemText(nItem, 3);
		POSITION pos2 = m_pSavedAllOrderLines->GetTailPosition();
		while(pos2)
		{
			POSITION posForDel = pos2;
			CString strOrderLine = m_pSavedAllOrderLines->GetPrev(pos2);
			if(strOrderLine.Find(strCodeAndDateType) >= 0)
			{
				m_pSavedAllOrderLines->RemoveAt(posForDel);
				RemoveLIneFromObList(strCodeAndDateType);
			}
		}			
		m_ctrlOLList.DeleteItem(nItem);
	}

	CString strFileName = m_strUserPath + OL_FILE;

	CFile f;
	//if( !f.Open( strFileName, CFile::modeWrite ) ) {
	if( !f.Open( strFileName, CFile::modeCreate | CFile::modeWrite ) ) {
		  return;
	}
	CArchive ar( &f, CArchive::store);

	m_pSavedAllOrderLines->Serialize(ar);
	
}

int COrderLineManDlg::FindListItem(CString strSerial)
{
	int nIndex = -1;
	strSerial.Delete(strSerial.GetLength() - 14, 14);
	for(int i=0;i<m_ctrlOLList.GetItemCount();i++)
	{
		if( strSerial == m_ctrlOLList.GetItemText(i,1) + m_ctrlOLList.GetItemText(i,3))
		{
			nIndex = i;
		}
	}
	return nIndex;


}

CString COrderLineManDlg::TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/)
{
	CString strRes = _T("");
	int iCur = iStart;
	iStart = strSrc.Find(pszTokens, iCur);
	if(iStart != -1)
	{
		strRes = strSrc.Mid(iCur, iStart-iCur);
		iStart++;
		return strRes;
	}
	return strRes;
}

void COrderLineManDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == 46)
		OnBtnDel();
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COrderLineManDlg::OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	if(pLVKeyDow->wVKey == 46)
		OnBtnDel();
	
	*pResult = 0;
}

void COrderLineManDlg::RemoveLIneFromObList(CString strSerial)
{
	POSITION pos = m_pLineList->GetHeadPosition();
	while (pos)
	{
		POSITION pos2 = pos;	// 삭제용
		CLineObj* pLine = (CLineObj*)m_pLineList->GetNext(pos);
		if(pLine->m_strSerial.Find(strSerial) >= 0)
		{
			delete pLine;
			pLine = NULL;
			m_pLineList->RemoveAt(pos2);
			break;
		}
	}
}
