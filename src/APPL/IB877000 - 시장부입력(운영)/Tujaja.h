
#include "AxisExt.h"
#include "afxcoll.h"

#include "grid/GridCtrl.h"
#include "../../control/fx_misc/fxImgButton.h"

#define IDC_INPUT		1000
#define IDC_PARSE		1001
#define IDC_RESULT		1002
#define IDC_SEND		1003
#define IDC_SEQN		1004

#pragma once

class CTujaja : public CWnd, public CAxisExt
{
public:
	CTujaja(CWnd *pParent);
	void SafeDelete( CWnd *pWnd );
	std::shared_ptr<CfxImgButton> CreateImgButton(LPCSTR caption, int nID, CBitmap **bmp);
	void ResponsePIDOTUJA(LPCSTR dat, int len);
	void testSaveFile(CString strName, LPCSTR datB, int size);	//2015.04.13 KSJ 로그파일 저장
	bool isNum(char c);

public:
	CBitmap *m_hBtn2[3], *m_hBtn20[3];
	std::unique_ptr<CRichEditCtrl> m_pInput;
	std::unique_ptr<CComboBox> m_pSeqn;
	std::shared_ptr<CfxImgButton>m_pParse, m_pSend;
	std::unique_ptr<CGridCtrl>m_pResult;

public:
	void LoadHJCode();
	void LoadCategory();
	CMapStringToString m_hnamMap, m_catgMap;

public:
	CRect m_rcDesc, m_rcTotal;
	CString m_strDesc, m_strTotal;
	vector< vector<char> > m_tuja_queue;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTujaja)
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//}}AFX_VIRTUAL

public:
	virtual ~CTujaja();

protected:
	//{{AFX_MSG(CTujaja)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnParse();
	afx_msg void OnSend();
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
};

