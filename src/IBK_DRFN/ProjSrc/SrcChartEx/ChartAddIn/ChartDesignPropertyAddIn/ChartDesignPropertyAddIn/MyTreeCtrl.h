#if !defined(AFX_MYTREECTRL_H__9A417DA4_31C4_11D2_A335_0060088E1AE4__INCLUDED_)
#define AFX_MYTREECTRL_H__9A417DA4_31C4_11D2_A335_0060088E1AE4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MyTreeCtrl.h : header file
//

#include <afxtempl.h>

#include "ChartDrop.h"
#include "ChartLink.h"
#include "resource.h"
#include "DlgGraphSetting.h"
#include "DlgBlockHorizontalScale.h"

#include "LineColor.h"
/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl window

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.26
//
//	수정	:	(2003.12.26) m_pChartRoot는 상항 생성하므로 Pointer가 아니라 Object로 관리한다.
///////////////////////////////////////////////////////////////////////////////
class CMyTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CMyTreeCtrl();

// Attributes
public:
	HTREEITEM		m_hTarget;
	CChartDrop		m_dropTarget;
	COleDataSource	m_dragSource;
	BOOL			m_DoDrag;

	// (2003.12.26, 배승원) m_pChartRoot는 상항 생성하므로 Pointer가 아니라 Object로 관리한다.
	CChartLink		m_clChartRoot;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetScaleCompartTypeToAllBlocks(const int nScaleCompartType);
	//set All Item Info of PacketPage.
	void SetAllItemInfoOfPacketPage(const CString& strAllItemInfoOfPacketPage);
	//save received colorvalue from ocx
	void SetColorOfLine(const int nLineIndex, const CString& strItemValue);

	void InitBlock(const CString& strData, const int nRow, const int nColumn); //block 생성..
	//ocx에서 받은 정보를 Tree에 적용할때 어느 블럭에 넣을건지 적용.
	HTREEITEM AddBlock(HTREEITEM hItem, LPSTR name);
	HTREEITEM AddItem(HTREEITEM hItem, LPSTR name);

	void ChangeBlockData(const int nBlock,
		          const CString& szScaleData,
				  const int nHorizonPos,
				  const int nScaleCompart,
				  const CString& szDate,
				  const CString& time,
				  const int nHorizonStyle,
				  const int nLog,
				  const int nOverTurn);

	CChartLink* AddVerticalScaleItem(	const CString &	p_strScaleData,
										const int		p_nScalePos,
										const int		p_nScaleCompart,
										const int		p_nScaleExpressionType,
										const BOOL		p_bMinMaxLock,			// Lock		// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
										const int		p_nMinMaxType,			// Type
										const double	p_dMin,					// Min
										const double	p_dMax,					// Max
										const int		p_nUpSpace,				// UpSpace		// (2007/7/19 - Seung-Won, Bae) Support Up/Down Space
										const int		p_nDownSpace,			// DownSpace
										const double	p_dHogaNumber,			//2007.01.22 Scale사용자 설정,호가단위별
										const double	p_dUserNumber,
										const int		p_nUserHogaType,
										const BOOL		p_bIsInvert				// 지표 뒤집기 - ojtaso (20071023)
									 );

	void AddItem(CChartLink* pVertiScale,
						  const CString& strName,
						  const CString &p_strIndicatorName,
						  const CString &p_strGraphName,			// (2006/11/29 - Seung-Won, Bae) Support Graph Name for Binding with Indicator Info.
						  const CString& strPacket,
						  const bool bLowHighUpDown,
						  const bool bLowNowUpDown,
						  const bool bHighNowUpDown,
						  const bool bPreNowContrast,
						  const int nCurData,
						  //sy 2006.04.17.
						  const bool bSignalUse,
						  const bool bSignalShow,
						  //sy end
						  const CString& strColorValue);

	//설정한 모든 값을 하나의 String으로 만든다.
	CString GetBlockandGraphAllData();

	//init TreeCtrl
	void InitTreeView();
	void InitDropTarget();
	virtual ~CMyTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTreeCtrl)
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTitlescalecolor();
	afx_msg void OnDelete();
	afx_msg void OnHorizonscale();
	afx_msg void OnVertScale();
	afx_msg void OnVertDelete();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChartinvert();
	//}}AFX_MSG
	LONG OnDragChartData(UINT wParam, LPARAM lParam);
	LONG OnDropChartData(UINT wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	void OnDeleteVertScale(CChartLink* pVertScale);
	//선칼라 관리 클래스.
	CLineColor m_LineColor;
	//Packet의 모든 정보가 넘어온다.
	CString m_strAllItemInfoOfPacketPage;
	//현재 Active한 블럭의 포인터.
	CChartLink* m_pChartBlock;
	CImageList m_ImageList;
	//Drag 하기 위해 선택된 Item.
	CChartLink *m_pSelectItem;

	HTREEITEM AddVertScale(HTREEITEM hItem, LPSTR name);
	//Block삭제
	void DeleteBLOCK_OnlyTreeItem(CChartLink *pLink);
	//Graph삭제
	void DeleteGRAPH_OnlyTreeItem(CChartLink *pLink);
	void DeleteVERT_SCALE_OnlyTreeItem(CChartLink *pLink);

	int GetimageofIndicatorTypeIndex(LPSTR strGraphType);
	
	//get SubGraph title.
	CString GetTitle(const CString& strTitle);
	//Root에서 설정한 값을 Root에 저장.
	void SetDlgValueToCChartLinkClass(CDlgBlockHorizontalScale& dlgBlockHorizonScale, CChartLink *pItem);
	//Root에서 설장한 값을 모든 블럭에 적용.
	void SetRootValueToAllBlocks(CChartLink* pItem);
	//아이템이 가지고 있던 설정값들을 Dialog에 적용.
	void SetDlgBlockHorizontalScale(CChartLink *pItem, CDlgBlockHorizontalScale& dlgBlockHorizonScale);

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
protected:
	IGraphInfoManager *	m_pIGraphInfoManager;
public:
	void SetIGraphInfoManager( IGraphInfoManager *p_pIGraphInfoManager);

// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
protected:
	CScaleBaseData::SCALEGRIDTYPE	GetFirstVScaleGridType( CChartLink *p_pLink);

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd);

//@solomon[sashia]	20090902 --- 기능 눈에 띄게 --------->>
public:
	CChartLink*		GetSelectItem(HTREEITEM hItem)	{ return m_clChartRoot.Find(hItem);	};
//@solomon[sashia]	20090902 --- 기능 눈에 띄게 ---------<<
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTREECTRL_H__9A417DA4_31C4_11D2_A335_0060088E1AE4__INCLUDED_)
