#if !defined(AFX_INDICATORNAMELISTCTRL_H__F5C39E2F_94DD_421C_85BF_E11BBE8EEFE6__INCLUDED_)
#define AFX_INDICATORNAMELISTCTRL_H__F5C39E2F_94DD_421C_85BF_E11BBE8EEFE6__INCLUDED_

#include "BlockBaseData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndicatorNameListCtrl.h : header file
// 04/03/30 종찬: 지표명을 담는 ListCtrl로 GroupButton 한개당 ListCtrl 한개를 생성한다.

#include <AFXTEMPL.H>

#include "Include_AddIn.h"		// for IPacketManager

/////////////////////////////////////////////////////////////////////////////
// CIndicatorNameListCtrl window
class CIndicatorGroupData;
class CGroupButtonCtrl;

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 11
// Comments		: 
// Using way	: 
// See			: class CListCtrl
//-----------------------------------------------------------------------------
class CIndicatorNameListCtrl : public CListCtrl
{
// Construction
public:
	CIndicatorNameListCtrl( CGroupButtonCtrl* pParent, LPCTSTR pcszIndicatorGroupName, int nIndex, IPacketManager *p_pIPacketManager);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndicatorNameListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIndicatorNameListCtrl();

	// Generated message map functions
public:
	//{{AFX_MSG(CIndicatorNameListCtrl)
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// Function
private:
	void InitColumn();	// 칼럼을 초기화한다.
	bool Initialize( IStringList *pGraphNameList, IStringList *pIndicatorNameList);	// ListCtrl 초기화한다.
	bool Initialize_Market( IStringList *pList);
	bool Initialize_PacketTimeType( IStringList * pList);	// ListCtrl 초기화한다.

	void GetDataInPacket( IPacketManager::ICOMMODITY_BOND_NAME& eBondName, CScaleBaseData::HORZSCALEDRAWERTYPE& eTimeType);

	bool IsInitialize_Market(const CString& strGroupName) const;
	bool IsInitialize_PacketTimeType(const CString& strGroupName) const;

	bool IsAddIndicatorName_DLL( void);
	bool IsAddIndicatorName( void);
	bool IsAddIndicatorName( const IPacketManager::ICOMMODITY_BOND_NAME eBondName, 
		const CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType, const char *p_szGraphName);

	bool IsStock(const IPacketManager::ICOMMODITY_BOND_NAME eBondName) const;
	bool IsFuturesOrOption(const IPacketManager::ICOMMODITY_BOND_NAME eBondName) const;
	bool IsDailyOrWeekleyOrMonthly(const CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType) const;
	bool IsDailyOrTimeOrTick(const CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType) const;
	bool IsDaily(const CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType) const;

public:
	bool Initialize( const char *p_szGroupName, IStringList *pGraphNameList, IStringList *pIndicatorNameList);	// ListCtrl 초기화한다.
	void SetCheckSeletecItem(IStringList *IndicatorNameList);	// 챠트에 있는 지표명들은 체크한다.
	void SetAllUnCheck();						// 모든 리스트 체크상태를 초기화(UnCheck)한다.
	int GetIndex(){ return m_nIndex;};			// ListCtrl의 인덱스를 가져온다.
	void SetFocusItem(const int nIndex);		// 해당인덱스에 포커스를 준다.
	void ReleaseFocusItem();					// 포커스를 푼다.
	CString GetGroupName(){ return m_strIndicatorGroupName; };				// 지표그룹명을 리턴한다.
	BOOL GetCheckState(const CString &strIndicatorName, int& nIndex);		// 아이템의 체크상태를 리턴한다.

	// ( 2007 / 01 / 08 - Sang-Woo, Cho ) 
	void ChangeFocusItem( void );
	CGroupButtonCtrl *GetOtherGroupButtonCtrl( void );
	
	BOOL AddGraph( const CString &strIndicatorName, const CString &p_strGraphName);
	BOOL RemoveGraph( int p_nIndex);
	int	 GetSelectItemIndex( void )						{	return m_nSelectItemIndex;	};
	void SetSelectItemIndex( int nSelectItemIndex )		{	m_nSelectItemIndex = nSelectItemIndex;	};
	
	
// Member
private:
	CString m_strIndicatorGroupName;		// 지표그룹명.
	int m_nIndex;							// ListCtrl의 인덱스
	int m_nSelectItemIndex;					// ListCtrl에 선택한 Item의 인덱스
	
	CGroupButtonCtrl *m_pParent;			// 부모의 포인터.

	BOOL m_bInitCheckIndicator;				// 초기설정값에 따라 지표명에 체크표시할 때 지표추가삭제루틴을 호출되지 않게 함.
	IPacketManager *	m_pIPacketManager;

// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDICATORNAMELISTCTRL_H__F5C39E2F_94DD_421C_85BF_E11BBE8EEFE6__INCLUDED_)
