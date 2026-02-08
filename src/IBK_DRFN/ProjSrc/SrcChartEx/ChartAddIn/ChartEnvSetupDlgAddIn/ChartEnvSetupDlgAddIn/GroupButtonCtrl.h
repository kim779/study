// GroupButtonCtrl.h: interface for the CGroupButtonCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GROUPBUTTONCTRL_H__09E85A91_5C64_4301_AD3E_6C4CBF9D6A6C__INCLUDED_)
#define AFX_GROUPBUTTONCTRL_H__09E85A91_5C64_4301_AD3E_6C4CBF9D6A6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <AFXTEMPL.H>

#include "Include_AddIn.h"			// for IGraphInfoManager

class CDlgMainTabIndicatorSetUp;
class CIndicatorNameListCtrl;
class CIndicatorGroupButton;


enum BTN_TYPE { GENERAL_GROUP, USE_GROUP };

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 11
// Comments		: 
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CGroupButtonCtrl
{
// Construction & Destruction
public:
	CGroupButtonCtrl();
	virtual ~CGroupButtonCtrl();


// Attribute
public:
	CFont m_font;				// Group버튼의 폰트

	CDlgMainTabIndicatorSetUp *m_pDlgMainTabIndicatorSetUp;				// 부모윈도우의 포인터
	CRect m_rcTotalRegion;

	CRect m_rcFirstIndicatorBtn;

	CList< CIndicatorNameListCtrl *, CIndicatorNameListCtrl * > m_lstListCtrl;	// ListCtrl의 List
	CList< CIndicatorGroupButton *, CIndicatorGroupButton * > m_lstGroupButton;	// 탭버튼의 List

protected:
	IPacketManager *	m_pIPacketManager;

// Implementaion
public:
	void SetData( CWnd *pWnd);			// 필요 변수값 세팅.
	
	// ( 2007 / 01 / 08 - Sang-Woo, Cho ) - GroupCtrl의 종류에 따라 내부적으로 다른 함수를 사용함.
	void CreateGeneralCtrl( IStringList * p_pIndicatorNameList, IChartOCX *p_pIChartOCX );
	void CreateUseCtrl( IStringList * p_pIndicatorNameList, IChartOCX *p_pIChartOCX);

	void SetIndicatorSize( const CRect &region );
	CDlgMainTabIndicatorSetUp* GetParent() { return m_pDlgMainTabIndicatorSetUp; };	// 부모윈도우를 리턴한다.
	CIndicatorGroupButton* GetSelectGroupButton(int nIndex = -1);		// 현재 선택한 IndicatorGroupButton을 가져옴.
	CIndicatorNameListCtrl* GetSelectListCtrl(int nIndex = -1);			// 현재 선택한 IndicatorGroup의 ListCtrl
	void OnGroupButtonDown(int nIndex, const bool bIsDlgCreat = true);	// 지표그룹버튼을 눌렀을때.
	void ReleaseFocusItem();			// 포커스를 푼다.
	void ReleaseAllFocusSpecialTab();	// 특수지표에 체크된 아이템을 모두 UnCheck한다.
	void SetFocusItem();				// 해당Index에 포커스를 줌.
	void ModifyCheckWhenSpecialIndicator(const CString &strIndicatorName);	// 특수지표는 한개만 체크되도록 한다.
	void SetCheck(IStringList * p_pIndicatorNameList);	// 현재 차트에 Display되어있는 지표명에 체크한다.
	void SetAllUnCheck();				// 지표명 ListCtrl의 모든 아이템을 UnCheck한다.
	void GetFirstCheckItem( CString &p_strIndicatorName, CString &p_strGraphName);		// ListCtrl에서 첫번째로 체크되어 있는 Item의 문자열을 가져옴.(없으면 맨윗놈)
	int GetCount(){ return m_lstListCtrl.GetCount();}	// 리스트컨트롤의 갯수를 가져온다.

private:
	int m_nSelectButtonIndex;	// 눌려진 버튼 인덱스
	int m_nSelectListCtrlIndex;	// 선택한 ListCtrl 인덱스

	void	SetEachCtrlSize( const int nGroupCount, UINT &nID, CRect &rcListCtrlRegion, CRect &rcButtonRegion );
	void	MoveWindowInAllListCtrl(const int nGroupCount, const CRect& curListCtrlRegion);
	bool	CreateListCtrlAndButton(const UINT& nID, const int nIndex, const char *p_szGroupName, const CRect& listCtrlRegion, const CRect& buttonRegion, 
				IStringList * pGraphNameList, IStringList * pIndicatorNameList, const bool bIsSorting = true);
	bool	CreateListCtrl( const UINT& nID, const int nIndex, const CRect& region, const CString& strGroupName, IStringList * pGraphNameList,
				IStringList * pIndicatorNameList, const bool bIsSorting = true);
	void	CreateButton(const UINT& nID, const int nIndex, const CString& strWndText, const CRect& region);
	CRect	GetButtonRegion(const CRect& baseRegion, const int nIndex);
	void	SelectGroupButton(const bool bIsCurSpecialIndicator);
	bool	IsCheckSpecialIndicator(IStringList * p_pIndicatorNameList);	// 현재 특수지표를 이용중인지 확인한다.


protected:
	// ( 2007 / 01 / 09 - Sang-Woo, Cho ) 
	int m_nBtnType;			// General : 0,		Use : 1

public:
	void SetGroupButtonCtrlType( int nBtnType )	{	m_nBtnType = nBtnType;	};
	int GetGroupButtonCtrlType( void )			{	return m_nBtnType;	};
	
	BOOL AddGraph( CString strIndicatorName );
	BOOL RemoveGraph( CString strIndicatorName );

// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
};


#endif // !defined(AFX_GROUPBUTTONCTRL_H__09E85A91_5C64_4301_AD3E_6C4CBF9D6A6C__INCLUDED_)
