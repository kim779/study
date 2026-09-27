#pragma once


// CSampleWnd

class CSampleWnd : public CWnd
{
	DECLARE_DYNAMIC(CSampleWnd)

public:
	CSampleWnd(CWnd* pwndParent);
	virtual ~CSampleWnd();


	CString m_strHome{}, m_slog{};;

	CWnd* m_pWizard{};
	class CContainerWnd* m_pMarketpicker{};

	// cx_symbol / cx_codectrl : same direct LoadLibrary + axCreate pattern as CMainWnd.cpp::InitCtrl(),
	// no CContainerWnd wrapper in between.
	HINSTANCE m_hiSymbol{};
	CWnd* m_pwndSymbol{};
	HINSTANCE m_hiCodeCtrl{};
	CWnd* m_pwndCodeCtrl{};


	int	m_iMarket{};
	CButton* m_pBtnSetMarket_KRX{}, * m_pBtnSetMarket_NXT{}, * m_pBtnSetMarket_TOT{}, *m_pBtnGSetMarket{};
	CButton* m_pBtnTR_OOP{};
	CButton* m_pBtnSymbol{};	// cx_symbol wiring sample button, mirrors CMainWnd.cpp::OnButtonSymbol()
	CButton* m_pBtnCodeGet{};	// reads cx_codectrl's "Data" COM property (current code)

	LRESULT SendTR(CString strCode, CString strData, int iKey, int iStat = NULL);
	CString GetCodeCtrlData();	// helper: cx_codectrl "Data" property via IDispatch, same pattern as ContainerWnd::GetCtrlProperty
	int GetMarketGubn();	// 1777 field value, read live from m_pMarketpicker->GetCtrlProperty("sMarket")
protected:
	afx_msg void OnButtonKRX();
	afx_msg void OnButtonNXT();
	afx_msg void OnButtonTOT();
	afx_msg void OnButtonGetMarket();
	afx_msg void OnButtonSendOOP();
	afx_msg void OnButtonSymbol();
	afx_msg void OnButtonCodeGet();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


