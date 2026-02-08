#if !defined(AFX_XLISTCTRLJONGDATA_H__CF36F974_F7B6_4CF4_B2BB_7F4C20621969__INCLUDED_)
#define AFX_XLISTCTRLJONGDATA_H__CF36F974_F7B6_4CF4_B2BB_7F4C20621969__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XListCtrlJongData.h : header file
//
#include "./control/XListCtrl.h"
//#include "COMMON_BASE/LocalUtil.h"
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CXListCtrlJongData window
class CJongDataInListCtrl
{
public:
	CJongDataInListCtrl() 
	{
		m_chSign = '0';
		m_bHighlight = FALSE;
		m_bCheck = FALSE;
		m_lKeepingCnt = 1;
		m_nCurrentIndex = 0;
		m_bKeeping = FALSE;		
		m_nMarketGb = 1;
	}
	const CJongDataInListCtrl& CJongDataInListCtrl::operator=(const CJongDataInListCtrl &ci)
	{
		m_strJongCode = ci.m_strJongCode;
		m_strCodeName = ci.m_strCodeName;
		m_strPrice = ci.m_strPrice;
		m_strChange = ci.m_strChange;
		m_strChgrate = ci.m_strChgrate;
		m_strVolume = ci.m_strVolume;
		m_chSign = ci.m_chSign;
		m_bCheck = ci.m_bCheck;
		m_bHighlight = ci.m_bHighlight;
		m_lKeepingCnt = ci.m_lKeepingCnt;
		m_nCurrentIndex = ci.m_nCurrentIndex;
		m_bKeeping = ci.m_bKeeping;
		m_strInsertedTime = ci.m_strInsertedTime;
		m_nMarketGb = ci.m_nMarketGb;
		return *this;
	}
	CString m_strJongCode;
	CString m_strCodeName;
	CString m_strPrice;
	CString m_strChange;
	CString m_strChgrate;
	CString m_strVolume;
	char m_chSign;
	BOOL	m_bHighlight;
	BOOL	m_bCheck;
	long	m_lKeepingCnt;
	int		m_nCurrentIndex;
	BOOL	m_bKeeping;
	CString m_strInsertedTime;
	int		m_nMarketGb;
};


class CLeftBarDlg;
class CXListCtrlJongData : public CXListCtrl
{
// Construction
public:
	CXListCtrlJongData();
	void	Init(CLeftBarDlg *pLeftBarDlg, int nType);
	void	InsertCodeData(LPCTSTR lpCode, LPCTSTR lpName,CString strPrice,CString strChange,CString strChgrate,CString strVolume, char chSign, int nMarketGubun=1);//,BOOL bKeeping = FALSE);
	BOOL	UpdateCodeData(LPCTSTR lpCode, LPCTSTR lpName,CString strPrice,CString strChange,CString strChgrate,CString strVolume,char chSign, int nIndex);
	void	RemoveCode(LPCTSTR lpCode);
	CString GetCodeSelected(int nCurSel =-1);
	CString GetTimeSelected(int nCurSel =-1);
	CString GetCheckedCodes();
	void	GetCheckedCodes(CStringArray* arrCodes, CUIntArray* arrMarketGb);
	int		RemoveAll();
	int		FindCode(LPCTSTR lpCode);
	void	GetCodeData(int nIndexIn, CString& rstrPriceOut,CString & rstrChangeOut,CString & rstrChgrateOut,CString & rstrVolumeOut,char& rchSign);
	CString GetSelectedCodes();
	void    CheckJongmokCode(BOOL bCheck, LPCTSTR lpJongmokCode);
	void	CheckJongmokCode(CStringList *plistStrJongmokCodeChecked, CStringList *plistStrJongmokCodeHighlighted);
	void	CheckJongmokCode(CStringList *plistStrJongmokCodeChecked);
	void	UpdateCheck(int nIndex, CWnd *pWnd);
	BOOL	GetCheckByJongmokCode(LPCTSTR lpJongmokCode);
	void	UnHighlightByJongmokCode(LPCTSTR lpJongmokCode);
	void	GetDrawColors(int nItem, int nSubItem, COLORREF& colorText, COLORREF& colorBkgnd);
	void	RemoveNoUpdateItem();
	void	SetCursorPrev(int nCnt, BOOL bRepeat=FALSE);
	BOOL	SetCursorNext(int nCnt, BOOL bRepeat=FALSE);
	void	SetAutoSort(BOOL bAutoSort) { m_bAutoSort = bAutoSort;}
	void	InitSortIndex();
	void	SetAlarmPeriod(long nStartAlarmKeepingCnt,long	nEndAlarmKeepingCnt);
	void    SetAlarmColor(BOOL bUseAlarmBkClr);
	void	SetAlarmSound(BOOL bUseAlarmSound, LPCTSTR lpAlarmSoundFile =NULL);
	CString GetCommentAboutHighlight();
	void    GetListOfChecked(CStringList *pListChecked);

// --> [Edit]  강지원 2008/10/21
	CString	GetCurrentColText( int nCol = 0 );
// <-- [Edit]  강지원 2008/10/21
// Attributes
public:	
	
private:	
//	CList<CJMaster, CJMaster>*			m_pListJ;
//	CList<CKMaster, CKMaster>*			m_pListK;
	CMap<CString, LPCSTR, CJongDataInListCtrl *, CJongDataInListCtrl *>	m_mapJongData;
	CMap<CString, LPCSTR, CJongDataInListCtrl *, CJongDataInListCtrl *>	m_mapJongDataHighlighted;
	CMap<CString, LPCSTR, CJongDataInListCtrl *, CJongDataInListCtrl *>	m_mapJongDataChecked;
	CLeftBarDlg *m_pLeftBarDlg;
	int		m_nType;
	BOOL	m_bSortIndex[10];
	BOOL	m_bSortIndexType[10];
	BOOL	m_bSortIndexConst[10];
	BOOL	m_bAutoSort;
	void	InitColumns();
	void	SetConfig(BOOL bLoad = TRUE);
	void	SortColumns(int nColumnIndex,BOOL bSortDecendFirst =TRUE);
	BOOL	DeleteAllItems() {ASSERT(0); return FALSE;}
	CString GetJongData(int nItem, int nSubItem);
	void	ReplaceIndex(int nIndex1, int nIndex2);
	void	InsertCodeDataLighted(CJongDataInListCtrl *pData, int nIndex);
	BOOL	GetPosOfHighLighted(long *pLStartPos, long *pLEndPos);

//@유진삭제	long	m_lIndexOfCheck;
	long	m_lIndexOfKeepingCnt;
	long	m_lIndexOfName;
	long	m_lIndexOfPrice;
	long	m_lIndexOfChange;
	long	m_lIndexOfChgrate;
	long	m_lIndexOfVolume;
	long	m_lIndexOfTime;

	// buffer For Sort..
	COLORREF m_crText;
	COLORREF m_crBackground;
	COLORREF m_crBackgroundDaebi;
	COLORREF m_crTextDaebi;
	//

	long	m_nStartAlarmKeepingCnt;
	long	m_nEndAlarmKeepingCnt;
	BOOL	m_bUseAlarmSound;
	BOOL	m_bUseAlarmBkClr;
	BOOL	m_bPlayedAlarmSound;
	CString m_strAlarmSound;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXListCtrlJongData)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXListCtrlJongData();
	void GetDispInfo(LVITEM* pItem);
	int m_nIndex;
	int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen);
	BOOL m_bAllCheck;
	//공백 줄 돌려보기에서 건너뛰기
	int SetBlankPrev(int nCur, int nSelCnt);	
	int SetBlankNext(int nCur, int nSelCnt);
	//체크된 종목 카운트
	UINT GetCheckedCount();
	CString GetCheckedCodeList();
	void SetHeaderColor(COLORREF clrBkgrnd, COLORREF clrOutLine);
	// Generated message map functions
protected:
	//{{AFX_MSG(CXListCtrlJongData)
	afx_msg BOOL OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XLISTCTRLJONGDATA_H__CF36F974_F7B6_4CF4_B2BB_7F4C20621969__INCLUDED_)
