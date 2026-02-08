/* Standard Disclaimer: 
Copyright (C) 2000  Dennis Howard
This file is free software; you can redistribute it and/or
modify it without any conditions. There is no warranty,
implied or expressed, as to validity or fitness for a particular purpose.
*/

// TreeDropList.cpp : implementation file
//

#include "stdafx.h"
#include "TreeDropList.h"

#include "TreeCombo.h"
#include "resource.h"							// for IDS_FORMAT_FILE_NOT_FOUND
#include "../Include_Chart/DLL_Load/IMetaTable.h"		// for ML_LANGUAGE_RES

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeDropList

CTreeDropList::CTreeDropList()  : m_pCombo (NULL)
{
	m_Delimiter = _T('/');
	m_CloseReason = e_KillFocus;
	m_bDropped = FALSE;

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;
}

CTreeDropList::~CTreeDropList()
{
	m_stringList.RemoveAll();
}


BEGIN_MESSAGE_MAP(CTreeDropList, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeDropList)
	ON_NOTIFY_REFLECT(NM_KILLFOCUS, OnKillfocus)
	ON_WM_CHAR()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(NM_SETFOCUS, OnSetfocus)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeDropList message handlers

void CTreeDropList::OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER (pNMHDR);

	if (m_pCombo)
	{
		//Focus will be on the drop down button focus lost because of
		//mouse click to dialog client area. Check cursor position to
		//find where mouse click was.
		BOOL bClickedOnButton = m_pCombo->IsMouseOnButton ();
		BOOL bClickedOnEdit =  m_pCombo->IsMouseOnEdit ();
			
		ShowWindow (SW_HIDE);	

		//Treat click on dropdown box the same as a selection
		if (bClickedOnButton)
		{
			m_CloseReason = e_Select;
			OnSelection ();
			//Don't clear the DroppedState flag yet to prevent
			//the mouse click on the button from popping up the tree again.
		}
		else if (bClickedOnEdit)
		{
			m_CloseReason = e_Select;
			OnSelection ();
			//m_pCombo->SetDroppedState (FALSE);
		}
		else
		{
			m_pCombo->SetDroppedState (FALSE);
		}

		if (m_CloseReason == e_KillFocus)
		{
			SelectString (m_pCombo->m_BeginPath);
		}

		m_pCombo->SendParentComboMessage (CBN_KILLFOCUS);
		m_pCombo->m_Edit.Invalidate ();

	}
	*pResult = 0;
}


void CTreeDropList::OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER (pNMHDR);

	if (m_pCombo)
	{
		m_pCombo->SendParentComboMessage (CBN_SETFOCUS);
	}
	*pResult = 0;
}


void CTreeDropList::OnSelection ()
{
	HTREEITEM hItem = GetSelectedItem ();
	if (m_pCombo)
	{
		CString Text;
		Text = GetTreePath (hItem);//GetItemText (hItem);
		m_pCombo->SetWindowText(Text);

		m_pCombo->SendParentComboMessage (CBN_SELENDOK);
		m_CloseReason = e_Select;
	}
	ShowWindow (SW_HIDE);
}


void CTreeDropList::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER (pNMHDR);
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hItem = GetSelectedItem ();
	if (m_pCombo && hItem)
	{
		m_pCombo->SendParentComboMessage (CBN_SELCHANGE);
	}
	*pResult = 0;
}


void CTreeDropList::OnCancel ()
{
	if (m_pCombo)
	{
		m_pCombo->OnCancel ();
		m_CloseReason = e_Cancel;
	}
	ShowWindow (SW_HIDE);
}


void CTreeDropList::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_RETURN)
	{
	   OnSelection ();
	   return;
	}
	else if (nChar == VK_ESCAPE)
	{
		OnCancel ();
		return;
	}
	else if (nChar == VK_TAB)
	{
		if (m_pCombo)
		{
			OnSelection ();

			BOOL bShift = (GetKeyState(VK_SHIFT) < 0);

			CWnd* pComboParent = m_pCombo->GetParent ();
			if (pComboParent && pComboParent->GetSafeHwnd ())
			{
				CWnd* pNext = pComboParent->GetNextDlgTabItem (m_pCombo, bShift);
				if (pNext && pNext->GetSafeHwnd ())
				{
					pNext->SetFocus ();
				}
			}
			return;
		}
	}
	CTreeCtrl::OnChar(nChar, nRepCnt, nFlags);
}

void CTreeDropList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnLButtonDblClk(nFlags, point);

	if (m_pCombo)
	{
		m_pCombo->SendParentComboMessage (CBN_DBLCLK);
	}

	OnSelection ();
}

/*
HTREEITEM CTreeDropList::AddString ( LPCTSTR lpszString)
{

	HTREEITEM hRoot = NULL;
	if (!lpszString ||  (_tcsclen (lpszString) == 0) )
	{
		return NULL;
	}

	CString strTreeBranch = lpszString;
	HTREEITEM hEndNode = DropListAddItem (hRoot, strTreeBranch);

	return hEndNode;
}
*/


void CTreeDropList::SplitPath (const CString& strTreeBranch, CString& strRoot, CString& strRemainder)
{ 
    int DelimiterPos = strTreeBranch.Find (m_Delimiter);

	strRoot      = _T("");
	strRemainder = _T("");

	if (DelimiterPos == -1)
	{
		strRoot = strTreeBranch;
		return;
	}

	//Separate root substring
	if (DelimiterPos > 0)
	{
		strRoot = strTreeBranch.Mid (0, DelimiterPos);
	}

	//Separate remainder substring
	if (DelimiterPos < strTreeBranch.GetLength ())
	{
	   strRemainder = strTreeBranch.Mid (DelimiterPos + 1);
	}

}

HTREEITEM CTreeDropList::MatchSibling (HTREEITEM hItem, CString& strMatch)
{
	CString NodeText;
	HTREEITEM hSibling = NULL;
	if (hItem)
	{
		hSibling = hItem;
		while (hSibling)
		{
			NodeText = GetItemText (hSibling);
			if (NodeText == strMatch)
			{
				break;
			}
			else
			{
				hSibling = GetNextSiblingItem (hSibling);
			}
		}
	}
	return hSibling;
}

HTREEITEM CTreeDropList::SelectString( LPCTSTR lpszString, HTREEITEM hParent /*= NULL*/)
{
	HTREEITEM hMatch = FindString (lpszString, hParent);
	if (hMatch)
	{
		SelectItem (hMatch);
	}
	return hMatch;
}


HTREEITEM CTreeDropList::FindString (CString strTreeBranch, HTREEITEM hParent /*=NULL*/ )
{
	HTREEITEM hPreviousMatch;
	HTREEITEM hMatch;
	CString strRoot;
	CString strRemainder;

	hMatch = FindString (hParent, strTreeBranch, hPreviousMatch, strRoot, strRemainder);

	if (!strRemainder.IsEmpty ())
	{
		hMatch = NULL;
	}

	return hMatch;
}

HTREEITEM CTreeDropList::FindString (HTREEITEM hParent, CString strTreeBranch,
								   HTREEITEM& hPreviousMatch, CString& strRoot, CString& strRemainder )
{
	HTREEITEM hItem = hParent;
	HTREEITEM hMatch = NULL;

	hPreviousMatch = hParent;


	SplitPath (strTreeBranch, strRoot, strRemainder);
	if (strRoot.IsEmpty ())
	{
		//Nothing to search for
		return NULL;
	}

	 //special case when the tree is empty
	 if (hParent == NULL)
	 {
		 hItem = GetChildItem (hParent);
	 }

	 //try to find a child item that matches the substring text
	 //at the corresponding level

	 //Match tree nodes until a node is encountered that doesn't match
	 hMatch = MatchSibling (hItem, strRoot);
	 while (hMatch && !strRemainder.IsEmpty())
	 {
 		hPreviousMatch = hMatch;
		strTreeBranch = strRemainder;
		SplitPath (strTreeBranch, strRoot, strRemainder);
		hItem = GetChildItem (hMatch);
		hMatch = MatchSibling (hItem, strRoot);
	 }

    return hMatch;
}


/*
HTREEITEM CTreeDropList::DropListAddItem (HTREEITEM hParent, CString strTreeBranch)
{
 	HTREEITEM hAddedItem = NULL;
	HTREEITEM hPreviousMatch = hParent;
	HTREEITEM hMatch = NULL;

	CString strRemainder;
	CString strRoot;

	hMatch = FindString (hParent, strTreeBranch, hPreviousMatch, strRoot, strRemainder);

	//Add nodes until the remainder is gone
	while (!strRemainder.IsEmpty())
	{
		hPreviousMatch = InsertItem (strRoot, hPreviousMatch);
		strTreeBranch = strRemainder;
		SplitPath (strTreeBranch, strRoot, strRemainder);
	}
	
	//add only if the node doesn't match an existing node
	if (!hMatch)
	{
		hAddedItem = InsertItem (strRoot, hPreviousMatch);
	}

	return hAddedItem;
}
*/



CString CTreeDropList::GetTreePath (HTREEITEM hItem)
{
	CString ItemText;
	CString PathText;


	if (hItem)
	{
		HTREEITEM hParent = GetParentItem(hItem);
		if(hParent == NULL)
		{
			TRACE("Root\n");
			ItemText = GetItemText(hItem);

			int index = ItemText.Find('[');
			if(index != -1) ItemText = ItemText.Mid(index +1);
			index = ItemText.Find(']');
			if(index != -1) ItemText = ItemText.Left(index);

			ItemText.TrimLeft();
			ItemText.TrimRight();

			PathText.Format("%s.*",ItemText);
		}
		else// if(hItem->iImage == 1)
		{
			TRACE("Child\n");
			ItemText = GetItemText(hParent);

			int index = ItemText.Find('[');
			if(index != -1) ItemText = ItemText.Mid(index +1);
			index = ItemText.Find(']');
			if(index != -1) ItemText = ItemText.Left(index);

			ItemText.TrimLeft();
			ItemText.TrimRight();

			PathText = ItemText+_T(".");


			ItemText = GetItemText(hItem);

			index = ItemText.Find(',');
			if(index != -1) ItemText = ItemText.Mid(index +1);
			index = ItemText.Find(',');
			if(index != -1) ItemText = ItemText.Left(index);

			ItemText.TrimLeft();
			ItemText.TrimRight();

			PathText += ItemText;

		}

	}
	

	return PathText;
}

CString CTreeDropList::GetCurrentTreePath ()
{
	HTREEITEM hItem = GetSelectedItem ();
	
	return GetTreePath (hItem);
}



HTREEITEM CTreeDropList::AddPacket(LPSTR szPacketName, LPSTR szDescrip)
{
	
	CString szItem;
	szItem.Format("[%s]%s",szPacketName,szDescrip);
	
	HTREEITEM hItem = 0;
	TV_INSERTSTRUCT tvstruct;

	
	tvstruct.hParent = NULL;
	tvstruct.hInsertAfter = TVI_LAST;

//	tvstruct.hItem.iImage = 0;
	tvstruct.item.pszText = (LPSTR)(LPCTSTR)szItem;
	tvstruct.item.mask = TVIF_TEXT;
	hItem = InsertItem(&tvstruct);


	return hItem;

}


HTREEITEM CTreeDropList::AddData(HTREEITEM hParent, LPSTR Descption, BOOL IsKey)
{
	HTREEITEM hItem;
	TV_INSERTSTRUCT tvstruct;

	tvstruct.hParent = hParent;
	tvstruct.hInsertAfter = TVI_LAST;
	
	tvstruct.item.mask = TVIF_TEXT;

	tvstruct.item.pszText = Descption;
	hItem = InsertItem(&tvstruct);
	return hItem;

}

HTREEITEM CTreeDropList::AddItem(HTREEITEM hParent, LPSTR Description, LPSTR Name,LPSTR type, LPSTR szLength)
{

	CString szText;
	szText.Format("%s,%s,%s,%s",Description,Name,type,szLength);
	HTREEITEM hItem;
	TV_INSERTSTRUCT tvstruct;
	tvstruct.hParent = hParent;
	tvstruct.hInsertAfter = TVI_LAST;
	
	tvstruct.item.mask = TVIF_TEXT;
	tvstruct.item.iImage = 1;

	tvstruct.item.pszText = (LPSTR)(LPCTSTR)szText;
	hItem = InsertItem(&tvstruct);

	return hItem;

	
}


BOOL CTreeDropList::LoadTableInfo(CString szConfigFile)
{

	int len;
	FILE *fp = fopen((LPCTSTR)szConfigFile,"r+t");

	if(fp == NULL)
	{
		ML_SET_LANGUAGE_RES();
		CString szMsg, strFormat;
		strFormat.LoadString( IDS_FORMAT_FILE_NOT_FOUND);
		szMsg.Format( strFormat,szConfigFile);
		AfxMessageBox(szMsg);
		return FALSE;
	}

	unsigned char tmp[256],buff[256];

	while(fgets((char*)tmp,256,fp))
	{
		if(tmp[0] == '/') continue;
		if(tmp[0] == '#') continue;

		len = DataRegular(tmp,buff);
		
		if(len > 0)
			m_stringList.AddTail((CString)buff);
	}

	fclose(fp);
	Parsing();


	return TRUE;

}



int CTreeDropList::DataRegular(unsigned char *tmp, unsigned char *buff)
{
	int n = 0;
	int i = 0;
	for( i = 0; tmp[i] != NULL;i++)
	{
		if(tmp[i] > ' ') break;
	}

	for(;tmp[i] != NULL;i++)
	{
		if(tmp[i] == '\n') break;
		if(tmp[i] == '\r') break;

		if(tmp[i] == '/' && tmp[i+1] == '*')
		{
			for(;tmp[i] != NULL;i++)
			{
				if(tmp[i] == '*' && tmp[i+1] == '/')
				{
					i += 2;
					break;
				}
			}
		}

		if(tmp[i] >= ' ')
			buff[n++] = tmp[i];
	}

	buff[n] = 0;
	return n;

}


BOOL CTreeDropList::Parsing()
{
	HTREEITEM hPackNameItem = 0;

	char szName[32],szDescrip[64],szType[32],szFormat[32];
	BOOL Attr;
	BOOL Begin = FALSE;

	for(POSITION pos = m_stringList.GetHeadPosition();pos != NULL;)
	{
		CString szTmp = m_stringList.GetNext(pos);
		CString szReal = szTmp;
		szTmp.MakeUpper();
		szTmp.TrimLeft();
		szTmp.TrimRight();

		if(szTmp.Left(14) == "BEGIN_DATA_MAP")		//새로운 Packet 추가
		{
			szTmp = m_stringList.GetNext(pos);
			if(szTmp.IsEmpty()) return FALSE;
			LPSTR p = (LPSTR)(LPCTSTR) szTmp;

			p = CutString(p,szName);
			p = CutString(p,szDescrip);


			szTmp = p;
			szTmp.MakeUpper();

			int index = -1;
			if((index = szTmp.Find("ATTR")) != -1)
				Attr = TRUE;
			else
				Attr = FALSE;
			

			TRACE("\nBEGIN_DATA_MAP\n");
			TRACE("Name[%s] Description[%s] Attr[%d]\n",szName,szDescrip,Attr);
			hPackNameItem = AddPacket(szName,szDescrip);
		}
		else if(szTmp.Left(9) == "KEY_BEGIN")
		{
			
		}
		else if(szTmp.Left(7) == "KEY_END")
		{
		}
		else if(szTmp.Left(12) == "DATA_BEGIN")// && hPackNameItem !=NULL)
		{
			Begin = TRUE;
		} 
		else if(szTmp.Left(8) == "DATA_END")
		{
			Begin = FALSE;
		}
		else if(szTmp.Left(12) == "END_DATA_MAP")
		{
			Begin = FALSE;
			hPackNameItem = 0;
		}
		else
		{
			if(Begin && hPackNameItem !=NULL)
			{
				LPSTR p = (LPSTR)(LPCTSTR) szReal;

				p = CutString(p,szDescrip);
				p = CutString(p, szName);
				p = CutString(p, szType);
				p = CutString(p, szFormat);

				AddItem(hPackNameItem,szDescrip,szName,szType,szFormat);		
			}

		}
		
	}
	return TRUE;

}

LPSTR CTreeDropList::CutString(LPSTR str, LPSTR buff)
{
	if(str == NULL || buff == NULL) return NULL;
	buff [0] = 0;
	CString szTmp = str;
	CString szBuff;
	szTmp.TrimLeft();


	int pos = szTmp.Find(',');
	if(pos == -1)	szTmp.Find(';');
	if(pos == -1)	pos = szTmp.GetLength();

	szBuff = szTmp.Left(pos);

	if(szTmp.GetLength() > pos)
		szTmp = szTmp.Mid(pos+1);
	else
		szTmp = "";

	strcpy(buff,(LPSTR)(LPCTSTR)szBuff);
	strcpy(str,(LPSTR)(LPCTSTR)szTmp);

	return str;


}