// STListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "STListCtrl.h"
#include "../../inc/ISTManager.h"
#include ".\stlistctrl.h"
#include "../../inc/IAUDataMonitorMng.h"	// UUID_IAUDataMonitorManager : 콜백함수 받는데 사용

const UINT RMSG_OB_ITEMCLICK = ::RegisterWindowMessage("RMSG_OB_ITEMCLICK");
const UINT RMSG_OB_ITEMDBLCLK = ::RegisterWindowMessage("RMSG_OB_ITEMDBLCLK");
const UINT RMSG_ISSTRUNSTATE = ::RegisterWindowMessage(_T("RMSG_ISSTRUNSTATE"));

IMPLEMENT_DYNAMIC(CSTListCtrl, CListCtrl)
CSTListCtrl::CSTListCtrl()
{
	m_pDC = NULL;
	m_pOldFont = NULL;
}

CSTListCtrl::~CSTListCtrl()
{
	if (m_fontBold.GetSafeHandle())
		m_fontBold.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSTListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClk)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
END_MESSAGE_MAP()


void CSTListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem<0) return;

	CString szText = GetItemText(pNMListView->iItem, pNMListView->iSubItem);
	int nType = (int)GetItemData(pNMListView->iItem);

	if(!szText.IsEmpty()) {
		STMNG_ITEMINFO Info;
		Info.szSTPath = m_szFullPath;
		switch(nType)
		{
//			case 1:
//				Info.szSTName.Format(_T("P_%s%c%d"), szText, gSTGubunKey, nType);
//				break;
			case 4:
			case 5:
//				Info.szSTName.Format(_T("P_%s%c%d"), szText, gSTGubunKey, nType);
				if(m_nFolderIndx == 3)
					Info.szSTName.Format(_T("P_%s%c%d"), szText, gSTGubunKey, nType);	// 제공용
				else
					Info.szSTName.Format(_T("%s%c%d"), szText, gSTGubunKey, nType);	// 사용자용
				break;
			case 9999:
				Info.szSTName = szText;
				break;
			default:
				Info.szSTName.Format(_T("%s%c%d"), szText, gSTGubunKey, nType);
				break;
		}

		SetUserSetting(0, pNMListView->iItem, m_bUseBoldFont);

		Info.nSTType = m_nSTFolderType;
		Info.szSTRData = m_szFolderName;
		Info.nFolderIndx = m_nFolderIndx;
		Info.nItemIndx = pNMListView->iItem;

		Invalidate();
		if(GetItemState(pNMListView->iItem, LVIS_SELECTED))
		GetParent()->GetOwner()->SendMessage(RMSG_OB_ITEMDBLCLK, (WPARAM)0, (LPARAM)&Info);
	}

	*pResult = 0;
}

void CSTListCtrl::OnNMClk(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem<0) return;

	CString szText = GetItemText(pNMListView->iItem, pNMListView->iSubItem);
	int nType = (int)GetItemData(pNMListView->iItem);

	if(!szText.IsEmpty()) {
		STMNG_ITEMINFO Info;
		Info.szSTPath = m_szFullPath;
		if(nType == 9999)
			Info.szSTName = szText;
		else
			Info.szSTName.Format(_T("%s%c%d"), szText, gSTGubunKey, nType);

		Info.nSTType = m_nSTFolderType;
		Info.szSTRData = m_szFolderName;
		Info.nFolderIndx = m_nFolderIndx;
		Info.nItemIndx = pNMListView->iItem;
		GetParent()->GetOwner()->SendMessage(RMSG_OB_ITEMCLICK, (WPARAM)0, (LPARAM)&Info);
	}

	*pResult = 0;
}

void CSTListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;          // ask for item notifications.
		break;

	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_DODEFAULT;

//		m_pDC = CDC::FromHandle(lplvcd->nmcd.hdc);
//		if(GetUserSetting(0, lplvcd->nmcd.dwItemSpec)==TRUE)
//		{
//			m_pOldFont = m_pDC->SelectObject(&m_fontBold);
//		}
//		else {
//			if(m_pOldFont)
//			{
//				m_pDC->SelectObject(m_pOldFont);
//				m_pOldFont = NULL;
//			}
//		}

		*pResult = CDRF_NEWFONT;
		break;

	default:
		*pResult = CDRF_DODEFAULT;
	}
}

void CSTListCtrl::InitFont(BOOL bUseBoldFont)
{
	CClientDC dc(this);

//	dc.GetCurrentFont()->GetLogFont(&m_logFont);
	::memset(&m_logFont, 0x00, sizeof(LOGFONT));
	m_logFont.lfWidth = 0; 
	m_logFont.lfEscapement = 0; 
	m_logFont.lfOrientation = 0; 
	m_logFont.lfWeight = FW_NORMAL; 
	m_logFont.lfItalic = 0; 
	m_logFont.lfUnderline = 0; 
	m_logFont.lfStrikeOut = 0; 
	m_logFont.lfCharSet = DEFAULT_CHARSET; 
	m_logFont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	m_logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	m_logFont.lfQuality = DEFAULT_QUALITY;
	m_logFont.lfPitchAndFamily = FF_ROMAN; 
	strcpy(m_logFont.lfFaceName, "굴림체");
	m_logFont.lfHeight = 12;
//	m_logFont.lfWeight = FW_BOLD;

	if(m_fontBold.GetSafeHandle() == NULL)
	{
		m_fontBold.CreateFontIndirect(&m_logFont);
		SetFont(&m_fontBold);
	}

	m_bUseBoldFont = bUseBoldFont;
}

void CSTListCtrl::SetUserSetting(int nType, int nIndex, BOOL bValue)
{
	if(bValue)
	{
		int nCount = GetItemCount();
		if(nCount<=nIndex) return;

		m_UserSelState.SetAt(nIndex, (DWORD)m_UserSelState.GetAt(nIndex) ? FALSE : TRUE);
	}
}

BOOL CSTListCtrl::GetUserSetting(int nType, int nIndex)
{
	int nCount = GetItemCount();
	if(nCount<=nIndex) 
		return FALSE;
	else if(nCount >= m_UserSelState.GetSize())
		m_UserSelState.SetSize(nCount);

	return (BOOL)m_UserSelState.GetAt(nIndex);	
}

void CSTListCtrl::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem<0) return;

	if(m_nSTFolderType == 0)
	{
		// AfxMessageBox("제공된 전략은 삭제할 수 없습니다.");
		*pResult = 0;
		return;
	}

	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, 50001, "삭제");

	CPoint pt;
	::GetCursorPos(&pt);

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nReturn = menu.TrackPopupMenu(nFlagsForMenu, pt.x, pt.y, this);
	menu.DestroyMenu();

	if(nReturn >0 && m_nSTFolderType == 0)
	{
		AfxMessageBox("제공된 전략은 삭제할 수 없습니다.");
		*pResult = 0;
	}

	if(nReturn == 50001)
	{
		CString szText = GetItemText(pNMListView->iItem, pNMListView->iSubItem);
		int nType = (int)GetItemData(pNMListView->iItem);
		if(!szText.IsEmpty()) {
			STMNG_ITEMINFO Info;
			Info.szSTPath = m_szFullPath;
			if(nType == 9999)
				Info.szSTName = szText;
			else
				Info.szSTName.Format(_T("%s%c%d"), szText, gSTGubunKey, nType);

			Info.nSTType = m_nSTFolderType;
			Info.szSTRData = m_szFolderName;
			Info.nFolderIndx = m_nFolderIndx;
			Info.nItemIndx = pNMListView->iItem;

			// 전략리스트가 변경되었다는 내용을 알린다.
			int nID = UUID_IAUDataMonitorManager;
			IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);

			//Ext_STChange_SDDelete=0x20,		//고급전략의 사용자단위전략삭제
			//Ext_STChange_SDAdd=0x21,			//고급전략의 사용자단위전략추가
			//Ext_STChange_Delete=0x40,			//고급전략의 사용자조합전략삭제
			//Ext_STChange_Add=0x41,			//고급전략의 사용자조합전략추가
			int nState = 0;
			if(m_szFolderName.Compare("사용자단위전략")==0)
				nState = 0x20;
			else if(m_szFolderName.Compare("사용자지표")==0)
				nState = 0x20;
			else if(m_szFolderName.Compare("사용자강세약세")==0)
				nState = 0x20;
			else if(m_szFolderName.Compare("사용자조합전략")==0)
				nState = 0x40;

			if(nState > 0 && AfxMessageBox("전략을 삭제하시겠습니까?", MB_YESNO)==IDYES)
			{
				CString strSTPath;
				strSTPath.Format("%s\\%s.ST", Info.szSTPath, Info.szSTName);
				CString strSTName(Info.szSTName);
				BOOL bRunState = GetParent()->GetOwner()->SendMessage(RMSG_ISSTRUNSTATE, (WPARAM)(LPCTSTR)strSTPath, (LPARAM)(LPCTSTR)strSTName);
				if(bRunState)
				{
					AfxMessageBox("차트상에 전략이 적용이 되어 있는지 확인하시기 바랍니다.");
				}
				else
				{
					::DeleteFile(strSTPath);
					pMng->NotifyData("STLIST_MODIFIED", nState, (LONG)(LPCSTR)strSTName);
				}
			}
		}
	}

	*pResult = 0;
}
