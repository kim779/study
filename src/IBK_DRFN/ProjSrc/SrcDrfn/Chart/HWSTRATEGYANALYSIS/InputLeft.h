#if !defined(AFX_INPUTLEFT_H__FADB0B3A_E1A4_40C1_A4C3_BAAE77553DE5__INCLUDED_)
#define AFX_INPUTLEFT_H__FADB0B3A_E1A4_40C1_A4C3_BAAE77553DE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputLeft.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputLeft dialog
#include "resource.h"


//#include "../_#HanWha/_include/COMMON_BASE/TrComm/TrComm.h"

#include "Control/InputControl.h"
#include "Control/XListCtrl.h"
#include "Control/xSkinButton.h"
#include "InfoRight.h"

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExDrawButton.h"
#include "../../inc/ExStatic.h"
#include "../../inc/ExEdit.h"
#include "../../inc/ExComboBox.h"

// CDlgSetEnv dialog
#define		NID_CODEINPUT		4578
#define		NID_ACCTINPUT		4579
#define		NID_FUTOPTINPUT		4582	//05.06.22 선물옵션  종목 코드 Ctrl Resource ID
#define     MAXDATANUM          36 
#define MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;

//class CDlgAnalysis;
class CInputLeft : public CDialog//, public CSharedBase 
{
// Construction
public:
	CInputLeft(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputLeft)
	enum { IDD = IDD_DLG_INPUTLEFT };
	CExComboBox		m_CtlComNum;
	CDateTimeCtrl	m_CtlDateTime;
	CExComboBox		m_CtlComboRep;
	CExComboBox		m_CtlComboDiv;
	CExDrawButton	m_ctlApply;
	CExComboBox		m_CtlStategy;
	CXListCtrl		m_CtlList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputLeft)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputLeft)
	//afx_msg void OnButton1();
	afx_msg void OnRadioStock();
	afx_msg void OnRadioFuture();
	afx_msg void OnBtnApply();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboDiv();
	afx_msg void OnButton2();
	afx_msg void OnClickListDay(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	LRESULT OnSTDataManagerMessagge( WPARAM wParam, LPARAM lParam);
protected:
	// * 소켓 통신 관련 Nested Class
	class CMyCallback : public ICallbackStatus	
	{
		STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * lplpObj) {return S_OK;}
		STDMETHOD_(ULONG,AddRef) (THIS)  {return S_OK;}
		STDMETHOD_(ULONG,Release) (THIS) {return S_OK;}
		
		STDMETHOD(ReceiveData) (HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
		{
			MYMETHOD_PROLOGUE(CInputLeft, MyCallback)
			return pThis->ReceiveData(hTrComm, szTR, szMsgCode, szMessage, aTRData, dwTRDateLen);
		}
		STDMETHOD(StatusChange) (int nState)
		{
			MYMETHOD_PROLOGUE(CInputLeft, MyCallback)
			return S_OK;			
		}
	} m_xMyCallback;
	friend class CMyCallback;
	HRESULT ReceiveData(HWND hTrComm, 
						LPCSTR szTR, 
						LPCSTR szMsgCode,
						LPCSTR szMessage, 
						LPVOID aTRData, 
						DWORD dwTRDateLen);	
	HRESULT RecieveError(int nError, LPCSTR szMessage);
//	CTRComm		m_TRComm;			// 소켓 처리 모듈
	CList<void*,void*>m_listRequested;
	BOOL Send2Server(LPCTSTR lpTrCode,LPCTSTR lpData,int nSize);
//////////////////////////////////////////////////////////////////////
protected://AP관련 
	CIStrategyItem *m_pStrategyItem;
	CStrategyLoader m_strategyLoader;
	CISDataItem *m_pDataItem;

	// For Shared Info  gm0604 2005/11/15
	BOOL UnAdviseSharedInfo(LPCTSTR lpShareName=NULL);
	BOOL AdviseSharedInfo(LPCTSTR lpShareName, int nSharedType, int nSharedArea);
	BOOL SetSharedData2(HWND hArea,int nArea, LPCTSTR lpShareName,BYTE* pData, int nLen, int nStock, BOOL bOnlySetData);
	long ChangeSharedData(CString sShareName, BYTE* pData, int nLen, int nStock=0);
	BOOL m_bInitShareBase;
	CStringArray m_strArrayAdviceInfo;

public:


	HWND m_hMainFrame;
	HWND m_hSocket;					// 소켓핸들
//	CDrMLib_CDBMgr m_IDrMLib_CDBMgr;
//	CDrMLib_CBaseInfo	m_BaseInfo;
	COLORREF m_crBk;
	CBrush*	m_pBrBkColor;
	CString m_strUserPath;
	CInfoRight *m_pInfoRight;

	COLORREF m_bgColor;
	CBrush	m_brBkColor;
	CStringArray m_stArrayPath;
	CString m_strRootPath;
	CString m_strStName; //전략명 
	CString m_strDataPath;
	BOOL m_bIsView;
	CString m_JMName;
	//KHD ADD 
	long m_pChartInterface;
	//
	CArray<ST_DATAINDEX,ST_DATAINDEX> m_stDataIndex; //Index: 0 전체 , 1~10: 분할자

	void InitInputLeft();
	void InitStrategy();
	void InitColumn();
	
	void SethMainFrame(HWND hMainFrame) { m_hMainFrame = hMainFrame;}
//	void SetpDBMgr(CDrMLib_CDBMgr pDBMgr) { m_IDrMLib_CDBMgr = pDBMgr;}
	void SetRightInfo(CInfoRight *pdlg);
	void Result();
	void SetSocket(HWND Socket)
	{ 
//		m_hSocket = Socket;
//		//CTRComm Init
//		m_TRComm.Create(this);
//		m_TRComm.ChangeCallback(&m_xMyCallback);
//		m_TRComm.SetSocket(m_hSocket);	
	}

	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetListFocus(int nIndex);
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}
	void InsertDateGubun(CString &str);
	void InsertTimeGubun(CString &str);
	void InsertGubun(CString &str, BOOL Gubun);
	BOOL IsInsertGubun(CString str);
	void SetCalculateData(int index);
	void InitCodeInfo(long lType);
	CString GetJMNameAndGubunFromCode(CString csCode);
	CInputLeft* GetInputLeft();
	void AllUpdateDataIndex();
	void SetStrategyItem(CIStrategyItem* pStrategy){ m_pStrategyItem = pStrategy;}
	CIStrategyItem* GetStrategyItem(){return m_pStrategyItem;}
	CString GetStName(){ return m_strStName;}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTLEFT_H__FADB0B3A_E1A4_40C1_A4C3_BAAE77553DE5__INCLUDED_)
