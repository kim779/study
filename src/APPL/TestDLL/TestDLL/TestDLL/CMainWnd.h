#pragma once


// CMainWnd
#include "CWC_FileSync.h"
class CMainWnd : public CWnd
{
	DECLARE_DYNAMIC(CMainWnd)

public:
	CMainWnd(CWnd* pwndParent);
	virtual ~CMainWnd();

	HINSTANCE	m_hiSymbol{};
	HINSTANCE	m_hiSha256{};
	class CContainerWnd* m_pMarketpicker{};

	CRect m_rectAccName{};

	CString m_strLog{};
	CString m_strHome{};
	CString m_strAccName{};
	CString m_strPswd{};

	CString m_slog{};

	CWnd* m_pwndParent{};
	CWnd* m_pwndSymbol{};
	CEdit* m_pPass{};
	CButton* m_pBtnSend{}, 
				 *m_pBtnSymbol{}, 
				 * m_pBtnSonaq388{}, 
		         * m_pBtnSonaq428{}, 
				 * m_pBtnSonaq429{}, 
		         * m_pBtnpoop0200{}, 
		         * m_pBtnBal{}, 
		         * m_pBtnWndRegister{}, 
		         * m_pBtnBroadCasting{},
				* m_pBtnWndUnRegister{},  
				* m_pBtnTopDlg{},
		        *m_pBtnWndCnt{};
	class CAccountCtrl* m_pAccount{};

	COLORREF GetAxColor(UINT nIndex);

	CString GetEncPassword(CString sPswd);
	CString LedgerTR(CString sGubn, CString sMaxRow = "999", CString sSvcn = "", CString sPswd = "",  int rcnt =0);	
	LRESULT CMainWnd::SendTR(CString strCode, CString strData, int iKey, int iStat = NULL);

	void InitCtrl();

	//filesync
	std::unique_ptr < CWC_FileSync> m_pWFileSync{};
	
protected:
	afx_msg void OnButtonSend();
	afx_msg void OnButtonSymbol();
	afx_msg void OnButtonSonaq388();
	afx_msg void OnButtonSonaq428();
	afx_msg void OnButtonSonaq429();  
	afx_msg void OnButtonpoop0200(); 
	afx_msg void OnButtonTrdList();
	afx_msg void OnButtonRegWnd();
	afx_msg void OnButtonBroadcastWnd();
	afx_msg void OnButtonUnRegWnd();
	afx_msg void OnButtonGetWndCnt();  
	afx_msg void OnButtonTopDlg();

	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAccMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMaketPickerMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};



