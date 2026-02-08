#if !defined(AFX_EXDIALOG_H__8DE11A41_6CE6_4BD3_AAB9_C93CF01D113B__INCLUDED_)
#define AFX_EXDIALOG_H__8DE11A41_6CE6_4BD3_AAB9_C93CF01D113B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExDialog.h : header file
//
#include "ExResize.h"

#include "IPCTRMng.h"
#include "IGateMng.h"

#include "IAuTrCommMng.h"
#include "ISkinColorMng.h"
#include "IAUGDIMng.h"
#include "IAUDrdsMng.h"
#include "IMasterDataMng.h"
#include "PcTrMngHelper.h"
/////////////////////////////////////////////////////////////////////////////
// CExDialog dialog
class CExDialog : public CDialog, public CExResize
{
	DECLARE_DYNCREATE(CExDialog);

// Construction
public:
	CExDialog();   // standard constructor
	CExDialog(const UINT nID, CWnd* pParent = 0, const UINT nFlags = 0);   // standard constructor

	void	SetBaseInterface(UINT nFlag = EXDLG_INTERFACE_ALL);
	POSITION	SetChangeSkinNotifyFunction(FpChangeSkinNotify pChangeSkinNotify);
	void	RemoveChangeSkinNotifyFunction(POSITION posFunction);

	virtual	HRESULT ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen) {return S_FALSE;}
	virtual	HRESULT ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData) {return S_FALSE;}
	virtual HRESULT StatusChange(int nState) {return S_FALSE;}
	virtual HRESULT ReceiveMessage(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage) {return S_FALSE;}

	virtual void ColorTableChanged() {}

	// 모든 데이터 정보 요청은 m_pIPCTRManager
	AFX_INLINE IPCTRManager* GetPCTRManager();
	AFX_INLINE IGateManager* GetGateManager();

	AFX_INLINE IAUTrCommManager* GetTrCommManager();
	// 서버와의 TR 통신은 m_pITrComm
	AFX_INLINE ITrComm* GetTrComm();

	// 스킨 테이블, 변경은 m_pISkinColorManager
	AFX_INLINE ISkinColorManager*	GetSkinColorManager();

	AFX_INLINE IAUDrdsManager*	GetDrdsManager();
	// 실시간 처리 DRDS는 m_pIDrdsLib
	AFX_INLINE IDrdsLib* GetDrdsLib();

	AFX_INLINE IMasterDataManager* GetMasterDataManager();

	AFX_INLINE IAUGDIManager* GetGDIManager();
	
	// 화면 배경색을 지정
	// 따로 배경색을 지정하지 않으면 컬러테이블의 배경색이  ISkinColorManager 인터페이스가 초기화 되어있야 자동 지정됨
	// ISkinColorManager 인터페이스가 초기화 되어있지 않고도 별도로 지정가능함
	AFX_INLINE void SetBackgroundColor(COLORREF clr);

// Implementation
protected:
	IPCTRManager*			m_pIPCTRManager;
	IGateManager*			m_pIGateManager;

	IAUTrCommManager*		m_pITrCommManager;
	ISkinColorManager*		m_pISkinColorManager;
	IAUDrdsManager*			m_pIAUDrdsManager;
	IMasterDataManager*		m_pIMasterDataManager;
	IAUGDIManager*			m_pGDIManager;

	ITrComm*				m_pITrComm;
	IDrdsLib*				m_pIDrdsLib;
	long					m_lSkinSiteKey;

	CPCTrMngHelper			* m_pPcTrHelper;


	CList<FpChangeSkinNotify, FpChangeSkinNotify> m_listChangeSkinNotify;

	class CMyITrCommSite : public ITrCommSite
	{
	public:
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
		{
			METHOD_PROLOGUE(CExDialog, MyITrCommSite)
			return pThis->ReceiveData(dwKey, szTR, szMsgCode, szMessage, aTRData, dwTRDateLen);
		}

		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData)
		{
			METHOD_PROLOGUE(CExDialog, MyITrCommSite)
				return pThis->ReceiveRealData(wFID, szMainKey, pRealData);
		}

		STDMETHOD(StatusChange) (int nState)
		{
			MYMETHOD_PROLOGUE(CExDialog, MyITrCommSite)
			return pThis->StatusChange(nState);
		}

		STDMETHOD(ReceiveMessage) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage)
		{
			MYMETHOD_PROLOGUE(CExDialog, MyITrCommSite)
			return pThis->ReceiveMessage(dwKey, szTR, szMsgCode, szMessage);
		}
	} m_xMyITrCommSite;
	friend class CMyITrCommSite;

	class CMyISkinColorSite : public ISkinColorSite
	{
	public:
		STDMETHOD_(void, ColorTableChanged)()
		{
			MYMETHOD_PROLOGUE(CExDialog, MyISkinColorSite)
				
			pThis->NotifyToCtrlColorTableChanged();
			pThis->ColorTableChanged();
		}
	} m_xMyISkinColorSite;
	friend class CMyISkinColorSite;

	UINT m_nDialogID; // Contains the ID number of a dialog box template resource

	// Generated message map functions
	//{{AFX_MSG(CExDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void	NotifyToCtrlColorTableChanged();

private:		// Test
	HBRUSH	m_hBrBackground;
	COLORREF m_clrBackground;
};

AFX_INLINE IPCTRManager* CExDialog::GetPCTRManager() {return m_pIPCTRManager;}
AFX_INLINE IGateManager*  CExDialog::GetGateManager() {return m_pIGateManager;}
AFX_INLINE IAUTrCommManager*  CExDialog::GetTrCommManager() {return m_pITrCommManager;}
AFX_INLINE ITrComm*  CExDialog::GetTrComm() {return m_pITrComm;}
AFX_INLINE ISkinColorManager*	 CExDialog::GetSkinColorManager() {return m_pISkinColorManager;}
AFX_INLINE IAUDrdsManager*	 CExDialog::GetDrdsManager() {return m_pIAUDrdsManager;}
AFX_INLINE IDrdsLib*  CExDialog::GetDrdsLib() {return m_pIDrdsLib;}
AFX_INLINE IMasterDataManager* CExDialog::GetMasterDataManager() {return m_pIMasterDataManager;}
AFX_INLINE IAUGDIManager* CExDialog::GetGDIManager() {return m_pGDIManager;}
AFX_INLINE void CExDialog::SetBackgroundColor(COLORREF clr)
{
	m_clrBackground = clr;
	
	// Test
	if(m_hBrBackground)
		::DeleteObject(m_hBrBackground);

	m_hBrBackground = ::CreateSolidBrush(clr);
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXDIALOG_H__8DE11A41_6CE6_4BD3_AAB9_C93CF01D113B__INCLUDED_)
