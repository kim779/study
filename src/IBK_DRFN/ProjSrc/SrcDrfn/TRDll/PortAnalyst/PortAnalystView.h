#if !defined(AFX_PORTANALYSTVIEW_H__7EA833F0_671A_4CE7_8992_7BE2CF997935__INCLUDED_)
#define AFX_PORTANALYSTVIEW_H__7EA833F0_671A_4CE7_8992_7BE2CF997935__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrTest002View.h : header file
//

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "resource.h"
// #include "../../itgen/Common/ScrFormView.h"
// #include "../../itgen/CtrlWrap/WrpForm.h"
// #include "../../itgen/CtrlWrap/WrpTranDM.h"
// #include "../../itgen/CtrlWrap/WrpRealDM.h"
// #include "../../itgen/CtrlWrap/WrpButton.h"
// #include "../../itgen/CtrlWrap/WrpCheck.h"
// #include "../../itgen/CtrlWrap/WrpCombo.h"
// #include "../../itgen/CtrlWrap/WrpRadio.h"
// #include "../../itgen/CtrlWrap/WrpCalendar.h"
// #include "../../ITGen/CommonSrc/LinkDataManager.h"

/////////////////////////////////////////////////////////////////////////////
// CPortAnalystView form view
class CAnalystMainFrmWnd;

class CPortAnalystView : public CScrFormView
{
protected:
	CPortAnalystView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPortAnalystView)
		
		// Form Data
public:
	//{{AFX_DATA(CPortAnalystView)
	enum { IDD = IDD_STD_DIALOG };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	// Attributes
public:
	COLORREF			m_crBackColor;			// 화면 백그라운드
	CAnalystMainFrmWnd* m_pwndMainFrm;
	CMapStringToOb		m_MapControlName;		// 화면에 생성된 Control들을 관리하는 Map	

// 	CLinkDataManager	m_LinkDataManager;
// 	CWrpCalendar		m_ctrlCalendar;

	// Operations
public:
	void OnCreate_MainWnd();
	void SetInit_MainWnd();	
	void OnChangeColorTbl(int nTableIndex);

	void    OnPlatFormBtn(ST_MOVEITEM* pItem, int nFlag);
	void    OnPlatFormCheckBtn(ST_MOVEITEM* pItem, int nFlag);
	void    OnPlatFormCalendar(ST_MOVEITEM* pItem, int nFlag);
	void    OnPlatFormComboBtn(ST_MOVEITEM* pItem, int nFlag);
	void	DoPlatFormBtnMove(LPCSTR szCtrlName, CWnd* pWnd);  
	void	DoPlatFormCheckBtnMove(LPCSTR szCtrlName, CWnd* pWnd);  
	void	DoPlatFormCalendarMove(LPCSTR szCtrlName, CWnd* pWnd);
	void	DoPlatFormComboBtnMove(LPCSTR szCtrlName, CWnd* pWnd);

	void	PlatForm_BtnEvent(ST_PF_CONTROL_EVENT* pfEvent);
	void    PlatForm_CheckBtnEvent(ST_PF_CONTROL_EVENT* pfEvent);
	void    PlatForm_ComboBtnEvent(ST_PF_CONTROL_EVENT* pfEvent);
	void    PlatForm_CalendarEvent(ST_PF_CONTROL_EVENT* pfEvent);
	void    SetOpenScreen(CString strScreen, CString strCode);
	void	OpenScreenData(CString strScreen, CString strCode);

	void	OnDBClick(LPCSTR strCode);	//DBClick으로 화면열기.

	LRESULT OnRmsgMoveCtrl(WPARAM wParam, LPARAM lParam);
	void	DoControlMove(LPCSTR szCtrlName, CWnd* pWnd);  
	// Overrides
	virtual		void OnInitialControl();
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortAnalystView)
public:
	virtual void OnInitialUpdate();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	virtual ~CPortAnalystView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	// 폼화면의 컨트롤과 연결된 변수
	//{{AFX_CONTROL(CPortAnalystView)
	CWrpButton		m_ctrlTotalBtn;
	CWrpButton		m_ctrlKospiBtn;
	CWrpButton		m_ctrlKosdaqBtn;
	CWrpButton		m_ctrlSearchBtn;
	CWrpCombo		m_ctrlComboTime;
	CWrpButton		m_ctrlSendBtn;
	//}}AFX_CONTROL
	
	// 폼화면의 이벤트 함수
	//{{AFX_EVENTSINK(CPortAnalystView)
	void	OnFormBeforeInit();
	void	OnFormAfterInit();
	void	OnReceiveTranData(LPCTSTR szTranID);
	void	OnReceiveTranCompleteData(LPCTSTR szTranID);
	void	OnReceiveRealData(LPCTSTR szTranID);
	void	OnReceiveRealCompleteData(LPCTSTR szTranID);
	void	OnClickbtnTotal();
	void	OnClickbtnKospi();
	void	OnClickbtnKosdaq();
	void	OnClickbtnSearch();
	void	OnClickbtnSend();
	void    OnSelChangebtnComboTime(int nIndex);
	void    OnSelChangebtnCalendar(LPCTSTR szData);
	//}}AFX_EVENTSINK
	
	// Generated message map functions
	//{{AFX_MSG(CPortAnalystView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnPlatformControlEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_EVENTSINK_MAP();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRTEST002VIEW_H__7EA833F0_671A_4CE7_8992_7BE2CF997935__INCLUDED_)
