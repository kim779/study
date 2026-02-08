#if !defined(AFX_XMLTREECTRL_H__32FFC677_E076_41D4_AA99_15D1DF2EE4CD__INCLUDED_)
#define AFX_XMLTREECTRL_H__32FFC677_E076_41D4_AA99_15D1DF2EE4CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XmlTreeCtrl.h : header file
//

/**********************************************************************/
/*																																		*/
/*												XML Load  Control														*/
/*													(Mouse Control)														*/
/*																																		*/
/*												 작성일 : 2003. 11~													*/
/*							       작성자 : 우동우 (TEXEN SYSTEM)									*/
/*																																		*/
/**********************************************************************/
//#
//#   # Command
//#     각 파일 또는 file folder의 내용을 List로 구성하고 Tree에 Insert한다.
//#
//#   # UPDATA
//#			2004. 05. 20 => Alert 정보 삽입, Alert 구분자 삽입
//#
//#			2004. 11. 20 => User/Order file 정보 구분 관리
//#									 => Mark 등록 기능 삽입.
//#			2004. 2. 15  => Explain Load 구조 변경!
//#			2006. 9.     => 복합 지표 구성 기능 추가.(Common/Order + File[SYS/USER])


#import "msxml.dll"
using namespace MSXML;
#include <afxtempl.h>   //<= CList

#include "../DefineFiles/client_typedef.h"

const UINT RMSG_XMLTREE_CHECKMENU = ::RegisterWindowMessage(_T("RMSG_XMLTREE_CHECKMENU"));

/////////////////////////////////////////////////////////////////////////////
// CXmlTreeCtrl window
class CXmlTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CXmlTreeCtrl();
	virtual ~CXmlTreeCtrl();

//# Set Out Class Values
public :
	CString GetFilePath(CString strLoadType);
	CString GetFolderPath(CString strLoadType);

	BOOL			m_bAllItemView;
	CString			m_strSelectItemText;

	BOOL			m_bLoadFlag;

	CList <PortGroupInfo, PortGroupInfo&> *m_plistPortInfo;
	CList	<LanguageInfo, LanguageInfo&>   *m_plistLanguageInfo;

	int             m_nSeleteClass;     //# -1 : Class, 0 : Item, 1 : File
	int				m_nSeleteType;
	XMLTreeInfo		m_stSelectInfo;
	XMLTreeInfoFile	m_stSelectInfoFile;
	BOOL MoveUserXmlFile(CString strFromDir, CString strToDir, CString strFileName);
	BOOL GetCheckSameFile(CString strFolderName, CString strFileName);

	int				m_nCaptureItem;	//# 0 : 사용자, # 1 : 즐겨찾기

	void ChangeMarkSort(CString strTarget, CString strBefore, HTREEITEM hItemMarkRoot);

private:
	CString			m_strAppPath;
  						
	BOOL			m_bIndustry;
	BOOL            m_bIsFnUser;

	CString			m_strLoadType;	//# 0 : Common, 01 : Common Sort, 02_ : Common Mark
									//# 1 : Order, 11 : Order Sort, 12_ : Order Mark,
									//# 2 : Recommend, 3 : user, 4 : Language, 5 : Recommend + user
	BOOL			m_bSendMsgThis;
	BOOL			m_bJustAlert;
	int				m_nUseBookMarkType;		//# 0 : Not User, 1 : Read, 2 : Read/Write
		

	CWnd			*m_pwndMsgTarget;
	BOOL			m_bEditModify;

	HTREEITEM		m_htiRoot;
	HTREEITEM		m_htiPortPolio;
	HTREEITEM		m_htiAccount;
	HTREEITEM		m_htiLanguage;
	HTREEITEM		m_htiPatten;
	HTREEITEM		m_htiNews;
	HTREEITEM		m_htiBookMark;

	HTREEITEM		m_htiRecomm;
	HTREEITEM		m_htiUser;

	CImageList*		m_pDragImage;
	BOOL			m_bLDragging;
	HTREEITEM		m_hitemDrag;
	HTREEITEM		m_hitemDrop;

	HCURSOR			cursor_hand;
	HCURSOR			cursor_arr;
	HCURSOR			cursor_no;
	bool			isCursorArrow;
	void			SetDefaultCursor();
	HTREEITEM		MoveChildItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter);

	int		m_nXmlListIndex;
	CList	<XMLTreeInfo, XMLTreeInfo&> m_listXMLTreeInfo;
	int		m_nXmlMarkListIndex;
	CList	<XMLTreeInfo, XMLTreeInfo&> m_listXMLTreeMarkInfo;

	CList	<NewsInfo, NewsInfo&> m_listNewsInfo;
	CList	<XMLTreeInfoFile, XMLTreeInfoFile&> m_listXMLTreeInfoFile;

	int		m_nXmlUserListIndex;
	CList	<XMLTreeInfoFile, XMLTreeInfoFile&> m_listUserInfoFile;

	int m_nXmlSysListIndex;
	CList	<XMLTreeInfoFile, XMLTreeInfoFile&> m_listSysInfoFile;

	int		m_nXmlItemExcavationIndex;
	CList	<XMLTreeInfoFile, XMLTreeInfoFile&> m_listItemExcavationInfoFile;
		
	CStringArray	m_saAccountInfo;

public:
	BOOL Create(CWnd* pParentWnd, UINT nID, CImageList *pListImage,
              CString strLoadType, 
							BOOL bSendMsgThis = TRUE, 
              BOOL bJustAlert = FALSE, 
              int nUseBookMarkType = 0, //# 0 : Not User, 1 : Read, 2 : Read/Write
              BOOL bIndustry = FALSE,
              BOOL bIsFnUser = TRUE);
	
	void SetItme(BOOL bModify, CString strItme, BOOL bAlertReg, BOOL bUnitCan);
	void SetItme_User(BOOL bModify, XMLTreeInfoFile	stXMLTreeInfoFile);

	void SetDeleteItme(CString strItme);
	
	int GetFindTreeItem(HTREEITEM htItemNode, CString strFindText, 
								  CArray<HTREEITEM, HTREEITEM> &saFindItemArray);

	BOOL GetFindTreeItem(HTREEITEM htItemNode, CString strFindText, HTREEITEM &hItem);

	void SetItemAllView(BOOL bAllItemView);
	

	void SetReloadPortPolio(BOOL bClear = TRUE);//tour2k
	void SetReloadUserFile(CList <XMLTreeInfoFolder, XMLTreeInfoFolder&> *plistUserInfoFolder);
	void SetReloadUserFile(CList <XMLTreeInfoFile, XMLTreeInfoFile&> *plistUserInfoFile);
	void SetLoadExcavationFile(CList <XMLTreeInfoFolder, XMLTreeInfoFolder&> *plistUserInfoFolder);
	void SetReloadLanguage();
	void SetReloadUserPatten();

	void SetAddMark(XMLTreeInfo stXMLTreeInfo);
	BOOL SetDelMark(XMLTreeInfo stXMLTreeInfo);

	void GetSelectItem(CPoint point = NULL);
	BOOL GetSelect_File(XMLTreeInfoFile &stXMLTreeInfoFile);

	BOOL GetCheckComboLoad();

	BOOL SetTreeView();

	void SelectTreeItemCondition(HTREEITEM htree);
	int SetSelectDataManual(HTREEITEM htree);
	BOOL GetSelectItem(XMLTreeInfo &stXMLTreeInfo, HTREEITEM htree);

	BOOL SetSelectItemExcavation_Manual(HTREEITEM hItem);
	int GetSelectFile(HTREEITEM hItem);

	void SetExpandLanguage();

private:
	//#DLL
	//typedef void	(* FpGetRemainCode)(LPSTR, CStringArray*);
	BOOL GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom);
	
	CString GetPath(int nComm);
	void SetInit();

	BOOL SetLoadXml();
	BOOL SetLoadXml_File();
	BOOL SetLoadXml_File(CString strLoadType, HTREEITEM htiParent);    //# Add : 2006.09
	
	void SetInitTree(MSXML::IXMLDOMNodeList *pNodes, HTREEITEM hti);
	void SetInitTreeFile(MSXML::IXMLDOMNodeList *pNodes, HTREEITEM hti, 
		CString strLoadType = "", CString strFoldPath = "");
	HTREEITEM SetTreeItem(HTREEITEM htiParent, CString strItem, 
		int nImgIndex, int nImgSelIndex, int lParam);
	
	BOOL SetTreeItem_Update(HTREEITEM htiItem, CString strItem,
		int nImgIndex, int nImgSelIndex);
	
	void SetIconLoad();
	int GetTreeIconIndex_File(BOOL bAlertReg, BOOL bUnitCan);
	
	int GetSelectItem(CString &strItem, int &nItemIndex, CPoint point);
	BOOL GetSelectItem(XMLTreeInfo &stXMLTreeInfo, CPoint point);
	int GetSelectFile(XMLTreeInfoFile &stXMLTreeInfoFile, CPoint point);
	
	BOOL SetDeleteTreeItem(CString strItem);
	BOOL SetDeleteFile(CString strItem);
	
	
	void SetSelectData_Send(CString strMode, CPoint point);
	int SetSelectData(CPoint point);
	
	//# PortPolio
	void SetLoadPortPolio();
	void SetPortPolioTree();
	XMLTreeInfo GetPortToTreeInfo(PortGroupInfo stPortGroupInfo);
	
	//# Language
	void SetLoadLanguage();
	void SetLanguageTree(BOOL bUpDate = FALSE);
	XMLTreeInfo GetLanguageToTreeInfo(LanguageInfo stLanguageInfo);
	
	//# Account
	void SetLoadAccount();
	void SetAccountTree(BOOL bUpDate = FALSE);
	
	//# News
	void SetLoadNews();
	void SetNewsTree(BOOL bUpDate = FALSE);
	XMLTreeInfo GetNewsToTreeInfo(NewsInfo	stNewsInfo);
	
	//# Patten
	void SetLoadUserPatten(CStringArray &saUserPatten);
	XMLTreeInfo GetPattenToTreeInfo(CString strName);
	
	//# Mark
	void SetSendPopupMenu(int nType, CPoint ptMenu, HTREEITEM hSelItem=NULL);
	BOOL GetHaveTreeMarkInfo(XMLTreeInfo stXMLTreeInfo, POSITION &posMark);
	
	BOOL SetSaveXmlMark(CString strLoadType, CList <XMLTreeInfo, XMLTreeInfo&> *plistXMLTreeInfo);
	BOOL SetLoadXmlMark(CString strLoadType);
	void SetLoadXmlMark(CString strLoadType, MSXML::IXMLDOMNodeList *pNodes, CList <XMLTreeInfo, XMLTreeInfo&> *plistXMLTreeInfo);
	void SetInitTreeMark(CList <XMLTreeInfo, XMLTreeInfo&> *plistXMLTreeInfo);
	
	CString GetItemPath(HTREEITEM hSelectItem);
	
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = PROGRAM_TITLE, UINT nType = MB_OK);
	
	
	BOOL SetInitTreeList(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pFileList);
	void SetItemChildView(HTREEITEM hItem, BOOL bAllItemView);
	BOOL SetSaveXmlFile(CString strLoadType);
	
	void SetControlState(BOOL bState);

	// 랭귀지 TREE의 서브인지 확인하기 위해서..
	BOOL IsParentLanguage(HTREEITEM hItem);

	// Drag & Drop
	HTREEITEM DragItem(HTREEITEM hDragItem, CPoint point);

	void TreeInfoSort(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *plistUserInfoFile); // 20110222 박찬모 사용자 조건 오름차순 정렬	>>

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXmlTreeCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	

	// Generated message map functions
protected:
	//{{AFX_MSG(CXmlTreeCtrl)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	afx_msg void OnSendMenu(UINT nIndex);
	afx_msg void OnNotify_ReloadLanguage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMLTREECTRL_H__32FFC677_E076_41D4_AA99_15D1DF2EE4CD__INCLUDED_)
