// XListCtrlJongData.cpp : implementation file
//

#include "stdafx.h"
#include "XListCtrlJongData.h"
#include "LeftBarDlg.h"
#include "TabDlg.h"
#include "ChartItem.h"
#include <Mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int colwidthsType0[6] = { 20, 50, 50, 50, 50, 50};	// sixty-fourths

TCHAR *	lpszHeadersType0[] = { _T(""),
						  _T("종목명"),
						  _T("현재가"),
						  _T("대비"),
						  _T("대비율"),
						  _T("거래량"),
						  NULL };

int colwidthsType1[8] = { 20, 36, 50, 50, 50, 50, 50, 62};	// sixty-fourths
TCHAR *	lpszHeadersType1[] = { _T(""),
						  _T("연속"),
						  _T("종목명"),
						  _T("현재가"),
						  _T("대비"),
						  _T("대비율"),
						  _T("거래량"),
						  _T("편입시간"),
						  NULL };


/////////////////////////////////////////////////////////////////////////////
// CXListCtrlJongData
extern CString SetComma(CString strNumber, BOOL bShowSign=FALSE);

int CompareByIndex(const void *elem1, const void *elem2)
{
        UINT *p1 = (UINT*)elem1;
        UINT *p2 = (UINT*)elem2;
		if(*p1 == *p2) return 0;
		if(*p1 > *p2) return 1;
        return -1;
}

CXListCtrlJongData::CXListCtrlJongData()
{
	m_pLeftBarDlg = NULL;
//	m_pListJ = NULL;
//	m_pListK = NULL;	
	m_nType = 0;	
	m_bAutoSort = FALSE;
	m_lIndexOfCheck = -1;
	m_lIndexOfKeepingCnt = -1;
	m_lIndexOfName = -1;
	m_lIndexOfPrice = -1;
	m_lIndexOfChange = -1;
	m_lIndexOfChgrate = -1;
	m_lIndexOfVolume = -1;
	m_lIndexOfTime = -1;
	m_nStartAlarmKeepingCnt = 0;
	m_nEndAlarmKeepingCnt = 0;
	m_bUseAlarmSound = FALSE;
	m_bUseAlarmBkClr = FALSE;
	m_bPlayedAlarmSound = FALSE;
	InitSortIndex();
}

CXListCtrlJongData::~CXListCtrlJongData()
{
	
}


BEGIN_MESSAGE_MAP(CXListCtrlJongData, CXListCtrl)
	//{{AFX_MSG_MAP(CXListCtrlJongData)
	ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnColumnClick)
	ON_WM_DESTROY()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXListCtrlJongData message handlers


void CXListCtrlJongData::Init(CLeftBarDlg *pLeftBarDlg, int nType)
{
	m_nType = nType;
	if(m_nType==0)
	{
		m_lIndexOfCheck = 0;
		m_lIndexOfName = 1;
		m_lIndexOfPrice = 2;
		m_lIndexOfChange = 3;
		m_lIndexOfChgrate = 4;
		m_lIndexOfVolume = 5;
	}
	else
	{
		m_lIndexOfCheck = 0;
		m_lIndexOfKeepingCnt = 1;
		m_lIndexOfName = 2;
		m_lIndexOfPrice = 3;
		m_lIndexOfChange = 4;
		m_lIndexOfChgrate = 5;
		m_lIndexOfVolume = 6;
		m_lIndexOfTime = 7;
	}
	m_pLeftBarDlg = pLeftBarDlg;
//	m_pLeftBarDlg->GetCodeMasterArray(CODE_JONGMOK, m_arKOSPICode	, m_arKOSPIName	);
//	m_pLeftBarDlg->GetCodeMasterArray(CODE_JONGMOK, m_arKOSDAQCode	, m_arKOSDAQName);
	//m_pListJ = (CList<CJMaster, CJMaster>*)m_pLeftBarDlg->GetCodePointer("JMASTER");
	//m_pListK = (CList<CKMaster, CKMaster>*)m_pLeftBarDlg->GetCodePointer("KMASTER");

	
	SetConfig();
	InitColumns();
	SetHilightColor(RGB(0,0,0),CRSELECTED);
}

void CXListCtrlJongData::InitColumns()
{
	
	SetExtendedStyle(LVS_EX_FULLROWSELECT);
	int w = 250 - 2*2 - 2;
	int i;
	int total_cx = 0;
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	if(m_nType==0)
	{
		// add columns
		for (i = 0; ; i++)
		{
			if (lpszHeadersType0[i] == NULL)
				break;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = i>1 ? LVCFMT_RIGHT:LVCFMT_LEFT;
			lvcolumn.pszText = lpszHeadersType0[i];
			lvcolumn.iSubItem = i;
			lvcolumn.cx = colwidthsType0[i];
			total_cx += lvcolumn.cx;
			InsertColumn(i, &lvcolumn);
		}
		// iterate through header items and attach the image list
		memset(&lvcolumn, 0, sizeof(lvcolumn));
		// set the format again - must do this twice or first column does not get set
		for (i = 0; ; i++)
		{
			if (lpszHeadersType0[i] == NULL)
				break;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
			lvcolumn.fmt = i>1 ? LVCFMT_RIGHT:LVCFMT_LEFT;
			lvcolumn.iSubItem = i;
			SetColumn(i, &lvcolumn);
		}	
	}
	else
	{
		// add columns
		for (i = 0; ; i++)
		{
			if (lpszHeadersType1[i] == NULL)
				break;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = i!=2 ? LVCFMT_RIGHT:LVCFMT_LEFT;
			lvcolumn.pszText = lpszHeadersType1[i];
			lvcolumn.iSubItem = i;
			lvcolumn.cx = colwidthsType1[i];
			total_cx += lvcolumn.cx;
			InsertColumn(i, &lvcolumn);
		}
		// iterate through header items and attach the image list
		memset(&lvcolumn, 0, sizeof(lvcolumn));
		// set the format again - must do this twice or first column does not get set
		for (i = 0; ; i++)
		{
			if (lpszHeadersType1[i] == NULL)
				break;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
			lvcolumn.fmt = i!=2 ? LVCFMT_RIGHT:LVCFMT_LEFT;
			lvcolumn.iSubItem = i;
			SetColumn(i, &lvcolumn);
		}	

	}
}

void CXListCtrlJongData::InitSortIndex()
{
	::ZeroMemory(m_bSortIndex,sizeof(m_bSortIndex));
	::ZeroMemory(m_bSortIndexType,sizeof(m_bSortIndexType));
	::ZeroMemory(m_bSortIndexConst,sizeof(m_bSortIndexConst));	
	SetSortImage(-1,0);
}

void CXListCtrlJongData::InsertCodeData(LPCTSTR lpCode, LPCTSTR lpName,CString strPrice,CString strChange,CString strChgrate,CString strVolume,char chSign, BOOL bKeeping /*=FALSE*/)
{
	CString strName = lpName;
	strName.TrimRight();
	strPrice.Remove(' ');
	strPrice.TrimLeft('0');
	strChange.Remove(' ');
	strChgrate.Remove(' ');
	strVolume.Remove(' ');

	long lPos = strName.Find('&');
	if(lPos>0)
	{
		strName.Insert(lPos,'&');
	}
/////////////
	COLORREF crText = RGB(0,0,0);
	COLORREF crBackground = RGB(255,255,255);
	COLORREF crBackgroundDaebi = RGB(255,255,255);
	COLORREF crTextDaebi = RGB(0,0,0);
	switch(chSign)
	{
		case UP_LIMIT:	// UpLimit
			crBackgroundDaebi = RGB(255,0,0);
			crTextDaebi = RGB(255,255,255);
			crText = RGB(255,0,0);
			break;
		case UP:	// Up
			crText = RGB(255,0,0);
			crTextDaebi = RGB(255,0,0);
			break;
		case DOWN:	// Down	
			crText = RGB(0,0,255);
			crTextDaebi = RGB(0,0,255);
			break;
		case DOWN_LIMIT:	// DownLimit
			crBackgroundDaebi = RGB(0,0,255);
			crTextDaebi = RGB(255,255,255);
			crText = RGB(0,0,255);
			break;	
		case EQUAL:	// Steady
		/* ===================== 05.05.17 comment by sy,nam
		case '1':
		case '2':
		case '8':
		case '9':
		case '0':
		 ===================== 05.05.17 comment by sy,nam */
			break;
	}

	CString strPriceWithComma = SetComma(strPrice);
	CString strChangeWithComma = SetComma(strChange);
	CString strVolumeWithComma = SetComma(strVolume);

	int nCnt = 0;
	nCnt = GetItemCount();
	InsertItem(nCnt,"");	

	CJongDataInListCtrl *pData = NULL;
	CTime tm = CTime::GetCurrentTime();
	CString strTime;
	if(m_nType==1)
	{
		strTime.Format("%02d:%02d:%02d",tm.GetHour(),tm.GetMinute(),tm.GetSecond());
	}
	pData = (CJongDataInListCtrl *)GetItemData(nCnt);
	if(!pData)
	{
		pData = new CJongDataInListCtrl;
		m_mapJongData.SetAt(lpCode,pData);
		SetItemData(nCnt,(DWORD)pData);
		pData->m_nCurrentIndex = nCnt;		
	}
	pData->m_strJongCode = lpCode;
	pData->m_strJongCode.Remove(' ');
	pData->m_chSign = chSign;
	pData->m_strCodeName = strName;
	pData->m_strPrice = strPriceWithComma;
	pData->m_strChange = strChangeWithComma;
	pData->m_strChgrate = strChgrate;
	pData->m_strVolume = strVolumeWithComma;
	pData->m_bKeeping = bKeeping;
	pData->m_strInsertedTime = strTime;
	
	CString strKeepingCnt;
	strKeepingCnt.Format("%d",pData->m_lKeepingCnt);
	if(nCnt%2)
	{
		SetCheckbox(nCnt,m_lIndexOfCheck, 0);
		SetItemText(nCnt,m_lIndexOfKeepingCnt,strKeepingCnt);
		SetItemText(nCnt,m_lIndexOfName,strName);
		SetItemText(nCnt,m_lIndexOfPrice,strPriceWithComma,crText,crBackground);
		SetItemText(nCnt,m_lIndexOfChange,strChangeWithComma,crTextDaebi,crBackgroundDaebi);
		SetItemText(nCnt,m_lIndexOfChgrate,strChgrate,crTextDaebi,crBackgroundDaebi);
		SetItemText(nCnt,m_lIndexOfVolume,strVolumeWithComma);		
		SetItemText(nCnt,m_lIndexOfTime,strTime);
	}
	else
	{
		if(crBackgroundDaebi==RGB(255,255,255))
		{
			crBackgroundDaebi = CRGRID;
		}
		SetCheckbox(nCnt,m_lIndexOfCheck, 0);
		SetItemText(nCnt,m_lIndexOfKeepingCnt,strKeepingCnt,RGB(0,0,0),CRGRID);
		SetItemText(nCnt,m_lIndexOfName,strName,RGB(0,0,0),CRGRID);
		SetItemText(nCnt,m_lIndexOfPrice,strPriceWithComma,crText,CRGRID);
		SetItemText(nCnt,m_lIndexOfChange,strChangeWithComma,crTextDaebi,crBackgroundDaebi);
		SetItemText(nCnt,m_lIndexOfChgrate,strChgrate,crTextDaebi,crBackgroundDaebi);
		SetItemText(nCnt,m_lIndexOfVolume,strVolumeWithComma,RGB(0,0,0),CRGRID);
		SetItemText(nCnt,m_lIndexOfTime,strTime,RGB(0,0,0),CRGRID);
	}	
}

void CXListCtrlJongData::RemoveCode(LPCTSTR lpCode)
{
	int nIndex = FindCode(lpCode);
	if(nIndex!=-1)
	{
		CJongDataInListCtrl *pData = NULL;
		pData = (CJongDataInListCtrl *)GetItemData(nIndex);
		//2005. 08. 05 
		//m_pLeftBarDlg->OnCodeUnAdvise(pData->m_strJongCode);
		delete pData;
		pData = NULL;
		DeleteItem(nIndex);		
	}

}

void CXListCtrlJongData::GetCodeData(int nIndexIn, CString& rstrPriceOut,CString & rstrChangeOut,CString & rstrChgrateOut,CString & rstrVolumeOut,char& rchSign)
{
	if(nIndexIn!=-1)
	{
		char szTemp[126];
		GetItemText(nIndexIn,m_lIndexOfPrice,szTemp,sizeof(szTemp));
		rstrPriceOut = szTemp;
		GetItemText(nIndexIn,m_lIndexOfChange,szTemp,sizeof(szTemp));
		rstrChangeOut = szTemp;
		GetItemText(nIndexIn,m_lIndexOfChgrate,szTemp,sizeof(szTemp));
		rstrChgrateOut = szTemp;
		GetItemText(nIndexIn,m_lIndexOfVolume,szTemp,sizeof(szTemp));
		rstrVolumeOut = szTemp;
		CJongDataInListCtrl *pData = (CJongDataInListCtrl *)GetItemData(nIndexIn);
		rchSign = pData->m_chSign;
	}

}

int CXListCtrlJongData::FindCode(LPCTSTR lpCode)
{
	CJongDataInListCtrl *pData = NULL;
	CString strCodeInput = lpCode;
	strCodeInput.Remove(' ');	
	int nCnt = GetItemCount();
	for(int i=0;i<nCnt;i++)
	{
		pData = (CJongDataInListCtrl *)GetItemData(i);
		if(pData->m_strJongCode==strCodeInput)
		{
			return i;
			break;
		}
	}

	return -1;
}

int CXListCtrlJongData::RemoveAll()
{
	CJongDataInListCtrl *pData = NULL;
	int nCnt = GetItemCount();
	for(int i=0;i<nCnt;i++)
	{
		pData = (CJongDataInListCtrl *)GetItemData(i);
		if(pData)
		{
			delete pData;
			pData = NULL;
		}
	}
	m_mapJongData.RemoveAll();
	m_mapJongDataHighlighted.RemoveAll();
	m_mapJongDataChecked.RemoveAll();
	CXListCtrl::DeleteAllItems();
	::ZeroMemory(m_bSortIndex,sizeof(m_bSortIndex));
	return 1;
}

void CXListCtrlJongData::OnDestroy() 
{
	// TODO: Add your message handler code here
	SetConfig(FALSE);
	RemoveAll();	
	CXListCtrl::OnDestroy();
	
}

CString CXListCtrlJongData::GetCodeSelected(int nCurSel /*=-1*/)
{	
	CJongDataInListCtrl *pData = NULL;
	if(nCurSel==-1)
	{
		nCurSel = GetCurSel();
	}	
	if(nCurSel==-1) return "";
	pData = (CJongDataInListCtrl *)GetItemData(nCurSel);
	return pData->m_strJongCode;
}

CString CXListCtrlJongData::GetTimeSelected(int nCurSel /*=-1*/)
{	
	CJongDataInListCtrl *pData = NULL;
	CString strTime;
	if(nCurSel==-1)
	{
		nCurSel = GetCurSel();
	}	
	if(nCurSel==-1) return "";
	pData = (CJongDataInListCtrl *)GetItemData(nCurSel);
	strTime = pData->m_strInsertedTime;
	strTime.Remove(':');
	return strTime;
}

CString CXListCtrlJongData::GetSelectedCodes()
{
	CString strMultiCodes;
	CJongDataInListCtrl *pData = NULL;
	POSITION pos = GetFirstSelectedItemPosition();
	long lCnt = 0;
	while(pos)
	{	
		int nItem = GetNextSelectedItem(pos);
		pData = (CJongDataInListCtrl *)GetItemData(nItem);
		if(pData)
		{
			strMultiCodes += pData->m_strJongCode;
			strMultiCodes += ",";
			strMultiCodes += pData->m_strInsertedTime;
			strMultiCodes += ";";
			lCnt++;
		}
	}
//	if(lCnt==1)
//	{
//		strMultiCodes.TrimRight(';');
//	}
	return strMultiCodes;
}

CString CXListCtrlJongData::GetCheckedCodes()
{
		CString strJCodes;
	CJongDataInListCtrl *pData = NULL;
	
	int nCheckBox = 0;
	long lCnt = GetItemCount();
	int nChkCnt = 0;
	for(long lPos=0;lPos<lCnt;lPos++)
	{
		nCheckBox = GetCheckbox(lPos,0);
		if(nCheckBox)
		{
			pData = (CJongDataInListCtrl *)GetItemData(lPos);
			if(pData)
			{
				strJCodes += pData->m_strJongCode;
				strJCodes += ";";
				nChkCnt++;
				//lCnt++;
			}
		}		
	}
	//if(lCnt==1)
	if(nChkCnt == 1)
	{
		strJCodes.TrimRight(';');
	}
	return strJCodes;
}

CString CXListCtrlJongData::GetJongData(int nItem, int nSubItem)
{
	char szTemp[126];
	GetItemText(nItem,nSubItem,szTemp,sizeof(szTemp));
	return szTemp;
}


void CXListCtrlJongData::SetConfig(BOOL bLoad /*= TRUE*/)
{
	CString strTemp;
	char szTemp[64];
	CString strPos;
	int nWidth = 0;
	CString strAppName;
	strAppName.Format("XListCtrlJongData%d",m_nType);
	if(m_nType==0)
	{
		if(bLoad)
		{		
			for (int i = 1; ; i++)
			{
				if (lpszHeadersType0[i] == NULL)
					break;
				strPos.Format("%02d",i);
				strTemp.Format("%d",colwidthsType0[i]);
				::GetPrivateProfileString(strAppName,strPos,strTemp,szTemp,sizeof(szTemp),m_pLeftBarDlg->m_strConfig);
				colwidthsType0[i] = atoi(szTemp);
			}
		}
		else
		{
			for(int i = 1; ; i++)
			{
				if (lpszHeadersType0[i] == NULL)
					break;
				nWidth = GetColumnWidth(i);
				strPos.Format("%02d",i);
				strTemp.Format("%d",nWidth);
				::WritePrivateProfileString(strAppName,strPos,strTemp,m_pLeftBarDlg->m_strConfig);
			}
		}
	}
	else
	{
		if(bLoad)
		{		
			for (int i = 1; ; i++)
			{
				if (lpszHeadersType1[i] == NULL)
					break;
				strPos.Format("%02d",i);
				strTemp.Format("%d",colwidthsType1[i]);
				::GetPrivateProfileString(strAppName,strPos,strTemp,szTemp,sizeof(szTemp),m_pLeftBarDlg->m_strConfig);
				colwidthsType1[i] = atoi(szTemp);
			}
		}
		else
		{
			for(int i = 1; ; i++)
			{
				if (lpszHeadersType1[i] == NULL)
					break;
				nWidth = GetColumnWidth(i);
				strPos.Format("%02d",i);
				strTemp.Format("%d",nWidth);
				::WritePrivateProfileString(strAppName,strPos,strTemp,m_pLeftBarDlg->m_strConfig);
			}
		}
	}
}


void CXListCtrlJongData::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if((nChar==40||nChar==38)&&m_pLeftBarDlg->GetEnableWindow())
	{
		STINPUTCODEDATA stInputCodeData;
		stInputCodeData.m_strCode = GetCodeSelected();
		if(((CTabDlg*)GetParent())->m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			((CTabDlg*)GetParent())->m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
	}	
	CXListCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CXListCtrlJongData::UpdateCodeData(LPCTSTR lpCode, LPCTSTR lpName, CString strPrice,CString strChange,CString strChgrate,CString strVolume,char chSign, int nIndex)
{
	::ZeroMemory(m_bSortIndex,sizeof(m_bSortIndex));
	int nCnt = 0;
	CJongDataInListCtrl *pData = NULL;
	m_mapJongData.Lookup(lpCode,pData);
	if(!pData)
	{
		InsertCodeData(lpCode,lpName,strPrice,strChange,strChgrate,strVolume,chSign,TRUE);
		return FALSE;
	}
	nCnt = pData->m_nCurrentIndex;

	strPrice.Remove(' ');
	strPrice.TrimLeft('0');
	strChange.Remove(' ');
	strChgrate.Remove(' ');
	strVolume.Remove(' ');

/////////////
	COLORREF crText = RGB(0,0,0);
	COLORREF crBackground = RGB(255,255,255);
	COLORREF crBackgroundDaebi = RGB(255,255,255);
	COLORREF crTextDaebi = RGB(0,0,0);
	switch(chSign)
	{
		case UP_LIMIT:	// UpLimit
			crBackgroundDaebi = RGB(255,0,0);
			crTextDaebi = RGB(255,255,255);
			crText = RGB(255,0,0);
			break;
		case UP:	// Up
			crText = RGB(255,0,0);
			crTextDaebi = RGB(255,0,0);
			break;
		case DOWN_LIMIT:	// DownLimit
			crBackgroundDaebi = RGB(0,0,255);
			crTextDaebi = RGB(255,255,255);
			crText = RGB(0,0,255);
			break;
		case DOWN:	// Down	
			crText = RGB(0,0,255);
			crTextDaebi = RGB(0,0,255);
			break;
		case EQUAL:	// Steady
			break;
		/* ===================== 05.05.17 comment by sy,nam
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
			break;
		======================= 05.05.17 comment by sy,nam*/

	}

	CString strPriceWithComma = SetComma(strPrice);
	CString strChangeWithComma = SetComma(strChange);
	CString strVolumeWithComma = SetComma(strVolume);

	pData->m_chSign = chSign;
	pData->m_strPrice = strPriceWithComma;
	pData->m_strChange = strChangeWithComma;
	pData->m_strChgrate = strChgrate;
	pData->m_strVolume = strVolumeWithComma;
	pData->m_lKeepingCnt++;
	pData->m_bKeeping = TRUE;
	CString strKeepingCnt;
	strKeepingCnt.Format("%d",pData->m_lKeepingCnt);

	if(nCnt%2)
	{
		SetCheckbox(nCnt,m_lIndexOfCheck,pData->m_bCheck);
		SetItemText(nCnt,m_lIndexOfKeepingCnt,strKeepingCnt);
		SetItemText(nCnt,m_lIndexOfPrice,strPriceWithComma,crText,crBackground);
		SetItemText(nCnt,m_lIndexOfChange,strChangeWithComma,crTextDaebi,crBackgroundDaebi);
		SetItemText(nCnt,m_lIndexOfChgrate,strChgrate,crTextDaebi,crBackgroundDaebi);
		SetItemText(nCnt,m_lIndexOfVolume,strVolumeWithComma);			
		SetItemText(nCnt,m_lIndexOfTime,pData->m_strInsertedTime);		
	}
	else
	{
		if(crBackgroundDaebi==RGB(255,255,255))
		{
			crBackgroundDaebi = CRGRID;
		}
		SetCheckbox(nCnt,m_lIndexOfCheck,pData->m_bCheck);
		SetItemText(nCnt,m_lIndexOfKeepingCnt,strKeepingCnt,RGB(0,0,0),CRGRID);
		SetItemText(nCnt,m_lIndexOfPrice,strPriceWithComma,crText,CRGRID);
		SetItemText(nCnt,m_lIndexOfChange,strChangeWithComma,crTextDaebi,crBackgroundDaebi);
		SetItemText(nCnt,m_lIndexOfChgrate,strChgrate,crTextDaebi,crBackgroundDaebi);
		SetItemText(nCnt,m_lIndexOfVolume,strVolumeWithComma,RGB(0,0,0),CRGRID);
		SetItemText(nCnt,m_lIndexOfTime,pData->m_strInsertedTime,RGB(0,0,0),CRGRID);
	}	
	return TRUE;
}

void CXListCtrlJongData::SortColumns(int nColumnIndex, BOOL bSortDecendFirst /*=TRUE*/)
{
	if(m_bSortIndex[nColumnIndex])	// reverse
	{
		int nCnt = GetItemCount();
		int nIndexForReplace1 = nCnt;
		int nIndexForReplace2 = nCnt;
		for(int i1=0;i1<nCnt;i1++)
		{
			nIndexForReplace1--;
			if(i1<nIndexForReplace1) 
			{
				ReplaceIndex(nIndexForReplace1,i1);
			}
		}
		if(nColumnIndex)
		{
			SetSortImage(nColumnIndex,2);
		}
	}
	else	
	{
		::ZeroMemory(m_bSortIndex,sizeof(m_bSortIndex));
		if(nColumnIndex!=0) 
		{
			m_bSortIndex[nColumnIndex] = 1;
		}
		char szTemp[126];
		CString strPrice;
		CString strLMPrice;
		double dPrice = 0;
		double dMPrice = 0;
		long lMPrice = 0;
		long lPrice = 0;
		int nMIndex = 0;
		int nCnt = GetItemCount();

		if(bSortDecendFirst)// when Sort first.... & pnmlv->iSubItem == 0,
		{
			for(int i1=0;i1<nCnt;i1++)
			{
				strPrice.Empty();
				strLMPrice.Empty();			
				dMPrice = -9999999;
				dPrice = 0;
				lMPrice = 0;
				lPrice = 0;
				nMIndex = 0;
				for(int i2=i1;i2<nCnt;i2++)
				{
					GetItemText(i2,nColumnIndex,szTemp,sizeof(szTemp));
					strPrice = szTemp;
					if(nColumnIndex==0) 
					{
						if(GetCheckbox(i2,0))
						{
							nMIndex = i2;
							break;
						}
						else
						{
							nMIndex = i1;
						}
					}
					else if(nColumnIndex==m_lIndexOfName||nColumnIndex==m_lIndexOfTime) // Jongmok Name
					{
						if(strLMPrice.IsEmpty()||strLMPrice>strPrice)
						{
							strLMPrice = strPrice;
							nMIndex = i2;
						}
					}
					else if(nColumnIndex==m_lIndexOfChange||nColumnIndex==m_lIndexOfChgrate)	// Change, ChangeRate
					{
						strPrice.Remove(',');
						dPrice = atof(strPrice);
						if(dMPrice == -9999999||dMPrice<dPrice)
						{
							dMPrice = dPrice;
							nMIndex = i2;
						}
					}
					else
					{
						strPrice.Remove(',');
						lPrice = atol(strPrice);
						if(!lMPrice||lMPrice<lPrice)
						{
							lMPrice = lPrice;
							nMIndex = i2;
						}
					}
				}
				if(nMIndex!=i1)
				{
					ReplaceIndex(nMIndex,i1);				
				}	
			}
		}
		else
		{
			for(int i1=0;i1<nCnt;i1++)
			{
				strPrice.Empty();
				strLMPrice.Empty();			
				dMPrice = -9999999;
				dPrice = 0;
				lMPrice = 0;
				lPrice = 0;
				nMIndex = 0;
				for(int i2=i1;i2<nCnt;i2++)
				{
					GetItemText(i2,nColumnIndex,szTemp,sizeof(szTemp));
					strPrice = szTemp;
					if(nColumnIndex==0) 
					{
						if(GetCheckbox(i2,0))
						{
							nMIndex = i2;
							break;
						}
						else
						{
							nMIndex = i1;
						}
					}
					else if(nColumnIndex==m_lIndexOfName||nColumnIndex==m_lIndexOfTime) // Jongmok Name
					{
						if(strLMPrice.IsEmpty()||strLMPrice<strPrice)
						{
							strLMPrice = strPrice;
							nMIndex = i2;
						}
					}
					else if(nColumnIndex==m_lIndexOfChange||nColumnIndex==m_lIndexOfChgrate)	// Change, ChangeRate
					{
						strPrice.Remove(',');
						dPrice = atof(strPrice);
						if(dMPrice == -9999999||dMPrice>dPrice)
						{
							dMPrice = dPrice;
							nMIndex = i2;
						}
					}
					else
					{
						strPrice.Remove(',');
						lPrice = atol(strPrice);
						if(!lMPrice||lMPrice>lPrice)
						{
							lMPrice = lPrice;
							nMIndex = i2;
						}
					}
				}
				if(nMIndex!=i1)
				{
					ReplaceIndex(nMIndex,i1);				
				}	
			}	
		}		
		if(nColumnIndex)
		{
			SetSortImage(nColumnIndex,!bSortDecendFirst);
		}
	}
	Invalidate();
}

BOOL CXListCtrlJongData::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pnmlv = (NMLISTVIEW*)pNMHDR;
	if(pnmlv->iSubItem>=0)
	{
		::ZeroMemory(m_bSortIndexConst,sizeof(m_bSortIndexConst));	
		m_bSortIndexConst[pnmlv->iSubItem] = TRUE;
		m_bSortIndexType[pnmlv->iSubItem] = !m_bSortIndexType[pnmlv->iSubItem];
		SortColumns(pnmlv->iSubItem);	
	}	
	return CXListCtrl::OnColumnClick(pNMHDR, pResult);
}


void CXListCtrlJongData::ReplaceIndex(int nIndex1, int nIndex2)
{
	CJongDataInListCtrl *pData1 = (CJongDataInListCtrl *)GetItemData(nIndex1);
	CJongDataInListCtrl *pData2 = (CJongDataInListCtrl *)GetItemData(nIndex2);
	InsertCodeDataLighted(pData1, nIndex2);
	InsertCodeDataLighted(pData2, nIndex1);
}

void CXListCtrlJongData::UpdateCheck(int nIndex, CWnd *pWnd)
{
	CJongDataInListCtrl *pData = (CJongDataInListCtrl *)GetItemData(nIndex);	
	if(pData)
	{
		pData->m_bCheck = GetCheckbox(nIndex,0);
		if(pData->m_bCheck)
		{
			m_mapJongDataChecked.SetAt(pData->m_strJongCode,pData);			
		}
		else
		{
			m_mapJongDataChecked.RemoveKey(pData->m_strJongCode);
		}
		if(pWnd)
		{
			pWnd->SendMessage(UMSG_SENDJCODECHECKED,pData->m_bCheck,(LPARAM)(LPCTSTR)pData->m_strJongCode);
		}
	}
}

void CXListCtrlJongData::InsertCodeDataLighted(CJongDataInListCtrl *pData, int nIndex)
{
	m_crText = RGB(0,0,0);
	m_crBackground = RGB(255,255,255);
	m_crBackgroundDaebi = RGB(255,255,255);
	m_crTextDaebi = RGB(0,0,0);
	switch(pData->m_chSign)
	{
		case UP_LIMIT:	// UpLimit
			m_crBackgroundDaebi = RGB(255,0,0);
			m_crTextDaebi = RGB(255,255,255);
			m_crText = RGB(255,0,0);
			break;
		case UP:	// Up
			m_crText = RGB(255,0,0);
			m_crTextDaebi = RGB(255,0,0);
			break;
		case DOWN_LIMIT:	// DownLimit
			m_crBackgroundDaebi = RGB(0,0,255);
			m_crTextDaebi = RGB(255,255,255);
			m_crText = RGB(0,0,255);
			break;
		case DOWN:	// Down	
			m_crText = RGB(0,0,255);
			m_crTextDaebi = RGB(0,0,255);
			break;
		case EQUAL:	// Steady
		default:
			break;
		/* ===================== 05.05.17 comment by sy,nam
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
		======================= 05.05.17 comment by sy,nam*/
	
	}

	if(nIndex%2)
	{
		SetCheckbox(nIndex,0,pData->m_bCheck);
		SetItemText(nIndex,1,pData->m_strCodeName);
	}
	else
	{
		if(m_crBackgroundDaebi==RGB(255,255,255))
		{
			m_crBackgroundDaebi = CRGRID;
		}
		SetCheckbox(nIndex,0,pData->m_bCheck);	
		SetItemText(nIndex,1,pData->m_strCodeName,RGB(0,0,0),CRGRID);		
	}
	
/*	CString strKeepingCnt;
	strKeepingCnt.Format("%d",pData->m_lKeepingCnt);
	if(nIndex%2)
	{
		SetCheckbox(nIndex,m_lIndexOfCheck,pData->m_bCheck);
		SetItemText(nIndex,m_lIndexOfKeepingCnt,strKeepingCnt);
		SetItemText(nIndex,m_lIndexOfName,pData->m_strCodeName);
		SetItemText(nIndex,m_lIndexOfPrice,pData->m_strPrice,m_crText,m_crBackground);
		SetItemText(nIndex,m_lIndexOfChange,pData->m_strChange,m_crTextDaebi,m_crBackgroundDaebi);
		SetItemText(nIndex,m_lIndexOfChgrate,pData->m_strChgrate,m_crTextDaebi,m_crBackgroundDaebi);
		SetItemText(nIndex,m_lIndexOfVolume,pData->m_strVolume);
		SetItemText(nIndex,m_lIndexOfTime,pData->m_strInsertedTime);
	}
	else
	{
		if(m_crBackgroundDaebi==RGB(255,255,255))
		{
			m_crBackgroundDaebi = CRGRID;
		}
		SetCheckbox(nIndex,m_lIndexOfCheck,pData->m_bCheck);
		SetItemText(nIndex,m_lIndexOfKeepingCnt,strKeepingCnt,RGB(0,0,0),CRGRID);
		SetItemText(nIndex,m_lIndexOfName,pData->m_strCodeName,RGB(0,0,0),CRGRID);
		SetItemText(nIndex,m_lIndexOfPrice,pData->m_strPrice,m_crText,CRGRID);
		SetItemText(nIndex,m_lIndexOfChange,pData->m_strChange,m_crTextDaebi,m_crBackgroundDaebi);
		SetItemText(nIndex,m_lIndexOfChgrate,pData->m_strChgrate,m_crTextDaebi,m_crBackgroundDaebi);
		SetItemText(nIndex,m_lIndexOfVolume,pData->m_strVolume,RGB(0,0,0),CRGRID);
		SetItemText(nIndex,m_lIndexOfTime,pData->m_strInsertedTime,RGB(0,0,0),CRGRID);
	}
	*/
	SetItemData(nIndex,(DWORD)pData);
	pData->m_nCurrentIndex = nIndex;
}

void CXListCtrlJongData::CheckJongmokCode(CStringList *plistStrJongmokCodeChecked, CStringList *plistStrJongmokCodeHighlighted)
{
	CJongDataInListCtrl *pData = NULL;
	int nCnt = GetItemCount();
	POSITION posOfHighlighted;
	POSITION posOfChecked;
	for(int i=0;i<nCnt;i++)
	{
		pData = (CJongDataInListCtrl *)GetItemData(i);
		if(pData)
		{
			posOfHighlighted = plistStrJongmokCodeHighlighted->Find(pData->m_strJongCode);
			if(posOfHighlighted)
			{
				pData->m_bHighlight = TRUE;
				m_mapJongDataHighlighted.SetAt(pData->m_strJongCode,pData);
				plistStrJongmokCodeHighlighted->RemoveAt(posOfHighlighted);
				if(!plistStrJongmokCodeChecked->IsEmpty())
				{
					posOfChecked = plistStrJongmokCodeChecked->Find(pData->m_strJongCode);
					if(posOfChecked)
					{
						SetCheckbox(i,0,TRUE);
						pData->m_bCheck = TRUE;	
						m_mapJongDataChecked.SetAt(pData->m_strJongCode,pData);
						plistStrJongmokCodeChecked->RemoveAt(posOfChecked);
					}
				}								
			}			
			if(plistStrJongmokCodeHighlighted->IsEmpty())
			{
				break;
			}
		}
	}
}

void CXListCtrlJongData::CheckJongmokCode(CStringList *plistStrJongmokCodeChecked)
{
	CJongDataInListCtrl *pData = NULL;
	int nCnt = GetItemCount();
	POSITION posOfChecked;
	for(int i=0;i<nCnt;i++)
	{
		pData = (CJongDataInListCtrl *)GetItemData(i);
		if(pData)
		{
			posOfChecked = plistStrJongmokCodeChecked->Find(pData->m_strJongCode);
			if(posOfChecked)
			{
				SetCheckbox(i,0,TRUE);
				pData->m_bCheck = TRUE;	
				m_mapJongDataChecked.SetAt(pData->m_strJongCode,pData);
				plistStrJongmokCodeChecked->RemoveAt(posOfChecked);
			}			
			if(plistStrJongmokCodeChecked->IsEmpty())
			{
				break;
			}
		}
	}
}

void CXListCtrlJongData::CheckJongmokCode(BOOL bCheck, LPCTSTR lpJongmokCode)
{
	CJongDataInListCtrl *pData = NULL;
	int nCnt = GetItemCount();
	for(int i=0;i<nCnt;i++)
	{
		pData = (CJongDataInListCtrl *)GetItemData(i);
		if(pData)
		{
			if(pData->m_strJongCode==lpJongmokCode)
			{
				SetCheckbox(i,0,bCheck);
				pData->m_bCheck = bCheck;
				if(pData->m_bCheck)
				{
					m_mapJongDataChecked.SetAt(pData->m_strJongCode,pData);			
				}
				else
				{
					m_mapJongDataChecked.RemoveKey(pData->m_strJongCode);
				}
				break;
			}
		}
	}
}

BOOL CXListCtrlJongData::GetCheckByJongmokCode(LPCTSTR lpJongmokCode)
{	
	CJongDataInListCtrl *pData = NULL;
	int nCnt = GetItemCount();
	for(int i=0;i<nCnt;i++)
	{
		pData = (CJongDataInListCtrl *)GetItemData(i);
		if(pData)
		{
			if(pData->m_strJongCode==lpJongmokCode)
			{
				pData->m_bHighlight = TRUE;
				m_mapJongDataHighlighted.SetAt(pData->m_strJongCode,pData);
				CRect rect;
				GetItemRect(i,rect,LVIR_BOUNDS);
				InvalidateRect(rect);
				return pData->m_bCheck;				
			}
		}
	}
	return 0L;
}

void CXListCtrlJongData::UnHighlightByJongmokCode(LPCTSTR lpJongmokCode)
{
	CString strJongmokCode = lpJongmokCode;
	CJongDataInListCtrl *pData = NULL;
	int nCnt = GetItemCount();
	if(strJongmokCode.IsEmpty()) return;
	for(int i=0;i<nCnt;i++)
	{
		pData = (CJongDataInListCtrl *)GetItemData(i);
		if(pData)
		{
			if(pData->m_strJongCode==lpJongmokCode)
			{
				pData->m_bHighlight = FALSE;
				m_mapJongDataHighlighted.RemoveKey(pData->m_strJongCode);
				CRect rect;
				GetItemRect(i,rect,LVIR_BOUNDS);
				InvalidateRect(rect);
			}
		}
	}
}

void CXListCtrlJongData::GetDrawColors(int nItem, int nSubItem, COLORREF& colorText, COLORREF& colorBkgnd)
{
	CXListCtrl::GetDrawColors(nItem,nSubItem,colorText,colorBkgnd);
	CJongDataInListCtrl *pData = (CJongDataInListCtrl *)GetItemData(nItem);
	if(pData)
	{
		if((pData->m_lKeepingCnt>=m_nStartAlarmKeepingCnt&&pData->m_lKeepingCnt<=m_nEndAlarmKeepingCnt)
			&&nSubItem==m_lIndexOfKeepingCnt			)
		{
			if(m_bUseAlarmBkClr)
			{
				colorBkgnd = RGB(255,255,0);
			}
			if(m_bUseAlarmSound&&!m_bPlayedAlarmSound&&!m_strAlarmSound.IsEmpty())
			{
				::PlaySound(m_strAlarmSound, NULL, SND_ASYNC);	
				m_bPlayedAlarmSound = TRUE;
			}			
		}	
		else if(pData->m_bHighlight&&nSubItem==m_lIndexOfName)
		{
			colorBkgnd = RGB(70,130,232);
		}	
	}
}

void CXListCtrlJongData::RemoveNoUpdateItem()
{
	CJongDataInListCtrl *pData = NULL;
	int nCnt = GetItemCount();
	int nMIndex = 0;
	BOOL bNeedToBeDeleted = FALSE;
	for(int i1=0;i1<nCnt;i1++)
	{
		bNeedToBeDeleted = TRUE;
		nMIndex = 0;
		for(int i2=i1;i2<nCnt;i2++)
		{
			pData = (CJongDataInListCtrl *)GetItemData(i2);
			if(pData->m_bKeeping)
			{
				nMIndex = i2;
				bNeedToBeDeleted = FALSE;
				break;
			}
			else
			{
				nMIndex = i1;
			}
		}
		if(bNeedToBeDeleted)
		{
			for(int i3=(nCnt-1);i3>=i1;i3--)
			{
				pData = (CJongDataInListCtrl *)GetItemData(i3);
				if(pData)
				{
					m_mapJongData.RemoveKey(pData->m_strJongCode);
					m_mapJongDataHighlighted.RemoveKey(pData->m_strJongCode);
					delete pData;
					pData = NULL;
				}
				DeleteItem(i3);
			}			
			break;
		}
		else
		{
			if(nMIndex!=i1)
			{
				ReplaceIndex(nMIndex,i1);				
			}	
			pData = (CJongDataInListCtrl *)GetItemData(i1);
			pData->m_bKeeping = FALSE;
		}
	}	
	if(m_bAutoSort)
	{
		for(int nPos=0;nPos<10;nPos++)
		{
			if(m_bSortIndexConst[nPos])
			{
				SortColumns(nPos,m_bSortIndexType[nPos]);
				break;
			}
		}	
	}
	m_bPlayedAlarmSound = FALSE;
}

void CXListCtrlJongData::SetAlarmColor(BOOL bUseAlarmBkClr)
{
	m_bUseAlarmBkClr = bUseAlarmBkClr;
}

void CXListCtrlJongData::SetAlarmSound(BOOL bUseAlarmSound, LPCTSTR lpAlarmSoundFile /*=NULL*/)
{
	m_bUseAlarmSound = bUseAlarmSound;
	if(lpAlarmSoundFile)
	{
		m_strAlarmSound = lpAlarmSoundFile;
	}
}


void CXListCtrlJongData::SetAlarmPeriod(long nStartAlarmKeepingCnt,long	nEndAlarmKeepingCnt)
{
	m_nStartAlarmKeepingCnt = nStartAlarmKeepingCnt;
	m_nEndAlarmKeepingCnt = nEndAlarmKeepingCnt;
	Invalidate();
}

void CXListCtrlJongData::SetCursorPrev(int nCnt)
{
	int nCntAll = GetItemCount();
	POSITION pos = GetFirstSelectedItemPosition();
	CUIntArray arrayIndex;
	int nItem = nCntAll;
	int nItemFirstSelected = nCntAll;
	while(pos)
	{
		nItem = GetNextSelectedItem(pos);
		arrayIndex.Add(nItem);
		if(nItemFirstSelected>nItem)
		{
			nItemFirstSelected = nItem;
		}
	}
	long lFirstIndex = 0;
	long lLastIndex = 0;
	BOOL bRetValue = GetPosOfHighLighted(&lFirstIndex,&lLastIndex);
	if(bRetValue)
	{
		nItemFirstSelected = lFirstIndex;
	}
	if(nItemFirstSelected<nCnt)
	{
		nItemFirstSelected = nCnt;
	}
	if(nItemFirstSelected!=0)
	{
		int nCntInArray = arrayIndex.GetSize();
		for(int nPosInArray=0;nPosInArray<nCntInArray;nPosInArray++)
		{
			SetCurSel(arrayIndex.GetAt(nPosInArray),FALSE);
		}
		for(int nPos=(nItemFirstSelected-1);nPos>=0&&(nItemFirstSelected-1-nPos)<nCnt;nPos--)
		{
			SetCurSel(nPos);
			EnsureVisible(nPos,FALSE);
		}
	}
}

void CXListCtrlJongData::SetCursorNext(int nCnt)
{
	int nCntAll = GetItemCount();
	POSITION pos = GetFirstSelectedItemPosition();
	CUIntArray arrayIndex;
	int nItem = -1;
	int nItemLastSelected = -1;
	while(pos)
	{
		nItem = GetNextSelectedItem(pos);
		arrayIndex.Add(nItem);
		if(nItemLastSelected<nItem)
		{
			nItemLastSelected = nItem;
		}
	}
	long lFirstIndex = 0;
	long lLastIndex = 0;
	BOOL bRetValue = GetPosOfHighLighted(&lFirstIndex,&lLastIndex);
	if(bRetValue)
	{
		nItemLastSelected = lLastIndex;
	}
	if(nItemLastSelected>(nCntAll-nCnt-1))
	{
		nItemLastSelected = nCntAll - nCnt - 1;
	}
	if(nCntAll!=(nItemLastSelected+1))
	{
		int nCntInArray = arrayIndex.GetSize();
		for(int nPosInArray=0;nPosInArray<nCntInArray;nPosInArray++)
		{
			SetCurSel(arrayIndex.GetAt(nPosInArray),FALSE);
		}
		for(int nPos=(nItemLastSelected+1);nPos<nCntAll&&(nPos-nItemLastSelected-1)<nCnt;nPos++)
		{
			SetCurSel(nPos);
			EnsureVisible(nPos,FALSE);
		}
	}
}


void CXListCtrlJongData::GetListOfChecked(CStringList *pListChecked)
{
	if(!pListChecked) return;
	POSITION pos = m_mapJongDataChecked.GetStartPosition();
	CString strJCode;
	CJongDataInListCtrl* pJongDataInListCtrl = NULL;
	while(pos)
	{
		m_mapJongDataChecked.GetNextAssoc(pos,strJCode,pJongDataInListCtrl);
		if(pJongDataInListCtrl)
		{
			if(pJongDataInListCtrl->m_bCheck)
			{
				pListChecked->AddTail(strJCode);
			}			
		}
	}	
}

BOOL CXListCtrlJongData::GetPosOfHighLighted(long *pLStartPos, long *pLEndPos)
{
	POSITION pos = m_mapJongDataHighlighted.GetStartPosition();
	CString strJCode;
	CUIntArray arrayIndex;
	CJongDataInListCtrl* pJongDataInListCtrl = NULL;
	while(pos)
	{
		m_mapJongDataHighlighted.GetNextAssoc(pos,strJCode,pJongDataInListCtrl);
		if(pJongDataInListCtrl)
		{
			if(pJongDataInListCtrl->m_bHighlight)
			{
				arrayIndex.Add(pJongDataInListCtrl->m_nCurrentIndex);
			}			
		}
	}	
	int nSize = arrayIndex.GetSize();
	long lFirstIndex = 0;
	long lLastIndex = 0;
	if(nSize)
	{
		qsort( static_cast<void*>(&arrayIndex[0]), arrayIndex.GetSize(), sizeof(UINT), CompareByIndex );
		for(int n=0;n<nSize;n++)
		{
			if(!n)
			{
				lFirstIndex = arrayIndex.GetAt(n);
				lLastIndex = lFirstIndex;
			}
			else
			{
				lLastIndex = arrayIndex.GetAt(n);
				if(lLastIndex != (lFirstIndex+n))
				{
					return FALSE;
				}
			}
		}
		*pLStartPos = lFirstIndex;
		*pLEndPos = lLastIndex;		
		return TRUE;
	}
	return FALSE;	

}

CString CXListCtrlJongData::GetCommentAboutHighlight()
{
	CString strComment;
	long lFirstIndex = 0;
	long lLastIndex = 0;
	BOOL bRetValue = GetPosOfHighLighted(&lFirstIndex,&lLastIndex);
	if(bRetValue)
	{
		if(lFirstIndex!=lLastIndex)
		{
			strComment.Format("(%d-%d)/%d",lFirstIndex+1,lLastIndex+1,GetItemCount());
		}
		else
		{
			strComment.Format("%d/%d",lFirstIndex+1,GetItemCount());
		}		
	}
	return strComment;
}

