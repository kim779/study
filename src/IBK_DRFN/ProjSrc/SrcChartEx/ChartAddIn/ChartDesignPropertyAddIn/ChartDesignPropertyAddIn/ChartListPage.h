#if !defined(AFX_CHARTLISTPAGE_H__AFA037A3_DD97_11D4_BCF1_00C026CA36CC__INCLUDED_)
#define AFX_CHARTLISTPAGE_H__AFA037A3_DD97_11D4_BCF1_00C026CA36CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartListPage.h : header file
//

#include "Include_AddIn.h"							// for Ixxxxxx.h
#include "resource.h"
#include "MyTreeCtrl.h"
#include "MyListCtrl.h"
#include "DlgToolTip.h"

////////////////////////////////////////////////////////////////////////////
// CChartListPage dialog

interface IPropertyMap;
class CChartListPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CChartListPage)

// Construction
public:
	void SetProperty( IPropertyMap *p_pIPropertyMap);
	//Add text to HTREEITEM.
	void SetToolTipInfo(HTREEITEM& hItem, const CString& strTip);
	void SetChartListData(const CString& strChartListData);
	void SetRowandColumn(const int& nRow, const int& nColumn);
	//Packet 속성페이지에서 접근해야하는 함수. 
	//Packet에서 선택된 속성들의 정보가 하나의 String으로 들어온다.
	void SetInfoOfAllPacketItems(const CString& strInfoOfAllPacketItems);  

	CChartListPage();
	~CChartListPage();

	//@solomon[sashia]	20090902 --- 기능 눈에 띄게 --------->>
	CChartLink	*m_pSelTreeItem;
	//@solomon[sashia]	20090902 --- 기능 눈에 띄게 ---------<<

// Dialog Data
	//{{AFX_DATA(CChartListPage)
	enum { IDD = IDD_PROPPAGE_CHARTLIST };
	CMyTreeCtrl	m_TreeChart;
	CMyListCtrl	m_listChart;
	//}}AFX_DATA

	//@solomon[sashia]	20090902 --- 기능 눈에 띄게 --------->>
	CButton		m_btnFunc1;
	CButton		m_btnFunc2;
	CStatic		m_stcSelGroup;
	//@solomon[sashia]	20090902 --- 기능 눈에 띄게 ---------<<

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CChartListPage)
	public:
	virtual BOOL OnApply();
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CChartListPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//@solomon[sashia]	20090902 --- 기능 눈에 띄게 --------->>
	afx_msg void OnBtnFunc1();
	afx_msg void OnBtnFunc2();
	afx_msg void OnSelchangedTreeChart(NMHDR* pNMHDR, LRESULT* pResult);
//@solomon[sashia]	20090902 --- 기능 눈에 띄게 ---------<<

private:
	enum
	{
		ONELINE_COLOR = 1,
		TWOLINE_COLOR,
		THREELINE_COLOR,
		FOURLINE_COLOR,
		FIVELINE_COLOR,
		SIXLINE_COLOR
	};

private:
	CDlgToolTip		m_toolTip;
	//ocx에서 받은 트리에 생성할 칼럼갯수
	int m_nColumn;   
	//ocx에서 받은 트리에 생성할 Row갯수
	int m_nRow;  
	//ocx에서 받은 블럭, Item값 저장
	CString m_strChartListData;  
	//Packet페이지의 모든 데이타(체크유무, 항목, 필드명, 길이, 타입,실시간유무)를 하나의 String으로 담을변수
	CString m_strAllItemInfoOfPacketPage;  
	//지표리스트컨트롤에 들어갈 이미지 저장.
	CImageList m_ImageList;  

	//리스트컨트롤 초기화
	void InitList(CListCtrl *pList);  
	void SetListItem(CListCtrl* pList);
	//툴팁생성
	void CreateToolTip();
	//Graph속성 Set 

	//툴팁.
	CMap< HTREEITEM, HTREEITEM&, CString, CString& > tooltipMap;
	CToolTipCtrl m_tooltip;

// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
protected:
	IPropertyMap *	m_pIPropertyMap;

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
protected:
	IGraphInfoManager *	m_pIGraphInfoManager;
public:
	void SetIGraphInfoManager( IGraphInfoManager *p_pIGraphInfoManager);

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTLISTPAGE_H__AFA037A3_DD97_11D4_BCF1_00C026CA36CC__INCLUDED_)
