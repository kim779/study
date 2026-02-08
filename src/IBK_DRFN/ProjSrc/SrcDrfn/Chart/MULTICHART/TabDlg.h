#if !defined(AFX_TABDLG_H__DEABCEF9_0F89_4177_840F_220B3C36BFEF__INCLUDED_)
#define AFX_TABDLG_H__DEABCEF9_0F89_4177_840F_220B3C36BFEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabDlg dialog
//#define GAP_TABDLG			2
#define HEIGHTOFBUTTON		20
#define HEIGHTOFBUTTON19	19
//#define WIDTHOFBUTTON		40
#include "RscDlg.h"





//=====================================================================================
// 2005. 07. 14 Han Hwa 관심 적용 
//=====================================================================================
//#include "../../COMMON_BASE/Tree/ginttree.h"
//#include "../../COMMON_BASE/Tree/CodeMnager.h"
//#include "../../COMMON_BASE/BaseInfo/BaseInfo.h"
//#include "../../../DrMLib/DrMiddleLib.h"
//#include "../../INCLUDE_BASE/IncMng.h"
//=====================================================================================

//=====================================================================================
// 2005. 07. 30 Han Hwa 조건 검색 
//=====================================================================================
//#include "../../COMMON_BASE/FindLib/EzFinderLib.h"
//=====================================================================================

//=====================================================================================
//이미지 리스트를 쓸 경우 
//=====================================================================================
#include "../chart_common/ILButton.h"
//=====================================================================================

class CLeftBarDlg;
class CStdDialog;
class CTabDlg : public CRscDlg//, public CSharedBase
{
public:
	//2005. 09. 03 add by sy,nam --------------------------------------------
	//ImageList	
	CImageList*   m_pIL_W20H20; //폭 20에 높이 19인 버튼 이미지리스트
	//CImageList*	  m_pIL_W33H19; //폭 33에 높이 19인 버튼 이미지리스트
//@유진삭제 CImageList*   m_pIL_W34H20; //폭 34에 높이 19인 버튼 이미지리스트
//@유진삭제 CImageList*   m_pIL_W46H20; //폭 46에 높이 19인 버튼 이미지리스트
//@유진삭제	CImageList*   m_pIL_W50H20; //폭 50에 높이 19인 버튼 이미지리스트
//@유진삭제	CImageList*   m_pIL_W55H19; //폭 55에 높이 19인 버튼 이미지리스트
//@유진삭제	CImageList*   m_pIL_W60H19; //폭 60에 높이 19인 버튼 이미지리스트
//@유진삭제 CImageList*   m_pIL_W88H19; //폭 88에 높이 19인 버튼 이미지리스트

	CImageList*   m_pIL_W151H21; //폭 151에 높이 21인 버튼 이미지리스트
//@유진삭제 CImageList*   m_pIL_W26H23; //폭 26에 높이 23인 버튼 이미지리스트
//@유진삭제 CImageList*   m_pIL_W58H20; //폭 58에 높이 20인 버튼 이미지리스트
//@유진삭제	CImageList*   m_pIL_W82H20; //폭 82에 높이 20인 버튼 이미지리스트
	CImageList*	  m_pIL_W43H20; //폭 43에 높이 20인 버튼 이미지리스트
//@유진삭제 CImageList*	  m_pIL_W43H19; //폭 43에 높이 19인 버튼 이미지리스트
//@유진삭제 CImageList*	  m_pIL_W46H19; //폭 46에 높이 19인 버튼 이미지리스트
//@유진삭제 CImageList*	  m_pIL_W58H19; //폭 58에 높이 19인 버튼 이미지리스트
//@유진삭제 CImageList*	  m_pIL_W72H19; //폭 72에 높이 19인 버튼 이미지리스트
	CImageList*	  m_pIL_W58H20; //폭 58에 높이 20인 버튼 이미지리스트
	CImageList*	  m_pIL_W72H20; //폭 72에 높이 20인 버튼 이미지리스트
//@유진삭제 CImageList*	  m_pIL_W52H19; //폭 52에 높이 19인 버튼 이미지리스트
	//-----------------------------------------------------------------------
	



	//2005. 07. 30 add by sy,nam --------------------------------------------
	//조건 검색 Lib 관련	
	//CEzFinderLib*		m_pEzFinderLib;	
	//-----------------------------------------------------------------------
	
public:	
	//CDrMLib_CDBMgr*			m_pDBMgr;
//=====================================================================================
//2005. 07. 26 added by sy, nam
	//CCodeMnager*	m_pCommonCodeMgr;		
	//CDrMLib_CBaseInfo2*		m_pCommonBaseInfo;
	//virtual long	ChangeSharedData(CString sShareName, BYTE* pData, int nLen, int nStock) { return 0;}
//=====================================================================================


	CTabDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL ); // standard constructor
	virtual ~CTabDlg();
// Dialog Data
	//{{AFX_DATA(CTabDlg)
	enum { IDD = NULL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:	
	virtual void ReceiveData(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID aTRData, long dwTRDateLen) {}
	virtual void SetRealPriceData(void *pData) {}
	virtual void ReceiveDataFromOutSide() {}
	virtual void ChangeSkinColor(COLORREF clrBkgrnd) {}


	CLeftBarDlg *m_pParentDlg;
	CWnd *m_pMultiChartWnd;
	CWnd *m_pChartMng;
protected:
	CBrush	m_brBkColor;
	COLORREF m_crBk;
	void RecalcLayoutCtrl(CWnd &rWnd,int nID =0 );
	virtual CRect GetRectOfCtrl(INT nID, BOOL bInit=FALSE) { return CRect(0,0,0,0); }

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABDLG_H__DEABCEF9_0F89_4177_840F_220B3C36BFEF__INCLUDED_)
