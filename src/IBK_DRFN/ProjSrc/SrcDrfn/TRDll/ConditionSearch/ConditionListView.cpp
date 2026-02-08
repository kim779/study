// ConditionListView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ConditionListView.h"

#include <afxpriv.h>		//# Url
#include "MainFrmWnd.h"
#include "ConditionMakeWnd.h"
#include "FolderEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CConditionListView
IMPLEMENT_DYNCREATE(CConditionListView, CView)

CConditionListView::CConditionListView()
{
	LOGFONT logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);

	m_bIndustry = FALSE;

	m_pwndMain = NULL;
	m_ptabCondition = NULL;

	m_nViewType_Tree = -1;
	m_pdlgFolderEdit = NULL;

	m_ptreeActive = NULL;
	m_clrStaic	  = COLOR_TAB_CANVAS;

	m_nLastSelectTab = -1;
}

CConditionListView::~CConditionListView()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();

	m_brushBK.DeleteObject();
	m_brushBKTab.DeleteObject();
}

BEGIN_MESSAGE_MAP(CConditionListView, CView)
	//{{AFX_MSG_MAP(CConditionListView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CONDITION, OnChangeTab)

	ON_BN_CLICKED(IDC_BTN_TAB_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_TAB_NEXT, OnBtnNext)
	ON_BN_CLICKED(IDC_BTN_TAB_PRE, OnBtnPrevious)
	ON_BN_CLICKED(IDC_BTN_TAB_EXP, OnBtnExp)
	ON_BN_CLICKED(IDC_BTN_TREETYPE_NORMAL, OnBtnNormalType)
	ON_BN_CLICKED(IDC_BTN_TREETYPE_SORT, OnBtnSortType)
	ON_BN_CLICKED(IDC_BTN_FOLDEREDIT, OnBtnFolderEdit)
	ON_BN_CLICKED(IDC_BTN_FILEADD, OnBtnFileAdd)
	ON_BN_CLICKED(IDC_BTN_ITEMHELP, OnBtnItemHelp)
	ON_WM_SETTINGCHANGE()
	ON_WM_ERASEBKGND() //20100827 이문수 >>
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConditionListView drawing

void CConditionListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CConditionListView diagnostics

#ifdef _DEBUG
void CConditionListView::AssertValid() const
{
	CView::AssertValid();
}

void CConditionListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CConditionListView message handlers

int CConditionListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CWnd* pwndParent = GetParent();
	pwndParent = pwndParent->GetParent();		//# CMainFrmWnd
	m_pwndMain = pwndParent;
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CString strAppPath;
	strAppPath.Format("%s", pwndMainFrm->Variant(homeCC));

	m_brushBKTab.CreateSolidBrush(pwndMainFrm->GetAxColor(66));

	CString strUserPath;	
	strUserPath.Format("%s%s%s", strAppPath, "\\User\\", pwndMainFrm->Variant(nameCC));
	::SetEnvironmentVariable(KEY_USERDIRECTORY, strUserPath);

	m_ptabCondition = new CDrTabCtrl();
	m_ptabCondition->Create(0, this, IDC_TAB_CONDITION);
	m_ptabCondition->SetBKColor(RGB(232, 226, 210));
	m_ptabCondition->SetCurSel(0);
		
	TC_ITEM item;
	item.mask = TCIF_TEXT | TCIF_IMAGE;
	item.pszText = "일반";
	m_ptabCondition->InsertItem(0, &item);

	if(!pwndMainFrm->m_nSeparateForm)
	{
		item.pszText = "일반+순차";
		m_ptabCondition->InsertItem(1, &item);
	}
  
  /*
	item.pszText = "사용자";
	m_ptabCondition->InsertItem(2, &item);
  */

	m_imglistTree.DeleteImageList();
	m_imglistTree.Create(16, 16, ILC_COLOR8 | ILC_MASK, 3, 3);

	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE00));	//# 0 st
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE01));
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE00_FILE));	//# 0 st - File
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE01_FILE));
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE10));	//# 1 st
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE11));	
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE10));	//# 2 st
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE11));	
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_NORMAL));		//# 일반
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_NONALERT));	//# Non Alert
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_NORMAL));		//# PortPolio
	
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE));					//# 일반 File
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_UNIT));			//# 조합 일반 File
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_ALERT));		//# Alert File
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_ALERTUNIT));	//# Alert 조합 File

	m_bIndustry = pwndMainFrm->m_bIndustry;

	BOOL bIsFnUser = pwndMainFrm->m_bIsFnUser;
	m_treeCommonCtrl.Create(this,	
							IDC_TREEXML_COMMON,
							&m_imglistTree,
							"0",
							FALSE,
							FALSE,
							2,
							m_bIndustry,
							bIsFnUser);
	
	m_treeCommonCtrl_Sort.Create(this,	
								IDC_TREEXML_COMMON_SORT,
								&m_imglistTree,
								"01",
								FALSE,
								FALSE,
								1,
								m_bIndustry,
								bIsFnUser);

	m_treeOrderCtrl.Create(this,	
							IDC_TREEXML_ORDER,
							&m_imglistTree,
							"1",
							FALSE,
							FALSE,
							2,
							m_bIndustry,
							bIsFnUser);

	m_treeOrderCtrl_Sort.Create(this,	
								IDC_TREEXML_ORDER_SORT,
								&m_imglistTree,
								"11",
								FALSE,
								FALSE,
								1,
								m_bIndustry,
								bIsFnUser);

	m_treeExcavation.Create(this,
							IDC_TREEXML_EXCAVATION,
							&m_imglistTree,
							"7",
							FALSE,
							FALSE,
							0,
							m_bIndustry,
							bIsFnUser);
	
	m_btnHelpView.Create(_T("선택지표설명 ▲"), CRect(0, 0, 0, 0), this, IDC_BTN_ITEMHELP, FALSE, TRUE);
	pwndMainFrm->SetBtnImg(&m_btnHelpView, 8);
	
	m_editHelp.Create(WS_CHILD | ES_MULTILINE | ES_READONLY | WS_VSCROLL, 
		CRect(0,0,0,0), this, IDC_EDIT_ITEMHELP);
	m_editHelp.SetFont(&m_fontDefault);
	m_editHelp.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
		
	m_ctrlSearchBar.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlSearchBar.ShowWindow(SW_HIDE);
  
	CString strHistoryFilePath;
	strHistoryFilePath = pwndMainFrm->GetFilePath("FINDERINFO");
	m_comboSearch.Create(this, IDC_COMBO_TAB_FIND, strHistoryFilePath, "TreeHistory");
	m_comboSearch.ModifyStyleEx(0, WS_EX_NOPARENTNOTIFY);
	m_comboSearch.SetFont(&m_fontDefault);

	m_btnSearch.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_TAB_SEARCH);
	pwndMainFrm->SetBtnImg(&m_btnSearch, "검색");
  
	m_btnPre.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_TAB_PRE);
	pwndMainFrm->SetBtnImg(&m_btnPre, "CS_Search_Pre");
	
	m_btnNext.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_TAB_NEXT);
	pwndMainFrm->SetBtnImg(&m_btnNext, "CS_Search_Next");
			
	m_ctrlMenuBar.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlMenuBar.ShowWindow(SW_HIDE);

	m_btnTypeNormal.Create(_T("분류"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
	CRect(0, 0, 0, 0), this, IDC_BTN_TREETYPE_NORMAL);
	m_btnTypeNormal.SetFont(&m_fontDefault);

	m_btnTypeSort.Create(_T("가나다"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
	CRect(0, 0, 0, 0), this, IDC_BTN_TREETYPE_SORT);
	m_btnTypeSort.SetFont(&m_fontDefault);

	m_btnFolderEdit.Create(_T("편집"), CRect(0, 0, 0, 0), this, IDC_BTN_FOLDEREDIT);
	pwndMainFrm->SetBtnImg(&m_btnFolderEdit, 20);
  	
	m_btnFileAdd.Create(_T("조건조합"), CRect(0, 0, 0, 0), this, IDC_BTN_FILEADD);
	pwndMainFrm->SetBtnImg(&m_btnFileAdd, 20);
  	
	m_btnCollaspe.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_TAB_EXP);
	pwndMainFrm->SetBtnImg(&m_btnCollaspe, "CS_Tree_Help_Down");
    
	m_strAfterExpTag = "XXXX";
	m_btnTypeNormal.SetCheck(TRUE);

	AfxSetResourceHandle(hInstSave);
    	
	return 0;
}

void CConditionListView::SetInit(int nSeparateForm)
{
	m_bTreeHelpExp = TRUE;
	m_nSeparateForm = nSeparateForm;
	
	OnBtnItemHelp();
	SetSelectTab(0, 0);
	SerTreeReload();	// 20110222 박찬모 사용자 조건 오름차순 정렬	>>
	OnSize();

	if(m_nSeparateForm == 1)
	{
		OnBtnExp();
	}	
}

void CConditionListView::SetSelectItem()
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CString strConditionName = pwndMainFrm->GetSelectedCondition();
	
	HTREEITEM hParentItem;
	hParentItem = m_treeExcavation.GetFirstVisibleItem();
	while(hParentItem)
	{
		
		CString strParent;
		strParent = m_treeExcavation.GetItemText(hParentItem);
		
		HTREEITEM hItem = m_treeExcavation.GetChildItem(hParentItem);
		CString strText;
		strText = m_treeExcavation.GetItemText(hItem);
		
		BOOL bOk = FALSE;
		
		while(hItem)
		{
			strText = m_treeExcavation.GetItemText(hItem);
			if(strConditionName == strText)
			{
				m_treeExcavation.SetSelectItemExcavation_Manual(hItem);
				bOk = TRUE;
				break;
			}
			hItem = m_treeExcavation.GetNextSiblingItem(hItem);
		}
		
		if(bOk)
			break;
		
		hParentItem = m_treeExcavation.GetNextSiblingItem(hParentItem);
	}
}

void CConditionListView::OnDestroy() 
{
	//	CView::OnDestroy();

	if (m_ptabCondition != NULL)
	{
		delete m_ptabCondition;
		m_ptabCondition = NULL;
	}
	
	if (m_pdlgFolderEdit != NULL)
	{
		delete m_pdlgFolderEdit;
		m_pdlgFolderEdit = NULL;
	}
	
	if (m_saFindItem.GetData() != NULL)
		m_saFindItem.RemoveAll();
	
	m_pDomXmlExp = NULL;
	
	m_treeCommonCtrl.DestroyWindow();
	m_treeCommonCtrl_Sort.DestroyWindow();
	m_treeOrderCtrl.DestroyWindow();
	m_treeOrderCtrl_Sort.DestroyWindow();
	m_treeExcavation.DestroyWindow();
	
	m_btnHelpView.DestroyWindow();
	
	m_comboSearch.DestroyWindow();
	m_btnNext.DestroyWindow();
	m_btnPre.DestroyWindow();
	m_btnSearch.DestroyWindow();
	m_btnCollaspe.DestroyWindow();
	
	m_btnFolderEdit.DestroyWindow();
	m_btnFileAdd.DestroyWindow();
	
	m_imglistTree.DeleteImageList();
	
	CView::OnDestroy();
}

BOOL CConditionListView::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			CWnd* pFocusWnd = GetFocus();
			if (pMsg->hwnd == this->GetSafeHwnd())
				return FALSE;
			else if ((pFocusWnd->GetSafeHwnd() == m_treeCommonCtrl.GetSafeHwnd()) ||
								(pFocusWnd->GetSafeHwnd() == m_treeCommonCtrl_Sort.GetSafeHwnd()) ||
								(pFocusWnd->GetSafeHwnd() == m_treeOrderCtrl.GetSafeHwnd()) ||
								(pFocusWnd->GetSafeHwnd() == m_treeOrderCtrl_Sort.GetSafeHwnd()) ||
								(pFocusWnd->GetSafeHwnd() == m_treeExcavation.GetSafeHwnd()))
								
				return FALSE;
			else
			{
				OnBtnSearch();
				return TRUE;
			}
		}
		else if (pMsg->wParam == VK_ESCAPE)
			return FALSE;
	}
  	
	return CView::PreTranslateMessage(pMsg);
}

void CConditionListView::OnBtnItemHelp()
{
	if (m_bTreeHelpExp == TRUE)
		m_btnHelpView.SetWindowText("선택지표설명 ▲");
	else
		m_btnHelpView.SetWindowText("선택지표설명 ▼");

	SetHelpExp(!m_bTreeHelpExp);
}

void CConditionListView::SetHelpExp(BOOL bHelpExp)
{
	m_bTreeHelpExp = bHelpExp;
	
	OnSize();
	SetFocus();
}

void CConditionListView::SetExplainView(CXmlTreeCtrl* pTreeCtrl)
{
  if (pTreeCtrl != NULL)
  {
    if (pTreeCtrl->m_nSeleteClass == -1)
      SetTextExplain("");
    else if (pTreeCtrl->m_nSeleteClass == 0)
      SetItemExplain(pTreeCtrl->m_stSelectInfo);
    else if (pTreeCtrl->m_nSeleteClass == 1)
      SetFileExplain(pTreeCtrl->m_stSelectInfoFile);
  }
  else
    SetTextExplain("");
}

void CConditionListView::SetFileExplain(XMLTreeInfoFile	stXMLTreeInfoFile)
{
	CString strExplain;

	if (stXMLTreeInfoFile.strType == "USER")
		strExplain = "[사용자 조건]\n\n";
	else if (stXMLTreeInfoFile.strType == "SYS")
		strExplain = "[추천 조건]\n\n";

	stXMLTreeInfoFile.strExplain.Replace("$", "\n");
	m_editHelp.SetWindowText(strExplain + stXMLTreeInfoFile.strExplain);
	  
	m_btnFileAdd.EnableWindow(stXMLTreeInfoFile.bUnitCan);

	if (m_bTreeHelpExp == TRUE)
		m_editHelp.ShowWindow(SW_SHOW);
	else
		m_editHelp.ShowWindow(SW_HIDE);
}

void CConditionListView::SetTextExplain(CString strExplain)
{
	m_editHelp.SetWindowText(strExplain);
  
	if (m_bTreeHelpExp == TRUE)
		m_editHelp.ShowWindow(SW_SHOW);
	else
		m_editHelp.ShowWindow(SW_HIDE);
}

void CConditionListView::SetItemExplain(XMLTreeInfo stXMLTreeInfo)
{
	int nItemType = stXMLTreeInfo.nItemType;

	BOOL bHelpSet = TRUE;
	if ((nItemType == (int)TREE_SELECT_TYPE_ITEM_COMM) || (nItemType == (int)TREE_SELECT_TYPE_ITEM_ORDER))
	{
		SetItemExplain(stXMLTreeInfo.strTag);
		bHelpSet = FALSE;
	}
	else if (nItemType == (int)TREE_SELECT_TYPE_PORTPOLIO)
		SetTextExplain("[관심그룹]");
	else if (nItemType == (int)TREE_SELECT_TYPE_ACCOUNT)
		SetTextExplain("[보유 계좌]");
	else if (nItemType == (int)TREE_SELECT_TYPE_LANGUAGE)
		SetTextExplain("[랭귀지]");
	else if (nItemType == (int)TREE_SELECT_TYPE_USERPATTEN)
		SetTextExplain("[사용자 패턴]");
	else if (nItemType == (int)TREE_SELECT_TYPE_NEWS)
		SetTextExplain("[뉴스 상황문]");
	else
		m_editHelp.SetWindowText("설명 내역이 존재하지 않습니다.");

	if (bHelpSet == TRUE)
	{
		if (m_bTreeHelpExp == TRUE)
			m_editHelp.ShowWindow(SW_SHOW);
		else
			m_editHelp.ShowWindow(SW_HIDE);
	}

	m_btnFileAdd.EnableWindow(FALSE);
}

void CConditionListView::SetItemExplain(CString strTag)
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CString strAppPath;
	strAppPath.Format("%s", pwndMainFrm->Variant(homeCC));

	if (strTag.IsEmpty() == FALSE)
	{
		if (m_strAfterExpTag != strTag.Left(1))	//#<= 중복 실행 방지
		{
			try 
			{
				MSXML::IXMLDOMDocumentPtr pXmldom("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);	
				pXmldom->async = FALSE;

				CString strFolder, strFileName;
				strFolder = (CString)FOLDER_EXP;

				strFileName = strTag.Left(1);
				if (m_bIndustry == TRUE)
					strFileName += "_Industry";

				CString strFilePath;
				strFilePath.Format("%s%s\\exp%s.xml", strAppPath, strFolder, strFileName);

				if (pXmldom->load(_bstr_t(strFilePath)) == VARIANT_TRUE)
				{
					m_pDomXmlExp = pXmldom;
					m_strAfterExpTag = strTag.Left(1);	//#<= Tag : A1_2 = [A] => expA.xml
				}
				else 
				{
					m_editHelp.SetWindowText("설명 내역이 존재하지 않습니다.");
					if (m_bTreeHelpExp == TRUE)
					{
						m_editHelp.ShowWindow(SW_SHOW);
					}

					return;
				}
			} 
			catch(_com_error e) { return; }
		}
	}
	else 
		return;
	
	if (GetExplainItem(m_pDomXmlExp->childNodes, strTag, m_stExplainInfo) == TRUE)
	{
		if (m_stExplainInfo.nType == 0)
		{
			m_stExplainInfo.strExplain.Replace("$", "\n");
			m_editHelp.SetWindowText(m_stExplainInfo.strExplain);
      
			if (m_bTreeHelpExp == TRUE)
			{
				m_editHelp.ShowWindow(SW_SHOW);
			}
		}
		else if (m_stExplainInfo.nType == 1)
		{
			CString strFile;
			strFile = strAppPath + FOLDER_DATA + "\\Help\\aaa001.html";

			if (m_bTreeHelpExp == TRUE)
			{
				m_editHelp.ShowWindow(SW_HIDE);
			}
		}
	}
	else
	{
		m_editHelp.SetWindowText("설명 내역이 존재하지 않습니다.");
		if (m_bTreeHelpExp == TRUE)
		{
			m_editHelp.ShowWindow(SW_SHOW);
		}
	}
}

BOOL CConditionListView::GetExplainItem(MSXML::IXMLDOMNodeList *pNodes, CString strTag, ExplainInfo &stExplainInfo)
{
	BOOL bResult = FALSE;
	CString strTemp, nodeName;

	MSXML::IXMLDOMNodePtr pAttr;
	MSXML::IXMLDOMNamedNodeMapPtr pattrs;	
	
	if (pNodes == NULL) 
		return FALSE; 
	
	for (LONG idx = 0; idx < pNodes->length; idx++) 
	{
		MSXML::IXMLDOMNodePtr pNode = pNodes->item[idx];
		if (!wcscmp((const wchar_t *)pNode->nodeTypeString,L"element"))
		{
			nodeName = BSTR(pNode->nodeName);  //# Tag명
			if (strTag.Compare(nodeName) == 0)
			{
				if (SUCCEEDED(pNode->get_attributes(&pattrs))) 
				{
					MSXML::IXMLDOMNodePtr pItemAttr = pattrs->item[0];
					if ((pattrs->length) > 0) 
					{
						_bstr_t attrname = pItemAttr->nodeName;
						strTemp = (LPTSTR)(LPCTSTR)attrname;
						if (!strTemp.Compare("MAP_NAME"))
						{
							_variant_t value = pItemAttr->nodeValue;
							_bstr_t bstrValue(value);
							stExplainInfo.strName = (LPTSTR)(LPCTSTR)bstrValue;
														
							if (pItemAttr = pattrs->item[1])
							{
								_bstr_t attrChkname = pItemAttr->nodeName;
								strTemp = (LPTSTR)(LPCTSTR)attrChkname;
								if (!strTemp.Compare("TYPE"))
								{
									_variant_t value = pItemAttr->nodeValue;
									_bstr_t bstrValue(value);
									stExplainInfo.nType = atoi((LPTSTR)(LPCTSTR)bstrValue);
								}
							}

							if (pItemAttr = pattrs->item[2])
							{
								_bstr_t attrChkname = pItemAttr->nodeName;
								strTemp = (LPTSTR)(LPCTSTR)attrChkname;
								if (!strTemp.Compare("EXPLANATION"))
								{
									_variant_t value = pItemAttr->nodeValue;
									_bstr_t bstrValue(value);
									stExplainInfo.strExplain = (LPTSTR)(LPCTSTR)bstrValue;	
								}
							}

							if (pItemAttr = pattrs->item[3])
							{
								_bstr_t attrChkname = pItemAttr->nodeName;
								strTemp = (LPTSTR)(LPCTSTR)attrChkname;
								if (!strTemp.Compare("URL"))
								{
									_variant_t value = pItemAttr->nodeValue;
									_bstr_t bstrValue(value);
									stExplainInfo.strURL = (LPTSTR)(LPCTSTR)bstrValue;	
								}
							}

							bResult = TRUE;
						}
					}
				}
			}
			
			if (bResult == FALSE)
				bResult = GetExplainItem(pNode->childNodes, strTag, stExplainInfo);
		}

		if (bResult == TRUE)
			break;
		pNode = NULL;
	}

	pAttr = NULL;
	pattrs = NULL;	

	return bResult;
}

void CConditionListView::OnBtnSearch()
{
	CString strTemp;
	HTREEITEM hRootItem, hSelItem;

	m_comboSearch.GetWindowText(strTemp);
	if (strTemp == "")
		return;

	CXmlTreeCtrl* pTreeCtrl = m_ptreeActive;
	SetFindBar_Init();
	
	hRootItem = pTreeCtrl->GetRootItem();
	while (hRootItem != NULL)
	{
		pTreeCtrl->GetFindTreeItem(hRootItem, strTemp, m_saFindItem);
		hRootItem = pTreeCtrl->GetNextSiblingItem(hRootItem);
	}
	m_nSearchCnt = m_saFindItem.GetSize();

	if (m_nSearchCnt > 0)
	{
		hSelItem = m_saFindItem.GetAt(m_nSearchPos);
		pTreeCtrl->Select(hSelItem, TVGN_CARET);
		
		if (m_btnPre.IsWindowEnabled())
			m_btnPre.EnableWindow(FALSE);
		
		if (m_nSearchPos < m_nSearchCnt)
			m_btnNext.EnableWindow();

		if (m_nSearchPos == m_nSearchCnt - 1)
		{
			if (m_btnNext.IsWindowEnabled())
				m_btnNext.EnableWindow(FALSE);
		}
		SetSendTitleBar("처음 검색 단어입니다.");
		pTreeCtrl->SetFocus();
	}
	else
	{
		m_btnPre.EnableWindow(FALSE);	
		m_btnNext.EnableWindow(FALSE);
    
		strTemp += "의(에) 해당하는 단어가 없습니다.";
		MessageBox(strTemp);
	}	
  
	if (m_nSearchCnt > 0)
		m_comboSearch.SetAddString(strTemp);
  	
	m_btnPre.Invalidate();
	m_btnNext.Invalidate();
	pTreeCtrl->Invalidate();
}

void CConditionListView::SetSendTitleBar(CString strMsg)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	pwndMainFrm->SetSendMainMsg(strMsg);
}

int CConditionListView::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	return pwndMainFrm->SetMessageBox(lpszText, lpszCaption, nType);
}

void CConditionListView::OnBtnNext()
{
	if (m_saFindItem.GetSize() <= 0)
		return;

	CXmlTreeCtrl* pTreeCtrl = m_ptreeActive;

	if (m_nSearchPos < m_nSearchCnt  - 1)
	{
		m_nSearchPos +=	1;
		if (m_nSearchPos >= m_saFindItem.GetSize()) 
			return;

		HTREEITEM hSelItem = m_saFindItem.GetAt(m_nSearchPos);
		pTreeCtrl->Select(hSelItem, TVGN_CARET);

		if (!m_btnPre.IsWindowEnabled())
			m_btnPre.EnableWindow(TRUE);	

		if (m_nSearchPos < m_nSearchCnt - 1)
			m_btnNext.EnableWindow();
		else
		{
			m_btnNext.EnableWindow(FALSE);
			SetSendTitleBar("마지막 검색 단어입니다.");
		}
		pTreeCtrl->SetFocus();
	}

	m_btnPre.Invalidate();
	m_btnNext.Invalidate();	
}

void CConditionListView::OnBtnPrevious()
{
	if (m_saFindItem.GetSize() <= 0)
		return;

	CXmlTreeCtrl* pTreeCtrl = m_ptreeActive;
	if (m_nSearchPos > 0)
	{
		m_nSearchPos -=	1;
		HTREEITEM hSelItem = m_saFindItem.GetAt(m_nSearchPos);
		pTreeCtrl->Select(hSelItem, TVGN_CARET);
		
		if (!m_btnNext.IsWindowEnabled())
			m_btnNext.EnableWindow(TRUE);	

		if( m_nSearchPos > 0 )
			m_btnPre.EnableWindow();
		else
		{
			m_btnPre.EnableWindow(FALSE);
			SetSendTitleBar("처음 검색 단어입니다.");
		}
		pTreeCtrl->SetFocus();
	}

	m_btnPre.Invalidate();
	m_btnNext.Invalidate();
}

void CConditionListView::OnBtnExp()
{
	CXmlTreeCtrl* pTreeCtrl = m_ptreeActive;
	
	HTREEITEM hItem = pTreeCtrl->GetSelectedItem();

	if(hItem == NULL)
	{
		hItem = pTreeCtrl->GetFirstVisibleItem();
	}
	
	pTreeCtrl->SetItemAllView(!pTreeCtrl->m_bAllItemView);
	
	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
	
	if (pTreeCtrl->m_bAllItemView == TRUE)
		pMainWnd->SetBtnImg(&m_btnCollaspe, "CS_Tree_Help_Down");
	else
		pMainWnd->SetBtnImg(&m_btnCollaspe, "CS_Tree_Help_Up");
	
	pTreeCtrl->SelectItem(hItem);
	pTreeCtrl->SelectSetFirstVisible(hItem);
	
	m_btnCollaspe.Invalidate();
}

void CConditionListView::OnBtnFolderEdit()
{	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	if (m_pdlgFolderEdit == NULL)
		m_pdlgFolderEdit = new CFolderEditDlg(m_pwndMain);

	if (m_pdlgFolderEdit->DoModal() == IDOK)
	  SerTreeReload();

	AfxSetResourceHandle(hInstSave);
}

void CConditionListView::SetItme_User(BOOL bModify, XMLTreeInfoFile	stXMLTreeInfoFile)
{
	m_treeCommonCtrl.SetItme_User(bModify, stXMLTreeInfoFile);
	m_treeCommonCtrl_Sort.SetItme_User(bModify, stXMLTreeInfoFile);
	m_treeOrderCtrl.SetItme_User(bModify, stXMLTreeInfoFile);
	m_treeOrderCtrl_Sort.SetItme_User(bModify, stXMLTreeInfoFile);
}

void CConditionListView::SerTreeReload()
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
  
	if(m_nSeparateForm == 0)
	{
		m_treeCommonCtrl.SetReloadUserFile(&pwndMainFrm->m_listUserInfoFolder);
		m_treeCommonCtrl_Sort.SetReloadUserFile(&pwndMainFrm->m_listUserInfoFolder);
		m_treeOrderCtrl.SetReloadUserFile(&pwndMainFrm->m_listUserInfoFolder);
		m_treeOrderCtrl_Sort.SetReloadUserFile(&pwndMainFrm->m_listUserInfoFolder);
	}
	else if(m_nSeparateForm == 1)
		m_treeExcavation.SetLoadExcavationFile(&pwndMainFrm->m_listExcavationFolder);
	
  
	pwndMainFrm->SetSaveTreeXml_User();
}

void CConditionListView::OnBtnFileAdd()
{
	XMLTreeInfoFile stXMLTreeInfoFile;
	int nSeleteFlag = m_ptreeActive->GetSelect_File(stXMLTreeInfoFile);
	if ((nSeleteFlag == TREE_SELECT_TYPE_USERFILE) || (nSeleteFlag == TREE_SELECT_TYPE_SYSFILE))
		m_pwndMain->SendMessage(WM_NOTIFY_TAB, (WPARAM)NM_ADDFILE, (LPARAM)&stXMLTreeInfoFile);
	else
	{
		CString strMessage;
		strMessage = "조합할 수 없는 조건식을 선택하였습니다.\n\n포함된 조건식은 조합이 불가능합니다.";
		if(m_nSeparateForm == 0)
			SetMessageBox(strMessage, PROGRAM_TITLE, MB_OK);
		else
			SetMessageBox(strMessage, PROGRAM_TITLE_BRIGHT, MB_OK);
	}
}

void CConditionListView::OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nTabIndex = m_ptabCondition->GetCurSel();

	BOOL bTabChange = TRUE;

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CConditionMakeWnd *pendConditionMake = (CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE");
	if (pendConditionMake->m_DataAdmin.GetConditionCnt() > 0)
	{
		CString strMsg;
		strMsg = "조건작성 탭을 변경하고자 할 경우 \n" 
					"기존에 작성하신 선택조건은 초기화됩니다. \n"
					"선택조건을 초기화하시겠습니까?";
		if(m_nSeparateForm == 0)
		{
			if (SetMessageBox(strMsg, PROGRAM_TITLE, MB_ICONQUESTION | MB_YESNO) == IDNO)
				bTabChange = FALSE;
		}
		else
		{
			if (SetMessageBox(strMsg, PROGRAM_TITLE_BRIGHT, MB_ICONQUESTION | MB_YESNO) == IDNO)
				bTabChange = FALSE;
		}
	}

	if (bTabChange == TRUE)
	{
		SetSelectTab(nTabIndex, -1);

		if (m_nViewType_Tree > -1)
			m_pwndMain->SendMessage(WM_NOTIFY_TAB, (WPARAM)NM_CHANGETAB, nTabIndex);
	}
	else
	{
		m_ptabCondition->SetCurSel((nTabIndex == 0)?1:0);
		Invalidate();
	}
}

void CConditionListView::SetSelectTab(int nViewType_Tree, int nTabIndex)
{
	// 20110628 박찬모 조건이 바뀌었을시 무조건 changeTab을 하는 오류 >>
	BOOL bChangeTree = FALSE;
	if(m_nLastSelectTab == -1)
		m_nLastSelectTab = nViewType_Tree;
	else if(m_nLastSelectTab != nViewType_Tree)
	{
		bChangeTree = TRUE;
		m_nLastSelectTab = nViewType_Tree;
	}
	// 20110628 박찬모 <<

	if (nTabIndex > -1)
			m_ptabCondition->SetCurSel(nTabIndex);

	if ((m_nViewType_Tree == nViewType_Tree) && (m_ptreeActive != NULL))
		SetTreeView(m_ptreeActive);
	else
	{
		m_nViewType_Tree = nViewType_Tree;
		SetTreeView();
	}
	// 20110623 박찬모 순차 탭일 때 사용자 저장 조건을 읽어오지 못하는 오류 >>
	if(bChangeTree)
	{
		CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
		m_ptreeActive->SetReloadUserFile(&pwndMainFrm->m_listUserInfoFolder);
	}
	// 20110623 박찬모 <<
}

void CConditionListView::SetFindBar_Init()
{
	if (m_saFindItem.GetSize() > 0)
		m_saFindItem.RemoveAll();

	m_nSearchPos =	0;
	m_nSearchCnt =	0;

	m_btnPre.EnableWindow(FALSE);	
	m_btnPre.Invalidate();

	m_btnNext.EnableWindow(FALSE);
	m_btnNext.Invalidate();
}

void CConditionListView::OnBtnNormalType()
{
	m_btnTypeSort.SetCheck(FALSE);
	m_btnTypeNormal.SetCheck(TRUE);

	SetTreeView();
}

void CConditionListView::OnBtnSortType()
{
	m_btnTypeSort.SetCheck(TRUE);
	m_btnTypeNormal.SetCheck(FALSE);

	SetTreeView();
	
	// 20110222 박찬모 "가나다" 정렬 시 사용자조건이 나타나지 않는 오류 >>
	SerTreeReload();

	m_treeCommonCtrl_Sort.Invalidate();
	// 20110222 박찬모 "가나다" 정렬 시 사용자조건이 나타나지 않는 오류 <<
}

void CConditionListView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	OnSize();
}

void CConditionListView::OnSize()
{	
	CRect reClient;
	GetClientRect(reClient);

	if(m_nSeparateForm == 1)
		reClient.top += 3;
	reClient.top += 3;
	reClient.left+= 3;
	reClient.right -= 1;
	reClient.bottom -= 5;
	
	if (m_ptabCondition != NULL)
		m_ptabCondition->MoveWindow(reClient.left, reClient.top, reClient.Width(), 22);

	m_rtTab = reClient;
	m_rtTab.top += 20;
	
	CRect reTop, reTemp;
	
	reClient.top += 24 + 3;
	reClient.bottom -= 5;
	reClient.left += 4;
	reClient.right -= 4;
	reTop = reClient;
	
	if (m_nViewType_Tree < 2)
	{
		reTop.bottom = reTop.top + 20;
		/*m_ctrlSearchBar.MoveWindow(reTop);*/
		OnSizeSearchBar(reTop);
		
		SetShowSearchBar(TRUE);
		
		reTop.top = reTop.bottom + 1;
	}
	else if (m_nViewType_Tree == 2)
		SetShowSearchBar(FALSE);

	CRect reTree;
	reTree.top = reTop.bottom + 3;
	reTree.left = reClient.left;
	reTree.right = reClient.right;
	
	CRect reBottom = OnSizeHelpView(m_bTreeHelpExp);
	reTree.bottom = reBottom.top - 2;
	reTree.bottom -= 25;
	m_treeCommonCtrl.MoveWindow(&reTree);
	/*m_treeCommonCtrl.ModifyStyleEx(0, WS_EX_TOPMOST);*/
	/*m_treeCommonCtrl.SetWindowPos(NULL, reTree.left, reTree.top, reTree.Width(), reTree.Height(), SWP_NOMOVE | TOPMOSST)*/

	m_treeOrderCtrl.MoveWindow(&reTree);
	m_treeCommonCtrl_Sort.MoveWindow(&reTree);
	m_treeOrderCtrl_Sort.MoveWindow(&reTree);
	m_treeExcavation.MoveWindow(&reTree);

	m_reTree = reTree;

	reTop.top = reTree.bottom + 2;
	reTop.bottom = reTop.top + 25;
	/*m_ctrlMenuBar.MoveWindow(reTop);*/
	OnSizeMenuBar(reTop);

	Invalidate();
}

void CConditionListView::SetTreeView()
{
	CXmlTreeCtrl* pTreeCtrl = NULL;
	if(m_nSeparateForm == 0)
	{
		if (m_nViewType_Tree == 0)
		{
			if (m_btnTypeSort.GetCheck() == TRUE)
				pTreeCtrl = &m_treeCommonCtrl_Sort;
			else if (m_btnTypeNormal.GetCheck() == TRUE)
				pTreeCtrl = &m_treeCommonCtrl;
		}
		else if (m_nViewType_Tree == 1)
		{
			if (m_btnTypeSort.GetCheck() == TRUE)
				pTreeCtrl = &m_treeOrderCtrl_Sort;
			else if (m_btnTypeNormal.GetCheck() == TRUE)
				pTreeCtrl = &m_treeOrderCtrl;
		}
	}
	else if(m_nSeparateForm == 1)
		pTreeCtrl = &m_treeExcavation;
	
	if (m_ptreeActive == pTreeCtrl)
		return;

	if (m_ptreeActive != NULL)
		m_ptreeActive->ShowWindow(SW_HIDE);
	
	SetTreeView(pTreeCtrl);
	
	m_ptreeActive = pTreeCtrl;
}

void CConditionListView::SetTreeView(CXmlTreeCtrl* pTreeCtrl)
{
	if (pTreeCtrl != NULL)
	{
		pTreeCtrl->SetTreeView();
		pTreeCtrl->ShowWindow(SW_SHOW);
		pTreeCtrl->BringWindowToTop();
		pTreeCtrl->GetSelectItem();

		m_bActiveTreeComboLoad = pTreeCtrl->GetCheckComboLoad();
		pTreeCtrl->Invalidate();

		SetFindBar_Init();

		//# Select Item => Explain Load!
		SetExplainView(pTreeCtrl);

		if (pTreeCtrl->m_nSeleteClass == 1)   //# File
			m_btnFileAdd.EnableWindow(pTreeCtrl->m_stSelectInfoFile.bUnitCan);
		else
			m_btnFileAdd.EnableWindow(FALSE);

		CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
		if (pTreeCtrl->m_bAllItemView == TRUE)
			pMainWnd->SetBtnImg(&m_btnCollaspe, "CS_Tree_Help_Down");
		else
			pMainWnd->SetBtnImg(&m_btnCollaspe, "CS_Tree_Help_Up");
		m_btnCollaspe.Invalidate();
	}
}

CRect CConditionListView::OnSizeHelpView(BOOL bView)
{
	CRect reClient, reBottom;
	GetClientRect(reClient);

	reClient.left += 7;
	reClient.right -= 6;
	reClient.bottom -= 7;
	reBottom = reClient;

	int nEditHeight = 153;
	if (bView == TRUE)
	{
		reBottom.top = reBottom.bottom - nEditHeight;
		
		if (m_stExplainInfo.nType == 1)
		{
			m_editHelp.ShowWindow(SW_HIDE);
		}
		else
		{
			m_editHelp.MoveWindow(reBottom);
			m_editHelp.ShowWindow(SW_SHOW);
			m_editHelp.BringWindowToTop();
		}
				
		reBottom.bottom = reBottom.top - 2;
	}
	else
	{
		m_editHelp.ShowWindow(SW_HIDE);
	}

	reBottom.top = reBottom.bottom - 20;
	m_btnHelpView.MoveWindow(reBottom);

	return reBottom;
}

void CConditionListView::SetShowSearchBar(BOOL bState)
{
	int nViewState = (bState == TRUE)?SW_SHOW:SW_HIDE;

	m_comboSearch.ShowWindow(nViewState);
	m_btnNext.ShowWindow(nViewState);
	m_btnPre.ShowWindow(nViewState);
	m_btnSearch.ShowWindow(nViewState);
	m_btnCollaspe.ShowWindow(nViewState);

	m_editHelp.ShowWindow(nViewState);
	
	m_btnTypeNormal.ShowWindow(nViewState);
	m_btnTypeSort.ShowWindow(nViewState);


	if (m_bActiveTreeComboLoad == FALSE)
		nViewState = (bState == FALSE)?SW_SHOW:SW_HIDE;

	m_btnFolderEdit.ShowWindow(nViewState);
	m_btnFileAdd.ShowWindow(nViewState);
}

void CConditionListView::OnSizeSearchBar(CRect reClient)
{
	CRect reTop = reClient;
	int btnWidth = 20;

	reTop.left = reTop.right - btnWidth;
	m_btnCollaspe.MoveWindow(reTop);
	
	reTop.right = reTop.left - 2;
	reTop.left = reTop.right - btnWidth;
	m_btnNext.MoveWindow(reTop);
  	
	reTop.right = reTop.left - 2;
	reTop.left = reTop.right - btnWidth;
	m_btnPre.MoveWindow(reTop);
  	
	reTop.right = reTop.left - 2;
	reTop.left = reTop.right - btnWidth;
	m_btnSearch.MoveWindow(reTop);

	reTop.right = reTop.left - 2;
	reTop.left = reClient.left;
	reTop.bottom = reTop.top + 100;
	m_comboSearch.MoveWindow(reTop);
}

void CConditionListView::OnSizeMenuBar(CRect reClient)
{
	CRect reLeft, reRight;
	reClient.top += 2;
	reClient.bottom -= 2;

	reLeft = reClient;
	/*reLeft.left += 2;*/

	reLeft.right = reLeft.left + 50;
	m_btnTypeNormal.MoveWindow(reLeft);

	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 60;
	m_btnTypeSort.MoveWindow(reLeft);

// 	reRight = reClient;
// 	reRight.right -= 2;
// 	reRight.left = reRight.right - 20;
// 	m_btnCollaspe.MoveWindow(reRight);

	return;

	//CRect reLeft, reRight;
	reClient.top += 2;
	reClient.bottom -= 2;

	reLeft = reClient;
	reLeft.bottom = reLeft.top + 20;

	reRight = reClient;
	reRight.bottom = reRight.top + 20;

	reLeft.left += 2;
	reLeft.right = reLeft.left + 60;
	m_btnTypeNormal.MoveWindow(reLeft);

	if (m_bActiveTreeComboLoad == TRUE)
	{
		reLeft.top += 24;
		reLeft.bottom = reLeft.top + 20;
	}
	m_btnFolderEdit.MoveWindow(reLeft);

	if (m_bActiveTreeComboLoad == TRUE)
	{
		reLeft.top -= 24;
		reLeft.bottom = reLeft.top + 20;
	}

	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 60;
	m_btnTypeSort.MoveWindow(reLeft);


	reRight.right -= 2;
	reRight.left = reRight.right - 20;
	m_btnCollaspe.MoveWindow(reRight);

	if (m_bActiveTreeComboLoad == TRUE)
	{
		reRight.top += 24;
		reRight.bottom = reRight.top + 20;
	}

	//reRight.right -= 2;
	reRight.left = reRight.right - 60;
	m_btnFileAdd.MoveWindow(reRight);
}

void CConditionListView::SetCallBulletin(CString strURL)
{
	CString url = strURL;
	int showcmd = SW_SHOW;
	
	TCHAR key[MAX_PATH + MAX_PATH];
	
	//First try ShellExecute()
	HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);
	
	//If it failed, get the .htm regkey and lookup the program
	if((UINT)result <= HINSTANCE_ERROR) 
	{		
		if(GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) 
		{
			lstrcat(key, _T("\\shell\\open\\command"));
			
			if(GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) 
			{
				TCHAR *pos;
				pos = _tcsstr(key, _T("\"%1\""));
				if(pos == NULL)						   //No quotes found
				{                     
					pos = _tcsstr(key, _T("%1"));      //Check for %1, without quotes 
					if(pos == NULL)                    //No parameter at all...
						pos = key+lstrlen(key)-1;
					else
						*pos = '\0';                   //Remove the parameter
				}
				else
					*pos = '\0';                       //Remove the parameter
				
				lstrcat(pos, _T(" "));
				lstrcat(pos, url);
				
				USES_CONVERSION;
				result = (HINSTANCE) WinExec(T2A(key),showcmd);
			}
		}
	}
}

LONG CConditionListView::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
	HKEY hkey;
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);
	
	if (retval == ERROR_SUCCESS) 
	{
		long datasize = MAX_PATH;
		TCHAR data[MAX_PATH];
		RegQueryValue(hkey, NULL, data, &datasize);
		lstrcpy(retdata,data);
		RegCloseKey(hkey);
	}

	return retval;
}

void CConditionListView::SetAddTreeMark(XMLTreeInfo stXMLTreeInfo)
{
	if (m_nViewType_Tree == 0)
	{
		m_treeCommonCtrl.SetAddMark(stXMLTreeInfo);
		m_treeCommonCtrl_Sort.SetAddMark(stXMLTreeInfo);
	}
	else if (m_nViewType_Tree == 1)
	{
		m_treeOrderCtrl.SetAddMark(stXMLTreeInfo);
		m_treeOrderCtrl_Sort.SetAddMark(stXMLTreeInfo);
	}
}

void CConditionListView::SetDelTreeMark(XMLTreeInfo stXMLTreeInfo)
{
	if (m_nViewType_Tree == 0)
	{
		m_treeCommonCtrl.SetDelMark(stXMLTreeInfo);
		m_treeCommonCtrl_Sort.SetDelMark(stXMLTreeInfo);
	}
	else if (m_nViewType_Tree == 1)
	{
		m_treeOrderCtrl.SetDelMark(stXMLTreeInfo);
		m_treeOrderCtrl_Sort.SetDelMark(stXMLTreeInfo);
	}
}

void CConditionListView::SetControlEnable(BOOL bState)
{
	m_btnNext.EnableWindow(bState);
	m_btnPre.EnableWindow(bState);
	m_btnSearch.EnableWindow(bState);
	m_btnCollaspe.EnableWindow(bState);

	m_ctrlMenuBar;
	m_btnTypeSort.EnableWindow(bState);
	m_btnTypeNormal.EnableWindow(bState);
	m_btnFolderEdit.EnableWindow(bState);
	m_btnFileAdd.EnableWindow(bState);
		
	m_editHelp.EnableWindow(bState);
	m_btnHelpView.EnableWindow(bState);

	Invalidate();
}

void CConditionListView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;	
	
	HPEN hOldPen;
	hOldPen = (HPEN)dc.SelectObject(pwndMainFrm->GetAxPen(pwndMainFrm->GetAxColor(13), 1, PS_SOLID));

	MoveToEx(dc.m_hDC, m_rtTab.right - 1, m_rtTab.top - 1, NULL);
	LineTo(dc.m_hDC, m_rtTab.right - 1,  m_rtTab.bottom + 1);

	MoveToEx(dc.m_hDC, m_rtTab.right - 1, m_rtTab.bottom + 1, NULL);
	LineTo(dc.m_hDC, m_rtTab.left, m_rtTab.bottom + 1);

	MoveToEx(dc.m_hDC, m_rtTab.left, m_rtTab.bottom + 1, NULL);
	LineTo(dc.m_hDC, m_rtTab.left, m_rtTab.top - 1);
	dc.SelectObject(hOldPen);
}

HBRUSH CConditionListView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
  
	if (nCtlColor == CTLCOLOR_STATIC)
	{
// 		pDC->SetBkMode(TRANSPARENT);
// //		pDC->SetBkColor(COLOR_TAB_CANVAS);
// 		pDC->SetBkColor(m_clrStaic);		
// 		hbr = m_brushBK;
	}

	if(pWnd->GetDlgCtrlID() == IDC_BTN_TREETYPE_NORMAL ||
		pWnd->GetDlgCtrlID() == IDC_BTN_TREETYPE_SORT)
	{
		CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;				
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(pwndMainFrm->GetAxColor(66));		
		hbr = m_brushBKTab;
	}
    
	return hbr;
}



// void CConditionListView::ChangeSkinColor(COLORREF clrSkin)
// {
// //	m_clrMainSkin = COLOR_TAB_CANVAS;
// 	m_clrMainSkin = RGB(213, 213, 213);		// KB투자증권
// }

// void CConditionResultWnd::ChangeSkinColor(COLORREF clrSkin)
// {
// 	if(!clrSkin)
// 		return;
// 
// 	m_clrMainSkin = clrSkin;
// 
// 	if(m_brushBk.DeleteObject())
// 		m_brushBk.CreateSolidBrush(m_clrMainSkin);
// 
// 	Invalidate();
// }

void CConditionListView::ChangeSkinColor(COLORREF clrSkin, ST_TABCOLOR stTabColor)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	
	if(!clrSkin)
		return;
	m_clrMainSkin = clrSkin;

	if(m_brushBK.DeleteObject())
		m_brushBK.CreateSolidBrush(stTabColor.clrBACK);	

	if(m_brushBKTab.DeleteObject())
		m_brushBKTab.CreateSolidBrush(pwndMainFrm->GetAxColor(66));

	m_clrStaic = stTabColor.clrBACK;

	m_brushBK.CreateSolidBrush(m_clrMainSkin);

 	m_ptabCondition->SetTabColor(m_clrMainSkin, m_clrMainSkin, stTabColor.clrB, NULL, NULL, m_clrMainSkin);
 	m_ptabCondition->SetTextColor(stTabColor.clrSF, stTabColor.clrSF);
	Invalidate();

}


void CConditionListView::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CView::OnSettingChange(uFlags, lpszSection);

	//Invalidate();
}

BOOL CConditionListView::OnEraseBkgnd(CDC* pDC)
{
	CRect reClient;
	GetClientRect(reClient);

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	
	CRgn rgn;
	rgn.CreateRectRgnIndirect(reClient);

	CRgn rgnTab;
	rgnTab.CreateRectRgnIndirect(m_rtTab);

	rgn.CombineRgn(&rgn, &rgnTab, RGN_DIFF);

	pDC->FillRgn(&rgn, &m_brushBK);
	
	CRgn rgnTree;
	rgnTree.CreateRectRgnIndirect(m_reTree);

	rgnTab.CombineRgn(&rgnTab, &rgnTree, RGN_DIFF);
	
	pDC->FillRgn(&rgnTab, &m_brushBKTab);

	return FALSE;
}

// 20110222 박찬모 사용자 조건 오름차순 정렬	>>
BOOL CConditionListView::GetSaveFlag()
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CConditionMakeWnd *pendConditionMake = (CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE");

	if(!pendConditionMake) return FALSE;
	return pendConditionMake->GetSaveFlag();
}

CString CConditionListView::GetSaveFileName()
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CConditionMakeWnd *pendConditionMake = (CConditionMakeWnd*)pwndMainFrm->GetWnd("MAKE");

	if(!pendConditionMake) return "";
	return pendConditionMake->GetSaveFileName();
}
// 20110222 박찬모 사용자 조건 오름차순 정렬	<<

void CConditionListView::SetShowTree(BOOL bShow)
{
	if(bShow)
	{
		m_ptreeActive->ShowWindow(SW_SHOW);
		m_ptreeActive->MoveWindow(m_reTree);
	}
	else
	{
		m_ptreeActive->ShowWindow(SW_HIDE);
		m_ptreeActive->MoveWindow(CRect(0, 0, 0, 0));
	}
}