#if !defined(AFX_DLGCONFIRM_H__B650CD80_DB43_48EA_B91B_E25C2CE4D6B4__INCLUDED_)
#define AFX_DLGCONFIRM_H__B650CD80_DB43_48EA_B91B_E25C2CE4D6B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfirm.h : header file
//
#include "resource.h"
#include "../Include_ST/STOrderDef.h"
//#include "../../inc/PcTrMngHelper.h"
#include "../../inc/IAuTrCommMng.h"
#include "../../inc/ExStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm dialog

class CDlgConfirm : public CDialog
{
// Construction
public:
	CDlgConfirm(CWnd* pParent = NULL);   // standard constructor
	void ApplyHoga();

	int		m_nResult;		// 주문결과. 0:취소, 주문결과
	float	m_fOrderPrice;

	STORDER_INDATA		*m_pInData;
	IAUTrCommManager*		m_pITrCommManager;
	ITrComm*				m_pITrComm;

	BOOL InitTrComm();
	HRESULT ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);	
	HRESULT ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData);

	void QueryPrice(CString strCode, int nOrderPriceType);

	int Send2Server(LPCTSTR lpData,int nSize,BOOL bWait=FALSE);
	void AddDataKey_Hoga();
	void ConvertHogaData(LPCSTR szTR, LPVOID	aTRData, long dwTRDateLen);
	LPCSTR GetPrice(int nPriceType);
	CBrush m_brBkColor;
	CString m_SellHoga[5],
			m_BuyHoga[5], 
			m_UHoga,
			m_LHoga,
			m_Current;
	
	class CMyITrCommSite : public ITrCommSite
	{
	public:
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
		{
			METHOD_PROLOGUE(CDlgConfirm, MyITrCommSite)
				return pThis->ReceiveData(dwKey, szTR, szMsgCode, szMessage, aTRData, dwTRDateLen);
		}

		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData)
		{
			METHOD_PROLOGUE(CDlgConfirm, MyITrCommSite)
				return pThis->ReceiveRealData(wFID, szMainKey, pRealData);
		}

		STDMETHOD(StatusChange) (int nState)
		{
			MYMETHOD_PROLOGUE(CDlgConfirm, MyITrCommSite)
			return S_OK;
		}
	} m_xMyITrCommSite;
	friend class CMyITrCommSite;

// Dialog Data
	//{{AFX_DATA(CDlgConfirm)
	enum { IDD = IDD_DLGCONFIRM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgConfirm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgConfirm)
	virtual BOOL OnInitDialog();
	LRESULT OnReceiveTR(WPARAM wp, LPARAM lp);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CExStatic m_ES_AccNum2;
	CExStatic m_ST_Acc1;
	CExStatic m_ES_AccName;
	CExStatic m_ES_AccName1;
	CExStatic m_ES_Jm;
	CExStatic m_ES_Jm1;
	CExStatic m_ES_Gb;
	CExStatic m_ES_Gb1;
	CExStatic m_ES_Num;
	CExStatic m_ES_Num1;
	CExStatic m_ES_Price;
	CExStatic m_ES_Price1;
	CExStatic m_ES_CO;
	CExStatic m_ES_Title;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONFIRM_H__B650CD80_DB43_48EA_B91B_E25C2CE4D6B4__INCLUDED_)
