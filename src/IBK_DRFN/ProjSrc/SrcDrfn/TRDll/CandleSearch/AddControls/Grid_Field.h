// Grid_MatchValDest.h: interface for the CGrid_Field class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRID_FIELD_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
#define AFX_GRID_FIELD_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import "msxml.dll"
using namespace MSXML;
#include <afxtempl.h>   //<= CList

#include "../../../DrCommon/UGridCtrl/ugctrl.h"
#include "../../../DrCommon/UGridCtrl/_Include/UGCTbutn.h"
#include "../../../DrCommon/UGridCtrl/_Include/ugctelps.h"
#include "../../../DrCommon/UGridCtrl/_Include/ugctsarw.h"

class CGrid_Field : public CUGCtrl
{
public:
	CGrid_Field(CWnd* pwndParent, CString strType, COLORREF	clrHeader);
	virtual ~CGrid_Field();
	
	int m_nSelectRow, m_nSelectCol;
	
private :
	CWnd	*m_pwndParent;
	CString m_strType;
	int m_nColCnt;
	
	CStringArray m_saOpt;
	
	CFont m_fontDefault, m_fontBold;
	CPen					m_penGridLine;
	BOOL          m_bCallAdjustOneTime;
	CUGButtonType	m_button;
	COLORREF	m_clrHeader;
	
public:	 // Member Functions
	void ClearAll();
	void HeaderSetup(int &nColCnt);
	void SetSelRow(int nRow);
	
	void SetCellData(int nRow, int nCol, CString strData);
	void SetData_MatchValSource(int nRow, CString strElement, int nFieldIndex, 
		HeadInfoType	stHeadInfoType, BOOL bInvalidate = FALSE);
	void SetData_MatchValSource(int nRow, BOOL bUser);
	BOOL GetData_MatchValSource(int nRow, MatchInfo	&stMatchInfo);
	
	void SetData_MatchValDest(int nRow, MatchInfo	stMatchInfo, BOOL bInvalidate = FALSE);
	void GetData_MatchValDest(int nRow, MatchInfo	&stMatchInfo);

	void OnHeaderSetup();
	
//	void SetData_QuickMakeSource(int nRow, XMLTreeInfoFile stXMLTreeInfoFile, BOOL bInvalidate = FALSE);
//	void GetData_QuickMakeSource(int nRow, XMLTreeInfoFile &stXMLTreeInfoFile);
	
//	void SetData_QuickMakeDest(int nRow, QuickList stQuickList, BOOL bInvalidate = FALSE);
//	void GetData_QuickMakeDest(int nRow, QuickList &stQuickList);
	
	
//	void SetData_Folder(int nRow, InfoFolder stInfoFolder, BOOL bInvalidate = FALSE);
//	BOOL GetData_Folder(int nRow, InfoFolder &stInfoFolder);
	
//	void SetData_File(int nRow, XMLTreeInfoFile	stXMLTreeInfoFile, BOOL bInvalidate = FALSE);
//	void GetData_File(int nRow, XMLTreeInfoFile	&stXMLTreeInfoFile);
	
	
	CString GetCellData(int nRow, int nCol);
	
	
	
	//# Check / Name / Id / Dest Row
	void SetData_FieldEditSource(int nRow, int nCol, CStringArray *psaData,
		BOOL bInvalidate = FALSE);
	//# Check / Name / Id / Dest Row
	void GetData_FieldEditSource(int nRow, int nCol, CStringArray &saData);
	
	
	//# Name / Id / Index
	void SetData_FieldEditDest(int nRow, CStringArray *psaData,
		BOOL bInvalidate = FALSE);
	//# Name / Id / Index
	void GetData_FieldEditDest(int nRow, CStringArray &saData);
	
	inline void GetEvenOddBkColor(int nRow, COLORREF &clrBk);
	void SetColor_EvenOdd();
	
	
public: // Overrides
	virtual void	OnSetup();
	
	virtual void	OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnRowChange(long oldrow, long newrow);
	virtual void	OnCellChange(long oldCol, long oldRow, long newCol, long newRow);
	
	virtual int		OnCanSizeTopHdg();
	virtual void	OnSysColorChange();
	
	virtual void	OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed);
	virtual int		OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	
	virtual void  OnAdjustComponentSizes(RECT *grid, RECT *topHdg,
		RECT *sideHdg, RECT *cnrBtn, 
		RECT *vScroll, RECT *hScroll, 
		RECT *tabs);
	
	
		/*
		virtual void	OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed);
		
		  virtual void	OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
		  
			
			  
				virtual int		OnHint(int col, long row, int section, CString *string);
	*/
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	
	//{{AFX_MSG(ShowGrid)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_GRID_FIELD_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
