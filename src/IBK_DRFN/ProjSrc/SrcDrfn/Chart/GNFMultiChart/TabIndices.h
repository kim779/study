#if !defined(AFX_TABINDICES_H__CECDC586_5B5D_4253_A7EB_BCAC5735B6B7__INCLUDED_)
#define AFX_TABINDICES_H__CECDC586_5B5D_4253_A7EB_BCAC5735B6B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabIndices.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTabIndices dialog
#include "TabDlg.h"
#include "./control/XListCtrl.h"
//#include "./control/xSkinButton.h"
#include "xySplitterWnd.h"
//#include "./include/LocalUtil.h"
#include "./control/TreeCtrlEx.h"
//#include "../../../include/commondef.h"


#include "./control/GfxOutBarCtrl.h"
#include "./control/TrTreeCtrl.h"
#include "../../inc/ExOCXDLL.h"
#include "afxcmn.h"

class CTabIndices : public CTabDlg
{
// Construction
public:	
	void Init_Button();
	//지표 돌려보기를 진행할지 여부를 판단.==============================================
	BOOL			m_bRun; 
	void			SetRunMode(BOOL bRun) {m_bRun = bRun;}
	//===================================================================================



	// 환형큐로 구현하는게 좋을 듯.
	// 지표 돌려보기 위한 현재 선택된 항목의 앞 뒤 포지션값. 2005. 05. 18. sy, nam
	POSITION		m_posCur, m_posPrev, m_posNext; 	
	int				LookUpStartIndex(int nSelIndex);
	void			MakeCheckedJipyoList();
	CStringList     m_listCheckedJipyo;
	void			ClearCheckJipyoList();
	CString			m_strOldJipyo;					// 기존에 지표 돌려보기 항목에 추가된 내역.
	int				m_nOldSelJipyoIndex;
	int				m_nNextJipyoIndex;
	void Move_Selected_JipyoItem(int nRecvItem = -1);// nRecvItem : 기존에 선택된 항목 Index, 없는 경우 -1
	CTabIndices(CWnd* pParent = NULL);				// standard constructor
	CySplitterWnd   m_ySplitterY1;
	CXListCtrl	m_listItems;
	BOOL		m_bShowJipyoItem; //2005. 05. 11 - 지표 돌려보기 할 항목을 보여줄지 결정한다.
	BOOL		m_bBack; // 지표 돌려보기 순서 

	CGfxOutBarCtrl		m_ctrlOutBar;
	CList <CTrTreeCtrl*, CTrTreeCtrl*> m_ListTree;

//007 {{
	BOOL			m_bFindExtend;			// 지표검색리스트 확장여부
	
	void			GetFindItemList(int nFindType, CString strFindData, CStringArray &strResult);		// 지표검색
//007 }}

	virtual void ChangeSkinColor(COLORREF clrBkgrnd);


//XListCtrl	m_listIndices;
// Dialog Data
	//{{AFX_DATA(CTabIndices)
	enum { IDD = IDD_TABINDICES };
	CExStatic		m_stTitle;
	//007 {{ 지표검색
	CEdit			m_editFind;		
	CILButton		m_btnFind;
	CILButton		m_btnExtend;
	CListBox 		m_listFind;		//지표검색결과 리스트
	//007 }} 
	CButton			m_RadioAuto;
	CButton			m_RadioManual;
	CILButton		m_btnStop;
	CILButton		m_btnPrev;
	CILButton		m_btnNext;
	CComboBox		m_cmbTime;
	CILButton		m_btnShowList;
	CILButton		m_btnDelItem;
	CILButton		m_btnAddItem;
	CILButton		m_btnSearch;
	CILButton		m_btnApply;
	//CTreeCtrlEx	m_treeItems;	
	CTrTreeCtrl		m_treeItems;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabIndices)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
private:	
//	CList<CUMaster, CUMaster>*			m_pListU;	// KOSPI업종지수
//	CList<CUPMaster, CUPMaster>*		m_pListUP;	// KOSPI200업종지수
//	CList<CKUMaster, CKUMaster>*		m_pListKU;	// KOSDAQ업종지수
//	CList<CUDMaster, CUDMaster>*		m_pListUD;  // 기타업종지수

	CStringArray m_arKOSPIUpCode	, m_arKOSPIUpName;			// KOSPI업종지수
	CStringArray m_arKOSPI200UpCode	, m_arKOSPI200UpName;		// KOSPI200업종지수
	CStringArray m_arKOSDAQUpCode	, m_arKOSDAQUpName;			// KOSDAQ업종지수
	CStringArray m_arETCUpCode		, m_arETCUpName;			// 기타업종지수

	BOOL	m_bLBtnDown;
	BOOL	m_bDragNDrop;
	long	m_nHeightOfIndices;
	long	m_nIndexSelected;
	void	SetItemForm();
	CRect	GetRectOfCtrl(INT nID,BOOL bInit=FALSE);
	void	RecalcLayout();
	void	SetUpjongDataTree();
	void	SetTechnicalIndicator();
	void	SetMarketIndicator();
	void	SetConfig(BOOL bLoad = TRUE);
	LONG	OnWmSplitterMoved(UINT wParam, LONG lParam );
	void	RemoveAllTree();
	void	RefreshIndex();

	// 이미지
	CImageList		m_ImageListIcon;
	//20061213 정유진 수정
	//LeftBar 트리뷰헤더 이미지 수정
	CImageList		m_ImageOutbarIcon;
	//20061213 수정 end
// Implementation

protected:
	// Generated message map functions
	//{{AFX_MSG(CTabIndices)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtapply();
	afx_msg void OnBtsearch();
	afx_msg void OnDblclkTreeitems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnBegindragTreeitems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBegindragListitems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListitems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnShowRepeat();
	afx_msg void OnBtnNext();
	afx_msg void OnBtnPrev();
	afx_msg void OnBtnStop();
	afx_msg void OnSelchangeCmbTime();
	afx_msg void OnRadioAuto();
	afx_msg void OnRadioManual();
	afx_msg void OnBtnAdditem();
	afx_msg void OnBtnDelitem();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnBnClickedBtnExtend();
	afx_msg void OnLbnSelchangeListFind();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg void OnSendMenu(UINT nIndex);
	DECLARE_MESSAGE_MAP()	
//	afx_msg void OnItemchangedListindices(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnClickListindices(NMHDR* pNMHDR, LRESULT* pResult);
	
public:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABINDICES_H__CECDC586_5B5D_4253_A7EB_BCAC5735B6B7__INCLUDED_)
